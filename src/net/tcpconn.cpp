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
/// @file tcpconn.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include "tcpconn.h"

namespace lightswing
{

tcpconn::tcpconn(eventloop* loop, int fd) :
	ev_(epollevent::create(loop, fd, EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLET)),
	peer_(),
	buf_(1024),
	status_(0)
{
	ev_->set_close_callback([this] (int)
							{
								LOG_DEBUG << "close callback";
								status_ |= kCLOSE;
							});
	ev_->set_read_callback([this] (int)
						   {
								LOG_DEBUG << "read callback";
								status_ |= kREAD_ABLE;
	   			           });
	ev_->set_write_callback([this] (int)
							{
								LOG_DEBUG << "write callback";
								status_ |= kWRITE_ABLE;
							});
	loop->add_event(ev_.get());
}

tcpconn::~tcpconn()
{
	LOG_DEBUG << "~tcpaddr";
}

tcpaddr& tcpconn::peer()
{
	return peer_;
}

void tcpconn::set_peer_addr(tcpaddr addr)
{
	peer_ = std::move(addr);
}

int tcpconn::fd() const
{
	return ev_->fd();
}

void tcpconn::send(const slice& pslice)
{
	send(pslice.data(), pslice.size());
}

void tcpconn::send(const char* buf, std::size_t len)
{
	if (status_ & kCLOSE)
	{
		return;
	}
	const char* p = buf;
	int nleft = len;
	while (nleft > 0)
	{
		coroutine_yield();
		auto result = __send_some(fd(), p, nleft);
		errcode& err = result.first;
		if (err.code() == errcode::kOK)
		{
			p += result.second;
			nleft -= result.second;
		}
	}
}

void tcpconn::send(const std::string& msg)
{
	send(msg.data(), msg.size());
}

std::pair<errcode, slice> tcpconn::recv()
{
	char* buf = buf_.data();
	std::size_t size = buf_.size();
	int nread = 0;
	while (!(status_ & kREAD_ABLE))
	{
		if (status_ & kCLOSE)
		{
			errcode err(errcode::kEOF, "eof");
			return std::make_pair(err, slice());
		}
		else if (status_ & kERROR)
		{
			errcode err(errcode::kERROR, "error");
			return std::make_pair(err, slice());
		}
		coroutine_yield();
	}
	auto result = __recv_some(fd(), buf, size);
	status_ &= ~kREAD_ABLE;
	if (result.second >= 0)
	{
		nread = result.second;
	}
	else
	{
		return std::make_pair(result.first, slice());
	}
	return std::make_pair(result.first, slice(buf, nread));
}

} // namespace lightswing


