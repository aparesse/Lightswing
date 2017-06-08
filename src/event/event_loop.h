#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <vector>
#include <memory>
#include <functional>
namespace lightswing {

class EpollEvent;
class Poller;

class EventLoop
{
public:
    typedef std::function<void()> TaskFunc;
public:
    EventLoop();
    ~EventLoop();
    void loop(std::size_t interval);
    std::size_t epoll_event_nums() const ;
    void add_event(EpollEvent *event);
    void delete_event(EpollEvent* event);
    void update_event(EpollEvent* event);

private:
    //nocopyable
   EventLoop& operator=(const EventLoop&) = delete;
   EventLoop(const EventLoop&) = delete;
private:
    std::vector<EpollEvent*> active_events_;
    std::shared_ptr<Poller> poller_;
};

} // namespace

#endif // EVENTLOOP_H
