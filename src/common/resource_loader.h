#ifndef _RESOURCE_LOADER_H
#define _RESOURCE_LOADER_H

#include "config.h"

#include "image.h"

namespace ORC_NAMESPACE
{

        class ResourceLoader final
        {

        public:

                static Image LoadImage(const string& path);

        private:

        };

};

#endif // _RESOURCE_LOADER_H
