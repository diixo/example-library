
#include "itc.hpp"

const std::string str_thread1("thread1");
const std::string str_thread2("thread2");

int main()
{
   itc::createEventLoop(str_thread1);
   itc::createEventLoop(str_thread2);

   while (!itc::_private::Dispatcher::getInstance().isEmpty())
   {
      std::thread::id main_thread_id = std::this_thread::get_id();
      itc::logInfo() << "main_thread_id:" << main_thread_id;
   }

   return 0;
}
