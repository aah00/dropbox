#include <fileop.h>
#include <iostream>
#include <fstream>
#include <clientSocket.h>

using namespace std;
using json = nlohmann::json;
using namespace dropbox;

void synchronizeDir(ClientSocket &c, json target_state)
{
    try
    {
        for (const auto& item : target_state.items())
        {
            json updated_obj;

            std::string state = item.value().dump();
            std::cout << "STATE: " << state << std::endl;

            auto j = json::parse(item.value().dump());
            updated_obj["name"] = j["name"];
            updated_obj["is_dir"] = j["is_dir"];
            if (j["is_dir"] == false)
            {
                fstream file;
                file.open(j["full_path"], ios::in | ios::out | ios::binary);
                if (!file.is_open())
                {
                    // cout<<"[ERROR] : Cannot read file: " << fname << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Read file-content and send it to server
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                updated_obj["content"] = content;
                file.close();
                std::cout << "Updated object = " << updated_obj.dump() << std::endl;
            }
            std::string data_sent = updated_obj.dump();
            c.send_to(data_sent.c_str(), data_sent.length());
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

json getChangedState(std::string dir_name, const json &current)
{
    std::string file_name = dir_name + "/status.json";
    try
    {
        if (dropbox::pathExists(file_name))
        {
            std::ifstream f(file_name);
            try
            {
                json data = json::parse(f);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            f.close();
        }
        else
        {
            return current;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char *argv[])
{
    // Get the directory name as argument
    std::string dir_name = "src_dir";
    if (argc > 1)
    {
        dir_name = argv[1];
    }

    // Start the server
    ClientSocket c;
    c.create_connection();

    int debug_ctr = 0;
    // Check if directory exists
    if(dropbox::pathExists(dir_name))
    {
        json current_state = dropbox::scanDir(dir_name);
        auto target_state = getChangedState(dir_name, current_state);
        synchronizeDir(c, target_state);
    }
    else
    {
        std::cout << "Folder not found!" << std::endl;
    }

    try
    {
        c.closeSocket();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

