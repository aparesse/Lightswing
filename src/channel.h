#ifndef CHANNEL
#define CHANNEL
#include <memory>
#include "util/blocking_queue.h"
#include "util/any.h"
namespace lightswing {
//这个是多线程之间用的通讯工具类，有点鸡肋，弃用。
//协程之间应该用chan
template <class T>
class Channel {
public:
    typedef T ValueType;
    typedef BlockingQueue<ValueType> QueueType;
    typedef std::shared_ptr<QueueType> QueuePointer;
    typedef std::shared_ptr<Channel> Pointer;

public:
    Channel();
    Channel(std::size_t size);

    //失败（满了） 返回-1
    int send(const ValueType& value);
    ValueType recv();
    Channel<ValueType> brother();
    std::size_t send_size() const;
    std::size_t recv_size() const;

private:
    struct __PrivateConstruct{};
    Channel(__PrivateConstruct);
private:
    QueuePointer send_queue_;
    QueuePointer recv_queue_;
};

const int __BROTHER = 0;

template<class T>
inline Channel<T> make_channel() {
    return Channel<T>();
}

template<class T>
inline Channel<T>::Channel() :
    send_queue_(std::make_shared<QueueType>()),
    recv_queue_(std::make_shared<QueueType>())
{

}

template<class T>
inline Channel<T>::Channel(std::size_t size) :
    send_queue_(std::make_shared<QueueType>(size)),
    recv_queue_(std::make_shared<QueueType>(size))
{

}

template<class T>
inline int Channel<T>::send(const Channel::ValueType &value) {
    return send_queue_->push(value);
}

template <class T>
inline std::size_t Channel<T>::send_size() const {
    return send_queue_->size();
}

template <class T>
inline std::size_t Channel<T>::recv_size() const {
    return send_queue_->size();
}

template <class T>
inline Channel<T> Channel<T>::brother() {
    //调用private的重载版本构造函数
    Channel<T> brother_channel{__PrivateConstruct()};
    brother_channel.recv_queue_ = send_queue_;
    brother_channel.send_queue_ = recv_queue_;
    return brother_channel;
}

template<class T>
inline T Channel<T>::recv() {
    return recv_queue_->pop();
}

template<class T>
inline Channel<T>::Channel(__PrivateConstruct) :
    send_queue_(nullptr),
    recv_queue_(nullptr)
{

}

}//namespace
#endif // CHANNEL

