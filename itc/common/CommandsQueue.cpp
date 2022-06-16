

//------------------------------------------------------
// Includes
//------------------------------------------------------
//#include <STDDumpers.hpp>
#include <algorithm>
#include <initializer_list>

#include "CommandsQueue.hpp"
#include "ICommand.hpp"
#include "../itc.hpp"
#include "../LogInfo.hpp"

//------------------------------------------------------
// Class Definition
//------------------------------------------------------

namespace itc {

//------------------------------------------------------
CommandsQueue::CommandsQueue()
    : mQueue()
    , mCommandsQueueListeners()
    , mBlocked(false)
//------------------------------------------------------
{
    ::itc::logMethod("CommandsQueue::CommandsQueue");
}

//------------------------------------------------------
CommandsQueue::~CommandsQueue()
//------------------------------------------------------
{
    ::itc::logMethod("CommandsQueue::~CommandsQueue");
}

//------------------------------------------------------
void CommandsQueue::push(std::shared_ptr<ICommand> command)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::push", command);

    command->subscribe(shared_from_this(), bt::eCommandNotificationType::CMD_FINISHED);
    mQueue.push_back(command);

    // Notify listeners about CommandsQueue is changed
    doNotifyListeners(bt::eCommandsQueueNotificationType::CQ_CHANGED);

    if (mBlocked)
    {
        //logError() << "CommandsQueue is blocked";
    }
    else if (!mBlocked && 1u == mQueue.size()) // If added command is first
    {
        ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandsQueue>>(
           [](std::shared_ptr<CommandsQueue> ptr){ ptr->runNextCommand(); },
           shared_from_this())
        );
    }
}

//------------------------------------------------------
bool CommandsQueue::empty() const
//------------------------------------------------------
{
    //logMethod("CommandsQueue::empty");

    return mQueue.empty();
}

//------------------------------------------------------
void CommandsQueue::clear()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::clear");

    std::for_each(mQueue.begin(), mQueue.end(), [](std::shared_ptr<ICommand>& command) {
        ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->cancel(); }, command));
    });

    // Notify listeners about CommandsQueue is finish
    doNotifyListeners(bt::eCommandsQueueNotificationType::CQ_CLEARED);
}

//------------------------------------------------------
size_t CommandsQueue::cancel(std::initializer_list<int32_t> types)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::cancel", log_size_t(types.size()));

    size_t count = 0;
    for (auto& type : types)
    {
        for (auto& command : mQueue)
        {
            if (type == command->getTypeId())
            {
                ++count;
                ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->cancel(); }, command));
            }
        }
    }

    return count;
}

//------------------------------------------------------
size_t CommandsQueue::cancel(std::function<bool(const std::shared_ptr<ICommand>)> functor)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::cancel", functor);

    size_t count = 0;
    if (functor != nullptr)
    {
        for (auto& command : mQueue)
        {
            if (functor(command))
            {
                ++count;
                ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->cancel(); }, command));
            }
        }
    }

    return count;
}

//------------------------------------------------------
void CommandsQueue::pause()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::pause");

    std::for_each(mQueue.begin(), mQueue.end(), [](std::shared_ptr<ICommand>& command) {
        ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->pause(); }, command));
    });
}

//------------------------------------------------------
void CommandsQueue::resume()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::resume");

    std::for_each(mQueue.begin(), mQueue.end(), [](std::shared_ptr<ICommand>& command) {
        ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->resume(); }, command));
    });
}

//------------------------------------------------------
uint32_t CommandsQueue::count(const int32_t commandTypeId) const
//------------------------------------------------------
{
    //logMethod("CommandsQueue::count", "commandTypeId:", commandTypeId);
    uint32_t counter = 0;
    for (const auto& command : mQueue)
    {
        if (commandTypeId == command->getTypeId())
        {
            ++counter;
        }
    }
    //logVerbose() << "commandTypeId " << commandTypeId << " was found " << counter << " times at the CommandsQueue";
    return counter;
}

