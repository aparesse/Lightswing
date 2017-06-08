#include "coroutine.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include "schedule.h"
#include "thread_context.h"
namespace lightswing {

void Coroutine::save_stack() {
//    char dummy = 0;
//    assert(stack_bottom_ > &dummy);
//    uint64_t distance = stack_bottom_ - &dummy;
//    assert(distance < (uint64_t)(stack_bottom_ - stack_top_));
//    stack_.resize(distance);
//    ::memcpy(stack_top(), &dummy, stack_.size());
}

Coroutine::Coroutine()  :
    stack_(kSTACK_MAX_SIZE),
    func_(),
    status_(eDEAD),
    id_(-1)
{

}

void Coroutine::yield() {
    //save_stack();
    assert(status_ == Coroutine::eRUNNING);
    status_ =  Coroutine::eSUSPEND;
    swapcontext(&ctx_ , ctx_.uc_link);
    //swapcontext(&ctx_, &(tcontext_->ctx_));
}

void Coroutine::swap_context(ucontext* context) {
    //swapcontext(&ctx_ , &schedule_->main_ctx_);
    swapcontext(&ctx_ , context);
}

void Coroutine::set_func(Coroutine::Function func) {
    func_ = std::move(func);
}

Coroutine::Pointer Coroutine::create() {
    return std::make_shared<Coroutine>();
}

ucontext_t *Coroutine::context() {
    return &ctx_;
}

std::size_t Coroutine::stack_size() const {
    return stack_.size();
}

Coroutine::Status Coroutine::status() const {
    return status_;
}

void Coroutine::set_status(Status status) {
    status_ = status;
}

char *Coroutine::stack_top() {
    return &*stack_.begin();
}

//void Coroutine::resume() {
//     //schedule_->resume(shared_from_this());
//}


} //namespace

