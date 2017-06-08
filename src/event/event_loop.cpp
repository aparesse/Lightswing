#include "event_loop.h"
#include <algorithm>
#include "ep_event.h"
#include "poller.h"
#include "../logger.h"
namespace lightswing {

EventLoop::EventLoop() :
    active_events_(),
    poller_(std::make_shared<Poller>())
{
}


EventLoop::~EventLoop()
{
}


void EventLoop::loop(std::size_t interval) {
    active_events_.clear();
    //epoll的wait
    poller_->wait(&active_events_, interval);
    std::for_each(active_events_.begin(), active_events_.end(), [](EpollEvent *event){
        event->handle_event();
    });
}

std::size_t EventLoop::epoll_event_nums() const {
    return poller_->epoll_event_nums();
}

void EventLoop::add_event(EpollEvent *event) {
    poller_->add_event(event);
}

void EventLoop::delete_event(EpollEvent *event) {
    poller_->delete_event(event);
}


void EventLoop::update_event(EpollEvent *event) {
    poller_->update_event(event);
}


} //namespace
