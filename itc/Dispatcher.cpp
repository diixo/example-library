
#include "LogInfo.hpp"
#include "Dispatcher.hpp"

#include <algorithm>
#include <iostream>
#include <thread>

#include "EventLoop.hpp"


namespace itc {
namespace _private {

std::recursive_mutex Dispatcher::gMutex;

Dispatcher::Dispatcher()
   : mThreads()
{
}

void Dispatcher::registerEventLoop(std::shared_ptr<EventLoop> eventLoop)
{
   std::lock_guard<std::recursive_mutex> lock(gMutex);
   const std::thread::id& id = eventLoop->getThreadId();
   if (mThreads.find(id) == mThreads.end())
   {
      mThreads[id] = eventLoop;
      itc::logInfo() << "Dispatcher::registerEventLoop " << id << " " << eventLoop->getThreadName() << " size=" << mThreads.size();
   }
}

void Dispatcher::unregisterEventLoop(const std::string& name)
{
   std::lock_guard<std::recursive_mutex> lock(gMutex);
   mThreads.erase(getThreadByName(name)->getThreadId());
}

void Dispatcher::printThreads() const
{
   std::lock_guard<std::recursive_mutex> lock(gMutex);
   for (auto thread : mThreads)
   {
      itc::logInfo() << thread.first << ':' << thread.second->getThreadName() << " " << thread.second->getThreadId();
   }
}

std::shared_ptr<EventLoop> Dispatcher::getThreadById(const std::thread::id& id) const
{
   std::lock_guard<std::recursive_mutex> lock(gMutex);
   auto it = mThreads.find(id);
   if (it != mThreads.end())
   {
      return it->second;
   }
   else
   {
      return nullptr;
   }
}

std::shared_ptr<EventLoop> Dispatcher::getThreadByName(const std::string& name) const
{
   std::lock_guard<std::recursive_mutex> lock(gMutex);

   auto it = std::find_if(mThreads.begin(), mThreads.end(), [name](const std::pair<std::thread::id, std::shared_ptr<EventLoop>>& p)
   {
      return p.second->getThreadName() == name;
   });

   if (it != mThreads.end())
   {
      return it->second;
   }
   else
   {
      return nullptr;
   }
}
} // namespace _private
} // namespace itc
