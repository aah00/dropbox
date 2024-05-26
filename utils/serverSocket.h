#include <socket.h>

class ServerSocket : public Socket
{
public:
    ServerSocket(std::string ip_addr = "127.0.0.1", int port = 8050);
    void bind_socket();
    void set_listen_set();
    int accept_connection();
    int send_to(int socket_dp, const void* data, size_t len);
    ssize_t receive_from(int socket_dp, void *buffer, size_t buffer_size);
};