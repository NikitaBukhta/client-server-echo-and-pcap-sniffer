#include "connectionError.h"

using namespace cs;

ConnectionError::ConnectionError(const std::string& msg)
{
    errorMessage = msg;
}

const char* ConnectionError::what(void) const noexcept
{
    return errorMessage.c_str();
}