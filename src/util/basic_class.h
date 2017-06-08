#ifndef BASIC_CLASS_H
#define BASIC_CLASS_H
#include <sstream>
#include <string>
namespace lightswing {

class __IntegerImpl {
public:
    __IntegerImpl(int value) : value_(value) {}

    std::string to_string() const {
        std::stringstream ss;
        ss<<value_;
        return ss.str();
    }
    bool operator==(int value) {
        return value_ == value;
    }


private:
    int value_;
};


} //namespace

#endif // BASIC_CLASS_H

