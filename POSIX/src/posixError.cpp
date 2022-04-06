#include "posixError.h"

#include <string.h>

PosixError::PosixError(const char *msg)
{
    errorMessage = new char[strlen(msg)];
    strcpy(errorMessage, msg);
}

PosixError::~PosixError(void)
{
    delete[] errorMessage;
}

const char* PosixError::what(void) const noexcept
{
    return errorMessage;
}