// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "univhook.hpp"
#include "globals.hpp"
#include "console.hpp"
#include "sbz_header.hpp"


void SBZCustomInit() {
    // Check Executable
    bool isSbz = CheckIsSBZ();
    if (isSbz) {
        DebugLog("[DllMain] Detected Sexy Beach Zero. Applying SBZ-specific hooks.\n");
        // std::cout << "Detected Sexy Beach Zero. Applying SBZ-specific hooks." << std::endl;
        // Apply any SBZ-specific hooks here if needed
        InitSBZ();
    }
}

void SBZCustomRender() {
    //DrawPythonREPL();
    DrawJupyterNotebookREPL();
    DrawSBZ();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        globals::SetCustomInit(SBZCustomInit);
        globals::SetCustomRender(SBZCustomRender);
        univhook::Attach(hModule);
        break;

    case DLL_PROCESS_DETACH:
        univhook::Detach();
        break;        
    }
    return TRUE;
}

