#pragma once

#include "HCNetSDK.h"
#include <memory>

class HK_Callback_V40;

class HK_Playback
{
public:
    HK_Playback(const LONG handle, const std::shared_ptr<HK_Callback_V40> & callback);
    ~HK_Playback();

    void resize();

    void command(const DWORD command, void * ptr = nullptr, const size_t len = 0);

    NET_DVR_TIME getOSDTime() const;
    size_t getDownloadPos() const;

private:
    const LONG myHandle;
    const std::shared_ptr<HK_Callback_V40> myCallback;
};
