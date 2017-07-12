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
/// @file echo_sever.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include "../lightswing.h"

using namespace lightswing;

void handle_conn(tcpconn::pointer conn)
{
    while (true)
    {
        auto result = conn->recv();
        errcode& err = result.first;
        slice msg = result.second;
        if (!msg.empty())
        {
            conn->send(msg);
        }
        if (err.code() != errcode::kOK)
        {
            LOG_DEBUG << "eof";
            return;
        }
    }
}

void echo_server()
{
    acceptor::pointer t_acceptor = acceptor::create(23333);
    while (true)
    {
        tcpconn::pointer conn = t_acceptor->accept();
        if (conn)
        {
            go(handle_conn, conn);
        }
    }
}

int echo_main()
{
    runtime* t_runtime = runtime::instance();

    // set core numbers
    t_runtime->set_max_procs(3);

    // start
    t_runtime->start(echo_server);
    return 0;
}
