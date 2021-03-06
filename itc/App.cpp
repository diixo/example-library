
#include <memory>
#include "App.hpp"
#include "DeviceManager.hpp"
#include "itc.hpp"
#include "Event.hpp"
// ConnectMusicCommand
void App::processQueue()
{
   while (true)
   {
      itc::logInfo() << "App::processQueue >> wait_for...";

      m_MsgQueue.waitForEvents();

      itc::logInfo() << "App::processQueue.wait >> started";

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
      else if ((CMD_CONNECT <= pEvt->evtID) && (pEvt->evtID < CMD_MAX))
      {
         //mConnService->receiveEvent();
      }
      else // NOT SUPPORTED
      {
         //DLT_LOG();
      }
      itc::logInfo() << "App::processQueue <<";
   }
   itc::logInfo() << "App::processQueue.exit <<";
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
   
   App::createEventLoop(itc::IDeviceManager::threadID);

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
   if (level == itc::eTraceMessageLevel::logInfo)
   {
      std::shared_lock<std::shared_mutex> gl(mGlobalLogInfoMutex);
      std::ostringstream outstream;
      outstream << /*threadId << "::" << */ prefix << message << std::endl;
      std::cout << outstream.str();
   }
   if (level == itc::eTraceMessageLevel::logVerbose)
   {
      std::shared_lock<std::shared_mutex> gl(mGlobalLogVerboseMutex);
      std::ostringstream outstream;
      outstream << /*threadId <<*/ "::" << prefix << message << std::endl;
      std::cout << outstream.str();
   }
}

bool App::invoke(const itc::_private::CallBinder& callBinder)
{
   return App::invoke(callBinder.getThreadName(), callBinder.getCallable());
}

bool App::invoke(const std::string& threadName, std::shared_ptr<itc::_private::ICallable> call)
{
   bool result = false;

   std::shared_ptr<itc::_private::EventLoop> thread = App::GetInstance().Dispatcher().getThreadByName(threadName);

   if (thread == nullptr)
   {
      itc::logInfo() << "itc::invoke. unknow threadName: "/* << threadName*/;
   }
   else
   {
      thread->push(call);
      result = true;
   }
   return result;
}

/*
bool App::invoke(std::unique_ptr<itc::_private::Event>&& event)
{
   std::list<std::unique_ptr<itc::_private::Event>> mList;
   mList.push_back(std::move(event));
   return true;
}
*/

void App::createEventLoop(const std::string& threadName)
{
   std::shared_ptr<itc::_private::EventLoop> eventLoop = std::make_shared<itc::_private::EventLoop>(threadName);
   App::GetInstance().Dispatcher().registerEventLoop(eventLoop);
}

void App::waitEventLoop(const std::string& thread_name)
{
   while (!App::GetInstance().Dispatcher().isEmpty())
   {
      auto eventLoop = App::GetInstance().Dispatcher().getThreadByName(thread_name);

      // waiting while all events finish responses by calling from parallel thread.
      if (eventLoop && (eventLoop->size() == 0))
      {
         break;
      }
   }
}

App::~App()
{
   itc::logInfo() << "App::~App";
}
