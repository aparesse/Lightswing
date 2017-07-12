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
/// @file memory_heap.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include "memory_heap.h"

namespace lightswing
{
memoryheap::memoryheap() :
    mutex_(),
    alloc_size_(kDEFAULT_SIZE)
{

}

memoryheap::~memoryheap()
{

}

void* memoryheap::allocate(std::size_t size)
{
    return malloc(size);
}

void memoryheap::deallocate(void* p, std::size_t size)
{
    free(p);
}

memoryheap* memoryheap::instance()
{
    return singleton<memoryheap>::instance();
}

}
