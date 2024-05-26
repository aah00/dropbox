#include <string>
#include<sys/socket.h>
#include<arpa/inet.h>

class Socket
{
private:
    int port;
    std::string ip_addr;

protected:
    int sock;
    int address_length;
    struct sockaddr_in sock_addr;

public:
    Socket(std::string ip_addr, int port);
};