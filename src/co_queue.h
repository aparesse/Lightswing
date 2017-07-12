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
/// @file co_queue.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef CO_QUEUE_H
#define CO_QUEUE_H

#include <queue>
#include <memory>
#include "util/slice.h"
#include "co_mutex.h"
#include "util/blocking_queue.h"

namespace lightswing
{

template<class T>
class coqueue
{
public:
    coqueue(std::size_t size) :
        queue_(),
        comutex_(),
        max_size_(size)
    {
    }

    coqueue() :
        queue_(),
        comutex_(),
        max_size_(kDEFAULT_BLKQUEUE_SIZE)
    {
    }

    int push(T elem)
    {
        mutexguard lock(comutex_);
        if (queue_.size() >= max_size_)
        {
            return -1;
        }
        queue_.push(std::move(elem));
        return 0;
    }

    T pop()
    {
        comutex_.lock();
        while (queue_.empty()) 
        {
            comutex_.unlock();
            coroutine_yield();
            comutex_.lock();
        }

        T value = std::move(queue_.front());
        queue_.pop();
        comutex_.unlock();
        return value;
    }

    bool empty() const
    {
        mutexguard lock(comutex_);
        return queue_.empty();
    }

    std::size_t size() const
    {
        mutexguard lock(comutex_);
        return queue_.size();
    }
private:
    coqueue(const coqueue& other) = delete;
    coqueue& operator=(const coqueue& ohter) = delete;

private:
    std::queue<T> queue_;
    mutable comutex comutex_;
    std::size_t max_size_;
};

} //namespace lightswing

#endif // CO_QUEUE_H
