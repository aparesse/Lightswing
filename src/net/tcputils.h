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
/// @file tcputils.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef TCPUTILS_H
#define TCPUTILS_H

#include <cstddef>
#include "../errcode.h"

namespace lightswing
{
class acceptor;
class tcpconn;
class tcpaddr;

std::pair<errcode, int>
__send_some(int fd, const char* buf, std::size_t len);

std::pair<errcode, int>
__recv_some(int fd, char* buf, std::size_t len);

} // namespace lightswing
#endif // TCPUTILS_H
