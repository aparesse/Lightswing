#ifndef BLOCKING_SET
#define BLOCKING_SET
#include <mutex>
#include <condition_variable>
#include <set>

namespace lightswing {

const std::size_t kDEFAULT_BLKSET_SIZE = -1;
template<class T, class Compare = std::less<T>>
class BlockingSet {
public:
    BlockingSet(std::size_t size) :
        set_(),
        mutex_(),
        cond_(),
        max_size_(size) {}

    BlockingSet() :
        set_(),
        mutex_(),
        cond_(),
        max_size_(kDEFAULT_BLKSET_SIZE) {}

    int insert(T new_value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (set_.size() >= max_size_) {
            return -1;
        }
        set_.insert(std::move(new_value));
        cond_.notify_one();
        return 0;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return set_.empty();
    }

    int count(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        //while (set_.empty()) {
        cond_.wait(lock, [this]{ return !set_.empty(); });
        //}
        return set_.count(value);
    }

    int erase(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto iter = set_.find(value);
        if (iter == set_.end()) {
            return -1;
        } else {
            set_.erase(iter);
            return 0;
        }
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return set_.size();
    }

private:
    BlockingSet(const BlockingSet& other) = delete;
    BlockingSet& operator=(const BlockingSet& other) = delete;

private:
    std::set<T, Compare> set_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::size_t max_size_;
};



} //namespace
#endif // BLOCKING_SET

