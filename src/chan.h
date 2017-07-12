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
/// @file chan.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-12

#ifndef CHAN_H
#define CHAN_H

#include "co_queue.h"
namespace lightswing
{

template<class T>
class chan : public std::enable_shared_from_this<chan<T>>
{
public:
    typedef T valuetype;
    typedef std::shared_ptr<chan<valuetype>> pointer;
    typedef std::weak_ptr<chan<valuetype>> weakpointer;
    typedef coqueue<valuetype> chanqueue;
    typedef std::shared_ptr<chanqueue> queuepointer;

    template<typename _Tp, typename... _Args>
    friend std::shared_ptr<_Tp> make_shared(_Args&&... __args);

public:
    chan();
    chan(queuepointer, queuepointer, pointer);

    // use move
    int send(T value);
    int recv(T&);

    pointer brother();

private:
    queuepointer recv_queue_;
    queuepointer send_queue_;
    weakpointer brother_;

};

// template initialization declaretion
typedef typename chan<int>::pointer intchan;
typedef typename chan<std::string>::pointer stringchan;
typedef typename chan<slice>::pointer slicechan;

// template initialization definition
template<class Type>
static typename chan<Type>::pointer make_chan()
{
    return std::make_shared<chan<Type>>();
}

template<class T>
inline chan<T>::chan() :
    recv_queue_(std::make_shared<chanqueue>()),
    send_queue_(std::make_shared<chanqueue>()),
    brother_()
{
}

template<class T>
inline chan<T>::chan(queuepointer recv_queue, queuepointer send_queue, pointer brother) :
    recv_queue_(recv_queue),
    send_queue_(send_queue),
    brother_(brother)
{
}

template<class T>
inline int chan<T>::send(T value)
{
    auto brother = brother_.lock();
    if (!brother)
    {
        return -1;
    }
    send_queue_->push(std::move(value));
    return 0;
}

template<class T>
inline int chan<T>::recv(T& value)
{
    auto brother = brother_.lock();
    if (!brother && recv_queue_->empty())
    {
        return -1;
    }
    value = recv_queue_->pop();
    return 0;
}


template<class T>
inline typename chan<T>::pointer chan<T>::brother()
{
    std::shared_ptr<chan<T>> myself = this->shared_from_this();
    pointer brother = std::make_shared<chan<T>>(send_queue_,
                                                recv_queue_,
                                                myself);
    brother_ = brother;
    return brother;
}

} // namespace lightswing
#endif // CHAN_H
