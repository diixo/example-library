
#include "timer_event.hpp"


namespace timer {

::queue::JobQueue TimerJobQueue::mTimerJobQueue;
std::recursive_mutex TimerEvent::mObserverLock;
std::map<Timer*, ITimerEventConsumer*> TimerEvent::mObservers;

TimerEventData::TimerEventData(Timer& timer)
    : mTimer(timer)
{
}

Timer& TimerEventData::getTimer() const
{
    return mTimer;
}

void TimerJobQueue::startExecution()
{
    mTimerJobQueue.startExecution();
}

void TimerJobQueue::stopExecution()
{
    mTimerJobQueue.stopExecution();
}

void TimerJobQueue::addJob(std::function<void()> job)
{
    mTimerJobQueue.addJob(std::move(job));
}

TimerEvent::TimerEvent(const TimerEventData& data)
    : mData(data)
{
}

TimerEvent* TimerEvent::createEvent(const TimerEventData& data)
{
    return new TimerEvent(data);
}

void TimerEvent::addListener(Timer* timer, ITimerEventConsumer* observer)
{
    std::lock_guard<std::recursive_mutex> lock(mObserverLock);
    if (mObservers.find(timer) == mObservers.end()) {
        mObservers.emplace(timer, observer);
    }
}

void TimerEvent::removeListener(Timer* timer)
{
    std::lock_guard<std::recursive_mutex> lock(mObserverLock);
    mObservers.erase(timer);
}

void TimerEvent::send(Timer* timer)
{
    TimerJobQueue::addJob([timer, this] {
        std::lock_guard<std::recursive_mutex> lock(mObserverLock);
        auto it = mObservers.find(timer);
        if (it != mObservers.end()) {
            it->second->processEvent(*this);
        }
        delete this;
    });
}

const TimerEventData& TimerEvent::data() const
{
    return mData;
}

} // namespace timer

