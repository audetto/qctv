#ifndef HK_SDK_H
#define HK_SDK_H

#include <string>

class HK_SDK
{
public:
    HK_SDK(const bool logPrint);

    ~HK_SDK();

    static std::string getSDKVersion();
    static std::string getPlayerVersion();

    [[ noreturn ]] static void error(const char * msg);
    static void debug(const char * msg);

};


#endif // HK_SDK_H
