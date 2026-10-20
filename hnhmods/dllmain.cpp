// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "univhook.hpp"
#include "globals.hpp"
#include "console.hpp"
#include "hnh_header.hpp"


void HNHCustomInit() {
    // Check Executable
    bool isHnh = CheckIsHNH();
    if (isHnh) {
        InitHNH();
    }
}

void HNHCustomRender(const char *window_name) {
    //DrawPythonREPL();
    DrawJupyterNotebookREPL();
    DrawHNH(window_name);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        globals::inputHookDisabled = true;
        globals::disableConsoleAutoHide = false;
        globals::SetCustomInit(HNHCustomInit);
        globals::SetCustomRender(HNHCustomRender);
        univhook::Attach(hModule);
        break;

    case DLL_PROCESS_DETACH:
        univhook::Detach();
        break;        
    }
    return TRUE;
}

