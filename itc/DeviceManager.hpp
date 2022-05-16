
#pragma once

#include <string.h>

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
   DeviceManager() {};
   ~DeviceManager() = default;

   void init() override;
};

std::string itc::IDeviceManager::threadID = "DevMan";

inline
void DeviceManager::init()
{
   // TODO:
}

} //namespace itc
