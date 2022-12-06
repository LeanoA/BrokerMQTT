/**
 * @file Broker.h
 * @author Alexander Leano
 * @brief Broker class
 * @version 0.1
 * @date 2022-12-05
*/

#ifndef BROKER_H
#define BROKER_H

#include "Message.h"
#include "SafeQueue.h"

#include <list>
#include <set>
#include <mutex>              ///sdt:mutex,std::unique_lock
#include <thread>             // std::thread
#include <queue>              /// std::queue
#include <condition_variable> ///std::condition_variable
#include <iostream>           /// std::cout

/* Struct to represent the concepts of subscriptions made 
 * by the clients and the topics published with the 
 * requirement to be retained for immediate use.
*/
struct Subscription;
struct RetainedTopic;
struct CompareTopic;
struct CompareRT;
class Broker;

/// @brief Interface implemented in the Broker, so that, SimClient sends messages
/// to the client representation in the broker side
class BrokerOpsIF
{
public:
    BrokerOpsIF() { ; };
    virtual void sendMsg(const Message &) = 0;

protected:
private:
};

/// @brief Interface implemented in the SimClient to receive messages from the client
/// representation in the broker side.
class ClientOpsIF
{
public:
    ClientOpsIF() { ; };
    virtual void recvMsg(const Message &) = 0;

protected:
private:
};


/// @brief Client representation in the broker side to
/// manage connection, messages, subscriptions and retained topics
class Client : public BrokerOpsIF
{
public:
    /// @brief
    /// @param cifops
    Client(ClientOpsIF *cifops);

    /// @brief Send message to the client representation in the broker to be added to the queue of messages.
    void sendMsg(const Message &m);

    /// @brief Send Msg to Sim Client
    void sendBrokerCl2Client(const Message &m);
    static void destroyCl(Client *cl);
private:
    std::thread *m_thread;            // Thread to manage the client
    ClientOpsIF *cif;                 // Client interface
    std::mutex cifmtx;                // Client interface mutex to protect the access to the interface
    std::list<Subscription *> subs;   // List of pointer of Client Subscriptions. Each client process its own subs.
    std::set<RetainedTopic *> topics; // Retained topic
    SafeQueue<Message *> m_queue;     // Message to process queue

    /// Thread Created to process each client
    bool CreateThread();

    /// Point of entry to the thread to process connection and msgs
    void Process();
    // Process of msg type
    bool processMsg(Message *msg);
    // Process to conenect the client with the broker
    void processConnect(ConnectMsg *m);
    // Process of subscription with a topic in the broker
    void processSubs(SubscribeMsg *msg);
    // Process of publish a msg
    void processPublish(PublishMsg *m);
    // Disconnect process
    void processDisconnect();
};

/// @brief Structure to represent the concept of
///        subscriptions made by the clients.
///  \param topic Name of the topic
///  \param owner *Client who subscribe the topic
struct Subscription
{
    TopicName topic;
    Client *owner;
};

/// @brief Structure for retained topics
/// \param topic Topic name
/// \param value Topic value
/// \param owner *Client who publish the topic
struct RetainedTopic
{
    TopicName topic;
    TopicValue value;
    Client *owner;
};

/// \brief Compare topics functor
struct CompareTopic
{
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

/// @brief Broker representation who register SimClients through the interface BrokerOpsIF
///. Singleton class.
class Broker
{
public:

    /// @brief Get the instance object of the broker who is a singleton.
    /// @return Broker *Broker instance
    static Broker *getInstance();
    void deleteInstance();

    /// @brief Create a new client representation in
    /// the broker side trough the interface ClientOpsIF implemented
    /// for the SimClient
    BrokerOpsIF *registerClient(ClientOpsIF *);
    
    /// @brief Register a new subscription in the Broker
    /// @param s Subscription msg
    void registerNewSubs(Subscription *s);

    /// @brief Look for a topic subscription and send the publish msg to the client
    /// @param m *Publish msg
    /// @param cl *Client who send the msg
    void forEachSubs(PublishMsg *m, Client *cl);

    /// @brief Get User name
    /// @return string
    string getUser() { return this->username; };

    /// @brief Get password
    /// @return string
    string getPass() { return this->password; };

    /// @brief Delete subscription
    /// @param s *Subscription
    void delSub(Subscription *s);
    /// @brief Delete retained topic
    /// @param rt *Retained topic
    void deleteRT(RetainedTopic *rt);

    /// @brief  Delete client
    /// @param cl *Client
    void deleteCl(Client *cl);

private:
    std::list<Client *> clients; // List of pointer of clients
    std::mutex clmtx;

    // cache for optimization of search of subscriptions with functor of comparation
    std::multiset<Subscription *, CompareTopic> subs_cache;
    std::mutex sbsmtx;

    // Cache for retained topics with functores of comparation
    std::set<RetainedTopic *, CompareRT> rt_cache;
    std::mutex rtmtx;

    /// Singleton
    Broker() { ; };                             /// Constructor
    Broker(Broker const &) = delete;            /// copy constructor private
    Broker &operator=(Broker const &) = delete; /// assignment operator private
    static Broker *m_pInstance;

    /// @brief Look for a retained topic for the new subscription
    /// and send the retained topic to the client
    /// @param sb *Subscription
    void ifRT(Subscription *sb);

    string username = "root"; // User name
    string password = "1234"; // Password
};

#endif // BROKER_H