
#include "itc.hpp"
#include <memory>

namespace ns_CallDemo
{
   static const std::string THREAD = "CallDemo_thread";

   struct I
   {
      virtual void vf(int i) = 0;
   };

   struct A : I
   {
      void func1()
      {
         std::cout << itc::currentThreadName() << ": " << "func1()" << std::endl;
      }

      void func2(int i)
      {
         std::cout << itc::currentThreadName() << ": " << "func2(" << i << ")" << std::endl;
      }

      void func3(std::string s, int i, float f)
      {
         std::cout << itc::currentThreadName() << ": " << "func3(" << s << ", " << i << ", " << f << ")" << std::endl;
      }

      void func4(std::shared_ptr<ns_CallDemo::A> pA)
      {
         std::cout << itc::currentThreadName() << ": " << "func4(" << pA << ")" << std::endl;
      }

      void vf(int i) override
      {
         std::cout << itc::currentThreadName() << ": " << "vf(" << i << ")" << std::endl;
      }

      A(int i, std::string s) : mI(i), mS(s) {}
      int mI;
      std::string mS;
   };

   std::ostream& operator<<(std::ostream &o, std::shared_ptr<A> a)
   {
      return o << "{i=" << a->mI << ", s=" << a->mS << "}";
   }
}


DECLARE_CALL(CALL_func1, ns_CallDemo::THREAD, ns_CallDemo::A, func1)
DECLARE_CALL(CALL_func2, ns_CallDemo::THREAD, ns_CallDemo::A, func2, int)
DECLARE_CALL(CALL_func3, ns_CallDemo::THREAD, ns_CallDemo::A, func3, std::string, int, float)
DECLARE_CALL(CALL_func4, ns_CallDemo::THREAD, ns_CallDemo::A, func4, std::shared_ptr<ns_CallDemo::A>)

DECLARE_CALL(CALL_funcV, ns_CallDemo::THREAD, ns_CallDemo::I, vf, int)

int main()
{
   itc::logInfo() << "main_tid:" << std::this_thread::get_id();

   //TraceManager::getInstance().registerStrategy("_UNKNOWN_", std::make_shared<DLTTraceStrategy>("DEMO", "Demo"));

   itc::createEventLoop(ns_CallDemo::THREAD);

   auto a = std::make_shared<ns_CallDemo::A>(123, "HELLO A");

   itc::invoke(CALL_func1::Call(a));
   itc::invoke(CALL_func2::Call(a, 42));
   itc::invoke(CALL_func3::Call(a, "Demo", 5, 3.14f));
   itc::invoke(CALL_func4::Call(a, a));

   itc::invoke(CALL_funcV::Call(a, 456));

   //////////////////////////////////////////////////////////////////////////////////////////

   // wait current thread with blocking.
   itc::waitEventLoop(ns_CallDemo::THREAD);

   itc::logInfo() << "return to main_tid << " << std::this_thread::get_id();

   return 0;
}
