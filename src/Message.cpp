/**
 * @file Message.cpp
 * @author Alexander Leano
 * @brief Message class
 * @version 0.1
 * @date 2022-12-05
*/

#include "../include/Message.h"

Message::TypeM Message::getType() const
{
    return type;
};

ConnectMsg::ConnectMsg(string UserN,string Pass) :  Message(Message::TypeM::CONNECT)
{
    //ctor
    this->username=UserN;
    this->password=Pass;
}

ConnAckMsg::ConnAckMsg(Status st) : Message(Message::TypeM::CONNACK),status(st)
{
}
SubscribeMsg::SubscribeMsg(TopicName tn) : Message(Message::TypeM::SUBSCRIBE)
{
    this->topic=tn;
}

UnsubscribeMsg::UnsubscribeMsg() : Message(Message::TypeM::UNSUBSCRIBE)
{
}

PublishMsg::PublishMsg(TopicName tn,TopicValue tv, bool f) : Message(Message::TypeM::PUBLISH)//, topic(tn), value(tv), retain(f)
{
    //ctor
    this->topic=tn;
    this->value=tv;
    this->retain=f;
};

DisconnectMsg::DisconnectMsg() : Message(Message::TypeM::DISCONNECT)
{
}


