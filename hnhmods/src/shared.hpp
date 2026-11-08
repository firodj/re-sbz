#pragma once

#include <type_traits>
#include "struct.hpp"

#ifndef HNHMAIN

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

extern void (*HanaDS_Shutdown)();
extern int(__cdecl* HanaDS_Init)(HWND hwnd);
extern IDirectSound3DListener* (*HanaDS_Create3DListener)();

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
extern DDPIXELFORMAT& g_ddPixFormat;

extern CObjItemRef_S208* &g_pObjectTable;
extern int &g_nObjectTableCount;
extern int &g_bDSInited_5185C4;

// persistent scene-flags copy @0x530238 (IDA g_sceneFlag CSceneFlag_28;
// written by ParseExecStr HITEMGET, memcpy'd into sceneState by MainMenu)
extern int (&g_Src)[10];
// 50-char name tables (entries: "GNN.GP" picture / sound wav names)
extern char (&g_aG00Gp_4C9FB0)[100][50];       // READ-ONLY @0x4C9FB0
extern String50 (&g_ascSndFileNames_4C78A0)[200]; // READ-ONLY @0x4C78A0
extern uint32_t &g_dwBytesPerPixel;
extern uint32_t &g_bInited_4D6C6C;
extern CHanaLightT3EF_S1A0* &g_pLight_4D6CE0;
extern CHanaMateT3EE_S184*  &g_pMaterial;
extern CHanaFrameRootT3EC_S2D4* &g_pSceneFrame;
extern CHanaFrameT3EC_S354*     &g_pCameraFrame;
extern CHanaFrameT3EC_S354*     &g_pLightFrame;

extern uint32_t &g_dwGBitMask;
extern uint32_t &g_dwRBitMask;
extern HGDIOBJ& g_hFont;
extern int& g_dwCharWidth;
extern int& g_dwFontHeight;
extern DWORD &g_dwPixelFormat;   // READ-ONLY (16/24/32 bpp)
extern DWORD &g_dwRGBBitMask;   // READ-ONLY (B channel mask)

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

extern void(__cdecl* DrawTextOnSurfaceSpaced_419B05)(LPDIRECTDRAWSURFACE7 pSurface, int a2, int y, int a4, LPCSTR lpString, COLORREF color);
extern void (__cdecl * DrawTextOnSurface)( LPDIRECTDRAWSURFACE7 pSurface, int x, int y, int reserved, LPCSTR lpString, COLORREF color);
extern int (__cdecl * IntPow_44AF15)(int nBase, int nExp);
extern void (__cdecl * CHanaSpriteT3F6__SetScale_414B09)(CHanaSpriteT3F6_S1FC *pSprite, float scale);
extern void (__cdecl * CHanaSpriteT3F6__SetSrcRect_414B28)(CHanaSpriteT3F6_S1FC *pSprite, LPRECT lpRect);
extern double (__cdecl * CHanaSpriteT3F6__GetAlpha_414AFB)(CHanaSpriteT3F6_S1FC *pSprite);
extern void (__cdecl * ShowFadeDialog_46164C)(CHanaSpriteT3F6_S1FC *pSprite, CHanaSpriteT3F6_S1FC *pPanelSprite, LPCSTR szText);
extern CHAR * (__cdecl * ZeroPadNumberToString_4612B0)(int nValue, int nDigits);
extern void (__cdecl * FormatFileLastWriteTime_461397)(CHAR *pszFileName, CHAR *pszOutBuf);
extern void (__cdecl * LoadDataFiles_461557)(CSceneState *pDataBuffer, char *pszTimestamps);


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
extern DWORD& g_dwFontRenderFlag;   // float text-scroll counter @0x52F1B0

extern void (__cdecl * SetupDirPath_486630)(void);
extern bool (__cdecl * CheckInstalledDirRoot_4869A7)(HWND hWnd);
extern BOOL (__cdecl*IsFileExists_47E8EE)(CHAR* pszFilePath);
extern void(__cdecl* ParseStrChunkText_47FF01)(CHanaSceneT458_S504* pScene);
extern void (__cdecl * Scene_CreateFontTextureFromStringData)(CHanaSceneT458_S504 *pScene);
extern int (__cdecl * FindCharUntil_487D46)(char *pBuf, int charToFind, int charToFind2OrEnd);

extern void(__cdecl* CHanaObj__NewFrameArrayT3EDFromChildT3EC)(CHanaFrameT3EC_S354* pFrame_a1, CHanaFrameArrayT3ED_S138** ppFrameArray);
extern int (__cdecl*CHanaObj__GetFramesCountT3ED_408E75)(CHanaFrameArrayT3ED_S138 *pFrameArray);
extern HRESULT (__cdecl*CHanaObj__GetFrameAtIndexT3ED)(CHanaFrameArrayT3ED_S138 *pFrameArray, unsigned int nIndex, CHanaFrameT3EC_S354 **ppOut);
extern void (__cdecl * Character_ApplyAnimationParamsToChildren)( CHanaFrameT3EC_S354 *pRootFrame, char bIsEaseAnimation, int nTimeFrom, int nTimeTo, int nEaseFrom, int nEaseTo);
extern void (__cdecl * CHanaObj__NewFrameArrayT3ED)(CHanaFrameArrayT3ED_S138 **ppFrameArray);
extern CHanaFrameT3EC_S354* (__cdecl* HanaFrame_FindByName)(LPCSTR lpString, CHanaFrameT3EC_S354* pHanaT3EC_v1);
extern void(__cdecl* HanaMesh_AddVertexReference)(CHanaMeshT3F5_S138* pMeshT3F5, CHanaVertT3EA_S1FC* pVertT3EA);

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

