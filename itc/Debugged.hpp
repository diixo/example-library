
#pragma once

#include <list>
#include <memory>
#include <string>

#include "LogInfo.hpp"
//#include "TraceManager.hpp"

class Debugged //: public std::enable_shared_from_this<Debugged>
{
public:
    // Declaration of <<struct itc>> forbids Debugged inheritors use namespace itc
    struct itc
    {
    };

    Debugged() = default;
    virtual ~Debugged() = default;

    virtual std::string buildPrefix() const = 0;

    inline void link(std::weak_ptr<Debugged> parent)
    {
        mParent = parent;

        // if (auto shared = parent.lock()) {
        //     shared->mChilds.push_back(shared_from_this());
        // }
    }

    inline std::unique_ptr<TraceStream> logVerbose() const
    {
        return ::itc::eTraceMessageLevel::logVerbose << LogPrefix(getPrefix());
    }

    inline std::unique_ptr<TraceStream> logDebug() const
    {
        return ::itc::eTraceMessageLevel::logDebug << LogPrefix(getPrefix());
    }

    inline std::unique_ptr<TraceStream> logInfo() const
    {
        return ::itc::eTraceMessageLevel::logInfo << LogPrefix(getPrefix());
    }

    inline std::unique_ptr<TraceStream> logWarn() const
    {
        return ::itc::eTraceMessageLevel::logWarn << LogPrefix(getPrefix());
    }

    inline std::unique_ptr<TraceStream> logError() const
    {
        return ::itc::eTraceMessageLevel::logError << LogPrefix(getPrefix());
    }

    inline std::unique_ptr<TraceStream> logKPI() const
    {
        return ::itc::eTraceMessageLevel::logKPI << LogPrefix(getPrefix());
    }

    template <typename... Args>
    void logMethod(const std::string& methodName, Args&&... args) const
    {
        return ::itc::logMethod(LogPrefix(getPrefix()), methodName, std::forward<Args&&>(args)...);
    }

private:
    inline std::string getPrefix() const
    {
        if (auto shared = mParent.lock())
        {
            return shared->getPrefix() + buildPrefix() + ":";
        }
        else
        {
            return buildPrefix() + ":";
        }
    }

    std::weak_ptr<Debugged> mParent;
    std::list<std::weak_ptr<Debugged>> mChilds;
};