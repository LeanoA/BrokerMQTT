#include <iostream>
#include <gtest/gtest.h>
#include "../include/Broker.h"
#include "../include/SimClient.h"
#include "../include/Message.h"

TEST(aux, CompareTopic){
    std::multiset<Subscription *, CompareTopic> subs_cache; 
    Subscription *s1 = new Subscription{"topic1", 0};
    Subscription *s2 = new Subscription{"topic2", 0};
    Subscription sub = Subscription{"topic1", 0};

    subs_cache.insert(s1);
    auto ret = subs_cache.equal_range(&sub);
    for (auto it = subs_cache.begin(); it != subs_cache.end(); ++it)
    {
        std::cout << "----> " << (*it)->topic << endl;
    }
    cout<< "----> " << endl;
    for (auto it = ret.first; it!= ret.second; it++)
    {
        std::cout << "----> " << (*it)->topic << endl;
    }

    assert(ret.first != subs_cache.end());
}

TEST(SimSubscriber, SimSubscriberInstantation){
    Broker* SingletonBroker=Broker::getInstance();
    std::vector<TopicName> tn;
    tn.push_back("Presion");
    SimSubscriber* sp1=new SimSubscriber(*SingletonBroker,tn);
    sp1->connect();
    sp1->subscribe();
    PublishMsg pm("Presion", "1atm");
    SingletonBroker->forEach(&pm, 0);
    // sp1->broker->forEach(new PublishMsg("Presion", "1atm"), 0);

    sp1->disconnect();
    
    ASSERT_TRUE(true);
}





int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}