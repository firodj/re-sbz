/* 
 * WinMain - Entry point for the HanaHime application
 * Initializes window, DirectInput, and runs the main game loop
 * Decompiled from address 0x485ce0
 */

#include "pch.h"
#include "console.hpp"

#if 0
#define vv(v) v##_
#else
#define vv(v) (v##_ADDR)
#endif

// Global variables
HINSTANCE g_hInstance_ = NULL;
HINSTANCE& g_hInstance_ADDR = *(HINSTANCE*)0x52F5C8;
HWND g_hwndParent_ = NULL;
HWND& g_hwndParent_ADDR = *(HWND*)0x52F5CC;

WNDPROC WndProcCheckSystem_485FC2 = (WNDPROC)0x485FC2;

typedef std::add_pointer_t<int __cdecl(HINSTANCE hinst, HWND hWnd)> DInputInit_43AD67_Fn;
const DInputInit_43AD67_Fn DInputInit_43AD67 = (DInputInit_43AD67_Fn)0x43AD67;
typedef std::add_pointer_t<void __cdecl(void)> GameMainLoop_47F013_Fn;
const GameMainLoop_47F013_Fn  GameMainLoop_47F013 = (GameMainLoop_47F013_Fn)0x47F013;

int __stdcall WinMain_485CE0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    size_t stringLength; // Length of window text
    struct tagMSG msg; // Message structure for window message loop
    CHAR windowText[256]; // Buffer for window text
    int directInputInitResult; // Return value from DirectInput initialization
    int exitCode; // Return value (0 for success, -1 for failure)
    WNDCLASSA windowClass; // Window class structure
    struct tagRECT desktopRect; // Desktop rectangle dimensions
    HWND installerWnd; // Handle to existing installer window
    int soundInitResult; // Return value from sound initialization
    HWND desktopWnd; // Handle to desktop window

    // Initialize return value
    exitCode = 0;
    
    // Store global instance handle
    vv(g_hInstance) = hInstance;
    
    // Clear string buffer
    memset(windowText, 0, 0xFFu);
    
    // Check if HanaHime installer window already exists
    installerWnd = FindWindowA("華の悲鳴のインストーラー", 0);
    if ( !installerWnd || (GetWindowTextA(installerWnd, windowText, 255), stringLength = lstrlenA(windowText), _memicmp("華の悲鳴のインストーラー", windowText, stringLength)) )
    {
        // Check if HanaHime main window already exists
        if ( FindWindowA("華の悲鳴", 0) )
        {
            exitCode = -1;
            return -1; // Another instance is running
        }
        else
        {
            vv(g_hInstance) = hInstance;
            
            // Register window class only if no previous instance
            if ( !hPrevInstance )
            {
                // Initialize window class
                windowClass.style = 0;
                windowClass.lpfnWndProc = WndProcCheckSystem_485FC2;
                windowClass.cbClsExtra = 0;
                windowClass.cbWndExtra = 0;
                windowClass.hInstance = vv(g_hInstance);
                windowClass.hIcon = LoadIconA(vv(g_hInstance), MAKEINTRESOURCEA(0x69));
                windowClass.hCursor = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
                windowClass.hbrBackground = (HBRUSH)GetStockObject(4);
                windowClass.lpszMenuName = 0;
                windowClass.lpszClassName = "華の悲鳴";
                
                // Register the window class
                if ( !RegisterClassA(&windowClass) )
                {
                    exitCode = -1;
                    return -1; // Failed to register window class
                }
            }
            
            // Get desktop window dimensions
            desktopWnd = GetDesktopWindow();
            desktopRect.left = 0;
            desktopRect.top = 0;
            desktopRect.right = 1;
            desktopRect.bottom = 1;
            GetWindowRect(desktopWnd, &desktopRect);
            
            // Create main application window
            vv(g_hwndParent) = CreateWindowExA(
                             0,
                            "華の悲鳴",
                            "華の悲鳴",
                             WS_POPUP,
                             0,
                             0,
                             desktopRect.right - desktopRect.left,
                             desktopRect.bottom - desktopRect.top,
                             0,
                             0,
                             vv(g_hInstance),
                             0);
            
            if ( vv(g_hwndParent) )
            {
                // Show and update window
                ShowWindow(vv(g_hwndParent), 5);
                UpdateWindow(vv(g_hwndParent));
                
                // Initialize subsystems
                directInputInitResult = 0;
                soundInitResult = 0;
                DInputInit_43AD67(vv(g_hInstance), vv(g_hwndParent));
                directInputInitResult = 1;
                soundInitResult = 1;
                
                // Check initialization results
                if ( !directInputInitResult )
                    1;
                if ( !soundInitResult )
                    1;
                
                // Invalidate window to trigger repaint
                InvalidateRect(vv(g_hwndParent), 0, 1);
                
                // Message loop
                while ( PeekMessageA(&msg, 0, 0, 0, 1u) )
                {
                    if ( msg.message == WM_QUIT )
                        return msg.wParam;
                    TranslateMessage(&msg);
                    DispatchMessageA(&msg);
                }
                
                // Cleanup
                InvalidateRect(vv(g_hwndParent), 0, 1);
                GameMainLoop_47F013();
                DestroyWindow(vv(g_hwndParent));
                
                // Final cleanup checks
                if ( !soundInitResult )
                    1;
                if ( !directInputInitResult )
                    1;
                
                return exitCode; // Success
            }
            else
            {
                exitCode = -1;
                return -1; // Failed to create window
            }
        }
    }
    else
    {
        exitCode = -1;
        return -1; // HanaHime installer window found with matching text
    }
}

void register_WinMain_485CE0() {
    LPVOID addr = (LPVOID)0x485CE0;
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(WinMain_485CE0), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked WinMain_485CE0; %s\n", MH_StatusToString(res));
}