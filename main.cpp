#include <iostream>
#include "include/Broker.h"
#include "include/SimClient.h"


#include <vector>

using namespace std;

int main()
{

    Broker* SingletonBroker=Broker::getInstance();
    printf("variable SingletonBroker is at address: %p\n", SingletonBroker);

    ///Nuevo subscriptor

    std::vector<TopicName> tn;///=new std::vector<TopicName>{"Temperatura"};
    tn.push_back("Temperatura");


    ///Espero antes de crear el publicador
    std::this_thread::sleep_for (std::chrono::seconds(2));

    ///Nuevo Publicador

    SimPublisher* sp1=new SimPublisher(*SingletonBroker,tn[0]);
    sp1->start();

    ///Espero antes de crear el Subscriptor
    std::this_thread::sleep_for (std::chrono::seconds(2));


    SimSubscriber* ss2 = new SimSubscriber(*SingletonBroker,tn,"Temperatura");

    ss2->start();

    // SimSubscriber* ss3 = new SimSubscriber(*SingletonBroker,tn);

    // ss3->start();

    sp1->exit();
    ss2->exit();
    // ss3->exit();

    return 0;
}

