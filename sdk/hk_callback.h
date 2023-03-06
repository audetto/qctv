#pragma once

#include "HCNetSDK.h"

class HK_Callback_V40
{
public:
    virtual ~HK_Callback_V40() = default;
    virtual void moreData(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize) = 0;

    static void CALLBACK playDataCallBack(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);
};
