#ifndef _IMAGE_H
#define _IMAGE_H

#include "config.h"

namespace ORC_NAMESPACE
{

        struct Image final
        {
                vector<uint8> data;
                uint32 width, height;
        };

};

#endif // _IMAGE_H
