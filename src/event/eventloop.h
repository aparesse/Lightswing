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
/// @file eventloop.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <memory>
#include <vector>
#include <functional>
namespace lightswing
{

class epollevent;
class poller;

class eventloop
{
public:
    typedef std::function<void()> task_fn;
public:
    eventloop();
    ~eventloop();

    void loop(std::size_t interval);

    std::size_t epollevent_nums() const;

    void add_event(epollevent* event);
    void delete_event(epollevent* event);
    void update_event(epollevent* event);

private:
    // nocopyable
    eventloop& operator=(const eventloop&) = delete;
    eventloop(const eventloop&) = delete;
    eventloop(eventloop&&) = delete;
    
private:
    std::vector<epollevent*> active_events_;
    std::shared_ptr<poller> poller_;

};

} // namespace lightswing

#endif // EVENTLOOP_H
