
#ifndef WATCH_DOG_H
#define WATCH_DOG_H

#include "ThreadTaskQueue.h"
#include <functional>

namespace cpptdsync {
using std::chrono::system_clock;

class SoftWatchDog {
public:
    explicit SoftWatchDog(std::shared_ptr<ThreadTaskQueue> queue);
    template <class F, class... Args>
    void SetCallback(F &&f, Args &&... args)
    {
        callback_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }
    void Boot();

private:
    void Run();
    void Feed();
    void Terminal();
    std::shared_ptr<ThreadTaskQueue> target_queue_;
    std::function<void(void)> callback_;
    std::mutex mutex_;
    system_clock::time_point last_feed_tp_;
    std::unique_ptr<std::thread> watch_thread_;
    std::unique_ptr<std::thread> feed_thread_;
};

} // CloudGaming
#endif // CLOUD_GAMING_WATCH_DOG_H