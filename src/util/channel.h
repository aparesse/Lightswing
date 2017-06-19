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
/// @file channel.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include "blocking_queue.h"

namespace lightswing
{
// multi-threads comunication tool

template<class T>
class channel
{
public:
	typedef T valuetype;
	typedef blockingqueue<valuetype> queuetype;
	typedef std::shared_ptr<queuetype> queuepointer;
	typedef std::shared_ptr<channel> pointer;

public:
	channel();
	channel(std::size_t size);

	int send(const valuetype& value);
	valuetype recv();
	channel<valuetype> brother();
	std::size_t send_size() const;
	std::size_t recv_size() const;

private:
	struct __privateconstruct
	{
	};

	channel(__privateconstruct constructor);

private:
	queuepointer send_queue_;
	queuepointer recv_queue_;

};

const int __BROTHER = 0;

template<class T>
inline channel<T> make_channel()
{
	return channel<T>();
}

template<class T>
inline channel<T>::channel() :
	send_queue_(std::make_shared<queuetype>()),
	recv_queue_(std::make_shared<queuetype>())
{
}

template<class T>
inline channel<T>::channel(std::size_t size) :
	send_queue_(std::make_shared<queuetype>(size)),
	recv_queue_(std::make_shared<queuetype>(size))
{
}

template<class T>
inline int channel<T>::send(const valuetype& value)
{
	return send_queue_->push(value);
}

template<class T>
inline T channel<T>::recv()
{
	return recv_queue_->pop();
}

template<class T>
inline std::size_t channel<T>::send_size() const
{
	return send_queue_->size();
}

template<class T>
inline std::size_t channel<T>::recv_size() const
{
	return recv_queue_->size();
}

template<class T>
inline channel<T> channel<T>::brother()
{
	// call overloaded private constructor
	channel<T> brother_channel{__privateconstruct()};
	brother_channel.recv_queue_ = send_queue_;
	brother_channel.send_queue_ = recv_queue_;
	return brother_channel;
}

template<class T>
inline channel<T>::channel(__privateconstruct constructor) :
	send_queue_(nullptr),
	recv_queue_(nullptr)
{

}

} // namespace lightswing

#endif // CHANNEL_H