extern float &g_fProjZFar;
extern float &g_fProjZNear;
extern float& g_fProjAspect;
extern float& g_fProjFov;

extern int (&g_arrZBufDepthCountPerDev_518C40)[16];
extern int (&g_arrZBufDepthsPerDev_518828)[256];

extern int      &g_nObjectTableCapacity_4B009C;
extern uint32_t &g_nLightningState;

extern BOOL      &g_bSpecularEnable;
extern HWND      &g_hWnd;
extern HINSTANCE &g_hInstance_518CB0;
extern BOOL      &g_bRunGame;
extern D3DXMATRIX &g_mat4x4View_4D4780;
extern D3DXMATRIX &g_mat4x4Projection_4CE870;

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
extern DWORD &g_nWidth;
extern DWORD &g_nHeight;
extern uint32_t &g_nHeight2;
extern unsigned int& g_nBitness;
extern uint32_t& g_nDeviceCount;
extern D3DX_DEVICEDESC*& g_pDevicesDesc;

extern BOOL(__cdecl* ShowTitleMenu_427DA8)(HWND hWnd, BOOL bFullScreen);
extern void(__cdecl* D3DSetRenderStates_4276C0)(void);
extern HRESULT(__cdecl* D3DInit_40A894)(CD3DInitParam* pDevParam);

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
extern CSceneSlot(&g_arrSceneNo_4C6F78)[10];
extern BYTE &g_nAviVolumeIndex_530266;
extern BYTE& g_nAviFrameMultipler_530267;
extern BYTE(&g_byData0Dat_530170)[256];

#endif

extern BYTE &g_sceneProgressFlag_530269;
extern CSceneState &g_sceneState_52F7E0;
#ifndef HNHMAIN

extern const char*& g_szLastDDError_51836C;

// === DEFINED LoadSoundEffects.cpp ===

extern LPDIRECTSOUNDBUFFER(__cdecl* LoadWav_422E7B)(LPSTR pszFileName);
extern CSoundStruct_8 *(__cdecl*LoadWavToDS3DBuffer_42277E)(LPSTR pszFileName);

// === DEFINED InGameMenu.cpp ===

extern void(__cdecl*DSBufferPlay)(LPDIRECTSOUNDBUFFER pDSBuf, int dwPlayFlags);
extern BOOL(__cdecl*DoPeekMessages)(void);
extern void(__cdecl*GetMouseBtn_45A41B)(LPWORD pBtnMouse);
extern void(__cdecl*GetMouseXY_45A4D8)(DWORD *pXmouse, DWORD *pYmouse);
extern LPDIRECTDRAWSURFACE7(__cdecl*LoadBmpScaled_4203B3)(LPCSTR lpFileName, int xRight, int yBottom);
extern int(__cdecl*FlushD3DAndTouchSurface)(LPDIRECTDRAWSURFACE7 pSurface);
extern void(__cdecl*CHanaSpriteT3F6__SetPos_414A5D)(CHanaSpriteT3F6_S1FC *pSprite, float fX, float fY);
extern void(__cdecl*DrawSprite2D)(CHanaSpriteT3F6_S1FC *pSprite);
extern void(__cdecl* LoadBmpWithMask_413A5E)(CHanaSpriteT3F6_S1FC** ppOut, LPCSTR szBmpName, LPCSTR szMaskName);
extern void(__cdecl* GetSurfaceWH)(LPDIRECTDRAWSURFACE7 pSurface, LPDWORD pWidth, LPDWORD pHeight);
extern LPDIRECTDRAWSURFACE7(__cdecl* CreateTextureFromSurfaceWithAlphaMask)(
  LPDIRECTDRAWSURFACE7 surface,
  LPDIRECTDRAWSURFACE7 maskSurface);
extern void(__cdecl* BltSurface)(LPDIRECTDRAWSURFACE7 pSurface, int left, int top, int right, int bottom, int fillColor);
extern LPDIRECTDRAWSURFACE7(__cdecl* CreateSurfaceWH)(DWORD width, DWORD height);
extern void(__cdecl* RecoverLostDevice)(void);
extern void(__cdecl*PresentFrame)(void);
extern void(__cdecl* DrawSpriteAtRect)(CHanaSpriteT3F6_S1FC* pSprite, D2D_RECT_F* rc);

