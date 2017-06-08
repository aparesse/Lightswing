#ifndef THREAD_OBJ
#define THREAD_OBJ
#include <thread>
#include <ucontext.h>
#include "coroutine.h"

namespace lightswing {
class ThreadPool;


class ThreadContext {
    friend void mainfunc(uint32_t low32, uint32_t hi32);
    friend class Coroutine;
public:
    ThreadContext(int id, ThreadPool* pool);
    void start();
    Coroutine::Pointer running_coroutine();
    int id() const;

private:
    void resume(Coroutine::Pointer co);
private:
    int id_;
    ThreadPool* pool_;
    ucontext ctx_;
    Coroutine::WeakPointer running_coroutine_;
};


}//namespace
#endif // THREAD_OBJ

