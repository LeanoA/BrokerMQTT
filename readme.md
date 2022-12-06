# Broker MQTT Simulation with C++

Broker MQTT Simulation with C++ is a project that was developed as a educational project. The project aims to show the use of diferent C ++ features, such as: classes, inheritance, polymorphism, templates, etc. Also, the project aims to show the use of gtest, a library for unit testing in C ++. The project use CMake as a build system.

## Introduction

MQTT (Message Queuing Telemetry Transport) is a machine-to-machine (M2M)/"Internet of Things" connectivity protocol. It was designed as an extremely lightweight publish/subscribe messaging transport. 

This project is a simulation of a broker MQTT, which is a protocol for communication between devices. The project is divided into two parts: the broker and the client. 
- The broker is responsible for receiving messages from the client and sending them to the other clients. 
- The client is responsible for sending messages to the broker and receiving messages from the broker.

## Publish-Subscribe Model
In this model of comunication exists producers of messages, called publishers, and consumers of messages, called subscribers. The subscribers express interest in one or more topics, without knowledge of who the publishers are or if they even exist. The subscribers only recive messages that are of interest.

In this systems, the publishers send messages to a broker, which is responsible for sending the messages to the subscribers. The broker receives messsages and sends them to all the subscribers that are interested in the topic of the message. In this way, the publishers not need to know who the subscribers are, and vice versa.

The messages recieved by the broker are stored in a queue, which is a data structure that stores messages in a FIFO (First In First Out) order. The broker sends the messages to the subscribers in the same order that they were received. 

In adition to the retransmission of messages to the subscribers, the messages can be reteined in the broker if the publisher indicates that the messages must be retained. In this case, if a new client subscribes to a topic that has retained messages, the broker will send inmediatly the retained messages to the new subscriber.

## Message MQTT implemented in this project

There are diferent types of messages in MQTT. In this project, the following messages were implemented:

- CONNECT: The client sends this message to the broker to establish a connection. The message contains the user name and password of the client. The broker sends a CONNACK message to the client to confirm the connection.
- CONNACK: The broker sends this message to the client to confirm the connection.
- PUBLISH: The client sends this message to the broker to publish a message with topic,value and flag to indicate if the message must be retained. The format of the topic of the message can be a string of this format "aaaa/bbbb".
- SUBSCRIBE: The client sends this message to the broker to subscribe to a topic. In a real case, the topic can be a string that conteins * as wildcards to indicate a filter of multiple topics with this patern.
- UNSUBSCRIBE: The client sends this message to the broker to unsubscribe to a topic.
- DISCONNECT: The client sends this message to the broker to disconnect from the broker.

## Simulation of clients and broker

- The broker receives and send messages to different clients and simulate the beheivor of each client.
- The project has a implementation that simulate the dinamic of sends a recieves messages between multiple clients and broker. 
- The project implements a thread pool to simulate sends/recieves messages dinamic of multiple clients and broker.
- The project implements recurent programing and provides a sincronization between the threads of the clients and the broker to access the queue of messages.

### Connection between clients and broker

The `Broker` has a method `BrokerOpsIF *registerClient(ClientOpsIF *clientOps)` that is called by the client to start the connection with. The broker returns a pointer to a `BrokerOpsIF` interface that the client uses to send messages to the broker. The broker also keeps a pointer to the `ClientOpsIF` interface of the client, which it uses to send messages to the client.

### Simulation of clients

The project implements a `SimClient` class to simulate the clientes. The task of each client is executed their own thred. When a `SimClient` object is created, it recibe a reference to the `Broker` that allow the client to register with the broker through the `BrokerOpsIF *registerClient(ClientOpsIF *clientOps)` method.

### Simulation of broker
The broker is implemented in the `Broker` class. The broker send and recieve messages from the clients. In order to do that, the broker has:
- conteiner of conected clients.
- queue of messages.
- thread pool to send and recieve messages.
- reteind topics with a eficient data structure for lookup by topic.

#### Subscription and Retained Messages
There are two types of messages that the broker must handle to consult: subscription messages and retained messages. This messages are related to the client who send the message. The representation is the following:
```cpp
class Client;

struct Subscription {
    TopicName topic;
    Client *owner;
};
struct RetainedTopic {
    TopicName topic;
    TopicValue value;
    Client *owner;
};
```

#### Client
The `Broker` class has an objet that represents connected clients who implement the interface `BrokerOpsIF`. The message sended by `SimClients` has to be processed by the client and has to be concurrent with his own execution. One way to this is with a independient execution thread for the message processing.  The `Clients` process the message send throw `BrokerOpsIf::senMsg` who queue the message in the client queue.  The client has to generate eventually `Subscriptions` and `RetainedTopics` to keep a trakking of all of them. 

#### Broker
The `Broker` implement the `registerClient` method to register a client. This method instantiate a new `Client` who are going to atend the messages of the `SimClient`. The `Broker` has a cache for quick access to the `Subscriptions` and `RetainedTopics`.

## Implementation of the project

In order to execute the project, you need to have installed the following tools:
- CMake
- C++ compiler
- gtest

### Build the project

The project uses CMake as a build system. To build the project, you need to execute the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

### Run the project

To run the project, in the build directory, you need to execute the following command:

```bash
./main
```

### Run the tests

To run the tests, in the `/build` directory, you need to execute the following command:

```bash
./BrokerTest
./MessageTest
./QueueTest
./SimSubscriberTest
./SimTest
```

## Project structure

The project is divided into the following directories:
- `include`: contains the header files of the project.
- `src`: contains the source files of the project.
- `test`: contains the test files of the project.
- `build`: contains the build files of the project. This directory is created when you build the project.
- `CMakeLists.txt`: contains the CMake configuration of the project.
- `README.md`: contains the documentation of the project.
- `LICENSE`: contains the license of the project.


