
#pragma once

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>


namespace itc {
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

   // return amount of active events, that not finished yet.
   size_t size() const;

private:

   void run();

   std::thread mThread;
   bool mbStop;
   std::thread::id mThreadId;
   const std::string mThreadName;

   std::queue<std::shared_ptr<Event>> mEvents;

   std::mutex mMutex;
   std::condition_variable mCV;

   std::atomic<size_t> mSize;
};

inline size_t EventLoop::size() const
{
   return mSize;
}

} // namespace _private
}
