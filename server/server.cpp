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

// #include <regex>

#include <fileop.h>

using namespace std;
using json = nlohmann::json;

class Server_socket{
    fstream file;

    int PORT;

    int general_socket_descriptor;
    int new_socket_descriptor;

    struct sockaddr_in address;
    int address_length;

    struct TFileInfo
    {
        std::string     name,wholePath;
        time_t          accessTime,modTime;
        bool            isDir, isSymLink;
    };

    typedef deque<TFileInfo> TDirListing;

    public:
        Server_socket()
        {
            create_socket();
            PORT = 8050;

            address.sin_family = AF_INET; 
            address.sin_addr.s_addr = INADDR_ANY; 
            address.sin_port = htons( PORT );
            address_length = sizeof(address);

            bind_socket();
            set_listen_set();
            accept_connection();

            // file.open("/home/kratos/tmp/dropbox/server/server_text.txt", ios::in | ios::binary);
            file.open("/home/kratos/tmp/dropbox/server/server_text.txt", ios::out | ios::trunc | ios::binary);
            if(file.is_open()){
                cout<<"[LOG] : File is ready to Transmit.\n";
            }
            else{
                cout<<"[ERROR] : File loading failed, Exititng.\n";
                exit(EXIT_FAILURE);
            }
        }

        void create_socket()
        {
            if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
                perror("[ERROR] : Socket failed");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Socket Created Successfully.\n";
        }

        void bind_socket()
        {
            if (bind(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address))<0) {
                perror("[ERROR] : Bind failed");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Bind Successful.\n";
        }

        void set_listen_set()
        {
            if (listen(general_socket_descriptor, 3) < 0) {
                perror("[ERROR] : Listen");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Socket in Listen State (Max Connection Queue: 3)\n";
        }

        void accept_connection()
        {
            if ((new_socket_descriptor = accept(general_socket_descriptor, (struct sockaddr *)&address, (socklen_t*)&address_length))<0) { 
                perror("[ERROR] : Accept");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Connected to Client.\n";
        }

        void receive_file()
        {
            char filename[255];
            char buffer[1024];
            char json_stream[1024];
            size_t file_size = 218;

            int valread;

            valread = read(new_socket_descriptor, filename, sizeof filename);
            cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
            cout<<"[LOG] : Received filename \""<< filename << std::endl;
                
            valread = read(new_socket_descriptor, &file_size, sizeof file_size);
            cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
            cout<<"[LOG] : Size of file: "<< (int) file_size <<" bytes\n";

            valread = read(new_socket_descriptor, buffer, sizeof buffer);
            cout<<"\n[LOG] : Received json obj size: "<< valread << std::endl;
            cout<<"[LOG] : Saving data to file.\n";
            file << buffer;
            cout<<"[LOG] : File Saved.\n";

            valread = read(new_socket_descriptor, json_stream, sizeof json_stream);
            cout<<" \n[LOG] : Received json obj size: "<< valread << std::endl;
            json_stream[valread]= '\0';
            std::string tmp_str(json_stream);
            cout<<"[LOG] : Received stream string: "<< json_stream << std::endl;

            dropbox::TDirListing d_tree;
            dropbox::scanDir("./test_dir", d_tree);
            for (const auto &item: d_tree)
            {
                std::cout << item.name << std::endl;
                std::cout << item.accessTime << std::endl;
                std::cout << item.modTime << std::endl;
                std::cout << std::endl;
            }

            memset(buffer, 1024, 0);
            memset(filename, 255, 0);
            memset(json_stream, 1024, 0);
        }

        void synchronizeDir(std::string flolder_name)
        {
        }
};

int main()
{
    Server_socket S;
    S.receive_file();
    return 0;
}