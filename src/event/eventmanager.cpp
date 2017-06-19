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
/// @file eventmanager.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#include "eventmanager.h"
#include "../net/tcputils.h"
#include "../net/net.h"
#include "../exec_coroutine.h"
#include "../logger.h"

namespace lightswing
{
eventmanager::eventmanager() :
	mutex_(), // std::mutex default construct called
	loop_(), // event_loop default construct called
	events_(), // empty map
	loop_interval_(kDEFUALT_INTERVAL)
{

}

eventmanager::~eventmanager()
{

}

void eventmanager::loop()
{
	loop_.loop(loop_interval_);
}

eventloop* eventmanager::the_eventloop()
{
	return &loop_;
}

void eventmanager::set_interval(std::size_t interval)
{
	loop_interval_ = interval;
}

void eventmanager::add_epoll_event(epollevent* event)
{
	loop_.add_event(event);
}

void eventmanager::delete_epoll_event(epollevent* event)
{
	loop_.delete_event(event);
}

int __read_one_byte(int fd)
{
	char byte;
	int n = ::read(fd, &byte, 1);
	// TODO
	if (n < 0)
	  LOG_DEBUG << "read error n < 0";

	if (n == 0)
	  LOG_DEBUG << "read close n == 0";

	return n;
}

int __send_one_byte(int fd)
{
	int n = ::write(fd, "", 1);
	// TODO
	if (n < 0)
	  LOG_DEBUG << "write error n < 0";
	
	if (n == 0)
	  LOG_DEBUG << "write close n == 0";

	return n;
}

void eventmanager::register_event(const std::string& name, func fn)
{
	std::lock_guard<std::mutex> lock(mutex_);
	int pipefd[2];
	int err = pipe(pipefd);
	if (err < 0)
	{
		LOG_DEBUG << "pipe error";
		return;
	}
	set_non_blocking(pipefd[kREAD_FD]);
	set_non_blocking(pipefd[kWRITE_FD]);

	eventfd::pointer ev_fd = eventfd::create(pipefd[kREAD_FD],
				pipefd[kWRITE_FD],
				&loop_);
	ev_fd->event_.set_read_callback([this, fn](int fd)
				                    {
										if (__read_one_byte(fd) < 1)
										{
											return;
										}
										go(fn);
									});

	ev_fd->event_.set_close_callback([this](int fd) { handle_close(fd); });
	ev_fd->event_.set_error_callback([this](int fd) { handle_error(fd); });
	events_[name] = ev_fd;
}

void eventmanager::unregister_event(const std::string& name)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto iter = events_.find(name);
	if (iter == events_.end())
	  LOG_DEBUG << "unregister_event failed";
	events_.erase(iter);
}

void eventmanager::emit_event(const std::string& name) const
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto iter = events_.find(name);
	if (iter == events_.end())
	{
		LOG_DEBUG << "unregistered event";
		return;
	}
	eventfd::pointer event = iter->second;
	int n = __send_one_byte(event->write_fd_);
	if (n < 1)
	  LOG_DEBUG << "send event failed";
	return;
}

void eventmanager::handle_close(int fd)
{
	// TODO
	LOG_DEBUG << "handle_error fd: " << fd;
}

void eventmanager::handle_error(int fd)
{
	// TODO
	LOG_DEBUG << "handle_error fd: " << fd;
}

} // namespace lightswing
