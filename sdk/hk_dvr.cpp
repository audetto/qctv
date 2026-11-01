#include "sdk/hk_dvr.h"
#include "sdk/hk_liveplayer.h"
#include "sdk/hk_playback.h"
#include "sdk/hk_sdk.h"
#include <chrono>
#include <cstring>
#include <thread>

HK_DVR::HK_DVR(const std::shared_ptr<const HK_SDK> &sdk, const std::string &ip, const WORD port,
               const std::string &username, const std::string &password)
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

const NET_DVR_DEVICEINFO_V40 &HK_DVR::getDeviceInfo() const
{
    return myDeviceInfo;
}

std::shared_ptr<HK_LivePlayer> HK_DVR::getLivePlayer(const size_t channel, const DWORD streamType,
                                                     const HWND window) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_PREVIEWINFO struPlayInfo = {};
    struPlayInfo.hPlayWnd = 0;
    struPlayInfo.lChannel = dChannel;
    struPlayInfo.dwStreamType = streamType;
    struPlayInfo.dwLinkMode = 0; // 0- TCP Mode, 1- UDP Mode, 2- Multicast Mode, 3- RTP Mode, 4-RTP/RTSP, 5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;   // 0- Non-blocking Streaming, 1- Blocking Streaming
    struPlayInfo.hPlayWnd = window;

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

std::shared_ptr<HK_Playback> HK_DVR::getPlayback(const size_t channel, const HWND window,
                                                 const std::shared_ptr<HK_Callback_V40> &callback,
                                                 const NET_DVR_TIME_V50 &start, const NET_DVR_TIME_V50 &end,
                                                 const BYTE byPlayMode) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_VOD_PARA_V50 struVodPara = {};

    struVodPara.dwSize = sizeof(struVodPara);
    struVodPara.struBeginTime = start;
    struVodPara.struEndTime = end;
    struVodPara.struIDInfo.dwSize = sizeof(struVodPara.struIDInfo);
    struVodPara.struIDInfo.dwChannel = dChannel;
    struVodPara.byPlayMode = byPlayMode;
    struVodPara.hWnd = window;

    const LONG hPlayback = NET_DVR_PlayBackByTime_V50(myUserID, &struVodPara);

    if (hPlayback < 0)
    {
        HK_SDK::error("NET_DVR_PlayBackByTime_V50");
    }

    return std::make_shared<HK_Playback>(hPlayback, callback);
}

std::shared_ptr<HK_Playback> HK_DVR::getPlayback(const std::string &filname, const HWND window,
                                                 const std::shared_ptr<HK_Callback_V40> &callback) const
{
    NET_DVR_PLAY_BY_NAME_PARA struPara = {};

    strncpy(struPara.szFileName, filname.c_str(), sizeof(struPara.szFileName));
    struPara.szFileName[sizeof(struPara.szFileName) - 1] = '\0';
    struPara.hWnd = window;

    const LONG hPlayback = NET_DVR_PlayBackByName_V50(myUserID, &struPara);

    if (hPlayback < 0)
    {
        HK_SDK::error("NET_DVR_PlayBackByName_V50");
    }

    return std::make_shared<HK_Playback>(hPlayback, callback);
}

std::shared_ptr<HK_Playback> HK_DVR::getReversePlayback(const std::string &filname, const HWND window,
                                                        const std::shared_ptr<HK_Callback_V40> &callback) const
{
    NET_DVR_PLAY_BY_NAME_PARA struPara = {};

    strncpy(struPara.szFileName, filname.c_str(), sizeof(struPara.szFileName));
    struPara.szFileName[sizeof(struPara.szFileName) - 1] = '\0';
    struPara.hWnd = window;

    const LONG hPlayback = NET_DVR_PlayBackReverseByName_V50(myUserID, &struPara);

    if (hPlayback < 0)
    {
        HK_SDK::error("NET_DVR_PlayBackReverseByName_V50");
    }

    return std::make_shared<HK_Playback>(hPlayback, callback);
}

void HK_DVR::getDeviceAbility(const DWORD dwAbilityType, std::vector<char> &in, std::vector<char> &out) const
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

