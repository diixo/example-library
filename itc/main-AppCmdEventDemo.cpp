
#include "itc.hpp"
#include "App.hpp"
#include "Event.hpp"

const std::string thread_name1("thread1");

struct EventConsumer
{
   void func1()
   {
      std::cout << itc::currentThreadName() << " --EventConsumer::func1()" << std::endl;
   }

   void func2(int i)
   {
      std::cout << itc::currentThreadName() << " --EventConsumer::func2(" << i << ")" << std::endl;
   }
};

DECLARE_EVENT(EVENT_func1, EventConsumer, func1)
DECLARE_EVENT(EVENT_func2, EventConsumer, func2, int)
DECLARE_STATIC_EVENT(EVENT_App_Shutdown, App::PushEvent, int, EventPriority, std::vector<uint8_t>)

// fill events-list to call consumer-methods from another thread 
void startEventDemo()
{
   auto consumer = std::make_shared<EventConsumer>();

   // append new events from thread_name1, which is current-thread for each event.
   App::invoke(EVENT_func1::Event(consumer));
   App::invoke(EVENT_func2::Event(consumer, 5));

   // push shutdown-command to stop App-thread. It will be handled in App::processQueue()
   App::invoke(EVENT_App_Shutdown::Event(App::CMD_APP_SHUTDOWN, PRIORITY_MAX, std::vector<uint8_t>()));
}

DECLARE_STATIC_CALL(STATIC_CALL_ED_start, thread_name1, startEventDemo)

int main()
{
   itc::logInfo() << "main_tid:" << std::this_thread::get_id();

   App::createEventLoop(thread_name1);

   App::invoke(STATIC_CALL_ED_start::CallStatic());

   // wait current thread with blocking.
   App::GetInstance().start();   // use instead of App::waitEventLoop(thread_name1).

   itc::logInfo() << "return to main_tid << " << std::this_thread::get_id();

   return 0;
}
