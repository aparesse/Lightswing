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
/// @file memory_pool.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include <cstdlib>
#include "memory_pool.h"
#include "../singleton.h"
#include "../runtime.h"

namespace lightswing
{

memorypool* memorypool::instance()
{
    return singleton<memorypool>::instance();
}

memorypool::memorypool() :
    centrals_(),
    caches_()
{
}

void memorypool::init(std::size_t size)
{
    int n = central::kBIG_OBJECT_BYTES / central::kBYTE_ALIGN;
    centrals_ = std::vector<central>(size);
    for (int i = 0; i < n; ++i)
    {
        centrals_.at(i).init(i);
    }
    caches_.resize(size);
}

void* memorypool::allocate(std::size_t size)
{
    if (size > central::kBIG_OBJECT_BYTES)
    {
        return ::malloc(size);
    }

    int id = runtime::instance()->get_thread_id();
    if (id == -1)
    {
        LOG_DEBUG << "mustn't malloc memory in non-coroutine thread";
        assert(id != -1);
    }
    cache& t_cache = caches_[id];
    void* result = t_cache.allocate(size);
    if (!result)
    {
        std::size_t level = central::index(size);
        central& t_central = centrals_[level];
        auto vec = t_central.allocate(central::kALLOC_BYTES / t_central.central_size());
        t_cache.expansion(vec, level);
        result = t_cache.allocate(size);
    }
    return result;
}

void memorypool::deallocate(void* addr, std::size_t len)
{
    if (len > central::kBIG_OBJECT_BYTES)
    {
        return ::free(addr);
    }

    int id = runtime::instance()->get_thread_id();
    if (id == -1)
    {
        LOG_DEBUG << "mustn't free memory in non-coroutine thread";
        assert(id != -1);
    }
    cache& t_cache = caches_[id];
    len = central::round_up(len);
    t_cache.deallocate(addr, len);
}

void* memorypool::ajust(std::size_t size)
{
    return nullptr;
}

std::size_t memorypool::free_size() const
{
    return 0;
}

} // namespace lightswing
