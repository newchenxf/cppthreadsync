#include "MainController.h"
#include "base/ThreadTaskQueue.h"
#include "base/logutils.h"

MainController::MainController(std::shared_ptr<ThreadTaskQueue> queue):
    workerQueue_(queue) {

}
void MainController::Setup() {
    //阻塞等待任务结束
    workerQueue_->InvokeTask("Setup", []() {//Lambda表达式
        //TODO 做一些任务
        ALOGI("I am doing Setup");
    });
    //任务完成
}
void MainController::SendMsg(string& message) {
    //异步调用
    workerQueue_->PostTask("SendMsg", [=]() {
        ALOGI("SendMsg %s", message.c_str());
    });
}