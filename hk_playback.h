#ifndef HK_PLAYBACK_H
#define HK_PLAYBACK_H

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

#endif // HK_PLAYBACK_H
