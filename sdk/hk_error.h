#pragma once

#include "HCNetSDK.h"

#include <stdexcept>

class HK_Error : public std::runtime_error
{
public:
    HK_Error(const char * function);

    LONG getError() const;

    const char * getMessage() const;
private:
    const LONG myError;
    const char * myMessage;
};
