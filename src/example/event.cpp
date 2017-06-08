#include "../lightswing.h"
using namespace lightswing;

void emit_func(int times) {
    for (int i = 0; i < times; ++i) {
        LOG_INFO<<"i emit a happy event signal";
        //发送为happy的信号
        emit_event("happy");
        //睡眠3秒
        cosleep(3);
    }
}

void example_event() {
    //注册信号以及handle函数
    on_event("happy", [] () {
        LOG_INFO<<"i recv the happy signal!";
    });

    //启动一个新的协程
    go(emit_func, 3);
}


//请改成int main ()函数
void event_main () {
    Runtime* runtime = Runtime::instance();
    //设置核心数量
    runtime->set_max_procs(3);
    //启动main
    runtime->start(example_event);
}

