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
/// @file cache.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cassert>
#include "central.h"

namespace lightswing
{
class cache
{
    typedef std::vector<char*> memorylist;
public:
    cache();
    void deallocate(void* p, std::size_t size);
    void* allocate(std::size_t size);
    void expansion(const memorylist& ptrs, std::size_t level);

private:
    std::vector<memorylist> freelists_;
};

}

#endif // CACHE_H
