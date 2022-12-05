#ifndef BROKER_H
#define BROKER_H

#include "Message.h"

#include <list>
#include <set>
#include <mutex> ///sdt:mutex,std::unique_lock
#include <thread>         // std::thread
#include <queue>          /// std::queue
#include <condition_variable>   ///std::condition_variable
#include <iostream>       /// std::cout


/** \brief Estructura para representar los conceptos de subscripciones
 * hechas por los clientes y los t�picos publicados con el
 * requerimiento de ser retenidos para su inmediata utilizaci�n.
 */
struct Subscription;
struct RetainedTopic;
struct CompareTopic;
struct CompareRT;
class Broker;


/// @brief Interface implementada por el Broker para enviarle mensajes al Cliente
class BrokerOpsIF{
public:
    BrokerOpsIF(){;};
    virtual void sendMsg(const Message &) = 0;
protected:
private:
};

/// @brief Interface implementada por el Cliente para recibir los tópicos subscriptos
class ClientOpsIF
{
    public:
        ClientOpsIF(){;};
        virtual void recvMsg (const Message &) = 0;
    protected:

    private:
};

class Client : public BrokerOpsIF
{
    public:
        Client(ClientOpsIF *cifops);
        /// @brief Mensaje Recibido en el cliente simulado en el broker
        void sendMsg(const Message &m); 
        /// @brief Send Msg to Sim Client
        void sendBrokerCl2Client(const Message &m);
        static void destroyCl (Client *cl);

    protected:

    private:
        std::thread* m_thread;     /* Thread para el manejo de la cola de mensajes */
        ClientOpsIF *cif;   /*Client interface*/
        std::mutex cifmtx;  /*Client interface mutex */
        std::list <Subscription *> subs; // List of pointer of Client Subscriptions. Each client process its own subs.
        std::set <RetainedTopic *> topics; // Retained topic
        std::queue<Message*> m_queue;   // Message to process queue
        std::mutex m_mutex; // Mutex for msg (m_queue)
        std::condition_variable m_cv;   // Condition variable to msg
        // Thread Created to process each client
        bool CreateThread();
        //Point of entry to the thread
        void Process();
        // Process of msg type
        bool processMsg(Message* msg);
        // Process to conenect the client with the broker
        void processConnect(ConnectMsg* m);
        // Process of subscription with a topic in the broker
        void processSubs(SubscribeMsg* msg);
        // Process of publish a msg 
        void processPublish(PublishMsg* m);
        // Disconnect process
        void processDisconnect();

};

/** \brief Estructura para representar los conceptos de subscripciones
 * hechas por los clientes y los tópicos publicados con el
 * requerimiento de ser retenidos para su inmediata utilización.
 *  \param topic Nombre del tópico
 *  \param *owner Cliente que realiza la subscripción
 */
struct Subscription
{
    TopicName topic;
    Client *owner;
};

/// \brief Structure for retained topics
/// \param topic Topic name
/// \param value Topic value
/// \param *owner Client that publish the topic
struct RetainedTopic{
    TopicName topic;
    TopicValue value;
    Client *owner;
};

/// \brief Compare topics functor
struct CompareTopic{
    bool operator()(Subscription *lhs, Subscription *rhs) const
    {
        return (lhs->topic < rhs->topic);
    };
};

/// \brief Compare retained topics functor
struct CompareRT
{
    bool operator()(RetainedTopic *lhs, RetainedTopic *rhs) const
    {
        return (lhs->topic < rhs->topic);
    };
};


/** \brief El Broker debe implementar el método registerClient(), que deberá 
 * instanciar un nuevo Client atender al correspondiente SimClient(). 
 * También deberá tener los caches para acceso búsqueda rápida de 
 * subscripciones y tópicos retenidos.
 * Clase Singleton.El broker del sistema debe recibir y enviar mensajes a distintos clientes.
 * Para poder realizar sus tareas, debería tener en su representación las siguientes entidades:
- Mantener un contenedor con los clientes conectados
- Conjunto de tópicos que están subscriptos, cada uno de ellos posiblemente subscripto por múltiples clientes. Tiene que ser eficiente buscar una subscripción por el nombre del tópico.
- Conjunto de tópicos retenidos. Tiene que ser eficiente buscar un tópico por su nombre.
 * \param BrokerOpsIF* registerClient(ClientOpsIF *)
 * \return
 */
class Broker
{
public:

    static Broker *getInstance();
    void deleteInstance();
    BrokerOpsIF *registerClient(ClientOpsIF *);
    /// @brief Register a new subscription in the Broker
    /// @param s Subscription msg
    void registerNewSubs(Subscription *s);

    /// @brief Look for a topic subscription and send the publish msg to the client
    void forEachSubs(PublishMsg *m , Client *cl);

    // Get user
    string getUser() { return this->username; };
    // Get password
    string getPass() { return this->password; };
    // Delete Subcription
    void delSub(Subscription *s);
    // Delete retained topic
    void deleteRT(RetainedTopic *rt);
    // Delete client
    void deleteCl(Client *cl);
protected:
private:
    std::list<Client *> clients; /// deque, list, vect
    std::mutex clmtx;

    // cache para búsquedas eficientes de suscriptores con functores de comparacion
    std::multiset<Subscription *, CompareTopic> subs_cache; 
    std::mutex sbsmtx;

    // Cache for retained topics with functores of comparation
    std::set<RetainedTopic *, CompareRT> rt_cache;
    std::mutex rtmtx;

    /// Singleton
    Broker(){;};                                 /// Constructor
    Broker(Broker const &) = delete;            /// copy constructor privado
    Broker &operator=(Broker const &) = delete; /// operador asignación privado
    static Broker *m_pInstance;

    void ifRT(Subscription *sb);

    string username = "root";
    string password = "1234";
};



#endif // BROKER_H