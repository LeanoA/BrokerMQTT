/**
 * @file SafeQueue.h
 * @author Alexander Leano
 * @brief SafeQueue class
 * @version 0.1
 * @date 2022-12-05
*/

#ifndef SafeQueue_H
#define SafeQueue_H
#include <mutex> // std::mutex
#include <queue> // std::queue
#include <condition_variable>

// How to do a queue with a mutex in C++11 (or C++0x)
// http://stackoverflow.com/questions/15278343/how-to-do-a-queue-with-a-mutex-in-c11-or-c0x
// http://stackoverflow.com/questions/15278343/how-to-do-a-queue-with-a-mutex-in-c11-or-c0x/15278389#15278389

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue(void): q(), m(), c()
    {
    }

    ~SafeQueue(void)
    {
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

    // Return size of queue
    int size(void)
    {
        std::lock_guard<std::mutex> lock(m);
        return q.size();
    }

    // Return true if queue is empty
    bool empty(void)
    {
        std::lock_guard<std::mutex> lock(m);
        return q.empty();
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif // SafeQueue_H