#pragma once

#include <type_traits>

extern HINSTANCE& g_hInstance;
extern HWND& g_hwndParent;



typedef std::add_pointer_t<int __cdecl(HINSTANCE hinst, HWND hWnd)> DInputInit_43AD67_Fn;
extern const DInputInit_43AD67_Fn DInputInit_43AD67;

typedef std::add_pointer_t<void __cdecl(void)> GameMainLoop_47F013_Fn;
extern const GameMainLoop_47F013_Fn GameMainLoop_47F013;

const char *__cdecl DDErrToString_423CE7(DWORD hr);
