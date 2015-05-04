#ifndef _CONFIG_H
#define _CONFIG_H

#include "scope_guard.h"

#include <cstdint>
using ErrorCode = uint16_t;

#include <string>
#include <vector>
using std::string;
using std::vector;

#define ORC_NAMESPACE orc

namespace ORC_NAMESPACE
{
        using int8 = int8_t;
        using int16 = int16_t;
        using int32 = int32_t;
        using int64 = int64_t;
        using uint8 = uint8_t;
        using uint16 = uint16_t;
        using uint32 = uint32_t;
        using uint64 = uint64_t;

        using ErrorCode = uint16_t;
};

#if _MSC_VER
#define THREAD_LOCAL_STORAGE __declspec(thread)
#define ALIGN_AS(size) __declspec(align(size))
#else
#error Unsupported platform
#endif

#endif // _CONFIG_H
