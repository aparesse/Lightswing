#include "net.h"

namespace lightswing {

//把用于socket设置为非阻塞方式
void setnonblocking(int sockfd) {
    int opts;
    opts = ::fcntl(sockfd, F_GETFL);
    if (opts < 0) {
        //perror("fcntl(sock, GETFL)");
        return;
    }
    opts = opts | O_NONBLOCK;
    if (::fcntl(sockfd, F_SETFL, opts) < 0) {
        //::perror("fcntl(sock,SETFL,opts)");
        return;
    }
}

int create_non_blocking_socket(){
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    //assert(socket > 0);
    setnonblocking(sockfd);
    return sockfd;
}

}
