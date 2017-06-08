#include "acceptor.h"
namespace lightswing {

Acceptor::Acceptor(int port) :
    new_conn_flag_(false),
    new_conn_(nullptr),
    port_(port),
    listenfd_(create_non_blocking_socket()),
    loop_(Runtime::instance()->event_manager().eventloop()),
    ev_(EpollEvent::create(loop_, listenfd_, EPOLLIN | EPOLLET))
{
    ev_->set_read_callback([this](int fd) {
        new_conn_ = doaccept(fd);
        new_conn_flag_ = true;
    });
    loop_->add_event(ev_.get());
    listen();
}

Acceptor::~Acceptor() {
    ::close(listenfd_);
}

TcpConn::Pointer Acceptor::accept() {
    while (!new_conn_flag_) {
        yield();
    }
    TcpConn::Pointer result = new_conn_;
    new_conn_ = nullptr;
    new_conn_flag_ = false;
    return result;
}

TcpConn::Pointer Acceptor::doaccept(int fd) {
    sockaddr_in clientaddr;
    size_t clilen = sizeof(clientaddr);
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    int connfd = ::accept(fd, (sockaddr *) &clientaddr, (unsigned int*)&clilen);
    if (connfd < 0) {
        LOG_DEBUG<<"accept error:" <<::strerror(errno);
        return nullptr;
    }
    TcpAddr tcp_addr(::inet_ntoa(clientaddr.sin_addr),
                     ::ntohs(clientaddr.sin_port));
    setnonblocking(connfd);
    TcpConn::Pointer conn =  TcpConn::create(ev_->loop(), connfd);
    conn->set_peer_addr(std::move(tcp_addr));
    return conn;
}

const int kLISTENQ = 10;
void Acceptor::listen() {
    int ret;
    sockaddr_in serveraddr;
    ::memset(&serveraddr, 0, sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY; //服务器IP地址--允许连接到所有本地地址上
    serveraddr.sin_port = ::htons(port_); //或者htons(SERV_PORT);
    int on = 1;
    ret = ::setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    ret = ::bind(listenfd_, (sockaddr *) &serveraddr, sizeof(serveraddr));
    if (ret < 0) {
        LOG_DEBUG<<"bind error:" <<::strerror(errno);
        return;
    }
    ret = ::listen(listenfd_, kLISTENQ);
    if (ret < 0){
        LOG_DEBUG<<"listen error"<<::strerror(errno);
        return;
    }
}
}
