
#pragma once

#include <mutex>
#include <sstream>
#include <iostream>

namespace itc
{
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

   class logMethod : public std::ostringstream
   {
   public:
      logMethod() = default;

      ~logMethod()
      {
         std::lock_guard<std::mutex> guard(_mutexPrint);
         std::cout << this->str() << std::endl;
      }

   private:
      static std::mutex _mutexPrint;
   };
}
