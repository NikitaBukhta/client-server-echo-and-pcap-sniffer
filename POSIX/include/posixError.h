#ifndef POSIX_ERROR_H
#define POSIX_ERROR_H

#include <exception>

namespace POSIX
{
    class PosixError : public std::exception
    {
    private:
        char *errorMessage;

    public:
        PosixError(const char *msg);
        
        ~PosixError(void);

        virtual const char* what(void) const noexcept override;
    };
};

#endif // !POSIX_ERROR_H