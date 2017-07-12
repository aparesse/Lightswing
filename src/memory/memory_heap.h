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
/// @file memory_heap.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H

#include <stdlib.h>
#include <mutex>
#include "../singleton.h"

namespace lightswing
{
class memoryheap
{
public:
    static const int kDEFAULT_SIZE = 1024;

public:
    memoryheap();
    ~memoryheap();

    static memoryheap* instance();
    void* allocate(std::size_t size);
    void deallocate(void* p, std::size_t size);

private:
    std::mutex mutex_;
    std::size_t alloc_size_;

};

} // namespace lightswing

#endif // MEMORY_HEAP_H
