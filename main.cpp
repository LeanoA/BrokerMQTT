#include <iostream>
#include "include/Broker.h"
#include "include/SimClient.h"


#include <vector>

using namespace std;

int main()
{

    Broker* SingletonBroker=Broker::getInstance();

    std::vector<TopicName> tn{"Temperatura/roof"};

    // Wait 2 seconds to create the publisher
    std::this_thread::sleep_for (std::chrono::seconds(2));

    //New SimPublisher Client B, with topic "Temperatura/roof" and retain topic
    SimPublisher* sp1=new SimPublisher(*SingletonBroker,tn[0]);
    sp1->start();

    // Wait 2 seconds to create the subscriber 
    std::this_thread::sleep_for (std::chrono::seconds(2));

    // New SimSubscriber Client A, with topic "Temperatura/roof" and without retain topic
    SimSubscriber* ss2 = new SimSubscriber(*SingletonBroker,tn,tn[0]);

    ss2->start();

    sp1->exit();
    ss2->exit();

    return 0;
}

