#include "sdk/hk_callback.h"


void HK_Callback_V40::playDataCallBack(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
    HK_Callback_V40 * callback = static_cast<HK_Callback_V40 *>(pUser);
    callback->moreData(lPlayHandle, dwDataType, pBuffer, dwBufSize);
}
