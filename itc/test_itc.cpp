
#include "itc.hpp"

const std::string thread_name1("thread1");

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
      std::cout << itc::currentThreadName() << " --EventConsumer::func1()" << std::endl;
   }

   void func2(int i)
   {
      std::cout << itc::currentThreadName() << " --EventConsumer::func2(" << i << ")" << std::endl;
   }

   void func3(std::string s, int i, float f)
   {
      std::cout << itc::currentThreadName() << " --EventConsumer::func3(" << s << ", " << i << ", " << f << ")" << std::endl;
   }

   void func4(std::shared_ptr<A> a)
   {
      std::cout << itc::currentThreadName() << " --EventConsumer::func4(" << a << ")" << std::endl;
   }
};

DECLARE_EVENT(EVENT_func1, EventConsumer, func1)
DECLARE_EVENT(EVENT_func2, EventConsumer, func2, int)
DECLARE_EVENT(EVENT_func3, EventConsumer, func3, std::string, int, float)
DECLARE_EVENT(EVENT_func4, EventConsumer, func4, std::shared_ptr<A>)

// fill events list to call consumer-methods from another thread 
void startEventDemo()
{
   auto consumer = std::make_shared<EventConsumer>();

   itc::invoke(EVENT_func1::Event(consumer));
   itc::invoke(EVENT_func2::Event(consumer, 5));
   itc::invoke(EVENT_func3::Event(consumer, "HELLO", 42, 5.5f));
   itc::invoke(EVENT_func4::Event(consumer, std::make_shared<A>(33, "Hello A")));
}

DECLARE_STATIC_CALL(STATIC_CALL_ED_start, thread_name1, startEventDemo)

// common characters sequence, which consists only of characters that are part of str2.
int strnspn(const char * s1, const char * s2)
{
   const char * pch = s1;
   int n = 0;

   while (*s1 && *s2)
   {
      pch = strchr(s2, *s1);
      if (pch)
      {
         n++;
      }
      ++s1;
   }
   return n;
}

// commonly biggest substring, which consists only of characters that are part of str2.
// return number of common chars sequence
int strncstr(const char * s1, const char * s2, const int nlimit = 1)
{
   const char * pch = s2;
   int n = 0;
   int nmax = 0;

   while (*s1)
   {
      if (*pch == *s1)
      {
         ++n;
         ++pch;
      }
      else
      {
         pch = s2; // TODO:?
         n = 0;
         const char *search = strchr(pch, *s1);
         if (search)
         {
            pch = search;
            ++pch;
            ++n;
         }
      }
      if (nmax < n && n > nlimit)
      {
         //printf("substr=%d\n", n);
         nmax = n;
      }
      ++s1;
   }
   return nmax;
}


// commonly biggest substring, which consists only of characters that are part of str2.
std::string strsstr(const char * s1, const char * s2)
{
   const char * pch = s2;
   int n = 0;
   int nmax = 0;
   //////////////////////////////////////////////////////////////////////////
   const char* search = pch;
   const char* result = 0;

   while (*s1)
   {
      if (*pch == *s1)
      {
         ++n;
         ++pch;
      }
      else
      {
         pch = s2; // TODO:?
         n = 0;
         search = strchr(pch, *s1);
         if (search)
         {
            pch = search;
            ++pch;
            ++n;
         }
      }
      if (nmax < n)
      {
         result = search;
         nmax = n;
      }
      ++s1;
   }
   return (nmax > 1) ? std::string(result, nmax) : std::string();
}

void test()
{
   int i = 0;
   char strtext[] = "121236dd9033ww903332b903333f";
   char cset[] = "1234903333e";

   //char strtext[] = "132a39";
   //char cset[] = "1023";

   i = strncstr(strtext, cset);
   printf("The number chars of common substring:%d\n", i);
   std::cout << strsstr(strtext, cset) << std::endl;

   i = strnspn(strtext, cset);
   printf("The number of common symbols:%d\n", i);
   return;
}

int main()
{
   itc::logInfo() << "main_tid:" << std::this_thread::get_id();

   itc::createEventLoop(thread_name1);

   itc::invoke(STATIC_CALL_ED_start::CallStatic());

   while (!itc::_private::Dispatcher::getInstance().isEmpty())
   {
      auto event = itc::_private::Dispatcher::getInstance().getThreadByName(thread_name1);

      // waiting while all events finish responses by calling from parallel thread.
      if (event && (event->size() == 0))
      {
         break;
      }
   }

   itc::logInfo() << "return to main_tid << " << std::this_thread::get_id();

   return 0;
}
