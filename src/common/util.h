#ifndef _UTIL_H
#define _UTIL_H

#include "config.h"

namespace ORC_NAMESPACE
{
        namespace util
        {

                std::size_t FileSize(const string& path);

                vector<uint8> LoadFileToMemory(const string& path);

        };
};

#endif // _UTIL_H
