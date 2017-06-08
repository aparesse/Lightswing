#ifndef CO_CONDITION
#define CO_CONDITION
#include <queue>
#include <memory>
#include "util/slice.h"
#include "co_mutex.h"
#include "util/blocking_queue.h"
namespace lightswing {

template<class T>
class CoQueue {
public:
    CoQueue(std::size_t size) : queue_(),
        mutex_(),
        max_size_(size) {}

    CoQueue() : queue_(),
        mutex_(),
        max_size_(kDEFAULT_BLKQUEUE_SIZE) {}

    int push(T new_value) {
        MutexGuard lock(mutex_);
        if (queue_.size() >= max_size_) {
            return -1;
        }
        queue_.push(std::move(new_value));
        return 0;
    }

    bool empty() const {
        MutexGuard lock(mutex_);
        return queue_.empty();
    }

    T pop() {
        mutex_.lock();
        while (queue_.empty()) {
            mutex_.unlock();
            yield();
            mutex_.lock();
        }

        T value = std::move(queue_.front());
        queue_.pop();
        mutex_.unlock();
        return value;
    }

    std::size_t size() const {
        MutexGuard lock(mutex_);
        return queue_.size();
    }

private:
    CoQueue(const CoQueue& other) = delete;
    CoQueue& operator=(const CoQueue& other) = delete;

private:
    std::queue<T> queue_;
    mutable CoMutex mutex_;
    std::size_t max_size_;
};

template <class T>
class Chan : public std::enable_shared_from_this<Chan<T>>{

public:
    typedef T ValueType;
    typedef std::shared_ptr<Chan<ValueType>> Pointer;
    typedef std::weak_ptr<Chan<ValueType>> WeakPointer;
    typedef CoQueue<ValueType> Queue;
    typedef std::shared_ptr<Queue> QueuePointer;

    template<typename _Tp, typename... _Args>
    friend std::shared_ptr<_Tp> make_shared(_Args&&... __args);


public:
    Chan();
    Chan(QueuePointer, QueuePointer, Pointer);

    //使用move
    int send(T value);
    int recv(T&);

    Pointer brother();


private:
    QueuePointer recv_queue_;
    QueuePointer send_queue_;
    WeakPointer brother_;
};

typedef typename Chan<int>::Pointer IntChan;
typedef typename Chan<std::string>::Pointer StrChan;
typedef typename Chan<Slice>::Pointer SliceChan;


template<class Type>
static typename Chan<Type>::Pointer make_chan() {
    return std::make_shared<Chan<Type>>();
}

template<class T>
inline Chan<T>::Chan() :
    recv_queue_(std::make_shared<Queue>()),
    send_queue_(std::make_shared<Queue>()),
    brother_()
{
}

template <class T>
inline int Chan<T>::send(T value) {
    auto brother = brother_.lock();
    if (!brother) {
        return -1;
    }
    send_queue_->push(std::move(value));
    return 0;
}

template <class T>
inline int Chan<T>::recv(T &value) {
    auto brother = brother_.lock();
    if (!brother && recv_queue_->empty()) {
        return -1;
    }
    value =  recv_queue_->pop();
    return 0;
}

template <class T>
inline typename Chan<T>::Pointer Chan<T>::brother() {
    std::shared_ptr<Chan<T> > myself =  this->shared_from_this();
    Pointer brother = std::make_shared<Chan<T>>
                                                (send_queue_,recv_queue_, myself);
    brother_ = brother;
    return brother;
}

template <class T>
inline Chan<T>::Chan(QueuePointer sendptr,
                     QueuePointer recvptr,
                     Pointer brother) :
    recv_queue_(sendptr),
    send_queue_(recvptr),
    brother_(brother)
{
}

}
#endif // CO_CONDITION

