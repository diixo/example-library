
#pragma once

#include <iostream>

#include "ICallable.hpp"
#include "CallBinder.hpp"

#include "tuple_utils.hpp"
#include <condition_variable>


namespace ipc {

template <typename... Args>
class CallStatic : public _private::ICallable
{
public:
    CallStatic(const std::function<void(Args...)>& func, std::tuple<Args...> params = std::make_tuple<>())
        : mFunc(func)
    // , mParams(params)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }

    void call() const override { tuple_utils::apply(mFunc, mParams); }

private:
    const std::function<void(Args...)> mFunc;
    std::tuple<Args...> mParams;
};

template <typename Ret, typename... Args>
class SyncCallStatic : public _private::ICallable
{
public:
    SyncCallStatic(const std::function<void(Args...)>& func, std::tuple<Args...> params = std::make_tuple<>())
        : mFunc(func)
        // , mParams(params)
        , mResult()
        , mMutex()
        , mCV()
        , mIsNotified(false)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }

    void call() const override
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mResult = tuple_utils::apply(mFunc, mParams);
        mIsNotified = true;
        mCV.notify_one();
    }

    Ret& getResult()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCV.wait(lock, [this]() { return mIsNotified; });
        return mResult;
    }

private:
    const std::function<void(Args...)> mFunc;
    std::tuple<Args...> mParams;
    mutable Ret mResult;
    mutable std::mutex mMutex;
    mutable std::condition_variable mCV;
    mutable bool mIsNotified;
};

const std::string& currentThreadName(); // To resolve name currentThreadName on first phase of ADL
template <typename Context, typename... Args>
class Call : public _private::ICallable
{
public:
    Call(std::shared_ptr<Context> context, std::function<void(Context*, Args...)> func, std::tuple<Args...> params = std::make_tuple<>())
        : mContext(context)
        , mFunc(func)
    // , mParams(params)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }

    void call() const override
    {
//        itc::logDebug() << "???"
//                        << " -R-> " << currentThreadName() << ": " << typeid(mContext).name() << "::" << typeid(mFunc).name() << mParams;
        tuple_utils::apply(mContext, mFunc, mParams);
    }

private:
    std::shared_ptr<Context> mContext;
    std::function<void(Context*, Args...)> mFunc;
    std::tuple<Args...> mParams;
};

template <typename... Args>
class InlineCall : public ipc::_private::CallBinder
{
public:
    InlineCall<Args...>(const std::string& threadId, const std::function<void(Args...)>& func, Args... args)
        : CallBinder(threadId, std::make_shared<ipc::CallStatic<Args...>>(func, std::make_tuple(args...)))
    {

    }
};

bool invoke(const ipc::_private::CallBinder& callBinder);
template <typename ContextRequest, typename ContextResponse, typename Ret, typename... Args>
class Request : public _private::ICallable
{
public:
    Request(std::shared_ptr<ContextRequest> contextRequest,
            std::shared_ptr<ContextResponse> contextResponse,
            std::function<Ret(ContextRequest*, Args...)> request,
            std::function<void(ContextResponse*, Ret)> response,
            std::tuple<Args...> params)
        : mContextRequest(contextRequest)
        , mContextResponse(contextResponse)
        , mRequest(request)
        , mResponse(response)
        , mLambdaResponse()
        //, mParams(params)
        , mResponseThread(currentThreadName())
        , mIsLamdaResponse(false)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }
    Request(std::shared_ptr<ContextRequest> contextRequest,
            std::function<Ret(ContextRequest*, Args...)> request,
            std::function<void(Ret)> response,
            std::tuple<Args...> params)
        : mContextRequest(contextRequest)
        , mRequest(request)
        , mResponse()
        , mLambdaResponse(response)
        // , mParams(params)
        , mResponseThread(currentThreadName())
        , mIsLamdaResponse(true)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }

    void call() const override
    {
        if (mIsLamdaResponse)
        {
            invoke(ipc::InlineCall<Ret>(
                mResponseThread, mLambdaResponse, std::forward<Ret>(tuple_utils::apply(mContextRequest, mRequest, mParams))));
        }
        else
        {
            invoke(_private::CallBinder(
                mResponseThread,
                std::make_shared<ipc::Call<ContextResponse, Ret>>(
                    mContextResponse, mResponse, std::make_tuple<Ret>(tuple_utils::apply(mContextRequest, mRequest, mParams)))));
        }
    }

private:
    std::shared_ptr<ContextRequest> mContextRequest;
    std::shared_ptr<ContextResponse> mContextResponse;
    std::function<Ret(ContextRequest*, Args...)> mRequest;
    std::function<void(ContextResponse*, Ret)> mResponse;
    std::function<void(Ret)> mLambdaResponse;
    std::tuple<Args...> mParams;
    std::string mResponseThread;
    bool mIsLamdaResponse;
};

template <typename ContextRequest, typename Ret, typename... Args>
class SyncRequest : public _private::ICallable
{
public:
    SyncRequest(std::shared_ptr<ContextRequest> contextRequest,
                std::function<Ret(ContextRequest*, Args...)> request,
                std::tuple<Args...> params)
        : mContextRequest(contextRequest)
        , mRequest(request)
        // , mParams(params)
        , mResult()
        , mMutex()
        , mCV()
        , mIsNotified(false)
    {
        // : operator= should be used instead of copy ctor for fixing "no type named 'type'" issue with one
        // CommonAPI::Variant element in tuple
        mParams = params;
    }

    void call() const override
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mResult = tuple_utils::apply(mContextRequest, mRequest, mParams);
        mIsNotified = true;
        mCV.notify_one();
    }

    Ret& getResult()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCV.wait(lock, [this]() { return mIsNotified; });
        return mResult;
    }

private:
    std::shared_ptr<ContextRequest> mContextRequest;
    std::function<Ret(ContextRequest*, Args...)> mRequest;
    std::tuple<Args...> mParams;
    mutable Ret mResult;
    mutable std::mutex mMutex;
    mutable std::condition_variable mCV;
    mutable bool mIsNotified;
};

template <typename... Args>
class InlineEvent : public ipc::_private::CallBinder
{
public:
    InlineEvent<Args...>(const std::function<void(Args...)>& func, Args... args)
        : CallBinder(ipc::currentThreadName(), std::make_shared<ipc::CallStatic<Args...>>(func, std::make_tuple(args...)))
    {
    }
};

} // namespace ipc
