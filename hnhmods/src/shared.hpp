#pragma once

#include <type_traits>
#include "struct.hpp"

// === DEFINED WinMain.cpp ===
extern HINSTANCE &g_hInstance;
extern HWND      &g_hwndParent;

extern BOOL                 &g_bdiAcquire_5196E0;
extern LPDIRECTINPUTA       &g_ppDI;
extern LPDIRECTINPUTDEVICEA &g_pdiMouse_5196D8;
extern DIDATAFORMAT         &g_diMouseFormat_4ACD20;
extern LPDIRECTINPUTDEVICEA &g_pdiKeyboard;
extern DIDATAFORMAT         &g_diKeyboardFormat_4ACD08;

extern BOOL (__cdecl*DIMouseKeybInit_43AF33)(int bAcquire);
extern BOOL (__cdecl*DInputInit_43AD67)(HINSTANCE hinst, HWND hWnd);

// === DEFINED WndProcCheckSystem.cpp ===

extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7    &g_d3dDevDesc_4D6980;

extern LPDIRECTSOUND3DLISTENER &g_pds3DListener; // 0x52F3C4
extern LPD3DXCONTEXT           &g_pRenderCtx;
extern LPDIRECTDRAWSURFACE7    &g_pZBuffer_4D6C94;
extern LPDIRECTDRAWSURFACE7    &g_pBackBuffer_4D6C90;
extern LPDIRECTDRAWSURFACE7    &g_pPrimaryBuffer_4D6C88;
extern LPDIRECTDRAW7           &g_pDD_4D6C7C;
extern LPDIRECT3D7             &g_pD3D_4D6C84;
extern LPDIRECTSOUND           &g_ppDS;
extern LPDIRECTSOUNDBUFFER     &g_pdsBuffer;

extern CObjItemRef_S208* &g_pObjectTable;
extern int &g_nObjectTableCount;
extern int &g_bDSInited_5185C4;
extern uint32_t &g_dwBytesPerPixel;
extern uint32_t &g_bInited_4D6C6C;
extern CHanaLightT3EF_S1A0* &g_pLight_4D6CE0;
extern CHanaMateT3EE_S184*  &g_pMaterial;
extern CHanaFrameRootT3EC_S2D4* &g_pSceneFrame;
extern CHanaFrameT3EC_S354*     &g_pCameraFrame;
extern CHanaFrameT3EC_S354*     &g_pLightFrame;

extern const char *(__cdecl*HanaDS_SetErrStr)(DWORD hr);
extern void (__cdecl*SetUiFont_419BCD)(int nCharW, int nFontH, LPCSTR szFaceName);
extern void (__cdecl*D3DDeinit)(void);
extern LRESULT (__stdcall*WndProcCheckSystem_485FC2)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// === DEFINED MemAlloc.cpp ===

extern int  &g_nMemCnt_4D6CF0;
extern char (&g_szDbgPrefix)[104];
extern HANDLE& g_hHeapHandle;
extern int &g_nHeapRefCount;

extern int    (__cdecl*MemFree_40DF64)(void* pBuf);
extern LPVOID (__cdecl*MemAlloc_40DEF7)(SIZE_T dwBytes);
extern void   (__cdecl*PrintDebugError)(int hr);

extern LPVOID(__cdecl* AllocMem)(SIZE_T size);
extern BOOL(__cdecl* SafeFree)(LPVOID lpMem);

// === DEFINED SetFogEnable.cpp ===

extern DWORD &g_dwFogEnable_4D6CF4;
extern DWORD &g_dwFogRangeEnable_4D6CFC;
extern DWORD &g_dwFogMode_4B00C8;
extern DWORD &g_dwFogTableEnable_4D6CF8;
extern DWORD &g_dwFogColor_4B00CC;
extern BOOL  &g_bFogIsActive_4B3480;

extern HRESULT (__cdecl*SetFogEnable_40DFB0)(BOOL bFogEnable, BOOL bFogRangeEnable);
extern HRESULT (__cdecl*SetFogMode_40E02D)(int bFogTableEnable, DWORD dwFogMode);
extern HRESULT (__cdecl*SetFogColor_40E0B4)(DWORD dwFogColor);
extern HRESULT (__cdecl*SetFogRange_40E0E2)(DWORD dwFogStart, DWORD dwFogEnd);

// === DEFINED CheckInstalledDirRoot.cpp ===

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

extern void (__cdecl * SetupDirPath_486630)(void);

extern bool (__cdecl * CheckInstalledDirRoot_4869A7)(HWND hWnd);

