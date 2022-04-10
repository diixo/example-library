
#pragma once

namespace itc {

namespace _private {

class ICallable
{
public:
   virtual ~ICallable() = default;
   virtual void call() const = 0;
};

}
}
