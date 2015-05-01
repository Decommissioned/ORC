#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "config.h"
#include "error_codes.h"

namespace ORC_NAMESPACE
{

        class Display final
        {

        public:

                explicit Display(const char* title, int32 width, int32 height, bool create_context = true);
                ~Display();

                void Present() const;
                void SetVisible(bool visible);

                Error CreateOpenGLContext();
                uint32 ID() const;

        private:

                void* _glContext;
                void* _wnd;

                static uint32 _ref_count;
                uint32 _wndID;

        };

};

#endif // _DISPLAY_H
