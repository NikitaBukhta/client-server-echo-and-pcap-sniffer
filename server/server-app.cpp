#include "posixError.h"
#include "server.h"

#include <iostream>

int main(int argc, char **argv)
{
    Server server(12345);

    std::cout << "End of program!" << std::endl;

    return 0;
}