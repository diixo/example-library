
#pragma once

//------------------------------------------------------
// Includes
//------------------------------------------------------
#include <initializer_list>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

#include "CommandsContainer.hpp"

//------------------------------------------------------
// Forward declarations
//------------------------------------------------------

//------------------------------------------------------
// Class Declaration
//------------------------------------------------------
namespace itc {
class CommandsQueue;
class ICommandsQueueListener
{
public:
    virtual ~ICommandsQueueListener() = default;
    virtual void onNotification(std::shared_ptr<CommandsQueue> pCommandsQueue, bt::eCommandsQueueNotificationType notificationType) = 0;
};

class CommandsQueue
    : public CommandsContainer
    , public std::enable_shared_from_this<CommandsQueue>
{
public:
    explicit CommandsQueue();
    virtual ~CommandsQueue();

    void push(std::shared_ptr<ICommand> command) override;
    bool empty() const override;
    void clear() override;
    size_t size() const { return mQueue.size(); }
    std::shared_ptr<ICommand> front() const { return mQueue.front(); }
    std::shared_ptr<ICommand> back() const { return mQueue.back(); }

    size_t cancel(std::initializer_list<int32_t> types) override;
    size_t cancel(std::function<bool(const std::shared_ptr<ICommand>)> functor) override;
    void pause() override;
    void resume() override;
    uint32_t count(const int32_t commandTypeId) const override;
    void subscribe(std::shared_ptr<ICommandsQueueListener> consumer, bt::eCommandsQueueNotificationType notificationType);
    void unsubscribe(std::shared_ptr<ICommandsQueueListener> consumer);

    // ICommandListener
    void onNotification(std::shared_ptr<ICommand> pCommand, bt::eCommandNotificationType notificationType) override;

    // Debugged
    std::string buildPrefix() const override
    {
        std::stringstream stream;
        stream << "[CQ_" << this << "]";
        return stream.str();
    }

    void block();
    void unblock();

private:
    void runNextCommand();
    void cancelSelectedType(int32_t type);
    inline void doNotifyListeners(bt::eCommandsQueueNotificationType type)
    {
        //logMethod("CommandsQueue::doNotifyListeners", type);

        // Remove unused ptr
        std::for_each(mCommandsQueueListeners.begin(),
                      mCommandsQueueListeners.end(),
                      [](std::pair<const bt::eCommandsQueueNotificationType, std::vector<std::weak_ptr<ICommandsQueueListener>>>& listeners) {
                          listeners.second.erase(
                              std::remove_if(listeners.second.begin(),
                                             listeners.second.end(),
                                             [](std::weak_ptr<ICommandsQueueListener>& listener) { return listener.use_count() == 0; }),
                              listeners.second.end());
                          return listeners.second.empty();
                      });

        std::for_each(mCommandsQueueListeners[type].begin(),
                      mCommandsQueueListeners[type].end(),
                      [this, type](std::weak_ptr<ICommandsQueueListener>& listener) {
                          if (auto st = listener.lock())
                          {
                              st->onNotification(shared_from_this(), type);
                          }
                          else
                          {
                              //logError() << "listener is broken";
                          }
                      });
    }

    std::list<std::shared_ptr<ICommand>> mQueue;
    std::map<bt::eCommandsQueueNotificationType, std::vector<std::weak_ptr<ICommandsQueueListener>>> mCommandsQueueListeners;
    bool mBlocked;
};

} // namespace itc

std::ostream& operator<<(std::ostream& out, const ::itc::ICommandsQueueListener& value);
std::ostream& operator<<(std::ostream& out, const ::itc::CommandsQueue& value);
