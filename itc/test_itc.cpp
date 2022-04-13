
#include "itc.hpp"
#include <atomic>

const std::string thread_name1("thread1");
std::atomic<int> aCounter = 0;

struct A
{
   A(int i, std::string s) : mI(i), mS(s) {}
   int mI;
   std::string mS;
};

std::ostream& operator<<(std::ostream &o, std::shared_ptr<A> a)
{
   return o << "{i=" << a->mI << ", s=" << a->mS << "}";
}

struct EventConsumer
{
   void func1()
   {
      std::cout << itc::currentThreadName() << ": EventConsumer::func1()" << std::endl;
      aCounter++;
   }

   void func2(int i)
   {
      std::cout << itc::currentThreadName() << ": EventConsumer::func2(" << i << ")" << std::endl;
      aCounter++;
   }

   void func3(std::string s, int i, float f)
   {
      std::cout << itc::currentThreadName() << ": EventConsumer::func3(" << s << ", " << i << ", " << f << ")" << std::endl;
      aCounter++;
   }

   void func4(std::shared_ptr<A> a)
   {
      std::cout << itc::currentThreadName() << ": EventConsumer::func4(" << a << ")" << std::endl;
      aCounter++;
   }
};

DECLARE_EVENT(EVENT_func1, EventConsumer, func1)
DECLARE_EVENT(EVENT_func2, EventConsumer, func2, int)
DECLARE_EVENT(EVENT_func3, EventConsumer, func3, std::string, int, float)
DECLARE_EVENT(EVENT_func4, EventConsumer, func4, std::shared_ptr<A>)

// fill events list to call into consumer from another thread 
void startEventDemo()
{
   auto consumer = std::make_shared<EventConsumer>();

   itc::invoke(EVENT_func1::Event(consumer));
   itc::invoke(EVENT_func2::Event(consumer, 5));
   itc::invoke(EVENT_func3::Event(consumer, "HELLO", 42, 5.5f));
   itc::invoke(EVENT_func4::Event(consumer, std::make_shared<A>(33, "Hello A")));
}

DECLARE_STATIC_CALL(STATIC_CALL_ED_start, thread_name1, startEventDemo)


int main()
{
   itc::logInfo() << "main_tid:" << std::this_thread::get_id();

   itc::createEventLoop(thread_name1);

   itc::invoke(STATIC_CALL_ED_start::CallStatic());

   while (!itc::_private::Dispatcher::getInstance().isEmpty())
   {
      // waiting while all events finish responses by calling from parallel thread.
      if (aCounter == 4)
      {
         break;
      }
   }

   itc::logInfo() << "return to main_tid << " << std::this_thread::get_id();

   return 0;
}
