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
/// @file blocking_set.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-15

#ifndef BLOCKING_SET_H
#define BLOCKING_SET_H

#include <mutex>
#include <condition_variable>
#include <set>

namespace lightswing
{
const std::size_t kDEFAULT_BLKSET_SIZE = -1;

template<class T, class Compare = std::less<T>>
class blockingset
{
public:
    blockingset(std::size_t size) :
        set_(),
        mutex_(),
        cond_(),
        max_size_(size)
    {
    }

    blockingset() :
        set_(),
        mutex_(),
        cond_(),
        max_size_(kDEFAULT_BLKSET_SIZE)
    {
    }

    int insert(T elem) 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (set_.size() >= max_size_)
          return -1;
        set_.insert(std::move(elem));
        cond_.notify_one();
        return 0;
    }

    bool empty() const 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return set_.empty();
    }

    int count(const T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !set_.empty(); });
        return set_.count(value);
    }

    int erase(const T& value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto iter = set_.find(value);
        if (iter == set_.end())
        {
            return -1;
        }
        else
        {
            set_.erase(iter);
            return 0;
        }
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return set_.size();
    }

private:
    blockingset(const blockingset& other) = delete;
    blockingset& operator=(const blockingset& other) = delete;

private:
    std::set<T, Compare> set_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::size_t max_size_;

};

} // namespace lightswing

#endif // BLOCKING_SET_H
