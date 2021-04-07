
#pragma once

#include "timer_event.hpp"

#include <cstdint>

namespace timer {

/**
 * @class Timer timer.hpp
 * @brief Implements Timer utility
 */
class MBTimer {
public:
    /**
     * Constructor
     * @param consumer Consumer of MBTimerEvent
     */
    explicit MBTimer(IMBTimerEventConsumer& consumer);
    /**
     * Destructor
     */
    ~MBTimer();
    /**
     * Deleted move constrcutor
     */
    MBTimer(MBTimer&&) = delete;
    /**
     * Deleted copy constrcutor
     */
    MBTimer(const MBTimer&) = delete;
    /**
     * Deleted move assignment operator
     */
    MBTimer& operator=(MBTimer&&) = delete;
    /**
     * Deleted copy assignment operator
     */
    MBTimer& operator=(const MBTimer&) = delete;
    /**
     * Compares timers based on the Timeout intervals
     * @result Result of comparison
     */
    bool operator>(const MBTimer& rhs) const;
    /**
     * Checks if timers are same
     * @result Result of comparison
     */
    bool operator==(const MBTimer& rhs) const;
    /**
     * Start timer
     * @param timeOut Time out when timer expires
     * @param eventsCount Number of MBTimerEvent expected in total
     */
    void start(uint32_t timeOut, uint32_t eventsCount);
    /**
     * Stop timer
     */
    void stop();
    /**
     * Checks if there are pending events to be sent
     */
    bool isActive() const;
    /**
     * Constant representing continuous events
     */
    static const uint32_t CONTINUOUSLY{0xFFFFFFFFU};

private:
    IMBTimerEventConsumer& mConsumer;
    std::thread mThread;
    mutable std::mutex mThreadMutex;
    std::mutex mConditonVariableMutex;
    std::condition_variable mConditionVariable;
    std::atomic<bool> mRunLoop{false};
    std::atomic<uint32_t> mEventsCount{0};
    std::atomic<uint32_t> mTimeout{0};
};

} // namespace timer
