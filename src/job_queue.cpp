
#include "job_queue.hpp"


namespace queue {

JobQueue::~JobQueue()
{
    stopExecution();
}

void JobQueue::startExecution()
{
    // Joining the thread before start
    stopExecution();

    std::unique_lock<std::mutex> lock(mThreadMutex);
    mRunLoop = true;
    mThread = std::thread(&JobQueue::run, this);
}

void JobQueue::stopExecution()
{
    std::unique_lock<std::mutex> lock(mThreadMutex);
    mRunLoop = false;
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mConditionVariable.notify_one();
    }
    if (mThread.joinable()) {
        mThread.join();
    }
}

void JobQueue::addJob(std::function<void()> job)
{
    std::unique_lock<std::mutex> lock(mQueueMutex);
    mQueue.push(std::move(job));
    mConditionVariable.notify_one();
}

void JobQueue::run()
{
    while (mRunLoop)
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        std::function<void()> job = nullptr;
        if (mQueue.empty() && mRunLoop) {
            mConditionVariable.wait(lock);
        }
        if (!mQueue.empty()) {
            job = mQueue.front();
            mQueue.pop();
        }
        lock.unlock();
        if (job) {
            job();
        }
    }
}

} // namespace queue

