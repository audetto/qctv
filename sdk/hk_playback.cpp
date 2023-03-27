#include "sdk/hk_playback.h"

#include "sdk/hk_callback.h"
#include "sdk/hk_sdk.h"

HK_Playback::HK_Playback(const LONG handle, const std::shared_ptr<HK_Callback_V40> & callback)
    : myHandle(handle)
    , myCallback(callback)
{
    if (callback)
    {
        if (!NET_DVR_SetPlayDataCallBack_V40(myHandle, HK_Callback_V40::playDataCallBack, myCallback.get()))
        {
            // the distructor wont be called
            NET_DVR_StopPlayBack(myHandle);
            HK_SDK::error("NET_DVR_SetPlayDataCallBack_V40");
        }
    }
}

HK_Playback::~HK_Playback()
{
    NET_DVR_StopPlayBack(myHandle);
}

void HK_Playback::resize()
{
    command(NET_DVR_CHANGEWNDRESOLUTION);
}

void HK_Playback::command(const DWORD command, void * ptr, const size_t len)
{
    if (!NET_DVR_PlayBackControl_V40(myHandle, command, ptr, len, nullptr, nullptr))
    {
        HK_SDK::error("NET_DVR_PlayBackControl_V40");
    }
}

NET_DVR_TIME HK_Playback::getOSDTime() const
{
    NET_DVR_TIME osd = {};
    if (!NET_DVR_GetPlayBackOsdTime(myHandle, &osd))
    {
        HK_SDK::error("NET_DVR_GetPlayBackOsdTime");
    }
    return osd;
}

size_t HK_Playback::getDownloadPos() const
{
    return NET_DVR_GetDownloadPos(myHandle);
}