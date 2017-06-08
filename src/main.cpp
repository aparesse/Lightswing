#include <iostream>
#include "test/dotest.h"
#include "example/example.h"
#include "lightswing.h"
#include "util/object.h"
using namespace std;
using namespace lightswing;


void main_func() {
}

int main () {
    do_example();
//    Runtime* runtime = Runtime::instance();
//    runtime->set_max_procs(1);
//    runtime->start(main_func);
    LOG_INFO<<"hello world";
}



