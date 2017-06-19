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
/// @file test_slice.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-19

#include "test.h"
#include "../util/slice.h"

namespace test
{

void test_slice()
{
	lightswing::slice sli1("hello world");
	lightswing::slice sli2("good morning");
	lightswing::slice sli3 = sli1 + sli2;
	DEBUG_INFO << sli3;
	EXCEPT_EQ(sli3, "hello world good morning");
}

} // namespace lightswing
