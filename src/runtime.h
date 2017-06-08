#ifndef RUNTIME
#define RUNTIME
#include <cstddef>
#include <assert.h>
#include <functional>
#include "singleton.h"
#include "schedule.h"
//#include "memory/memory_pool.h"
#include "thread_pool.h"
#include "logger.h"
#include "event/event_manager.h"
namespace lightswing {


class Runtime{
public:

public:
    typedef Schedule::Function Func;
    friend class ThreadContext;
    friend class Goroutine;
    friend void yield();
public:

    Runtime();

    ~Runtime();

    static Runtime* instance();

    void on_event(const std::string& ev, EventManager::Func func);

    void emit_event(const std::string& ev) const;

    Coroutine::Pointer new_coroutine(std::function<void()> func);

    void set_max_procs(std::size_t num);

    std::size_t max_procs() const;

    template<class ...Arg>
    void start(Arg&& ...arg);

    void set_loop_interval(std::size_t interval);

    EventManager& event_manager();

    int get_thread_id() const;

    void yield();

private:
    Runtime& operator=(const Runtime& other) = delete;
    Runtime(const Runtime& other) = delete;

private:
    std::size_t core_num_;
    Schedule schedule_;
    ThreadPool threadpool_;
    EventManager event_manager_;
};

template<class ...Arg>
inline void Runtime::start(Arg && ...arg) {
    assert(!threadpool_.is_running());
    schedule_.init(core_num_);
    threadpool_.start(core_num_);
    auto func = std::bind(std::forward<Arg>(arg)...);
    new_coroutine(func);
    while (!schedule_.empty()) {
        event_manager_.loop();
    }
}

}//namespace
#endif // RUNTIME

