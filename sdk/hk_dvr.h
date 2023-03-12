#pragma once

#include "HCNetSDK.h"
#include "sdk/hk_callback.h"
#include <string>
#include <memory>
#include <vector>

class HK_SDK;
class HK_LivePlayer;
class HK_Playback;
class HK_Callback_V40;

class HK_DVR
{
public:
    HK_DVR(const std::shared_ptr<const HK_SDK> & sdk, const std::string & ip, const WORD port, const std::string & username, const std::string & password);
    ~HK_DVR();

    const NET_DVR_DEVICEINFO_V40 & getDeviceInfo() const;

    std::shared_ptr<HK_LivePlayer> getLivePlayer(const size_t channel, const HWND window) const;
    std::shared_ptr<HK_Playback> getPlayback(const size_t channel, const HWND window, const NET_DVR_TIME_V50 & start, const NET_DVR_TIME_V50 & end, 
        const std::shared_ptr<HK_Callback_V40> & callback) const;
    std::shared_ptr<HK_Playback> getReversePlayback(const size_t channel, const HWND window, const NET_DVR_TIME & start,
        const NET_DVR_TIME & end) const;

    void getDeviceAbility(const DWORD dwAbilityType, std::vector<char> & in, std::vector<char> & out) const;

    LONG getDigitalChannel(const size_t channel) const;

private:
    const std::shared_ptr<const HK_SDK> mySDK;
    LONG myUserID;
    NET_DVR_DEVICEINFO_V40 myDeviceInfo;
};
