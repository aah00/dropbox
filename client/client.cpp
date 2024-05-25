#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fileop.h>

#include <json.hpp>
#include <netinet/tcp.h>
#include <chrono>
#include <thread>

#include <sys/stat.h>
#include <dirent.h>
#include <deque>

using namespace std;
using json = nlohmann::json;
using namespace dropbox;

class Client_socket{
    fstream file;

    int PORT;
    
    int general_socket_descriptor;

    struct sockaddr_in address;
    int address_length;

    // struct TFileInfo
    // {
    //     std::string     name,wholePath;
    //     time_t          accessTime,modTime;
    //     bool            isDir, isSymLink;
    // };

    // typedef deque<TFileInfo> TDirListing;

    public:
        Client_socket(){
            create_socket();
            PORT = 8050;

            address.sin_family = AF_INET;
            address.sin_port = htons( PORT );
            address_length = sizeof(address);
            if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) {
                cout<<"[ERROR] : Invalid address\n";
            }

            create_connection();

            // file.open("/home/kratos/tmp/dropbox/client/client_text.txt", ios::out | ios::trunc | ios::binary);
            if(dropbox::pathExists("/home/kratos/tmp/dropbox/client/client_text.txt"))
            {
                dropbox::isSameFile("/home/kratos/tmp/dropbox/client/client_text.txt");
                file.open("/home/kratos/tmp/dropbox/client/client_text.txt", ios::in | ios::binary);

                if(file.is_open()){
                    cout<<"[LOG] : File Creted.\n";
                }
                else{
                    cout<<"[ERROR] : File loading failed, Exititng.\n";
                    exit(EXIT_FAILURE);
                }
            }
        }

        void create_socket(){
            if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
                perror("[ERROR] : Socket failed.\n");
                exit(EXIT_FAILURE);
            }

            int yes = 1;
            int result = setsockopt(general_socket_descriptor,
                                    IPPROTO_TCP,
                                    TCP_NODELAY,
                                    (char *) &yes, 
                                    sizeof(int));    // 1 - on, 0 - off
            if (result < 0)
            {
                std::cout << "Error in setting SOCKET OPTION!!" << std::endl;
            }

            cout<<"[LOG] : Socket Created Successfully.\n";
        }

        void create_connection(){
            if (connect(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) { 
                perror("[ERROR] : connection attempt failed.\n");
                exit(EXIT_FAILURE);
            }
            cout<<"[LOG] : Connection Successfull." << std::endl;
        }

        void transmit_file(){

            std::string filename = "dummy_file.txt";
            int bytes_sent = send(general_socket_descriptor, filename.c_str(), filename.length(), 0);
            cout<<"\n[LOG] : Transmitted Data Size "<< bytes_sent <<" Bytes." << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            size_t file_size = 288; // st.st_size;
            bytes_sent = send(general_socket_descriptor, &file_size, sizeof file_size, 0);
            cout<<"\n[LOG] : Transmitted Data Size "<< bytes_sent <<" Bytes." << std::endl;

            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            // cout<<"[LOG] : Expected Transmission file size "<< contents.length() <<" Bytes"  << std::endl;
            cout<<"[LOG] : Sending...\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            bytes_sent = send(general_socket_descriptor, contents.c_str(), contents.length(), 0);
            cout<<"\n[LOG] : Transmitted file Size "<< bytes_sent <<" Bytes."  << std::endl;
            cout<<"[LOG] : File Transfer Complete." << std::endl;
        }

        void transmit_json(std::string data_stream)
        {
            // std::string data_stream = obj.dump();
            // int bytes_sent = send(general_socket_descriptor, data_stream.c_str(), data_stream.length(), 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            int bytes_sent = send(general_socket_descriptor, data_stream.c_str(), data_stream.length(), 0);
            cout<<"\n\n [LOG] : Transmitted json obj (size: "<< bytes_sent << "): " << data_stream << std::endl;
        }
};

int main(){
    json msg;
    msg["filename"] = "readme.md";
    msg["filesize"] = "200";
    // std::string data_stream = msg.dump(-1, ' ', true);
    // string out_stream = std::regex_replace(data_stream, std::regex(R"(")"), R"(\)");
    // char temp_buff[200];
    // msg.json_dumps(json, JSON_PRESERVE_ORDER);
    // strcpy(temp_buff, json_object_to_json_string(msg));
    // json_object_to_json_string
    Client_socket C;
    C.transmit_file();
    // C.transmit_json(out_stream);
    C.transmit_json(msg.dump());

    return 0;
}