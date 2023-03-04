#include "sdk/hk_sdk.h"
#include "sdk/hk_error.h"

#include "HCNetSDK.h"
#include "PlayM4.h"

#include <sstream>
#include <iostream>

namespace
{

    void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void * /* pUser */)
    {
        std::cerr << "Got some exception: " << dwType << " for user: " << lUserID << " and handle: " << lHandle << std::endl;
    }

    std::string getVersionString(const DWORD version)
    {
        const size_t major = (version >> 24) & 0xFF;
        const size_t minor = (version >> 16) & 0xFF;
        const size_t b1    = (version >> 8) & 0xFF;
        const size_t b2    = version & 0xFF;

        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(b1) + "." + std::to_string(b2);
    }

}

HK_SDK::HK_SDK(const bool logPrint)
{
    NET_DVR_Init();
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    NET_DVR_SetLogPrint(logPrint);
}

HK_SDK::~HK_SDK()
{
    NET_DVR_Cleanup();
}

[[ noreturn ]] void HK_SDK::error(const char * msg)
{
    debug(msg);
    throw HK_Error(msg);
}

void HK_SDK::debug(const char * msg)
{
    LONG err = NET_DVR_GetLastError();
    std::ostringstream ss;

    ss << msg << ": " << err;
    ss << " = " << NET_DVR_GetErrorMsg(&err);
    std::cerr << ss.str() << std::endl;
}

std::string HK_SDK::getSDKVersion()
{
    const DWORD version = NET_DVR_GetSDKBuildVersion();
    return getVersionString(version);
}

std::string HK_SDK::getPlayerVersion()
{
    const DWORD version = PlayM4_GetSdkVersion();
    return getVersionString(version);
}
