#include "thread_context.h"
#include <assert.h>
#include "thread_pool.h"
#include "runtime.h"
#include "logger.h"
#include <mutex>
namespace lightswing {

ThreadContext::ThreadContext(int id, ThreadPool *pool) :
    id_(id),
    pool_(pool),
    running_coroutine_()
{
}

void ThreadContext::start() {
    assert(pool_);
    std::thread t([this] () {
        auto& thread_map = pool_->thread_map();
        thread_map[std::this_thread::get_id()] = this;
        while (pool_->is_running()) {
            Runtime* runtime = Runtime::instance();
            Coroutine::Pointer co = runtime->schedule_.pop_coroutine(id_);
            running_coroutine_ = co;
            resume(co);
            if (co->status() != Coroutine::eDEAD) {
                runtime->schedule_.push_coroutine(id_, co);
            } else {
                runtime->schedule_.remove(co);
            }
        }
    });
    t.detach();
}

Coroutine::Pointer ThreadContext::running_coroutine() {
    return running_coroutine_.lock();
}

int ThreadContext::id() const {
    return id_;
}

void mainfunc(uint32_t low32, uint32_t hi32) {
    uintptr_t ptr = (uintptr_t)low32;
    uintptr_t high = (uintptr_t)hi32;
    high <<= high;
    ThreadContext* tcontext = (ThreadContext*) ptr;
    Coroutine::Pointer co = tcontext->running_coroutine();
    assert(co);
    co->set_status(Coroutine::eRUNNING);
    co->func_(co);
    co->status_ = Coroutine::eDEAD;
}

void ThreadContext::resume(Coroutine::Pointer co) {
    if (!co) {
        LOG_DEBUG<<"!co";
        return;
    }
    int status = co->status();
    ucontext_t * co_context = co->context();
    switch (status) {
    case Coroutine::eREADY : {
        getcontext(co_context);  //init the ucontext_t
        co_context->uc_stack.ss_sp = co->stack_top();
        co_context->uc_stack.ss_size = co->stack_size();
        co_context->uc_stack.ss_flags = 0;
        //Here uc_link points to the context that will be resumed when the current context terminates
        // (in case the current context was created using makecontext
        co_context->uc_link = &ctx_;
        //makecontext的第一个参数必须要是getcontext得到的。
        //If the context was obtained by a call of makecontext(3), program execu‐
        //tion continues by a call to the function func specified as  the  second
        //argument  of  that  call  to  makecontext(3).   When  the function func
        //returns, we continue with the uc_link member of the structure ucp spec‐
        //ified  as the first argument of that call to makecontext(3).  When this
        //member is NULL, the thread exits.
        uintptr_t ptr = (uintptr_t)this;
        uint32_t low = (uint32_t)ptr;
        uintptr_t high = ptr;
        high >>= high;
        makecontext(co_context, (void (*)(void)) mainfunc, 2, low, (uint32_t)high);
        //The swapcontext() function saves the current context in the structure pointed to by first param,
        //and then activates the context pointed to by second param.
         co->set_status(Coroutine::eRUNNING);
        swapcontext(&ctx_, co_context);
        break;
    }
    case Coroutine::eSUSPEND :
        //co_context->uc_link = &ctx_;
        co->set_status(Coroutine::eRUNNING);
        swapcontext(&ctx_, co_context);
        break;
    case Coroutine::eDEAD:
        LOG_INFO<<"eDEAD"<<std::this_thread::get_id();
        assert(0);
        break;
    default:
        LOG_DEBUG<<"default:"<<status;
        assert(0);
        break;
    }
}

}//namespace
