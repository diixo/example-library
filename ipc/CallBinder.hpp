
#pragma once

#include <string>
#include <memory>

namespace ipc {
    namespace _private {

        class ICallable;

        class CallBinder {
        public:
            CallBinder(const std::string& threadName, std::shared_ptr<ipc::_private::ICallable> pCallable) 
                : mpCallable(pCallable)
                , mThreadName(threadName)
                {}
            virtual ~CallBinder() {};
            inline std::shared_ptr<ipc::_private::ICallable> getCallable() const { return mpCallable; }
            inline const std::string& getThreadName() const { return mThreadName; }
        protected:
            std::shared_ptr<ipc::_private::ICallable> mpCallable; 
            const std::string& mThreadName; 
        };
    }
}