// === DEFINED CheckRunGame.cpp ===

extern BOOL     &g_bSpecularEnable_4B3474;
extern BOOL     &g_bAntiAliasEnable;
extern BOOL     &g_bDitherEnable;
extern BOOL     &g_bLightEnable_4B3484;
extern BOOL     &g_bShadowEnable_518CA8;
extern int      &g_nResolutionMode_518C80;
extern uint32_t &g_nZBufDepthMode_518C9C;
extern int      &g_nRenderHALMode_518CA0;
extern uint32_t &g_nWidth2;
extern uint32_t &g_nScreenScale_52F19C;

extern int (&g_arrZBufDepthCountPerDev_518C40)[16];
extern int (&g_arrZBufDepthsPerDev_518828)[256];

extern int      &g_nObjectTableCapacity_4B009C;
extern uint32_t &g_nLightningState;

extern BOOL      &g_bSpecularEnable;
extern HWND      &g_hWnd;
extern HINSTANCE &g_hInstance_518CB0;
extern BOOL      &g_bRunGame;


extern void (__cdecl*CHanaObj__NewLightT3EF)(CHanaLightT3EF_S1A0** pOut, D3DLIGHT7* pD3Dlight);
extern void (__cdecl*AttachHanaObj_40BAB8)(CHanaBase_S130* pParent, LPCSTR szName);
extern void (__cdecl*CHanaObj__AddObjItemRef_40B848)(LPCSTR szName, CHanaBase_S130* pObj);
extern D3DXMATRIX* (__cdecl* MatrixRotateCubeMapFace)(CHanaFrameT3EC_S354* pFrame, int mode, D3DXVECTOR3* pV, float angle);
extern void(__cdecl* CalculateLocalRotationMatrix)(
  CHanaFrameT3EC_S354* pCameraFrame,
  CHanaFrameT3EC_S354* pSceneFrame,
  float* pXpos,
  float* pYpos,
  float* pZpos,
  float* pXYrot,
  float* pYXrot,
  float* pZXrot);
extern BOOL(__cdecl* CheckRunGame_4863C8)(HWND hWnd, BOOL bFullScreen);
extern DWORD(__cdecl* ReadFileToBuffer)(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize);
extern void(__cdecl* WriteBufferToFile)(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
extern void(__cdecl* DisplayConfig_Init)(CDisplayConfig* pDisplayConfig);
extern int(__cdecl* ShowTitleMenuWnd_428927)(HINSTANCE hInstance);
extern HRESULT(__cdecl* D3DInitBuffers_40AF60)(void);

// === DEFINED ShowTitleMenu.cpp ===

extern uint32_t &g_dwShadowOff;
extern uint32_t &g_bForceDirectionalLights;
extern CDisplayConfig &g_displayConfig_518C30;
extern D3DXMATRIX &g_pM2;
extern CD3DInitParam &g_D3DDevParams;
extern uint32_t &g_bNonShowMenu_4D6C70;
extern uint32_t &g_pCurMatObj;
extern CHanaTextureT3EB_S2CC* &g_pCurTexObj;
extern uint32_t &g_bAlphaBlendEnabled_4D6CC0;
extern uint32_t &g_nAlphaOpState_4D6CC4;
extern uint32_t &g_bCreateFlags_4D6CD4;
extern HWND &g_hWndDev_4D6C74;
extern uint32_t &g_nObjCount;
extern uint32_t &g_nCapObjects_4CD780;
extern CObjPoolMgr_S58* &g_pObjPool;
extern D3DX_SURFACEFORMAT& g_dwSurfaceFormat_4B00B4;
extern uint32_t& g_nColorBits_518C2C;
extern uint32_t &g_nZBufDepthSelValue_518C98;
extern int &g_nModeSetIndex;
extern uint32_t &g_nWidth;
extern uint32_t &g_nHeight;
extern uint32_t &g_nHeight2;
extern unsigned int& g_nBitness;
extern uint32_t& g_nDeviceCount;
extern D3DX_DEVICEDESC*& g_pDevicesDesc;

extern BOOL(__cdecl* ShowTitleMenu_427DA8)(HWND hWnd, BOOL bFullScreen);
extern void(__cdecl* D3DSetRenderStates_4276C0)(void);

// === DEFINED CHanaObj__Free.cpp ===

extern uint32_t& g_nLightningEnabled;
extern __int64& g_bitsLightIndexes_4D6CB8;

extern void(__cdecl*ApplyMaterial)(CHanaMateT3EE_S184* pMate, D3DMATERIAL7* pD3Dmaterial);
extern void(__cdecl*ApplyLightSettings)(CHanaLightT3EF_S1A0* pLight);
extern void (__cdecl*CHanaObj__Free_40C1E4)(CHanaBase_S130 *pObj);
extern void (__cdecl*CHanaObj__GetRealPath_486D9A)(LPSTR szPathOut, int nPathCode);
extern void (__cdecl*CHanaObj__AddRef_40BFC3)(CHanaBase_S130* pObj);
extern void(__cdecl* RemoveChild_4058C3)(CHanaFrameRootT3EC_S2D4* pParent, CHanaFrameT3EC_S354* pChild);
extern void(__cdecl* FreeSoundStruct_422C72)(CSoundStruct_8* pSound);

// === CDROM_VerifyDiscSignature.cpp ===

extern BOOL(__cdecl* CDROM_VerifyDiscSignature)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize);

