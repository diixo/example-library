
#include "App.hpp"

void App::processQueue()
{
   while (true)
   {
      m_MsgQueue.waitForEvents();

      std::lock_guard<std::mutex> lock(m_Lock);

      MessageQueue::Function func = m_MsgQueue.popFunc();

      if (func != nullptr)
      {
         func();
         continue;
      }

      std::unique_ptr<SEvent> pEvt = m_MsgQueue.popEvent();

      if (pEvt == nullptr)
      {
         continue;
      }

      if (CMD_APP_SHUTDOWN == pEvt->evtID)
      {
         // quit main loop
         break;
      }
      // A2DPstart
      else if ((CMD_CONNECT <= pEvt->evtID) && (pEvt->evtID < CMD_MAX))
      {
         //mConnService->receiveEvent();
      }
      else // NOT SUPPORTED
      {
         //DLT_LOG();
      }
   }
}

void App::PushEvent(int eventID, EventPriority priority, std::vector<uint8_t> buffer)
{
   std::unique_ptr<SEvent> pEvt = std::unique_ptr<SEvent>(new SEvent());
   pEvt->evtID = eventID;
   pEvt->payload = std::move(buffer);
   pEvt->priority = priority;

   App::GetInstance().m_MsgQueue.pushEvent(std::move(pEvt));
}

void App::PushFunc(const std::function<void()>& functor)
{
   App::GetInstance().m_MsgQueue.pushFunc(functor);
}

void App::start()
{
   processQueue();
}
