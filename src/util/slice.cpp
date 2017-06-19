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
/// @file slice.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-14

#include "slice.h"
#include <algorithm>

namespace lightswing
{

slice::slice() :
	buf_(nullptr),
	start_(nullptr),
	size_(0)
{
}

slice::slice(const char* buf) :
	buf_(nullptr),
	start_(buf),
	size_(std::strlen(buf))
{
}

slice::slice(const char* buf, std::size_t size) :
	buf_(nullptr),
	start_(buf),
	size_(size)
{
}

slice slice::clone() const
{
	slice sli;
	sli.buf_ = std::make_shared<__buffer>(size());
	sli.start_ = sli.buf_->data_;
	sli.size_ = size();
	memcpy(sli.buf_->data_, data(), size());
	return sli;
}

bool slice::empty() const
{
	return size_ == 0 || !start_;
}

void slice::clear()
{
	size_ = 0;
	start_ = nullptr;
}

char slice::at(std::size_t pos) const 
{
	if (pos >= size())
	{
		return '\0';
	}
	return start_[pos];
}

char slice::operator[](std::size_t pos) const
{
	return at(pos);
}

slice slice::operator+(const slice& other)
{
	slice sli;
	std::size_t new_size = size() + other.size();
	sli.buf_ = std::make_shared<__buffer>(new_size);
	sli.start_ = sli.buf_->data_;
	sli.size_ = new_size;
	char* p = sli.buf_->data_;
	memcpy(p, start_, size());
	memcpy(p + size(), other.data(), other.size());
	return sli;
}


std::size_t slice::find(const char* target, std::size_t len, std::size_t pos) const
{
	if (empty() || pos >= size())
	{
		return npos;
	}
	constiter iter = cbegin() + pos;
	while (iter != cend())
	{
		if ((std::size_t)std::distance(iter, cend()) < len)
		{
			return npos;
		}
		if (std::equal(target, target + len, iter))
		{
			return std::distance(begin(), iter);
		}
		++iter;
	}
	return npos;
}

std::size_t slice::find(const std::string& target, std::size_t pos) const
{
	return find(target.data(), target.length(), pos);
}

slice slice::subslice(std::size_t pos, std::size_t len) const 
{
	if (empty() || pos >= size())
	{
		return slice();
	}
	constiter biter = pos + begin();
	len  = std::min(len, static_cast<std::size_t>(std::distance(biter, cend())));
	slice sli;
	sli.buf_ = buf_;
	sli.start_ = biter;
	sli.size_ = len;
	return sli;
}

std::string slice::to_string() const
{
	if (empty())
	{
		return std::string("");
	}
	return std::string(begin(), size());
}


// friend functions
bool operator==(const slice& s, const char* p)
{
	std::size_t plen = std::strlen(p);
	if (plen != s.size())
	{
		return false;
	}
	for (std::size_t i = 0; i < s.size(); ++i, ++p)
	{
		if (*p != s[i])
		{
			return false;
		}
	}
	return true;
}

bool operator==(const char* p, const slice& s)
{
	return operator==(s, p);
}

std::ostream& operator<<(std::ostream& os, const slice& s)
{
	os << s.to_string();
	return os;
}

} // namespace lightswing
