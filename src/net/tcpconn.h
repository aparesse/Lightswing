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
/// @file tcpconn.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-10

#ifndef TCPCONN_H
#define TCPCONN_H

#include <memory>
#include <atomic>
#include "tcputils.h"
#include "tcpaddr.h"
#include "../event/epollevent.h"
#include "../util/slice.h"
#include "../exec_coroutine.h"

namespace lightswing
{

class tcpconn
{
public:
	const static int kREAD_ABLE   = 1;
	const static int kWRITE_ABLE  = 2;
	const static int kERROR       = 4;
	const static int kCLOSE       = 8;

	typedef std::shared_ptr<tcpconn> pointer;

public:
	tcpconn(eventloop* loop, int fd);
	~tcpconn();

	static pointer create(eventloop* loop, int fd)
	{
		return std::make_shared<tcpconn>(loop, fd);
	}

	tcpaddr& peer();
	// use std::move()
	void set_peer_addr(tcpaddr addr);
	int fd() const;
	void send(const slice& pslice);
	void send(const char* buf, std::size_t len);
	void send(const std::string& msg);

	std::pair<errcode, slice> recv();

private:
	epollevent::pointer ev_;
	tcpaddr peer_;
	std::vector<char> buf_;
	std::atomic<int> status_;
};

} // namespace lightswing

#endif //TCPCONN_H
