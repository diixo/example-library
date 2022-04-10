
#include <algorithm>
#include <chrono>

#include "EventLoop.hpp"
#include "Call.hpp"
#include "Event.hpp"


namespace ipc {
namespace _private {

EventLoop::EventLoop(const std::string& threadName)
    : mThread()
    , mbStop(false)
    , mThreadId()
    , mThreadName(threadName)
    , mEvents()
    , mMutex()
    , mCV()
{
    mThread = std::thread(&EventLoop::run, this);
    mThreadId = mThread.get_id();    
}

std::thread::id EventLoop::getThreadId() const
{
    return mThreadId;
}

const std::string& EventLoop::getThreadName() const
{
    return mThreadName;
}

std::thread::id EventLoop::getCurrentThreadId()
{
    return std::this_thread::get_id();
}

void EventLoop::push(std::shared_ptr<ICallable> call)
{
    std::unique_lock <std::mutex> lock(mMutex);
    if (!mbStop)
    {
        mEvents.push(std::make_shared<Event>(call));
        mCV.notify_one();
    }
}


void EventLoop::stop()
{
   std::unique_lock <std::mutex> lock(mMutex);

   std::queue<std::shared_ptr<Event>> empty;
   std::swap(mEvents, empty);

   mbStop = true;
   mCV.notify_one();

   mThread.join();
}

void EventLoop::run()
{
    while (!mbStop)
    {
        std::shared_ptr<Event> event = nullptr;
        {
            // std::contidion_variable::wait_for with max period not wait, so will use just some big period value for wait if no timers
            auto timeToNextTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::duration::max());
            //auto timeToNextTimer = boost::chrono::milliseconds(1000000);

            std::unique_lock<std::mutex> lock(mMutex);

            if (event == nullptr)
            {
                if (mEvents.empty() && !mbStop)
                {
                    mCV.wait_for(lock, timeToNextTimer /*[this]() { return (!mEvents.empty()); } */);
                }

                if (mEvents.empty())
                    continue;

                event = mEvents.front();
                mEvents.pop();
            }
        }

        std::shared_ptr<ICallable> callable = event->getCallable();
        callable->call();
    }
}

} // namespace _private
}
