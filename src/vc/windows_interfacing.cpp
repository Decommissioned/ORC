#define _WIN32_WINNT 0x0500 
#include <windows.h> 

void HideConsoleWindow()
{
        HWND hWnd = GetConsoleWindow();
        ShowWindow(hWnd, SW_HIDE);
}