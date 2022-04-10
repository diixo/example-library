
#include "LogInfo.hpp"
#include "Call.hpp"
#include "EventLoop.hpp"
#include "Dispatcher.hpp"


namespace ipc 
{
   void createEventLoop(const std::string& threadName)
   {
      std::shared_ptr<_private::EventLoop> eventLoop = std::make_shared<_private::EventLoop>(threadName);
      _private::Dispatcher::getInstance().registerEventLoop(eventLoop);
   }
}

int main()
{
   return 0;
}
