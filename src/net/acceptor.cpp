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
/// @file acceptor.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-17

#include "acceptor.h"
#include "net.h"

namespace lightswing
{

acceptor::acceptor(int port) :
    new_conn_flag_(false),
    new_conn_(nullptr),
    port_(port),
    listenfd_(create_non_blocking_socket()),
    loop_(runtime::instance()->event_manager().the_eventloop()),
    ev_(epollevent::create(loop_, listenfd_, EPOLLIN | EPOLLET))
{
    ev_->set_read_callback([this] (int fd)
                           {
                                new_conn_ = do_accept(fd);
                                new_conn_flag_ = true;
                           });
    loop_->add_event(ev_.get());
    listen();
}

acceptor::~acceptor()
{
    ::close(listenfd_);
}

tcpconn::pointer acceptor::accept()
{
    while (!new_conn_flag_)
    {
        coroutine_yield();
    }
    tcpconn::pointer result = new_conn_;
    new_conn_ = nullptr;
    new_conn_flag_ = false;
    return result;
}

tcpconn::pointer acceptor::do_accept(int fd)
{
    sockaddr_in clientaddr;
    size_t clilen = sizeof(clientaddr);
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    int connfd = ::accept(fd, (sockaddr*)&clientaddr, (unsigned int*)&clilen);
    if (connfd < 0)
    {
        LOG_DEBUG << "accept error: " << ::strerror(errno);
        return nullptr;
    }
    tcpaddr tcp_addr(::inet_ntoa(clientaddr.sin_addr), ::ntohs(clientaddr.sin_port));
    set_non_blocking(connfd);
    tcpconn::pointer conn = tcpconn::create(ev_->loop(), connfd);
    conn->set_peer_addr(std::move(tcp_addr));
    return conn;
}

const int kLISTENFQ = 10;
void acceptor::listen() 
{
    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY; 
    serveraddr.sin_port = ::htons(port_); // or htons(SERV_PORT)

    int on = 1;
    int ret;
    ret = ::setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ret = ::bind(listenfd_, (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (ret < 0)
    {
        LOG_DEBUG << "bind error: " << ::strerror(errno);
        return;
    }
    ret = ::listen(listenfd_, kLISTENFQ);
    if (ret < 0)
    {
        LOG_DEBUG << "listen error: " << ::strerror(errno);
        return;
    }
}

} // namespace lightswing
