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
/// @file coroutine.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-11

#ifndef COROUTINE_H
#define COROUTINE_H
#include <vector>
#include <ucontext.h>
#include <functional>
#include <set>
#include <memory>
#include "logger.h"

namespace lightswing
{
class threadcontext;
class schedule;
class coroutine : public std::enable_shared_from_this<coroutine>
{
public:
	enum estatus
	{
		eDEAD,
		eREADY,
		eSUSPEND,
		eRUNNING,
		eSIZE
	};

	friend class schedule;
	friend class threadcontext;
	// declared in threadcontext.h
	friend void e_run(uint32_t low32, uint32_t hi32);
	typedef std::shared_ptr<coroutine> pointer;
	typedef std::weak_ptr<coroutine> weakpointer;
	typedef std::function<void(coroutine::pointer)> func;

public:
	coroutine();
	~coroutine() { LOG_INFO << "~coroutine"; }
	int id() const { return this->id_; }
	void yield();
	void swap_context(ucontext* context);
	void set_func(func fn);
	static pointer create();
	ucontext_t* context();
	std::size_t stack_size() const;

	estatus status() const;
	void set_status(estatus status);

private:
	char* stack_top();
	void save_stack();

private:
	std::vector<char> stack_;
	func fn_;
	ucontext_t ctx_;
	estatus status_;
	int id_;
	schedule* schedule_;
};

const int kSTACK_MAX_SIZE = 4 * 1024;

} // namespace lightswing


#endif // COROUTINR_H
