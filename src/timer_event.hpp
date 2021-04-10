
#pragma once

#include <map>
#include <mutex>
#include <thread>
#include <functional>

#include "job_queue.hpp"

namespace timer {

class Timer;
class TimerEvent;

class ITimerEventConsumer {
public:
    /**
     * Process Event
     * @param event TimerEvent object
     */
    virtual void processEvent(const TimerEvent& event) = 0;
};

/**
 * @class TimerEventData timer_event.hpp
 * @brief Implements data class to be sent in TimerEvent
 */
class TimerEventData {
public:
    /**
     * Constructor
     * @param timer Timer object
     */
    explicit TimerEventData(Timer& timer);
    /**
     * Default Destructor
     */
    ~TimerEventData() = default;
    /**
     * Get reference of Timer
     * @result Reference of Timer
     */
    Timer& getTimer() const;

private:
    Timer& mTimer;
};

class TimerEvent {
public:
    /**
     * Constructor
     * @param data TimerEventData object
     */
    explicit TimerEvent(const TimerEventData& data);
    /**
     * Default Destructor
     */
    ~TimerEvent() = default;
    /**
     * createEvent
     * @param data TimerEventData object
     */
    const TimerEventData& data() const;

private:
    static TimerEvent* createEvent(const TimerEventData& data);
    /**
     * Add Listener
     * @param timer Timer pointer
     * @param observer ITimerEventConsumer pointer
     */
    static void addListener(Timer* timer, ITimerEventConsumer* observer);
    /**
     * Remove Listener
     * @param timer Timer pointer
     */
    static void removeListener(Timer* timer);
    /**
     * Send Event
     * @param timer Timer pointer
     */
    void send(Timer* timer);
    /**
     * Get Timer Event Data
     * @return TimerEventData
     */

    const TimerEventData mData;
    static std::map<Timer*, ITimerEventConsumer*> mObservers;
    static std::recursive_mutex mObserverLock;

    friend Timer;
};

class TimerJobQueue {
public:
    /**
     * Start Execution of Job Queue
     */
    static void startExecution();
    /**
     * Stop Execution of Job Queue
     */
    static void stopExecution();

private:
    /**
     * Add Job to Job Queue
     * @param job std::function<void()>
     */
    static void addJob(std::function<void()> job);

    static ::queue::JobQueue mTimerJobQueue;

    friend TimerEvent;
};

} // namespace timer

