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
/// @file schedule.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <atomic>
#include "coroutine.h"
#include "util/blocking_queue.h"

namespace lightswing
{

const int kSTACK_SIZE = 4 * 1024;

class schedule
{
	friend class coroutine;
public:
	typedef coroutine::func func;
	typedef blockingqueue<coroutine::pointer> co_queue;

public:
	schedule();
	~schedule();

	void init(int thread_num);

	coroutine::pointer new_coroutine(func fn);

	void push_coroutine(int index, coroutine::pointer co);

	coroutine::pointer pop_coroutine(int index);

	std::size_t size() const;
	bool empty() const;
	int remove(coroutine::pointer co);

private:
	schedule& operator=(const schedule& other) = delete;
	schedule(const schedule& other) = delete;

private:
	mutable std::mutex mutex_;
	std::vector<co_queue> coroutine_vec_;
	std::atomic_size_t size_; // atmoic operation
	int max_id_;
};

} // namespace lightswing
#endif // SCHEDULE_H
