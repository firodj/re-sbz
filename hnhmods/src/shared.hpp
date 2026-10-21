#pragma once

#include <type_traits>
#include "struct.hpp"

extern HINSTANCE &g_hInstance;
extern HWND &g_hwndParent;

extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7 &g_d3dDevDesc_4D6980;
extern LPDIRECTSOUND3DLISTENER &g_pds3DListener; // 0x52F3C4
extern LPD3DXCONTEXT& g_pRenderCtx;
extern LPDIRECTDRAWSURFACE7 &g_pZBuffer_4D6C94;
extern LPDIRECTDRAWSURFACE7 &g_pBackBuffer_4D6C90;
extern LPDIRECTDRAWSURFACE7& g_pPrimaryBuffer_4D6C88;
extern LPDIRECTDRAW7& g_pDD_4D6C7C;
//LPDIRECTDRAW7 &g_pDD_4D6C7C = *(LPDIRECTDRAW7 *)0x4D6C7C; // WRITABLE
extern LPDIRECT3D7& g_pD3D_4D6C84;
//LPDIRECT3D7 &g_pD3D_4D6C84 = *(LPDIRECT3D7 *)0x4D6C84; // WRITABLE
//LPDIRECTDRAWSURFACE7 &g_pBackBuffer_4D6C90 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C90; // WRITABLE
//LPDIRECTDRAWSURFACE7 &g_pZBuffer_4D6C94 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C94; // WRITABLE


extern DWORD &g_dwFogEnable_4D6CF4;
extern DWORD &g_dwFogRangeEnable_4D6CFC;
extern DWORD & g_dwFogMode_4B00C8;
extern DWORD &g_dwFogTableEnable_4D6CF8;
extern DWORD &g_dwFogColor_4B00CC;

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
extern char (&g_szDbgPrefix)[104];

extern CObjItemRef_S208* &g_pObjectTable;
extern int      &g_nObjectTableCount;
extern int      &g_bDSInited_5185C4;
extern BOOL     &g_bFogIsActive_4B3480;
extern BOOL     &g_bSpecularEnable_4B3474;
extern uint32_t &g_bAntiAliasEnable;
extern uint32_t &g_bDitherEnable;
extern uint32_t &g_bLightEnable_4B3484;
extern uint32_t &g_bShadowEnable_518CA8;
extern int      &g_nResolutionMode_518C80;
extern uint32_t &g_nColorDepthMode_518C9C;
extern int      &g_nRenderHALMode_518CA0;
extern uint32_t &g_nWidth2;
extern uint32_t &g_fScreenScale_52F19C;
extern BOOL     &g_bSpecularEnable;

extern int (&g_arrZBufDepthCountPerDev_518C40)[16];
extern int (&g_arrZBufDepthsPerDev_518828)[256];

extern int& g_nObjectTableCapacity_4B009C;

extern uint32_t& g_nLightningState;
extern uint32_t& g_nLightningEnabled;
extern __int64& g_bitsLightIndexes_4D6CB8;

extern uint32_t& g_bInited_4D6C6C;
extern CHanaLightT3EF_S1A0*& g_pLight_4D6CE0;
extern CHanaMateT3EE_S184*& g_pMaterial;
//CHanaMateT3EE_S184 * &g_pMaterial = *(CHanaMateT3EE_S184 * *)0x4D6CDC; // WRITABLE
extern CHanaFrameRootT3EC_S2D4*& g_pSceneFrame;
//CHanaFrameRootT3EC_S2D4 * &g_pSceneFrame = *(CHanaFrameRootT3EC_S2D4 * *)0x4D6CA8; // WRITABLE
extern CHanaFrameT3EC_S354*& g_pCameraFrame;
//CHanaFrameT3EC_S354 * &g_pCameraFrame = *(CHanaFrameT3EC_S354 * *)0x4D6CAC; // WRITABLE
extern CHanaFrameT3EC_S354*& g_pLightFrame;
//CHanaFrameT3EC_S354 * &g_pLightFrame = *(CHanaFrameT3EC_S354 * *)0x4D6CB0; // WRITABLE


extern void (__cdecl * SetupDirPath_486630)(void);
extern bool (__cdecl * CheckInstalledDirRoot_4869A7)(HWND hWnd);
extern BOOL(__cdecl* CDROM_VerifyDiscSignature)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize);

const char *__cdecl DDErrToString_423CE7(DWORD hr);
BOOL __cdecl IsFileExists_47E8EE(CHAR* pszFilePath);
HRESULT __cdecl SetFogEnable_40DFB0(BOOL bFogEnable, BOOL bFogRangeEnable);
HRESULT __cdecl SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode);
HRESULT __cdecl SetFogColor_40E0B4(DWORD dwFogColor);
HRESULT __cdecl SetFogRange_40E0E2(DWORD dwFogStart, DWORD dwFogEnd);

extern int (__cdecl*MemFree_40DF64)(void* pBuf);
extern LPVOID (__cdecl*MemAlloc80_40DEF7)(SIZE_T dwBytes);
extern void (__cdecl*PrintDebugError)(int hr);
extern void(__cdecl*ApplyMaterial)(CHanaMateT3EE_S184* pMate, D3DMATERIAL7* pD3Dmaterial);
extern void(__cdecl*ApplyLightSettings)(CHanaLightT3EF_S1A0* pLight);

void __cdecl CHanaObj__NewLightT3EF(CHanaLightT3EF_S1A0** pOut, D3DLIGHT7* pD3Dlight);
void __cdecl AttachHanaObj_40BAB8(CHanaBase_S130* pParent, LPCSTR szName);
void __cdecl CHanaObj__AddObjItemRef_40B848(LPCSTR szName, CHanaBase_S130* pObj);
D3DXMATRIX* __cdecl MatrixRotateCubeMapFace(CHanaFrameT3EC_S354* pFrame, int mode, D3DXVECTOR3* pV, float angle);
void __cdecl CalculateLocalRotationMatrix(
  CHanaFrameT3EC_S354* pCameraFrame,
  CHanaFrameT3EC_S354* pSceneFrame,
  float* pXpos,
  float* pYpos,
  float* pZpos,
  float* pXYrot,
  float* pYXrot,
  float* pZXrot);

void __cdecl SetUiFont_419BCD(int nCharW, int nFontH, LPCSTR szFaceName);
void __cdecl D3DDeinit();
BOOL __cdecl CheckRunGame_4863C8(HWND hWnd, BOOL bFullScreen);
LRESULT __stdcall WndProcCheckSystem_485FC2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
