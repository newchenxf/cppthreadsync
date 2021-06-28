#include <iostream>
#include <thread>
#include "base/ThreadTaskQueue.h"
#include "base/WatchDog.h"
#include "base/logutils.h"
#include "MainController.h"

using namespace cpptdsync;

int main(int argc, char* argv[]) {
    intializeLog("f", "/home/chenxiaofeng", "my_test.txt");

    //创建一个智能指针，指向ThreadTaskQueue
    std::shared_ptr<ThreadTaskQueue> worker_queue(new ThreadTaskQueue());

    //启动一个线程，执行方法是ThreadTaskQueue::Run，给Run的参数是ThreadTaskQueue指针
    std::thread worker_thread(&ThreadTaskQueue::Run, worker_queue.get());

    //使用任务的业务对象
    std::shared_ptr<MainController> gc(new MainController(worker_queue));

    //看门狗，如果有某个任务执行超过30s，则报警，走到下面的回调
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

