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
/// @file epollevent.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#include <unistd.h>
#include "epollevent.h"
#include "eventloop.h"
#include "../logger.h"

namespace lightswing
{
epollevent::epollevent(eventloop* loop, int fd, uint32_t events) :
	loop_(loop),
	fd_(fd),
	events_(events)
{
}


epollevent::~epollevent()
{
	loop_->delete_event(this);
	::close(fd_);
}

uint32_t epollevent::events() const
{
	return events_;
}

void epollevent::set_read_callback(const event_callback& cb)
{
	read_callback_ = cb;
}

void epollevent::set_write_callback(const event_callback& cb)
{
	write_callback_ = cb;
}

void epollevent::set_close_callback(const event_callback& cb)
{
	close_callback_ = cb;
}

void epollevent::set_error_callback(const event_callback& cb)
{
	error_callback_ = cb;
}

void epollevent::set_revents(uint32_t ev)
{
	revents_ = ev;
}

void epollevent::update()
{
	loop_->update_event(this);
}

void epollevent::disable_all()
{
	events_ &= !EPOLLIN;
	events_ &= !EPOLLOUT;
	events_ &= !EPOLLRDHUP;
	events_ &= !EPOLLERR;
	update();
}

int epollevent::fd() const
{
	return fd_;
}

void epollevent::set_fd(int fd)
{
	fd_ = fd;
}

void epollevent::handle_event()
{
	if (revents_ & EPOLLIN && read_callback_)
	{
		read_callback_(fd_);
	}

	if (revents_ & EPOLLOUT && write_callback_)
	{
		write_callback_(fd_);
	}

	if (revents_ & EPOLLRDHUP && close_callback_)
	{
		close_callback_(fd_);
	}

	if (revents_ & EPOLLERR && error_callback_)
	{
		// client close the socket
		error_callback_(fd_);
	}
}

}
