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
/// @file threadpool.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <map>
#include <cassert>
#include "logger.h"
#include "threadcontext.h"

namespace lightswing
{
class threadpool
{
public:
	typedef std::function<void()> func;
	typedef std::map<std::thread::id, threadcontext*> threadmap;

public:
	threadpool();
	void set_func(func fn);
	void stop();
	void start(std::size_t size);
	std::size_t size() const;
	bool is_running() const;
	const threadmap& thread_map() const;
	threadmap& thread_map();

private:
	threadpool& operator=(const threadpool&) = delete;
	threadpool(const threadpool&) = delete;

private:
	std::size_t size_;
	std::atomic_bool quit_;
	func thread_loop_fn_;
	std::vector<threadcontext> thread_context_;
	threadmap thread_map_;
};

const int kDEFAULT_THREAD_NUM = 0;

inline threadpool::threadpool() :
	size_(0),
	quit_(true),
	thread_loop_fn_(),
	thread_context_(),
	thread_map_()
{

}

inline void threadpool::set_func(func fn)
{
	thread_loop_fn_ = fn;
}

inline void threadpool::start(std::size_t size)
{
	quit_ = false;
	thread_context_.reserve(size);
	for (std::size_t i = 0; i < size; ++i)
	  thread_context_.push_back(threadcontext(i, this));

	for(std::size_t i = 0; i < size; ++i)
	  thread_context_[i].start();
}

inline void threadpool::stop()
{

}

inline std::size_t threadpool::size() const
{
	return size_;
}

inline bool threadpool::is_running() const
{
	return !quit_;
}

inline const threadpool::threadmap& threadpool::thread_map() const 
{
	return thread_map_;
}

inline threadpool::threadmap& threadpool::thread_map()
{
	return thread_map_;
}

} // namespace lightswing
#endif // THREADPOOL_H
