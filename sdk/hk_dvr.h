#ifndef HK_DVR_H
#define HK_DVR_H

#include "HCNetSDK.h"
#include <string>
#include <memory>

class HK_SDK;
class HK_LivePlayer;
class HK_Playback;


class HK_DVR
{
public:
    HK_DVR(const std::shared_ptr<const HK_SDK> & sdk, const std::string & ip, const WORD port, const std::string & username, const std::string & password);
    ~HK_DVR();

    const NET_DVR_DEVICEINFO_V40 & getDeviceInfo() const;

    std::shared_ptr<HK_LivePlayer> getLivePlayer(const size_t channel, const HWND window) const;
    std::shared_ptr<HK_Playback> getPlayback(const size_t channel, const HWND window, const NET_DVR_TIME_V50 & start, const NET_DVR_TIME_V50 & end) const;

private:
    const std::shared_ptr<const HK_SDK> mySDK;
    LONG myUserID;
    NET_DVR_DEVICEINFO_V40 myDeviceInfo;
};

#endif // HK_DVR_H
