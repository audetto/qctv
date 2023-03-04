#include "sdk/hk_liveplayer.h"
#include "sdk/hk_sdk.h"

#include "sdk/hk_utils.h"

HK_LivePlayer::HK_LivePlayer(const LONG handle, const size_t channel)
    : myHandle(handle)
    , myChannel(channel)
    , myRecording(false)
{

}

HK_LivePlayer::~HK_LivePlayer()
{
    stop();
    NET_DVR_StopRealPlay(myHandle);
}

void HK_LivePlayer::resize()
{
    NET_DVR_ChangeWndResolution(myHandle);
}

size_t HK_LivePlayer::getChannel() const
{
    return myChannel;
}

void HK_LivePlayer::snapshot(const std::string & filename) const
{
    if (!NET_DVR_CapturePicture(myHandle, cast(filename)))
    {
        HK_SDK::error("NET_DVR_CapturePicture");
    }
}

void HK_LivePlayer::stop()
{
    if (myRecording)
    {
        NET_DVR_StopSaveRealData(myChannel);
        myRecording = false;
    }
}

void HK_LivePlayer::record(const std::string & filename)
{
    stop();
    if (!NET_DVR_SaveRealData(myHandle, cast(filename)))
    {
        HK_SDK::error("NET_DVR_SaveRealData");
    }
    myRecording = true;
}
