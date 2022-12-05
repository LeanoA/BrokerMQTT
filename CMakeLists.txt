include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# set minimum cmake version
cmake_minimum_required(VERSION 3.14) # version can be different
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged


project(BrokerMQTT_Project) #name of your project


add_library(BrokerMQTT
include/Broker.h
src/Broker.cpp
include/Message.h
src/Message.cpp
include/SimClient.h
src/SimClient.cpp
include/SafeQueue.h
)

# target_include_directories(BrokerMQTT PUBLIC '${CMAKE_CURRENT_SOURCE_DIR}'/inc
# '${CMAKE_CURRENT_SOURCE_DIR}'/src) #include directories

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(main PRIVATE BrokerMQTT)

add_executable(MessageTest ${CMAKE_CURRENT_SOURCE_DIR}/test/Message_test.cpp)
target_link_libraries(MessageTest PRIVATE BrokerMQTT gtest)

add_executable(BrokerTest ${CMAKE_CURRENT_SOURCE_DIR}/test/broker_test.cpp)
target_link_libraries(BrokerTest PRIVATE BrokerMQTT gtest gmock)

add_executable(QueueTest ${CMAKE_CURRENT_SOURCE_DIR}/test/queue_test.cpp)
target_link_libraries(QueueTest PRIVATE BrokerMQTT gtest gmock)

add_executable(SimTest ${CMAKE_CURRENT_SOURCE_DIR}/test/sim_test.cpp)
target_link_libraries(SimTest PRIVATE BrokerMQTT gtest gmock)

add_executable(SimSubscriberTest ${CMAKE_CURRENT_SOURCE_DIR}/test/sim_subscriber_test.cpp)
target_link_libraries(SimSubscriberTest PRIVATE BrokerMQTT gtest gmock)