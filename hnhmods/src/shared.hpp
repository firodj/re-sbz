#pragma once

#include <type_traits>

extern HINSTANCE& g_hInstance;
extern HWND& g_hwndParent;

extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7 g_d3dDevDesc_4D6980;
extern DWORD &g_dwFogEnable_4D6CF4;
extern DWORD &g_dwFogRangeEnable_4D6CFC;
extern DWORD& g_dwFogMode_4B00C8;
extern DWORD& g_dwFogTableEnable_4D6CF8;
extern DWORD& g_dwFogColor_4B00CC;

HRESULT __cdecl SetFogEnable_40DFB0(BOOL bFogEnable, BOOL bFogRangeEnable);

typedef std::add_pointer_t<int __cdecl(HINSTANCE hinst, HWND hWnd)> DInputInit_43AD67_Fn;
extern const DInputInit_43AD67_Fn DInputInit_43AD67;

typedef std::add_pointer_t<void __cdecl(void)> GameMainLoop_47F013_Fn;
extern const GameMainLoop_47F013_Fn GameMainLoop_47F013;

const char *__cdecl DDErrToString_423CE7(DWORD hr);
BOOL __cdecl IsFileExists_47E8EE(CHAR* pszFilePath);
HRESULT __cdecl SetFogEnable_40DFB0(DWORD bFogEnable, int bFogRangeEnable);
HRESULT __cdecl SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode);
