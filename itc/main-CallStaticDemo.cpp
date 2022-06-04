
#include "itc.hpp"
#include <memory>

namespace ns_CallStaticDemo
{
   static const std::string THREAD = "thread";

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

   void func1()
   {
      std::cout << itc::currentThreadName() << ": func1()" << std::endl;
   }

   void func2(int i)
   {
      std::cout << itc::currentThreadName() << ": func2(" << i << ")" << std::endl;
   }

   class SomeClass
   {
   public:
      static void func3(std::string s, int i, float f)
      {
         std::cout << itc::currentThreadName() << ": SomeClass::func3(" << s << ", " << i << ", " << f << ")" << std::endl;
      }

      static void func4(std::shared_ptr<A> a)
      {
         std::cout << itc::currentThreadName() << ": SomeClass::func4(" << a << ")" << std::endl;
      }
   };
}

DECLARE_STATIC_CALL(STATIC_CALL_func1, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func1)
DECLARE_STATIC_CALL(STATIC_CALL_func2, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func2, int)
DECLARE_STATIC_CALL(STATIC_CALL_func3, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::SomeClass::func3, std::string, int, float)
DECLARE_STATIC_CALL(STATIC_CALL_func4, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::SomeClass::func4, std::shared_ptr<ns_CallStaticDemo::A>)

int main()
{
   itc::logInfo() << "main_tid:" << std::this_thread::get_id();

   itc::createEventLoop(ns_CallStaticDemo::THREAD);

   itc::invoke(STATIC_CALL_func1::CallStatic());
   itc::invoke(STATIC_CALL_func2::CallStatic(5));
   itc::invoke(STATIC_CALL_func3::CallStatic("HELLO", 42, 5.5f));
   itc::invoke(STATIC_CALL_func4::CallStatic(std::make_shared<ns_CallStaticDemo::A>(33, "Hello A")));

   itc::invoke(itc::InlineCall<int, std::string>(ns_CallStaticDemo::THREAD, 
      [](int i, std::string s)
      {
         std::cout << itc::currentThreadName() << ": L: i=" << i << ", s=" << s << std::endl; 
      }, 7, "I'm in lambda"));

   //////////////////////////////////////////////////////////////////////////////////////////

   // wait current thread with blocking.
   itc::waitEventLoop(ns_CallStaticDemo::THREAD);

   itc::logInfo() << "return to main_tid << " << std::this_thread::get_id();

   return 0;
}
