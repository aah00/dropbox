
#include <deque>
#include <string>
#include <time.h>

namespace dropbox
{
    struct TFileInfo
    {
        std::string     name,wholePath;
        time_t          accessTime,modTime;
        bool            isDir, isSymLink;
    };
    typedef std::deque<TFileInfo> TDirListing;

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

    bool scanDir(std::string dir_name, TDirListing &d_tree);

} // End of namespace dropbox