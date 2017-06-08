#ifndef POLLER_H
#define POLLER_H
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <mutex>
namespace lightswing {

class EpollEvent;

class Poller {
public:
    Poller();

    ~Poller();

    std::size_t epoll_event_nums() const { return events_map_.size(); }

    void add_event(EpollEvent *event);

    void delete_event(EpollEvent* event);

    void update_event(EpollEvent *event);

    int wait(std::vector<EpollEvent*>* active_events, int interval);

    void set_interval(int interval) { interval_ = interval; }

private:
    void ctl(int fd, uint32_t events, int op);

    void add(int fd, uint32_t events);

    void del(int fd, uint32_t events);

    void mod(int fd, uint32_t events);

    epoll_event& get_event(std::size_t i);


private:
     //nocopyable
    Poller& operator=(const Poller&) = delete;
    Poller(const Poller&) = delete;

private:
    mutable std::mutex mutex_;
    int epoll_socket_;

    std::vector<epoll_event> events_;

    std::map<int, EpollEvent*> events_map_;

    int interval_;
};

} //namespace

#endif // POLLER_H
