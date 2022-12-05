#include <iostream>
#include <gtest/gtest.h>
#include "../include/Broker.h"
#include "../include/SimClient.h"
#include "../include/Message.h"

// TEST(SimSubscriber, SimSubscriberRecibeMsjFromBroker){
//     Broker* SingletonBroker=Broker::getInstance();
//     std::vector<TopicName> tn;
//     tn.push_back("Presion");
//     SimSubscriber* sp1 = new SimSubscriber(*SingletonBroker,tn);
//     sp1->connect();
//     sp1->subscribe();
//     PublishMsg pm("Presion", "1atm");
//     SingletonBroker->forEachSubs(&pm, 0);
//     PublishMsg pm2("Presion", "2atm");
//     SingletonBroker->forEachSubs(&pm, 0);
//     sp1->disconnect();  
//     ASSERT_TRUE(true);
// }

TEST(SimSubscriber, SimSubscriberRecibeMsjFromBroker){
    Broker* SingletonBroker=Broker::getInstance();
    std::vector<TopicName> tn;
    tn.push_back("Presion");
    SimSubscriber* ss1 = new SimSubscriber(*SingletonBroker,tn);
    // ss1->start();

    // SimSubscriber* ss2 = new SimSubscriber(*SingletonBroker,tn);
    // ss2->connect();
    // ss2->subscribe();
    
    std::this_thread::sleep_for (std::chrono::seconds(2));
    PublishMsg pm("Presion", "1atm",true);
    SingletonBroker->forEachSubs(&pm, 0);
    std::this_thread::sleep_for (std::chrono::seconds(2));
    PublishMsg pm2("Presion", "2atm",true);
    SingletonBroker->forEachSubs(&pm, 0);

    ss1->start();
    // ss1->connect();
    // ss1->subscribe();


    // for(size_t i=1;i<15;i++)
    // {
    //     std::this_thread::sleep_for (std::chrono::milliseconds(2000));
    //     PublishMsg pm = PublishMsg( "Presion" , std::to_string(i) , false);
    //     SingletonBroker->forEachSubs(&pm, 0);
    // };

    ss1->disconnect();
    // ss2->disconnect();

    // std::this_thread::sleep_for (std::chrono::seconds(1));
    // ss1->exit();
    ASSERT_TRUE(true);
}





int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}