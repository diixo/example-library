// this_thread::yield example
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::yield
#include <atomic>         // std::atomic

const int sz = 8;
std::atomic<bool> ready(false);

void count1m(int id)
{
   while (!ready) // wait until main() sets ready...
   {
      std::this_thread::yield();
   }
   for (volatile int i = 0; i < 1000000; ++i) {}
   std::cout << id;
}

int main ()
{
   std::thread threads[sz];
   std::cout << "race of 10 threads that count to 1 million:\n";

   for (int i = 0; i < sz; ++i)
   {
      threads[i] = std::thread(count1m, i);
   }
   ready = true;               // go!
   for (auto& th : threads)
   {
      th.join();
   }
   std::cout << " <<-\n";

   return 0;
}
