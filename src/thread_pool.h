#ifndef THREAD_POOL
#define THREAD_POOL
#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <map>
#include <assert.h>
#include "thread_context.h"
#include "logger.h"
namespace lightswing {

class ThreadPool {
public:
    typedef std::function<void()> Function;
    typedef std::map<std::thread::id, ThreadContext*> ThreadMap;
public:
    ThreadPool();
    void set_func(Function func);
    void stop();
    void start(std::size_t size);
    std::size_t size() const;
    bool is_running() const;
    const ThreadMap& thread_map() const;
    ThreadMap& thread_map();

private:
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&) = delete;

private:
    std::size_t size_;
    std::atomic_bool quit_;
    Function thread_loop_func_;
    std::vector<ThreadContext> thread_contexts_;
    ThreadMap thread_map_;
};

const int kDEFUALT_THREAD_NUM = 0;

inline ThreadPool::ThreadPool() :
    size_(0),
    quit_(true),
    thread_contexts_(),
    thread_map_()
{
}

inline void ThreadPool::set_func(ThreadPool::Function func) {
    thread_loop_func_ = func;
}

inline void ThreadPool::start(std::size_t size) {
    quit_ = false;
    thread_contexts_.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        thread_contexts_.push_back(ThreadContext(i, this));
    }
    for (std::size_t i = 0; i < size; ++i) {
        thread_contexts_[i].start();
    }
}

inline std::size_t ThreadPool::size() const {
    return size_;
}

inline bool ThreadPool::is_running() const {
    return !quit_;
}

inline const ThreadPool::ThreadMap &ThreadPool::thread_map() const {
    return thread_map_;
}

inline ThreadPool::ThreadMap &ThreadPool::thread_map() {
    return thread_map_;
}


} //namespace
#endif // THREAD_POOL

