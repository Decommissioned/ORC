#ifndef _RESOURCE_TYPES_H
#define _RESOURCE_TYPES_H

#include "config.h"

namespace ORC_NAMESPACE
{

        // Minimum information required to create VBOs using the Mesh class
        struct MeshData
        {
                vector<uint32> indices;
                vector<float> positions;
                vector<float> normals;
                vector<float> uvs;
                string nameID;
        };

        // Raw image data
        struct ImageData final
        {
                vector<uint8> data;
                uint32 width, height;
                string nameID;
        };

};

#endif // _RESOURCE_TYPES_H
