// example for thread::join
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "LogInfo.hpp"

bool run = true;

void thrx()
{
   while (run)
   {
      logInfo() << "active...";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      //std::this_thread::yield();
   }
}

void pause_thread(int n)
{
   std::this_thread::sleep_for(std::chrono::seconds(n));
   logInfo() << "pause <<-" << n;
}

int main()
{
   std::thread t(thrx);
   t.join();

   // run never reseted, because blocked by join.
   // Sleep (or yield) switched between started thread and main thread, and never continue to reset run-variable.
   run = false;

   std::cout << "Spawning 3 threads...\n";
   std::thread t1(pause_thread, 1);
   std::thread t2(pause_thread, 2);
   std::thread t3(pause_thread, 3);

   logInfo() << "Done spawning threads. Now waiting for them to join:";

   // Blocks the current thread, until the function-thread finishes its execution, and returns with the completion of all the operations in the thread
   // Each next thread start join, when only previous finished and returned to join() as entry-point.
   t1.join();
   logInfo() << "<<1";
   t2.join();
   logInfo() << "<<2";
   t3.join();
   logInfo() << "<<3";

   logInfo() << "All threads joined!\n";

   return 0;
}
/*
active...
active...
active...
active...
active...
active...
*/
