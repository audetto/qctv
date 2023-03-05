#pragma once

#include "HCNetSDK.h"

#include <stdexcept>
#include <string>


class HK_Error : public std::runtime_error
{
public:
    static HK_Error create(const char * function);

    LONG getError() const;
    const char * getFunction() const;
    const std::string & getHKMessage() const;
private:
    HK_Error(std::string full, const char * function, const LONG error, std::string hk);

    const char * myFunction;
    const LONG myError;
    const std::string myHK;
};
