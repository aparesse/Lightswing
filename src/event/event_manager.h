#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include <map>
#include <functional>
#include <mutex>
#include <unistd.h>
#include "event_loop.h"
#include "ep_event.h"
namespace lightswing {

struct EventFd {
    typedef std::shared_ptr<EventFd> Pointer;
    static Pointer create(int readfd, int writefd, EventLoop* loop) {
        return std::make_shared<EventFd>(readfd, writefd, loop);
    }

    EventFd(int readfd, int writefd, EventLoop* loop):
        write_fd(writefd),
        event(loop, readfd, EPOLLIN | EPOLLERR | EPOLLRDHUP) {
        loop->add_event(&event);
    }

    ~EventFd() {
        event.loop()->delete_event(&event);
        ::close(write_fd);
    }

    int write_fd;
    EpollEvent event;
};

class EventManager {
public:
    static const int kWRITE_FD = 1;
    static const int kREAD_FD = 0;

    typedef std::function<void()> Func;


public:
    EventManager();
    ~EventManager();
    void loop();
    EventLoop* eventloop();
    void set_interval(std::size_t interval);
    void add_epoll_event(EpollEvent* ev);
    void delete_epoll_event(EpollEvent* ev);

    void register_event(const std::string& name, Func func);
    void emit_event(const std::string& ev) const;
    void unregister_event(const std::string& name);

private:
    void handle_error(int fd);
    void handle_close(int fd);

private:
    EventManager(const EventManager& other) = delete;
    EventManager& operator=(const EventManager& other) = delete;

private:
    mutable std::mutex mutex_;
    EventLoop loop_;
    std::map<std::string, EventFd::Pointer> events_;
    std::size_t loop_interval_;
};

//把用于socket设置为非阻塞方式
//int setnonblocking(int sockfd);
const std::size_t kDEFAULT_INTERVAL = 500;

}//namespace
#endif // EVENT_MANAGER_H
