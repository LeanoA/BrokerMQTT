#include "../include/Queue.h"

// template<class T>
// Queue<T>::Queue(){
//     Tam=0;
// };

// template<class T>
// size_t Queue<T>::Cantidad(){
//     return this->m_queue.size();
// };

// template<class T>
// void Queue<T>::put(T& m)
// {
//     std::unique_lock < std::mutex > lck (m_mutex) ;
//     m_queue.push(m.clone());
//     m_cv.notify_one();
//     lck.unlock();
// }

// template<class T>
// void Queue<T>::get(T* m)
// {
//     std::unique_lock<std::mutex> lk(m_mutex);
//     while (m_queue.empty())
//         m_cv.wait(lk);
//     m = m_queue.front();
//     m_queue.pop();
//     lk.unlock();
// }

