
#include "../lightswing.h"
using namespace lightswing;

void example_chan() {
    typedef typename Chan<int>::Pointer IntChanPtr;
    //Chan可以产生一个brother兄弟， 这两个双胞胎可以互相通讯
    IntChanPtr chan = make_chan<int>();
    IntChanPtr brother = chan->brother();
    //启动两个协程，一个收一个发
    go ([brother] () {
        while (true) {
            int recv = 0;
            int err = brother->recv(recv);
            if (err < 0) {
                LOG_INFO<<"兄弟已经析构，无法从对方接收";
                return;
            }
            LOG_INFO<<"recv:"<<recv;
            if (recv == 3) {
                LOG_INFO<<"quit";
                return;
            }
        }
    });
    go ([chan] () {
        for (int i = 0; i <= 5; ++i) {
            int err = chan->send(i);
            if (err < 0) {
                 LOG_INFO<<"兄弟已经析构，无法发送给对方";
                return;
            }
            //睡眠
            cosleep(3);
        }
    });

}


//请改成int main ()函数
void chan_main () {
    Runtime* runtime = Runtime::instance();
    //设置核心数量
    runtime->set_max_procs(3);
    //启动main
    runtime->start(example_chan);
}
