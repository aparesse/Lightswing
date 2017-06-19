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
/// @file acceptor.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string>
#include <cerrno>
#include <cstdio>
#include <atomic>
#include <functional>
#include "../runtime.h"
#include "../event/epollevent.h"
#include "tcpconn.h"

namespace lightswing
{

class acceptor
{
public:
	typedef std::shared_ptr<acceptor> pointer;

public:
	acceptor(int port);
	~acceptor();

	static acceptor::pointer create(int port)
	{
		return std::make_shared<acceptor>(port);
	}

	tcpconn::pointer accept();

private:
	tcpconn::pointer do_accept(int fd);
	void listen();

private:
	std::atomic<bool> new_conn_flag_;
	tcpconn::pointer new_conn_;
	int port_;
	int listenfd_;
	eventloop* loop_;
	epollevent::pointer ev_;
};

} // namespace lightswing

#endif // ACCEPTOR_H
