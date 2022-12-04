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
    std::cout<<"Mensaje Connnect Creado"<<endl;
}

ConnAckMsg::ConnAckMsg(Status st) : Message(Message::TypeM::CONNACK),status(st)
{
    std::cout<<"ConnAckMsg Created"<<std::endl;
}
SubscribeMsg::SubscribeMsg(TopicName tn) : Message(Message::TypeM::SUBSCRIBE)
{
    this->topic=tn;
    std::cout<<"SubscribeMsg Created"<<endl;

}

UnsubscribeMsg::UnsubscribeMsg() : Message(Message::TypeM::UNSUBSCRIBE)
{
    //ctor
}

PublishMsg::PublishMsg(TopicName tn,TopicValue tv, bool f) : Message(Message::TypeM::PUBLISH)//, topic(tn), value(tv), retain(f)
{
    //ctor
    this->topic=tn;
    this->value=tv;
    this->retain=f;
    std::cout<<"PublishMsg Created -> Topic: "<<topic <<" Value: " << tv<<" retain: "<<retain <<std::endl;
};

DisconnectMsg::DisconnectMsg() : Message(Message::TypeM::DISCONNECT)
{
    //ctor
    std::cout<<"DisconnectMsg Created"<<std::endl;
}


