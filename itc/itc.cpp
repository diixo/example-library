
#include "itc.hpp"


namespace itc
{

const std::string UNKNOWN_THREAD_NAME = "_UNKNOWN_";

bool invoke(const std::string& threadName, std::shared_ptr<_private::ICallable> call)
{
    bool result = false;

    std::shared_ptr<_private::EventLoop> thread = _private::Dispatcher::getInstance().getThreadByName(threadName);

    if (thread == nullptr)
    {
        itc::logInfo() << "itc::invoke. unknow threadName: " << threadName;
    }
    else
    {
        thread->push(call);
        result = true;
    }
    return result;
}

void createEventLoop(const std::string& threadName)
{
   std::shared_ptr<_private::EventLoop> eventLoop = std::make_shared<_private::EventLoop>(threadName);
   _private::Dispatcher::getInstance().registerEventLoop(eventLoop);
}

void stopEventLoop(const std::string& threadName)
{
    //itc::logInfo("itc::stopEventLoop", threadName);
    auto eventLoop = _private::Dispatcher::getInstance().getThreadByName(threadName);
    if (eventLoop == nullptr)
    {
        itc::logInfo() << "itc::stopEventLoop. unknow threadName: " << threadName;
    }
    else
    {
        eventLoop->stop();
        _private::Dispatcher::getInstance().unregisterEventLoop(threadName);
    }
}

bool invoke(const itc::_private::CallBinder& callBinder)
{
    return invoke(callBinder.getThreadName(), callBinder.getCallable());
}

const std::string& currentThreadName()
{
    std::shared_ptr<_private::EventLoop> eventLoop = _private::Dispatcher::getInstance().getThreadById(std::this_thread::get_id());
    return eventLoop ? eventLoop->getThreadName() : UNKNOWN_THREAD_NAME;
}

void waitEventLoop(const std::string& thread_name)
{
   while (!itc::_private::Dispatcher::getInstance().isEmpty())
   {
      auto eventLoop = itc::_private::Dispatcher::getInstance().getThreadByName(thread_name);

      // waiting while all events finish responses by calling from parallel thread.
      if (eventLoop && (eventLoop->size() == 0))
      {
         break;
      }
   }
}

}
