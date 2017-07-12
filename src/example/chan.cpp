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
/// @file chan.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-06-18

#include "../lightswing.h"

using namespace lightswing;

void example_chan()
{
    typedef typename chan<int>::pointer intchanpointer;

    // chan can generate a brother, they can comunicate with each other
    intchanpointer chan = make_chan<int>();
    intchanpointer brother = chan->brother();

    // start two coroutine to recv and send
    go ([brother] () 
        {
            while (true)
            {
                int recv = 0;
                int err = brother->recv(recv);
                if (err < 0)
                {
                    LOG_INFO << "brother is deconstructed, can't recv anything";
                    return;
                }
                LOG_INFO << "recv: " << recv;
                if (recv == 3)
                {
                    LOG_INFO << "quit";
                    return;
                }
            }
        });

    go ([chan] ()
        {
            for (int i = 0; i <= 5; ++i)
            {
                int err = chan->send(i);
                if (err < 0)
                {
                    LOG_INFO << "brother is deconstructed, can't send anything";
                    return;
                }
                // sleep
                coroutine_sleep(3);
            }
        });
}

int chan_main()
{
    runtime* t_runtime= runtime::instance();
    // set core numbers
    t_runtime->set_max_procs(3);
    t_runtime->start(example_chan);
    return 0;
}
