
#pragma once

#include <memory>
#include <shared_mutex>
#include "MessageQueue.hpp"
#include "LogInfo.hpp"
#include "Dispatcher.hpp"

namespace itc
{
   class IDeviceManager;

   namespace _private
   {
      class Event;
      class ICallable;
      class CallBinder;
   }
}

class App
{
private:

   MessageQueue m_MsgQueue;
   std::mutex m_Lock;
   std::shared_mutex mGlobalLoggingMutex;
   App() {};

   std::shared_ptr<itc::IDeviceManager> mDeviceManager;

   itc::_private::Dispatcher mDispatcher;

public:

   static App& GetInstance()
   {
      static App instance;
      return instance;
   }

   itc::_private::Dispatcher& Dispatcher()
   {
      return mDispatcher;
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

   bool invoke(const std::string& threadName, std::shared_ptr<itc::_private::ICallable> call);

   bool invoke(const itc::_private::CallBinder& callBinder);

private:
   void processQueue();
};

