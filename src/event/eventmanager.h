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
/// @file eventmanager.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <map>
#include <functional>
#include <mutex>
#include <unistd.h>
#include "eventloop.h"
#include "epollevent.h"

namespace lightswing
{
struct eventfd
{
	typedef std::shared_ptr<eventfd> pointer;
	static pointer create(int read_fd, int write_fd, eventloop* loop)
	{
		return std::make_shared<eventfd>(read_fd, write_fd, loop);
	}
	
	eventfd(int read_fd, int write_fd, eventloop* loop) :
		event_(loop, read_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP),
		write_fd_(write_fd)
	{
		loop->add_event(&event_);
	}

	~eventfd()
	{
		event_.loop()->delete_event(&event_);
		::close(write_fd_);
	}

	epollevent event_;
	int write_fd_;
};


class eventmanager
{
public:
	static const int kWRITE_FD = 1;
	static const int kREAD_FD = 0;

	typedef std::function<void()> func;

public:
	eventmanager();
	~eventmanager();

	void loop();
	eventloop* the_eventloop();
	void set_interval(std::size_t interval);
	void add_epoll_event(epollevent* ev);
	void delete_epoll_event(epollevent* ev);

	void register_event(const std::string& name, func fn);
	void emit_event(const std::string& name) const;
	void unregister_event(const std::string& name);

private:
	void handle_error(int fd);
	void handle_close(int fd);

private:
	eventmanager(const eventmanager& other) = delete;
	eventmanager& operator=(const eventmanager& other) = delete;

private:
	mutable std::mutex mutex_;
	eventloop loop_;
	std::map<std::string, eventfd::pointer> events_;
	std::size_t loop_interval_;
};

const std::size_t kDEFUALT_INTERVAL = 500;
}

#endif //EVENTMANAGER_H
