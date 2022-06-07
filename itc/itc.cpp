
#include "itc.hpp"
#include "App.hpp"


namespace itc
{

const std::string UNKNOWN_THREAD_NAME = "_UNKNOWN_";

bool invoke(const std::string& threadName, std::shared_ptr<_private::ICallable> call)
{
    bool result = false;

    std::shared_ptr<_private::EventLoop> thread = App::GetInstance().Dispatcher().getThreadByName(threadName);

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
   App::GetInstance().Dispatcher().registerEventLoop(eventLoop);
}

void stopEventLoop(const std::string& threadName)
{
    //itc::logInfo("itc::stopEventLoop", threadName);
    auto eventLoop = App::GetInstance().Dispatcher().getThreadByName(threadName);
    if (eventLoop == nullptr)
    {
        itc::logInfo() << "itc::stopEventLoop. unknow threadName: " << threadName;
    }
    else
    {
        eventLoop->stop();
        App::GetInstance().Dispatcher().unregisterEventLoop(threadName);
    }
}

bool invoke(const itc::_private::CallBinder& callBinder)
{
    return invoke(callBinder.getThreadName(), callBinder.getCallable());
}

const std::string& currentThreadName()
{
    std::shared_ptr<_private::EventLoop> eventLoop = App::GetInstance().Dispatcher().getThreadById(std::this_thread::get_id());
    return eventLoop ? eventLoop->getThreadName() : UNKNOWN_THREAD_NAME;
}

void waitEventLoop(const std::string& thread_name)
{
   while (!App::GetInstance().Dispatcher().isEmpty())
   {
      auto eventLoop = App::GetInstance().Dispatcher().getThreadByName(thread_name);

      // waiting while all events finish responses by calling from parallel thread.
      if (eventLoop && (eventLoop->size() == 0))
      {
         break;
      }
   }
}

}
