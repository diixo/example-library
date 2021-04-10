
#include "timer.hpp"
#include "trace_macro.hpp"

namespace timer {

Timer::Timer(ITimerEventConsumer& consumer)
    : mConsumer(consumer)
{
    LOG_DEBUG("Timer::Timer, entered");
    TimerEvent::addListener(this, &consumer);
}

Timer::~Timer()
{
    LOG_DEBUG("Timer::~Timer, entered");
    TimerEvent::removeListener(this);
    stop();
}

bool Timer::isActive() const
{
    std::lock_guard<std::mutex> lock(mThreadMutex);
    return mEventsCount > 0;
}

bool Timer::operator>(const Timer& rhs) const
{
    return mTimeout > rhs.mTimeout;
}

bool Timer::operator==(const Timer& rhs) const
{
    return this == &rhs;
}

void Timer::start(uint32_t timeOut, uint32_t eventsCount)
{
    LOG_DEBUG("Timer::start entered, timeout = %d, eventsCount=%d", timeOut, eventsCount);
    if (0 == timeOut || 0 == eventsCount) {
        return;
    }

    // Joining the thread before start
    stop();

    std::lock_guard<std::mutex> lock(mThreadMutex);
    mTimeout = timeOut;
    mEventsCount = eventsCount;
    mRunLoop = true;
    mThread = std::thread([this]()
    {
        while (mRunLoop) {
            std::unique_lock<std::mutex> lock(mConditonVariableMutex);
            std::cv_status timeoutStatus = std::cv_status::no_timeout;
            if (mRunLoop) {
                timeoutStatus
                    = mConditionVariable.wait_for(lock, std::chrono::milliseconds(mTimeout.load()));
            }
            if (std::cv_status::timeout == timeoutStatus) {
                LOG_DEBUG("Timer::thread timed out after %d milliseconds", mTimeout.load());
                TimerEvent* event = TimerEvent::createEvent(TimerEventData(*this));
                event->send(this);
                if (CONTINUOUSLY != mEventsCount) {
                    if (0 == --mEventsCount) {
                        LOG_DEBUG("Timer::thread, no events pending. Exiting loop");
                        mRunLoop = false;
                    }
                }
            } else {
                LOG_DEBUG(
                    "Timer::thread with timeout %d milliseconds has been stopped",
                    mTimeout.load());
            }
        }
        mEventsCount = 0;
    });
    LOG_DEBUG("Timer::start, started the loop");
}

void Timer::stop()
{
    LOG_DEBUG("Timer::stop entered");
    std::lock_guard<std::mutex> lock(mThreadMutex);
    mRunLoop = false;
    {
        std::lock_guard<std::mutex> lock(mConditonVariableMutex);
        mConditionVariable.notify_one();
    }
    if (mThread.joinable()) {
        mThread.join();
    }
    LOG_DEBUG("Timer::stop, joined thread");
}

} // namespace timer

