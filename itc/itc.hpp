
#include "LogInfo.hpp"
#include "ICallable.hpp"
#include "CallBinder.hpp"
#include "Call.hpp"
#include "EventLoop.hpp"
#include "Dispatcher.hpp"


// Declares global alias for global or static function which could be called on specified thread from any thread in application
// * CONNECTOR - alias for call, should be UNIQUE for all application
// * THREAD - thread where function will be calling, should be created and registered in Dispatcher by void createEventLoop(const
// std::string& threadName);
// * METHOD - full name of function with namespaces and class names, function should be public and return void
// * ... function argument types
#define DECLARE_STATIC_CALL(CONNECTOR, THREAD, METHOD, ...)                                                    \
    namespace CONNECTOR {                                                                                      \
    template <typename... Args>                                                                                \
    class TCallStatic : public itc::_private::CallBinder                                                       \
    {                                                                                                          \
    public:                                                                                                    \
        TCallStatic(Args... args)                                                                              \
            : CallBinder(THREAD, std::make_shared<itc::CallStatic<Args...>>(METHOD, std::make_tuple(args...))) \
        {                                                                                                      \
        }                                                                                                      \
    };                                                                                                         \
    typedef TCallStatic<__VA_ARGS__> CallStatic;                                                               \
    }

// Declares global alias for member function which could be called on specified thread from any thread in application
// * CONNECTOR - alias for call, should be UNIQUE for all application
// * THREAD - thread where function will be calling, should be created and registered in Dispatcher by void createEventLoop(const
// std::string& threadName);
// * CLASS - full name of class where function declared with namespaces and class names
// * METHOD - name of function without namespaces and class names, function should be public and return void
// * ... function argument types
#define DECLARE_CALL(CONNECTOR, THREAD, CLASS, METHOD, ...)                                          \
    namespace CONNECTOR {                                                                            \
    template <typename... Args>                                                                      \
    class TCall : public itc::_private::CallBinder                                                   \
    {                                                                                                \
    public:                                                                                          \
        TCall<Args...>(std::shared_ptr<CLASS> context, Args... args)                                 \
            : CallBinder(THREAD,                                                                     \
                         std::make_shared<itc::Call<CLASS, ##__VA_ARGS__>>(                          \
                             context,                                                                \
                             std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)), \
                             std::make_tuple(args...)))                                              \
        {                                                                                            \
        }                                                                                            \
    };                                                                                               \
    typedef TCall<__VA_ARGS__> Call;                                                                 \
    }

// Declares global alias for global or static function which will be called thread where it invoked asynchronously
// * CONNECTOR - alias for event, should be UNIQUE for all application
// * METHOD - full name of function with namespaces and class names, function should be public and return void
// * ... function argument types
#define DECLARE_STATIC_EVENT(CONNECTOR, METHOD, ...)                                                                                 \
    namespace CONNECTOR {                                                                                                            \
    template <typename... Args>                                                                                                      \
    class TEvent : public itc::_private::CallBinder                                                                                  \
    {                                                                                                                                \
    public:                                                                                                                          \
        TEvent<Args...>(Args... args)                                                                                                \
            : CallBinder(itc::currentThreadName(), std::make_shared<itc::CallStatic<__VA_ARGS__>>(METHOD, std::make_tuple(args...))) \
        {                                                                                                                            \
        }                                                                                                                            \
    };                                                                                                                               \
    typedef TEvent<__VA_ARGS__> Event;                                                                                               \
    }

// Declares global alias for global or static function with return value which will be called thread where it invoked asynchronously
// * CONNECTOR - alias for event, should be UNIQUE for all application
// * METHOD - full name of function with namespaces and class names, function should be public and return void
// * RET - function return value type
// * ... function argument types
#define DECLARE_SYNC_STATIC_EVENT(CONNECTOR, METHOD, RET, ...)                                                                  \
    namespace CONNECTOR {                                                                                                       \
    template <typename... Args>                                                                                                 \
    class TEvent : public itc::_private::CallBinder                                                                             \
    {                                                                                                                           \
    public:                                                                                                                     \
        TEvent<Args...>(Args... args)                                                                                           \
            : CallBinder(itc::currentThreadName(),                                                                              \
                         std::make_shared<itc::SyncCallStatic<RET, ##__VA_ARGS__>>(METHOD, std::make_tuple(args...)))           \
        {                                                                                                                       \
        }                                                                                                                       \
        RET& getResult() { return std::static_pointer_cast<itc::SyncCallStatic<RET, ##__VA_ARGS__>>(mpCallable)->getResult(); } \
    };                                                                                                                          \
    typedef TEvent<__VA_ARGS__> Event;                                                                                          \
    }

// Declares global alias for function type which will be called thread where it invoked asynchronously
// * CONNECTOR - alias for event, should be UNIQUE for all application
// * ... function argument types
#define DECLARE_INLINE_EVENT(CONNECTOR, ...)                                                                         \
    namespace CONNECTOR {                                                                                            \
    template <typename... Args>                                                                                      \
    class TEvent : public itc::_private::CallBinder                                                                  \
    {                                                                                                                \
    public:                                                                                                          \
        TEvent<Args...>(const std::function<void(Args...)>& func, Args... args)                                      \
            : CallBinder(itc::currentThreadName(), new itc::CallStatic<__VA_ARGS__>(func, std::make_tuple(args...))) \
        {                                                                                                            \
        }                                                                                                            \
    };                                                                                                               \
    typedef TEvent<__VA_ARGS__> Event;                                                                               \
    }

// Declares global alias for member function which will be called thread where it invoked asynchronously
// * CONNECTOR - alias for event, should be UNIQUE for all application
// * CLASS - full name of class where function declared with namespaces and class names
// * METHOD - name of function without namespaces and class names, function should be public and return void
// * ... function argument types
#define DECLARE_EVENT(CONNECTOR, CLASS, METHOD, ...)                                                 \
    namespace CONNECTOR {                                                                            \
    template <typename... Args>                                                                      \
    class TEvent : public itc::_private::CallBinder                                                  \
    {                                                                                                \
    public:                                                                                          \
        TEvent<Args...>(std::shared_ptr<CLASS> context, Args... args)                                \
            : CallBinder(itc::currentThreadName(),                                                   \
                         std::make_shared<itc::Call<CLASS, ##__VA_ARGS__>>(                          \
                             context,                                                                \
                             std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)), \
                             std::make_tuple(args...)))                                              \
        {                                                                                            \
        }                                                                                            \
    };                                                                                               \
    typedef TEvent<__VA_ARGS__> Event;                                                               \
    }

