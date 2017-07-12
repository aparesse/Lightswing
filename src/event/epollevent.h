// Copyright (C) 
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU GeneratorExiteral Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., Free Road, Shanghai 000000, China.
// 
/// @file epollevent.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef EPOLLEVENT_H
#define EPOLLEVENT_H

#include <memory>
#include <functional>
#include <sys/epoll.h>

namespace lightswing
{
class eventloop;

class epollevent
{
public:
    typedef std::function<void(int)> event_callback;
    typedef std::shared_ptr<epollevent> pointer;

public:
    static pointer create(eventloop* loop, int fd, uint32_t events)
    {
        return std::make_shared<epollevent>(loop, fd, events);
    }

public:
    epollevent(eventloop* loop, int fd, uint32_t events);
    ~epollevent();


    void handle_event();
    void set_read_callback(const event_callback& cb);
    void set_write_callback(const event_callback& cb);
    void set_error_callback(const event_callback& cb);
    void set_close_callback(const event_callback& cb);

    int fd() const;
    void set_fd(int fd);

    // concerd about events
    uint32_t events() const;

    // active events currently
    void set_revents(uint32_t ev);

    void update();
    void enable_reading() { events_ |= EPOLLIN; update(); }
    void enbale_writing() { events_ |= EPOLLOUT; update(); }
    void enable_closing() { events_ |= EPOLLRDHUP; update(); }
    void enbale_error() {events_ |= EPOLLERR; update(); }

    void disable_reading() { events_ &= !EPOLLIN; update(); }
    void disable_writing() { events_ &= !EPOLLOUT; update(); }
    void disbale_closing() { events_ &= !EPOLLRDHUP; update(); }
    void disbale_error() { events_ &= !EPOLLERR; update(); }

    void disable_all();
    eventloop* loop() { return loop_; }

private:
    // noncopyable
    epollevent(epollevent&&) = delete;
    epollevent& operator=(const epollevent&) = delete;
    epollevent(const epollevent&) = delete;

private:
    eventloop* loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;

    event_callback read_callback_;
    event_callback write_callback_;
    event_callback error_callback_;
    event_callback close_callback_;
};

} // namespace lightswing
#endif // EPOLLEVENT_H
