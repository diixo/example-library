// condition_variable::wait (with predicate)
#include <iostream>           // std::cout
#include <thread>             // std::thread, std::this_thread::yield
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

////////////////////////////////////////////////////
#include <sstream>

class logInfo : public std::ostringstream
{
public:
   logInfo() = default;

   ~logInfo()
   {
      std::lock_guard<std::mutex> guard(_mutexPrint);
      std::cout << this->str() << std::endl;
   }

private:
   static std::mutex _mutexPrint;
};
std::mutex logInfo::_mutexPrint{};
/////////////////////////////////////////////////////

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
    std::unique_lock<std::mutex> lck(mtx);
    
    cv.wait(lck, shipment_available);
    // consume:
    logInfo() << cargo;
    cargo = 0;
  }
}

int main ()
{
  std::thread consumer_thread(consume, 10);

  // produce 10 items when needed:
  for (int i = 0; i < 10; ++i)
  {
    while (shipment_available())
      std::this_thread::yield();

    std::unique_lock<std::mutex> lck(mtx);
    cargo = i + 1;
    cv.notify_one();
  }

  consumer_thread.join();

  return 0;
}
