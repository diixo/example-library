

//------------------------------------------------------
// Includes
//------------------------------------------------------
#include <algorithm>

#include "CommandBase.hpp"
//#include "logger/TraceManager.hpp"
#include "../itc.hpp"
#include "../LogInfo.hpp"

//------------------------------------------------------
// Class Definition
//------------------------------------------------------

namespace itc {
uint32_t CommandBase::msNextCmdUID = 1;
uint32_t CommandBase::msLambdaUID = 1;

//------------------------------------------------------
CommandBase::CommandBase()
    : mFSM(this)
    , mCommandListeners()
    , mLambdaCommandListeners()
    , mResult(bt::eCommandResult::RESULT_ERROR)
    //, mWatchdogTimer()
    , mCmdUID(msNextCmdUID++)
//------------------------------------------------------
{
    mFSM.setIsAborted(false);
    mFSM.init();
}

//------------------------------------------------------
CommandBase::~CommandBase()
//------------------------------------------------------
{
}

//------------------------------------------------------
void CommandBase::run()
//------------------------------------------------------
{
    logMethod("CommandBase::run");

    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_RUN); }, shared_from_this()));
}

uint32_t CommandBase::getCmdUID() const
{
    return mCmdUID;
}

//------------------------------------------------------
void CommandBase::cancel()
//------------------------------------------------------
{
    logMethod("CommandBase::cancel");

    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_CANCEL); }, shared_from_this()));
}

//------------------------------------------------------
void CommandBase::pause()
//------------------------------------------------------
{
    logMethod("CommandBase::pause");

    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_PAUSE); }, shared_from_this()));
}

//------------------------------------------------------
void CommandBase::resume()
//------------------------------------------------------
{
    logMethod("CommandBase::resume");

    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_RESUME); }, shared_from_this()));
}

//------------------------------------------------------
void CommandBase::finish(bt::eCommandResult result)
//------------------------------------------------------
{
    logMethod("CommandBase::finish", result);

    mResult = result;
    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_FINISH); }, shared_from_this()));
}

//------------------------------------------------------
void CommandBase::subscribe(std::shared_ptr<ICommandListener> consumer, bt::eCommandNotificationType notificationType)
//------------------------------------------------------
{
    logMethod("CommandBase::subsribe", consumer.get(), notificationType);

    auto findResult = std::find_if(mCommandListeners[notificationType].begin(),
                                   mCommandListeners[notificationType].end(),
                                   [&consumer](std::shared_ptr<ICommandListener> listener) { return listener == consumer; });

    if (findResult == mCommandListeners[notificationType].end())
    {
        mCommandListeners[notificationType].push_back(consumer);
    }
    else
    {
        //logError() << "consumer already subscribed";
    }
}

//------------------------------------------------------
uint32_t CommandBase::subscribe(LambdaCommandListener consumer, bt::eCommandNotificationType notificationType)
//------------------------------------------------------
{
    logMethod("CommandBase::subscribe", consumer, notificationType);

    mLambdaCommandListeners[notificationType].push_back({msLambdaUID, consumer});
    return msLambdaUID++;
}

//------------------------------------------------------
void CommandBase::unsubscribe(std::shared_ptr<ICommandListener> consumer)
//------------------------------------------------------
{
    logMethod("CommandBase::unsubscribe", consumer.get());

    /*
     * Delete consumer from vector
     */
    std::for_each(mCommandListeners.begin(),
                  mCommandListeners.end(),
                  [&consumer](std::pair<const bt::eCommandNotificationType, std::vector<std::shared_ptr<ICommandListener>>>& listeners) {
                      listeners.second.erase(
                          std::remove_if(listeners.second.begin(),
                                         listeners.second.end(),
                                         [&consumer](std::shared_ptr<ICommandListener>& listener) { return listener == consumer; }),
                          listeners.second.end());
                      return listeners.second.empty();
                  });
}

//------------------------------------------------------
void CommandBase::unsubscribe(uint32_t lambdaId)
//------------------------------------------------------
{
    logMethod("CommandBase::unsubscribe", lambdaId);

    if (0u != lambdaId)
    {
        /*
         * Delete consumer from mLambdaCommandListeners
         */
        std::for_each(
            mLambdaCommandListeners.begin(),
            mLambdaCommandListeners.end(),
            [lambdaId](std::pair<const bt::eCommandNotificationType, std::vector<std::pair<uint32_t, LambdaCommandListener>>>& listeners) {
                listeners.second.erase(std::remove_if(listeners.second.begin(),
                                                      listeners.second.end(),
                                                      [lambdaId](const std::pair<uint32_t, LambdaCommandListener>& listener) {
                                                          return listener.first == lambdaId;
                                                      }),
                                       listeners.second.end());
                return listeners.second.empty();
            });
    }
    else
    {
        //logWarn() << "Try to delete a null lambdaId";
    }
}

