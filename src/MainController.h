#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "base/ThreadTaskQueue.h"
#include "SomeProxy.h"

using namespace cpptdsync;
using namespace std;

class MainController: public SomeProxy::Callback {
public:
    MainController(std::shared_ptr<ThreadTaskQueue> queue);//编译器帮忙生成
    ~MainController() = default;

    void Setup();
    void SendMsg(string& message);

    void onHappen(string msg) override;//callback方法

private:
    std::shared_ptr<ThreadTaskQueue> workerQueue_;
    std::shared_ptr<SomeProxy> someProxy_;

};
#endif