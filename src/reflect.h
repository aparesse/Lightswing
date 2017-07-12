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
/// @file reflect.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#ifndef REFLECT_H
#define REFLECT_H

#include <map>
#include <string>
#include "util/any.h"
#include "singleton.h"

namespace lightswing
{

class reflect
{
public:
    reflect();

    static reflect* instance();

    template<typename T>
    void set(const std::string& name, T* value);

    any get(const std::string& name) const;

private:
    reflect& operator=(const reflect&) = delete;
    reflect(const reflect&) = delete;

private:
    std::map<std::string, any> objects_;
};

inline reflect::reflect() :
    objects_()
{
}

reflect* reflect::instance()
{
    return singleton<reflect>::instance();
}

inline any reflect::get(const std::string& name) const
{
    auto iter = objects_.find(name);
    if (iter == objects_.end())
      return any();
    return iter->second;
}

template<typename T>
inline void reflect::set(const std::string& name, T* value)
{
    objects_[name] = value;
}

inline any get_reflect(const std::string& name)
{
    reflect* refl = reflect::instance();
    return refl->get(name);
}

template<typename T>
inline int get_reflect(const std::string& name, T** result)
{
    assert(result);
    assert(*result);
    any obj = get_reflect(name);
    if (typeid(T*) == obj.type())
    {
        T* pointer = any_cast<T*>(obj);
        *result = pointer;
        return 0;
    }
    return -1;
}

#define REG_REFLECT(s)                                      \
do                                                          \
{                                                           \
    reflect* t_reflect = reflect::intance();                \
    t_reflect->set(#s, &s);                                 \
}                                                           \
while (0)                                                    \

} // namespace lightswing

#endif // REFLECT_H
