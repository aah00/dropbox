#include <clientSocket.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

ClientSocket::ClientSocket(std::string ip_addr, int port) : Socket(ip_addr, port)
{}

bool ClientSocket::create_connection()
{
    if (connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        std::cout <<"[ERROR] : Connection attempt failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout <<"[LOG] : Connection Successfull." << std::endl;
}

ssize_t ClientSocket::send_to(const void* data, size_t len)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto bytes_sent = send(sock, data, len, 0);
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

ssize_t ClientSocket::receive_from(char buffer[], size_t buffer_size)
{
    int bytes_recv = read(sock, buffer, buffer_size);
    return bytes_recv;
}