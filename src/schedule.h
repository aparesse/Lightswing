#ifndef SCHEDULE
#define SCHEDULE
#include <atomic>
#include "coroutine.h"
#include "util/blocking_queue.h"
namespace lightswing {

const int kSTACK_SIZE = 4 * 1024;
class Schedule {
    friend struct Coroutine;
public:
    typedef Coroutine::Function Function;
    typedef std::owner_less<Coroutine::WeakPointer> WeakPtrLess;
    typedef BlockingQueue<Coroutine::Pointer> CoQueue;
public:
    Schedule();
    ~Schedule() {}

    void init(int thread_num);

    Coroutine::Pointer new_coroutine(Function func);

    void push_coroutine(int index, Coroutine::Pointer co);
    Coroutine::Pointer pop_coroutine(int index);

    std::size_t size() const;
    bool empty() const;
    int remove(Coroutine::Pointer co);

private:
    Schedule& operator=(const Schedule&) = delete;
    Schedule(const Schedule&) = delete;

private:
    mutable std::mutex mutex_;
    std::vector<CoQueue> coroutines_table_;
    std::atomic_size_t size_; //原子操作
    int max_id_;
};

} //namespace
#endif // SCHEDULE

