
#include "LogInfo.hpp"
#include "ICallable.hpp"
#include "CallBinder.hpp"
#include "Call.hpp"
#include "EventLoop.hpp"
#include "Dispatcher.hpp"

namespace itc
{
   // Create thread and register in Dispatcher, threadName should be UNIQE for all application
   void createEventLoop(const std::string& threadName);
}
