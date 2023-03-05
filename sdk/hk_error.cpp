#include "sdk/hk_error.h"

#include <sstream>

HK_Error HK_Error::create(const char * function)
{
    LONG error = 0;
    const char * hk = NET_DVR_GetErrorMsg(&error);

    std::ostringstream message;
    message << function << " (" << error << "): " << hk;
    return HK_Error(message.str(), function, error, hk);
}

HK_Error::HK_Error(std::string full, const char * function, const LONG error, std::string hk)
    : std::runtime_error(std::move(full))
    , myFunction(function)
    , myError(error)
    , myHK(std::move(hk))
{
}

LONG HK_Error::getError() const
{
    return myError;
}

const char * HK_Error::getFunction() const
{
    return myFunction;
}

const std::string & HK_Error::getHKMessage() const
{
    return myHK;
}
