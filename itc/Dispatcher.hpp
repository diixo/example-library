
#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace itc {
namespace _private {

class EventLoop;

class Dispatcher
{
public:
    static Dispatcher& getInstance()
    {
        static Dispatcher instance;
        return instance;
    }

    void registerEventLoop(std::shared_ptr<EventLoop> eventLoop);
    void unregisterEventLoop(const std::string& name);

    void printThreads() const;

    bool isEmpty() const;

    std::shared_ptr<EventLoop> getThreadByName(const std::string& name) const;
    std::shared_ptr<EventLoop> getThreadById(const std::thread::id& id) const;

private:
    Dispatcher();
    static std::recursive_mutex gMutex;
    std::map<std::thread::id, std::shared_ptr<EventLoop>> mThreads;
};

} // namespace _private
} // namespace itc
