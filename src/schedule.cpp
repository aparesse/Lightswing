#include "schedule.h"
#include <assert.h>
#include <string.h>
#include "logger.h"
#include <algorithm>

namespace lightswing {


Schedule::Schedule() :
    max_id_(0)
{
}

void Schedule::init(int thread_num) {
    coroutines_table_ = std::vector<CoQueue>(thread_num);
}

//todo
Coroutine::Pointer Schedule::new_coroutine(Function func) {
    std::lock_guard<std::mutex> lock(mutex_);
    Coroutine::Pointer co = Coroutine::create();
    assert(co);
    co->func_ = std::move(func);
    co->id_ = max_id_++;
    co->status_ = Coroutine::eREADY;
    std::size_t index = co->id_ % coroutines_table_.size();
    CoQueue& q = coroutines_table_[index];
    q.push(co);
    ++size_;
    return co;
}

void Schedule::push_coroutine(int index, Coroutine::Pointer co) {
    CoQueue& q = coroutines_table_[index];
    q.push(co);
}

Coroutine::Pointer Schedule::pop_coroutine(int index) {
    CoQueue& q = coroutines_table_[index];
    Coroutine::Pointer co =  q.pop();
    return co;
}

std::size_t Schedule::size() const {
    return size_;
}

bool Schedule::empty() const {
    return size_ == 0;
}

int Schedule::remove(Coroutine::Pointer co) {
    assert(co);
    --size_;
    return 0;
}


//inline int Schedule::resume(Coroutine::Pointer co) {
//    if (!co) {
//        return -1;
//    }
//    int status = co->status_;
//    switch (status) {
//    case Coroutine::eREADY :
//        getcontext(&co->ctx_);
//        co->ctx_.uc_stack.ss_sp = stack_top();
//        co->ctx_.uc_stack.ss_size = stack_size();

//        //Here uc_link points to the context that will be resumed when the current context terminates
//        // (in case the current context was created using makecontext
//        co->ctx_.uc_link = &main_ctx_;
//        co->status_ = Coroutine::eRUNNING;
//        co->schedule_ = this;
//        running_coroutine_ = co;
//        //makecontext的第一个参数必须要是getcontext得到的。
//        //If the context was obtained by a call of makecontext(3), program execu‐
//        //tion continues by a call to the function func specified as  the  second
//        //argument  of  that  call  to  makecontext(3).   When  the function func
//        //returns, we continue with the uc_link member of the structure ucp spec‐
//        //ified  as the first argument of that call to makecontext(3).  When this
//        //member is NULL, the thread exits.
//        makecontext(&co->ctx_, (void (*)(void)) mainfunc, 1, this);
//        //The swapcontext() function saves the current context in the structure pointed to by first param,
//        //and then activates the context pointed to by second param.
//        swapcontext(&main_ctx_, &co->ctx_);
//        break;
//    case Coroutine::eSUSPEND :
//        assert(stack_size() >= co->stack_.size());  //todo
//        memcpy(stack_bottom() - co->stack_.size(), co->stack_top(), co->stack_.size());
//        running_coroutine_ = co;
//        co->status_ = Coroutine::eRUNNING;
//        swapcontext(&main_ctx_, &co->ctx_);
//        break;
//    default:
//        LOG_DEBUG<<"default:"<<status;
//        assert(0);
//        break;
//    }
//    return 0;
//}

//inline void Schedule::resume_all() {
//    for (auto iter = coroutines_.begin();
//         iter != coroutines_.end();) {
//        resume(*iter);
//        if ((*iter)->status_ == Coroutine::eDEAD) {
//            iter = coroutines_.erase(iter);
//        } else {
//            ++iter;
//        }
//    }
//}

//inline void Schedule::remove(Coroutine::Pointer) {
//    //    auto iter = coroutines_.find(co);
//    //    assert(iter != coroutines_.end());
//    //    coroutines_.erase(iter);
//}


} //namespace
