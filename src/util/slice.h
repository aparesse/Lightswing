#ifndef SLICE
#define SLICE

#include <string>
#include <cstring>
#include <memory>
#include <iostream>
namespace lightswing {

struct __Buffer{

    __Buffer() : data(nullptr) {}

    __Buffer(std::size_t size) :
        data(new char[size]) {
    }

    ~__Buffer() {
        if (data) {
            delete data;
        }
    }
    char* data;
};

class Slice {
public:
    typedef const char* ConstIterator;
    friend Slice make_slice(const char*);
    static const std::size_t npos = std::string::npos;
public:
    Slice();

    explicit Slice(const char* buf);

    Slice(const char* buf, std::size_t size);

    //克隆一份并且自己持有char *
    Slice clone() const;

    //引用容器的内容，不发生拷贝
    friend Slice make_ref(const char* str, std::size_t len);
    friend Slice make_ref(const char* first, const char* last);

    void clear();

    bool empty() const;

    char operator[](std::size_t pos) const;

    char at(std::size_t pos) const;


    std::size_t size() const { return size_; }

    std::size_t length() const { return size(); }

    ConstIterator begin() const { return start_; }

    ConstIterator end() const { return start_ + size_; }

    ConstIterator cbegin() const { return start_; }

    ConstIterator cend() const { return start_ + size_; }

    std::size_t find(const std::string& target, std::size_t pos = 0) const;

    std::size_t find(const char* target, std::size_t len, std::size_t pos = 0) const;

    Slice subslice(std::size_t pos, std::size_t len) const;

    std::string to_string() const;

    const char* data() const { return start_; }

    Slice operator+(const Slice& other);
    friend std::ostream& operator <<(std::ostream&, const Slice&);
    friend bool operator==(const Slice& s, const char* p);
    friend bool operator==(const char* p, const Slice& s);

private:
    std::shared_ptr<__Buffer> buf_;
    const char* start_;
    std::size_t size_;
};

//引用容器的内容，不发生拷贝
inline Slice make_ref(const char* str, std::size_t len) {
    Slice slice;
    slice.buf_ = nullptr;
    slice.start_ = str;
    slice.size_ = len;
    return slice;
}

//引用容器的内容，不发生拷贝
inline Slice make_ref(const char* start, const char* last) {
    std::size_t len = static_cast<std::size_t>(last - start);
    return make_ref(start, len);
}


}
#endif // SLICE

