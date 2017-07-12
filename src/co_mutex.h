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
/// @file co_mutex.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef CO_MUTEX_H
#define CO_MUTEX_H

#include <atomic>
#include <mutex>
#include "exec_coroutine.h"
#include "runtime.h"

namespace lightswing
{

class comutex
{
public:
    comutex();
    void lock();
    void unlock();

private:
    void lock_free_aux();
    void lock_aux();

private:
    std::atomic_int contenders_;
    std::mutex mutex_;

};

//TODO: the best way that is checking in compile phrase
inline comutex::comutex() :
    contenders_(0),
    mutex_()
{

}

//TODO: add a move constructor

inline void comutex::lock()
{
    runtime* pruntime = runtime::instance();
    if (pruntime->max_procs() > 1)
    {
        lock_aux();
    }
    else
    {
        lock_free_aux();
    }
}

inline void comutex::unlock()
{
    --contenders_;
}

inline void comutex::lock_aux()
{
    while (true)
    {
        while (contenders_ > 0)
            coroutine_yield();

        if (mutex_.try_lock())
          break;
    }
    ++contenders_;
    mutex_.unlock();
}

inline void comutex::lock_free_aux()
{
    while (contenders_ > 0)
      coroutine_yield();
    ++contenders_;
}

class mutexguard
{
public:
    mutexguard(comutex& comtx);
    ~mutexguard();

private:
    comutex& comutex_;

};

inline mutexguard::mutexguard(comutex& comtx) :
    comutex_(comtx)
{
    comutex_.lock();
}

inline mutexguard::~mutexguard()
{
    comutex_.unlock();
}

} // namespace lightswing

#endif // CO_MUTEX_H
