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
/// @file tcputils.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include <unistd.h>
#include <errno.h>
#include "tcputils.h"

namespace lightswing
{
std::pair<errcode, int>
__send_some(int fd, const char* buf, std::size_t len)
{
    int nsend = 0, n = 0;
    while (1)
    {
        n = ::write(fd, buf + nsend, len - nsend);
        if (n <= 0)
        {
            if (errno == EAGAIN)
            {
                return std::make_pair(errcode(errcode::kOK, "eagain"), nsend);
            }
            else
            {
                return std::make_pair(errcode(errno), nsend);
            }
        }
        nsend += n;
        if (static_cast<std::size_t>(nsend) == len)
        {
            break;
        }
    }
    return std::make_pair(errcode(errcode::kOK, "ok"), nsend);
}

std::pair<errcode, int>
__recv_some(int fd, char* buf, std::size_t len)
{
    int nread = 0, n = 0, buf_length = len;
    while (1)
    {
        n = ::read(fd, buf + nread, buf_length - nread);
        if (n <= 0)
        {
            if (n == 0)
            {
                return std::make_pair(errcode(errcode::kEOF, "eof"), nread);
            }

            if (errno == EAGAIN)
            {
                return std::make_pair(errcode(errcode::kOK, "eagain"), nread);
            }
            else
            {
                return std::make_pair(errcode(errno), nread);
            }
            break;
        }
        nread += n;
        if (nread == buf_length)
        {
            break;
        }
    }
    return std::make_pair(errcode(errcode::kOK, "ok"), nread);
}

} // namespace lightswing
