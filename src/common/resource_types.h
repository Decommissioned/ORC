#ifndef _RESOURCE_TYPES_H
#define _RESOURCE_TYPES_H

#include "config.h"

namespace ORC_NAMESPACE
{

        struct MeshData
        {
                vector<uint32> indices;
                vector<float> positions;
                vector<float> normals;
                vector<float> uvs;
        };

        struct ImageData final
        {
                vector<uint8> data;
                uint32 width, height;
        };

};

#endif // _RESOURCE_TYPES_H
