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
/// @file memory_pool.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <vector>
#include <list>
#include "central.h"
#include "cache.h"

namespace lightswing
{
class memorypool
{
	typedef char* pointer;
	typedef std::list<pointer> pointerlist;
public:
	static memorypool* instance();

public:
	memorypool();
	void init(std::size_t size);
	void* allocate(std::size_t size);
	void deallocate(void* addr, std::size_t len);

private:
	void* ajust(std::size_t n);
	std::size_t free_size() const;

	memorypool& operator=(const memorypool&) = delete;
	memorypool(const memorypool&) = delete;

private:
	std::vector<central> centrals_;
	std::vector<cache> caches_;
};

}


#endif // MEMORY_POOL_H
