#ifndef POSIX_ERROR_H
#define POSIX_ERROR_H

#include <exception>
#include <string>

namespace POSIX
{
    class PosixError : public std::exception
    {
    private:
        std::string errorMessage;

    public:
        PosixError(const std::string& msg);

        virtual const char* what(void) const noexcept override;
    };
}

#endif // !POSIX_ERROR_H