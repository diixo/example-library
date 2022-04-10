
#include <mutex>
#include <sstream>
#include <iostream>

namespace ipc
{
   class logInfo : public std::ostringstream
   {
   public:
      logInfo() = default;

      ~logInfo()
      {
         std::lock_guard<std::mutex> guard(_mutexPrint);
         std::cout << this->str();
      }

   private:
      static std::mutex _mutexPrint;
   };
}
