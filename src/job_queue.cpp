
#include "job_queue.hpp"


namespace queue {

MBJobQueue::~MBJobQueue()
{
    stopExecution();
}

void MBJobQueue::startExecution()
{
    // Joining the thread before start
    stopExecution();

    std::unique_lock<std::mutex> lock(mThreadMutex);
    mRunLoop = true;
    mThread = std::thread(&MBJobQueue::run, this);
}

void MBJobQueue::stopExecution()
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

void MBJobQueue::addJob(std::function<void()> job)
{
    std::unique_lock<std::mutex> lock(mQueueMutex);
    mQueue.push(std::move(job));
    mConditionVariable.notify_one();
}

void MBJobQueue::run()
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