extern void (__cdecl*MainMenu_47DF0D)(void);
extern int (__cdecl*InGameMenu_4799E2)(void);
extern float (&g_fVolumeDbTable)[10];
extern HRESULT(__cdecl*HanaVert_DrawBatch)(int nParam);
extern DWORD& g_dwDSStatus_5185C8;

// === DEFINED SystemMenu_4655AB.cpp ===

extern int(__cdecl* SystemMenu_4655AB)(LPDIRECTSOUNDBUFFER pBgm, int nVolOffset, int nMenuMode,
  float fCurHP, float fMaxHP, CSceneState sceneState);

// === DEFINED ShowYesNoDialog_461A9E.cpp ===

extern int(__cdecl* ShowYesNoDialog_461A9E)(CHanaSpriteT3F6_S1FC* pCursor,
  CHanaSpriteT3F6_S1FC* pDialog, LPCSTR szCaption);

// === DEFINED PlayOdsDialogueScene_477F90.cpp ===

extern int(__cdecl* PlayOdsDialogueScene_477F90)(int nOdsIndex);
extern void(__cdecl* CHanaObj__NewSceneT458_S504)(CHanaSceneT458_S504** ppOut);

// === DEFINED DrawDialogueText_4803B5.cpp ===

extern void(__cdecl* DrawDialogueText_4803B5)(CHanaSceneT458_S504* pScene, float fFrameTime);


// === DEFINED Surface_AlphaBlendRect.cpp ===

extern void(__cdecl* Surface_AlphaBlendRect)(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX,
  int nDestY, LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect, int nBlendWeight,
  char bSkipColorKey);

// === DEFINED ParseExecStr_487D89.cpp ===

extern BOOL(__cdecl* ParseExecStr_487D89)(CHanaSceneT458_S504* pScene, float* pfExecTime);


// === DEFINED SysMenu_SaveMenu_462135.cpp ===

extern void(__cdecl* SysMenu_SaveMenu_462135)(CSceneState* sceneStates, CHAR* arrSceneName,
  LPDIRECTDRAWSURFACE7* arrSurf, CHanaSpriteT3F6_S1FC* pCursor, CHanaSpriteT3F6_S1FC* pYesNo,
  int nMouseX, int nMouseY, int bPressed);
extern void(__cdecl* DrawTextOnSurface)(LPDIRECTDRAWSURFACE7 pSurface, int x, int y,
  int reserved, LPCSTR lpString, COLORREF color);
extern void(__cdecl* ShowFadeDialog_46164C)(CHanaSpriteT3F6_S1FC* pSprite,
  CHanaSpriteT3F6_S1FC* pPanelSprite, LPCSTR szText);

extern DWORD& g_dwUnlockedSaveSlots_530260;
extern int(&g_arrSceneStepThresholds_4B5970)[40];
extern char(&g_szSceneInfoText_4B5A10)[64000];

// === DEFINED SysMenu_LoadMenu_462851.cpp ===

extern int(__cdecl* SysMenu_LoadMenu_462851)(CSceneState* arrScene, CHAR* arrSceneName,
  LPDIRECTDRAWSURFACE7* arrSurf, CHanaSpriteT3F6_S1FC* pCursor, CHanaSpriteT3F6_S1FC* pYesNo,
  int nMouseX, int nMouseY, int bPressed);

// === DEFINED SysMenu_GirlStatusCheck_464919.cpp ===

extern int(__cdecl* SysMenu_GirlStatusCheck_464919)(LPDIRECTDRAWSURFACE7* arrSurf,
  CHanaSpriteT3F6_S1FC* pCursor, CHanaSpriteT3F6_S1FC* pYesNo, int nMouseX, int nMouseY,
  int bPressed, int* pnSceneStep, float fCurHP, float fMaxHP, QWORD* pSceneFlags);

// === DEFINED SysMenu_Options_462FE9.cpp ===

extern void(__cdecl* SysMenu_Options_462FE9)(CSceneState* sceneStates,
  LPDIRECTDRAWSURFACE7* arrSurf, CHanaSpriteT3F6_S1FC* pCursor, CHanaSpriteT3F6_S1FC* pYesNo,
  int nMouseX, int nMouseY, int bPressed);

extern BYTE& g_mapFrameFlag_530264;
extern BYTE& g_nEffectLevel_530265;
extern BYTE& g_btoggleC_530268;

// === DEFINED SysMenu_Graphics_46380D.cpp ===

extern void(__cdecl* SysMenu_Graphics_46380D)(int* arrGfxOpts, CSceneState* arrSceneStates,
  LPDIRECTDRAWSURFACE7* arrSurf, CHanaSpriteT3F6_S1FC* pCursor, CHanaSpriteT3F6_S1FC* pYesNo,
  int nMouseX, int nMouseY, int bPressed);

// === DEFINED MenuCharSelect.cpp ===

extern int(__cdecl* MenuCharSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite);
extern bool(__cdecl*IsMenuKeyPressed_481170)(void);

