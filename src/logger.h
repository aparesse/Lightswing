#ifndef LOGGER
#define LOGGER
#include <iostream>
#include <sstream>
namespace lightswing {

class SimpleDebugPrinter {
public:
    SimpleDebugPrinter() :ss_() { }
    ~SimpleDebugPrinter() { std::cout<<ss_.str()<<std::endl; }
    std::stringstream& stream() {return ss_;}
private:
      std::stringstream ss_;
};

#define LOG_DEBUG SimpleDebugPrinter().stream()<<"[LOG_DEBUG]"  \
                                    <<__FILE__ <<" @ "<<  __func__ << "(" <<  __LINE__ <<")"

#define LOG_INFO SimpleDebugPrinter().stream()<<"[LOG_INFO]"


} //namespace

#endif // LOGGER

