
#include "itc.hpp"
#include "App.hpp"


namespace itc
{

const std::string UNKNOWN_THREAD_NAME = "_UNKNOWN_";

void stopEventLoop(const std::string& threadName)
{
    //itc::logInfo("itc::stopEventLoop", threadName);
    auto eventLoop = App::GetInstance().Dispatcher().getThreadByName(threadName);
    if (eventLoop == nullptr)
    {
        itc::logInfo() << "itc::stopEventLoop. unknow threadName: " << threadName;
    }
    else
    {
        eventLoop->stop();
        App::GetInstance().Dispatcher().unregisterEventLoop(threadName);
    }
}

const std::string& currentThreadName()
{
    std::shared_ptr<_private::EventLoop> eventLoop = App::GetInstance().Dispatcher().getThreadById(std::this_thread::get_id());
    return eventLoop ? eventLoop->getThreadName() : UNKNOWN_THREAD_NAME;
}

}
