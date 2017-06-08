#ifndef NET_H
#define NET_H

#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <errno.h>
namespace lightswing {
//把用于socket设置为非阻塞方式
void setnonblocking(int sockfd);

int create_non_blocking_socket();

class ErrorCode {
public:
    const static int kOK = -1;
    const static int kERROR = -2;
    const static int kEOF = -3;
    ErrorCode(int code, std::string msg) : code_(code), msg_(std::move(msg)) {}
    ErrorCode(int errno_code) : code_(errno_code), msg_(::strerror(errno_code)) {}
    int code() const { return code_; }
    std::string msg() const { return msg_; }
private:
    int code_;
    std::string msg_;
};


}

#endif // NET_H
