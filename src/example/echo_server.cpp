#include "../lightswing.h"
using namespace lightswing;

void handle_conn(TcpConn::Pointer conn) {
    while (true) {
        auto result = conn->recv();
        ErrorCode& err = result.first;
        Slice msg = result.second;
        if (!msg.empty()) {
            conn->send(msg);
        }

        if (err.code() != ErrorCode::kOK) {
            //handle condition
            if (err.code() == ErrorCode::kEOF) {
                //关闭连接
                LOG_DEBUG<<"eof";
                return;
            }
        }
    } // while
}

void example_echo_server() {
    Acceptor::Pointer acceptor = Acceptor::create(23333);
    while (true) {
        TcpConn::Pointer conn = acceptor->accept();
        if (conn) {
            go(handle_conn, conn);
        }
    }

}

//请改成int main ()函数
void echo_main () {
    Runtime* runtime = Runtime::instance();
    //设置核心数量
    runtime->set_max_procs(3);
    //启动main
    runtime->start(example_echo_server);
}
