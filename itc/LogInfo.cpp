
#include "LogInfo.hpp"
#include "App.hpp"

#define LOG_ENABLE 1

TraceStream::TraceStream(const std::string& threadId, const std::string& prefix, const itc::eTraceMessageLevel logLevel)
   : mBaseStream()
   , mLevel(logLevel)
   , mThreadId(threadId)
   , mPrefix(prefix)
{
}

TraceStream::TraceStream(TraceStream&& other)
{
#if LOG_ENABLE
   mBaseStream.str(other.mBaseStream.str());
   std::swap(mThreadId, other.mThreadId);
   std::swap(mLevel, other.mLevel);
   std::swap(mPrefix, other.mPrefix);
#endif
}

TraceStream::~TraceStream()
{
#if LOG_ENABLE
   if (itc::eTraceMessageLevel::logKPI == mLevel)
   {
      //TraceManager::getInstance().logKPI(mThreadId, mLevel, mBaseStream.str());
   }
   else
   {
      //TraceManager::getInstance().trace(mThreadId, mLevel, mPrefix, mBaseStream.str());
      App::GetInstance().trace(mThreadId, mLevel, mPrefix, mBaseStream.str());
   }
#endif
}

TraceStream& TraceStream::operator=(TraceStream&& other)
{
#if LOG_ENABLE
   mBaseStream.str(other.mBaseStream.str());
   std::swap(mThreadId, other.mThreadId);
   std::swap(mLevel, other.mLevel);
   std::swap(mPrefix, other.mPrefix);
#endif
   return *this;
}

