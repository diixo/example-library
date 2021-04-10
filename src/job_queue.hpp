
#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>


namespace queue {

class JobQueue {
public:
    JobQueue() = default;
    ~JobQueue();
    JobQueue(JobQueue&&) = delete;
    JobQueue(const JobQueue&) = delete;
    JobQueue& operator=(JobQueue&&) = delete;
    JobQueue& operator=(const JobQueue&) = delete;
    void startExecution();
    void stopExecution();
    void addJob(std::function<void()> job);

private:
    void run();
    std::queue<std::function<void()>> mQueue;
    std::thread mThread;
    std::mutex mQueueMutex;
    std::mutex mThreadMutex;
    std::atomic<bool> mRunLoop{false};
    std::condition_variable mConditionVariable;
};

} // namespace queue
