#include "hk_play.h"

HK_Play::HK_Play(const LONG handle)
    : myHandle(handle)
{

}

HK_Play::~HK_Play()
{
    NET_DVR_StopRealPlay(myHandle);
}
