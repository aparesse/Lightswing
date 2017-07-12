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
/// @file central.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef CENTRAL_H
#define CENTRAL_H

#include <list>
#include <mutex>
#include <vector>
#include "memory_heap.h"

namespace lightswing
{
class central
{
public:
    static const int kBYTE_ALIGN = 8;
    static const int kBIG_OBJECT_BYTES = 64;
    static const int kALLOC_BYTES = 4 * kBIG_OBJECT_BYTES;
    typedef char* pointer;

public:
    central();
    void init(std::size_t size);
    void deallocate(void* p);
    void* allocate();
    std::vector<pointer> allocate(std::size_t n);
    bool empty() const;
    std::size_t size() const;

    std::size_t central_size() const;

    static std::size_t index(std::size_t bytes);

    static std::size_t round_up(std::size_t bytes);

private:
    void __ajust_list(std::size_t n);

private:
    mutable std::mutex mutex_;
    int level_;
    std::list<pointer> list_;
};

} // lightswing

#endif // CENTRAL_H
