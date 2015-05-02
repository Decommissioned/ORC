#include "util.h"

#include <fstream>
#include <iterator>

#include <sys/stat.h>
#include "error_codes.h"

namespace ORC_NAMESPACE
{
        namespace util
        {

                std::size_t FileSize(const string& path)
                {
                        struct stat st;
                        if (stat(path.c_str(), &st) != 0)
                                throw Error::IO_FILE_NOT_FOUND;

                        return st.st_size;
                }

                vector<uint8> LoadFileToMemory(const string& path)
                {
                        std::size_t length = FileSize(path);
                        std::ifstream file(path, std::ios::binary);
                        vector<uint8> data(length);

                        data.assign(std::istreambuf_iterator<char>(file),
                                    std::istreambuf_iterator<char>());

                        return data;
                }

        };
};