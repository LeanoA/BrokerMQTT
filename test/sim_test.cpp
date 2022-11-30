#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Broker.h"
#include "../include/Message.h"
#include "../include/SimClient.h"

using namespace std;
using ::testing::_;
using ::testing::Return;


// TEST(SimPublisher, SimPublisherInstantation){
//     Broker* SingletonBroker=Broker::getInstance();
//     SimPublisher* sp1=new SimPublisher(*SingletonBroker,"Temperatura");
//     sp1->runSim();
//     ASSERT_TRUE(sp1!=NULL);
    
// }

TEST(SimPublisher, SimSubscriberInstantation){
    Broker* SingletonBroker=Broker::getInstance();
    std::vector<TopicName> tn;///=new std::vector<TopicName>{"Temperatura"};
    tn.push_back("Temperatura");
    SimSubscriber* sp1=new SimSubscriber(*SingletonBroker,tn);
    sp1->start();
    // sp1->runSim();
    SimPublisher* sp2=new SimPublisher(*SingletonBroker,"Humedad");
    sp2->start();
    // sp2->runSim();
    // ASSERT_TRUE(sp2!=NULL);
    std::this_thread::sleep_for (std::chrono::seconds(40));
    std::cout<<"Finalizando la prueba"<<std::endl;
    sp2->exit();
    sp1->exit();
    std::cout<<"Fin de la prueba"<<std::endl;
    ASSERT_TRUE(sp1!=NULL);

}



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}