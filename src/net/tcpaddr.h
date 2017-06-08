#ifndef TCPADDR_INC
#define TCPADDR_INC
#include <string>

namespace lightswing {
const std::size_t TCP_DEFAULT_ADDR_PORT = std::string::npos;

struct TcpAddr {
public:
    explicit TcpAddr(const std::string& ip, std::size_t portarg = TCP_DEFAULT_ADDR_PORT) :
        ip(ip), port(portarg) {}

    TcpAddr() : ip(), port(TCP_DEFAULT_ADDR_PORT) {}

    bool operator<(const TcpAddr& other) const ;

    bool operator>(const TcpAddr& other) const ;

    bool operator==(const TcpAddr& other) const;

public:
    std::string ip;

    std::size_t port;
};

inline bool TcpAddr::operator<(const TcpAddr &other) const {
    if (ip > other.ip) {
        return true;
    }
    if (port > other.port) {
        return true;
    }
    return false;
}

inline bool TcpAddr::operator>(const TcpAddr &other) const {
    if (other == *this) {
        return false;
    }
    return !operator<(other);
}

inline bool TcpAddr::operator==(const TcpAddr &other) const {
    return other.ip == ip && other.port == port;
}


}
#endif // TCPADDR_INC

