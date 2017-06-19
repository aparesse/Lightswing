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
/// @file go.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include "../lightswing.h"

using namespace lightswing;

void go_func(int a, int b, int c)
{
	LOG_INFO << "a: " << a << ", b: " << b << ", c: " << c;
}

void example_go()
{
	// go-coroutine supports variable arguments
	go([] ()
	   {
			for (int i = 0; i < 10; ++i)
			{
				LOG_INFO << "hello: " << i;
				
				coroutine_yield();
			}
	   });
}

int go_main()
{
	runtime* t_runtime = runtime::instance();
	t_runtime->set_max_procs(3);
	t_runtime->start(example_go);
	return 0;
}
