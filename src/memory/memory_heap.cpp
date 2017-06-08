#include "memory_heap.h"
namespace lightswing {

MemoryHeap::MemoryHeap() :
    mutex_(),
    alloc_size_(kDEFAULT_SIZE)
{

}

void MemoryHeap::deallocate(void *p, std::size_t) {
    //std::lock_guard<std::mutex> lock(mutex_);
    ::free(p);
}

MemoryHeap::~MemoryHeap() {

}

MemoryHeap *MemoryHeap::instance() {
    return Singleton<MemoryHeap>::instance();
}

void *MemoryHeap::allocate(std::size_t size) {
    //std::lock_guard<std::mutex> lock(mutex_);
    return malloc(size);
}


} //namespace
