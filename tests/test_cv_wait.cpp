
// https://en.cppreference.com/w/cpp/thread/condition_variable/notify_all
// https://en.cppreference.com/w/cpp/thread/condition_variable/wait

#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
 
std::condition_variable cv;
std::mutex cv_m; // This mutex is used for three purposes:
                 // 1) to synchronize accesses to i
                 // 2) to synchronize accesses to std::cerr
                 // 3) for the condition variable cv
int i = 0;
 
void waits()
{
   std::unique_lock<std::mutex> lk(cv_m);
   std::cerr << "Waiting... \n";
   cv.wait(lk, []{return i == 1;});
   std::cerr << "...finished waiting. i == 1\n";
}
 
void signals()
{
   std::this_thread::sleep_for(std::chrono::seconds(1));
   {
      std::lock_guard<std::mutex> lk(cv_m);
      std::cerr << "Notifying...\n";
   }
   // this is the false notifying since the indicator is not updated
   cv.notify_all();
 
   std::this_thread::sleep_for(std::chrono::seconds(1));
 
   {
      std::lock_guard<std::mutex> lk(cv_m);
      i = 1;
      std::cerr << "Notifying again...\n";
   }
   // this notify is ok, since the indicator has been modified
   cv.notify_all();
}
 
int main()
{
   std::thread t1(waits), t2(waits), t3(waits), t4(signals);
   t1.join(); 
   t2.join(); 
   t3.join();
   t4.join();

   return 0;
}
////////////////////////////////////////////////////////////
//Output:
/*
Waiting...
Waiting...
Waiting...
Notifying...
Notifying again...
...finished waiting. i == 1
...finished waiting. i == 1
...finished waiting. i == 1
*/