// === scene-engine pins defined in ParseODS / PlayOdsDialogueScene ===
extern CHanaFrameT3EC_S354* (__cdecl* FindFrameByName_47E999)(CHanaFrameT3EC_S354* pRootFrame,
  LPCSTR pszName);
extern bool(__cdecl* UpdateSceneAtTime_4825D5)(CHanaSceneT458_S504* pScene, float* pfExecTime,
  float fDelta);
extern void(__cdecl* UpdateSceneTimer_4827FE)(CHanaSceneT458_S504* pScene, float* pfExecTime,
  float fDelta);
extern void(__cdecl* PauseSceneAudio_46A2A0)(CHanaSceneT458_S504* pScene, int bPause);

// === more cross-file pins (needed by PlayExploring_45BCAE.cpp) ===
// defined in ParseODS_483932.cpp:
extern int(__cdecl* ParseODFFile_441F19)(LPCSTR lpFileName, CHanaFrameT3EC_S354** pOutRootFrameT3EC,
  CHanaAnimSetT3F1_S150** pOutT3F1);
extern void(__cdecl* CSekBody__PlayAnim_40179D)(CSekBody* pSekBody, int nMode);
extern CHanaFrameT3EC_S354*(__cdecl* CHanaFrameT3EC__FindByName_483650)(CHanaFrameT3EC_S354* pRoot,
  LPCSTR lpString);
// defined in PlayOdsDialogueScene_477F90.cpp:
extern BOOL(__cdecl* LoadOds_4843CF)(LPCSTR lpFileName, CHanaSceneT458_S504** ppScene);
extern void(__cdecl* ResetSceneBodies_4857CA)(CHanaSceneT458_S504* pScene);
extern void(__cdecl* SetProjectionFov_40DCA3)(float fFov);
extern void(__cdecl* DrawDialogueWindow_480B2D)(CHanaSceneT458_S504* pScene,
  float frameTimeMultiplied, int xMouse, int yMouse, short btnMouse);
extern BOOL(__cdecl* CSekBody__UpdateAnim_4019E1)(CSekBody* pSekBody, float fDelta);
extern void(__cdecl* CSekBody__CutToAnim_4018E1)(CSekBody* pSekBody, int nIndex);
extern void(__cdecl* SekBody__UpdateTo2_401647)(CSekBody* pSekBody, int nIndex);
// defined in CHanaObj__Free_40C1E4.cpp:
extern void(__cdecl* CSekBody__Release_4012FE)(CSekBody* pSekBody);
// defined in ProcessFrame_40600C.cpp:
extern void(__cdecl* ProjectToScreen_40E2A5)(D3DXMATRIX* pM, POINT* pOut);

// === DEFINED RunModeSelectScreen.cpp ===

extern void(__cdecl* Frame_ApplyFadeRecursive)(CHanaFrameT3EC_S354* pFrame, float alpha);
extern int(__cdecl* RunModeSelectScreen)(CHanaSceneT458_S504* pScene,
  CHanaSpriteT3F6_S1FC** arrModeSprites, CHanaSpriteT3F6_S1FC** arrMouseSprites, int* piSelected,
  int iModeId, QWORD* pSelMask, int iMenuTab);
extern int(__cdecl* SetFrameVisibilityAndGetHIndex)(CHanaSceneT458_S504* pScene, int iVisState);
extern void(__cdecl* CameraLookAtFrame)(CHanaFrameT3EC_S354* pCameraFrame,
  CHanaFrameT3EC_S354* pTargetFrame, CHanaFrameT3EC_S354* pSceneFrame, short iMode);
extern void (__cdecl* ApplyFrameEffectOption)(CHanaSceneT458_S504* pScene);
extern void(__cdecl* DrawSpritePulse_46D7A7)(CHanaSpriteT3F6_S1FC* pSprite, float fPhase);
extern LPDIRECTSOUNDBUFFER& g_pDSBuf_52EFD0;
extern int(__cdecl* RunSceneSelectScreen)(CHanaSceneT458_S504* pScene,
  CHanaSpriteT3F6_S1FC** arrModeSprites, CHanaSpriteT3F6_S1FC** arrMouseSprites, int* piSelected,
  int iSceneGroup, QWORD* pSelMask, int iMenuTab);

// === DEFINED PlayExploring_45BCAE.cpp ===
// (faithful pseudocode dump of the 0x55FB-byte map scene; helpers pinned in-file)

extern int(__cdecl* PlayExploring_45BCAE)(int nStageIndex);

// === DEFINED PlayHunting_44C38D.cpp ===
// (faithful pseudocode dump of the 0x6F3B-byte duel scene; helpers pinned in-file)

extern int(__cdecl* PlayHunting_44C38D)(int nDuelIndex);

// === DEFINED PlayAssault_4703AE.cpp ===

