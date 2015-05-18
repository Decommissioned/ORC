#include "util.h"

#include <fstream>
#include <iterator>

#include <sys/stat.h>
#include "error_codes.h"

#include <boost/filesystem.hpp>

namespace ORC_NAMESPACE
{
        namespace util
        {

                size_t FileSize(const string& path)
                {
                        struct stat st;

                        if (stat(path.c_str(), &st) != 0)
                                throw Error::IO_FILE_NOT_FOUND;

                        return st.st_size;
                }

                string ReadFileText(const string& path)
                {
                        size_t length = FileSize(path);
                        std::ifstream file(path);
                        string content(length, 0);

                        file.read(&content[0], length);

                        return content;
                }

                vector<uint8> ReadFileBinary(const string& path)
                {
                        size_t length = FileSize(path);
                        std::ifstream file(path, std::ios::binary);
                        vector<uint8> data(length);

                        data.assign(std::istreambuf_iterator<char>(file),
                                    std::istreambuf_iterator<char>());

                        return data;
                }

                vector<string> ListDirectoryFiles(const string& folder)
                {
                        vector<string> files;

                        boost::filesystem::path path = folder;
                        boost::filesystem::directory_iterator end;

                        for (boost::filesystem::directory_iterator itr(path); itr != end; ++itr)
                        {
                                if (boost::filesystem::is_regular_file(itr->path()))
                                        files.emplace_back(itr->path().string());
                        }

                        return files;
                }

                vector<string> ListDirectoryFiles(const string& folder, const string& extension)
                {
                        vector<string> files;

                        boost::filesystem::path path = folder;
                        boost::filesystem::directory_iterator end;

                        for (boost::filesystem::directory_iterator itr(path); itr != end; ++itr)
                        {
                                if (boost::filesystem::is_regular_file(itr->path()))
                                {
                                        string file = itr->path().string();

                                        if (file.substr(file.find_last_of('.')) == extension)
                                                files.emplace_back(file);
                                }
                        }

                        return files;
                }

                

        };
};