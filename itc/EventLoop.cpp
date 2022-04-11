
#include <algorithm>
#include <chrono>

#include "LogInfo.hpp"
#include "EventLoop.hpp"
#include "Call.hpp"
#include "Event.hpp"


static const bool LOG_ENABLE = true;

namespace itc {
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
    if (LOG_ENABLE)
        itc::logInfo() << getThreadName() << " EventLoop::push";

    std::unique_lock <std::mutex> lock(mMutex);
    if (!mbStop)
    {
        mEvents.push(std::make_shared<Event>(call));
        mCV.notify_one();
    }
}


void EventLoop::stop()
{
#ifdef SYS_gettid
    itc::logInfo() << "EventLoop::stop(). " << mThreadName << "(" << getThreadId() << ", " << syscall(SYS_gettid) << ")";
#else
    itc::logInfo() << "EventLoop::stop(). " << mThreadName << "(" << getThreadId() << ")";
#endif
   {
      std::unique_lock <std::mutex> lock(mMutex);

      std::queue<std::shared_ptr<Event>> empty;
      std::swap(mEvents, empty);

      mbStop = true;
      mCV.notify_one();
   }
   if (LOG_ENABLE)
      itc::logInfo() << getThreadName() << " EventLoop::stopThread unlocked";

   mThread.join();

    if (LOG_ENABLE)
        itc::logInfo() << getThreadName() << " EventLoop::stopThread exit";
}

void EventLoop::run()
{
#ifdef SYS_gettid
    itc::logInfo() << "EventLoop::run() " << mThreadName << " tid=" << std::this_thread::get_id() << ", " << syscall(SYS_gettid) << ")";
#else
    itc::logInfo() << "EventLoop::run() " << mThreadName << " tid=" << std::this_thread::get_id();
#endif

    while (!mbStop)
    {
        if (LOG_ENABLE)
            itc::logInfo() << getThreadName() << " loop";

        std::shared_ptr<Event> event = nullptr;
        {
            // std::contidion_variable::wait_for with max period not wait, so will use just some big period value for wait if no timers
            //auto timeToNextTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::duration::max());
            auto timeToNextTimer = std::chrono::milliseconds(1000000);

            std::unique_lock<std::mutex> lock(mMutex);
            if (LOG_ENABLE)
                itc::logInfo() << getThreadName() << " pass lock, id=" << mThreadId;

            if (event == nullptr)
            {
                if (mEvents.empty() && !mbStop)
                {
                    if (LOG_ENABLE)
                        itc::logInfo() << getThreadName() << " wait_for " << timeToNextTimer.count();
                    mCV.wait_for(lock, timeToNextTimer /*, [this]() { return (!mEvents.empty()); } */);
                    if (LOG_ENABLE)
                        itc::logInfo() << getThreadName() << " exit wait_for " << timeToNextTimer.count();
                }

                if (mEvents.empty())
                    continue;

                if (LOG_ENABLE)
                   itc::logInfo() << getThreadName() << " event handling";

                event = mEvents.front();
                mEvents.pop();
            }
        }

        if (LOG_ENABLE)
            itc::logInfo() << getThreadName() << " got event and call. mEvents.size: " << mEvents.size();
        std::shared_ptr<ICallable> callable = event->getCallable();
        callable->call();
    }

    itc::logInfo() << "Exit event loop " << mThreadName;
}

} // namespace _private
}
