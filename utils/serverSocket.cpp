#include <serverSocket.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

ServerSocket::ServerSocket(std::string ip_addr, int port) : Socket(ip_addr, port)
{}

void ServerSocket::bind_socket()
{
    if (bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        std::cerr << "[ERROR] : Bind failed! " << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout <<"[LOG] : Bind Successful." << std::endl;
}

void ServerSocket::set_listen_set()
{
    if (listen(sock, 3) < 0)
    {
        std::cerr << "[ERROR] : Listen failed! " << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"[LOG] : Socket in Listen State (Max Connection Queue: 3)" << std::endl;
}

int ServerSocket::accept_connection()
{
    int socket_dp;
    if ((socket_dp = accept(sock, (struct sockaddr *)&sock_addr, (socklen_t*)&address_length)) < 0)
    {
        std::cerr << "[ERROR] : Accept failed! " << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "[LOG] : Connected to Client." << std::endl;
    return socket_dp;
}

int ServerSocket::send_to(int socket_dp, const void* data, size_t len)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int bytes_sent = send(socket_dp, data, len, 0);
    if (bytes_sent > 0)
    {
        std::cout << "\n[LOG] : Transmitted file Size "<< bytes_sent <<" Bytes."  << std::endl;
    }
    else
    {
        std::cout << "[ERROR] : Failed to send data!" << std::endl;
    }
    return bytes_sent;
}

ssize_t ServerSocket::receive_from(int socket_dp, void *buffer, size_t buffer_size)
{
    auto bytes_recv = read(socket_dp, buffer, buffer_size);
    return bytes_recv;
}