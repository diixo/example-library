
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

void itc::logMethod(const std::string& methodName)
{
   // TODO: fix it.
   const std::string thread_id = std::to_string(0);// std::this_thread::get_id();   //App::GetInstance().getCurrentThreadName()

   std::unique_ptr<TraceStream> buff(
      new TraceStream(thread_id, "", itc::eTraceMessageLevel::logVerbose));
   *buff.get() << methodName << "()";
}

void itc::logMethod(const LogPrefix& pref, const std::string& methodName)
{
   // TODO: fix it.
   const std::string thread_id = std::to_string(0);// std::this_thread::get_id()._To_text();   //App::GetInstance().getCurrentThreadName()

   std::unique_ptr<TraceStream> buff(
      new TraceStream(thread_id, pref.getPrefix(), itc::eTraceMessageLevel::logVerbose));
   *buff.get() << methodName << "()";
}

template <typename Arg, typename... Args>
void itc::logMethod(const std::string& methodName, Arg&& arg, Args&&... args)
{
   // TODO: fix it.
   const std::string thread_id = std::to_string(0);// std::this_thread::get_id();   //App::GetInstance().getCurrentThreadName()

   std::unique_ptr<TraceStream> ts(
      new TraceStream(thread_id, "", itc::eTraceMessageLevel::logVerbose));
   *ts << methodName << "(" << std::forward<Arg>(arg);
   using expander = int[];
   (void)expander {
      0, (void(*ts << ", " << std::forward<Args>(args)), 0)...
   };
   *ts << ")";
}

template <typename Arg, typename... Args>
void itc::logMethod(const LogPrefix& pref, const std::string& methodName, Arg&& arg, Args&&... args)
{
   // TODO: fix it.
   const std::string thread_id = std::to_string(0);// std::this_thread::get_id();   //App::GetInstance().getCurrentThreadName()

   std::unique_ptr<TraceStream> ts(
      new TraceStream(thread_id, pref.getPrefix(), itc::eTraceMessageLevel::logVerbose));
   *ts << methodName << "(" << std::forward<Arg>(arg);
   using expander = int[];
   (void)expander {
      0, (void(*ts << ", " << std::forward<Args>(args)), 0)...
   };
   *ts << ")";
}
