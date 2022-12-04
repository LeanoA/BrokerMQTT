#include "../include/SimClient.h"

SimClient::SimClient(Broker &bkr)
{
    this->broker = &bkr;
    printf("Broker is at address: %p\n", this->broker);
}

void SimClient::start() 
{
    if (!this->simth.joinable())
    {
        simth=move(std::thread(&SimClient::runSim,this));
        cout<<"SIM CLIENT --> Thred for a client created \n " <<endl;
    }
};

void SimClient::connect()
{
    brops = this->broker->registerClient(this);
    cout<<"\tSIM CLIENT --> Client registered in the broker \n " <<endl;
    ///El cliente manda un mensaje de conexión al broker
    brops->sendMsg(ConnectMsg{"root","1234"});  ///CONNNECT

    ///esperar por CONNACK sin errores a traves de rcvMsg en el client        
    std::cout<<"\tSIM CLIENT --> waiting connack..."<<endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));

    if (this->connack)///(this->connack)
    {
        std::cout<<"\tSIM CLIENT --> CONNACK OK "<<endl;        
    }

};

void SimClient::disconnect()
{
    brops->sendMsg( DisconnectMsg() );         ///DISCONNECT
    std::cout<<"\tSIM CLIENT --> Disconnect "<<endl;        
};

void SimClient::exit()
{
    /*
    if (!this->simth.joinable())
        return;
    */   
    simth.join(); 
    cout<<"Sim Thread terminado\n"<<endl;
}


//------------------------------------------------
//------------------SimPublisher------------------

void SimPublisher::runSim()
{
    this->connect();
    
    if (this->connack)///(this->connack)
    { 
        for(size_t i=1;i<20;i++)
        {
            // Publicar un mensaje de prueba
            std::cout<<"<< i = "<<i <<endl;
            PublishMsg m = PublishMsg( topic , std::to_string(25+i) , false);

            std::cout<<"\t\tSIM CLIENT --> Enviando Publicacion al Broker topico: "<<topic<<endl;
            brops->sendMsg(m); ///PUBLISH
            std::this_thread::sleep_for (std::chrono::milliseconds(4000));
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


SimSubscriber::SimSubscriber(Broker &bkr,vector <TopicName> &TN) :
    SimClient(bkr), topicSub{TN}{};

void SimSubscriber::subscribe()
{
    for (auto &t: topicSub)
    {
        SubscribeMsg m = SubscribeMsg(t);
        brops->sendMsg(m); ///SUBSCRIBE
        std::cout<<"\tSIM CLIENT --> Enviando Subscribe al Broker"<<endl;
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
};

void SimSubscriber::runSim()
{
    this->connect();
    if (this->connack)
    {
        this->subscribe();
        std::this_thread::sleep_for (std::chrono::seconds(20)); ///Tiempo que se queda esperando Mensajes antes de desconectarse
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
                    std::cout<<"\t\tSIM SUBSCRIBER --> CONNACK OK"<<endl;
                    }
                break;
            }
        case Message::TypeM::PUBLISH:
        {
            const PublishMsg* pm = dynamic_cast <const PublishMsg* const> (&m);
            std::cout<<"\t\tSIM SUBSCRIBER: Recibida una publicacion subscrita - Topico: "<< pm->getTopic() << "Value: "<< pm->getValue() <<endl; /// no me deja acceder a pm como const      
            break;
        }
        default:
            break;
    }
};
