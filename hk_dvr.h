#ifndef HK_DVR_H
#define HK_DVR_H

#include "HCNetSDK.h"
#include <string>
#include <memory>

class HK_SDK;
class HK_Play;


class HK_DVR
{
public:
    HK_DVR(const std::shared_ptr<const HK_SDK> & sdk, const std::string & ip, const WORD port, const std::string & username, const std::string & password);

    const NET_DVR_DEVICEINFO_V40 & getDeviceInfo() const;

    std::shared_ptr<HK_Play> getPlayer(const LONG channel, const HWND window) const;

    ~HK_DVR();

private:

    [[ noreturn ]] void error(const char * msg) const;
    void debug(const char * msg) const;

    const std::shared_ptr<const HK_SDK> mySDK;
    LONG myUserID;
    NET_DVR_DEVICEINFO_V40 myDeviceInfo;
};

#endif // HK_DVR_H
