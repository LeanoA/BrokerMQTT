#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Broker.h"

using namespace std;
using ::testing::_;
using ::testing::Return;

// class BrokerTestsClass : public ::testing::Test {
// public:

// protected:
// 	Broker broker;
// };

class MockCOIF : public ClientOpsIF {
	public:
    string name = "1";
	void recvMsg (const Message &){
    };
};



TEST(BrokerTest, CreateBroker) {
    Broker* SingletonBroker=Broker::getInstance();
    ASSERT_TRUE(SingletonBroker!=NULL);
    ASSERT_TRUE(SingletonBroker->getPass()=="1234");
    ASSERT_TRUE(SingletonBroker->getUser()=="root");
    // delete SingletonBroker;
    SingletonBroker->deleteInstance();
}

TEST(ClienteOpsIF, CreateClientOpsIF) {
    MockCOIF* clientOpsIF=new MockCOIF();
    Client client1(clientOpsIF);
    ASSERT_TRUE(clientOpsIF!=NULL);
    delete clientOpsIF;
}

// TEST(BrokerTest, DeleteBroker) {
//     Broker* SingletonBroker=Broker::getInstance();
//     ASSERT_TRUE(SingletonBroker!=NULL);
//     // Something is wrong with this testls
//     ASSERT_TRUE(SingletonBroker->getPass()=="1234");
//     ASSERT_TRUE(SingletonBroker->getUser()=="root");

//     MockCOIF* clientOpsIF=new MockCOIF();
//     BrokerOpsIF *pBroker;
//     pBroker = SingletonBroker->registerClient(clientOpsIF);
//     ASSERT_EQ(pBroker->name,"1");
//     SingletonBroker->deleteInstance();
//     // ASSERT_EQ(SingletonBroker,NULL);
// }



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}