#include "sdk/hk_error.h"

HK_Error::HK_Error(const char * function)
    : std::runtime_error(function)
    , myError(NET_DVR_GetLastError())
{
    LONG error = myError;
    myMessage = NET_DVR_GetErrorMsg(&error);
}

LONG HK_Error::getError() const
{
    return myError;
}

const char * HK_Error::getMessage() const
{
    return myMessage;
}
