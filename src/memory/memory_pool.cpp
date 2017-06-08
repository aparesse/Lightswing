#include "memory_pool.h"
#include <malloc.h>
#include "../singleton.h"
#include "../runtime.h"
namespace lightswing {

MemoryPool *MemoryPool::instance() {
    return Singleton<MemoryPool>::instance();
}

MemoryPool::MemoryPool() :
    centrals_(),
    caches_()
{
}

void MemoryPool::init(std::size_t size) {
    int n = Central::kBIG_OBJECT_BYTES / Central::kBYTES_ALIGN;
    centrals_ = std::vector<Central>(size);
    for (int i = 0; i < n; ++i) {
        centrals_.at(i).init(i);
    }
    caches_.resize(size);
}

void *MemoryPool::allocate(std::size_t size) {
    if (size > Central::kBIG_OBJECT_BYTES) {
        return ::malloc(size);
    }
    int id = Runtime::instance()->get_thread_id();
    if (id == -1) {
        LOG_DEBUG<<"请勿在非协程线程申请内存";
        assert(id != -1);
    }
    Cache& cache = caches_[id];
    void* result = cache.allocate(size);
    if (!result) {
        std::size_t level = Central::index(size);
        Central& central = centrals_[level];
        auto vec = central.allocate(
                    Central::kALLOC_BYTES / central.central_size());
        cache.fill(vec, level);
        result = cache.allocate(size);
    }
    return result;
}

void MemoryPool::deallocate(void *addr, std::size_t len) {
    if (len > Central::kBIG_OBJECT_BYTES) {
        return ::free(addr);
    }
    int id = Runtime::instance()->get_thread_id();
    if (id == -1) {
        LOG_DEBUG<<"请勿在非协程线程申请内存";
        assert(id != -1);
    }
    Cache& cache = caches_[id];
    len = Central::round_up(len);
    cache.deallocate(addr, len);
}

void *MemoryPool::refill(std::size_t size) {
    //    size = round_up(size);
    //    std::size_t times = 10;
    //    std::size_t bytes_to_get = (times + 1) * size;
    //    Pointer new_space = (Pointer)malloc(bytes_to_get);
    //    std::size_t index = freelist_index(size);
    //    PointerList& freelist = free_lists_[index];
    //    Pointer p = new_space + size;
    //    for (std::size_t i = 0; i < times; ++i) {
    //        freelist.push_back(p + size * i);
    //    }
    return nullptr;
}

std::size_t MemoryPool::free_size() const {
    // return static_cast<std::size_t>(free_addr_end_ - free_addr_start_);
    //todo
    return 0;
}

} //namespace
