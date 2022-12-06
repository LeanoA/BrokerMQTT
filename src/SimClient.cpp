/**
 * @file SimClient.cpp
 * @author Alexander Leano
 * @brief SimClient implementation
 * @version 0.1
 * @date 2022-12-05
*/

#include "../include/SimClient.h"

SimClient::SimClient(Broker &bkr)
{
    this->broker = &bkr;
    cout<<"SIM CLIENT --> Created with Broker address:"<< this->broker <<endl;
}

void SimClient::start() 
{
    if (!this->simth.joinable())
    {
        simth=move(std::thread(&SimClient::runSim,this));
        cout<<"SIM CLIENT --> Thread for a client created" <<endl;
    }
};

void SimClient::connect()
{
    brops = this->broker->registerClient(this);
    cout<<"SIM CLIENT --> Trying to connect to broker" <<endl;
    ///El cliente manda un mensaje de conexión al broker
    brops->sendMsg(ConnectMsg{"root","1234"});  ///CONNNECT

    ///esperar por CONNACK sin errores a traves de rcvMsg en el client        
    std::this_thread::sleep_for (std::chrono::seconds(1));

    if (this->connack)///(this->connack)
    {
        std::cout<<"SIM CLIENT --> CONNACK OK "<<endl;        
    }

};

void SimClient::disconnect()
{
    std::cout<<"SIM CLIENT --> Disconnecting client "<< this <<endl;  
    brops->sendMsg( DisconnectMsg() );         ///DISCONNECT
      
};

void SimClient::exit()
{
    /*
    if (!this->simth.joinable())
        return;
    */   
    simth.join(); 
    cout<<"SIM CLIENT --> Sim Thread terminado\n"<<endl;
}


//------------------------------------------------
//------------------SIM CLIENT B------------------

void SimPublisher::runSim()
{

    this->connect();
    if (this->connack)///(this->connack)
    { 
        for(size_t i=0;i<10;i++)
        {
            // random number between 25 and 38
            size_t random = 25 + (rand() % (38 - 25 + 1));
            // Value to publish in string format
            std::string sValue =  std::to_string(random) + "\u2103";

            PublishMsg m = PublishMsg( topic , sValue , true);
            std::cout<<"\t\tCLIENT B --> Publish: "<<topic<<" "<<m.getValue() <<endl;
            brops->sendMsg(m); ///PUBLISH
            std::this_thread::sleep_for (std::chrono::seconds(3));
        };
    };
    this->disconnect();
}

/// @brief Recibe message from Client simulated in the Broker
/// @param m Message::TypeM::CONNACK and Message::TypeM::PUBLISH
void SimPublisher::recvMsg(const Message &m)
{
    switch(m.getType())
    {
        case Message::TypeM::CONNACK:
            {
                const ConnAckMsg* mcm = dynamic_cast <const ConnAckMsg* const> (&m);
                if(mcm->getStatus() == ConnAckMsg::Status::CONNECTION_OK)
                    connack=true;
                break;
            }
        default:
            break;
    }
};


//------------------------------------------------
//------------------SimSubscriber-----------------
//------------------------------------------------


SimSubscriber::SimSubscriber(Broker &bkr,vector <TopicName> &TN, TopicName topNam) :
    SimClient(bkr), topicSub{TN}, topic{topNam}{};

void SimSubscriber::subscribe()
{
    for (auto &t: topicSub)
    {
        SubscribeMsg m = SubscribeMsg(t);
        brops->sendMsg(m); ///SUBSCRIBE
        std::cout<<"\tCLIENT A  --> Enviando Subscribe al Broker"<<endl;
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
};

void SimSubscriber::runSim()
{
    this->connect();
    if (this->connack)
    {
        this->subscribe();

        for(size_t i=0;i<4;i++)
        {
            // value 25 grad celsius
            PublishMsg m = PublishMsg( topic , "25\u2103" , false);
            std::cout<<"\tCLIENT A --> Publish: "<<topic<<" "<<m.getValue() <<endl;
            brops->sendMsg(m); ///PUBLISH
            std::this_thread::sleep_for (std::chrono::seconds(5));
        };

        std::this_thread::sleep_for (std::chrono::seconds(1)); ///Tiempo que se queda esperando Mensajes antes de desconectarse
        this->disconnect();
    };   
};


/// @brief Mensajes recibidos por el Subscriptor enviados por el Broker
/// @param m Message::TypeM::CONNACK
void SimSubscriber::recvMsg(const Message &m)
{
    switch(m.getType())
    {
        case Message::TypeM::CONNACK:
            {
                const ConnAckMsg* mcm = dynamic_cast <const ConnAckMsg* const> (&m);
                if(mcm->getStatus() == ConnAckMsg::Status::CONNECTION_OK)
                    {
                    connack=true;
                    std::cout<<"\tCLIENT A --> CONNACK OK"<<endl;
                    }
                break;
            }
        case Message::TypeM::PUBLISH:
        {
            const PublishMsg* pm = dynamic_cast <const PublishMsg* const> (&m);
            std::cout<<"\tCLIENT A --> Publish receive: "<< pm->getTopic() << " "<< pm->getValue() <<endl;      
            break;
        }
        default:
            break;
    }
};
