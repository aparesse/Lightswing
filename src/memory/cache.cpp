#include "cache.h"

namespace lightswing {

Cache::Cache() :
    freelists_(Central::kBIG_OBJECT_BYTES / Central::kBYTES_ALIGN)
{
}

void Cache::deallocate(void *p, std::size_t size) {
    std::size_t index = Central::index(size);
    MemoryList& mlist = freelists_[index];
    mlist.push_back((char *)p);
}

void *Cache::allocate(std::size_t size) {
    std::size_t index = Central::index(size);
    MemoryList& mlist = freelists_[index];
    if (mlist.empty()) {
        return nullptr;
    }
    void *result = (void*)mlist.back();
    mlist.pop_back();
    return result;
}

void Cache::fill(const MemoryList &ptrs, std::size_t level) {
    assert(level < freelists_.size());
    MemoryList& mlist = freelists_[level];
    for (auto ptr : ptrs) {
        mlist.push_back(ptr);
    }
}

}//namespace
