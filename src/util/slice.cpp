#include "slice.h"
#include <algorithm>
namespace lightswing {
Slice::Slice() :
    buf_(nullptr),
    start_(nullptr),
    size_(0)
{

}

Slice::Slice(const char *buf):
    buf_(nullptr),
    start_(buf),
    size_(std::strlen(buf))
{
}

Slice::Slice(const char *buf, std::size_t size) :
    buf_(nullptr),
    start_(buf),
    size_(size) {
}

Slice Slice::clone() const {
    Slice slice;
    slice.buf_ = std::make_shared<__Buffer>(size());
    slice.start_ = slice.buf_->data;
    slice.size_ = size();
    memcpy(slice.buf_->data, data(), size());
    return slice;
}

bool Slice::empty() const {
    return size_ == 0 || !start_;
}

char Slice::operator[](std::size_t pos) const {
    return at(pos);
}

char Slice::at(std::size_t pos) const {
    if (pos >= size()) {
        return '\0';
    }
    return start_[pos];
}

Slice Slice::operator+(const Slice &other) {
    Slice slice;
    std::size_t new_size = size() + other.size();
    slice.buf_ = std::make_shared<__Buffer>(new_size);
    slice.start_ = slice.buf_->data;
    slice.size_ = new_size;
    char* p = slice.buf_->data;
    std::memcpy(p, start_, size());
    std::memcpy(p + size(), other.data(), other.size());
    return slice;
}


void Slice::clear() {
    size_ = 0;
    start_ = nullptr;
}

std::size_t Slice::find(const std::string &target, std::size_t pos) const {
    return find(target.data(), target.length(), pos);
}

std::size_t Slice::find(const char *target, std::size_t len, std::size_t pos) const {
    if (empty() || pos >= size()) {
        return npos;
    }
    ConstIterator iter = cbegin() + pos;
    while (iter != cend()) {
        if ((std::size_t)std::distance(iter, cend()) < len) {
            return npos;
        }
        if (std::equal(target, target + len, iter)) {
            return std::distance(begin(), iter);
        }
        ++iter;
    }
    return npos;
}

Slice Slice::subslice(std::size_t pos, std::size_t len) const {
    if (empty() || pos >= size()) {
        return Slice();
    }
    ConstIterator begin_iter = pos + begin();
    len = std::min(len, static_cast<std::size_t>(std::distance(begin_iter, cend())));
    Slice slice;
    slice.buf_ = buf_;
    slice.start_ = begin_iter;
    slice.size_ = len;
    return slice;
}

std::string Slice::to_string() const {
    if (empty()) {
        return std::string("");
    }
    return std::string(begin(), size());
}

bool operator==(const char *p, const Slice &s) {
    return operator==(s, p);
}

using namespace std;
bool operator==(const Slice &s, const char *p) {
    std::size_t plen = std::strlen(p);
    if (plen != s.size()) {
        return false;
    }
    for (std::size_t i = 0; i < s.size(); ++i, ++p) {
        if (*p != s[i]) {
            return false;
        }
    }
    return true;
}

std::ostream& operator <<(std::ostream & os, const Slice &s) {
    os<<s.to_string();
    return os;
}

} //namespace
