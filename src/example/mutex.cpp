#include "../lightswing.h"
using namespace lightswing;




void example_mutex() {
    static CoMutex mutex;
    static int g_int = 0;
    LOG_INFO<<g_int;
    go([] () {
        MutexGuard lock(mutex);
        g_int = 11;
        LOG_INFO<<"静态变量被改成了11";;
    });

    go([] () {
        MutexGuard lock(mutex);
        g_int = 12;
        LOG_INFO<<"静态变量被改成了12";;
    });
}

//请改成int main ()函数
void mutex_main () {
    Runtime* runtime = Runtime::instance();
    //设置核心数量
    runtime->set_max_procs(3);
    //启动main
    runtime->start(example_mutex);
}

