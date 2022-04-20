#ifndef CONNECTION_ERROR_H
#define CONNECTION_ERROR_H

#include <exception>
#include <string>

namespace cs
{
    class ConnectionError : std::exception
    {
    private:
        std::string errorMessage;
        
    public:
        ConnectionError(const std::string& msg);

        virtual const char* what(void) const noexcept override;
    };
}

#endif // !CONNECTION_ERROR_H