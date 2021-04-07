
#pragma once

#include <list>
#include <mutex>
#include <thread>
#include <algorithm>


namespace event {

enum class MBEventCallType : uint8_t {
    SYNC_CALL = 0x00,
    ASYNC_CALL = 0x01,
};

template<class EventData, class JobQueue>
class MBEventGenerator {
public:
    class MBEvent {
    public:
        class MBEventConsumer {
        public:
            virtual void processEvent(const MBEvent&) = 0;
        };

        MBEvent(const EventData& data)
            : mData(data)
        {
        }

        static MBEvent* createEvent(const EventData& data)
        {
            return new MBEvent(data);
        }

        static void sendEvent(const EventData& data)
        {
            MBEvent* temp = createEvent(data);
            temp->send();
        }

        static void addListener(MBEventConsumer& observer)
        {
            std::lock_guard <std::mutex> lock(mObserverLock);
            auto it = std::find(mObservers.begin(), mObservers.end(), &observer);
            if (it == mObservers.end()) {
                mObservers.push_back(&observer);
            }
        }

        static void removeListener(MBEventConsumer& observer)
        {
            std::lock_guard <std::mutex> lock(mObserverLock);
            auto it = std::find(mObservers.begin(), mObservers.end(), &observer);
            if (it != mObservers.end()) {
                mObservers.erase(it);
            }
        }

        void send(MBEventCallType type = MBEventCallType::ASYNC_CALL)
        {
            auto action = [this] {
                std::unique_lock <std::mutex> lock(mObserverLock);
                for (auto observer : mObservers) {
                    observer->processEvent(*this);
                }
                delete this;
            };
            if (MBEventCallType::ASYNC_CALL == type) {
                JobQueue::addJob(std::move(action));
            } else {
                action();
            }
        }

        const EventData& data() const
        {
            return mData;
        }

    private:
        const EventData mData;
        static std::list<MBEventConsumer*> mObservers;
        static std::mutex mObserverLock;
    };
};

template<class EventData, class JobQueue>
std::list<typename MBEventGenerator<EventData, JobQueue>::MBEvent::MBEventConsumer*>
    MBEventGenerator<EventData, JobQueue>::MBEvent::mObservers;

template<class EventData, class JobQueue>
std::mutex MBEventGenerator<EventData, JobQueue>::MBEvent::mObserverLock;

} // namespace event

/**
 * Declares Event
 * @param EventData Name of the class which holds event data
 * @param Event Declares Event class which facilitates adding/removing of listeners, and
 * sending events to them using apis like  send and sendEvent
 * @param EventConsumer Declares the event consumer interface for receiving callbacks
 * @param JobQueue Name of the job queue to which events has to be pushed
 */

#define DECLARE_MB_EVENT(EventData, Event, EventConsumer, JobQueue)                       \
    typedef ::event::MBEventGenerator<EventData, JobQueue>::MBEvent Event;                \
    typedef Event::MBEventConsumer EventConsumer;
