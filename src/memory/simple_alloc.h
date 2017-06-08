#ifndef SIMPLE_ALLOC
#define SIMPLE_ALLOC
#include <vector>
#include "memory_pool.h"
namespace lightswing {

template<class T>
class SimpleAlloc {
public:
    static T *allocate();
    static T *allocate(std::size_t n);
    static void deallocate(T *ptr);
    static void deallocate(T *ptr, std::size_t n);
    static void construct(T *ptr);
    static void construct(T *ptr, const T& value);
    static void destroy(T *ptr);
    static void destroy(T *first, T *last);
};

template<class T>
inline T *SimpleAlloc<T>::allocate() {
    return allocate(1);
}

template<class T>
inline T *SimpleAlloc<T>::allocate(std::size_t n) {
    MemoryPool* pool = MemoryPool::instance();
    return (T*)pool->allocate(sizeof(T) * n);
}

template<class T>
inline void SimpleAlloc<T>::deallocate(T *ptr) {
    deallocate(ptr, 1);
}

template<class T>
inline void SimpleAlloc<T>::deallocate(T *ptr, std::size_t n) {
    MemoryPool* pool = MemoryPool::instance();
    pool->deallocate(ptr, sizeof(T) * n);
}

template<class T>
inline void SimpleAlloc<T>::construct(T *ptr) {
    ::new ((void*)ptr) T();
}

template<class T>
inline void SimpleAlloc<T>::construct(T *ptr, const T &value) {
    ::new ((void*)ptr) T(value);
}

template<class T>
inline void SimpleAlloc<T>::destroy(T *ptr) {
    ptr->~T();
}

template<class T>
inline void SimpleAlloc<T>::destroy(T *first, T *last) {
    for (;first != last; ++first) {
        first->~T();
    }
}

}

#endif // SIMPLE_ALLOC

