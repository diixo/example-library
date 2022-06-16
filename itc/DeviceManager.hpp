
#pragma once

#include <string>
#include <memory>

#include "common/CommandsQueue.hpp"
#include "itc.hpp"


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

class DeviceManagerCAPIServer;

class DeviceManager final
   : public itc::IDeviceManager
   , public std::enable_shared_from_this<DeviceManager>
{
public:
   DeviceManager()
      : mCommandsQueue(std::make_shared<CommandsQueue>())
      , mDeviceManagerCAPIServer(std::make_shared<DeviceManagerCAPIServer>())
   {}

   ~DeviceManager() = default;

   void init() override;
   void connectProfiles(DeviceManagerTypes::ProfileTypes profileTypes);

   std::shared_ptr<CommandsQueue> mCommandsQueue;
   std::shared_ptr<DeviceManagerCAPIServer> mDeviceManagerCAPIServer;

};

DECLARE_CALL(DM_CAPI_connectProfiles,
   IDeviceManager::threadID,
   DeviceManager,
   connectProfiles,
   DeviceManagerTypes::ProfileTypes)


class DeviceManagerCAPIServer
   : public std::enable_shared_from_this<DeviceManagerCAPIServer>
{
public:
   DeviceManagerCAPIServer() {};

   void start(std::shared_ptr<DeviceManager> deviceManager) { mDeviceManager = deviceManager; }

   void connectProfiles(DeviceManagerTypes::ProfileTypes profileTypes);

private:
   std::weak_ptr<DeviceManager> mDeviceManager;

};

} //namespace itc
