#include "../include/Broker.h"
#include <stddef.h>  //Define NULL
#include <algorithm> // std::for_each


/// Garantiza un solo Broker
Broker *Broker::m_pInstance = NULL;

// typedef std::multiset<Subscription *,CompareTopic>::iterator it_ct; // aliasing the iterator type used

/// @brief Esta función es llamada para crear una unica instancia de la clase.
/// No se permite llamar al constructor públicamente. El constructor es
/// privado y solo es llamado por esta función de instancia.
/// @return
Broker *Broker::getInstance()
{
    if (!m_pInstance)
    { 
        m_pInstance = new Broker;
        std::cout << "\t\t\tBROKER --> Instance created "<< m_pInstance << std::endl;
    };
    return m_pInstance; /// al momento de crear, se almacena
};

void Broker::deleteInstance()
{
    std::cout << "\t\t\tBROKER --> Delete Instance" << endl;
    delete m_pInstance;
    m_pInstance = NULL;
};

/// @brief Create a new client with SimPublish/SimSub who derivate from CIientOpsfIF.
BrokerOpsIF *Broker::registerClient(ClientOpsIF *cifops)
{
    std::cout << "\t\t\tBROKER --> Register Client: " << cifops << endl;
    Client *cl = new Client(cifops);
    std::unique_lock<std::mutex> lk{clmtx};
    this->clients.push_back(cl); /// New client is added to the list of clients
    lk.unlock();
    return cl;
};

void Broker::registerNewSubs(Subscription *s)
{
    std::cout << "\t\t\tBROKER --> Register new subscription" << endl;
    std::unique_lock<std::mutex> lk{sbsmtx};
    this->subs_cache.insert(s);
    lk.unlock();
    this->ifRT(s);
}

void Broker::forEachSubs(PublishMsg *m, Client *cl)
{
    /// Si hay subscripciones envia el valor publicado al Cliente
    Subscription sub = Subscription{m->getTopic(), 0};
    
    std::unique_lock<std::mutex> lk(sbsmtx);
    auto range = subs_cache.equal_range(&sub);

    if (range.first != subs_cache.end())
    {
        size_t total = std::distance(range.first, range.second);
        std::cout << "\t\t\tBROKER --> Number of Subs for this topic: "<< total << endl;
        size_t count = 0;
        for (auto it = range.first; it!= range.second; it++)
        {
            count++;
            std::cout << "\t\t\tBROKER --> Iteration "<< count <<" on Subscribers" << endl;
            Client *client = (*it)->owner;
            if(client != cl)
                client->sendBrokerCl2Client(*m);
            else
                std::cout << "\t\t\tBROKER --> Not send to the same client" << endl;

        }
    }
    else
        std::cout << "\t\t\tBROKER --> No Subscribers for this topic" << endl;


    lk.unlock();

    // if the msg has to be retained, store it
    if (m->getRetain())
    {
        RetainedTopic *RT = new RetainedTopic{m->getTopic(), m->getValue(), cl};
        std::unique_lock<std::mutex> lk(this->rtmtx);
        // Find RT in the cache to erase it and insert the new one
        auto ret = this->rt_cache.equal_range(RT);
        if (ret.first != this->rt_cache.end())
            this->rt_cache.erase(ret.first); // erase the old one
        this->rt_cache.insert(RT);
        lk.unlock();
    }
}

/// @brief Manda Mensaje a todos los subscriptores nuevos si hay retenidos
/// @param sb *Subscripcion
void Broker::ifRT(Subscription *sb)
{
    Client *cl;
    RetainedTopic RT = RetainedTopic{sb->topic, "0", 0};

    std::unique_lock<std::mutex> lk(this->rtmtx);
    auto ret = this->rt_cache.equal_range( &RT );
    lk.unlock();

    for (auto it = ret.first; it != ret.second; ++it)
    {
        std::cout << "\t\t\tBROKER --> Sending RetainedTopic " << endl; 

        PublishMsg m = PublishMsg((*it)->topic, (*it)->value);
        
        Client *client = sb->owner;
        if(client != cl)
            client->sendBrokerCl2Client(m);
        else
            std::cout << "\t\t\tBROKER --> Not send to the same client" << endl;
    }


}

void Broker::delSub(Subscription *s)
{
    std::cout << "\t\t\tBROKER --> Delete Subscriptions topic: " << s->topic << " Client Owner: " << s->owner << endl;
    std::unique_lock<std::mutex> lk{sbsmtx};
    auto it = this->subs_cache.equal_range(s);
    this->subs_cache.erase(it.first, it.second);
    lk.unlock();
}

void Broker::deleteRT(RetainedTopic *rt)
{
    std::cout << "\t\t\tBROKER --> Delete Retained Topic: "<< rt->topic<<" Client Owner: "<< rt->owner << endl;
    std::unique_lock<std::mutex> lk(rtmtx);
    auto ret = this->rt_cache.equal_range(rt);
    this->rt_cache.erase(ret.first, ret.second);
    lk.unlock();
}

void Broker::deleteCl(Client *cl)
{
    std::unique_lock<std::mutex> lk{clmtx};
    this->clients.remove(cl);
    lk.unlock();
}

Client::Client(ClientOpsIF *cifops) : cif{cifops}
{
    // ctor
    this->m_thread = new std::thread(&Client::Process, this);
}

