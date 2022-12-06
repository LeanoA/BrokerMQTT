/**
 * @file SimClient.h
 * @author Alexander Leano
 * @brief SimClient class
 * @version 0.1
 * @date 2022-12-05
*/

#ifndef SIMCLIENT_H
#define SIMCLIENT_H

#include <thread> // std::thread
#include <chrono>
#include <string> // std::string, std::to_string
#include "../include/Broker.h"

/// @brief Class SimClient is a abstract class for Simulated Clients who want to connect to the Broker
class SimClient : public ClientOpsIF
{
public:
    /// @brief Constructor de SimClient
    /// @param bkr Reference to the Broker
    SimClient(Broker &bkr);

    /// @brief Connect the SimClient with the Broker
    void connect();

    /// @brief Disconnect the Simclient of the Broker
    void disconnect();

    /// @brief Start the thread. Move runSim to a diferent thread.
    void start();

    /// @brief Stop the thread
    void exit();

protected:
    bool connack = false; // Flag for connection
    BrokerOpsIF *brops; // Broker interface pointer
private:

    std::thread simth; // Variable for thread execution
    Broker *broker;    // Pointer to the broker
    /// @brief Run the thread
    virtual void runSim() = 0;
};

//--------------------------------------
//--------------------------------------
//--------------------------------------

// SimPublisher only publish one topic
class SimPublisher : public SimClient
{
public:
    SimPublisher(Broker &bkr, TopicName topNam) : SimClient(bkr), topic(topNam){};
    void recvMsg(const Message &);
    ~SimPublisher() { this->exit(); };

private:
    /// @brief Run the thread
    void runSim();
    /// @brief Topic to Publish
    TopicName topic;
};

//--------------------------------------
//--------------------------------------
//--------------------------------------

/// @brief Subscriber and Publisher Simulated Client A
class SimSubscriber : public SimClient
{
public:
    /// @brief Constructor de SimSubscriber
    /// @param bkr Broker
    /// @param TN Vector of Topics to subscribe by reference
    /// @param topNam Topic to publish. Default "topic1".
    SimSubscriber(Broker &bkr, vector<TopicName> &TN, TopicName topNam = "topic1");
    /// @brief Message received from the broker
    void recvMsg(const Message &);
    ~SimSubscriber() { this->exit(); }

private:
    void runSim();
    void subscribe();
    /// @brief Topic Subscribed Vector
    vector<TopicName> topicSub;
    /// @brief Topic to Publish
    TopicName topic;
};

#endif // SIMCLIENT_H
