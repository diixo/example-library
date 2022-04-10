
#include "LogInfo.hpp"
#include "ICallable.hpp"
#include "CallBinder.hpp"
#include "Call.hpp"
#include "EventLoop.hpp"
#include "Dispatcher.hpp"

namespace itc
{
   bool invoke(const std::string& threadName, std::shared_ptr<_private::ICallable> call);

   // Create thread and register in Dispatcher, threadName should be UNIQE for all application
   void createEventLoop(const std::string& threadName);

   void stopEventLoop(const std::string& threadName);

   bool invoke(const itc::_private::CallBinder& callBinder);

   const std::string& currentThreadName();
}
