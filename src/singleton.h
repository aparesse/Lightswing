#ifndef SINGLETON
#define SINGLETON
namespace lightswing {

template<class T>
class Singleton {
public:
    static T* instance() {
        static T instance_;
        return &instance_;
    }
};

}//namespace
#endif // SINGLETON

