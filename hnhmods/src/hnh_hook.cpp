#include "hnh_hook.hpp"
#include "console.hpp"
#include "minhook/include/MinHook.h"

static bool _create_hook = true;

void hnh_HookOn() { _create_hook = true; }
void hnh_HookOff() { _create_hook = false; }

LPVOID hnh_Hook(DWORD dwAddr, LPVOID pDetour, const char* pszName) {
  if (!_create_hook) {
    return NULL;
  }
  LPVOID addr = (LPVOID)dwAddr;
  LPVOID orig = NULL;
  MH_STATUS res = MH_CreateHook(addr, pDetour, reinterpret_cast<LPVOID*>(&orig));
  if (res == MH_OK) res = MH_EnableHook(addr);
  DebugLog("[HNH] Hooked %s; %s\n", pszName, MH_StatusToString(res));
  return orig;
}

