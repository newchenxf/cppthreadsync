
#include <iostream>
#include <thread>
#include "base/ThreadTaskQueue.h"
#include "base/WatchDog.h"
#include "base/logutils.h"
#include "MainController.h"

using namespace cpptdsync;

int main(int argc, char* argv[]) {
    //intializeLog("f", "/data/qiyi/gsLog", "gameService_log.txt");

    //创建一个智能指针，指向ThreadTaskQueue
    std::shared_ptr<ThreadTaskQueue> worker_queue(new ThreadTaskQueue());

    //启动一个线程，执行方法是ThreadTaskQueue::Run，给Run的参数是ThreadTaskQueue指针
    std::thread worker_thread(&ThreadTaskQueue::Run, worker_queue.get());

    std::shared_ptr<MainController> gc(new MainController(worker_queue));

    std::unique_ptr<SoftWatchDog> dog(new SoftWatchDog(worker_queue));
    dog->SetCallback([=]() {
        const char *last_name = worker_queue->GetLastTaskName().c_str();
        ALOGI("Do Timed out. LastTask:%s", last_name);
        //gc->UnregistAndExit(CG_ETASK_TIMEOUT, 0, last_name);
        worker_queue->CancelAllTask();
    });
    dog->Boot();

    gc->Setup();
    
    worker_thread.join();

    return 0;
}

