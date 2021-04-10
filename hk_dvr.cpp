#include "hk_dvr.h"
#include "hk_sdk.h"
#include "hk_play.h"

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

std::shared_ptr<HK_Play> HK_DVR::getPlayer(const size_t channel, const HWND window) const
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
        return std::make_shared<HK_Play>(realPlayHandle, channel);
    }
}
