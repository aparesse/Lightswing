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
/// @file poller.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef POLLER_H
#define POLLER_H

#include <vector>
#include <map>
#include <mutex>
#include <sys/epoll.h>

namespace lightswing
{
class epollevent;

class poller
{
public:
	poller();
	~poller();

	std::size_t epoll_event_nums() const { return events_map_.size(); }

	void add_event(epollevent*);
	void delete_event(epollevent*);
	void update_event(epollevent*);

	int wait(std::vector<epollevent*>* active_events, int interval);

	void set_interval(int interval) { interval_ = interval; }

private:
	void ctl(int fd, uint32_t events, int op);
	void add(int fd, uint32_t events);
	void del(int fd, uint32_t events);
	void mod(int fd, uint32_t events);

	epoll_event& get_event(std::size_t i);

private:
	// noncopyable
	poller& operator=(const poller&) = delete;
	poller(const poller&) = delete;

private:
	mutable std::mutex mutex_;
	int epoll_socket_;

	std::vector<epoll_event> events_;
	std::map<int, epollevent*> events_map_;
	int interval_;
};
} // namespace lightswing

#endif // POLLER_H
