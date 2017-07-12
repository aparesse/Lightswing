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
/// @file central.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include "central.h"

namespace lightswing
{
central::central() :
    mutex_(),
    level_(-1),
    list_()
{
}

void central::init(std::size_t level)
{
    level_ = level;
}

void central::deallocate(void* p)
{
    std::lock_guard<std::mutex> lock(mutex_);
    list_.push_back((char*)p);
}

void* central::allocate()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (list_.empty())
    {
        __ajust_list(kALLOC_BYTES / central_size());
    }
    void* result = (void*)list_.back();
    list_.pop_back();
    return result;
}

std::vector<central::pointer> central::allocate(std::size_t n)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (list_.size() < n)
    {
        __ajust_list(n);
    }
    std::vector<pointer> result;
    result.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        result.push_back(list_.back());
        list_.pop_back();
    }
    return result;
}

bool central::empty() const 
{
    std::lock_guard<std::mutex> lock(mutex_);
    return list_.empty();
}

std::size_t central::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return list_.size();
}

std::size_t central::index(std::size_t bytes)
{
    return ((bytes + kBYTE_ALIGN - 1) / kBYTE_ALIGN) - 1;
}

std::size_t central::round_up(std::size_t bytes)
{
    return (bytes + kBYTE_ALIGN - 1) & ~(kBYTE_ALIGN - 1);
}

void central::__ajust_list(std::size_t n )
{
    memoryheap* heap = memoryheap::instance();
    char* p = (char*)heap->allocate(kALLOC_BYTES);
    for (std::size_t i = 0; i < n; ++i)
    {
        list_.push_back(p);
        p += central_size();
    }
}

std::size_t central::central_size() const
{
    return (level_ + 1) * kBYTE_ALIGN;
}

} // namespace lightswing
