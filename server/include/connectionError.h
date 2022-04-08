#ifndef CONNECTION_ERROR_H
#define CONNECTION_ERROR_H

#include <exception>

namespace cs
{
    class ConnectionError : std::exception
    {
    private:
        char *errorMessage;
        
    public:
        ConnectionError(char *msg);

        ~ConnectionError(void);

        virtual const char* what(void) const noexcept override;
    };
}

#endif // !CONNECTION_ERROR_H