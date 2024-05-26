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


#include <clientSocket.h>

class Client_socket
{
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
        // Client_socket(){
        //     create_socket();
        //     PORT = 8050;

        //     address.sin_family = AF_INET;
        //     address.sin_port = htons( PORT );
        //     address_length = sizeof(address);
        //     if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) {
        //         cout<<"[ERROR] : Invalid address\n";
        //     }

        //     create_connection();

        //     // file.open("/home/kratos/tmp/dropbox/client/client_text.txt", ios::out | ios::trunc | ios::binary);
        //     if(dropbox::pathExists("/home/kratos/tmp/dropbox/client/client_text.txt"))
        //     {
        //         dropbox::isSameFile("/home/kratos/tmp/dropbox/client/client_text.txt");
        //         file.open("/home/kratos/tmp/dropbox/client/client_text.txt", ios::in | ios::binary);

        //         if(file.is_open()){
        //             cout<<"[LOG] : File Creted.\n";
        //         }
        //         else{
        //             cout<<"[ERROR] : File loading failed, Exititng.\n";
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        // }

        // void create_socket(){
        //     if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        //         perror("[ERROR] : Socket failed.\n");
        //         exit(EXIT_FAILURE);
        //     }

        //     int yes = 1;
        //     int result = setsockopt(general_socket_descriptor,
        //                             IPPROTO_TCP,
        //                             TCP_NODELAY,
        //                             (char *) &yes, 
        //                             sizeof(int));    // 1 - on, 0 - off
        //     if (result < 0)
        //     {
        //         std::cout << "Error in setting SOCKET OPTION!!" << std::endl;
        //     }

        //     cout<<"[LOG] : Socket Created Successfully.\n";
        // }

        // void create_connection(){
        //     if (connect(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) { 
        //         perror("[ERROR] : connection attempt failed.\n");
        //         exit(EXIT_FAILURE);
        //     }
        //     cout<<"[LOG] : Connection Successfull." << std::endl;
        // }

        // void transmit_file(){

        //     std::string filename = "dummy_file.txt";
        //     int bytes_sent = send(general_socket_descriptor, filename.c_str(), filename.length(), 0);
        //     cout<<"\n[LOG] : Transmitted Data Size "<< bytes_sent <<" Bytes." << std::endl;

        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     size_t file_size = 288; // st.st_size;
        //     bytes_sent = send(general_socket_descriptor, &file_size, sizeof file_size, 0);
        //     cout<<"\n[LOG] : Transmitted Data Size "<< bytes_sent <<" Bytes." << std::endl;

        //     std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        //     // cout<<"[LOG] : Expected Transmission file size "<< contents.length() <<" Bytes"  << std::endl;
        //     cout<<"[LOG] : Sending...\n";

        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     bytes_sent = send(general_socket_descriptor, contents.c_str(), contents.length(), 0);
        //     cout<<"\n[LOG] : Transmitted file Size "<< bytes_sent <<" Bytes."  << std::endl;
        //     cout<<"[LOG] : File Transfer Complete." << std::endl;
        // }

        // void transmit_json(std::string data_stream)
        // {
        //     // std::string data_stream = obj.dump();
        //     // int bytes_sent = send(general_socket_descriptor, data_stream.c_str(), data_stream.length(), 0);
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //     int bytes_sent = send(general_socket_descriptor, data_stream.c_str(), data_stream.length(), 0);
        //     cout<<"\n\n [LOG] : Transmitted json obj (size: "<< bytes_sent << "): " << data_stream << std::endl;
        // }
};

void synchronizeDir(std::string flolder_name)
{
    // dropbox::scanDir(flolder_name, d_tree);

    // auto server_dir = json::array();
    // for (const auto &item: d_tree)
    // {
    //     json obj;
    //     obj["name"] = item.name;
    //     obj["accessTime"] = item.access_time;
    //     obj["modTime"] = item.modification_time;
    //     server_dir.push_back(obj);
    //     std::cout << item.name << std::endl;
    //     std::cout << item.access_time << std::endl;
    //     std::cout << item.modification_time << std::endl;
    //     std::cout << std::endl;
    // }

    // std::cout << "----- JSON DUMP -----" <<  std::endl;
    // std::cout << server_dir.dump();
}


int main(int argc, char *argv[])
{
    std::string dir_name = "src_dir";
    if (argc > 1)
    {
        dir_name = argv[1];
    }

    // json msg;
    // msg["filename"] = "readme.md";
    // msg["filesize"] = "200";
    // // std::string data_stream = msg.dump(-1, ' ', true);
    // // string out_stream = std::regex_replace(data_stream, std::regex(R"(")"), R"(\)");
    // // char temp_buff[200];
    // // msg.json_dumps(json, JSON_PRESERVE_ORDER);
    // // strcpy(temp_buff, json_object_to_json_string(msg));
    // // json_object_to_json_string
    // Client_socket C;
    // C.transmit_file();
    // // C.transmit_json(out_stream);
    // C.transmit_json(msg.dump());

    ClientSocket c;
    c.create_connection();

    fstream file;
    if(dropbox::pathExists(dir_name))
    {
        dropbox::ListFileStat d_tree;
        dropbox::scanDir(dir_name);

        json current_state = dropbox::scanDir(dir_name);

        // std::string state = current_state.dump();
        // c.send_to(state.c_str(), state.length());

        for (const auto& item : current_state.items())
        {
            std::cout << "Key: " << item.key() << "\n";

            std::string state = item.value().dump();
            std::cout << " ------: " << state << std::endl;
            c.send_to(state.c_str(), state.length());

            // for (const auto& val : item.value().items())
            // {
            //     std::cout << "  " << val.key() << ": " << val.value() << "\n";
            // }
        }

        // for (const auto& item : current_state.items())
        // {
        //     std::cout << "Key: " << item.key() << "\n";
        //     for (const auto& val : item.value().items())
        //     {
        //         std::cout << "  " << val.key() << ": " << val.value() << "\n";
        //     }
        // }
        // auto prev_state = json::array();
        // json obj;
        // prev_state.push_back(obj);
        // std::cout << "\nOld state: " << prev_state.dump();

        // json patch = json::diff(current_state, prev_state);
        // std::cout << "\nPatch state: " << patch.dump();

        // synchronizeDir(dir_name);

        // std::string filename = "client.txt";
        // c.send_to(filename.c_str(), filename.length());

        // size_t file_size = 288;
        // c.send_to(&file_size, sizeof file_size);

        // file.open("/home/kratos/tmp/dropbox/build/client/src_dir/client.txt", ios::in | ios::out | ios::binary);
        // if(file.is_open())
        // {
        //     cout<<"[LOG] : File Created.\n";
        // }
        // else
        // {
        //     cout<<"[ERROR] : File loading failed, Exititng.\n";
        //     exit(EXIT_FAILURE);
        // }
        // std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // if (c.send_to(contents.c_str(), contents.length()) > 0)
        // {
        //     std::cout << "Sent content:" << contents << std::endl;
        // }
    }
    else
    {
        std::cout << "Folder not found!" << std::endl;
    }

    // auto recv_bytes = c.receive_from(filename, sizeof filename);
    // if (recv_bytes > 0)
    // {
    //     cout<<"[LOG] : File name: "<< filename << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Failed to read file size" << std::endl;
    // }
    return 0;
}

