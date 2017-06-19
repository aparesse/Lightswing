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
/// @file runtime.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#include "runtime.h"

namespace lightswing
{
runtime::runtime() :
	core_num_(1),
	schedule_(),
	threadpool_(),
	event_manager_()
{
	LOG_INFO << "runtime begin";	
}

runtime* runtime::instance()
{
	return singleton<runtime>::instance();
}

runtime::~runtime()
{
	LOG_INFO << "runtime end";
}

void runtime::set_max_procs(std::size_t num)
{
	assert(num > 0);
	core_num_ = num;
}

std::size_t runtime::max_procs() const
{
	return core_num_;
}

eventmanager& runtime::event_manager()
{
	return event_manager_;
}

int runtime::get_thread_id() const
{
	const threadpool::threadmap& m = threadpool_.thread_map();
	auto iter = m.find(std::this_thread::get_id());
	if (iter == m.end())
	  return -1;
	return iter->second->id();
}

void runtime::yield()
{
	threadpool::threadmap& m = threadpool_.thread_map();
	auto iter = m.find(std::this_thread::get_id());
	if (iter == m.end())
	{
		LOG_DEBUG << "coroutine run out of thread forbidden";
		return;
	}
	iter->second->running_coroutine()->yield();
}

void runtime::on_event(const std::string& ev, eventmanager::func fn)
{
	event_manager_.register_event(ev, std::move(fn));
}

void runtime::emit_event(const std::string& ev) const
{
	event_manager_.emit_event(ev);
}

coroutine::pointer runtime::new_coroutine(std::function<void()> fn)
{
	return schedule_.new_coroutine([fn] (coroutine::pointer) { fn(); });
}

}
