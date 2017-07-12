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
/// @file object.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-15

#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <string>
#include <sstream>
namespace lightswing
{
template<class T>
class object
{

public:
    template<class... Arg>
    object(Arg&&... arg);

    T& get() { return *ptr_; }

    const T& get() const { return *ptr_; }

    object<T> clone() const 
    {
        T* copy = ptr_->clone();
        object<T> obj{__cloneconstruct(), copy};
        return obj;
    }

private:
    struct __cloneconstruct{};
    object(__cloneconstruct copy, T* ptr) : ptr_(ptr) {}
    std::shared_ptr<T> ptr_;

};

template<class T>
template<class... Arg>
inline object<T>::object(Arg&&... arg) :
    ptr_(std::make_shared<T>(std::forward<Arg>(arg)...))
{
}

} // namespace lightswing

#endif
