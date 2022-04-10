
#pragma once

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>


namespace ipc {
namespace _private {

class ICallable;
class Event;
class Call;

class EventLoop
{
public:
    explicit EventLoop(const std::string& threadName);
    ~EventLoop() = default;
    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    void stop();

    std::thread::id getThreadId() const;
    const std::string& getThreadName() const;
    static std::thread::id getCurrentThreadId();

    void push(std::shared_ptr<ICallable> call);

private:

    void run();

    std::thread mThread;
    bool mbStop;
    std::thread::id mThreadId;
    const std::string mThreadName;

    std::queue<std::shared_ptr<Event>> mEvents;

    std::mutex mMutex;
    std::condition_variable mCV;
};

} // namespace _private
}
