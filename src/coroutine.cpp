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
/// @file coroutine.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-11

#include <iostream>
#include <assert.h>
#include <string>
#include "schedule.h"
#include "coroutine.h"
#include "threadcontext.h"

namespace lightswing
{

coroutine::coroutine() :
    stack_(kSTACK_MAX_SIZE),
    fn_(),
    status_(eDEAD),
    id_(-1)
{
    
}

void coroutine::yield()
{
    assert(status_ == coroutine::eRUNNING);
    status_ = coroutine::eSUSPEND;
    swapcontext(&ctx_, ctx_.uc_link);
}

//void coroutine::resume()
//{
//  schedule_ = resume(shared_from_this());
//}

void coroutine::swap_context(ucontext* context)
{
    swapcontext(&ctx_, context);
}

void coroutine::set_func(func fn)
{
    fn_ = std::move(fn);
}

coroutine::pointer coroutine::create()
{
    return std::make_shared<coroutine>();
}

ucontext_t* coroutine::context()
{
    return &ctx_;
}

std::size_t coroutine::stack_size() const
{
    return stack_.size();
}

coroutine::estatus coroutine::status() const
{
    return status_;
}

void coroutine::set_status(coroutine::estatus status)
{
    status_ = status;
}

char* coroutine::stack_top()
{
    return &*stack_.begin();
}

void coroutine::save_stack()
{
    
}

}

