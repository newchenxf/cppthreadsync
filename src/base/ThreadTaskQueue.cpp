//
// Created by Yujunlei on 2020/03/16.
//

#include "ThreadTaskQueue.h"
#include <assert.h>

namespace cpptdsync {

/**
 * 提交一个任务
 * 
 * @task 任务
 * @wait 为true表示等到任务结束，该函数才执行完，为false表示提交完任务，无需等待任务执行，结束该调用
 * */
void ThreadTaskQueue::Post(std::unique_ptr<QueuedTask> task, bool wait)
{
    assert(thread_ref_ != 0);
    if (thread_ref_ == pthread_self())
    {
        //进入这里，表示是在某个task执行过程中由插入了一个task
        //这种情况，先执行插入的task
        task.release()->DoTask();
        return;
    }

    std::unique_lock<std::mutex> lk(mutex_);
    auto t = std::shared_ptr<QueuedTask>(task.release());
    queue_.push(t);
    cv_.notify_one();
    if (wait)
    {
        lk.unlock();
        t->WaitTask();
    }
}

/**
 * static函数，线程启动执行的入口
 * */
void ThreadTaskQueue::Run(void* obj) {
    ThreadTaskQueue* impl = static_cast<ThreadTaskQueue*>(obj);
    if (impl->thread_ref_ != 0) 
        return;
    impl->thread_ref_ = pthread_self();//记录线程号
    do {
        impl->Process();
    } while(impl->Wait());//Process()
}

/**
 * 读取任务，执行任务，直到任务栈 全部执行完
 * */
bool ThreadTaskQueue::Process()
{
    std::unique_lock<std::mutex> lk(mutex_);
    //每次取一个task执行，直到task取完
    while (!queue_.empty()) {
        auto task = queue_.front();
        queue_.pop();
        last_task_name_ = task->TaskName();
        lk.unlock();
        // 执行Task时解除锁，防止Task任务耗时过长，阻塞其他线程push操作。
        task->DoTask();

        //执行完task，解除锁，因为要继续while取下一个task，要操作queue，不允许其他线程插入queue
        lk.lock();
    }

    //任务全部执行完必
    return true;
}

/**
 * 等待
 * cv_.wait：当前线程调用wait()后将被阻塞，直到另外某个线程调用notify_*唤醒当前线程；
 * 当线程被阻塞时，该函数会自动调用std::mutex的unlock()释放锁，使得其它被阻塞在锁竞争上的线程得以继续执行。
 * */
bool ThreadTaskQueue::Wait() {
    std::unique_lock<std::mutex> lk(mutex_);
    ALOGI("Wait");
    cv_.wait(lk);
    ALOGI("Wait done");
    return true;
}

void ThreadTaskQueue::CancelAllTask() {
    std::unique_lock<std::mutex> lk(mutex_);
    std::queue<std::shared_ptr<QueuedTask>> empty;
    std::swap(queue_, empty);
}

} // CloudGaming