std::vector<NET_DVR_FINDDATA_V50> HK_DVR::findFiles(const size_t channel, const NET_DVR_TIME_SEARCH_COND &start,
                                                    const NET_DVR_TIME_SEARCH_COND &end) const
{
    const LONG dChannel = myDeviceInfo.struDeviceV30.byStartDChan + channel;

    NET_DVR_FILECOND_V50 fileCond = {};
    fileCond.struStreamID.dwSize = sizeof(fileCond.struStreamID);
    fileCond.struStreamID.dwChannel = dChannel;
    fileCond.dwFileType = 0xFF;
    fileCond.struStartTime = start;
    fileCond.struStopTime = end;
    const LONG lFindHandle = NET_DVR_FindFile_V50(myUserID, &fileCond);
    if (lFindHandle < 0)
    {
        HK_SDK::error("NET_DVR_FindFile_V50");
    }

    std::vector<NET_DVR_FINDDATA_V50> files;
    bool cont = true;
    while (cont)
    {
        NET_DVR_FINDDATA_V50 struFileData = {};
        const LONG result = NET_DVR_FindNextFile_V50(lFindHandle, &struFileData);
        switch (result)
        {
        case NET_DVR_ISFINDING:
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            break;
        }
        case NET_DVR_FILE_SUCCESS:
        {
            files.push_back(struFileData);
            break;
        }
        case NET_DVR_FILE_NOFIND:
        case NET_DVR_NOMOREFILE:
        {
            cont = false;
            break;
        }
        default:
        {
            const HK_Error error = HK_SDK::debug("NET_DVR_FindNextFile_V50");
            NET_DVR_FindClose_V30(lFindHandle);
            throw error;
        }
        }
    }
    NET_DVR_FindClose_V30(lFindHandle);
    return files;
}

NET_DVR_DEVICECFG_V50 HK_DVR::getDeviceConfig() const
{
    NET_DVR_DEVICECFG_V50 config = {};
    config.dwSize = sizeof(config);
    DWORD dwReturn = 0;
    if (!NET_DVR_GetDVRConfig(myUserID, NET_DVR_GET_DEVICECFG_V50, -1, &config, sizeof(config), &dwReturn))
    {
        HK_SDK::error("NET_DVR_GetDVRConfig");
    }
    return config;
}

std::string HK_DVR::getChannelName(const size_t channel) const
{
    NET_DVR_PICCFG_V40 config = {};
    config.dwSize = sizeof(config);
    DWORD dwReturn = 0;
    if (!NET_DVR_GetDVRConfig(myUserID, NET_DVR_GET_PICCFG_V40, getDigitalChannel(channel), &config, sizeof(config),
                              &dwReturn))
    {
        HK_SDK::error("NET_DVR_GetDVRConfig");
    }
    const char *name = reinterpret_cast<const char *>(config.sChanName);
    return std::string(name, strnlen(name, sizeof(config.sChanName)));
}

std::vector<HK_IPChannel> HK_DVR::getIPChannels() const
{
    NET_DVR_IPPARACFG_V40 ipConfig = {};
    ipConfig.dwSize = sizeof(ipConfig);
    DWORD dwReturn = 0;
    if (!NET_DVR_GetDVRConfig(myUserID, NET_DVR_GET_IPPARACFG_V40, 0, &ipConfig, sizeof(ipConfig), &dwReturn))
    {
        HK_SDK::error("NET_DVR_GetDVRConfig");
    }

    std::vector<HK_IPChannel> ipChannels(std::min<DWORD>(ipConfig.dwDChanNum, MAX_CHANNUM_V40));
    for (size_t i = 0; i < ipChannels.size(); ++i)
    {
        const NET_DVR_STREAM_MODE &mode = ipConfig.struStreamMode[i];
        if (mode.byGetStreamType != 0) // 0- Unused, 1- IP Channel, 2- Analog Channel
        {
            // URL
            continue;
        }

        const NET_DVR_IPCHANINFO &info = mode.uGetStream.struChanInfo;
        const int device = info.byIPID + info.byIPIDHigh * 256;
        if (device <= 0 || device > MAX_IP_DEVICE)
        {
            // Invalid device
            continue;
        }

        HK_IPChannel &channel = ipChannels[i];
        channel.attached = true;
        channel.online = info.byEnable != 0;

        const char *ip = ipConfig.struIPDevInfo[device - 1].struIP.sIpV4;
        channel.ip = std::string(ip, strnlen(ip, sizeof(ipConfig.struIPDevInfo[device - 1].struIP.sIpV4)));

        try
        {
            channel.name = getChannelName(i);
        }
        catch (const HK_Error &)
        {
            // Ignore errors when retrieving channel name
        }
    }
    return ipChannels;
}
