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
/// @file event.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include "../lightswing.h"

using namespace lightswing;

void emit_func(int times)
{
	for (int i = 0; i < times; ++i)
	{
		LOG_INFO << "emit a happy event signal";
		coroutine_emit_event("happy");
		coroutine_sleep(3);
	}
}

void example_event()
{
	// register signal and handler-function
	coroutine_on_event("happy", [] ()
				                {
									LOG_INFO << "recv a happy signal";
								});

	// new a go-coroutine
	go(emit_func, 3);
}

int event_main()
{
	runtime* t_runtime = runtime::instance();

	t_runtime->set_max_procs(3);

	t_runtime->start(example_event);
	return 0;
}

