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
/// @file slice.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-14

#ifndef SLICE_H
#define SLICE_H

#include <string>
#include <cstring>
#include <memory>
#include <iostream>

namespace lightswing
{
struct __buffer
{
    __buffer() : data_(nullptr) {}

    __buffer(std::size_t size) : 
        data_(new char[size])
    {
    }

    ~__buffer()
    {
        if (data_)
          delete[] data_;
    }
    char* data_;
};

class slice
{
public:
    typedef const char* constiter;
    friend slice make_slice(const char*);
    static const std::size_t npos = std::string::npos;

public:
    slice();
    explicit slice(const char* buf);
    slice(const char* buf, std::size_t size);

    // clone and maintain itself
    slice clone() const;

    // reference to container's implementation
    friend slice make_ref(const char* str, std::size_t len);
    friend slice make_ref(const char* first, const char* last);

    void clear();
    bool empty() const;
    char operator[](std::size_t pos) const;
    char at(std::size_t pos) const;

    std::size_t size() const { return size_; }
    std::size_t length() const { return size(); }
    constiter begin() const { return start_; }
    constiter end() const { return start_ + size_; }
    constiter cbegin() const { return start_; }
    constiter cend() const { return start_ + size_; }

    std::size_t find(const std::string& target, std::size_t pos = 0) const;
    std::size_t find(const char* target, std::size_t len, std::size_t pos = 0) const;

    slice subslice(std::size_t pos, std::size_t len) const;

    std::string to_string() const;
    const char* data() const { return start_; }

    slice operator+(const slice& other);
    friend std::ostream& operator<<(std::ostream&, const slice&);
    friend bool operator==(const slice& s, const char* p);
    friend bool operator==(const char* p, const slice& s);

    
private:
    std::shared_ptr<__buffer> buf_;
    const char* start_;
    std::size_t size_;
};
} // namespace lightswing

#endif // SLICE_H
