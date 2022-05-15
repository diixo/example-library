
#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <mutex>
#include <queue>
#include <memory>
#include <array>
#include <condition_variable>
#include <functional>
#include <list>

enum EventPriority
{
   PRIORITY_MAX = 0,
   PRIORITY_HIGH,
   PRIORITY_DEFAULT,
   PRIORITY_LOW,
   PRIORITY_MIN,
   PRIORITY_SIZE
};

struct BTEvent
{
   // to be used in case of object pool
   bool bProcessed;
   int evtID;
   EventPriority priority;
   int order;

   // payload to have only trivial destructor
   std::vector<uint8_t> payload;
};

class MessageQueue
{
public:
   typedef std::function<void()> Function;

private:
   //friend class App;

   size_t getSizePriv() const;

   using EventQueue = std::queue<std::unique_ptr<BTEvent>>;

   mutable std::mutex m_QueueMtx;
   mutable std::condition_variable m_CV;
   std::array<EventQueue, PRIORITY_SIZE> m_EventsQueue;
   std::list<Function> m_Queue;

public:
   MessageQueue();

   void pushEvent(std::unique_ptr<BTEvent> pEvt);
   void pushFunc(const Function& func);

   Function popFunc();
   std::unique_ptr<BTEvent> popEvent();

   void waitForEvents() const;
   size_t getSize() const;
};

#endif // MESSAGEQUEUE_H
