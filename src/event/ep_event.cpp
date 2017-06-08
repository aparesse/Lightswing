#include "ep_event.h"
#include <unistd.h>
#include "event_loop.h"
#include "../logger.h"
namespace lightswing {


EpollEvent::EpollEvent(EventLoop *loop, int fd, uint32_t event):
    loop_(loop),
    fd_(fd), events_(event){

}

EpollEvent::~EpollEvent() {
    loop_->delete_event(this);
    ::close(fd_);
}


uint32_t EpollEvent::events() const {
    return events_;
}

void EpollEvent::set_read_callback(const EventCallback &cb) {
    read_callback_ = cb;
}

void EpollEvent::set_write_callback(const EventCallback &cb) {
    write_callback_ = cb;
}

void EpollEvent::set_error_callback(const EventCallback &cb) {
    error_callback_ = cb;
}

void EpollEvent::set_revents(uint32_t ev){
    revents_ = ev;
}

void EpollEvent::update() {
    loop_->update_event(this);
}

void EpollEvent::disable_all()
{

    events_ &= !EPOLLOUT;
    events_ &= !EPOLLIN;
    events_ &= !EPOLLRDHUP;
    events_ &= !EPOLLERR;
    update();

}

void EpollEvent::set_close_callback(const EventCallback &cb) {
    close_callback_ = cb;
}

int EpollEvent::fd() const {
    return fd_;
}

void EpollEvent::set_fd(int fd) {
    fd_ = fd;
}


void EpollEvent::handle_event() {
    if(revents_ & EPOLLIN && read_callback_) {
        read_callback_(fd_);
    }

    if(revents_ & EPOLLOUT && write_callback_) {
        write_callback_(fd_);
    }

    if(revents_ & EPOLLERR && error_callback_) {
        error_callback_(fd_);
    }

    if(revents_ & EPOLLRDHUP && close_callback_) {
        //当对方close套接字的时候,会调用这里
        close_callback_(fd_);
    }
}

} //namespace
