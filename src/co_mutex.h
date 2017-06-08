#ifndef CO_MUTEX
#define CO_MUTEX
#include "utils.h"
#include <atomic>
#include <mutex>
namespace lightswing {

class CoMutex{
public:
    CoMutex();
    void lock();
    void unlock();
private:
    void lock_lockfree_aux();
    void lock_aux();
private:
    std::atomic_int contenders_;
    std::mutex mutex_;
};


//todo 最好改成编译期判断
inline CoMutex::CoMutex() :
    contenders_(0),
    mutex_()
{

}

//inline CoMutex::CoMutex(CoMutex && other) :
//    contenders_(std::move(other.contenders_)),
//    mutex_(other.mutex_)
//{

//}

inline void CoMutex::lock() {
    Runtime* runtime = Runtime::instance();
    if (runtime->max_procs() > 1) {
        lock_aux();
    } else {
        lock_lockfree_aux();
    }
}

inline void CoMutex::unlock() {
    --contenders_;
}

inline void CoMutex::lock_lockfree_aux() {
    while (contenders_ > 0) {
        yield();
    }
    ++contenders_;
}

inline void CoMutex::lock_aux() {
    while (true) {
        while (contenders_ > 0) {
            yield();
        }
        if (mutex_.try_lock()) {
            break;
        }
    }
    ++contenders_;
    mutex_.unlock();

}

class MutexGuard {
public:
    MutexGuard (CoMutex& mut);
    ~MutexGuard();
private:
    CoMutex &mutex_;
};

inline MutexGuard::MutexGuard(CoMutex &mut) :
    mutex_(mut) {
    mutex_.lock();
}

inline MutexGuard::~MutexGuard() {
    mutex_.unlock();
}

}
#endif // CO_MUTEX

