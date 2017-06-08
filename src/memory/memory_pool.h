#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <cstddef>
#include <vector>
#include <list>
#include "central.h"
#include "cache.h"
namespace lightswing {

class MemoryPool {
    typedef  char* Pointer;
    typedef std::list<Pointer> PointerList;
public:
    static MemoryPool* instance();
public:
    MemoryPool();
    void init(std::size_t size);
    void* allocate(std::size_t size);
    void deallocate(void * addr, std::size_t len);

private:
    void* refill(std::size_t n);
    std::size_t free_size() const;

    MemoryPool& operator=(const MemoryPool& )  = delete;
    MemoryPool(const MemoryPool&) = delete;

private:
    std::vector<Central> centrals_;
    std::vector<Cache> caches_;
};

} //namespace

#endif // MEMORY_POOL_H
