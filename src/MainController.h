#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "base/ThreadTaskQueue.h"

using namespace cpptdsync;
using namespace std;

class MainController {
public:
    MainController(std::shared_ptr<ThreadTaskQueue> queue);//编译器帮忙生成
    ~MainController() = default;

    void Setup();
    void SendMsg(string& message);

private:
    std::shared_ptr<ThreadTaskQueue> workerQueue_;
};
#endif