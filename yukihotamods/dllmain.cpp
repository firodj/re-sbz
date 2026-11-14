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

bool CheckIsyukihota() {
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[yukihota] Found Base Module: %p\n", base);

	if (_stricmp(base, TO_STRING(APPEXE)) == 0) {
		DebugLog("[yukihota] Detected " TO_STRING(APPEXE) ", applying specific hooks.\n");
		return true;
	}

	return false;
}

void register_game_hook();

typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;
static WinMainFn origWinMain = NULL;
HANDLE g_hStartEvent = NULL;
LPVOID pfnWinMain = (LPVOID)0x4A9AB9;

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[yukihota] WinMain ! Wait for Start Event...\n");
	WaitForSingleObject(g_hStartEvent, INFINITE);
	DebugLog("[yukihota] WinMain continue ...\n");

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return ret;
}

void yukihotaCustomInit() {
    if (!CheckIsyukihota())return;

	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	MH_STATUS res;

	if (pfnWinMain == 0x000000) {
		DebugLog("{yukihota} Please update pfnWinMain into correct address\n");
	} else {
		res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
		if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
		DebugLog("[yukihota] Hooked WinMain; %s\n", MH_StatusToString(res));
	}

	register_game_hook();
	//user32hook::Init();

	DebugLog("[yukihota] All hooking done !\n");
	if (g_hStartEvent)
	{
		// Signal the event, waking up the main thread
		SetEvent(g_hStartEvent);
	}
}

bool yukihotaCustomRender(const char *window_name)
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
        globals::SetCustomInit(yukihotaCustomInit);
        globals::SetCustomRender(yukihotaCustomRender);
        univhook::Attach(hModule);
        break;

    case DLL_PROCESS_DETACH:
        univhook::Detach();
        break;        
    }
    return TRUE;
}

BOOL(__cdecl* IsFileExists_4744F1)(void* Src) = reinterpret_cast<BOOL(__cdecl*)(void* Src)>(0x4744F1);
void (*SetupDirPath_492353)() =reinterpret_cast<void(__cdecl*)()>(0x492353);

CHAR(&g_szDiscPath_593A54)[256] = *(CHAR(*)[256])0x593A54;
CHAR(&g_szGamePath_593B54)[256] = *(CHAR(*)[256])0x593B54;


int __cdecl hook_CheckInstalledDirRoot_4926E7(HWND hWnd, int nDiscNum)
{
  char v3[256]; // [esp+4h] [ebp-C84h] BYREF
  char v4[256]; // [esp+104h] [ebp-B84h] BYREF
  char v5[256]; // [esp+204h] [ebp-A84h] BYREF
  char v6[256]; // [esp+304h] [ebp-984h] BYREF
  char Dir[256]; // [esp+404h] [ebp-884h] BYREF
  char Ext[256]; // [esp+504h] [ebp-784h] BYREF
  char Drive[256]; // [esp+604h] [ebp-684h] BYREF
  char Filename[256]; // [esp+704h] [ebp-584h] BYREF
  CHAR RootPathName[256]; // [esp+804h] [ebp-484h] BYREF
  CHAR Src[256]; // [esp+904h] [ebp-384h] BYREF
  char FullPath[256]; // [esp+A04h] [ebp-284h] BYREF
  unsigned int i; // [esp+B04h] [ebp-184h]
  CHAR szHdFile[256]; // [esp+B88h] [ebp-100h] BYREF

  static char arrszDiscFiles_v15[5][25] = {
    "CDFile1",
    "CDFile2",
    "CDFile2"
  }; // [esp+B08h] [ebp-180h] BYREF


  if ( !g_szDiscPath_593A54[0] )
  {
    for ( i = 65; i <= 0x5A && !g_szDiscPath_593A54[0]; ++i )
    {
      wsprintfA(RootPathName, "%c:\\", i);
      if ( GetDriveTypeA(RootPathName) == 5 )
      {
        wsprintfA(Src, "%c:\\%s", i, arrszDiscFiles_v15[nDiscNum]);
        DebugLog("[CheckInstalledDirRoot] %s\n", Src);
        if ( IsFileExists_4744F1(Src) )
        {
          wsprintfA(Src, "%c905050%cddCD027%c%c", 0, 0, 240, 255);
          //if ( subrecursive_49210E(RootPathName, arrszDiscFiles_v15[nDiscNum], FullPath, Src, 17u) )
          {
            wsprintfA(g_szDiscPath_593A54, "%c:", i);
            break;
#if 0
            _splitpath(FullPath, Drive, Dir, Filename, Ext);
            wsprintfA(g_szDiscPath_593A54, "%s%s", Drive, Dir);
            if ( g_szDiscPath_593A54[lstrlenA(g_szDiscPath_593A54)-1] == '\\' )
              g_szDiscPath_593A54[lstrlenA(g_szDiscPath_593A54)-1] = 0;
            break;
#endif
          }
        }
      }
    }
  }
  if ( g_szGamePath_593B54[0] )
    goto LABEL_29;
  for ( i = 65; i <= 0x5A && !g_szGamePath_593B54[0]; ++i )
  {
    if ( g_szDiscPath_593A54[0] != (char)i )
    {
      wsprintfA(RootPathName, "%c:\\", i);
      if ( GetDriveTypeA(RootPathName) == 3 )
      {
        wsprintfA(szHdFile, "%c:\\%s\\HDFile", i, "YuKiHoTa");
        wsprintfA(Src, "%c905050%cddHD027%c%c", 0, 0, 240, 255);
        DebugLog("[CheckInstalledDirRoot] %s\n", szHdFile);
        if ( IsFileExists_4744F1(szHdFile) )
        //if ( subrecursive_49210E(RootPathName, szHdFile, FullPath, Src, 0x11u) )
        {
          wsprintfA(g_szGamePath_593B54, "%c:\\YuKiHoTa", i);
          break;
#if 0
          wsprintfA(RootPathName, "%s", FullPath);
          _splitpath(RootPathName, v5, v3, v6, v4);
          wsprintfA(g_szGamePath_593B54, "%s%s", v5, v3);
          if ( g_szDiscPath_593A54[lstrlenA(g_szGamePath_593B54) + 255] == '\\' )
            g_szDiscPath_593A54[lstrlenA(g_szGamePath_593B54) + 255] = '\0';
          break;
#endif
        }
      }
    }
  }
  if ( i <= 'Z' )
  {
  LABEL_29:
    SetCurrentDirectoryA(g_szGamePath_593B54);
    SetupDirPath_492353();
    return 0;
  }
  else
  {
    if ( nDiscNum == 2 )
      MessageBoxA(hWnd, "必要なフォルダーが見つけられません", "", 0x1000u);
    return -2;
  }
}

void register_game_hook() {
  util::Hook(0x4926E7, hook_CheckInstalledDirRoot_4926E7, "CheckInstalledDirRoot_4926E7");
}