#pragma once

#include <type_traits>
#include "struct.hpp"

void hnh_Hook(DWORD dwAddr, LPVOID pDetour, const char* pszName);

extern HINSTANCE& g_hInstance;
extern HWND& g_hwndParent;

extern DWORD &g_dwFogEnable_4D6CF4;
extern DWORD &g_dwFogRangeEnable_4D6CFC;
extern DWORD& g_dwFogMode_4B00C8;
extern DWORD& g_dwFogTableEnable_4D6CF8;
extern DWORD& g_dwFogColor_4B00CC;

extern CHAR (&g_szHnhPath_530270)[256];
extern CHAR (&g_szODSPath)[256];
extern CHAR (&g_szDriveLetr_52FC70)[256];
extern CHAR (&g_szSPRPath)[256];
extern CHAR (&g_szStrPath)[256];
extern CHAR (&g_szODFPath)[256];
extern CHAR (&g_szWAVPath)[256];
extern CHAR (&g_szGRPPath)[256];
extern CHAR (&g_szGRP800Path)[256];
extern CHAR (&g_szSEPath)[256];
extern CHAR (&g_szDATAPath)[256];
extern char(&g_szDbgPrefix)[104];

extern CObjItemRef_S208* &g_pObjectTable;
extern int &g_nObjectTableCount;
extern int &g_bDSInited_5185C4;
extern BOOL& g_bFogIsActive_4B3480;
extern uint32_t &g_bSpecularEnable_4B3474;
extern uint32_t &g_bAntiAliasEnable;
extern uint32_t &g_bDitherEnable;
extern uint32_t &g_bLightEnable_4B3484;
extern uint32_t &g_bShadowEnable_518CA8;
extern int &g_nResolutionMode_518C80;
extern uint32_t &g_nColorDepthMode_518C9C;
extern int &g_nRenderHALMode_518CA0;
extern uint32_t &g_nWidth2;
extern uint32_t& g_fScreenScale_52F19C;

void __cdecl SetupDirPath_486630();
bool __cdecl CheckInstalledDirRoot_4869A7(HWND hWnd);
BOOL __cdecl CDROM_VerifyDiscSignature(int chDriveLetter, const char *szExpectedLabel, size_t nLabelSize);

const char *__cdecl DDErrToString_423CE7(DWORD hr);
BOOL __cdecl IsFileExists_47E8EE(CHAR* pszFilePath);
HRESULT __cdecl SetFogEnable_40DFB0(BOOL bFogEnable, BOOL bFogRangeEnable);
HRESULT __cdecl SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode);
HRESULT __cdecl SetFogColor_40E0B4(DWORD dwFogColor);
HRESULT __cdecl SetFogRange_40E0E2(DWORD dwFogStart, DWORD dwFogEnd);

int __cdecl GlobalFree_40DF64(void* pBuf);
LPVOID __cdecl GAlloc80_40DEF7(SIZE_T dwBytes);
void __cdecl PrintDebugError(int hr);
