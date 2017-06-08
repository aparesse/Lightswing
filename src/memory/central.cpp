#include "central.h"

namespace lightswing {

Central::Central() :
    mutex_(),
    level_(-1),
    list_()
{

}

void Central::init(std::size_t level) {
    level_ = level;
}

void Central::deallocate(void *p) {
    std::lock_guard<std::mutex> lock(mutex_);
    list_.push_back((char*)p);
}

void *Central::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (list_.empty()) {
        __fill_list(kALLOC_BYTES / central_size());
    }
    void* result = (void *)list_.back();
    list_.pop_back();
    return result;
}

std::vector<Central::Pointer> Central::allocate(std::size_t n) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (list_.size() < n) {
        __fill_list(n);
    }
    std::vector<Pointer> result;
    result.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        result.push_back(list_.back());
        list_.pop_back();
    }
    return result;
}

bool Central::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return list_.empty();
}

std::size_t Central::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return list_.size();
}

std::size_t Central::index(std::size_t bytes) {
    return ((bytes + kBYTES_ALIGN - 1) / kBYTES_ALIGN) -1;
}

std::size_t Central::round_up(std::size_t bytes) {
    return (bytes + kBYTES_ALIGN - 1) & ~(kBYTES_ALIGN - 1);
}

void Central::__fill_list(std::size_t n) {
    MemoryHeap* heap = MemoryHeap::instance();
    char* p = (char *)heap->allocate(kALLOC_BYTES);
    for (std::size_t i = 0; i < n; ++i ) {
        list_.push_back(p);
        p += central_size();
    }
}



std::size_t Central::central_size() const {
    return (level_ + 1) * kBYTES_ALIGN;
}

}//namespace
