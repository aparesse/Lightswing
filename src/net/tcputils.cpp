#include "tcputils.h"

namespace lightswing {

std::pair<ErrorCode,int> __send_some(int fd, const char *buf, std::size_t len) {
    int nsend = 0;
    int n = 0;
    while (1) {
        n = ::write(fd, buf + nsend, len - nsend);
        if (n <= 0) {
            if (errno == EAGAIN){
                return std::make_pair(ErrorCode(ErrorCode::kOK, "eagain"), nsend);
            } else {
                return std::make_pair(ErrorCode(errno), nsend);
            }
            break;
        }
        nsend += n;
        if (static_cast<std::size_t>(nsend) == len) {
            break;
        }
    }
    return std::make_pair(ErrorCode(ErrorCode::kOK, "ok"), nsend);
}

std::pair<ErrorCode, int> __recv_some(int fd, char *buf, std::size_t lenofbuf) {
    int len = lenofbuf;
    int nread = 0;
    int n = 0;
    while (true) {
        n = ::read(fd, buf + nread, len - nread);
        if (n <= 0) {
            if (n == 0) {
                return std::make_pair(ErrorCode(ErrorCode::kEOF, "eof"), nread);
            }
            if (errno == EAGAIN){
                return std::make_pair(ErrorCode(ErrorCode::kOK, "eagain"), nread);
            } else {
                return std::make_pair(ErrorCode(errno), nread);
            }
            break;
        }
        nread += n;
        if (nread == len) {
            break;
        }
    }
    return std::make_pair(ErrorCode(ErrorCode::kOK, "ok"), nread);
}

}
