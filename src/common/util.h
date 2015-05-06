#ifndef _UTIL_H
#define _UTIL_H

#include "config.h"

namespace ORC_NAMESPACE
{
        /*
        Util
        Convenience methods that don't belong anywhere else
        */
        namespace util
        {

                size_t FileSize(const string& path);

                string ReadFileText(const string& path);
                vector<uint8> ReadFileBinary(const string& path);

        };
};

#endif // _UTIL_H
