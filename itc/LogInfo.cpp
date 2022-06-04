
#include "LogInfo.hpp"

/*
TraceManager& TraceManager::getInstance()
{
   static TraceManager sTraceManager;
   return sTraceManager;
}

bool TraceManager::trace(const std::string& threadId, itc::eTraceMessageLevel level, const std::string& prefix, const std::string& message)
{
   {
      boost::shared_lock<boost::shared_mutex> gl(mClientsMutex);
      for (auto& strategy : mClients[threadId])
      {
         strategy->trace(level, prefix, message);
      }
   }

   {
      boost::shared_lock<boost::shared_mutex> gl(mGlobalStrategiesMutex);
      for (auto& strategy : mGlobalStrategies)
      {
         strategy->trace(level, prefix, message);
      }
   }
   return true;
}
*/

TraceStream::TraceStream(const std::string& threadId, const std::string& prefix, const itc::eTraceMessageLevel logLevel)
   : mBaseStream()
   , mLevel(logLevel)
   , mThreadId(threadId)
   , mPrefix(prefix)
{
}

TraceStream::TraceStream(TraceStream&& other)
{
   mBaseStream.str(other.mBaseStream.str());
   std::swap(mThreadId, other.mThreadId);
   std::swap(mLevel, other.mLevel);
   std::swap(mPrefix, other.mPrefix);
}

TraceStream::~TraceStream()
{
   //if (itc::eTraceMessageLevel::logKPI == mLevel)
   //{
   //   TraceManager::getInstance().logKPI(mThreadId, mLevel, mBaseStream.str());
   //}
   //else
   //{ // TODO: call from TraceManager
   //
   //   TraceManager::getInstance().trace(mThreadId, mLevel, mPrefix, mBaseStream.str());
   //}
   std::cout << /*mThreadId << "::" <<*/ mPrefix << mBaseStream.str() << std::endl;
}

TraceStream& TraceStream::operator=(TraceStream&& other)
{
   mBaseStream.str(other.mBaseStream.str());
   std::swap(mThreadId, other.mThreadId);
   std::swap(mLevel, other.mLevel);
   std::swap(mPrefix, other.mPrefix);
   return *this;
}

