#include "hk_sdk.h"
#include "HCNetSDK.h"

#include <iostream>

namespace
{

    void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void * /* pUser */)
    {
        std::cerr << "Got some exception: " << dwType << " for user: " << lUserID << " and handle: " << lHandle << std::endl;
    }

}

HK_SDK::HK_SDK()
{
    NET_DVR_Init();
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    NET_DVR_SetLogPrint(true);
}

HK_SDK::~HK_SDK()
{
    NET_DVR_Cleanup();
}
