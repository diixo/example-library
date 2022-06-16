
#include "DeviceManager.hpp"

namespace itc
{

std::string IDeviceManager::threadID = "DevMan";

void DeviceManager::init()
{
   mDeviceManagerCAPIServer->start(shared_from_this());
}

void DeviceManager::connectProfiles(DeviceManagerTypes::ProfileTypes profileTypes)
{
   //logMethod("DeviceManager::connectProfiles", btAddress, log_vector<nsBTCAPI::DeviceManagerTypes::ProfileType>(profileTypes), reply);
   for (auto profileType : profileTypes)
   {
      //connectProfile(btAddress, profileType);
   }
   //reply();
}


void DeviceManagerCAPIServer::connectProfiles(DeviceManagerTypes::ProfileTypes profileTypes)
{
   itc::invoke(DM_CAPI_connectProfiles::Call(mDeviceManager.lock(), profileTypes));
}

} //namespace itc
