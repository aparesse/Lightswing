#include "runtime.h"
namespace lightswing {

Runtime *Runtime::instance() {
    return Singleton<Runtime>::instance();
}

Runtime::Runtime() :
    core_num_(1),
    schedule_(),
    threadpool_(),
    event_manager_()
{
    LOG_INFO<<"运行时启动";
}

Runtime::~Runtime()
{
    LOG_INFO<<"运行时已关闭";
}

void Runtime::set_max_procs(std::size_t num) {
    assert(num > 0);
    core_num_ = num;
}

std::size_t Runtime::max_procs() const {
    return core_num_ ;
}

EventManager &Runtime::event_manager() {
    return event_manager_;
}

int Runtime::get_thread_id() const {
    const ThreadPool::ThreadMap& m = threadpool_.thread_map();
    auto iter = m.find(std::this_thread::get_id());
    if (iter == m.end()) {
        return -1;
    }
    return iter->second->id();
}

void Runtime::yield() {
    ThreadPool::ThreadMap& m = threadpool_.thread_map();
    auto iter = m.find(std::this_thread::get_id());
    if (iter == m.end()) {
        LOG_DEBUG<<"请勿在非协程线程使用yield";
        return;
    }
    iter->second->running_coroutine()->yield();
}

void Runtime::on_event(const std::string &ev, EventManager::Func func) {
    event_manager_.register_event(ev, std::move(func));
}

void Runtime::emit_event(const std::string &ev) const {
    event_manager_.emit_event(ev);
}

Coroutine::Pointer Runtime::new_coroutine(std::function<void()> func) {
    return schedule_.new_coroutine([func] (Coroutine::Pointer) {
        func();
    });
}

}
