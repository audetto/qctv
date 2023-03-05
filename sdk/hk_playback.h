#pragma once

#include "HCNetSDK.h"

class HK_Playback
{
public:
    HK_Playback(const LONG handle);
    ~HK_Playback();

    void resize();

    void command(const DWORD command);

    NET_DVR_TIME getOSDTime();

private:
    const LONG myHandle;
};
