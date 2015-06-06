#ifndef _CONFIG_H
#define _CONFIG_H

#include "scope_guard.h"

#include <cstdint>
using ErrorCode = uint16_t;

#include <string>
#include <vector>

#define ORC_NAMESPACE orc
#define ORC_GLSL_VERSION "140"

namespace ORC_NAMESPACE
{
        using std::string;
        using std::vector;
        using std::size_t;

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
#define ORC_SHADER_FOLDER_RELATIVE_PATH "../shader/"
#define ORC_MESH_FOLDER_RELATIVE_PATH "../mesh/"
#define ORC_TEXTURE_FOLDER_RELATIVE_PATH "../texture/"
#else
#error Unsupported platform
#endif

#endif // _CONFIG_H
