#ifndef HK_SDK_H
#define HK_SDK_H


class HK_SDK
{
public:
    HK_SDK(const bool logPrint);

    ~HK_SDK();

    [[ noreturn ]] static void error(const char * msg);
    static void debug(const char * msg);

};


#endif // HK_SDK_H
