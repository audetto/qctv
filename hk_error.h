#ifndef HK_ERROR_H
#define HK_ERROR_H

#include "HCNetSDK.h"

#include <stdexcept>

class HK_Error : public std::runtime_error
{
public:
    HK_Error(const char * function);

    LONG getError() const;
private:
    const LONG myError;
};


#endif // HK_ERROR_H
