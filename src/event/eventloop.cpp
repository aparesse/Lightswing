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
/// @file eventloop.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#include <algorithm>
#include "eventloop.h"
#include "poller.h"
#include "epollevent.h"

namespace lightswing
{

eventloop::eventloop() :
    active_events_(),
    poller_(std::make_shared<poller>())
{
}


eventloop::~eventloop()
{
}

void eventloop::loop(std::size_t interval)
{
    active_events_.clear();
    poller_->wait(&active_events_, interval);
    std::for_each(active_events_.begin(), active_events_.end(),
                  [] (epollevent* event)
                  {
                    event->handle_event();
                  });
}

std::size_t eventloop::epollevent_nums() const
{
    return poller_->epoll_event_nums();
}

void eventloop::add_event(epollevent* event)
{
    poller_->add_event(event);
}

void eventloop::delete_event(epollevent* event)
{
    poller_->delete_event(event);
}

void eventloop::update_event(epollevent* event)
{
    poller_->update_event(event);
}

} // namespace lightswing
