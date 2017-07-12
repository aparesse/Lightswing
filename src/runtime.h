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
/// @file runtime.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-11

#ifndef RUNTIME_H
#define RUNTIME_H
#include <cstddef>
#include <cassert>
#include <functional>
#include "singleton.h"
#include "schedule.h"
#include "threadpool.h"
#include "logger.h"
#include "event/eventmanager.h"
#include "coroutine.h"

namespace lightswing
{

class runtime
{
public:
    friend class threadcontext;
    friend class goroutine;
    friend void coroutine_yield();

public:
    runtime();
    ~runtime();

    static runtime* instance();

    void on_event(const std::string& ev, eventmanager::func fn);
    void emit_event(const std::string& ev) const;

    coroutine::pointer new_coroutine(std::function<void()> fn);
    void set_max_procs(std::size_t num);
    std::size_t max_procs() const;

    template<typename...Arg>
    void start(Arg&&... arg);

    void set_loop_interval(std::size_t interval);

    eventmanager& event_manager();

    int get_thread_id() const;

    void yield();

private:
    runtime& operator=(const runtime& other) = delete;
    runtime(const runtime& other) = delete;

private:
    std::size_t core_num_;
    schedule schedule_;
    threadpool threadpool_;
    eventmanager event_manager_;

};

template<typename... Arg>
void runtime::start(Arg&&... arg)
{
    assert(!threadpool_.is_running());
    schedule_.init(core_num_);
    threadpool_.start(core_num_);
    auto fn = std::bind(std::forward<Arg>(arg)...);
    new_coroutine(fn);
    while (!schedule_.empty())
    {
        event_manager_.loop();
    }
}

} // namespace lighswing
#endif // RUNTIME_H
