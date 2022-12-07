# Broker MQTT simulation using C++

Broker MQTT Simulation with C++ is a project designed as an educational project. The purpose of this project is to demonstrate the use of various C++ features such as classes, inheritance, polymorphism, templates, etc. Additionally, this project aims to demonstrate the use of gtest, a C++ unit testing library. The project uses CMake as the build system.

## Introduction

MQTT (Message Queuing Telemetry Transport) is a machine-to-machine (M2M)/"Internet of Things" connectivity protocol. It is designed to be an extremely lightweight publish/subscribe message transport. 

This project is a simulation of a broker MQTT, which is a protocol for communication between devices. The project is divided into two parts: the broker and the client. 
- The broker is responsible for receiving messages from the client and sending them to the other clients. 
- The client is responsible for sending messages to the broker and receiving messages from the broker. In order to differentiate between the client representation in the broker side, the client are going to be called SimClient( Simulated Client).

## Publish-subscribe model
In this communication model, there are message producers, called publishers, and message consumers, called subscribers. Subscribers express interest in one or more topics without knowing who the publishers are or even if they exist. Subscribers only receive messages that interest them. Both of these are called SimClient in this project.

In this system, publishers send messages to a broker who is responsible for sending the messages to subscribers. The broker receives messages and sends them to all subscribers interested in the topic of the message. This way the publishers don't need to know who the subscribers are and vice versa.

Messages received by the broker are stored in a queue, which is a data structure that stores messages in FIFO (First In First Out) order. The broker sends messages to subscribers in the same order in which they were received.

In addition to forwarding messages to subscribers, messages may be retained at the broker if the publisher indicates that the messages should be retained. In this case, when a new client subscribes to a topic with retained messages, the broker immediately sends the retained messages to the new subscriber.

## MQTT message implemented in this project

There are different types of messages in MQTT. The following messages have been implemented in this project:

- CONNECT: The client sends this message to the broker to establish a connection. The message contains the username and password. The broker sends a CONNACK message to the client to confirm the connection.
- CONNACK: The broker sends this message to the client to confirm the connection. 
- PUBLISH: The client sends this message to the broker to publish a message with a topic, a value and a flag to indicate if the message should be retained. The format of the topic of the message can be a string of this format "aaaa/bbbb".
- SUBSCRIBE: The client sends this message to the broker to subscribe to a topic. In a real case, the topic can be a string containing * as a wildcard to specify a multi-topic filter with this pattern (the wildcards have not yet been implemented in this project)
- UNSUBSCRIBE: The client sends this message to the broker to unsubscribe from a topic. 
- DISCONNECT: The client sends this message to the broker to disconnect from the broker.

## Repesentation of clients on the broker 

- The broker receives and sends messages to different clients (publisher, subscriber, or both). The broker implements a thread pool of clients representation to simulate sending/receiving dynamic messages from multiple clients.
- Each client represented on the broker side has its own thread.
- The client represented on the broker side has an implementation to send and receive messages from the Publisher/Subscriber client (SimClients) and use the broker's methods.
- The project implements recurring programming and guarantees synchronization between client threads and the broker to access the client message queue.

### Connection between client and broker

The `Broker` has a method `BrokerOpsIF *registerClient(ClientOpsIF *clientOps)` which is called by the SimClient to start the connection to the broker. The broker returns a pointer to a `BrokerOpsIF` interface which the client uses to send messages to the broker (through the client representation on the broker side). The broker also stores a pointer to the SimClient `ClientOpsIF` interface, which the broker uses to send messages to the client.

### Clients simulation

The project implements a `SimClient` class to simulate the clients. Each client's work runs in its own thread. When a `SimClient` object is created, it receives a reference to the `Broker` that allows the client to register with the broker via the `BrokerOpsIF *registerClient(ClientOpsIF *clientOps)` method.

### Broker simulation
The broker is implemented in the `Broker` class. The broker sends and receives messages from the clients. To do this, the broker has:
- container of connected clients.
- a messages queue.
- thread pool for sending and receiving messages.
- retained topics with an efficient data structure for topic searches.

#### Subscription and Retained Messages
There are two types of messages that the broker must handle to consult: subscription messages and retained messages. These messages are related to the client sending message. The representation is the following:

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

#### Clients
The `Broker` has client objects representing Simclients. Messages sent by `SimClients` are to be processed by the broker's own client representation.  These clients have the `BrokerOpsIF` interface.  The `Clients` process the message sent by SimClient throw `BrokerOpsIf::senMsg` and queue the message. 
The client eventually needs to generate  `Subscriptions` and `RetainedTopics` to keep track of them. 

#### Broker
The `Broker` implements the `registerClient` method to register a client. This method creates a new `Client` which will attend messages from the `SimClient`. The `Broker` has a cache for quick access to the `Subscriptions` and `RetainedTopics`.

## Implementation of the project

To run the project, the following tools must be installed:
- CMake
- C++ compiler
- gtest

### Build the project

The project uses CMake as its build system. To compile the project, you need to run the following commands:

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

To run the tests in the `/build` directory, you need to run the following command:

```bash
./BrokerTest
./MessageTest
./QueueTest
./SimSubscriberTest
./SimTest
```

## Project structure

The project is divided into the following directories:
- `include`: contains project header files.
- `src`: contains the source files of the project.
- `test`: contains the test files of the project.
- `build`: contains the project build files. This directory is created when you build the project.
- `DoxygenDoc`: contains the project documentation.

and the following files:
- `CMakeLists.txt`: contains the CMake configuration of the project.
- `Doxyfile`: contains the Doxygen configuration of the project.
- `README.md`: contains the project documentation.
- `LICENSE`: contains the project license.

## Documentation

Project documentation is generated with Doxygen. Before the generation of the documentation, you need to install:

    - Doxygen: For more information, visit https://www.doxygen.nl/index.html
    - Graphviz: For more information, visit https://graphviz.org/

To generate the documentation, in the project root directory, run the following commands:

```bash
doxygen Doxyfile
```

The documentation is generated in the `DoxygenDoc` directory.


