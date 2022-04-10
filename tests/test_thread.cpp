
// this_thread::yield example
#include <iostream>
#include <thread>
#include <atomic>

const int sz = 8;
std::atomic<bool> ready(false);

void count1m(int id)
{
   while (!ready) // wait until main() sets ready...
   {
      // binded thread waits for other threads to advance without blocking.
      std::this_thread::yield();
   }
   for (volatile int i = 0; i < 1000000; ++i) {}
   std::cout << id;
}

int main ()
{
   std::thread threads[sz];
   std::cout << "race of threads that count to 1 million:\n";

   for (int i = 0; i < sz; ++i)
   {
      // set thread as joinable
      threads[i] = std::thread(count1m, i);
   }
   ready = true;               // go!
   for (auto& th : threads)
   {
      // blocks the execution of the thread that calls this function 
      // until the function called on construction returns (if it hasn't yet).
      th.join();
   }
   std::cout << " <<-\n";

   return 0;
}
