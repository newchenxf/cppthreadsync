#include "SomeProxy.h"
#include "thread"
#include <unistd.h>
#include "base/logutils.h"
/***
 * 这个类是为了说明，启动多个线程，间接调用MainController，做一些任务
 * */
SomeProxy::SomeProxy() {
    sendCallback("hello world 11");
    sendCallback("hello world 22");
}

void SomeProxy::sendCallback(std::string message) {
    std::thread delayT([=]() {
        sleep(2);
        ALOGI("sleep done");
        if(callback_ != nullptr) {
            callback_->onHappen(message);
        }
    });
    delayT.detach();
}