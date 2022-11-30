#ifndef SafeQueue_H
#define SafeQueue_H
#include <mutex>          // std::mutex
#include <queue>          // std::queue
#include <condition_variable>

// How to do a queue with a mutex in C++11 (or C++0x)
// http://stackoverflow.com/questions/15278343/how-to-do-a-queue-with-a-mutex-in-c11-or-c0x
// http://stackoverflow.com/questions/15278343/how-to-do-a-queue-with-a-mutex-in-c11-or-c0x/15278389#15278389

template <class T>
class SafeQueue
{
private:
    std::queue<T*> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
public:
    // No copy constructor allowed
    SafeQueue(const SafeQueue&) = delete;
    // No assignment operator allowed
    SafeQueue& operator=(const SafeQueue&) = delete;

    SafeQueue();
    // ~SafeQueue();
    // Add an element to the queue.
    void enqueue(T& t);
    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T* dequeue(void);
    // Size of the queue
    size_t size(void);
};

template <class T>
SafeQueue<T>::SafeQueue(): m_queue(), m_mutex(), m_cv()
{
}

// template <class T>
// SafeQueue<T>::~SafeQueue()
// {
// }

template <class T>
void SafeQueue<T>::enqueue(T& t)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(t.clone());
    lock.unlock();
    m_cv.notify_one();
}
template <class T>
T* SafeQueue<T>::dequeue(void)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty())
    {
        m_cv.wait(lock);
    }
    T* t = m_queue.front();
    m_queue.pop();
    lock.unlock();
    return t;
}

template <class T>
size_t SafeQueue<T>::size(void)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    size_t size = m_queue.size();
    lock.unlock();
    return size;
}

#endif // SafeQueue_H