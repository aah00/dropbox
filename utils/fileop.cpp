
#include <fileop.h>
#include <utime.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <cerrno>
#include <sys/stat.h>
#include <dirent.h>


#define _access access
#define _rmdir rmdir
#define MAX_PATH_LENGTH 4096

using json = nlohmann::json;

json dropbox::scanDir(std::string dir_name)
{
    DIR *dirp;
    if ((dirp = opendir(dir_name.c_str())) == NULL) {
        std::cerr << "couldn't open " << dir_name << std::endl;
        return false;
    }

    struct dirent *dp;
    auto server_dir = json::array();
    do {
        while((dp = readdir(dirp)) != nullptr)
        {
            std::cout << "\nReading new file" << std::endl;
            json obj;
            std::string d_name = dp->d_name;
            if(d_name != "." && d_name != "..")
            {
                obj["name"] = d_name;
                struct stat statDat;
                char full_path[MAX_PATH_LENGTH];
                sprintf(full_path,"%s/%s/%s", get_current_dir_name(), dir_name.c_str(), d_name.c_str());
                if (stat(full_path, &statDat) != 0)
                {
                    closedir(dirp);
                    return false;
                }
                obj["full_path"] = full_path;
                obj["mod_time"] = statDat.st_mtime;
                obj["access_time"] = statDat.st_atime;
                obj["size"] = statDat.st_size;
                obj["is_dir"] = (0 != (statDat.st_mode & 0x4000));
                obj["is_symlink"] = false;
                server_dir.push_back(obj);
            }
        }
    } while (dp != nullptr);

    closedir(dirp);
    return server_dir;
}

bool dropbox::scanDir(std::string dir_name, ListFileStat &d_tree)
{
    DIR *dirp;
    struct dirent *dp;
    FileStat newEntry;

    if ((dirp = opendir(dir_name.c_str())) == NULL) {
        std::cerr << "couldn't open " << dir_name << std::endl;
        return false;
    }

    do {
        errno = 0;
        while((dp = readdir(dirp)) != nullptr)
        {
            std::string d_name = dp->d_name;
            newEntry.name = d_name;
            if(d_name != "." && d_name != "..")
            {
                struct stat statDat;
                char auxStr[MAX_PATH_LENGTH];
                sprintf(auxStr,"%s/%s/%s", get_current_dir_name(), dir_name.c_str(), d_name.c_str());
                if (stat(auxStr, &statDat) != 0)
                {
                    closedir(dirp);
                    return false;
                }
                newEntry.full_path = auxStr;
                newEntry.modification_time    = statDat.st_mtime;
                newEntry.access_time = statDat.st_atime;
                newEntry.size = statDat.st_size;
                newEntry.is_dir = (0 != (statDat.st_mode & 0x4000));
                newEntry.is_symlink = false;
                d_tree.push_back(newEntry);
            }
        }
    } while (dp != nullptr);

    closedir(dirp);
    return true;
}

// ---------------------------------------------------------------
// Returns true if path exists:
// ---------------------------------------------------------------
bool dropbox::pathExists(const std::string &path)
{
    return 0 == _access(path.c_str(), 0x00); // 0x00 = Check for existence only!
}

// bool dropbox::isSameFile(const std::string &f1, const std::string &f2)
bool dropbox::isSameFile(const std::string &f1)
{
    struct stat fileInfo1, fileInfo2;

    if (stat(f1.c_str(), &fileInfo1) != 0)
    {   // Use stat() to get the info
        std::cerr << "Error: file1 info error!!" << std::endl;
        return false;
    }

    std::cout << "Type:         : ";
    if ((fileInfo1.st_mode & S_IFMT) == S_IFDIR) { // From sys/types.h
        std::cout << "Directory\n";
    } else {
        std::cout << "File\n";
    }

    // std::cout << "Size          : " <<
    //     fileInfo1.st_size << '\n';               // Size in bytes
    // std::cout << "Device        : " <<
    //   (char)(fileInfo1.st_dev + 'A') << '\n';  // Device number
    // std::cout << "Created       : " <<
    //     std::ctime(&fileInfo1.st_ctime);         // Creation time
    // std::cout << "Modified      : " <<
    //     std::ctime(&fileInfo1.st_mtime);         // Last mod time
}