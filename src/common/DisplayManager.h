#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H

#include "config.h"
#include <functional>

namespace ORC_NAMESPACE
{

        class DisplayManager final
        {

        public:

                using MouseButtonCallback = std::function < void(uint8, bool) > ;
                using KeyboardCallback = std::function < void(char, bool) > ;
                using MouseMovementCallback = std::function < void(int16, int16) > ;
                using MouseWheelCallback = std::function < void(int16) >;

                // DESCRIPTION: Blocks the caller thread and starts processing the message queue
                // REMARK: This method should only be called once per program
                static void EnterMessageLoop();

                static uint32 CreateWindow(const char* title, uint32 width, uint32 height);
                static void CreateOpenGLContext(uint32 windowID);
                static void DestroyWindow(uint32 windowID);

                static void SetVisible(uint32 windowID, bool visible);
                static void Present(uint32 windowID);

                static void AddMouseButtonHandler(uint32 windowID, MouseButtonCallback& callback);
                static void AddKeyboardHandler(uint32 windowID, KeyboardCallback& callback);
                static void AddMouseMovementHandler(uint32 windowID, MouseMovementCallback& callback);
                static void AddMouseWheelHandler(uint32 windowID, MouseWheelCallback& callback);

                static bool ExitRequested;

        };

};

#endif // _DISPLAY_MANAGER_H
