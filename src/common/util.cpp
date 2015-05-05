#include "util.h"

#include <fstream>
#include <iterator>

#include <sys/stat.h>
#include "error_codes.h"

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
        };
};