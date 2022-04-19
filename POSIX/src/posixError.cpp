#include "posixError.h"

#include <string.h>

POSIX::PosixError::PosixError(const std::string& msg)
{
    errorMessage = msg;
}

const char* POSIX::PosixError::what(void) const noexcept
{
    return errorMessage.c_str();
}