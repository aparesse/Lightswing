#ifndef GOROUTINE
#define GOROUTINE
#include "runtime.h"
#include "coroutine.h"
#include <ctime>
namespace lightswing {

//不用加锁，每个线程有各自的ThreadContext
inline void yield() {
    Runtime::instance()->yield();
}

inline std::time_t get_timestrap_now() {
    std::time_t tt;
    std::time(&tt);
    return tt;
    //    struct tm *t = std::localtime(&tt);
    //    if (!t) {
    //        LOG_INFO<<"time err";
    //        return 0;
    //    }
    //    time_t time_uint= ::mktime(t);
    //printf("localtime %4d%02d%02d %02d:%02d:%02d\n",
    //t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

inline void cosleep(std::time_t sec) {
    std::time_t wait_sec = get_timestrap_now() + sec;
    while (get_timestrap_now() < wait_sec) {
        yield();
    }
}

template <class ...Arg>
inline void go(Arg&& ...arg) {
    auto func = std::bind(std::forward<Arg>(arg)...);
    //new_coroutine已加锁
    Runtime::instance()->new_coroutine(std::move(func));
}

inline void emit_event(const std::string& ev) {
    //emit_event已加锁
    Runtime::instance()->emit_event(ev);
}

inline void on_event(const std::string& ev, EventManager::Func func) {\
    //on_event已加锁
    Runtime::instance()->on_event(ev, std::move(func));
}

} //namespace

#endif // GOROUTINE

