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
      std::this_thread::yield();
   }
   logInfo() << "inactive";
}

void pause_thread(int n)
{
   logInfo() << "p ->>" << n;
   std::this_thread::sleep_for(std::chrono::milliseconds(n));
   logInfo() << "p <<-" << n;
}

int main()
{
   std::thread t(thrx);
   t.detach();

   logInfo() << "Spawning 3 threads...\n";
   std::thread t1(pause_thread, 10);

   logInfo() << "Done spawning threads. Now waiting for them to join:\n";

   // Blocks the current thread, until the function-thread finishes its execution, and returns with the completion of all the operations in the thread
   t1.join();

   run = false;

   // call sleep to re-switch into another thread, with enough time, while its finished.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));

   logInfo() << "All threads joined!\n";

   return 0;
}
