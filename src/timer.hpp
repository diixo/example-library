
#pragma once

#include "timer_event.hpp"

#include <cstdint>

namespace timer {

/**
 * @class Timer timer.hpp
 * @brief Implements Timer utility
 */
class Timer {
public:
    /**
     * Constructor
     * @param consumer Consumer of MBTimerEvent
     */
    explicit Timer(ITimerEventConsumer& consumer);
    /**
     * Destructor
     */
    ~Timer();
    /**
     * Deleted move constrcutor
     */
    Timer(Timer&&) = delete;
    /**
     * Deleted copy constrcutor
     */
    Timer(const Timer&) = delete;
    /**
     * Deleted move assignment operator
     */
    Timer& operator=(Timer&&) = delete;
    /**
     * Deleted copy assignment operator
     */
    Timer& operator=(const Timer&) = delete;
    /**
     * Compares timers based on the Timeout intervals
     * @result Result of comparison
     */
    bool operator>(const Timer& rhs) const;
    /**
     * Checks if timers are same
     * @result Result of comparison
     */
    bool operator==(const Timer& rhs) const;
    /**
     * Start timer
     * @param timeOut Time out when timer expires
     * @param eventsCount Number of TimerEvent expected in total
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
    ITimerEventConsumer& mConsumer;
    std::thread mThread;
    mutable std::mutex mThreadMutex;
    std::mutex mConditonVariableMutex;
    std::condition_variable mConditionVariable;
    std::atomic<bool> mRunLoop{false};
    std::atomic<uint32_t> mEventsCount{0};
    std::atomic<uint32_t> mTimeout{0};
};

} // namespace timer
