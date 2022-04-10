
#include "LogInfo.hpp"
#include "Call.hpp"
#include "EventLoop.hpp"
#include "Dispatcher.hpp"

namespace ipc
{
   // Create thread and register in Dispatcher, threadName should be UNIQE for all application
   void createEventLoop(const std::string& threadName);
}
