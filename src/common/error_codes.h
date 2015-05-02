#ifndef _CODE_TABLE_H
#define _CODE_TABLE_H

#include "config.h"

namespace ORC_NAMESPACE
{

        enum class Error : ErrorCode
        {
                OK,

                SDL_VIDEO_INITIALIZATION,
                SDL_WINDOW_CREATION,

                OPENGL_INITIALIZATION,
                GLEW_INITIALIZATION,

                DISPLAY_CONTEXT_NOT_CREATED,
                DISPLAY_CONTEXT_IN_USE,

                OPENGL_SHADER_COMPILATION,
                OPENGL_SHADER_LINKING,
                OPENGL_SHADER_VALIDATION,

                ORC_SHADER_FILE_NOT_FOUND
        };

};

#endif // _CODE_TABLE_H
