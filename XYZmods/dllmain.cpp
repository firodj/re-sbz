// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>
#include <sstream>
#include <iomanip> // Required for setw and setfill

#if 0
#include <pybind11/embed.h> // Essential header for embedding
namespace py = pybind11;
#endif

#include "univhook.hpp"
#include "globals.hpp"
#include "console.hpp"
#include "menu.hpp"
#include "util.hpp"
#include "user32/user32hook.hpp"

bool CheckIsXYZ() {
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[XYZ] Found Base Module: %p\n", base);

	if (_stricmp(base, TO_STRING(APPEXE)) == 0) {
		DebugLog("[XYZ] Detected " TO_STRING(APPEXE) ", applying specific hooks.\n");
		return true;
	}

	return false;
}

typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;
static WinMainFn origWinMain = NULL;
HANDLE g_hStartEvent = NULL;
LPVOID pfnWinMain = (LPVOID)0x000000;	// <- fill this

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[XYZ] WinMain ! Wait for Start Event...\n");
	WaitForSingleObject(g_hStartEvent, INFINITE);
	DebugLog("[XYZ] WinMain continue ...\n");

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return ret;
}
void XYZCustomInit() {
    if (!CheckIsXYZ())return;

	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	MH_STATUS res;
	if (pfnWinMain == 0x000000) {
		DebugLog("{yukihota} Please update pfnWinMain into correct address\n");
	}
	else {
		res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
		if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
		DebugLog("[XYZ] Hooked WinMain; %s\n", MH_StatusToString(res));
	}
	user32hook::Init();

	DebugLog("[XYZ] All hooking done !\n");
	if (g_hStartEvent)
	{
		// Signal the event, waking up the main thread
		SetEvent(g_hStartEvent);
	}
}

bool XYZCustomRender(const char *window_name)
{
	return false;
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
        globals::SetCustomInit(XYZCustomInit);
        globals::SetCustomRender(XYZCustomRender);
        univhook::Attach(hModule);
        break;

    case DLL_PROCESS_DETACH:
        univhook::Detach();
        break;        
    }
    return TRUE;
}