void Client::sendMsg(const Message &m)
{
    std::cout << "\t\t\tBROKER/CLIENT --> New message recieved" << endl;
    /// Agregar el dato como mensaje en la cola
    std::unique_lock<std::mutex> lck(this->m_mutex);
    this->m_queue.push(m.clone());
    lck.unlock();
    this->m_cv.notify_one();
}

bool Client::CreateThread()
{
    if (!m_thread)
    {
        this->m_thread = new std::thread(&Client::Process, this);
        std::cout << "\t\t\tBROKER/CLIENT --> Thread for Client " << this << " created" << endl;
    }
    return true;
};

/// @brief  Proceso en el thread del cliente
void Client::Process()
{
    bool b = true;
    while (b)
    {
        Message *msg;
        /// slow consumption
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        /// wait for a msg
        std::unique_lock<std::mutex> lk(m_mutex);

        while (m_queue.empty())
            m_cv.wait(lk);

        if (m_queue.empty())
            continue;

        msg = m_queue.front(); // Oldest msg
        m_queue.pop(); // Remove oldest msg
        lk.unlock();
        b = this->processMsg(msg);
    };
};

/// @brief Process of msg type
/// @param msg Pointer to msg
/// @return False with disconect msg. True in other case
bool Client::processMsg(Message *msg)
{
    switch (msg->getType())
    {
    case Message::TypeM::CONNECT:
    {
        this->processConnect(dynamic_cast<ConnectMsg *>(msg));
        return true;
        break;
    }
    case Message::TypeM::PUBLISH:
    {
        this->processPublish(dynamic_cast<PublishMsg *>(msg));
        return true;
        break;
    }
    case Message::TypeM::DISCONNECT:
    {
        this->processDisconnect();
        return false;
        break;
    }
    case Message::TypeM::SUBSCRIBE:
    {
        this->processSubs(dynamic_cast<SubscribeMsg *>(msg));
        return true;
        break;
    }
    default:
    {
        return true;
        break;
    }
    }
}

/// @brief Cuando hay mensaje que pide conexion
/// @param m Msg with user and pass
void Client::processConnect(ConnectMsg *m)
{
    if ((m->getUser() == Broker::getInstance()->getUser()) && ((m->getPass() == Broker::getInstance()->getPass())))
    {
        std::cout << "\t\t\tBROKER/CLIENT --> CONNECTION OK" << endl;
        ConnAckMsg *msj = new ConnAckMsg(ConnAckMsg::Status::CONNECTION_OK);
        this->sendBrokerCl2Client(*msj);
    }
    else
    {
        std::cout << "\t\t\tBROKER/CLIENT --> LOGIN ERROR" << endl;
        ConnAckMsg *msj = new ConnAckMsg(ConnAckMsg::Status::LOGIN_ERROR);
        this->sendBrokerCl2Client(*msj);
    }
}

/// @brief Subcripcion Process. One process in client and other in the broker.
/// @param msg Msg with topic
void Client::processSubs(SubscribeMsg *msg)
{
    std::cout << "\t\t\tBROKER/CLIENT --> Register Subscription : "<< msg->getTopic() <<" Owner: "<< this << endl;
    // creation of a new msg to add a subs in client and broker
    Subscription *s = new Subscription{msg->getTopic(), this};
    this->subs.push_back(s);                   /// New sub in the client

    // New Sub in broker
    Subscription *s2 = new Subscription{msg->getTopic(), this}; 
    Broker::getInstance()->registerNewSubs(s2); /// New Sub in the Broker

}

void Client::processPublish(PublishMsg *m)
{
    std::cout << "\t\t\tBROKER/CLIENT --> Process Publish Msg" << endl;
    Broker::getInstance()->forEachSubs(m, this);
}

void Client::sendBrokerCl2Client(const Message &m)
{
    std::cout << "\t\t\tBROKER/CLIENT --> Send msg from Broker/Client to SimClient" << endl;
    std::unique_lock<std::mutex> lk{this->cifmtx};
    this->cif->recvMsg(m);
    lk.unlock();
}

void Client::destroyCl(Client *cl)
{
    std::cout << "\t\t\tBROKER/CLIENT --> Destroy Client" << endl;
    Broker::getInstance()->deleteCl(cl);
    delete cl;
};

void Client::processDisconnect()
{
    Message *msg;
    std::cout << "\t\t\tBROKER/CLIENT --> DISCONNECTING client: " << this->cif << std::endl;

    std::unique_lock<std::mutex> lk(m_mutex);
    while (!m_queue.empty())
    {
        msg = m_queue.front();
        m_queue.pop();
    };
    lk.unlock();

    /// Recorro las subscripciones y las borro
    for (auto it = this->subs.begin(); it != this->subs.end(); ++it)
        Broker::getInstance()->delSub(*it);

    std::cout << "\t\t\tBROKER/CLIENT --> Delete Subscription" << endl;

    /// Si hay topico retenido
    for (auto it = this->topics.begin(); it != this->topics.end(); ++it)
        Broker::getInstance()->deleteRT(*it);
    
    std::cout << "\t\t\tBROKER/CLIENT --> Delete RT" << endl;

    std::thread dt(&Client::destroyCl, this);
    dt.detach();
}
