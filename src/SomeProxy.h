#ifndef SOME_PROXY_H
#define SOME_PROXY_H
#include <unistd.h>
#include <string>

class SomeProxy {
public:
    struct Callback//struct的方法默认public
    {
        virtual ~Callback() {}
        virtual void onHappen(std::string msg) = 0;
    };
    
    SomeProxy();

    void RegisterCallback(Callback* cb) {callback_ = cb;}

private:
    Callback* callback_;
};

#endif