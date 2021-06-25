
#include "WatchDog.h"
#include <assert.h>
#include "logutils.h"

namespace cpptdsync {

const int kWatchDogFeedIntevalMS = 5000;
const int kWatchDogTimeOutMS = 30100;

SoftWatchDog::SoftWatchDog(std::shared_ptr<ThreadTaskQueue> queue):target_queue_(queue),
                                    callback_(nullptr){
}

void SoftWatchDog::Boot() {
    assert(callback_ != nullptr);
    if (feed_thread_ != nullptr && feed_thread_->joinable()) {
        return;
    }
    last_feed_tp_ = system_clock::now();
    watch_thread_ = std::unique_ptr<std::thread>(new std::thread(&SoftWatchDog::Run, this));
    feed_thread_ = std::unique_ptr<std::thread>(new std::thread(&SoftWatchDog::Feed, this));
}

void SoftWatchDog::Run() {
    while (callback_ != nullptr) {
        system_clock::time_point now = system_clock::now();
        std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_feed_tp_);
        if (dur.count() >= kWatchDogTimeOutMS) {
            Terminal();
            break;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

void SoftWatchDog::Terminal() {
    std::unique_lock<std::mutex> lk(mutex_);
    if (this->callback_ != nullptr) {
        this->callback_( );
    }
    callback_ = nullptr;
}

void SoftWatchDog::Feed() {
    while (callback_ != nullptr) {
        system_clock::time_point now = system_clock::now();
        std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_feed_tp_);
        if (dur.count() >= kWatchDogFeedIntevalMS) {
            // ALOGI("WatchDog Invoke Feed.");
            target_queue_->InvokeTask("feedDog", [=](){
                // ALOGI("WatchDog Feed.");
                std::unique_lock<std::mutex> lk(mutex_);
                this->last_feed_tp_ = now;
            });
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}
} //CloudGaming