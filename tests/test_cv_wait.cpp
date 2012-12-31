// condition_variable::wait (with predicate)
#include <iostream>           // std::cout
#include <thread>             // std::thread, std::this_thread::yield
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#include "LogInfo.hpp"

std::mutex mtx;
std::condition_variable cv;

int cargo = 0;

bool shipment_available()
{
   return cargo != 0;
}

void consume (int n)
{
   for (int i = 0; i < n; ++i)
   {
      logInfo() << "consume cargo>>" << cargo;

      std::unique_lock<std::mutex> lck(mtx);
    
      cv.wait(lck, shipment_available);
      // consume:
      logInfo() << "consume cargo<<" << cargo;
      cargo = 0;
   }
}

int main ()
{
   std::thread consumer_thread(consume, 10);

   std::thread::id main_thread_id = std::this_thread::get_id();
   logInfo() << "main_thread_id:" << main_thread_id;

   // produce 10 items when needed:
   for (int i = 0; i < 10; ++i)
   {
      while (shipment_available())
      {
         logInfo() << "yield>>";
         std::this_thread::yield();
         logInfo() << "yield<<";
      }

      std::unique_lock<std::mutex> lck(mtx);
      cargo = i + 1;
      cv.notify_one();
   }

   consumer_thread.join();

   return 0;
}
