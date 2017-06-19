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
/// @file schedule.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#include <cassert>
#include <string>
#include <algorithm>
#include "logger.h"
#include "schedule.h"

namespace lightswing
{
schedule::schedule() :
	max_id_(0)
{

}

schedule::~schedule()
{

}

void schedule::init(int thread_num)
{
	coroutine_vec_ = std::vector<co_queue>(thread_num);
}

coroutine::pointer schedule::new_coroutine(func fn)
{
	std::lock_guard<std::mutex> lock(mutex_);
	coroutine::pointer co = coroutine::create();
	assert(co);
	// schedule is coroutine's friend class
	co->fn_ = std::move(fn);
	co->id_ = max_id_++;
	co->status_ = coroutine::eREADY;
	std::size_t index = co->id_ % coroutine_vec_.size();
	co_queue& q = coroutine_vec_[index];
	q.push(co);
	++size_;
	return co;
}

void schedule::push_coroutine(int index, coroutine::pointer co)
{
	co_queue& q = coroutine_vec_[index];
	q.push(co);
}

coroutine::pointer schedule::pop_coroutine(int index)
{
	co_queue& q = coroutine_vec_[index];
	return q.pop();
}

std::size_t schedule::size() const
{
	return size_;
}

bool schedule::empty() const
{
	return size_ == 0;
}

// TODO
int schedule::remove(coroutine::pointer co)
{
	assert(co);
	--size_;
	return 0;
}
} // namespace lightswing
