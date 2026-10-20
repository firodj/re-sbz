/* 
 * WinMain - Entry point for the HanaHime application
 * Initializes window, DirectInput, and runs the main game loop
 * Decompiled from address 0x485ce0
 */

#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===
HINSTANCE &g_hInstance  = *(HINSTANCE*)0x52F5C8;
HWND      &g_hwndParent = *(HWND*)0x52F5CC;

// === GLOBAL FUNCTIONS ===
int (__cdecl *DInputInit_43AD67)(HINSTANCE hinst, HWND hWnd) = reinterpret_cast<int(__cdecl*)(HINSTANCE, HWND)>(0x43AD67);
void (__cdecl *GameMainLoop_47F013)(void) = reinterpret_cast<void(__cdecl*)(void)>(0x47F013);

// === FUNCTION IMPLEMENTATIONS ===
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

    DebugLog("[HNH:SUB] WinMain_485CE0\n");

    // Initialize return value
    exitCode = 0;
    
    // Store global instance handle
    g_hInstance = hInstance;
    
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
            g_hInstance = hInstance;
            
            // Register window class only if no previous instance
            if ( !hPrevInstance )
            {
                // Initialize window class
                windowClass.style = 0;
                windowClass.lpfnWndProc = WndProcCheckSystem_485FC2;
                windowClass.cbClsExtra = 0;
                windowClass.cbWndExtra = 0;
                windowClass.hInstance = g_hInstance;
                windowClass.hIcon = LoadIconA(g_hInstance, MAKEINTRESOURCEA(0x69));
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
            g_hwndParent = CreateWindowExA(
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
                             g_hInstance,
                             0);
            
            if ( g_hwndParent )
            {
                // Show and update window
                ShowWindow(g_hwndParent, 5);
                UpdateWindow(g_hwndParent);
                
                // Initialize subsystems
                directInputInitResult = 0;
                soundInitResult = 0;
                DInputInit_43AD67(g_hInstance, g_hwndParent);
                directInputInitResult = 1;
                soundInitResult = 1;
                
                // Check initialization results
                if ( !directInputInitResult )
                    1;
                if ( !soundInitResult )
                    1;
                
                // Invalidate window to trigger repaint
                InvalidateRect(g_hwndParent, 0, 1);
                
                // Message loop
                while ( PeekMessageA(&msg, 0, 0, 0, 1u) )
                {
                    if ( msg.message == WM_QUIT )
                        return msg.wParam;
                    TranslateMessage(&msg);
                    DispatchMessageA(&msg);
                }
                
                // Cleanup
                InvalidateRect(g_hwndParent, 0, 1);
                GameMainLoop_47F013();
                DestroyWindow(g_hwndParent);
                
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
    hnh_Hook(0x485CE0, WinMain_485CE0, "WinMain_485CE0");
}