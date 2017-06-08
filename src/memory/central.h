#ifndef CENTRAL
#define CENTRAL
#include <list>
#include <mutex>
#include "memory_heap.h"
#include <vector>
namespace lightswing {

class Central {
public:
    static const int kBYTES_ALIGN = 8;
    static const int kBIG_OBJECT_BYTES = 64;
    static const int kALLOC_BYTES = 4 * kBIG_OBJECT_BYTES;
    typedef char* Pointer;

public:
    Central();
    void init(std::size_t level);
    void deallocate(void *p);
    void* allocate();
    std::vector<Pointer> allocate(std::size_t n);
    bool empty() const;
    std::size_t size() const;

    //这个central保存的指针的size
    std::size_t central_size() const;

    //根据字节数找到引索
    static std::size_t index(std::size_t bytes);

    //把字节数上调到8 的倍数对其
    static std::size_t round_up(std::size_t bytes);

private:
    void __fill_list(std::size_t n);
private:


private:
    mutable std::mutex mutex_;
    int level_;
    std::list<Pointer> list_;
};


}//namespace
#endif // CENTRAL



