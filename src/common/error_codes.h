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

                GLEW_INITIALIZATION,

                DISPLAY_CONTEXT_NOT_CREATED,
                DISPLAY_CONTEXT_IN_USE,

                OPENGL_INITIALIZATION,

                OPENGL_MESH_INVALID,

                OPENGL_SHADER_COMPILATION,
                OPENGL_SHADER_LINKING,
                OPENGL_SHADER_VALIDATION,

                OPENGL_INVALID_UNIFORM_NAME,
                OPENGL_INVALID_UNIFORM_TYPE,

                OPENGL_UNIFORM_BUFFER_QUERY,
                OPENGL_UNIFORM_BUFFER_FULL,

                IO_FILE_NOT_FOUND,
                IO_FILE_OPEN,
                IO_LOAD_PNG,
                IO_LOAD_OBJ
        };

};

#endif // _CODE_TABLE_H
