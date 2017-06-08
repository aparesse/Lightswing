#ifndef REFLECT
#define REFLECT
#include <map>
#include <string>
#include "any.h"
#include "singleton.h"
namespace lightswing {

//非线程安全
class Reflect {
public:
    Reflect();

    static Reflect* instance();

    template<class T>
    void set(const std::string& name, T* value);

    Any get(const std::string& name) const;

private:
    Reflect& operator=(const Reflect&) = delete;
    Reflect(const Reflect&) = delete;

private:
    std::map<std::string, Any> objects_;
};

inline Reflect::Reflect() :
    objects_() {

}

Reflect *Reflect::instance() {
    return Singleton<Reflect>::instance();
}

inline Any Reflect::get(const std::string &name) const {
    auto iter = objects_.find(name);
    if (iter == objects_.end()) {
        return Any();
    }
    return iter->second;
}

template<class T>
inline void Reflect::set(const std::string &name, T *value) {
    objects_[name] = value;
}

inline Any get_reflect(const std::string& name) {
    Reflect* reflect = Reflect::instance();
    return reflect->get(name);
}

//失败返回-1,成功返回0
template<class T>
inline int get_reflect(const std::string& name, T** result) {
    assert(result);
    assert(*result);
    Any any = get_reflect(name);
    if (typeid(T*) == any.type()) {
        T* pointer = any_cast<T*>(any);
        *result = pointer;
        return 0;
    }
    return -1;
}


#define REG_REFLECT(s) \
                                    do {                                                                                                \
                                        Reflect* __reflect_ = Reflect::instance();                                 \
                                        __reflect_->set(#s, &s);                                                             \
                                    }while(0)


}//namespace
#endif // REFLECT

