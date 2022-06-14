
#pragma once

#include <memory>

#include "ICommand.hpp"
//#include "logger/Debugged.hpp"

namespace itc {

class CommandsContainer
    : public ICommandListener
    //, public Debugged
{
public:
    explicit CommandsContainer() = default;
    virtual ~CommandsContainer() = default;

    virtual void push(std::shared_ptr<ICommand> command) = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual size_t cancel(std::initializer_list<int32_t> types) = 0;
    virtual size_t cancel(std::function<bool(const std::shared_ptr<ICommand>)> functor) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    // virtual void subscribe(std::shared_ptr<ICommandsQueueListener> consumer, eCommandsQueueNotificationType notificationType) = 0;
    // virtual void unsubscribe(std::shared_ptr<ICommandsQueueListener> consumer) = 0;
    virtual uint32_t count(const int32_t commandTypeId) const = 0;
};

} // namespace itc