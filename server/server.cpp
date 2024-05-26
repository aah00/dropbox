#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <deque>
#include <sys/stat.h>
#include <json.hpp>
#include <fileop.h>
#include <string>
#include <serverSocket.h>

using namespace std;
using json = nlohmann::json;

// class Server_socket{
//     fstream file;

//     int PORT;

//     int general_socket_descriptor;
//     int new_socket_descriptor;

//     struct sockaddr_in address;
//     int address_length;

//     public:
//         Server_socket()
//         {
//             create_socket();
//             PORT = 8050;

//             address.sin_family = AF_INET; 
//             address.sin_addr.s_addr = INADDR_ANY; 
//             address.sin_port = htons( PORT );
//             address_length = sizeof(address);

//             bind_socket();
//             set_listen_set();
//             accept_connection();

//             // file.open("/home/kratos/tmp/dropbox/server/server_text.txt", ios::in | ios::binary);
//             file.open("/home/kratos/tmp/dropbox/server/server_text.txt", ios::out | ios::trunc | ios::binary);
//             if(file.is_open()){
//                 cout<<"[LOG] : File is ready to Transmit.\n";
//             }
//             else{
//                 cout<<"[ERROR] : File loading failed, Exititng.\n";
//                 exit(EXIT_FAILURE);
//             }
//         }

//         void create_socket()
//         {
//             if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
//                 perror("[ERROR] : Socket failed");
//                 exit(EXIT_FAILURE);
//             }
//             cout<<"[LOG] : Socket Created Successfully.\n";
//         }

//         void bind_socket()
//         {
//             if (bind(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address))<0) {
//                 perror("[ERROR] : Bind failed");
//                 exit(EXIT_FAILURE);
//             }
//             cout<<"[LOG] : Bind Successful.\n";
//         }

//         void set_listen_set()
//         {
//             if (listen(general_socket_descriptor, 3) < 0) {
//                 perror("[ERROR] : Listen");
//                 exit(EXIT_FAILURE);
//             }
//             cout<<"[LOG] : Socket in Listen State (Max Connection Queue: 3)\n";
//         }

//         void accept_connection()
//         {
//             if ((new_socket_descriptor = accept(general_socket_descriptor, (struct sockaddr *)&address, (socklen_t*)&address_length))<0) { 
//                 perror("[ERROR] : Accept");
//                 exit(EXIT_FAILURE);
//             }
//             cout<<"[LOG] : Connected to Client.\n";
//         }

//         void receive_file()
//         {
//             char filename[255];
//             char buffer[1024];
//             char json_stream[1024];
//             size_t file_size = 218;

//             int valread;

//             valread = read(new_socket_descriptor, filename, sizeof filename);
//             cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
//             cout<<"[LOG] : Received filename \""<< filename << std::endl;
                
//             valread = read(new_socket_descriptor, &file_size, sizeof file_size);
//             cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
//             cout<<"[LOG] : Size of file: "<< (int) file_size <<" bytes\n";

//             valread = read(new_socket_descriptor, buffer, sizeof buffer);
//             cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
//             cout<<"[LOG] : Saving data to file.\n";
//             file << buffer;
//             cout<<"[LOG] : File Saved.\n";

//             valread = read(new_socket_descriptor, json_stream, sizeof json_stream);
//             cout<<" \n[LOG] : Received json obj size: "<< valread << std::endl;
//             json_stream[valread]= '\0';
//             std::string tmp_str(json_stream);
//             cout<<"[LOG] : Received stream string: "<< json_stream << std::endl;

//             memset(buffer, 1024, 0);
//             memset(filename, 255, 0);
//             memset(json_stream, 1024, 0);
//         }

//         void synchronizeDir(std::string flolder_name)
//         {
//             dropbox::TDirListing d_tree;
//             dropbox::scanDir(flolder_name, d_tree);

//             auto server_dir = json::array();

//             for (const auto &item: d_tree)
//             {
//                 json obj;
//                 obj["name"] = item.name;
//                 obj["accessTime"] = item.accessTime;
//                 obj["modTime"] = item.modTime;
//                 server_dir.push_back(obj);
//                 std::cout << item.name << std::endl;
//                 std::cout << item.accessTime << std::endl;
//                 std::cout << item.modTime << std::endl;
//                 std::cout << std::endl;
//             }

//             std::cout << "----- JSON DUMP -----" <<  std::endl;
//             std::cout << server_dir.dump();
//         }
// };

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

    int ctr = 0;

    while (ctr < 2)
    {
        // Wait for filename, filesize and content
        char content[1024];
        ssize_t content_size = s.receive_from(client_dp, content, sizeof content);
        if (content_size < 0)
        {
            std::cerr << "Failed to get the name of the file!" << std::endl;
        }
        else
        {
            // std::cout << "Received content: " << content << std::endl;
            try
            {
                auto j = json::parse(content);
                std::cout << "File name: " << j["name"] << std::endl;
                std::cout << "File size: " << j["size"] << std::endl;
                // for (const auto& val : j.items())
                // {
                //     std::cout << "  " << val.key() << ": " << val.value() << "\n";
                // }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                std::cout << "Failed to parse: " << content << std::endl;
            }
        }
        memset(content, 0, sizeof content);
        ctr++;
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