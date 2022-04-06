#include "posix_wrapping.h"

#include <iostream>
#include <unistd.h>

int main(void)
{
    int server = -1;
    sockaddr_in serverAddress = {0};
    //serverAddress.sin_family = AF_INET;
    //serverAddress.sin_port = htons(12345);
    try
    {
        int server = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        Bind(-1, (sockaddr*)(&serverAddress), sizeof(serverAddress));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    if (server != -1) close(server);
    
    return 0;
}