#include "hk_playback.h"

#include "hk_sdk.h"

HK_Playback::HK_Playback(const LONG handle)
    : myHandle(handle)
{

}

HK_Playback::~HK_Playback()
{
    NET_DVR_StopPlayBack(myHandle);
}

void HK_Playback::resize()
{
    command(NET_DVR_CHANGEWNDRESOLUTION);
}

void HK_Playback::command(const DWORD command)
{
    if (!NET_DVR_PlayBackControl_V40(myHandle, command, nullptr, 0, nullptr, nullptr))
    {
        HK_SDK::error("NET_DVR_PlayBackControl_V40");
    }
}

NET_DVR_TIME HK_Playback::getOSDTime()
{
    NET_DVR_TIME osd = {};
    if (!NET_DVR_GetPlayBackOsdTime(myHandle, &osd))
    {
        HK_SDK::error("NET_DVR_GetPlayBackOsdTime");
    }
    return osd;
 }
