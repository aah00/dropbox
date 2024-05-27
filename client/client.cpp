#include <fstream>
#include <iostream>
#include <fileop.h>
#include <clientSocket.h>

using namespace std;
using json = nlohmann::json;
using namespace dropbox;


bool synchronizeDir(ClientSocket &c, json target_state)
{
    std::cout << "******************************************" << std::endl;
    try
    {
        for (const auto& item : target_state.items())
        {
            json updated_obj;
            json property = item.value();
            if (property["is_dir"] == false)
            {
                fstream file;
                file.open(property["full_path"], ios::in | ios::out | ios::binary);
                if (!file.is_open())
                {
                    // cout<<"[ERROR] : Cannot read file: " << fname << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Read file-content and send it to server
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                property["content"] = content;
                file.close();
            }
            updated_obj[item.key()] = property;
            std::string data_sent = updated_obj.dump();
            c.send_to(data_sent.c_str(), data_sent.length());
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

// bool synchronizeDir(ClientSocket &c, json target_state)
// {
//     try
//     {
//         for (const auto& item : target_state.items())
//         {
//             json updated_obj;
//             std::string state = item.value().dump();
//             std::cout << "STATE: " << state << std::endl;

//             auto j = json::parse(item.value().dump());
//             updated_obj["name"] = j["name"];
//             updated_obj["is_dir"] = j["is_dir"];
//             if (j["is_dir"] == false)
//             {
//                 fstream file;
//                 file.open(j["full_path"], ios::in | ios::out | ios::binary);
//                 if (!file.is_open())
//                 {
//                     // cout<<"[ERROR] : Cannot read file: " << fname << std::endl;
//                     exit(EXIT_FAILURE);
//                 }

//                 // Read file-content and send it to server
//                 std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//                 updated_obj["content"] = content;
//                 file.close();
//                 std::cout << "Updated object = " << updated_obj.dump() << std::endl;
//             }
//             std::string data_sent = updated_obj.dump();
//             c.send_to(data_sent.c_str(), data_sent.length());
//         }
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//         return false;
//     }
//     return true;
// }

json monitorDiff(std::string dir_name, const json &cur_state)
{
    std::string file_name = dir_name + "/status.json";
    try
    {
        if (dropbox::pathExists(file_name))
        {
            std::ifstream f(file_name);
            json last_state = json::parse(f);
            for (const auto& cur : cur_state.items())
            {
                auto it = last_state.find(cur.key());
                if (it != last_state.end())
                {
                    std::cout << "Skipping " << cur.key() << " to be transferred" << std::endl;
                    last_state.erase(it);
                }
                else
                {
                    std::cout << "Adding : " << cur.key() << std::endl;
                    last_state[cur.key()] = cur.value();
                }
            }

            std::cout << "-------------------- Monitored Differences ----------------------" << std::endl;
            std::cout << last_state.dump() << std::endl;
            std::cout << "--------------------------------------------------------" << std::endl;

            return last_state;
        }
        else
        {
            return cur_state;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return cur_state;
    }
    
    // for (const auto& cur : cur_state.items())
    // {
    //     update_list[cur.value()["name"]] = operation::ADD;
    // }
    // try
    // {
    //     if (dropbox::pathExists(file_name))
    //     {
    //         std::ifstream f(file_name);
    //         try
    //         {
    //             json last_state = json::parse(f);
    //             // // Compare and find the latest updates
    //             // std::cout << "========================================" << std::endl;
    //             // std::cout << last_state.dump() << std::endl;
    //             // std::cout << "----------------------------------------" << std::endl;
    //             // std::cout << cur_state << std::endl << std::endl;


    //             for (const auto& cur : cur_state.items())
    //             {
    //                 auto i = json::parse(cur.value().dump());
    //                 bool is_found = false;
    //                 for (const auto& last : last_state.items())
    //                 {
    //                     auto j = json::parse(last.value().dump());
    //                     if (i["name"] == j["name"])
    //                     {
    //                         // Also check the size is same, otherwise
    //                         // consider the file is modified.
    //                         if (i["size"] == j["size"])
    //                         {
    //                             is_found = true;
    //                         }
    //                         break;
    //                     }
    //                 }
    //                 if (is_found)
    //                 {
    //                     std::cout << "Match found for file " << i["name"] << std::endl;
    //                 }
    //                 else
    //                 {
    //                     // Add it to changed dir
    //                 }
    //                 std::cout << std::endl;
    //             }
    //         }
    //         catch(const std::exception& e)
    //         {
    //             std::cerr << e.what() << std::endl;
    //         }
    //         f.close();
    //     }
    //     else
    //     {
    //         return cur_state;
    //     }
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << std::endl;
    // }
}


// json monitorDiff(std::string dir_name, const json &cur_state)
// {
//     std::string file_name = dir_name + "/status.json";

//     try
//     {
//         if (dropbox::pathExists(file_name))
//         {
//             std::ifstream f(file_name);
//             try
//             {
//                 json last_state = json::parse(f);
//                 // // Compare and find the latest updates
//                 // std::cout << "========================================" << std::endl;
//                 // std::cout << last_state.dump() << std::endl;
//                 // std::cout << "----------------------------------------" << std::endl;
//                 // std::cout << cur_state << std::endl << std::endl;


//                 for (const auto& cur : cur_state.items())
//                 {
//                     auto i = json::parse(cur.value().dump());
//                     bool is_found = false;
//                     for (const auto& last : last_state.items())
//                     {
//                         auto j = json::parse(last.value().dump());
//                         if (i["name"] == j["name"])
//                         {
//                             // Also check the size is same, otherwise
//                             // consider the file is modified.
//                             if (i["size"] == j["size"])
//                             {
//                                 is_found = true;
//                             }
//                             break;
//                         }
//                     }
//                     if (is_found)
//                     {
//                         std::cout << "Match found for file " << i["name"] << std::endl;
//                     }
//                     else
//                     {
//                         // Add it to changed dir
//                     }
//                     std::cout << std::endl;
//                 }
//             }
//             catch(const std::exception& e)
//             {
//                 std::cerr << e.what() << std::endl;
//             }
//             f.close();
//         }
//         else
//         {
//             return cur_state;
//         }
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }

int main(int argc, char *argv[])
{
    // Check if directory name is provided
    if (argc < 2)
    {
        std::cout << "Please provide directory name as argument." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Get the directory name
    std::string dir_name = argv[1];

    // Check if directory exists
    if (!dropbox::pathExists(dir_name))
    {
        std::cout << "File does not exists!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to server
    ClientSocket sock_client;
    sock_client.create_connection();

    // Identify the changes in the directory and
    // Synchronize with the remove server
    json file_list = dropbox::scanDir2(dir_name);

    // auto it = file_list.find("abc.txt");
    // auto it = file_list.find("abc.txt");
    // if (it != file_list.end())
    // {
    //     file_list.erase(it);
    // }
    // std::cout << "After erase:" << file_list.dump() << std::endl;
    // std::cout << it.key() << std::endl;
    
    auto changes = monitorDiff(dir_name, file_list);
    if (synchronizeDir(sock_client, changes))
    {
        std::string file_name = dir_name + "/status.json";
        dropbox::write(file_name, file_list);
    }

    // Disconnect from the server
    sock_client.closeSocket();

    return 0;
}

// int main()
// {
//     // create JSON values
//     json j_boolean = true;
//     json j_number_integer = 17;
//     json j_number_float = 23.42;
//     json j_object = {{"one", 1}, {"two", 2}};
//     json j_array = {1, 2, 4, 8, 16};
//     json j_string = "Hello, world";

//     json obj;

//     // // json o1;
//     // // json o2;
//     // json o1;
//     // o1["a"] = {1, 3, 80};
//     // json o2;
//     // o2["b"] = { 1, 3, 80};
//     // json o3;
//     // o3["c"] = {1, 3, 80};

//     // json::array o_arr;
//     // o_arr.emplace_back(o1);
//     // o_arr.emplace_back(o2);
//     // o_arr.emplace(o3);


//     json obj_file;


//     // json o1 = {"name", 123};
//     json o1 = {"size", 123};
//     json o2 = {"path", "c"};
//     json o;
//     o.emplace(o1);
//     o.emplace(o2);
    
//     obj_file["abc"] = o;

//     std::cout << o.dump() << std::endl;
//     std::cout << obj_file.dump() << std::endl;

//     auto it = obj_file.find("abc");
//     std::cout << it.key() << std::endl;


//     // obj_file["abc.txt"]

//     // std::cout << o1.dump() << std::endl;
//     // std::cout << "Contains a:" << o_arr.contains("c") << std::endl;
//     // std::cout << "Contains b:" << o_arr.contains("b") << std::endl;

//     // o_arr.erase(o_arr.begin()+o_arr.find("b"));

//     // std::cout << o_arr.dump() << std::endl;
//     // o2.insert(o2);


//     // // call erase()
//     // j_boolean.erase(j_boolean.begin());
//     // j_number_integer.erase(j_number_integer.begin());
//     // j_number_float.erase(j_number_float.begin());
//     // j_object.erase(j_object.find("two"));
//     // j_array.erase(j_array.begin() + 2);
//     // j_string.erase(j_string.begin());

//     // j_object.emplace(json{"three", 3});

//     // // print values
//     // std::cout << j_boolean << '\n';
//     // std::cout << j_number_integer << '\n';
//     // std::cout << j_number_float << '\n';
//     // std::cout << j_object << '\n';
//     // std::cout << j_array << '\n';
//     // std::cout << j_string << '\n';

// }

