#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Message.h"
#include "../include/SafeQueue.h"

TEST(SafeQueueTest,Instantiation){
    SafeQueue<int> q;
    ASSERT_TRUE(q.size()==0);
}

TEST(SafeQueueTest,enqueue){
    SafeQueue<Message *> q;
    TopicName tN = "topic";
    SubscribeMsg msg(tN);
    q.enqueue(&msg);
    ASSERT_TRUE(q.size()==1);
}

TEST(SafeQueueTest,dequeue){
    SafeQueue<Message*> q;
    TopicName tN = "topic";
    SubscribeMsg msg(tN);
    q.enqueue(&msg);
    EXPECT_EQ(q.size(),1);
    Message *m;
    m=q.dequeue();
    EXPECT_EQ(q.size(),0);
    EXPECT_EQ(dynamic_cast<SubscribeMsg *>(m)->getType(), Message::TypeM::SUBSCRIBE);
    EXPECT_EQ(dynamic_cast<SubscribeMsg *>(m)->getTopic(),"topic");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

