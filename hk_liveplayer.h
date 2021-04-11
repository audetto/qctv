#ifndef HK_LIVEPLAYER_H
#define HK_LIVEPLAYER_H

#include "HCNetSDK.h"

#include <string>

class HK_LivePlayer
{
public:
    HK_LivePlayer(const LONG handle, const size_t channel);
    ~HK_LivePlayer();

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

#endif // HK_LIVEPLAYER_H
