#ifndef _SHADER_SLOTS_H
#define _SHADER_SLOTS_H

#include "config.h"

namespace ORC_NAMESPACE
{
        /*
        Slots layout for attribute arrays in VAOs and shader programs
        */
        enum : uint8
        {
                VERTEX_POSITION = 0,
                VERTEX_UV,
                VERTEX_NORMAL
        };

};

#endif // _SHADER_SLOTS_H
