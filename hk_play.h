#ifndef HK_PLAY_H
#define HK_PLAY_H

#include "HCNetSDK.h"

#include <string>

class HK_Play
{
public:
    HK_Play(const LONG handle, const size_t channel);
    ~HK_Play();

    void resize();

    void snapshot(const std::string & filename) const;
    size_t getChannel() const;

    void record(const std::string & filename);
    void stop();

private:
    const LONG myHandle;
    const size_t myChannel;
    bool myRecording;
};

#endif // HK_PLAY_H
