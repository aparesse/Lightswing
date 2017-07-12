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
/// @file exec_coroutine.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef EXEC_COROUTINE_H
#define EXEC_COROUTINE_H

#include "runtime.h"
#include "coroutine.h"
#include <ctime>

namespace lightswing
{
// need not lock, because of every thread has their own threadcontext
inline void coroutine_yield()
{
    runtime* pruntime = runtime::instance();
    pruntime->yield();
}

inline std::time_t get_timestamp_now()
{
    std::time_t tt;
    std::time(&tt);
    return tt;
}

inline void coroutine_sleep(std::time_t sec)
{
    std::time_t wait_sec = get_timestamp_now() + sec;
    while (get_timestamp_now() < wait_sec)
    {
        coroutine_yield();
    }
}

template<typename... Arg>
inline void go(Arg&&... arg)
{
    auto fn = std::bind(std::forward<Arg>(arg)...);
    // new_coroutine already locked
    runtime::instance()->new_coroutine(std::move(fn));
}

inline void coroutine_emit_event(const std::string& ev)
{
    // emit_event already locked
    runtime::instance()->emit_event(ev);
}

inline void coroutine_on_event(const std::string& ev, eventmanager::func fn)
{
    // on_event already locked
    runtime::instance()->on_event(ev, std::move(fn));
}

} // namespace lightswing

#endif // EXEC_COROUTINE_H
