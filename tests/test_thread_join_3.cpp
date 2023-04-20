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
}

void pause_thread(int n)
{
   logInfo() << "p ->>" << n;
   std::this_thread::sleep_for(std::chrono::seconds(n));
   logInfo() << "p <<-" << n;
}

void stop_thread()
{
   logInfo() << "stop ->>";
   run = false;
}

int main()
{
   std::thread t(thrx);
   t.detach();

   std::cout << "Spawning 3 threads...\n";
   std::thread t1(pause_thread, 1);
   std::thread t2(pause_thread, 1);
   std::thread t3(pause_thread, 1);

   std::cout << "Done spawning threads. Now waiting for them to join:\n";

   t1.join();
   // Blocks the current thread, until the function-thread finishes its execution, and returns with the completion of all the operations in the thread
   t2.join();
   t3.join();

   std::thread st(stop_thread);
   st.join();
   std::cout << "All threads joined!\n";

   return 0;
}
