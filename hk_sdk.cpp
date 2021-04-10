#include "hk_sdk.h"
#include "hk_error.h"

#include "HCNetSDK.h"

#include <sstream>
#include <iostream>

namespace
{

    void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void * /* pUser */)
    {
        std::cerr << "Got some exception: " << dwType << " for user: " << lUserID << " and handle: " << lHandle << std::endl;
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
