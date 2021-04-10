#include "hk_play.h"
#include "hk_sdk.h"

#include "utils.h"

HK_Play::HK_Play(const LONG handle, const size_t channel)
    : myHandle(handle)
    , myChannel(channel)
{

}

HK_Play::~HK_Play()
{
    NET_DVR_StopRealPlay(myHandle);
}

void HK_Play::resize()
{
    NET_DVR_ChangeWndResolution(myHandle);
}

size_t HK_Play::getChannel() const
{
    return myChannel;
}

void HK_Play::snapshot(const std::string & filename) const
{
    if (!NET_DVR_CapturePicture(myHandle, cast(filename)))
    {
        HK_SDK::error("NET_DVR_CapturePicture");
    }
}
