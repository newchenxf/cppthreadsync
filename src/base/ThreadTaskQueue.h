
#ifndef THREAD_TASK_QUEUE_H
#define THREAD_TASK_QUEUE_H

#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include "logutils.h"

namespace cpptdsync {

/**
 * 任务的接口
 * */
class QueuedTask {
public:
    virtual ~QueuedTask() = default;

    virtual bool DoTask() = 0;
    virtual void WaitTask() {};
    explicit QueuedTask(std::string &&name):taskName_(name){} 
    std::string& TaskName() { return taskName_; }
private: 
    std::string taskName_;
};

/**
 * QueuedTask的实现类
 * 
 * @name 任务名称
 * @closure 具体执行工作，是一个Lambda表达式(函数)
 * */
template <typename Closure>
class ClosureTask : public QueuedTask {
public:
    explicit ClosureTask(std::string &&name, Closure &&closure)
        : QueuedTask(std::forward<std::string>(name)), 
        closure_(std::forward<Closure>(closure)), 
        finished_(false) {
            ALOGI("create task. 0x%x ", this);
        }

    void WaitTask() override {
        std::unique_lock<std::mutex> lk(mutex_);
         ALOGI("WaitTask. 0x%x", this);
        if (finished_) {
            ALOGI("Task. 0x%x OK", this);
            return;
        }
        cv_.wait(lk);
        ALOGI("Task. 0x%x OK", this);
    }

     ~ClosureTask() {
        ALOGI("Delete task. 0x%x ", this);
     }
private:
    bool DoTask() override {
        std::unique_lock<std::mutex> lk(mutex_);
        ALOGI("DoTask. 0x%x", this);
        closure_();
        finished_ = true;
        cv_.notify_one();
        ALOGI("Task. 0x%x finish", this);
        return true;
    }

    typename std::decay<Closure>::type closure_;
    std::condition_variable cv_;
    std::mutex mutex_;
    bool finished_;
};


/**
 * 任务队列的管理
 * */
class ThreadTaskQueue {
public:
    ThreadTaskQueue():thread_ref_(0) {}
    
    static void Run(void* obj);

    // 异步调用，非阻塞
    template <typename Closure>
    void PostTask(std::string &&name, Closure &&closure)
    {
        Post(std::unique_ptr<QueuedTask>(
            new ClosureTask<Closure>(std::forward<std::string>(name), std::forward<Closure>(closure))));
    }

    // 异步调用，阻塞等待结束
    template <typename Closure>
    void InvokeTask(std::string &&name, Closure &&closure)
    {
        Post(std::unique_ptr<QueuedTask>(
                 new ClosureTask<Closure>(std::forward<std::string>(name), std::forward<Closure>(closure))),
             true);
    }

    std::string& GetLastTaskName() {return last_task_name_;}
    void CancelAllTask();

private:
    void Post(std::unique_ptr<QueuedTask> task, bool wait = false);
    bool Process();
    bool Wait();

    std::mutex mutex_;

    //条件变量std::condition_variable用于多线程之间的通信，它可以阻塞一个或同时阻塞多个线程。
    //std::condition_variable需要与std::unique_lock配合使用
    std::condition_variable cv_;

    std::queue<std::shared_ptr<QueuedTask>> queue_;
    pthread_t thread_ref_;
    std::string last_task_name_;
};
} // CloudGaming

#endif // CLOUD_GAMING_THREAD_TASK_QUEUE_H