#ifndef _CONFIG_H
#define _CONFIG_H

#include "scope_guard.h"

#include <cstdint>
using ErrorCode = uint16_t;

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

#endif // _CONFIG_H