//------------------------------------------------------
void CommandsQueue::subscribe(std::shared_ptr<ICommandsQueueListener> consumer, bt::eCommandsQueueNotificationType notificationType)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::subsribe", consumer.get(), notificationType);

    auto findResult = std::find_if(mCommandsQueueListeners[notificationType].begin(),
                                   mCommandsQueueListeners[notificationType].end(),
                                   [&consumer](std::weak_ptr<ICommandsQueueListener> listener) { return listener.lock() == consumer; });

    if (findResult == mCommandsQueueListeners[notificationType].end())
    {
        mCommandsQueueListeners[notificationType].push_back(consumer);
    }
    else
    {
        //logError() << "consumer already subscribed";
    }
}

//------------------------------------------------------
void CommandsQueue::unsubscribe(std::shared_ptr<ICommandsQueueListener> consumer)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::unsubsribe", consumer.get());

    /*
     * Delete consumer from vector
     */
    std::for_each(
        mCommandsQueueListeners.begin(),
        mCommandsQueueListeners.end(),
        [consumer](std::pair<const bt::eCommandsQueueNotificationType, std::vector<std::weak_ptr<ICommandsQueueListener>>>& listeners) {
            listeners.second.erase(
                std::remove_if(listeners.second.begin(),
                               listeners.second.end(),
                               [consumer](std::weak_ptr<ICommandsQueueListener>& listener) { return listener.lock() == consumer; }),
                listeners.second.end());
            return listeners.second.empty();
        });
}

//------------------------------------------------------
void CommandsQueue::onNotification(std::shared_ptr<ICommand> command, bt::eCommandNotificationType notificationType)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::onNotification", command, notificationType);

    if (bt::eCommandNotificationType::CMD_FINISHED == notificationType)
    {
        if (0u < mQueue.size())
        {
            if (mQueue.front() != command)
            {
                //logDebug() << "Finished command is not active";
                mQueue.erase(std::remove(mQueue.begin(), mQueue.end(), command), mQueue.end()); // remove command if exist in the queue
            }
            else
            {
                mQueue.pop_front();
                if (0u < mQueue.size())
                {
                    //logDebug() << "mQueue size:" << mQueue.size();
                    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandsQueue>>(
                        [](std::shared_ptr<CommandsQueue> ptr) { ptr->runNextCommand(); }, shared_from_this()));
                }
                else
                {
                    doNotifyListeners(bt::eCommandsQueueNotificationType::CQ_FINISHED);
                }
            }
        }
        else
        {
            //logError() << "mQueue is empty";
        }
    }
}

//------------------------------------------------------
void CommandsQueue::block()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::block");
    mBlocked = true;
}

//------------------------------------------------------
void CommandsQueue::unblock()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::unblock");
    mBlocked = false;
    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandsQueue>>([](std::shared_ptr<CommandsQueue> ptr) { ptr->runNextCommand(); },
                                                                     shared_from_this()));
}

//------------------------------------------------------
void CommandsQueue::runNextCommand()
//------------------------------------------------------
{
    //logMethod("CommandsQueue::runNextCommand");

    if (mBlocked)
    {
        //logWarn() << "CommandsQueue is blocked";
    }
    else if (0u < mQueue.size())
    {
        mQueue.front()->run();
    }
    else
    {
        //logWarn() << "Queue is empty";
    }
}

//------------------------------------------------------
void CommandsQueue::cancelSelectedType(int32_t type)
//------------------------------------------------------
{
    //logMethod("CommandsQueue::cancelSelectedType", type);

    std::for_each(mQueue.begin(), mQueue.end(), [type](std::shared_ptr<ICommand>& command) {
        if (type == command->getTypeId())
        {
            ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommand>>([](std::shared_ptr<ICommand> cmd) { cmd->cancel(); }, command));
        }
    });
}

} // namespace bt

std::ostream& operator<<(std::ostream& out, const ::itc::ICommandsQueueListener& value)
{
    return out << "ICQListener";
}

std::ostream& operator<<(std::ostream& out, const ::itc::CommandsQueue& value)
{
   return out << value.buildPrefix();
}
