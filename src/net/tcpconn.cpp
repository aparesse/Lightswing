#include "tcpconn.h"

namespace lightswing {

TcpConn::TcpConn(EventLoop *loop, int fd) :
    ev_(EpollEvent::create(loop, fd, EPOLLIN | EPOLLERR |EPOLLRDHUP | EPOLLET )),
    peer_(),
    buf_(1024),
    status_(0)
{
    ev_->set_close_callback([this] (int) {
        LOG_DEBUG<<"close callback";
        status_ |= kCLOSE;
    });

    ev_->set_read_callback([this] (int) {
        LOG_DEBUG<<"read callback";
        status_ |= kREAD_ABLE;
    });

    ev_->set_error_callback([this] (int) {
        LOG_DEBUG<<"error callback";
        status_ |= kERROR;
    });

    loop->add_event(ev_.get());
}

TcpConn::~TcpConn() {
    LOG_DEBUG<<"~TcpConn";
}

TcpAddr& TcpConn::peer() {
    return peer_;
}

void TcpConn::set_peer_addr(TcpAddr addr) {
    peer_ = std::move(addr);
}

int TcpConn::fd() const {
    return ev_->fd();
}

void TcpConn::send(const Slice &slice) {
    send(slice.data(), slice.size());
}

void TcpConn::send(const char *buf, std::size_t len) {
    if (status_ & kCLOSE) {
        return;
    }
    const char* p = buf;
    int nleft = len;
    while (nleft > 0) {
        yield();
        auto result = __send_some(fd(), p, nleft);
        ErrorCode& err = result.first;
        if (err.code() == ErrorCode::kOK) {
            p += result.second;
            nleft -= result.second;
        }
    }
}

void TcpConn::send(const std::string &msg) {
    send(msg.data(), msg.size());
}

std::pair<ErrorCode, Slice> TcpConn::recv() {
    char * buf = buf_.data();
    std::size_t size = buf_.size();
    int nread = 0;
    while (!(status_ & kREAD_ABLE)) {
        if (status_ & kCLOSE) {
            ErrorCode err(ErrorCode::kEOF, "eof");
            return std::make_pair(err, Slice());
        } else if (status_ & kERROR){
            status_ &= ~kERROR;
            ErrorCode err(ErrorCode::kERROR, "error");
            return std::make_pair(err, Slice());
        }
        yield();
    }
    auto result = __recv_some(fd(), buf, size);
    status_ &= ~kREAD_ABLE;
    if (result.second >= 0) {
        nread = result.second;
    } else {
        return std::make_pair(result.first, Slice());
    }
    return std::make_pair(result.first, Slice(buf, nread));
}

} //namespace
