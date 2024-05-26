#include <socket.h>

class ClientSocket : public Socket
{
public:
    ClientSocket(std::string ip_addr = "127.0.0.1", int port = 8050);
    bool create_connection();
    ssize_t send_to(const void* data, size_t len);
    ssize_t receive_from(char buffer[], size_t buffer_size);
};