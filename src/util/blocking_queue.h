#ifndef BLOCKING_QUEUE
#define BLOCKING_QUEUE
#include <mutex>
#include <condition_variable>
#include <queue>

namespace lightswing {

const std::size_t kDEFAULT_BLKQUEUE_SIZE = -1;

template<class T>
class BlockingQueue {
public:
    BlockingQueue(std::size_t size) : queue_(),
        mutex_(),
        cond_(),
        max_size_(size) {}

    BlockingQueue() : queue_(),
        mutex_(),
        cond_(),
        max_size_(kDEFAULT_BLKQUEUE_SIZE) {}

    int push(T new_value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= max_size_) {
            return -1;
        }
        queue_.push(std::move(new_value));
        cond_.notify_one();
        return 0;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        //while (queue_.empty()) {
        cond_.wait(lock, [this]{ return !queue_.empty(); });
        //}
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;
    BlockingQueue(BlockingQueue&& other) = delete;

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::size_t max_size_;
};


} //namespace
#endif // BLOCKING_QUEUE