//------------------------------------------------------
bt::eCommandResult CommandBase::getCommandResult() const
//------------------------------------------------------
{
    return mResult;
}

////------------------------------------------------------
//void CommandBase::wdtEnable(std::chrono::milliseconds timeout)
////------------------------------------------------------
//{
//    logMethod("CommandBase::wdtEnable", timeout);
//
//    mWatchdogTimer = ::itc::timer(EVENT_Watchdog_onTimer::Event(shared_from_this()), timeout, false);
//    if (auto st = mWatchdogTimer.lock())
//    {
//        st->start();
//    }
//    else
//    {
//        logError() << "Could not start watchdog";
//    }
//}

////------------------------------------------------------
//void CommandBase::wdtReset()
////------------------------------------------------------
//{
//    logMethod("CommandBase::wdtReset");
//
//    if (auto st = mWatchdogTimer.lock())
//    {
//        st->stop();
//    }
//    else
//    {
//        logError() << "Could not reset watchdog";
//    }
//}

////------------------------------------------------------
//void CommandBase::wdtDisable()
////------------------------------------------------------
//{
//    logMethod("CommandBase::wdtDisable");
//
//    if (auto st = mWatchdogTimer.lock())
//    {
//        st->stop();
//    }
//    else
//    {
//        logError() << "Could not stop watchdog";
//    }
//}

//------------------------------------------------------
void CommandBase::stateRunningEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    (void)m;
    doNotifyListeners(bt::eCommandNotificationType::CMD_STARTED);
    processCommand();
}

//------------------------------------------------------
void CommandBase::stateFinishedEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    processFinish();
    doNotifyListeners(bt::eCommandNotificationType::CMD_FINISHED);
}

//------------------------------------------------------
void CommandBase::stateAbortingEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    doNotifyListeners(bt::eCommandNotificationType::CMD_ABORT_STARTED);
    processAbort();
}

//------------------------------------------------------
void CommandBase::stateCancelingEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    doNotifyListeners(bt::eCommandNotificationType::CMD_CANCEL_STARTED);
    processCancel();
}

//------------------------------------------------------
void CommandBase::statePausedEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    doNotifyListeners(bt::eCommandNotificationType::CMD_PAUSED);
    processPause();
}

void CommandBase::statePausedExit(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    doNotifyListeners(bt::eCommandNotificationType::CMD_RESUMED);
}

//------------------------------------------------------
void CommandBase::statePostProcessingEnter(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    postProcessing();
    ::itc::invoke(::itc::InlineEvent<std::shared_ptr<CommandBase>>(
        [](std::shared_ptr<CommandBase> ptr) { ptr->mFSM.dispatch(&CCommandFSM::state::trigger_FINISHED); }, shared_from_this()));
}

//------------------------------------------------------
void CommandBase::onReset(CCommandFSM::data_model& m)
//------------------------------------------------------
{
    processReset();
}

//------------------------------------------------------
void CommandBase::doNotifyListeners(bt::eCommandNotificationType type)
//------------------------------------------------------
{
    logMethod("CommandBase::doNotifyListeners", type);
    
    std::for_each(
        mCommandListeners[type].begin(), mCommandListeners[type].end(), [this, type](std::shared_ptr<ICommandListener>& listener) {
            ::itc::invoke(::itc::InlineEvent<std::shared_ptr<ICommandListener>, std::shared_ptr<CommandBase>, bt::eCommandNotificationType>(
                [](std::shared_ptr<ICommandListener> listener, std::shared_ptr<CommandBase> command, bt::eCommandNotificationType type) {
                    listener->onNotification(command, type);
                },
                listener,
                shared_from_this(),
                type));
        });

    for (auto listener : mLambdaCommandListeners[type])
    {
        ::itc::invoke(
            ::itc::InlineEvent<std::pair<uint32_t, LambdaCommandListener>, std::shared_ptr<CommandBase>, bt::eCommandNotificationType>(
                [](std::pair<uint32_t, LambdaCommandListener> listener,
                   std::shared_ptr<CommandBase> command,
                   bt::eCommandNotificationType type) {
                    listener.second(command, type);
                },
                listener,
                shared_from_this(),
                type));
    }
}

} /* namespace itc */

std::ostream& operator<<(std::ostream& out, const itc::CommandBase& value)
{
    return out << value.getName() << "(" << value.getCmdUID() << ")";
}
