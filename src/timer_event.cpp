
#include "timer_event.hpp"


namespace timer {

::queue::MBJobQueue MBTimerJobQueue::mTimerJobQueue;
std::recursive_mutex MBTimerEvent::mObserverLock;
std::map<MBTimer*, IMBTimerEventConsumer*> MBTimerEvent::mObservers;

MBTimerEventData::MBTimerEventData(MBTimer& timer)
    : mTimer(timer)
{
}

MBTimer& MBTimerEventData::getTimer() const
{
    return mTimer;
}

void MBTimerJobQueue::startExecution()
{
    mTimerJobQueue.startExecution();
}

void MBTimerJobQueue::stopExecution()
{
    mTimerJobQueue.stopExecution();
}

void MBTimerJobQueue::addJob(std::function<void()> job)
{
    mTimerJobQueue.addJob(std::move(job));
}

MBTimerEvent::MBTimerEvent(const MBTimerEventData& data)
    : mData(data)
{
}

MBTimerEvent* MBTimerEvent::createEvent(const MBTimerEventData& data)
{
    return new MBTimerEvent(data);
}

void MBTimerEvent::addListener(MBTimer* timer, IMBTimerEventConsumer* observer)
{
    std::lock_guard<std::recursive_mutex> lock(mObserverLock);
    if (mObservers.find(timer) == mObservers.end()) {
        mObservers.emplace(timer, observer);
    }
}

void MBTimerEvent::removeListener(MBTimer* timer)
{
    std::lock_guard<std::recursive_mutex> lock(mObserverLock);
    mObservers.erase(timer);
}

void MBTimerEvent::send(MBTimer* timer)
{
    MBTimerJobQueue::addJob([timer, this] {
        std::lock_guard<std::recursive_mutex> lock(mObserverLock);
        auto it = mObservers.find(timer);
        if (it != mObservers.end()) {
            it->second->processEvent(*this);
        }
        delete this;
    });
}

const MBTimerEventData& MBTimerEvent::data() const
{
    return mData;
}

} // namespace timer

