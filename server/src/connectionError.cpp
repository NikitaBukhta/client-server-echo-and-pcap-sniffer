#include "connectionError.h"

cs::ConnectionError::ConnectionError(const std::string& msg)
{
    errorMessage = msg;
}

const char* cs::ConnectionError::what(void) const noexcept
{
    return errorMessage.c_str();
}