extern CHanaFrameT3EC_S354 *(__cdecl* Scene_FindMapFrame)(CHanaSceneT458_S504 *pScene);
extern int(__cdecl* PlayCutInScene_46A76F)(CHanaSceneT458_S504 *pScene,
  CHanaSpriteT3F6_S1FC **arrMouseSprites, CHanaSpriteT3F6_S1FC **arrGOTypeSprites,
  CHanaSpriteT3F6_S1FC **arrPostureSprites, CSekBody *pLockOnBody,
  CSekBody *pModeBody, int iGroup, CSoundStruct_8 **arrSoundAHs,
  CSoundStruct_8 **arrSoundSEs, CSekBody **arrTearsBodies, int iMenuTab);

// === DEFINED MenuCharSelect.cpp (continued) ===

extern int(__cdecl* PlayAssault_4703AE)(int iModeId, int iMenuTab);
extern HRESULT(__cdecl*Surface_SetColorKeyRGB)(LPDIRECTDRAWSURFACE7 surface, COLORREF color);
extern int(__cdecl*PlayAssault_4703AE)(int iModeId, int iMenuTab);
extern void(__cdecl* DrawText_43BCAE)(LPDIRECTDRAWSURFACE7 pSurface, char* szText, int left, int top, int right, int bottom);
extern int(__cdecl* MenuMusicSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite);
extern int(__cdecl* MenuGallery)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite);

extern char(&g_arrGalleryUnlocked_5301A2)[150];

// === DEFINED PlayScreenFade.cpp ===

extern LPDIRECTDRAWSURFACE7(__cdecl* PlayScreenFade)(int bisGo, COLORREF colorref,
  LPDIRECTDRAWSURFACE7 pDdSurface, float fFade, char bFlags);

extern DWORD  (__cdecl* RGBToSurfaceFormat)(LPDIRECTDRAWSURFACE7 surface, COLORREF color);
extern int  (__cdecl* CHanaObj__NewSpriteT3F6bySurf)(CHanaSpriteT3F6_S1FC **pOut, LPDIRECTDRAWSURFACE7 pSurfaceSrc);
extern int  (__cdecl* CHanaObj__ReplaceTexture3F6)(CHanaSpriteT3F6_S1FC *pSprite, LPDIRECTDRAWSURFACE7 *pTexture);
extern void (__cdecl* CHanaObj__SetAlpha3F6)(CHanaSpriteT3F6_S1FC *pSprite, float alpha);
extern int  (__cdecl* DrawSpriteLTRB)(CHanaSpriteT3F6_S1FC *pSprite, int left, int top, int right, int bottom);
extern int  (__cdecl* BltFastWH)(LPDIRECTDRAWSURFACE7 pSurf, int xDest, int yDest, LPDIRECTDRAWSURFACE7 pSrc, int left, int top, int width, int height, int ddbltfastFlags);
extern void (__cdecl* RenderFlush_4874A5)(void);
extern HRESULT  (__cdecl* RenderClear_40BF4F)(DWORD ClearFlags);
extern HRESULT  (__cdecl* HanaFrame_Draw)(CHanaFrameT3EC_S354 *pFrame);
extern double (__cdecl* GetTimerScaleFactor)(void);
extern void   (__cdecl* SetTimerScaleFactor)(float fTimeScale);
extern int    (__cdecl* FrameTimer_BeginFrame)(void);
extern DWORD  (__cdecl* FrameTimer_EndFrame)(void);
extern double (__cdecl* GetFrameTime)(void);
extern void   (__cdecl * HanaFrame_GetRelPosition)( CHanaFrameT3EC_S354 *pFrame, CHanaFrameRootT3EC_S2D4 *pRefFrame, float *pOutX, float *pOutY, float *pOutZ);
extern void   (__cdecl*SelfBltDC)(LPDIRECTDRAWSURFACE7 pDDSurf);
extern void   (* HanaFrame_BuildMatrixStack)();
extern void   (__cdecl * CHanaObj__LockVtxBufT3EA_418544)(CHanaVertT3EA_S1FC *pVert, DWORD *pNumVertices, LPVOID *ppData);
extern void   (__cdecl * CHanaObj__UnlockVrxBufT3EA_41858E)(CHanaVertT3EA_S1FC *pVert);
extern unsigned int (* CalculateVertexSize)(DWORD fvf);
extern HRESULT  (__cdecl* HanaFrame_PushNode)(CHanaFrameT3EC_S354* pFrame);
extern void   (__cdecl * HanaMorpT457_Animate)(CHanaFrameT3EC_S354 *pFrame, float fValue, float fEase, int nTimeAnim);
extern int    (__cdecl*CHanaObj__GetType_40BBF4)(CHanaBase_S130* pObj);
extern void(__cdecl* SetInversTransf_405BB4)(D3DXMATRIX* pM);
extern void(__cdecl*HanaFrame_PreDraw)(CHanaFrameT3EC_S354* pFrame);

extern LPD3DXMATRIXSTACK& g_pMatrixStack_4CD77C;
extern uint32_t& g_fCurFrameIsScene;
extern CHanaFrameT3EC_S354 * &g_pCurFrame;
extern BOOL& g_bRendering;
extern BOOL& g_bObjBatchActive;

// === DEFINED ParseODFContent.cpp ===

