#ifndef EXAMPLE_H
#define EXAMPLE_H
#include "../lightswing.h"
using namespace lightswing;
void go_main();
void chan_main ();
void event_main();
void mutex_main();
void echo_main();


inline  void do_example() {
    echo_main();
}

#endif // EXAMPLE_H

