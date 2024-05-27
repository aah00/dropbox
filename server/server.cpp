#include<iostream>
#include<fstream>
#include <fileop.h>
#include <serverSocket.h>

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    // Get the directory name as argument
    std::string dir_name = "dest_dir";
    if (argc > 1)
    {
        dir_name = argv[1];
    }

    // Start the server
    ServerSocket s;
    s.bind_socket();
    s.set_listen_set();

    // Wait for client connection
    auto client_dp = s.accept_connection();
    if(client_dp > 0)
    {
        std::cout << "Server receives connection request!" << std::endl;
    }

    try
    {
        int debug_ctr = 0;
        ssize_t content_size = 0;
        do
        {
            // Wait for filename, filesize and content
            char content[1024];
            content_size = s.receive_from(client_dp, content, sizeof content);
            if (content_size > 0)
            {
                std::cout << "Server Received content: \n";
                std::cout << content << std::endl;
                try
                {
                    auto j = json::parse(content);
                    for (const auto& cur : j.items())
                    {
                        std::string fname = cur.key();
                        if(cur.value()["is_dir"] == false)
                        {
                            dropbox::sync_op("dest_dir/" + fname, cur.value()["sync_op"], cur.value()["content"]);
                        }
                        else
                        {
                            dropbox::sync_op("dest_dir/" + fname, cur.value()["sync_op"]);
                        }
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    // std::cout << "Failed to parse: " << content << std::endl;
                }
            }
            memset(content, 0, sizeof content);
        } while (content_size > 0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // Wait for filename, filesize and content
    // char name[255];
    // if (s.receive_from(client_dp, name, sizeof name) < 0)
    // {
    //     std::cerr << "Failed to get the name of the file!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "File name: " << name << std::endl;
    // }

    // size_t file_size;
    // if (s.receive_from(client_dp, &file_size, sizeof file_size) < 0)
    // {
    //     std::cerr << "Failed to get the size of file!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "File size: " << file_size << std::endl;

    // }

    // char buffer[1024];
    // if (s.receive_from(client_dp, &buffer, sizeof buffer) < 0)
    // {
    //     std::cerr << "Failed to get the file content!" << std::endl;
    // }
    // else
    // {
    //     std::cout << buffer << std::endl;
    // }

    // std::string filename = "dummy_file.txt";
    // s.send_to(client_dp, filename.c_str(), filename.length());

    return 0;
}