#include <unistd.h>
#include <socket.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Socket::Socket(std::string ip_addr, int port)
            : ip_addr(ip_addr)
            , port(port)
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "[ERROR] : Socket failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    address_length = sizeof(sock_addr);

    if (inet_pton(AF_INET, ip_addr.c_str(), &sock_addr.sin_addr) <= 0)
    {
        std::cerr << "[ERROR] : Invalid address!" << std::endl;
    }
}
