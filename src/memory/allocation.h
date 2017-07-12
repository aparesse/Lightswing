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
/// @file allocation.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <vector>
#include "memory_pool.h"

namespace lightswing
{
template<class T>
class allocation
{
public:
    static T* allocate();
    static T* allocate(std::size_t n);
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, std::size_t n);
    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void destroy(T* ptr);
    static void destroy(T* first, T* last);

};

template<typename T>
inline T* allocation<T>::allocate()
{
    return allocate(1);
}

template<typename T>
inline T* allocation<T>::allocate(std::size_t n)
{
    memorypool* pool = memorypool::instance();
    return (T*)pool->allocate(sizeof(T) * n);
}

template<typename T>
inline void allocation<T>::deallocate(T* ptr)
{
    deallocate(ptr, 1);
}

template<typename T>
inline void allocation<T>::deallocate(T* ptr, std::size_t n)
{
    memorypool* pool = memorypool::instance();
    pool->deallocate(ptr, sizeof(T) * n);
}

template<typename T>
inline void allocation<T>::construct(T* ptr)
{
    ::new ((void*)ptr) T();
}

template<typename T>
inline void allocation<T>::construct(T* ptr, const T& value)
{
    ::new ((void*)ptr) T(value);
}

template<typename T>
inline void allocation<T>::destroy(T* ptr)
{
    ptr->~T();
}

template<typename T>
inline void allocation<T>::destroy(T* first, T* last)
{
    for ( ; first != last; ++first)
    {
        first->~T();
    }
}

} // namespace lightswing

#endif // ALLOCATION_H
