
#include "Event.hpp"
#include "ICallable.hpp"

namespace itc {
namespace _private {

Event::Event(std::shared_ptr<ICallable> callable)
   : mType(_private::EventType::CALL)
   , mPriority(EventPriority::MEDIUM)
   , mpCallable(callable)
{}

Event::Event(EventType type, EventPriority priority, std::shared_ptr<ICallable> callable)
   : mType(type)
   , mPriority(priority)
   , mpCallable(callable)
{}
}
}
