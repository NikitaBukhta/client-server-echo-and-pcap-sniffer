#include "posixError.h"

#include <string.h>

POSIX::PosixError::PosixError(const char *msg)
{
    errorMessage = new char[strlen(msg)];
    strcpy(errorMessage, msg);
}

POSIX::PosixError::~PosixError(void)
{
    delete[] errorMessage;
}

const char* POSIX::PosixError::what(void) const noexcept
{
    return errorMessage;
}