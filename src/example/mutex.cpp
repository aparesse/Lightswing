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
/// @file mutex.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include "../lightswing.h"

using namespace lightswing;

void example_mutex()
{
    static comutex mutex;
    static int g_int = 0;
    LOG_INFO << g_int;
    go([] ()
       {
            mutexguard lock(mutex);
            g_int = 11;
            LOG_INFO << "static var is modified to 11";
       });
    go([] ()
       {
            mutexguard lock(mutex);
            g_int = 22;
            LOG_INFO << "static var is modified to 22";
       });

}

void mutex_main()
{
    runtime* t_runtime = runtime::instance();
    t_runtime->set_max_procs(3);
    t_runtime->start(example_mutex);
}
