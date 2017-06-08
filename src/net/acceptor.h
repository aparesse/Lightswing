#ifndef ACCEPTOR_INC
#define ACCEPTOR_INC
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <atomic>
#include <functional>
#include "../runtime.h"
#include "../event/ep_event.h"
#include "tcpconn.h"
namespace lightswing {

class Acceptor : public std::enable_shared_from_this<Acceptor>{
public:
    typedef std::shared_ptr<Acceptor> Pointer;
public:
    Acceptor(int port);
    ~Acceptor();

    static Acceptor::Pointer create(int port) {
        return std::make_shared<Acceptor>(port);
    }

    TcpConn::Pointer accept();

private:
    TcpConn::Pointer doaccept(int fd);
    void listen();

private:
    std::atomic<bool> new_conn_flag_;
    TcpConn::Pointer new_conn_;
    int port_;
    int listenfd_;
    EventLoop* loop_;
    EpollEvent::Pointer ev_;
};


}//namespace
#endif // ACCEPTOR_INC

