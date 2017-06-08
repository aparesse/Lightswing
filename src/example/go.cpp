#include "../lightswing.h"
using namespace lightswing;

void go_func(int a, int b, int c) {
    LOG_INFO<<"a:" <<a <<", b:" <<b <<", c:" <<c;
}

void example_go() {
    //go支持可变参数
    go(go_func, 66,22,33);
    //支持各种lambda表达式
    go([] () {
        for (int i = 0; i < 10; ++i) {
            LOG_INFO<<"hello :"<<i;
            //让出时间片
            yield();
        }
    });
}

//请改成int main ()函数
void go_main () {
    Runtime* runtime = Runtime::instance();
    //设置核心数量
    runtime->set_max_procs(3);
    //启动main
    runtime->start(example_go);
}