// Declares global alias for request which could be sent to member function in specified thread and result value of this function
// received in member function in current thread
// * CONNECTOR - alias for request, should be UNIQUE for all application
// * THREAD - thread where function will be calling, should be created and registered in Dispatcher by void createEventLoop(const
// std::string& threadName);
// * CLASS_RESPONSE - full name of class where response will be received
// * METHOD_RESPONSE - name of method in CLASS_RESPONSE where result will be received, should have only one argument with type RET
// * RET - type of result value
// * CLASS_REQUEST - full name of class where request will be processed
// * METHOD_REQUEST - name of function in class METHOD_REQUEST which will process request, function should be public and return value of type
// RET
// * ... METHOD_REQUEST argument types
#define DECLARE_REQUEST(CONNECTOR, THREAD, CLASS_RESPONSE, METHOD_RESPONSE, RET, CLASS_REQUEST, METHOD_REQUEST, ...)                    \
    namespace CONNECTOR {                                                                                                               \
    template <typename... Args>                                                                                                         \
    class TRequest : public itc::_private::CallBinder                                                                                   \
    {                                                                                                                                   \
    public:                                                                                                                             \
        TRequest<Args...>(std::shared_ptr<CLASS_REQUEST> contextRequest, std::shared_ptr<CLASS_RESPONSE> contextResponse, Args... args) \
            : CallBinder(THREAD,                                                                                                        \
                         std::make_shared<itc::Request<CLASS_REQUEST, CLASS_RESPONSE, RET, ##__VA_ARGS__>>(                             \
                             contextRequest,                                                                                            \
                             contextResponse,                                                                                           \
                             std::mem_fn(static_cast<RET (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)),             \
                             std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(RET)>(&CLASS_RESPONSE::METHOD_RESPONSE)),                 \
                             std::make_tuple(args...)))                                                                                 \
        {                                                                                                                               \
        }                                                                                                                               \
        TRequest<Args...>(std::shared_ptr<CLASS_REQUEST> contextRequest, std::function<void(RET)> response, Args... args)               \
            : CallBinder(THREAD,                                                                                                        \
                         std::make_shared<itc::Request<CLASS_REQUEST, CLASS_RESPONSE, RET, ##__VA_ARGS__>>(                             \
                             contextRequest,                                                                                            \
                             std::mem_fn(static_cast<RET (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)),             \
                             response,                                                                                                  \
                             std::make_tuple(args...)))                                                                                 \
        {                                                                                                                               \
        }                                                                                                                               \
    };                                                                                                                                  \
    typedef TRequest<__VA_ARGS__> Request;                                                                                              \
    }

// Declares global alias for request which could be sent to member function with return value in specified thread and result value of this
// function received in member function in current thread
// * CONNECTOR - alias for request, should be UNIQUE for all application
// * THREAD - thread where function will be calling, should be created and registered in Dispatcher by void createEventLoop(const
// std::string& threadName);
// * CLASS_REQUEST - full name of class where request will be processed
// * METHOD_REQUEST - name of function in class METHOD_REQUEST which will process request, function should be public and return value of type
// RET
// * RET - type of result value
// * ... METHOD_REQUEST argument types
#define DECLARE_SYNC_REQUEST(CONNECTOR, THREAD, CLASS_REQUEST, METHOD_REQUEST, RET, ...)                                    \
    namespace CONNECTOR {                                                                                                   \
    template <typename... Args>                                                                                             \
    class TRequest : public itc::_private::CallBinder                                                                       \
    {                                                                                                                       \
    public:                                                                                                                 \
        TRequest<Args...>(std::shared_ptr<CLASS_REQUEST> contextRequest, Args... args)                                      \
            : CallBinder(THREAD,                                                                                            \
                         std::make_shared<itc::SyncRequest<CLASS_REQUEST, RET, ##__VA_ARGS__>>(                             \
                             contextRequest,                                                                                \
                             std::mem_fn(static_cast<RET (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)), \
                             std::make_tuple(args...)))                                                                     \
        {                                                                                                                   \
        }                                                                                                                   \
        RET& getResult()                                                                                                    \
        {                                                                                                                   \
            return std::static_pointer_cast<itc::SyncRequest<CLASS_REQUEST, RET, ##__VA_ARGS__>>(mpCallable)->getResult();  \
        }                                                                                                                   \
    };                                                                                                                      \
    typedef TRequest<__VA_ARGS__> Request;                                                                                  \
    }

namespace itc {

extern const std::string UNKNOWN_THREAD_NAME;

// Stop event loop
void stopEventLoop(const std::string& threadName);

// Returns name of current thread
const std::string& currentThreadName();

} // namespace itc
