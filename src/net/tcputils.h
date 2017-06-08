#include <cstddef>
#include "net.h"

namespace lightswing {

class Acceptor;
class TcpConn;
struct TcpAddr;

std::pair<ErrorCode, int>
__send_some(int fd, const char* buf, std::size_t len);

std::pair<ErrorCode, int>
__recv_some(int fd, char* buf, std::size_t len);



}
