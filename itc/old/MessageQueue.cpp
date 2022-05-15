#include "MessageQueue.hpp"

#include <memory>
#include <iostream>


MessageQueue::MessageQueue()
{
}

void MessageQueue::pushFunc(const Function& func)
{
   if (!func)
   {
      return;
   }

   std::lock_guard<std::mutex> scopeMutex(m_QueueMtx);
   m_Queue.push_back(std::move(func));
   m_CV.notify_one();
}

void MessageQueue::pushEvent(std::unique_ptr<SEvent> pEvt)
{
   if (!pEvt)
   {
      return;
   }

   EventPriority priority = pEvt->priority < PRIORITY_SIZE ?
      pEvt->priority : PRIORITY_MIN;

   std::lock_guard<std::mutex> scopeMutex(m_QueueMtx);
   m_EventsQueue[priority].push(std::move(pEvt));
   m_CV.notify_one();
}

MessageQueue::Function MessageQueue::popFunc()
{
   std::lock_guard<std::mutex> scopeMutex(m_QueueMtx);

   if (m_Queue.size() == 0)
   {
      return nullptr;
   }

   Function first = m_Queue.front();
   m_Queue.pop_front();

   return first;
}

std::unique_ptr<SEvent> MessageQueue::popEvent()
{
   std::lock_guard<std::mutex> scopeMutex(m_QueueMtx);

   for (size_t i = 0; i < m_EventsQueue.size(); ++i)
   {
      if (m_EventsQueue[i].size() > 0)
      {
         std::unique_ptr<SEvent> first = std::move(m_EventsQueue[i].front());
         m_EventsQueue[i].pop();
         return first;
      }
   }

   return nullptr;
}

void MessageQueue::waitForEvents() const
{
   std::unique_lock<std::mutex> lock(m_QueueMtx);
   while (getSizePriv() == 0)
   {
      m_CV.wait(lock);
   }
}

size_t MessageQueue::getSizePriv() const
{
   size_t size = 0;
   for (size_t i = 0; i < m_EventsQueue.size(); ++i)
   {
      size += m_EventsQueue[i].size();
   }
   size += m_Queue.size();
   return size;
}

size_t MessageQueue::getSize() const
{
   std::lock_guard<std::mutex> scopeMutex(m_QueueMtx);
   return getSizePriv();
}
