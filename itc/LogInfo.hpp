
#pragma once

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>


class LogPrefix
{
public:
   LogPrefix(const std::string& prefix);

   inline const std::string& getPrefix() const
   {
      return mPrefix;
   }

private:
   std::string mPrefix;
};

inline
LogPrefix::LogPrefix(const std::string& prefix)
   : mPrefix(prefix)
{
}

inline
std::ostream& operator<<(std::ostream& os, LogPrefix prefix)
{
   return os << prefix.getPrefix();
}

namespace itc {

   enum class eTraceMessageLevel : uint8_t
   {
      logKPI,
      logVerbose,
      logDebug,
      logInfo,
      logWarn,
      logError
   };

   constexpr eTraceMessageLevel logInfo()
   {
      return eTraceMessageLevel::logInfo;
   }

} //namespace itc

class TraceStream
{
public:
   TraceStream(const std::string& threadId, const std::string& prefix, const itc::eTraceMessageLevel logLevel);
   TraceStream(TraceStream&& other);
   TraceStream(const TraceStream&) = delete;
   ~TraceStream();

   TraceStream& operator=(const TraceStream&) = delete;
   TraceStream& operator=(TraceStream&& other);

   template <typename ValueT>
   std::ostream& operator<<(ValueT const& val)
   {
      return mBaseStream << std::boolalpha << val;
   }

protected:
   std::ostringstream mBaseStream;
   itc::eTraceMessageLevel mLevel;
   std::string mThreadId;
   std::string mPrefix;
};

template <typename ValueT> inline
std::unique_ptr<TraceStream> operator<<(std::unique_ptr<TraceStream> ts, const ValueT& val)
{
   *ts << val;
   return std::move(ts);
}

/////////////////////////////// TraceManager -->>

template <typename ValueT> inline
std::unique_ptr<TraceStream> operator<<(itc::eTraceMessageLevel level, const ValueT& val)
{
   std::unique_ptr<TraceStream> ts(new TraceStream(std::string("")/*TraceManager::getInstance().getCurrentThreadName()*/, "", level));
   *ts << val;
   return std::move(ts);
}

//template <>
//std::unique_ptr<TraceStream> operator<<(itc::eTraceMessageLevel level, const LogPrefix& pref);

template <> inline
std::unique_ptr<TraceStream> operator<<(itc::eTraceMessageLevel level, const LogPrefix& pref)
{
   std::unique_ptr<TraceStream> b(new TraceStream(std::string("")/*TraceManager::getInstance().getCurrentThreadName()*/, pref.getPrefix(), level));
   return std::move(b);
}