// === DEFINED DDErrToString.cpp ===
extern const char * (__cdecl*DSErrToString_423CE7)(HRESULT hr);
extern const char* (__cdecl* DDErrToString_41898E)(HRESULT hr);

// === DEFINED IsFileExists.cpp ===

extern BOOL (__cdecl*IsFileExists_47E8EE)(CHAR* pszFilePath);

// === DEFINED GameMainLoop.cpp ===

extern void (__cdecl*GameMainLoop_47F013)(void);
extern LPDIRECTDRAWSURFACE7(__cdecl* GetSurfaceFromBmp_419DE5)(LPCSTR lpFileName);
extern LPDIRECTDRAWSURFACE7(__cdecl*LoadSoundEffects_47EB7F)(void);
extern void (__cdecl*ReleaseSoundEffects)(void);

extern LPDIRECTSOUNDBUFFER (&g_arrUIEffectDSBuffer_52F168)[5];
extern CSoundStruct_8 *(&g_arrFootstepSound_52F18C)[4];
extern CSoundStruct_8 *(&g_arrFootstepSound_52F17C)[4];
extern LPDIRECTDRAWSURFACE7 &g_pSurfaceDarkenBmp_52F1A8;
extern LPDIRECTDRAWSURFACE7(__cdecl* GetSurfaceFromBmp_419DE5)(LPCSTR lpFileName);
extern HRESULT(__cdecl* GetDC_4188B4)(LPDIRECTDRAWSURFACE7 pDDSurface, HDC* pDC);
extern void(__cdecl* PlayVideoWith3DAudio_4791A3)(const char* szMoviePath, int bLoopFlags);

extern int(__cdecl* ConvertFrom640AsInt)(int nNum);
extern double(__cdecl*ConvertFrom640AsFloat)(float fNum);
extern int(__cdecl* ConvertFrom1024AsInt)(int nNum);
extern double(__cdecl* ConvertFrom1024AsFloat)(float fNum);

// === DEFINED CHanaObj__NewFrameT3EC_S354.cpp ===

extern void(__cdecl* CHanaObj__NewFrameT3EC_S354)(CHanaFrameT3EC_S354* pParent, CHanaFrameT3EC_S354** ppOutFrame);
extern void(__cdecl* CHanaObj__NewFrameRootT3EC_S2D4)(CHanaFrameRootT3EC_S2D4** ppOutFrameRoot);
extern void(__cdecl* CHanaObj__NewMateT3EE_S184)(CHanaMateT3EE_S184** ppOutMate);

extern void(__cdecl* AttachChild_4055FD)(CHanaFrameRootT3EC_S2D4* pParent, CHanaFrameT3EC_S354* pChild);
extern void(__cdecl* MtxStackStartMultLocal_4074EF)(void);
extern void(__cdecl* MtxStackRecursiveMulLocal_4075B0)(CHanaFrameT3EC_S354* pFrame);

// === DEFINED WndProcTitleMenu.cpp ===

extern WNDPROC WndProcTitleMenu_428C11;
extern BOOL(__cdecl* FreeBmp_4211ED)(CBMPFile* pBMPFile);
extern CBMPFile* (__cdecl* LoadBmp_420DF0)(LPCSTR lpFileName);

// === DEFINED StopAndRestartThread.cpp ===

extern CInputState* (*GetInputState_481B37)(void);
extern bool(__cdecl* GetCursorPos_4811FA)(int* pOutX, int* pOutY, __int16* pOutBtn);
extern void(__cdecl* MutexThreadWait_4815EC)(bool bWait);
extern void(__cdecl* StopAndRestartThread)(DWORD nThreadType);
