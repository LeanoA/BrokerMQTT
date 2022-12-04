#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Broker.h"
#include "../include/SimClient.h"

using namespace std;
using ::testing::_;
using ::testing::Return;

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

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}