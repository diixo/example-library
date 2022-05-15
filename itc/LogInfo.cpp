
#include "LogInfo.hpp"

namespace itc
{
   std::mutex logInfo::_mutexPrint{};

   std::mutex logMethod::_mutexPrint{};
}
