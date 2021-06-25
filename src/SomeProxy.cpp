#include "SomeProxy.h"
#include "thread"
#include <unistd.h>
#include "base/logutils.h"
/***
 * 这个类是为了说明，间接调用MainController，做一些任务
 * */
SomeProxy::SomeProxy() {
    std::thread delayT([=]() {
        sleep(10);
        ALOGI("sleep done");
        if(callback_ != nullptr) {
            callback_->onHappen("hello world");
        }
    });
    delayT.detach();
}