
#include "App.hpp"
#include "DeviceManager.hpp"
#include "itc.hpp"

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
   mDeviceManager = std::make_shared<itc::DeviceManager>();
   
   itc::createEventLoop(itc::IDeviceManager::threadID);

   //itc::invoke(itc::DM_init::Call(mDeviceManager));

   processQueue();
}

//////////////////////////////////////////////////////////////////////////
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
}
*/

void App::trace(const std::string& threadId, itc::eTraceMessageLevel level, const std::string& prefix, const std::string& message)
{
   {
      std::shared_lock<std::shared_mutex> gl(mGlobalLoggingMutex);
      std::ostringstream outstream;
      outstream << /*threadId << "::" << */ prefix << message << std::endl;
      std::cout << outstream.str();
   }
}
