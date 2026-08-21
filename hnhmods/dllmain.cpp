// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "univhook.hpp"
#include "globals.hpp"
#include "console.hpp"
#include "hnh_header.hpp"


void HNHCustomInit() {
    // Check Executable
    bool isSbz = CheckIsHNH();
    if (isSbz) {
        InitHNH();
    }
}

void HNHCustomRender() {
    //DrawPythonREPL();
    DrawJupyterNotebookREPL();
    DrawHNH();
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

