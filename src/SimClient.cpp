#include "../include/SimClient.h"

SimClient::SimClient(Broker &bkr)
{
    this->broker = &bkr;
    //this->simth=0;
    printf("Broker is at address: %p\n", this->broker);
}

void SimClient::start()      //En pseudo C++
{
    if (!this->simth.joinable())
    {
        simth=move(std::thread(&SimClient::runSim,this));
        cout<<"*Thred for a client created" <<endl;
        cout<< "-----------------------------\n" <<endl;
    }
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
    ///Se registra un Nuevo cliente en el broker y se obtiene la dirección
    BrokerOpsIF *brops = Broker::getInstance()->registerClient(this);

    ///El cliente manda un mensaje de conexión al broker
    brops->sendMsg(ConnectMsg{"root","1234"});  ///CONNNECT

    ///esperar por CONNACK sin errores a traves de rcvMsg en el client        
    std::cout<<"waiting connack..."<<endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));

    if (this->connack)///(this->connack)
    {
        std::cout<<"CONNACK OK - Se publican msj del Topico "<<topic<<endl;        

        for(size_t i=1;i<40;i++)
        {

            // Publicar un mensaje de prueba
            std::cout<<"<< i = "<<i <<endl;
            PublishMsg m = PublishMsg( topic , std::to_string(25+i) , false);

            std::cout<<"<<Enviando Publicacion al Broker"<<endl;
            brops->sendMsg(m); ///PUBLISH
            std::this_thread::sleep_for (std::chrono::milliseconds(2000));
        };
    };
    DisconnectMsg m;
    brops->sendMsg(m);         ///DISCONNECT
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
        case Message::TypeM::PUBLISH:
        {
            PublishMsg pm = *(dynamic_cast <const PublishMsg* const> (&m));
            std::cout<<"<<---- Recibida una publicación subscripta: ";
            // cout<< pm.getTopic()<<" = "<< pm.getValue() << endl;
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


void SimSubscriber::runSim()
{
    ///Se registra un Nuevo cliente en el broker y se obtiene la dirección
    BrokerOpsIF *brops = Broker::getInstance()->registerClient(this);

    brops->sendMsg(ConnectMsg{"root","1234"});  //CONNNECT

    ///esperar por CONNACK sin errores              
    std::cout<<"wait connack..."<<endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));  //wait CONNACK
    if (this->connack)
    {
        std::cout<<"CONNACK OK - Se subscriben al Topico "<<topicSub[0]<<endl;
        brops->sendMsg(SubscribeMsg{this->topicSub[0]});  ///Subscripción
        std::this_thread::sleep_for (std::chrono::seconds(20)); ///Tiempo que se queda esperando Mensajes antes de desconectarse
    };
    brops->sendMsg(DisconnectMsg());         ///DISCONNECT
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
                    connack=true;
                break;
            }
        case Message::TypeM::PUBLISH:
        {
            const PublishMsg* pm = dynamic_cast <const PublishMsg* const> (&m);
            std::cout<<"Recibida una publicacion subscrita" <<endl; /// no me deja acceder a pm como const
            break;
        }
        default:
            break;
    }
};
