#include <iostream>
#include <gtest/gtest.h>
#include "../include/Message.h"

TEST(MessageTest, ConnectMsg) {
    ConnectMsg msg("user", "pass");
    EXPECT_EQ(msg.getType(), Message::TypeM::CONNECT);
    EXPECT_EQ(msg.getUser(), "user");
    EXPECT_EQ(msg.getPass(), "pass");
}
TEST(MessageTest, ConnAckMsgConnectionOk) {
    ConnAckMsg msg(ConnAckMsg::Status::CONNECTION_OK);
    EXPECT_EQ(msg.getType(), Message::TypeM::CONNACK);
    EXPECT_EQ(msg.getStatus(), ConnAckMsg::Status::CONNECTION_OK);
}

TEST(MessageTest, ConnAckMsgLoginError) {
    ConnAckMsg msg(ConnAckMsg::Status::LOGIN_ERROR);
    EXPECT_EQ(msg.getType(), Message::TypeM::CONNACK);
    EXPECT_EQ(msg.getStatus(), ConnAckMsg::Status::LOGIN_ERROR);
}

TEST(MessageTest, SubscribeMsg) {
    TopicName tN = "topic";
    SubscribeMsg msg(tN);
    EXPECT_EQ(msg.getType(), Message::TypeM::SUBSCRIBE);
    EXPECT_EQ(msg.getTopic(), "topic");
}
TEST(MessageTest, UnsubscribeMsg) {
    UnsubscribeMsg msg;
    EXPECT_EQ(msg.getType(), Message::TypeM::UNSUBSCRIBE);
}

TEST(MessageTest, PublishMsg) {
    TopicName tN="TopicName";
    std::string tV="TopicValue";
    PublishMsg msg(tN, tV);
    EXPECT_EQ(msg.getType(), Message::TypeM::PUBLISH);
    EXPECT_EQ(msg.getTopic(),tN);
    EXPECT_EQ(msg.getValue(), tV);
    EXPECT_EQ(msg.getRetain(), false);
}

TEST(MessageTest, PublishMsgRetain) {
    TopicName tN="TopicName";
    std::string tV="TopicValue";
    PublishMsg msg(tN, tV, true);
    EXPECT_EQ(msg.getType(), Message::TypeM::PUBLISH);
    EXPECT_EQ(msg.getTopic(),tN);
    EXPECT_EQ(msg.getValue(), tV);
    EXPECT_EQ(msg.getRetain(), true);
}

TEST(MessageTest, DisconnectMsg) {
    DisconnectMsg msg;
    EXPECT_EQ(msg.getType(), Message::TypeM::DISCONNECT);
}

TEST(MessageTest, MsgClone){
    ConnectMsg msg("user", "pass");
    Message* msg2 = msg.clone();
    EXPECT_EQ(dynamic_cast<ConnectMsg *>(msg2)->getType(), Message::TypeM::CONNECT);
    EXPECT_EQ(dynamic_cast<ConnectMsg *>(msg2)->getUser(), "user");
    EXPECT_EQ(dynamic_cast<ConnectMsg *>(msg2)->getPass(), "pass");
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}