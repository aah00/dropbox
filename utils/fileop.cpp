
#include <fileop.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>

#define _access access
#define _rmdir rmdir
#define MAX_PATH_LENGTH 4096

using json = nlohmann::json;


json dropbox::scanDir2(std::string dir_name)
{
    DIR *dirp;
    if ((dirp = opendir(dir_name.c_str())) == NULL)
    {
        std::cerr << "couldn't open " << dir_name << std::endl;
        return false;
    }

    json obj;
    struct dirent *dp;
    while((dp = readdir(dirp)) != nullptr)
    {
        std::string d_name = dp->d_name;
        if(d_name != "." && d_name != "..")
        {
            json property;
            struct stat statDat;
            char full_path[MAX_PATH_LENGTH];
            sprintf(full_path,"%s/%s/%s", get_current_dir_name(), dir_name.c_str(), d_name.c_str());
            if (stat(full_path, &statDat) != 0)
            {
                closedir(dirp);
                return false;
            }
            property["full_path"] = full_path;
            property["mod_time"] = statDat.st_mtime;
            property["access_time"] = statDat.st_atime;
            property["size"] = statDat.st_size;
            property["is_dir"] = (0 != (statDat.st_mode & 0x4000));
            property["is_symlink"] = false;
            property["sync_op"] = FileOP::ADD;
            obj[d_name] = property;
        }
    }
    closedir(dirp);

    return obj;
}

// ---------------------------------------------------------------
//  isDirectory
// ---------------------------------------------------------------
bool isDirectory( const std::string &fileName)
{
    struct stat statDat;
    if (stat(fileName.c_str(), &statDat))
    {
        // exitProgram(-1);
        return false;
    }

    return (0 !=(statDat.st_mode & 0x4000));
}

// ---------------------------------------------------------------
//  deleteFileOrDir - Deletes a file/empty-dir
//  Returns true on success, false on error.
// ---------------------------------------------------------------
bool deleteFileOrDir(const std::string &filename)
{
    try
    {
        if (isDirectory( filename ))
        {
            _rmdir(filename.c_str());
        }
        else
        {
            remove(filename.c_str());
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}


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
                newEntry.modification_time = statDat.st_mtime;
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

// ---------------------------------------------------------------------
// Creates a new directory (does not fail if if already exists) 
//  In linux, the directory is created with RWX permisions for everyone 
//  Returns true on success, false on error.
// ---------------------------------------------------------------------
bool dropbox::createDirectory(const std::string &path)
{
    if (dropbox::pathExists(path))
    {
        return true;
    }
    return (0 == mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO));
}


bool dropbox::write(std::string file_name, std::string data)
{
    std::ofstream file;
    try
    {
        file.open(file_name);
        file << data;
        file.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    return true;
}

bool dropbox::write(std::string file_name, const json& obj)
{
    std::ofstream file;
    try
    {
        file.open(file_name);
        file << obj.dump();
        file.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    return true;
}

bool dropbox::sync_op(std::string file, FileOP op, std::string content)
{
    switch (op)
    {
        case FileOP::ADD:
        {
            std::cout << "Creating a file or directory!" << std::endl;
            return dropbox::write(file, content);
        }
        case FileOP::REMOVE:
        {
            std::cout << "Removing a file or directory" << std::endl;
            return deleteFileOrDir(file);
        }
        default:
        {
            std::cout << "\n\nTHIS WILL NOT OCCUR!" << std::endl;
        }
    }
    return false;
}

