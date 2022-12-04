#ifndef SIMCLIENT_H
#define SIMCLIENT_H


#include <thread>         // std::thread
#include <chrono>
#include <string>     // std::string, std::to_string
#include "../include/Broker.h"


/** \brief Clase abstracta SimClient simula un cliente que se ejecuta en un therad.
 * Cuando se crea un objeto SimClient se recibe una referencia al Broker,
 * que le permitirá ejecutar el método registerClient.
 */
class SimClient : public ClientOpsIF
{
    public:
        /// @brief Simula un cliente que se ejecuta en un therad.
        /// @param bkr Reference to Broker
        SimClient(Broker& bkr);
        /// @brief Connect the SimClient
        void connect();
        /// @brief Disconnect the Simclient
        void disconnect();

        /// @brief Start the thread. Move runSim to a diferent thread.
        void start();
        /// @brief Stop the thread
        void exit();
    protected:
        bool connack=false;
        BrokerOpsIF *brops; // Broker interface
    private:
        std::thread simth; // Variable for thread execution
        Broker* broker; // Pointer to the broker
        /// @brief Run the thread
        virtual void runSim() = 0;
        // BrokerOpsIF *brops; // Broker interface
        ///..
};

//--------------------------------------
//--------------------------------------
//--------------------------------------

// SimPublisher only publish one topic
class SimPublisher : public SimClient
{
    public:
        SimPublisher(Broker &bkr,TopicName topNam) : SimClient(bkr), topic(topNam){};
        void recvMsg(const Message &);
        ~SimPublisher(){this->exit();};
        // void runSim();
    // protected:

    // private:
        void runSim();
        TopicName topic;    ///El topico que va a Publicar

};

//--------------------------------------
//--------------------------------------
//--------------------------------------

/// @brief Unico subscriptor que puede recibir muchos topicos de un publicador
class SimSubscriber : public SimClient
{
    public:
        /// @brief Constructor de SimSubscriber
        /// @param bkr Broker
        /// @param TN Vector of Topics to subscribe by reference
        SimSubscriber(Broker &bkr,vector <TopicName> &TN);
        /// @brief Message received from the broker
        void recvMsg(const Message &);
        ~SimSubscriber(){this->exit();}
    // protected:

    // private:
        void runSim();
        void subscribe();
        /// @brief Topic Subscribed Vector
        vector<TopicName> topicSub;
};





#endif // SIMCLIENT_H
