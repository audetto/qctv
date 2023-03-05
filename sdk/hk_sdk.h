#pragma once

#include "sdk/hk_error.h"
#include <string>

class HK_SDK
{
public:
    HK_SDK(const bool logPrint);
    ~HK_SDK();

    static std::string getSDKVersion();
    static std::string getPlayerVersion();

    [[ noreturn ]] static void error(const char * function);
    static HK_Error debug(const char * function);
};
