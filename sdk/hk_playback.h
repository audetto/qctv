#pragma once

#include "HCNetSDK.h"
#include <memory>

class HK_Callback_V40;

class HK_Playback
{
public:
    HK_Playback(const LONG handle, const std::shared_ptr<const HK_Callback_V40> & callback);
    ~HK_Playback();

    void resize();

    void command(const DWORD command);

    NET_DVR_TIME getOSDTime() const;
    LONG getDownloadPos() const;

private:
    const LONG myHandle;
    const std::shared_ptr<const HK_Callback_V40> myKeepAliveCallback;
};
