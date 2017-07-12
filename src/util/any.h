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
/// @file any.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-16

#ifndef ANY_H
#define ANY_H

#include <typeinfo>
#include <cassert>

namespace lightswing
{

class __anyplaceholder
{
public:
    virtual ~__anyplaceholder() {}
    virtual __anyplaceholder* clone() const { return nullptr; }
    virtual const std::type_info& type() const { return typeid(void); }

};

template<class T>
class __anyholder : public __anyplaceholder 
{
public:
    __anyholder(const T& value) : value_(value) {}
    virtual ~__anyholder() {}
    virtual __anyplaceholder* clone() const { return new __anyholder<T>(value_); }
    virtual const std::type_info& type() const { return typeid(T); }

    T value_;
};

class any
{
public:
    any() : holder_(nullptr) {}
    any(const any& other) :
        holder_(other.holder_ ? other.holder_->clone(): nullptr)
    {
    }

    any& operator=(const any& other) 
    {
        holder_ = other.holder_ ? other.holder_->clone() : nullptr;
        return *this;
    }

    template<typename T>
    any(const T& value) : 
        holder_(new __anyholder<T>(value))
    {
    }

    any(any&& other)
    {
        holder_ = other.holder_;
        other.holder_ = nullptr;
    }

    ~any()
    {
        if (holder_)
        {
            delete holder_;
        }
    }

    const std::type_info& type() const 
    {
        return holder_ ? holder_->type() : typeid(void);
    }

    template<typename V>
    friend  V any_cast(const any&);

private:
    __anyplaceholder* holder_;
};

template<typename V>
static V any_cast(const any& anyobj)
{
    assert(typeid(V) == anyobj.type());
    __anyholder<V>* holder = dynamic_cast<__anyholder<V>*>(anyobj.holder_);
    return holder->value_;
}

} // namespace lightswing

#endif //ANY_H
