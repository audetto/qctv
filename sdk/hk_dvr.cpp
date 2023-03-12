#include "sdk/hk_dvr.h"
#include "sdk/hk_callback.h"
#include "sdk/hk_sdk.h"
#include "sdk/hk_liveplayer.h"
#include "sdk/hk_playback.h"

#include <cstring>

HK_DVR::HK_DVR(const std::shared_ptr<const HK_SDK> & sdk, const std::string & ip, const WORD port, const std::string & username, const std::string & password)
    : mySDK(sdk)
{
    NET_DVR_USER_LOGIN_INFO loginInfo = {};
    myDeviceInfo = {};
    loginInfo.bUseAsynLogin = FALSE;

    loginInfo.wPort = port;
    strncpy(loginInfo.sDeviceAddress, ip.c_str(), sizeof(loginInfo.sDeviceAddress) - 1);
    strncpy(loginInfo.sUserName, username.c_str(), sizeof(loginInfo.sUserName) - 1);
    strncpy(loginInfo.sPassword, password.c_str(), sizeof(loginInfo.sPassword) - 1);

    myUserID = NET_DVR_Login_V40(&loginInfo, &myDeviceInfo);

    if (myUserID < 0)
    {
        HK_SDK::error("NET_DVR_Login_V40");
    }
}

HK_DVR::~HK_DVR()
{
    const BOOL ok = NET_DVR_Logout(myUserID);
    myUserID = 0;
    if (!ok)
    {
        HK_SDK::debug("NET_DVR_Logout");
    }
}

std::shared_ptr<HK_LivePlayer> HK_DVR::getLivePlayer(const size_t channel, const HWND window) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_PREVIEWINFO struPlayInfo = {};
    struPlayInfo.hPlayWnd     = 0;
    struPlayInfo.lChannel     = dChannel;
    struPlayInfo.dwStreamType = 0;       //0-Main Stream, 1-Sub Stream, 2-Stream 3, 3-Stream 4, and so on
    struPlayInfo.dwLinkMode   = 0;       //0- TCP Mode, 1- UDP Mode, 2- Multicast Mode, 3- RTP Mode, 4-RTP/RTSP, 5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- Non-blocking Streaming, 1- Blocking Streaming
    struPlayInfo.hPlayWnd     = window;

    const LONG realPlayHandle = NET_DVR_RealPlay_V40(myUserID, &struPlayInfo, nullptr, nullptr);
    if (realPlayHandle < 0)
    {
        HK_SDK::error("NET_DVR_RealPlay_V40");
    }
    else
    {
        return std::make_shared<HK_LivePlayer>(realPlayHandle, channel);
    }
}

std::shared_ptr<HK_Playback> HK_DVR::getPlayback(const size_t channel, const HWND window, const NET_DVR_TIME_V50 & start,
    const NET_DVR_TIME_V50 & end, const std::shared_ptr<HK_Callback_V40> & callback) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_VOD_PARA_V50 struVodPara = {};

    struVodPara.dwSize                  = sizeof(struVodPara);
    struVodPara.struBeginTime           = start;
    struVodPara.struEndTime             = end;
    struVodPara.struIDInfo.dwSize       = sizeof(struVodPara.struIDInfo);
    struVodPara.struIDInfo.dwChannel    = dChannel;
    struVodPara.hWnd                    = window;

    const LONG hPlayback = NET_DVR_PlayBackByTime_V50(myUserID, &struVodPara);

    if (hPlayback < 0)
    {
        HK_SDK::error("NET_DVR_PlayBackByTime_V50");
    }

    if (callback)
    {
        if (!NET_DVR_SetPlayDataCallBack_V40(hPlayback, HK_Callback_V40::playDataCallBack, callback.get()))
        {
            HK_SDK::error("NET_DVR_SetPlayDataCallBack_V40");
        }
    }

    return std::make_shared<HK_Playback>(hPlayback, callback);
}

std::shared_ptr<HK_Playback> HK_DVR::getReversePlayback(const size_t channel, const HWND window, const NET_DVR_TIME & start,
    const NET_DVR_TIME & end) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_PLAYCOND struVodPara = {};

    struVodPara.dwChannel               = dChannel;
    struVodPara.struStartTime           = start;
    struVodPara.struStopTime            = end;

    const LONG hPlayback = NET_DVR_PlayBackReverseByTime_V40(myUserID, window, &struVodPara);

    if (hPlayback < 0)
    {
        HK_SDK::error("NET_DVR_PlayBackReverseByTime_V40");
    }

    return std::make_shared<HK_Playback>(hPlayback, nullptr);
}

void HK_DVR::getDeviceAbility(const DWORD dwAbilityType, std::vector<char> & in, std::vector<char> & out) const
{
    const BOOL ret = NET_DVR_GetDeviceAbility(myUserID, dwAbilityType, in.data(), in.size(), out.data(), out.size());
    if (!ret)
    {
        HK_SDK::error("NET_DVR_GetDeviceAbility");
    }
}

LONG HK_DVR::getDigitalChannel(const size_t channel) const
{
    return myDeviceInfo.struDeviceV30.byStartDChan + channel;
}
