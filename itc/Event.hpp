
#pragma once

#include <memory>

namespace itc {
    namespace _private {

        class ICallable;

        enum EventType {
            SYSTEM,
            CALL
        };

        enum EventPriority {
            HIGHEST,
            HIGH,
            MEDIUM,
            LOW,
            LOWEST
        };

        class Event
        {
        public:
            explicit Event(std::shared_ptr<ICallable> callable);
            Event(EventType type, EventPriority priority, std::shared_ptr<ICallable> callable);
            ~Event() = default;

            inline EventType getType() const { return mType; }
            inline EventPriority getPriority() const { return mPriority; }
            inline std::shared_ptr<ICallable> getCallable() const { return mpCallable; }

        private:
            const EventType mType;
            const EventPriority mPriority;
            std::shared_ptr<ICallable> mpCallable;
        };
    }
}
