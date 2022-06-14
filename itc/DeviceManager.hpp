
#pragma once

#include <string>
#include <memory>

#include "common/CommandsQueue.hpp"

namespace itc
{
class IDeviceManager
{
public:
   static std::string threadID;

   IDeviceManager() = default;
   virtual ~IDeviceManager() = default;

   virtual void init() = 0;
};

class DeviceManager final
   : public itc::IDeviceManager
   , public std::enable_shared_from_this<DeviceManager>
{
public:
   DeviceManager() //: mCommandsQueue(std::make_shared<CommandsQueue>())
   {};

   ~DeviceManager() = default;

   void init() override;

   //std::shared_ptr<CommandsQueue> mCommandsQueue;
};

} //namespace itc
