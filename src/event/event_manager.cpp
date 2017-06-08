#include "event_manager.h"
#include <unistd.h>
#include "../net/tcputils.h"
#include "../logger.h"
#include "ep_event.h"
#include "../utils.h"
namespace lightswing {

EventManager::EventManager() :
    mutex_(),
    loop_(),
    events_(),
    loop_interval_(kDEFAULT_INTERVAL)
{

}

EventManager::~EventManager() {

}

void EventManager::loop() {
    loop_.loop(loop_interval_);
}

EventLoop *EventManager::eventloop() {
    return &loop_;
}

void EventManager::set_interval(std::size_t interval) {
    loop_interval_ = interval;
}

void EventManager::add_epoll_event(EpollEvent *ev) {
    loop_.add_event(ev);
}

void EventManager::delete_epoll_event(EpollEvent *ev) {
    loop_.delete_event(ev);
}

int __read_one_byte(int fd) {
    char byte;
    int n = ::read(fd, &byte, 1);
    //todo
    if (n < 0) {
        LOG_DEBUG<<"read error n < 0";
    }
    if (n == 0) {
        LOG_DEBUG<<"read close n == 0";
    }
    return n;
}

int __send_one_byte(int fd) {
    int n = ::write(fd, "", 1);
    //todo
    if (n < 0) {
        LOG_DEBUG<<"write error n < 0";
    }
    if (n == 0) {
        LOG_DEBUG<<"write close n == 0";
    }
    return n;
}

void EventManager::register_event(const std::string& name, Func func) {
    std::lock_guard<std::mutex> lock(mutex_);
    int pipefd[2];
    int err = pipe(pipefd);
    if (err < 0) {
        LOG_DEBUG<<"pipe error";
        return;
    }
    setnonblocking(pipefd[kREAD_FD]);
    setnonblocking(pipefd[kWRITE_FD]);
    EventFd::Pointer ev = EventFd::create(
                pipefd[kREAD_FD], pipefd[kWRITE_FD], &loop_);
    ev->event.set_read_callback([this, func] (int fd) {
        //会在runtime线程执行
        if (__read_one_byte(fd) < 1) {
            return;
        }
        go(func);
    });
    ev->event.set_close_callback([this] (int fd) { handle_close(fd);  });
    ev->event.set_error_callback([this] (int fd) { handle_error(fd);  });
    events_[name] = ev;
}

void EventManager::emit_event(const std::string &ev) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = events_.find(ev);
    if (iter == events_.end()) {
        LOG_DEBUG<<"没有注册的信号";
        return;
    }
    EventFd::Pointer event = iter->second;
    int n = __send_one_byte(event->write_fd);
    if (n < 1) {
        LOG_DEBUG<<"发送信号失败";
    }
}

void EventManager::unregister_event(const std::string &name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = events_.find(name);
    if (iter == events_.end()) {
        LOG_DEBUG<<"没有注册的信号";
        return;
    }
    events_.erase(iter);
}


void EventManager::handle_error(int fd) {
    //todo
    LOG_DEBUG<<"handle_error fd:"<<fd;
}

void EventManager::handle_close(int fd) {
    //todo
    LOG_DEBUG<<"handle_close fd:"<<fd;
}



}//namespace
