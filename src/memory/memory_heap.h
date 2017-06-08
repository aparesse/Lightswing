#ifndef MEMORY_HEAP
#define MEMORY_HEAP
#include <malloc.h>
#include <mutex>
#include "../singleton.h"
namespace lightswing {

class MemoryHeap {
public:
    static const int kDEFAULT_SIZE = 1024;
public:
    MemoryHeap();
    ~MemoryHeap();
    static MemoryHeap* instance();
    void* allocate(std::size_t size);
    void deallocate(void *p, std::size_t size);
private:
    std::mutex mutex_;
    std::size_t alloc_size_;
};



} //namespace
#endif // MEMORY_HEAP

