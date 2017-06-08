#ifndef OBJECT
#define OBJECT
#include <memory>
#include <string>
#include <sstream>
namespace lightswing {

template<class T>
class Object {
public:
    template<class ...Arg>
    Object(Arg&& ...arg);

    T& get() { return *ptr_; }

    const T& get() const { return *ptr_; }

    Object<T> clone() const {
        T* copy = ptr_->clone();
        Object<T> obj{__CloneConstruct(), copy};
        return obj;
    }

private:
    struct __CloneConstruct{};
    Object(__CloneConstruct, T* ptr) : ptr_(ptr) {}

    std::shared_ptr<T> ptr_;
};

template<class T>
template<class ...Arg>
inline Object<T>::Object(Arg && ...arg) :
    ptr_(std::make_shared<T>(std::forward<Arg>(arg)...))
{
}

}
#endif // OBJECT

