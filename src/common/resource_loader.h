#ifndef _RESOURCE_LOADER_H
#define _RESOURCE_LOADER_H

#include "config.h"

#include "resource_types.h"

namespace ORC_NAMESPACE
{

        class ResourceLoader final
        {

        public:

                static MeshData LoadOBJ(const string& path);
                static ImageData LoadPNG(const string& path);

        private:

        };

};

#endif // _RESOURCE_LOADER_H
