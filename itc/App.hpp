
#pragma once

#include "MessageQueue.hpp"

class App
{
private:

   MessageQueue m_MsgQueue;
   std::mutex m_Lock;
   App() {};

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

private:
   void processQueue();
};

