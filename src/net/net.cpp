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
/// @file net.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include "net.h"

namespace lightswing
{
void set_non_blocking(int sockfd)
{
	int opts;
	opts = ::fcntl(sockfd, F_GETFL);
	if (opts < 0)
	{
		return;
	}

	opts = opts | O_NONBLOCK;
	if (::fcntl(sockfd, F_SETFL, opts) < 0)
	{
		return;
	}
}

int create_non_blocking_socket()
{
	int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	set_non_blocking(sockfd);
	return sockfd;
}
}
