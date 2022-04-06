#include "posix_wrapping.h"

#include <iostream>

int main(void)
{
    try
    {
        Socket(2, 2, 2);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    int a = 2 + 2;
    
    return 0;
}