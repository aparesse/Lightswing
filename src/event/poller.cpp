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
/// @file poller.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#include <assert.h>
#include <algorithm>
#include <unistd.h>
#include "epollevent.h"
#include "poller.h"
#include "../logger.h"

namespace lightswing
{
const int kMAX_EPOLL_NUM = 65535;

poller::poller() :
	epoll_socket_(::epoll_create(kMAX_EPOLL_NUM)),
	events_(kMAX_EPOLL_NUM),
	events_map_()
{

}

poller::~poller()
{
	::close(epoll_socket_);
}

void poller::add_event(epollevent* event)
{
	std::lock_guard<std::mutex> lock(mutex_);
	assert(event);
	add(event->fd(), event->events());
	events_map_[event->fd()] = event;
}

void poller::delete_event(epollevent* event)
{
	assert(event);
	{
		std::lock_guard<std::mutex> lock(mutex_);
		auto iter = events_map_.find(event->fd());
		assert(iter != events_map_.end());
		events_map_.erase(iter);
	}
	del(event->fd(), event->events());
}


void poller::update_event(epollevent* event)
{
	assert(event);
	{
		std::lock_guard<std::mutex> lock(mutex_);
		events_map_[event->fd()] = event;
	}
	mod(event->fd(), event->events());
}

int poller::wait(std::vector<epollevent*>* active_events, int interval)
{
	int event_num = ::epoll_wait(epoll_socket_,
				&*events_.begin(),
				kMAX_EPOLL_NUM,
				interval);

	if (event_num < 0)
	{
		return event_num;
	}

	std::lock_guard<std::mutex> lock(mutex_);
	std::for_each(events_.begin(), events_.begin() + event_num,
				[=](epoll_event event)
				{
					auto ch = events_map_.find(event.data.fd);
					assert(ch != events_map_.end());
					epollevent* pevent = ch->second;
					assert(pevent->fd() == event.data.fd);
					// triggered event currently
					pevent->set_revents(event.events);
					// save all active event to a vector
					active_events->push_back(pevent);
				});
	return 0;
}

void poller::ctl(int fd, uint32_t events, int op)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = events;
	::epoll_ctl(epoll_socket_, op, fd, &ev);
}

void poller::add(int fd, uint32_t events)
{
	ctl(fd, events, EPOLL_CTL_ADD);
}

void poller::del(int fd, uint32_t events)
{
	ctl(fd, events, EPOLL_CTL_DEL);
}

void poller::mod(int fd, uint32_t events)
{
	ctl(fd, events, EPOLL_CTL_MOD);
}

struct epoll_event& poller::get_event(std::size_t i)
{
	assert(i < kMAX_EPOLL_NUM);
	return events_[i];
}

} // lightswing
