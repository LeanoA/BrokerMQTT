#ifndef QUEUE_H
#define QUEUE_H
#include <mutex>          // std::mutex
#include <queue>          // std::queue

template <class T>
class Queue
{
    public:
        Queue(){Tam=0;};
        ///Destructor
        void put(T&);
        // Get function
        // T get();
        void get(T* m);
        size_t Cantidad();

    protected:

    private:
        size_t Tam;
        Queue(const Queue&);
        Queue& operator=(const Queue&); //

        std::queue <T*> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_cv;
};

template<class T>
size_t Queue<T>::Cantidad(){
    return this->m_queue.size();
};

template<class T>
void Queue<T>::put(T& m)
{
    std::unique_lock < std::mutex > lck (m_mutex) ;
    m_queue.push(m.clone());
    m_cv.notify_one();
    lck.unlock();
}

template<class T>
void Queue<T>::get(T* m)
{
    std::unique_lock<std::mutex> lk(m_mutex);
    while (m_queue.empty())
        m_cv.wait(lk);
    m = m_queue.front();
    m_queue.pop();
    lk.unlock();
}

// template<class T>
// T Queue<T>::get()
// {
//     std::unique_lock<std::mutex> lk(m_mutex);
//     while (m_queue.empty())
//         m_cv.wait(lk);
//     T m = *m_queue.front();
//     m_queue.pop();
//     lk.unlock();
//     return m;
// }


#endif // QUEUE_H
