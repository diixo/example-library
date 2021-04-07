
#pragma once

#include <map>
#include <mutex>
#include <thread>
#include <functional>

#include "job_queue.hpp"

namespace timer {

class MBTimer;
class MBTimerEvent;

class IMBTimerEventConsumer {
public:
    /**
     * Process Event
     * @param event MBTimerEvent object
     */
    virtual void processEvent(const MBTimerEvent& event) = 0;
};

/**
 * @class MBTimerEventData timer_event.hpp
 * @brief Implements data class to be sent in MBTimerEvent
 */
class MBTimerEventData {
public:
    /**
     * Constructor
     * @param timer MBTimer object
     */
    explicit MBTimerEventData(MBTimer& timer);
    /**
     * Default Destructor
     */
    ~MBTimerEventData() = default;
    /**
     * Get reference of MBTimer
     * @result Reference of MBTimer
     */
    MBTimer& getTimer() const;

private:
    MBTimer& mTimer;
};

class MBTimerEvent {
public:
    /**
     * Constructor
     * @param data MBTimerEventData object
     */
    explicit MBTimerEvent(const MBTimerEventData& data);
    /**
     * Default Destructor
     */
    ~MBTimerEvent() = default;
    /**
     * createEvent
     * @param data MBTimerEventData object
     */
    const MBTimerEventData& data() const;

private:
    static MBTimerEvent* createEvent(const MBTimerEventData& data);
    /**
     * Add Listener
     * @param timer MBTimer pointer
     * @param observer IMBTimerEventConsumer pointer
     */
    static void addListener(MBTimer* timer, IMBTimerEventConsumer* observer);
    /**
     * Remove Listener
     * @param timer MBTimer pointer
     */
    static void removeListener(MBTimer* timer);
    /**
     * Send Event
     * @param timer MBTimer pointer
     */
    void send(MBTimer* timer);
    /**
     * Get Timer Event Data
     * @return MBTimerEventData
     */

    const MBTimerEventData mData;
    static std::map<MBTimer*, IMBTimerEventConsumer*> mObservers;
    static std::recursive_mutex mObserverLock;

    friend MBTimer;
};

class MBTimerJobQueue {
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

    static ::queue::MBJobQueue mTimerJobQueue;

    friend MBTimerEvent;
};

} // namespace timer

