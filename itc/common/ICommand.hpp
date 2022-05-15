
#pragma once

//------------------------------------------------------
// Includes
//------------------------------------------------------
#include <memory>
#include <stdint.h>

#include "CommandTypes.hpp"

//------------------------------------------------------
// Forward declarations
//------------------------------------------------------
namespace itc {
class ICommand;
}

//------------------------------------------------------
// Class Declaration
//------------------------------------------------------
namespace itc {

class ICommandListener
{
public:
    virtual ~ICommandListener() = default;
    virtual void onNotification(std::shared_ptr<ICommand> pCommand, itc::eCommandNotificationType notificationType) = 0;
};

using LambdaCommandListener = std::function<void(std::shared_ptr<ICommand>, itc::eCommandNotificationType)>;

class ICommand
{
public:
    /**
     * @brief Run command
     *
     */
    virtual void run() = 0;

    /**
     * @brief Cancel command
     *
     */
    virtual void cancel() = 0;

    /**
     * @brief Pause command
     *
     */
    virtual void pause() = 0;

    /**
     * @brief Resume command from pause state
     *
     */
    virtual void resume() = 0;

    /**
     * @brief Finish command
     *
     * @param result[in] Result finish state
     */
    virtual void finish(eCommandResult result) = 0;

    /**
     * @brief Subscribe CommandListener to selected activities.
     *
     * @param pConsumer[in] listener
     * @param notificationType[in] selected activities type
     */
    virtual void subscribe(std::shared_ptr<ICommandListener> pConsumer, itc::eCommandNotificationType notificationType) = 0;

    /**
     * @brief Subscribe CommandListener to selected activities.
     *
     * @param pConsumer[in] listener
     * @param notificationType[in] selected activities type
     * @return lambdaId
     */
    virtual uint32_t subscribe(LambdaCommandListener consumer, eCommandNotificationType notificationType) = 0;

    /**
     * @brief Unsubscribe CommandListener
     *
     * @param pConsumer[in] listener
     */
    virtual void unsubscribe(std::shared_ptr<ICommandListener> pConsumer) = 0;

    /**
     * @brief Unsubscribe LambdaCommandListener
     *
     * @param lambdaId[in] lambdaId
     */
    virtual void unsubscribe(uint32_t lambdaId) = 0;

    /**
     * @brief Main command callback method
     *
     */
    virtual void processCommand() = 0;

    /**
     * @brief Calback will be called when command going to abort state.
     *
     */
    virtual void processAbort() = 0;

    /**
     * @brief Calback will be called when command going to cancel state.
     *
     */
    virtual void processCancel() = 0;

    /**
     * @brief Calback will be called when command going to pause state.
     *
     */
    virtual void processPause() = 0;

    /**
     * @brief Calback will be called when command going to reset state.
     *
     */
    virtual void processReset() = 0;

    /**
     * @brief Synchronous callback will be called before command going to finish.
     *
     */
    virtual void postProcessing() = 0;

    /**
     * @brief Calback will be called when command going to finish state.
     *
     */
    virtual void processFinish() = 0;

    /**
     * @brief Watchdog handler. Will be called if watchdog is happened.
     *
     */
    virtual void processWatchdog() = 0;

    /**
     * @brief Get the command result
     *
     * @return bt::eCommandResult
     */
    virtual itc::eCommandResult getCommandResult() const = 0;

    /**
     * @brief Get the command name
     *
     * @return std::string
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get the command typeId
     *
     * @return int32_t
     */
    virtual int32_t getTypeId() const = 0;

    /**
     * @brief Get the command unique id
     *
     * @return uint32_t
     */
    virtual inline uint32_t getCmdUID() const = 0;

protected:
    virtual ~ICommand() = default;
};

template <typename T>
std::shared_ptr<T> command_cast(std::shared_ptr<ICommand> command)
{
    //  itc::logMethod("command_cast", command);

    if (command)
    {
        return (T::mTypeId == command->getTypeId() ? std::static_pointer_cast<T>(command) : std::shared_ptr<T>(nullptr));
    }
    else
    {
        return std::shared_ptr<T>(nullptr);
    }
}

} /* namespace itc */

//std::ostream& operator<<(std::ostream& out, const itc::ICommand& value);
//std::ostream& operator<<(std::ostream& out, const itc::ICommandListener& value);
