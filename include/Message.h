/**
 * @file Message.h
 * @author Alexander Leano
 * @brief Message class
 * @version 0.1
 * @date 2022-12-05
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
using namespace std;

using TopicName = string;
using TopicValue = string;


/// @brief Abstract Message class 
class Message
{
    public:
        enum class TypeM { CONNECT, CONNACK, PUBLISH, SUBSCRIBE, UNSUBSCRIBE, DISCONNECT };
        Message(TypeM t) : type(t) {};
        TypeM getType() const;
        virtual Message *clone()const = 0; /// para evitar object splicing
        virtual TopicName getTopic() const { return "Empty"; };
        virtual TopicValue getValue() const { return "Empty"; };
        // virtual ~Message();
    protected:

    private:
        TypeM type;
};



class ConnectMsg:public Message
{
    public:
        ConnectMsg(string UserN,string Pass);
        Message* clone() const {return new ConnectMsg(this->username,this->password);};
        string getUser(){return this->username;};
        string getPass(){return this->password;};
    protected:

    private:
        string username;
        string password;
// ...
};

class ConnAckMsg : public Message
{
    public:
        enum class Status { CONNECTION_OK, LOGIN_ERROR };
        ConnAckMsg(Status st);
        /// @brief Return the status of the connection
        Status getStatus() const { return this->status;};
        Message* clone() const {return new ConnAckMsg(this->status);};
    protected:

    private:
        Status status;
};

class SubscribeMsg : public Message
{
    public:
        SubscribeMsg(TopicName tn);///,Client* own);
        TopicName getTopic()const {return topic;}
        Message* clone() const {return new SubscribeMsg(this->topic);};
    protected:

    private:
        TopicName topic;
        ///Subscription sub;

};

class UnsubscribeMsg : public Message
{
    public:
        UnsubscribeMsg();
        Message* clone() const {return new UnsubscribeMsg();};
    protected:

    private:
};

class PublishMsg : public Message
{
    public:
        /// @brief Constructor for PublishMsg
        /// @param tn Topic name
        /// @param tv Topic value
        /// @param f Retain flag
        PublishMsg(TopicName tn,TopicValue tv, bool f=false);
        Message* clone() const {return new PublishMsg(this->topic,this->value,this->retain);};
        TopicName getTopic() const {return this->topic;};
        TopicValue getValue() const {return this->value;}
        bool getRetain() {return this->retain;};
    protected:

    private:
        TopicName topic;
        TopicValue value;
        bool retain;
};

class DisconnectMsg : public Message
{
    public:
        DisconnectMsg();
        Message* clone() const {return new DisconnectMsg();};

    protected:

    private:
};


#endif // MESSAGE_H
