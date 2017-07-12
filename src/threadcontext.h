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
/// @file threadcontext.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef THREADCONTEXT_H
#define THREADCONTEXT_H
#include <thread>
#include <ucontext.h>
#include "coroutine.h"

namespace lightswing
{
class threadpool;

class threadcontext
{
    friend void e_run(uint32_t low32, uint32_t hi32);
    friend class coroutine;

public:
    threadcontext(int id, threadpool* pool);
    void start();
    coroutine::pointer running_coroutine();
    int id() const;

private:
    void resume(coroutine::pointer co);

private:
    int id_;
    threadpool* pool_;
    ucontext ctx_;
    coroutine::weakpointer running_coroutine_;
};

void e_run(uint32_t low32, uint32_t hi32);

} // namespace lightswing


#endif // THREADCONTEXT_H
