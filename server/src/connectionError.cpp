#include "connectionError.h"

#include <string.h>

cs::ConnectionError::ConnectionError(char *msg)
{
    errorMessage = new char[strlen(msg)];
    strcpy(errorMessage, msg);
}

cs::ConnectionError::~ConnectionError(void)
{
    delete[] errorMessage;
}

const char* cs::ConnectionError::what(void) const noexcept
{
    return errorMessage;
}