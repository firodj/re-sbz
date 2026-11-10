#pragma once

// add headers that you want to pre-compile here
#include "framework.h"

LPVOID hnh_Hook(DWORD dwAddr, LPVOID pDetour, const char* pszName);
void hnh_HookOn();
void hnh_HookOff();
