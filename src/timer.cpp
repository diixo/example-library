
#include "timer.hpp"
#include "trace_macro.hpp"

namespace timer {

MBTimer::MBTimer(IMBTimerEventConsumer& consumer)
    : mConsumer(consumer)
{
    LOG_DEBUG("MBTimer::MBTimer, entered");
    MBTimerEvent::addListener(this, &consumer);
}

MBTimer::~MBTimer()
{
    LOG_DEBUG("MBTimer::~MBTimer, entered");
    MBTimerEvent::removeListener(this);
    stop();
}

bool MBTimer::isActive() const
{
    std::lock_guard<std::mutex> lock(mThreadMutex);
    return mEventsCount > 0;
}

bool MBTimer::operator>(const MBTimer& rhs) const
{
    return mTimeout > rhs.mTimeout;
}

bool MBTimer::operator==(const MBTimer& rhs) const
{
    return this == &rhs;
}

void MBTimer::start(uint32_t timeOut, uint32_t eventsCount)
{
    LOG_DEBUG("MBTimer::start entered, timeout = %d, eventsCount=%d", timeOut, eventsCount);
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
                LOG_DEBUG("MBTimer::thread timed out after %d milliseconds", mTimeout.load());
                MBTimerEvent* event = MBTimerEvent::createEvent(MBTimerEventData(*this));
                event->send(this);
                if (CONTINUOUSLY != mEventsCount) {
                    if (0 == --mEventsCount) {
                        LOG_DEBUG("MBTimer::thread, no events pending. Exiting loop");
                        mRunLoop = false;
                    }
                }
            } else {
                LOG_DEBUG(
                    "MBTimer::thread with timeout %d milliseconds has been stopped",
                    mTimeout.load());
            }
        }
        mEventsCount = 0;
    });
    LOG_DEBUG("MBTimer::start, started the loop");
}

void MBTimer::stop()
{
    LOG_DEBUG("MBTimer::stop entered");
    std::lock_guard<std::mutex> lock(mThreadMutex);
    mRunLoop = false;
    {
        std::lock_guard<std::mutex> lock(mConditonVariableMutex);
        mConditionVariable.notify_one();
    }
    if (mThread.joinable()) {
        mThread.join();
    }
    LOG_DEBUG("MBTimer::stop, joined thread");
}

} // namespace timer

