// example for thread::join
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "LogInfo.hpp"

void pause_thread(int n)
{
   std::this_thread::sleep_for(std::chrono::seconds(n));
   logInfo() << "pause <<-" << n;
}

int main()
{
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
Spawning 3 threads...
Done spawning threads.Now waiting for them to join :
pause << -1
<< 1
pause << -2
<< 2
pause << -3
<< 3
All threads joined!
*/