extern BOOL(__cdecl* ParseODFContent_441A5D)(LPBYTE pBuf1, CHanaFrameT3EC_S354* pRootFrameT3EC,
  CHanaAnimSetT3F1_S150* pAnimSetT3F1);
extern int(__cdecl*ParseOdfANIM_4414EB)(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSetT3F1, CHanaFrameT3EC_S354 *pCurrentFramT3EC);
extern void(__cdecl*AssignLigthtToFrame_40539D)(CHanaFrameT3EC_S354 *pHanaT3EC, CHanaLightT3EF_S1A0 *pHanaLight);
extern void (__cdecl * ApplyQTS_4035B1)( D3DXMATRIX *pOut, D3DXQUATERNION *pQ, float x, float y, float z, float sx, float sy, float sz);
extern void (__cdecl * ConfigureLight)( CHanaLightT3EF_S1A0 *pHanaLight, float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
extern void (__cdecl * HanaFrame_GetCenterPosition_408C3E)( CHanaFrameRootT3EC_S2D4 *pFrame, CHanaFrameRootT3EC_S2D4 *pRelFrame, float *pOutX, float *pOutY, float *pOutZ);
extern BOOL(__cdecl* GetSoundBufferAverageLevel)(LPDIRECTSOUNDBUFFER pDSBuffer, float* pfPlayPos);
extern void(__cdecl* CalcFrameBound_404B80)(CHanaFrameT3EC_S354* pFrameT3EC, CHanaMorpT457_S40C* pMorpT3F5);
extern void(__cdecl* CHanaAnimSetT3F1__SetTime_403D61)(CHanaAnimSetT3F1_S150* pHanaT3F1, float t2);
extern D3DXMATRIX* (__cdecl* MtxMult_4069DD)(CHanaFrameT3EC_S354* pChildFrame, CHanaFrameT3EC_S354* pParentFrame, float x, float y, float z);

// === DEFINED ParseODS.cpp ===

extern int(__cdecl*ParseODS_483932)(LPBYTE pBuf, CHanaSceneT458_S504 *pScene);
extern void(__cdecl* CHanaObj__NewMeshT3F5)(CHanaMeshT3F5_S138** pOut);
extern void(__cdecl* CHanaObj__NewAnimSetT3F1_S150)(CHanaAnimSetT3F1_S150** ppOut);
extern BOOL(__cdecl* LoadOdsEx_48564B)(LPCSTR lpFileName, CHanaSceneT458_S504** ppScene,
  CHanaFrameT3EC_S354** arrFrames, CHanaAnimSetT3F1_S150** arrAnimSets,
  unsigned int nCount);
extern void(__cdecl* SekBody__UpdateTo_4014E4)(CSekBody* pSekBody, int nItem);

extern BOOL(__cdecl* LoadSekAndOdf_4445DD)(LPCSTR pszSekFile, LPCSTR pszOdfFile, CSekBody** ppSekBody);

// === DEFINED CHanaObj__NewFrameT3EC_S354.cpp ===

extern void(__cdecl* CHanaObj__NewFrameT3EC_S354)(CHanaFrameT3EC_S354* pParent, CHanaFrameT3EC_S354** ppOutFrame);
extern void(__cdecl* CHanaObj__NewFrameRootT3EC_S2D4)(CHanaFrameRootT3EC_S2D4** ppOutFrameRoot);
extern void(__cdecl* CHanaObj__NewMateT3EE_S184)(CHanaMateT3EE_S184** ppOutMate);
extern void(__cdecl* CHanaObj__NewSprite3F6byWH)(CHanaSpriteT3F6_S1FC** ppOut, DWORD w, DWORD h);

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

// === DEFINED ProcessFrame.cpp ===

extern void(__cdecl*ProcessFrame)(CHanaFrameT3EC_S354* pChildFrame, int nAmbient, DWORD* qwBitLightEnables);
extern void(__cdecl* CameraLookAtFrame)(CHanaFrameT3EC_S354* pCameraFrame, CHanaFrameT3EC_S354* pTargetFrame, CHanaFrameT3EC_S354* pSceneFrame, __int16 iMode);
extern void(__cdecl* UpdateBoneTransform)(CHanaFrameT3EC_S354* boneStruct, CHanaFrameT3EC_S354* parentStruct, CHanaFrameT3EC_S354* pSceneFrame);
extern void (__cdecl*AddObjectToPool)(D3DXMATRIX *mat4x4After, CHanaFrameT3EC_S354 *pChildFrame, CHanaVertT3EA_S1FC *pVert, int nAmbient, QWORD qwBitLightEnables);
extern DWORD (__cdecl*CHanaObj__GetChildCountT3F5_40FF0F)(CHanaMeshT3F5_S138 *pHanaT3F5);
extern CHanaVertT3EA_S1FC* (__cdecl* CHanaObj__GetVertsAtT3F5_40FF1D)(CHanaMeshT3F5_S138* pHanaT3F5, int nIndex);
extern void(__cdecl* HanaVert_DrawByType)(CHanaVertT3EA_S1FC* pVert);
extern void(__cdecl* SortObjectsByDepth)();
extern void(__cdecl*SetLightEnableMask)(QWORD bitLightEnables);
extern void(__cdecl*UpdateVertTextureFromFrame)(CHanaVertT3EA_S1FC* pVertArg, CHanaFrameT3EC_S354* pFrameArg);
extern void(__cdecl* UpdateBoneLookAtTransform)(CHanaFrameT3EC_S354* pBoneFrame, CHanaFrameT3EC_S354* pParentFrame, float dirForwardX, float dirForwardY, float dirForwardZ, float dirUpX, float dirUpY, float dirUpZ);
extern void(__cdecl* BuildViewportMatrix)();

// === DEFINED SetupHeroEntry_4593E0.cpp ===

extern void(__cdecl* SetupHeroEntry_4593E0)(CHanaSceneT458_S504* pScene,
  CHanaFrameT3EC_S354* pHeroFrame, int nSubIndex);
extern bool(__cdecl* UpdateHeroBody_44474E)(CSekBody* pHeroBody, CHanaFrameT3EC_S354* pMapFrame, float fDeltaTime, float* pfHeroHp, float fHeroHpMax);
extern void(__cdecl* BuildMapCollisionGrid_445B12)(CHanaFrameT3EC_S354* pMapFrame, char** pOutGrid);
extern void(__cdecl* Frame_RotateMatrixAxis_4070AB)(CHanaFrameT3EC_S354* pFrame, int nMode, D3DXVECTOR3* pAxis, float fAngleRad);
extern int(__cdecl* CheckFrameCollision_443CC5)(CHanaFrameT3EC_S354* pBodyFrame, CHanaFrameT3EC_S354* pMapFrame, int bTestType);
extern bool(__cdecl* TestMoveCollision_4443FC)(CHanaFrameT3EC_S354* pBodyFrame, CHanaFrameT3EC_S354* pMapFrame, float fNewX, float fNewY, float fNewZ, int bTestType);
extern bool(__cdecl* HanaGrid_DFSPath)(char* pGrid, int nCellX, int nCellY, int nDepth, __int16 nSteps, int bIgnoreBlocked);
extern D3DXVECTOR2* (__cdecl* HanaGrid_FloodFill_4460AD)(D3DXVECTOR2* pfOutStep, char* pGrid, POINT* pnFoeCell, POINT pnHeroCell, int* pnDepth, int nMode, POINT* ppOutPathPts, WORD* pnOutNumbPathPts);
extern void(__cdecl* UpdateVoicePlayback_458B3A)(CSekBody* pFoeBody, CHanaFrameRootT3EC_S2D4* pFoeFocusFrame, CSoundStruct_8** arrVoiceBufs, float fDelta);
extern int(__cdecl* DSBufferGetVolume_423C65)(LPDIRECTSOUNDBUFFER pDSBuf);
extern double(__cdecl* UpdateFoeAI_446A38)(CSekBody* pFoeBody, CSekBody* pHeroBody,
  CHanaFrameT3EC_S354* pMapFrame, char* pOccGrid, float fDeltaTime,
  float* pfAggro, float* pfHeroHp, float fAggroMax, int nDuelIndex);


extern int& g_dwBattleMode;


// === DEFINED PlayExploring.cpp ===

extern CObjItemRef_S208* (__cdecl* UpdateCameraLinkedObjects_44451F)(CHanaFrameT3EC_S354* pCameraFrame);

extern int (__cdecl*BuildMapSubFrames_443DD5)(CHanaFrameT3EC_S354 *pMapFrame);

extern CHanaSpriteT3F6_S1FC* (__cdecl* InitHudResources_4490A7)(CHanaSpriteT3F6_S1FC** arrSprNumbers,
  CHanaSpriteT3F6_S1FC** arrSprTimes, CHanaSpriteT3F6_S1FC** arrSprModes, int nMode);
extern void (__cdecl*UpdateCameraFollowChase_445525)(CHanaFrameT3EC_S354 *pHeroFrame,
  CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pMapFrame, float fTime, int bMove);
extern char (__cdecl*UpdateHeroControl_45A513)(CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame,
  float fDeltaTime, float *pfHeroHp, float fHeroHpMax, int nMapIndex, float *pfCamFactor);
extern int (__cdecl*CheckSceneTransition_459768)(CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 *pHeroFrame, int *pnMapIndex, int *pnSubIndex, int nStageIndex);
extern HRESULT(__cdecl* UpdateHudGauge_44BC36)(CHanaSpriteT3F6_S1FC** pHudRes, int nCurHp, int nMaxHp);

extern int (__cdecl*PrepareTargetSelect_45B5BD)(CSekBody *pHeroBody, CSekBody **pItemBodies,
  CSekBody **pSpriteNames, CHanaFrameT3EC_S354 *pMapFrame, CHanaSpriteT3F6_S1FC **arrSprNumbers,
  CHanaSpriteT3F6_S1FC **arrSprTimes, float fHeroHp, float fHeroHpMax, int nTimeLimit,
  unsigned int nElapsed);
extern int (__cdecl*ShowMouseSelectDialog_4573CF)(CHanaSpriteT3F6_S1FC **arrSprModes,
  LPDIRECTSOUNDBUFFER *pArrSndBuf);
extern HRESULT (__cdecl*DrawSpriteListEcho_45B2FB)(CHanaSpriteT3F6_S1FC **arrSprites, int nTargetIdx,
  float fDeltaTime);
extern void(__cdecl* HanaMatrix_BuildLookAtAndTransform)(CHanaFrameT3EC_S354* pFoeFrame, CHanaFrameT3EC_S354* pHeroFrame, CHanaFrameRootT3EC_S2D4* pSceneFrame, BOOL bSkip);
extern void(__cdecl* UpdateCameraFollowChase_445525)(CHanaFrameT3EC_S354* pHeroFrame,
  CHanaFrameT3EC_S354* pCameraFrame, CHanaFrameT3EC_S354* pMapFrame, float fTime, int bMove);

extern void(__cdecl* InterpolateHanaAnimFrame)(CHanaAnimT3F0_S240* pAnim, float fTime); // in: ParseODFContent.cpp

extern void(__cdecl* HanaMate_SetMaterial)(CHanaMateT3EE_S184* pMate); // in: ProcessFrame.cpp
extern double (*GetProjectionFov_40DCF6)(); // in: ProcessFrame.cpp

extern void(__cdecl* PlayCinematicSequence_45460A)(CHanaSceneT458_S504* pOdsScene,
  CSekBody* pHeroBody, CSekBody* pFoeBody, CHanaFrameT3EC_S354* pMapFrame,
  CHanaSpriteT3F6_S1FC** pArrHudMeters, CHanaSpriteT3F6_S1FC** pArrSprites,
  float* pfHeroHp, float fFieldC4, float* pfAggro, float fAggroMax,
  int* pnClockBase, unsigned int* pdwElapsedMs, int dwClockBase2,
  CSoundStruct_8** arrVoiceBufs, CHanaSpriteT3F6_S1FC* pMaskBmp); // in: PlayCinematicSequence.cpp
extern void* (__cdecl* DrawDuelHealthHud_44B23B)(CHanaSpriteT3F6_S1FC** pSprites,
  int nHeroHp, int nHeroHpMax, int nAggro, int nAggroMax); // in: PlayHunting.cpp
extern void(__cdecl* DrawClockHud_44AF5B)(CHanaSpriteT3F6_S1FC** arrSprNumbers, int nX, int nY,
  int nTimeLeft); // in: PlayCinematicSequence.cpp

extern void(__cdecl* DrawSprite3D)(CHanaSpriteT3F6_S1FC* pSprite, D3DXVECTOR3* parrPos); // in: PlayHunting.cpp
extern void(__cdecl* HanaSurface_CreateAndFade)(); // in: PlayExploring.cpp
extern void(__cdecl* CHanaSpriteT3F6__GetPos_414A7A)(CHanaSpriteT3F6_S1FC* pSprite, float* pOutX, float* pOutY); // in: RunModeSelectScreen.cpp

extern void(__cdecl* ApplyMateAnimMaterial)(CHanaMateAnimT3F7_S148* pMateAnim, float fNum); // in: ParseODFContent.cpp

extern int(__cdecl* PlayCutInScene_46A76F)(CHanaSceneT458_S504* pScene,
  CHanaSpriteT3F6_S1FC** arrMouseSprites, CHanaSpriteT3F6_S1FC** arrGoSprites,
  CHanaSpriteT3F6_S1FC** arrPostureSprites, CSekBody* pLockOnBody,
  CSekBody* pModeBody, int iGroup, CSoundStruct_8** arrSoundAHs,
  CSoundStruct_8** arrSoundSEs, CSekBody** arrTearsBodies, int iMenuTab); // in: PlayCutInScene.cpp
extern void(__cdecl* InitializePracticeAnimations_4667A9)(
  CHanaFrameT3EC_S354* pModeFrame); // in: PlayAssault.cpp
extern bool(__cdecl* CHanaAnimSetT3F1__AddAnimT3F0_403C15)(CHanaAnimSetT3F1_S150* pAnimSet, CHanaAnimT3F0_S240* pAnim); // in: ParseODFContent.cpp
extern void(__cdecl* CHanaAnimSetT3F1__RemoveAnimT3F0_404349)(CHanaAnimSetT3F1_S150* pAnimSet, CHanaAnimT3F0_S240* pAnim); // in: PlayExploring.cpp

extern CSekItem*& g_pSekItem_4CD760; // in: PlayOdsDialogueScene.cpp

#endif

// == MACROS

#define ONCELOG(format, ...) \
  { \
    static bool _debugged = true; \
    if (_debugged) { \
      _debugged = false; \
      DebugLog(format, __VA_ARGS__); \
    } \
  }
