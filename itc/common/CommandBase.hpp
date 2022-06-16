
#pragma once

//------------------------------------------------------
// Includes
//------------------------------------------------------
#include <map>
#include <memory>
#include <vector>

#include "ICommand.hpp"
#include "../Debugged.hpp"

#include "CommandFSM.hpp"
//#pragma GCC diagnostic pop

//------------------------------------------------------
// Forward declarations
//------------------------------------------------------

//------------------------------------------------------
// Class Declaration
//------------------------------------------------------
namespace itc {
//typedef std::weak_ptr<::itc::ITimer> Watchdog;

class CommandBase
    : public ICommand
    , public CCommandActionHandlerDefault
    , public Debugged
    , public std::enable_shared_from_this<CommandBase>
{
public:
    CommandBase();
    void run() override;
    void cancel() override;
    void pause() override;
    void resume() override;
    void subscribe(std::shared_ptr<ICommandListener> pConsumer, bt::eCommandNotificationType notificationType) override;
    uint32_t subscribe(LambdaCommandListener consumer, bt::eCommandNotificationType notificationType) override;
    void unsubscribe(std::shared_ptr<ICommandListener> pConsumer) override;
    void unsubscribe(uint32_t lambdaId) override;
    bt::eCommandResult getCommandResult() const override;

    uint32_t getCmdUID() const override;

    //Debugged
    std::string buildPrefix() const override
    {
        return std::string("[") + getName() + "_" + std::to_string(mCmdUID) + "]";
        // return std::string("[") + "_" + std::to_string(mCmdUID) + "]";
    }

protected:
    virtual ~CommandBase();
    void finish(bt::eCommandResult result) override;
    void processAbort() override
    {
        //logMethod("CommandBase::processAbort");
        finish(bt::eCommandResult::RESULT_ABORTED);
    }

    void processCancel() override
    {
        //logMethod("CommandBase::processCancel");
        finish(bt::eCommandResult::RESULT_CANCELED);
    }

    void processPause() override {
       //logMethod("CommandBase::processPause");
    }

    void processReset() override {
       //logMethod("CommandBase::processReset");
    }

    void postProcessing() override {
       //logMethod("CommandBase::postProcessing");
    }
    void processFinish() override {
       //logMethod("CommandBase::processFinish");
    }

    void processWatchdog() override
    {
        //logMethod("CommandBase::processWatchdog");
        //logError() << "Fault detection";
        finish(bt::eCommandResult::RESULT_WATCHDOG); // TODO: Maybe need call cancel() for processAbort()
    }

    /**
     * @brief Method for start Watchdog. When timeout, the processWatchdog method will be called.
     *
     * @param timeout[in] Milliseconds timeout
     */
    //void wdtEnable(std::chrono::milliseconds timeout);

    /**
     * @brief Restart watchdog. Need call for restart timeout.
     *
     */
    //void wdtReset();

    /**
     * @brief Method for stop Watchdog
     *
     */
    //void wdtDisable();

    /**
     * @brief Method for check current state is aborting
     *
     */
    inline bool isAborted()
    {
        return mFSM.model.isAborted;
    }

private:
    void stateRunningEnter(CCommandFSM::data_model& m) override;
    void stateFinishedEnter(CCommandFSM::data_model& m) override;
    void stateAbortingEnter(CCommandFSM::data_model& m) override;
    void stateCancelingEnter(CCommandFSM::data_model& m) override;
    void statePausedEnter(CCommandFSM::data_model& m) override;
    void statePausedExit(CCommandFSM::data_model& m) override;
    void statePostProcessingEnter(CCommandFSM::data_model& m) override;
    void onReset(CCommandFSM::data_model& m) override;
    void doNotifyListeners(bt::eCommandNotificationType type);

    CCommandFSM mFSM;
    std::map<bt::eCommandNotificationType, std::vector<std::shared_ptr<ICommandListener>>> mCommandListeners;
    std::map<bt::eCommandNotificationType, std::vector<std::pair<uint32_t, LambdaCommandListener>>> mLambdaCommandListeners;
    bt::eCommandResult mResult;
    //Watchdog mWatchdogTimer;

    static uint32_t msNextCmdUID;
    const uint32_t mCmdUID;
    static uint32_t msLambdaUID;
};

//DECLARE_EVENT(EVENT_Watchdog_onTimer, CommandBase, processWatchdog)

} // namespace itc

std::ostream& operator<<(std::ostream& out, const itc::CommandBase& value);
