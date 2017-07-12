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
/// @file blocking_queue.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

namespace lightswing
{

const std::size_t kDEFAULT_BLKQUEUE_SIZE = -1;

template<class T>
class blockingqueue
{
public:
    blockingqueue(std::size_t size) :
        queue_(),
        mutex_(),
        cond_(),
        max_size_(size)
    {
    }

    blockingqueue() :
        queue_(),
        mutex_(),
        cond_(),
        max_size_(kDEFAULT_BLKQUEUE_SIZE)
    {
    }

    int push(T elem)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= max_size_)
        {
            return -1;
        }
        queue_.push(std::move(elem));
        cond_.notify_one();
        return 0;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] ()
                         { 
                            return !queue_.empty();
                         });
        T elem = std::move(queue_.front());
        queue_.pop();
        return elem;
    }

private:
    blockingqueue(const blockingqueue& other) = delete;
    blockingqueue operator=(const blockingqueue& other) = delete;
    // delete move constructor
    blockingqueue(blockingqueue&& other) = delete;

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::size_t max_size_;
};
}
#endif // BLOCKING_QUEUE_H
