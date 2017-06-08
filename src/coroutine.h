#ifndef COROUTINE_IMPL_H
#define COROUTINE_IMPL_H
#include <vector>
#include <ucontext.h>
#include <functional>
#include <set>
#include <memory>
#include "logger.h"

namespace lightswing {
class ThreadContext;
class Schedule;
class Coroutine : public std::enable_shared_from_this<Coroutine>{
public:
    enum Status{
        eDEAD,
        eREADY,
        eSUSPEND,
        eRUNNING,
        eSIZE
    };
    friend class Schedule;
    friend class ThreadContext;
    friend void mainfunc(uint32_t low32, uint32_t hi32);
    typedef std::shared_ptr<Coroutine> Pointer;
    typedef std::weak_ptr<Coroutine> WeakPointer;
    typedef std::function<void(Coroutine::Pointer s)> Function;

public:
    Coroutine();
    //~Coroutine(){LOG_INFO<<"~Coroutine";}
    int id() const { return this->id_; }
    void yield();
    void swap_context(ucontext* context);
    void set_func(Function func);
    static Pointer create();
    ucontext_t* context();
    std::size_t stack_size() const;

    Status status() const;
    void set_status(Status status);

private:
    char* stack_top() ;
    void save_stack();
private:
    std::vector<char> stack_;
    Function func_;
    ucontext_t ctx_;
    Status status_;
    int id_;
    Schedule* schedule_;
};

void mainfunc(uint32_t low32, uint32_t hi32);
const int kSTACK_MAX_SIZE = 4 * 1024;

}//namespace
#endif // COROUTINE_IMPL_H
