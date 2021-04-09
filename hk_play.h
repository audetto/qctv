#ifndef HK_PLAY_H
#define HK_PLAY_H

#include "HCNetSDK.h"

class HK_Play
{
public:
    HK_Play(const LONG handle);

    ~HK_Play();

private:

    const LONG myHandle;

};

#endif // HK_PLAY_H
