#ifndef CACHE_H
#define CACHE_H
#include <vector>
#include <assert.h>
#include "central.h"
namespace lightswing {

//
class Cache{
    typedef std::vector<char *>  MemoryList;
public:
    Cache();
    void deallocate(void* p, std::size_t size);
    void* allocate(std::size_t size);
    void fill(const MemoryList &ptrs, std::size_t level);
private:
    std::vector<MemoryList> freelists_;
};

}
#endif // CACHE_H

