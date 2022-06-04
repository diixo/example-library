
#pragma once

#include <memory>
#include <shared_mutex>
#include "MessageQueue.hpp"
#include "LogInfo.hpp"

namespace itc
{
   class IDeviceManager;
}

class App
{
private:

   MessageQueue m_MsgQueue;
   std::mutex m_Lock;
   std::shared_mutex mGlobalLoggingMutex;
   App() {};

   std::shared_ptr<itc::IDeviceManager> mDeviceManager;

public:
   static App& GetInstance()
   {
      static App instance;
      return instance;
   }

   enum EEvents
   {
      CMD_CONNECT,
      CMD_DISCONNECT,
      CMD_MAX,
      CMD_APP_SHUTDOWN
   };

   void start();

   static void PushEvent(int eventID, EventPriority priority, std::vector<uint8_t> buffer);
   static void PushFunc(const std::function<void()>& functor);

   void trace(const std::string& threadId, itc::eTraceMessageLevel level, const std::string& prefix, const std::string& message);

private:
   void processQueue();
};

