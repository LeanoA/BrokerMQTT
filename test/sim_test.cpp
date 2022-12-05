#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Broker.h"
#include "../include/Message.h"
#include "../include/SimClient.h"

using namespace std;
using ::testing::_;
using ::testing::Return;



TEST(SimPublisher, SimSubscriberInstantation){
    Broker* SingletonBroker=Broker::getInstance();
    std::vector<TopicName> tn;
    tn.push_back("Humedad");
    SimSubscriber* ss1=new SimSubscriber(*SingletonBroker,tn);
    ss1->start();
    SimPublisher* sp2=new SimPublisher(*SingletonBroker,"Humedad");
    sp2->start();
    // sp2->runSim();
    // ASSERT_TRUE(sp2!=NULL);
    std::this_thread::sleep_for (std::chrono::seconds(2));
    sp2->exit();
    ss1->exit();
    std::cout<<"Fin de la prueba"<<std::endl;
    ASSERT_TRUE(ss1!=NULL);
    ASSERT_TRUE(sp2!=NULL);
}



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}