
#pragma once

#include <deque>
#include <string>
#include <time.h>
#include <json.hpp>

namespace dropbox
{
    using json = nlohmann::json;

    enum FileOP
    {
        NOOP,
        ADD,
        REMOVE
    };

    struct FileStat
    {
        std::string name;
        std::string full_path;
        time_t access_time;
        time_t modification_time;
        off_t size;
        bool is_dir;
        bool is_symlink;
    };
    typedef std::deque<FileStat> ListFileStat;

    /**
     * Returns true if path exists and we have permissions to read it.
     */
    bool pathExists(const std::string &path);

    /**
     * 
    */
    bool isSameFile(const std::string &f1);
    // bool isSameFile(const std::string &f1, const std::string &f2);

    /**
     * 
    */
    int sendTo(std::string data);

    /**
     * 
    */
    bool receiveFrom(std::string data);

    bool scanDir(std::string dir_name, ListFileStat &d_tree);

    json scanDir(std::string dir_name);

    bool createDirectory(const std::string &path);

    bool write(std::string file_name, std::string data);

    bool write(std::string file_name, const json& obj);
    
    json scanDir2(std::string dir_name);

    bool sync_op(std::string file, FileOP op, std::string content = "");


} // End of namespace dropbox