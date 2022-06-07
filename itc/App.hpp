
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

   static bool invoke(const std::string& threadName, std::shared_ptr<itc::_private::ICallable> call);

   // Create thread and register in Dispatcher, threadName should be UNIQE for all application
   static void createEventLoop(const std::string& threadName);

   // Invoke ICallable object on binded thread.
   // * callBinder - binder ICallable object types to thread should be declared using one of macro:
   // DECLARE_STATIC_CALL, DECLARE_CALL, DECLARE_EVENT, DECLARE_REQUEST
   static bool invoke(const itc::_private::CallBinder& callBinder);

   // run while eventLoop is not empty. Blocked current thread.
   static void waitEventLoop(const std::string& thread_name);

private:
   void processQueue();
};

