#include "hk_error.h"

HK_Error::HK_Error(const char * function)
    : std::runtime_error(function)
    , myError(NET_DVR_GetLastError())
{
}

LONG HK_Error::getError() const
{
    return myError;
}
