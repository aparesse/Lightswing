#ifndef TCPCONN_INC
#define TCPCONN_INC
#include <memory>
#include <atomic>
#include "../event/ep_event.h"
#include "../util/slice.h"
#include "../utils.h"
#include "tcputils.h"
#include "tcpaddr.h"

namespace lightswing {

class TcpConn {
public:
    const static int kREAD_ABLE = 1;
    const static int kWRITE_ABLE = 2;
    const static int kERROR = 4;
    const static int kCLOSE = 8;

    typedef std::shared_ptr<TcpConn> Pointer;

public:
    TcpConn(EventLoop* loop, int fd);
    ~TcpConn();

    static Pointer create(EventLoop* loop, int fd) {
        return std::make_shared<TcpConn>(loop, fd);
    }

    TcpAddr& peer();
    //use std::move()
    void set_peer_addr(TcpAddr addr);
    int fd() const;
    void send(const Slice& slice);
    void send(const char* buf, std::size_t len);
    void send(const std::string& msg);

    std::pair<ErrorCode, Slice> recv();

private:
    EpollEvent::Pointer ev_;
    TcpAddr peer_;
    std::vector<char> buf_;
    std::atomic<int> status_;
};


} //namespace
#endif // TCPCONN_INC

