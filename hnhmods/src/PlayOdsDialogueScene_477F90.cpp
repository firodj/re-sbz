// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayOdsDialogueScene_477F90)(int nOdsIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x477F90);

BOOL (__cdecl*LoadOds_4843CF)(LPCSTR lpFileName, CHanaSceneT458_S504 **ppScene) =
    reinterpret_cast<BOOL(__cdecl*)(LPCSTR, CHanaSceneT458_S504 **)>(0x4843CF);
void (__cdecl*ResetSceneBodies_4857CA)(CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *)>(0x4857CA);
bool (__cdecl*UpdateSceneAtTime_4825D5)(CHanaSceneT458_S504 *pScene, float *pfExecTime, float fDelta) =
    reinterpret_cast<bool(__cdecl*)(CHanaSceneT458_S504 *, float *, float)>(0x4825D5);
void (__cdecl*UpdateSceneTimer_4827FE)(CHanaSceneT458_S504 *pScene, float *pfExecTime, float fDelta) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, float *, float)>(0x4827FE);
// ParseExecStr_487D89 lives in src/ParseExecStr_487D89.cpp (extern in shared.hpp)
void (__cdecl*PauseSceneAudio_46A2A0)(CHanaSceneT458_S504 *pScene, int bPause) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, int)>(0x46A2A0);
void (__cdecl*SetProjectionFov_40DCA3)(float fFov) =
    reinterpret_cast<void(__cdecl*)(float)>(0x40DCA3);
void (__cdecl*DrawDialogueWindow_480B2D)(CHanaSceneT458_S504 *pScene,
    float frameTimeMultiplied, int xMouse, int yMouse, WORD btnMouse) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, float, int, int, WORD)>(0x480B2D);
// Surface_AlphaBlendRect lives in src/Surface_AlphaBlendRect.cpp (extern in shared.hpp)

void (__cdecl *DrawAdvanceDialogueText_47FA84)(CHanaSceneT458_S504 *pScene, int, int, __int16)
= reinterpret_cast<void (__cdecl*)(CHanaSceneT458_S504 *pScene, int, int, __int16)>(0x47fa84);
void (__cdecl*DrawDialogueText_4803B5)(CHanaSceneT458_S504 *pScene, float fFrameTime) =
reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, float)>(0x4803B5);
void (__cdecl *CHanaObj__NewSceneT458_S504)(CHanaSceneT458_S504 **ppOut)
= reinterpret_cast<void (__cdecl*)(CHanaSceneT458_S504 **ppOut)>(0x481c5f);
void (__cdecl *CSekBody__CutToAnim_4018E1)(CSekBody *pSekBody, int nIndex)
= reinterpret_cast<void (__cdecl*)(CSekBody *pSekBody, int nIndex)>(0x4018e1);
BOOL (__cdecl *CSekBody__UpdateAnim_4019E1)(CSekBody *pSekBody, float fDelta)
= reinterpret_cast<BOOL (__cdecl*)(CSekBody *pSekBody, float fDelta)>(0x4019e1);
void (__cdecl *CHanaAnimSetT3F1__BeginBlend_43A371)(CHanaAnimSetT3F1_S150 *pAnimSet)
= reinterpret_cast<void (__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet)>(0x43a371);
bool (__cdecl *CHanaAnimSetT3F1__BlendStep_43A643)(CHanaAnimSetT3F1_S150 *pAnimSet, float *pfWeight, float fDelta)
= reinterpret_cast<bool (__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet, float *pfWeight, float fDelta)>(0x43a643);
void (__cdecl *HanaAnim_UpdateBoneTransform)(CHanaAnimT3F0_S240 *pAnim, float fWeight)
= reinterpret_cast<void (__cdecl*)(CHanaAnimT3F0_S240 *pAnim, float fWeight)>(0x43a43f);
void (__cdecl *SekBody__UpdateTo2_401647)(CSekBody *pSekBody, int nIndex)
= reinterpret_cast<void (__cdecl*)(CSekBody *pSekBody, int nIndex)>(0x401647);

void (__cdecl*Surface_AlphaBlendRect)(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY,
  LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect, int nBlendWeight, char bSkipColorKey) =
  reinterpret_cast<void(__cdecl*)(LPDIRECTDRAWSURFACE7, int, int, LPDIRECTDRAWSURFACE7,
    LPRECT, int, char)>(0x41A039);
LPVOID (__cdecl*Surface_LockAndGetPointer)(LPDIRECTDRAWSURFACE7 pSurface) =
reinterpret_cast<LPVOID(__cdecl*)(LPDIRECTDRAWSURFACE7)>(0x4196FD);


// === FUNCTIONS ===

// PlayOdsDialogueScene_477F90 -- the main story/ending dialogue scene player
// (GameMainLoop dispatches nSceneType 1 here). nOdsIndex selects from a
// 47-entry path table (built below from the strcpy/memset noise): p02/p05/
// p04/p03/p01 chapter files "pNN\pNN_sXX" (+ _h01 scenes, bare "PNN" branch
// stubs) and e00/e01/e02 endings; "<name>.ods" resolved through the ODS dir
// (1002). Load fails -> -1.
//
// Setup: antialias off; LoadOds + ResetSceneBodies post-init; mouse.bmp
// cursor; timer scale 15; projection FOV 0.5; exec-state primed by running
// UpdateSceneAtTime twice at t=1 then ParseExecStr at t=5 (x2, WCurNum=5).
//
// Main loop: menu key -> PauseSceneAudio + SystemMenu (BGM = scene's
// pSndBuf, vol offset = field_470, mode 0, protagonist HP pair from
// g_sceneState); >= 1 exits. Middle-click TOGGLES bSkipMode (skips the
// script and the dialogue window/cursor); right-click (btn 3) multiplies
// frame time x5 while held; a left-click while text is still scrolling
// (by_461 && !by_460) completes the line, and otherwise speeds the script
// x1000 for that frame. Scene advance: UpdateSceneTimer + UpdateSceneAtTime
// (0 = scene over) + ParseExecStr (0 = over). Background: while flt_480
// < 1 a crossfade from pUnk_47C toward pSurface_468 via
// Surface_AlphaBlendRect(alpha = flt_480*100, advancing /25 per frame),
// else a plain Blt; Lock/Unlock around blits (DDRAW sync quirk); D3DX
// Clear(2) with a bg / Clear(3) without; 3D pass with ALPHABLEND off
// around HanaFrame_Draw. Dialogue window + cursor drawn only when not
// skipping. Teardown restores the timer scale and frees scene + cursor.

int __cdecl hook_PlayOdsDialogueScene_477F90(int nOdsIndex)
{
    CHanaSceneT458_S504 *pHanaScene;        // [esp+238h] [ebp-400h]
    CHanaSpriteT3F6_S1FC *pCursor;          // [esp+244h] [ebp-3F4h]
    CHAR szFileName[256];                   // [esp+124h] [ebp-514h]
    CSceneState stcScene;                   // [esp-114h] [ebp-74Ch] scratch/copy
    DDSURFACEDESC2 ddsd;                    // [esp+A0h] [ebp-598h]
    RECT rc;                                // [esp+8Ch] [ebp-5ACh]
    DWORD dwAaInit, dwAaBk;
    float fExecTime, fFrameTime, fSavedScale, fSpeedMult;
    int nResult, xMouse, yMouse;
    WORD btnMouse = 0, wPrevBtn;
    BOOL bSkipMode;

    static const char *const arrszOdsFiles[47] = {
        "p02\\p02_s01", "p02\\p02_s02", "p02\\p02_s03", "p02\\p02_s04",
        "p02\\p02_s05", "P02",          "p02\\p02_s06", "p02\\p02_h01",
        "p02\\p02_s07",
        "p05\\p05_s01", "p05\\p05_s021", "p05\\p05_s022", "p05\\p05_s03",
        "P05\\p05_s04", "P05",          "p05\\p05_s05", "p05\\p05_h01",
        "p05\\p05_s06",
        "p04\\p04_s01", "p04\\p04_s02", "p04\\p04_s03", "p04\\p04_s04",
        "P04",          "P04\\p04_s05", "p04\\p04_h01", "p04\\p04_s06",
        "p03\\p03_s01", "p03\\p03_s02", "p03\\p03_s03", "p03\\p03_s04",
        "P03",          "P03\\p03_s05", "p03\\p03_s06", "p03\\p03_h01",
        "p03\\p03_s07",
        "p01\\p01_s01", "p01\\p01_s02", "p01\\p01_s03", "P01\\p01_s05",
        "P01",          "p01\\p01_s06", "p01\\p01_s07", "p01\\p01_h01",
        "p01\\p01_s08",
        "e00", "e02", "e01",
    };
    // (the original zeroes a 60-byte scratch buffer v22 here -- never used)

    ONCELOG("[HNH:SUB] PlayOdsDialogueScene_477F90\n");

    nResult = 0;
    bSkipMode = 0;
    fExecTime = 5.0f;
    pHanaScene = NULL;
    pCursor = NULL;

    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAaInit);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);

    wsprintfA(szFileName, "%s.ods", arrszOdsFiles[nOdsIndex]);
    CHanaObj__GetRealPath_486D9A(szFileName, 1002);           // 1002 = ODS dir
    if (!LoadOds_4843CF(szFileName, &pHanaScene))
        return -1;
    ResetSceneBodies_4857CA(pHanaScene);

    wsprintfA(szFileName, "mouse.bmp");                       // aMouseBmp_1 @0x4C6C7C
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);           // 1003 = SPR dir
    LoadBmpWithMask_413A5E(&pCursor, szFileName, szFileName);
    RestartInputThread(2);

    fSavedScale = (float)GetFrameTime();
    SetTimerScaleFactor(15.0f);
    SetProjectionFov_40DCA3(0.5f);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();

    // prime the exec state: two ticks at t=1, then two script passes at t=5
    fExecTime = 1.0f;
    UpdateSceneAtTime_4825D5(pHanaScene, &fExecTime, 1.0f);
    UpdateSceneAtTime_4825D5(pHanaScene, &fExecTime, 1.0f);
    ParseExecStr_487D89(pHanaScene, &fExecTime);
    fExecTime = 5.0f;
    pHanaScene->WCurNum = 5.0f;
    ParseExecStr_487D89(pHanaScene, &fExecTime);
    fExecTime = 5.0f;
    pHanaScene->WCurNum = 5.0f;
    ParseExecStr_487D89(pHanaScene, &fExecTime);

    bSkipMode = 0;
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAaInit);

    while (1)
    {
        DoPeekMessages();
        fFrameTime = (float)GetFrameTime();
        if (fFrameTime > 5.0f)              // clamp (right-click adds x5 below)
            fFrameTime = 5.0f;
        FrameTimer_EndFrame();

        // menu key -> pause into the system menu
        if (IsMenuKeyPressed_481170())
        {
            PauseSceneAudio_46A2A0(pHanaScene, 1);
            RestartInputThread(0);
            memcpy(&stcScene, &g_sceneState_52F7E0, sizeof(stcScene));
            nResult = SystemMenu_4655AB(pHanaScene->pSndBuf_46C,
                                        pHanaScene->field_470, 0,
                                        g_sceneState_52F7E0.protCurrentHP_C0,
                                        g_sceneState_52F7E0.protMaximumHP_C4,
                                        stcScene);
            RestartInputThread(2);
            PauseSceneAudio_46A2A0(pHanaScene, 0);
            if (nResult >= 1)
                break;
        }

        wPrevBtn = btnMouse;
        GetMouseBtn_45A41B(&btnMouse);
        GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);
        if (btnMouse == 2 && !wPrevBtn)     // middle: toggle skip
            bSkipMode = !bSkipMode;
        if (btnMouse == 3 && !bSkipMode)    // right held: fast-forward x5
            fFrameTime = fFrameTime * 5.0f;

        if (pHanaScene)
        {
            UpdateSceneTimer_4827FE(pHanaScene, &fExecTime, fFrameTime);
            if (!UpdateSceneAtTime_4825D5(pHanaScene, &fExecTime, fFrameTime))
                break;                      // scene finished
        }

        fSpeedMult = 1.0f;
        if (!bSkipMode)
        {
            if (!ParseExecStr_487D89(pHanaScene, &fExecTime))
                break;                      // script finished
            // click while a line is still rolling -> complete it instantly
            if ((btnMouse == 3 || btnMouse == 1 && btnMouse != wPrevBtn)
                && pHanaScene->by_461 && !pHanaScene->by_460)
                pHanaScene->by_461 = 0;
            if (btnMouse == 1 && btnMouse != wPrevBtn)
                fSpeedMult = 1000.0f;       // click: rush the script this frame
        }

        MtxStackStartMultLocal_4074EF();
        if (pHanaScene && pHanaScene->pSurface_468)
        {
            ddsd.dwSize = 124;
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAaBk);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            Sleep(1);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            if (pHanaScene->flt_480 < 1.0f && pHanaScene->pUnk_47C)
            {
                // crossfade pUnk_47C -> pSurface_468 (alpha = flt_480*100)
                SetRect(&rc, 0, 0, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
                g_pBackBuffer_4D6C90->Lock(NULL, &ddsd, DDLOCK_WAIT, 0);
                g_pBackBuffer_4D6C90->Unlock(NULL);
                g_pBackBuffer_4D6C90->Blt(NULL, (LPDIRECTDRAWSURFACE7)pHanaScene->pUnk_47C,
                                          NULL, DDBLT_WAIT, NULL);
                if (pHanaScene->flt_480 != 0.0f)
                    Surface_AlphaBlendRect(g_pBackBuffer_4D6C90, 0, 0,
                                           pHanaScene->pSurface_468, &rc,
                                           (int)(pHanaScene->flt_480 * 100.0f), 0);
                pHanaScene->flt_480 = fFrameTime / 25.0f + pHanaScene->flt_480;
                g_pBackBuffer_4D6C90->Lock(NULL, &ddsd, DDLOCK_WAIT, 0);
                g_pBackBuffer_4D6C90->Unlock(NULL);
            }
            else
            {
                pHanaScene->flt_480 = 1.0f;
                g_pBackBuffer_4D6C90->Lock(NULL, &ddsd, DDLOCK_WAIT, 0);
                g_pBackBuffer_4D6C90->Unlock(NULL);
                g_pBackBuffer_4D6C90->Blt(NULL, pHanaScene->pSurface_468, NULL, DDBLT_WAIT, NULL);
                g_pBackBuffer_4D6C90->Lock(NULL, &ddsd, DDLOCK_WAIT, 0);
                g_pBackBuffer_4D6C90->Unlock(NULL);
            }
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAaBk);
            Sleep(1);
            g_pRenderCtx->Clear(2);         // z-buffer only (bg already drawn)
        }
        else
        {
            g_pRenderCtx->Clear(3);         // z + target
        }

        // 3D pass with alpha blending off
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
            HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            HanaVert_DrawBatch(1);
            g_pD3DDevice_4D6C78->EndScene();
        }

        if (!bSkipMode)
        {
            stcScene.frameTimeMultiplied_180 = fFrameTime * fSpeedMult;
            DrawDialogueWindow_480B2D(pHanaScene, stcScene.frameTimeMultiplied_180,
                                      xMouse, yMouse, btnMouse);
        }

        // cursor on top
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
            if (!bSkipMode && pCursor)
            {
                CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(xMouse + 16), (float)(yMouse + 16));
                DrawSprite2D(pCursor);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        //nop_4863C3();
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    SetTimerScaleFactor(fSavedScale);
    RestartInputThread(0);
    if (pHanaScene)
    {
        CHanaObj__Free_40C1E4(pHanaScene);
        pHanaScene = NULL;
    }
    if (pCursor)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130*)pCursor);
        pCursor = NULL;
    }
    return nResult;
}

BOOL __cdecl hook_LoadOds_4843CF(LPCSTR lpFileName, CHanaSceneT458_S504 **ppScene)
{
  int name_len_v3; // eax
  CHanaSceneT458_S504 *pScene; // [esp-4h] [ebp-214h]
  HANDLE hFile; // [esp+0h] [ebp-210h]
  CHAR String1[512]; // [esp+4h] [ebp-20Ch] BYREF
  DWORD nNumberOfBytesToRead; // [esp+204h] [ebp-Ch]
  LPVOID lpBuffer; // [esp+208h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+20Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] LoadOds_4843CF\n");

  lpBuffer = 0;
  CHanaObj__NewSceneT458_S504(ppScene);
  AttachHanaObj_40BAB8((*ppScene)->pAnimSet_148, lpFileName);
  hFile = CreateFileA(lpFileName, 0x80000000, 0, 0, 3u, 0, 0);
  NumberOfBytesRead = 0;
  if ( !hFile )
    return 0;
  if ( hFile == (HANDLE)-1 )
    return 0;
  nNumberOfBytesToRead = GetFileSize(hFile, 0);
  if ( !nNumberOfBytesToRead )
    return 0;
  if ( nNumberOfBytesToRead == -1 )
    return 0;
  lpBuffer = MemAlloc_40DEF7(nNumberOfBytesToRead + 128);
  wsprintfA(String1, "êVÉtÉHÅ[É}ÉbÉgÇnÇcÇr%c%c%c", 13, 10, 0);
  ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, 0);
  CloseHandle(hFile);
  if ( !lstrcmpA(String1, (LPCSTR)lpBuffer) )
  {
    pScene = *ppScene;
    name_len_v3 = lstrlenA(String1);
    ParseODS_483932((LPBYTE)lpBuffer + name_len_v3 + 1, pScene);
  }
  if ( lpBuffer )
  {
    MemFree_40DF64(lpBuffer);
    lpBuffer = 0;
  }
  if ( *ppScene )
    ResetSceneBodies_4857CA(*ppScene);
  MtxStackStartMultLocal_4074EF();
  return 1;
}

void __cdecl hook_CHanaObj__NewSceneT458_S504(CHanaSceneT458_S504 **ppOut)
{
  LPDIRECTDRAWSURFACE7 pSurface; // [esp+4h] [ebp-A20h]
  int k; // [esp+Ch] [ebp-A18h]
  int i; // [esp+10h] [ebp-A14h]
  int j; // [esp+10h] [ebp-A14h]
  int m; // [esp+10h] [ebp-A14h]
  CHAR szWindow1bmp[10][256] = {
    "window1.bmp",  "window2.bmp",  "window3.bmp",  "window4.bmp",  "window5.bmp",  "window6.bmp",  "window7.bmp"
  }; // [esp+14h] [ebp-A10h] BYREF
  //int v7; // [esp+A14h] [ebp-10h]
  LPDIRECTDRAWSURFACE7 pTexture; // [esp+A18h] [ebp-Ch] BYREF
  LPDIRECTDRAWSURFACE7 pSurfaceMask; // [esp+A1Ch] [ebp-8h]
  //int v10; // [esp+A20h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__NewSceneT458_S504_481C5F\n");

  //v10 = 0;
  pSurfaceMask = 0;
  pTexture = 0;
  //v7 = 0;
  *ppOut = (CHanaSceneT458_S504 *)MemAlloc_40DEF7(0x504u);
  (*ppOut)->dwType = 0x458;
  CHanaObj__AddRef_40BFC3(*ppOut);
  (*ppOut)->dwCount_130 = 0;
  CHanaObj__NewAnimSetT3F1_S150(&(*ppOut)->pAnimSet_148);
 

  for ( i = 0; i < 10; ++i )
    CHanaObj__GetRealPath_486D9A(szWindow1bmp[i], 1003);
  for ( j = 0; j < 5; ++j )
    (*ppOut)->pSprWindowBmp[j] = 0;
  LoadBmpWithMask_413A5E((*ppOut)->pSprWindowBmp, szWindow1bmp[1], szWindow1bmp[0]);
  LoadBmpWithMask_413A5E(&(*ppOut)->pSprWindowBmp[1], szWindow1bmp[2], szWindow1bmp[2]);
  LoadBmpWithMask_413A5E(&(*ppOut)->pSprWindowBmp[2], szWindow1bmp[3], szWindow1bmp[3]);
  LoadBmpWithMask_413A5E(&(*ppOut)->pSprWindowBmp[3], szWindow1bmp[4], szWindow1bmp[5]);
  LoadBmpWithMask_413A5E(&(*ppOut)->pSprWindowBmp[4], szWindow1bmp[6], szWindow1bmp[6]);
  (*ppOut)->flt_450 = 1.0;
  (*ppOut)->flt_454 = 1.0;
  pSurface = CreateSurfaceWH(0x20u, 0x20u);
  pSurfaceMask = CreateSurfaceWH(0x20u, 0x20u);
  if ( pSurface && pSurfaceMask )
  {
    BltSurface(pSurface, 0, 0, 32, 32, 0);
    BltSurface(pSurfaceMask, 0, 0, 32, 32, 0);
    for ( k = 0; k < 4; ++k )
    {
      for ( m = 0; m < 27; ++m )
      {
        CHanaObj__NewSpriteT3F6bySurf(&(*ppOut)->textSprites_264[k][m], pSurface);
        if ( (*ppOut)->textSprites_264[k][m] )
        {
          if ( (*ppOut)->textSprites_264[k][m]->pTexture_134 )
          {
            (*ppOut)->textSprites_264[k][m]->pTexture_134->Release();
            (*ppOut)->textSprites_264[k][m]->pTexture_134 = 0;
          }
          pTexture = CreateTextureFromSurfaceWithAlphaMask(pSurface, pSurfaceMask);
          CHanaObj__ReplaceTexture3F6((*ppOut)->textSprites_264[k][m], &pTexture);
          CHanaObj__SetAlpha3F6((*ppOut)->textSprites_264[k][m], 0.0);
        }
      }
    }
    pSurface->Release();
    pSurfaceMask->Release();
  }
  (*ppOut)->by_460 = 0;
  (*ppOut)->WCurNum = 0.0;
  (*ppOut)->flt_464 = 0.0;
}

// Post-load body init: for each CHanaScene body (refsSekBody_13C[i], guarded by arrSizes_144[i]) cut it to its designated start SEK item (arrSekItemProps_140[i].index), clear the pending blend weights (float_21C/float_224 = 0), and step one frame via UpdateAnim(1.0). Establishes the neutral 'rest' pose before playback.
void __cdecl hook_ResetSceneBodies_4857CA(CHanaSceneT458_S504 *pScene)
{
  unsigned int i; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] ResetSceneBodies_4857CA\n");
  if ( pScene && pScene->arrSizes_144 )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->arrSizes_144[i] )
      {
        CSekBody__CutToAnim_4018E1(pScene->refsSekBody_13C[i], pScene->arrSekItemProps_140[i]->index);
        pScene->refsSekBody_13C[i]->float_21C = 0.0;
        pScene->refsSekBody_13C[i]->float_224 = 0.0;
        CSekBody__CutToAnim_4018E1(pScene->refsSekBody_13C[i], pScene->arrSekItemProps_140[i]->index);
        CSekBody__UpdateAnim_4019E1(pScene->refsSekBody_13C[i], 1.0);
      }
    }
  }
}

// CSekBody::CutToAnim(pSekBody, nItemIndex) - hard-cut (NO blend) switch to animation item nItemIndex.
// Same item prep as PlayAnim_40179D (set field_200, reset field_44/6C/64, recompute float_5C speed,
// start at float_54) but forces float_21C = float_224 = 0.0 so there is no crossfade. This is the
// target UpdateAnim_4019E1 chains to when the current item's field_74 (next-item) transition fires.
void __cdecl hook_CSekBody__CutToAnim_4018E1(CSekBody *pSekBody, int nItemIndex)
{
  ONCELOG("[HNH:SUB] CSekBody__CutToAnim_4018E1\n");
  pSekBody->field_200 = nItemIndex;
  pSekBody->items_250[nItemIndex].field_44 = 0;
  pSekBody->items_250[nItemIndex].field_6C = 0;
  pSekBody->items_250[nItemIndex].field_64 = 0;
  pSekBody->items_250[nItemIndex].float_5C = (pSekBody->items_250[nItemIndex].float_58
    - pSekBody->items_250[nItemIndex].float_54)
    / (double)pSekBody->items_250[nItemIndex].field_50;
  pSekBody->items_250[nItemIndex].float_60 = pSekBody->items_250[nItemIndex].float_54;
  pSekBody->field_244 = 0;
  pSekBody->field_248 = 0;
  pSekBody->float_21C = 0.0;
  pSekBody->float_224 = 0.0;
}

CSekItem * &g_pSekItem_4CD760 = *(CSekItem * *)0x4CD760; // WRITABLE
uint32_t &g_nSekStep_4CD764 = *(uint32_t *)0x4CD764; // WRITABLE

// CSekBody::UpdateAnim(pSekBody, fDelta) - per-frame animation state machine (returns 1 when the
// current item reaches its end this frame, else 0). No-op if pSekBody/field_24C absent, or the header's
// anim set (pAnimSetT3F1) is missing. Selects the active item g_pSekItem = &items_250[field_200] and
// advances its playback time: float_60 += fDelta * float_5C. On reaching the end (float_58<=float_60):
//  flag_0==0 (one-shot) -> either snap to end (field_70 case) or crossfade to the next item via
//  sub_4018E1(body, field_74); flag_0==1 (looping) -> ++field_64 loop counter, restart at float_54 while
//  under the loop limit (gap78), else crossfade. Then, if a blend is pending (float_21C!=0) flush it
//  (sub_43A371) and set blend factor float_224=1; applies the pose with sub_403D61(animSet, float_60) and
//  advances the blend via sub_43A643(animSet, &float_224, fDelta). Callers use the ==1 return to call
//  PlayAnim_40179D.
BOOL __cdecl hook_CSekBody__UpdateAnim_4019E1(CSekBody *pSekBody, float fDelta)
{
  BOOL bCompleted; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] CSekBody__UpdateAnim_4019E1\n");

  bCompleted = 0;
  if ( !pSekBody || pSekBody->field_24C || !pSekBody->pSekHeader_220 || !pSekBody->pSekHeader_220->pAnimSetT3F1 )
    return 0;
  g_pSekItem_4CD760 = &pSekBody->items_250[pSekBody->field_200];
  g_pSekItem_4CD760->float_60 = fDelta * g_pSekItem_4CD760->float_5C + g_pSekItem_4CD760->float_60;
  if ( g_pSekItem_4CD760->float_58 <= (double)g_pSekItem_4CD760->float_60 )
  {
    bCompleted = 1;
    if ( !g_pSekItem_4CD760->flag_0 )
    {
      if ( !g_pSekItem_4CD760->field_70 )
      {
        g_pSekItem_4CD760->field_44 = g_pSekItem_4CD760->field_50;
        g_nSekStep_4CD764 = g_pSekItem_4CD760->field_50 - 1;
        pSekBody->field_244 = 1;
        g_pSekItem_4CD760->float_60 = g_pSekItem_4CD760->float_58;
        goto LABEL_15;
      }
    LABEL_11:
      CSekBody__CutToAnim_4018E1(pSekBody, g_pSekItem_4CD760->field_74);
      pSekBody->float_21C = 0.0;
      goto LABEL_15;
    }
    if ( g_pSekItem_4CD760->flag_0 == 1 )
    {
      ++g_pSekItem_4CD760->field_64;
      if ( !g_pSekItem_4CD760->field_70 || g_pSekItem_4CD760->field_64 < g_pSekItem_4CD760->gap78 )
      {
        g_nSekStep_4CD764 = g_pSekItem_4CD760->field_48;
        g_pSekItem_4CD760->field_44 = g_nSekStep_4CD764;
        pSekBody->field_244 = 1;
        pSekBody->field_248 = 1;
        g_pSekItem_4CD760->float_60 = g_pSekItem_4CD760->float_54;
        goto LABEL_15;
      }
      goto LABEL_11;
    }
  }
LABEL_15:
  if ( pSekBody->pSekHeader_220->pAnimSetT3F1 )
  {
    if ( pSekBody->float_21C != 0.0 )
    {
      CHanaAnimSetT3F1__BeginBlend_43A371(pSekBody->pSekHeader_220->pAnimSetT3F1);
      pSekBody->float_21C = 0.0;
      pSekBody->float_224 = 1.0;
    }
    CHanaAnimSetT3F1__SetTime_403D61(pSekBody->pSekHeader_220->pAnimSetT3F1, g_pSekItem_4CD760->float_60);
    CHanaAnimSetT3F1__BlendStep_43A643(pSekBody->pSekHeader_220->pAnimSetT3F1, &pSekBody->float_224, fDelta);
  }
  return bCompleted;
}

// CHanaAnimSetT3F1::BeginBlend(pAnimSet) - capture the target pose so a crossfade can start.
// For each sub-anim ref (nAnimCount_134) with nCount_138 and a valid pHanaFram_130: set float_23C = 1.0
// and memcpy objS78_1C4 <- objS78_14C (snap the target pose into the blend-source buffer); otherwise set
// float_23C = 0.0. Called by UpdateAnim_4019E1 when a blend weight (float_21C) is pending, just before
// BlendStep_43A643 starts interpolating. Returns the last index+1 (unused).
void __cdecl hook_CHanaAnimSetT3F1__BeginBlend_43A371(CHanaAnimSetT3F1_S150 *pAnimSet)
{
  int i; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] CHanaAnimSetT3F1__BeginBlend_43A371\n");
  for ( i = 0; i < pAnimSet->nAnimCount_134; ++i )
  {
    if ( pAnimSet->refsAnimT3F0_138[i]->nKeyCount_138 )
    {
      if ( pAnimSet->refsAnimT3F0_138[i]->pHanaFram_130 )
      {
        pAnimSet->refsAnimT3F0_138[i]->float_23C = 1.0;
        memcpy(
          &pAnimSet->refsAnimT3F0_138[i]->keyPrev_1C4,
          &pAnimSet->refsAnimT3F0_138[i]->keyCur_14C,
          sizeof(pAnimSet->refsAnimT3F0_138[i]->keyPrev_1C4));
      }
      else
      {
        pAnimSet->refsAnimT3F0_138[i]->float_23C = 0.0;
      }
    }
  }
}

// CHanaAnimSetT3F1::BlendStep(pAnimSet, pfWeight, fDelta) - advance one crossfade step.
// Decays the blend weight *pfWeight -= 0.1*fDelta (clamped to [0,1]). While *pfWeight > 0 it interpolates
// every sub-anim ref's pose toward its target via sub_43A43F(ref, *pfWeight) and returns 1 (still blending);
// when the weight hits 0 it returns 0 (blend complete). pfWeight is pSekBody->float_224 in UpdateAnim_4019E1.
bool __cdecl hook_CHanaAnimSetT3F1__BlendStep_43A643(CHanaAnimSetT3F1_S150 *pAnimSet, float *pfWeight, float fDelta)
{
  int i; // [esp+0h] [ebp-8h]
  ONCELOG("[HNH:SUB] CHanaAnimSetT3F1__BlendStep_43A643\n");
  *pfWeight = *pfWeight - 0.1 * fDelta;
  if ( *pfWeight > 0.0 )
  {
    if ( *pfWeight >= 1.0 )
      *pfWeight = 1.0;
    for ( i = 0; i < pAnimSet->nAnimCount_134; ++i )
      HanaAnim_UpdateBoneTransform(pAnimSet->refsAnimT3F0_138[i], *pfWeight);
    return 1;
  }
  else
  {
    *pfWeight = 0.0;
    return 0;
  }
}

void __cdecl hook_HanaAnim_UpdateBoneTransform(CHanaAnimT3F0_S240 *pAnim, float fWeight)
{
  float startX; // [esp+8h] [ebp-8Ch]
  float startY; // [esp+Ch] [ebp-88h]
  float startZ; // [esp+10h] [ebp-84h]
  float endX; // [esp+20h] [ebp-74h]
  float endY; // [esp+24h] [ebp-70h]
  float endZ; // [esp+28h] [ebp-6Ch]
  D3DXQUATERNION resultQuat; // [esp+38h] [ebp-5Ch] BYREF
  float resultX; // [esp+48h] [ebp-4Ch]
  float resultY; // [esp+4Ch] [ebp-48h]
  float resultZ; // [esp+50h] [ebp-44h]
  ONCELOG("[HNH:SUB] HanaAnim_UpdateBoneTransform_43A43F\n");
  pAnim->pHanaFram_130->pHanaAnim_2C0 = pAnim;
  D3DXQuaternionSlerp(&resultQuat, &pAnim->keyCur_14C.quatRot, &pAnim->keyPrev_1C4.quatRot, fWeight);
  resultX = pAnim->keyCur_14C.fPosX;
  resultY = pAnim->keyCur_14C.fPosY;
  resultZ = pAnim->keyCur_14C.fPosZ;
  startX = (pAnim->keyPrev_1C4.fPosX - resultX) * fWeight + resultX;
  startY = (pAnim->keyPrev_1C4.fPosY - resultY) * fWeight + resultY;
  startZ = (pAnim->keyPrev_1C4.fPosZ - resultZ) * fWeight + resultZ;
  resultX = pAnim->keyCur_14C.fScaleX;
  resultY = pAnim->keyCur_14C.fScaleY;
  resultZ = pAnim->keyCur_14C.fScaleZ;
  endX = (pAnim->keyPrev_1C4.fScaleX - resultX) * fWeight + resultX;
  endY = (pAnim->keyPrev_1C4.fScaleY - resultY) * fWeight + resultY;
  endZ = (pAnim->keyPrev_1C4.fScaleZ - resultZ) * fWeight + resultZ;
  ApplyQTS_4035B1(&pAnim->pHanaFram_130->matLocal_138, &resultQuat, startX, startY, startZ, endX, endY, endZ);
  pAnim->keyCur_14C.fPosX = startX;
  pAnim->keyCur_14C.fPosY = startY;
  pAnim->keyCur_14C.fPosZ = startZ;
  pAnim->keyCur_14C.fScaleX = endX;
  pAnim->keyCur_14C.fScaleY = endY;
  pAnim->keyCur_14C.fScaleZ = endZ;
  pAnim->keyCur_14C.quatRot = resultQuat;
  qmemcpy(&pAnim->keyCur_14C.matLocal, &pAnim->pHanaFram_130->matLocal_138, sizeof(pAnim->keyCur_14C.matLocal));
}

// Drive the whole scene to a script time. pfExecTime is clamped to the anim-set length (pAnimSet_148->fRes_13C) and to WCurNum; bContinue=0 when the end is reached. Calls SetTime on the master anim set, updates the camera look-at (CalculateLocalRotationMatrix + UpdateBoneLookAtTransform + MtxMult) when by_474/pCamFrame_478 are set, then per body finds the first item prop whose start (flt_0) < fClampedTime and CutTo it (UpdateTo2_401647), advancing with UpdateAnim(fDelta). Returns bContinue.
bool __cdecl hook_UpdateSceneAtTime_4825D5(CHanaSceneT458_S504 *pScene, float *pfExecTime, float fDelta)
{
  float dirUpZ; // [esp+0h] [ebp-28h] BYREF
  float dirForwardZ; // [esp+4h] [ebp-24h] BYREF
  float dirUpY; // [esp+8h] [ebp-20h] BYREF
  float dirForwardY; // [esp+Ch] [ebp-1Ch] BYREF
  float dirUpX; // [esp+10h] [ebp-18h] BYREF
  float dirForwardX; // [esp+14h] [ebp-14h] BYREF
  signed int j; // [esp+18h] [ebp-10h]
  unsigned int i; // [esp+1Ch] [ebp-Ch]
  float t2; // [esp+20h] [ebp-8h]
  bool bContinue; // [esp+24h] [ebp-4h]

  ONCELOG("[HNH:SUB] UpdateSceneAtTime_4825D5\n");

  bContinue = 1;
  t2 = *pfExecTime;
  if ( *pfExecTime >= (double)pScene->pAnimSet_148->fRes_13C )
  {
    *pfExecTime = pScene->pAnimSet_148->fRes_13C;
    bContinue = 0;
  }
  if ( *pfExecTime >= (double)pScene->WCurNum )
    t2 = pScene->WCurNum;
  CHanaAnimSetT3F1__SetTime_403D61(pScene->pAnimSet_148, t2);
  if ( pScene->by_474 && pScene->pCamFrame_478 )
  {
    CalculateLocalRotationMatrix(
      pScene->pCamFrame_478,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      &dirForwardX,
      &dirForwardY,
      &dirForwardZ,
      &dirUpX,
      &dirUpY,
      &dirUpZ);
    UpdateBoneLookAtTransform(
      g_pCameraFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      dirForwardX,
      dirForwardY,
      dirForwardZ,
      dirUpX,
      dirUpY,
      dirUpZ);
    HanaFrame_GetRelPosition(pScene->pCamFrame_478, g_pSceneFrame, &dirForwardX, &dirForwardY, &dirForwardZ);
    SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, dirForwardX, dirForwardY, dirForwardZ);
  }
  if ( pScene->arrSizes_144 )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      for ( j = pScene->arrSizes_144[i] - 1; j >= 0; --j )
      {
        if ( pScene->arrSekItemProps_140[i][j].flt_0 < (double)t2 )
        {
          SekBody__UpdateTo2_401647(pScene->refsSekBody_13C[i], pScene->arrSekItemProps_140[i][j].index);
          break;
        }
      }
      if ( pScene->refsSekBody_13C[i] )
        CSekBody__UpdateAnim_4019E1(pScene->refsSekBody_13C[i], fDelta);
    }
  }
  return bContinue;
}

void __cdecl hook_SekBody__UpdateTo2_401647(CSekBody *pSekBody, int nIndex)
{
  ONCELOG("[HNH:SUB] SekBody__UpdateTo2_401647\n");

  if ( pSekBody->field_208 != nIndex )
  {
    if ( !pSekBody->items_250[nIndex].field_7C )
      pSekBody->float_21C = 1.0;
    pSekBody->field_200 = nIndex;
    pSekBody->field_208 = nIndex;
    pSekBody->items_250[nIndex].field_44 = 0;
    pSekBody->items_250[nIndex].field_6C = 0;
    pSekBody->items_250[nIndex].field_64 = 0;
    if ( pSekBody->items_250[nIndex].field_50 )
      pSekBody->items_250[nIndex].float_5C = (pSekBody->items_250[nIndex].float_58 - pSekBody->items_250[nIndex].float_54)
      / (double)pSekBody->items_250[nIndex].field_50;
    else
      pSekBody->items_250[nIndex].float_5C = 0.0;
    pSekBody->items_250[nIndex].float_60 = pSekBody->items_250[nIndex].float_54;
    pSekBody->field_244 = 0;
    pSekBody->field_248 = 0;
  }
}

// Scene fade/timer step. flt_454 accumulates a 0..1 fade envelope (flt_454 = fDelta*flt_458 + flt_454, clamped to [0,1] and re-arming flt_458=0 at the limits). When by_461[0] is set, flt_450 ramps a 0..1 timer at fDelta/10s. pfExecTime is advanced by fDelta. Called every frame in the ODS main loop.
void __cdecl hook_UpdateSceneTimer_4827FE(CHanaSceneT458_S504 *pScene, float *pfExecTime, float fDelta)
{
  ONCELOG("[HNH:SUB] UpdateSceneTimer_4827FE\n");
  pScene->flt_454 = fDelta * pScene->flt_458 + pScene->flt_454;
  if ( pScene->flt_454 > 1.0 )
  {
    pScene->flt_454 = 1.0;
    pScene->flt_458 = 0.0;
  }
  if ( pScene->flt_454 < 0.0 )
  {
    pScene->flt_454 = 0.0;
    pScene->flt_458 = 0.0;
  }
  if ( pScene->by_461 )
  {
    pScene->flt_450 = fDelta / 10.0 + pScene->flt_450;
    if ( pScene->flt_450 > 1.0 )
      pScene->flt_450 = 1.0;
  }
  else
  {
    pScene->flt_450 = pScene->flt_450 - fDelta / 10.0;
    if ( pScene->flt_450 < 0.0 )
      pScene->flt_450 = 0.0;
  }
  *pfExecTime = fDelta + *pfExecTime;
}

// Pause/resume the scene's DirectSound. bPause=1: stop every anim-set sound (pAnimSet_148->arroS118_14C[i].pSnd_0->pDSBuffer_0->lpVtbl->Stop) and mark field_110=9999 (skipped). bPause=0: reset field_110=0 across all refsAnimeSets_138 so playback can restart.
void __cdecl hook_PauseSceneAudio_46A2A0(CHanaSceneT458_S504 *pScene, int bPause)
{
  unsigned int j; // [esp+0h] [ebp-8h]
  unsigned int i; // [esp+4h] [ebp-4h]
  unsigned int k; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] PauseSceneAudio_46A2A0\n");

  if ( pScene )
  {
    if ( bPause == 1 )
    {
      for ( i = 0; i < pScene->pAnimSet_148->nCount_148; ++i )
      {
        if ( pScene->pAnimSet_148->arroS118_14C[i].pSnd_0 )
        {
          pScene->pAnimSet_148->arroS118_14C[i].pSnd_0->pDSBuffer_0->Stop();
          pScene->pAnimSet_148->arroS118_14C[i].field_110 = 9999;
        }
      }
    }
    else
    {
      for ( j = 0; j < pScene->dwCount_130; ++j )
      {
        for ( k = 0; k < pScene->refsAnimeSets_138[j]->nCount_148; ++k )
          pScene->refsAnimeSets_138[j]->arroS118_14C[k].field_110 = 0;
      }
    }
  }
}

// Set the D3D projection FOV. Stores fFov in global flt_4B00C4, rebuilds g_mat4x4Projection_4CE870 via MatrixPerspectiveFovLH, and applies it as D3DTS_PROJECTION. Called at battle start (1.2) and during transitions (0.5).
void __cdecl hook_SetProjectionFov_40DCA3(float fFov)
{
  ONCELOG("[HNH:SUB] SetProjectionFov_40DCA3\n");
 

  g_fProjFov = fFov;
  MatrixPerspectiveFovLH(&g_mat4x4Projection_4CE870, fFov, g_fProjAspect, g_fProjZNear, g_fProjZFar);
  g_pD3DDevice_4D6C78->SetTransform(
    D3DTRANSFORMSTATE_PROJECTION,
    (LPD3DMATRIX)&g_mat4x4Projection_4CE870);
  BuildViewportMatrix();
}

uint32_t &g_fFrameTime_52F1B4 = *(uint32_t *)0x52F1B4; // WRITABLE

// DrawDialogueWindow(pScene, fFrameTime, xMouse, yMouse, wBtnMouse)
// Dialogue-window renderer. pScene=CHanaSceneT458_S504*. Scales fFrameTime by the speed index
// (byte_530267) and accumulates the text scroll offset flt_52F1B4; right-click fast-forwards.
// Static mode (by_463==1, flt_454<=0): fades in pSprWindow6bmp (frame) and dispatches text
// via sub_47FA84(pScene,x,y,wBtn). Scrolling mode: fades pSprWindow6bmp out, draws
// pSprWindow2bmp (text tape) in 3 offset passes, then DrawDialogueText_4803B5(pScene,fFrameTime).
// Called from Scene1Dialog_477F90 / Scene2_45BCAE / Scene4_4703AE.
void __cdecl hook_DrawDialogueWindow_480B2D(
  CHanaSceneT458_S504 *pScene,
  float fDeltaTime,
  int xMouse,
  int yMouse,
  WORD btnMouse)
{
  ONCELOG("[HNH:SUB] DrawDialogueWindow_480B2D\n");

  double fAlphaCur; // st7
  int nScrollTop; // eax
  int nWin1L; // eax
  int nWin2L; // eax
  int nWin1T; // [esp+60h] [ebp-64h]
  int nWin2T; // [esp+60h] [ebp-64h]
  int nScrollR; // [esp+64h] [ebp-60h]
  int nWin1R; // [esp+64h] [ebp-60h]
  int nWin2R; // [esp+64h] [ebp-60h]
  float fFadeOutAlpha; // [esp+68h] [ebp-5Ch]
  float fTapeAlpha; // [esp+68h] [ebp-5Ch]
  int nScrollB; // [esp+68h] [ebp-5Ch]
  float fFadeInAlpha; // [esp+68h] [ebp-5Ch]
  int nWin1B; // [esp+68h] [ebp-5Ch]
  int nWin2B; // [esp+68h] [ebp-5Ch]
  float fSavedAlpha; // [esp+6Ch] [ebp-58h]
  float arrTextSpeedMult[10] = { 1.0, 2.5, 5.0 }; // [esp+70h] [ebp-54h] BYREF
  DWORD nSurfH; // [esp+98h] [ebp-2Ch] BYREF
  RECT rcDest; // [esp+9Ch] [ebp-28h] BYREF
  DWORD nSurfW; // [esp+ACh] [ebp-18h] BYREF
  RECT rcSrc; // [esp+B0h] [ebp-14h] BYREF
  float fFrameTime; // [esp+C0h] [ebp-4h]

  fFrameTime = fDeltaTime;
  if ( pScene )
  {
    fFrameTime = fFrameTime * arrTextSpeedMult[(unsigned __int8)g_nTextSpeed_530267];
    g_fFrameTime_52F1B4 = g_fFrameTime_52F1B4 + fFrameTime;
    if ( btnMouse == 3 && pScene->by_460 )
      fFrameTime = 1000.0 * fDeltaTime;
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);
    if ( pScene->flt_454 <= 0.0 )
    {
      if ( pScene->by_463 == 1 && g_pD3DDevice_4D6C78->BeginScene() >= 0 )
      {
        if ( pScene->pSprWindowBmp[3] )
        {
          fSavedAlpha = CHanaSpriteT3F6__GetAlpha_414AFB(pScene->pSprWindowBmp[3]);
          fFadeInAlpha = fFrameTime * 0.050000001 + fSavedAlpha;
          CHanaObj__SetAlpha3F6(pScene->pSprWindowBmp[3], fFadeInAlpha);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
          GetSurfaceWH(pScene->pSprWindowBmp[3]->pTexture_134, &nSurfW, &nSurfH);
          SetRect(&rcSrc, 1, 1, nSurfW - 1, nSurfH - 1);
          nWin1B = ConvertFrom640AsInt(378);
          nWin1R = ConvertFrom640AsInt(320);
          nWin1T = ConvertFrom640AsInt(250);
          nWin1L = ConvertFrom640AsInt(64);
          SetRect(&rcDest, nWin1L, nWin1T, nWin1R, nWin1B);
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->pSprWindowBmp[3], &rcSrc);
          DrawSpriteLTRB(pScene->pSprWindowBmp[3], rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
          SetRect(&rcSrc, nSurfW - 1, 1, 1, nSurfH - 1);
          nWin2B = ConvertFrom640AsInt(378);
          nWin2R = ConvertFrom640AsInt(576);
          nWin2T = ConvertFrom640AsInt(250);
          nWin2L = ConvertFrom640AsInt(320);
          SetRect(&rcDest, nWin2L, nWin2T, nWin2R, nWin2B);
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->pSprWindowBmp[3], &rcSrc);
          DrawSpriteLTRB(pScene->pSprWindowBmp[3], rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
          DrawAdvanceDialogueText_47FA84(pScene, xMouse, yMouse, btnMouse);
        }
        g_pD3DDevice_4D6C78->EndScene();
      }
    }
    else
    {
      fAlphaCur = CHanaSpriteT3F6__GetAlpha_414AFB(pScene->pSprWindowBmp[3]);
      fFadeOutAlpha = fAlphaCur - fFrameTime * 0.1;
      CHanaObj__SetAlpha3F6(pScene->pSprWindowBmp[3], fFadeOutAlpha);
      if ( pScene->pSprWindowBmp[0] )
      {
        if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
        {
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
          fTapeAlpha = pScene->flt_454 * 0.40000001;
          CHanaObj__SetAlpha3F6(pScene->pSprWindowBmp[0], fTapeAlpha);
          nScrollB = ConvertFrom640AsInt(480);
          nScrollR = ConvertFrom640AsInt(640);
          nScrollTop = ConvertFrom640AsInt(352);
          SetRect(&rcDest, 0, nScrollTop, nScrollR, nScrollB);
          SetRect(&rcSrc, (int)(-(__int64)g_fFrameTime_52F1B4) / 2, 2, 384 - (__int64)g_fFrameTime_52F1B4 / 2, 124);
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->pSprWindowBmp[0], &rcSrc);
          DrawSpriteLTRB(pScene->pSprWindowBmp[0], rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
          SetRect(&rcSrc, (int)(-(__int64)g_fFrameTime_52F1B4) / 3, 2, 256 - (__int64)g_fFrameTime_52F1B4 / 3, 124);
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->pSprWindowBmp[0], &rcSrc);
          DrawSpriteLTRB(pScene->pSprWindowBmp[0], rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
          SetRect(&rcSrc, (int)(-(__int64)g_fFrameTime_52F1B4) / 4, 2, 128 - (__int64)g_fFrameTime_52F1B4 / 4, 124);
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->pSprWindowBmp[0], &rcSrc);
          DrawSpriteLTRB(pScene->pSprWindowBmp[0], rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
          DrawDialogueText_4803B5(pScene, fFrameTime);
          g_pD3DDevice_4D6C78->EndScene();
        }
      }
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
  }
}

// DrawAdvanceDialogueText(pScene, xMouse, yMouse, wBtn)
// Static (non-scrolling) dialogue renderer + click-to-advance. Walks pScene->textSprites_264[4][29],
// drawing each glyph in a fixed 3-line layout (row i at y~32*i+270) with the window's current alpha;
// stops at the first glyph flagged field_10C (end of line). Then, if the mouse Y (yMouse) is within one of
// the 3 line bands, highlights that row with pSprWindow7bmp; on left-click (wBtn==1) it advances
// dwPtrCurrent_448 (skipping up to 200 chars across '*' chunk markers, counting rows crossed) and, on a
// row change, sets by_463=0, flt_454=0, flt_458=0.05 and returns to end the static display.
void __cdecl hook_DrawAdvanceDialogueText_47FA84(CHanaSceneT458_S504 *pScene, int xMouse, int yMouse, __int16 wBtn)
{
  int v4; // eax
  int left1; // eax
  int v6; // [esp-Ch] [ebp-44h]
  int top1; // [esp-Ch] [ebp-44h]
  int v8; // [esp-8h] [ebp-40h]
  int right1; // [esp-8h] [ebp-40h]
  int v10; // [esp-4h] [ebp-3Ch]
  int bottom1; // [esp-4h] [ebp-3Ch]
  int currentk; // [esp+0h] [ebp-38h]
  float v13; // [esp+4h] [ebp-34h]
  int i; // [esp+8h] [ebp-30h]
  int k; // [esp+8h] [ebp-30h]
  int j; // [esp+Ch] [ebp-2Ch]
  int m; // [esp+Ch] [ebp-2Ch]
  struct tagRECT rc; // [esp+10h] [ebp-28h] BYREF
  struct tagRECT rc1; // [esp+20h] [ebp-18h] BYREF
  int bIsOk; // [esp+30h] [ebp-8h]
  float alpha; // [esp+34h] [ebp-4h]

  ONCELOG("[HNH:SUB] DrawAdvanceDialogueText_47FA84\n");
  
  v13 = 0.0;
  alpha = 0.0;
  bIsOk = 0;
  if (!pScene || !pScene->pSprWindowBmp[3] || !pScene->ptrStrData_260) return;
  
  alpha = CHanaSpriteT3F6__GetAlpha_414AFB(pScene->pSprWindowBmp[3]);
  SetRect(&rc, 0, 0, 24, 24);
  for ( i = 0; i < 4; ++i )
  {
    for ( j = 0; j < 29; ++j )
    {
      if ( pScene->textSprites_264[i][j] )
      {
        SetRect(&rc1, 22 * j, 22 * i, 22 * (j + 1) + 2, 22 * (i + 1) + 2);
        if ( pScene->textSprites_264[i][j]->bOk_10C == 1 )
        {
          bIsOk = 1;
          break;
        }
        v13 = v13 + 1.0;
        g_pD3DDevice_4D6C78->SetTexture(0, 0);
        CHanaObj__SetAlpha3F6(pScene->textSprites_264[i][j], alpha);
        v10 = ConvertFrom640AsInt(22 * (i + 1) + 10 * i + 272);
        v8 = ConvertFrom640AsInt(22 * (j + 2) + 67);
        v6 = ConvertFrom640AsInt(32 * i + 270);
        v4 = ConvertFrom640AsInt(22 * (j + 1) + 65);
        SetRect(&rc1, v4, v6, v8, v10);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
        DrawSpriteLTRB(pScene->textSprites_264[i][j], rc1.left, rc1.top, rc1.right, rc1.bottom);
        DrawSpriteLTRB(pScene->textSprites_264[i][j], rc1.left, rc1.top, rc1.right, rc1.bottom);
      }
    }
    if ( bIsOk == 1 )
      break;
  }
  for ( k = 0; k < 3; ++k )
  {
    if ( ConvertFrom640AsInt(32 * k + 270) <= yMouse && ConvertFrom640AsInt(22 * (k + 1) + 10 * k + 270) >= yMouse )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
      bottom1 = ConvertFrom640AsInt(22 * (k + 1) + 10 * k + 275);
      right1 = ConvertFrom640AsInt(550);
      top1 = ConvertFrom640AsInt(32 * k + 265);
      left1 = ConvertFrom640AsInt(90);
      SetRect(&rc1, left1, top1, right1, bottom1);
      if ( pScene->pSprWindowBmp[4] )
        DrawSpriteLTRB(pScene->pSprWindowBmp[4], rc1.left, rc1.top, rc1.right, rc1.bottom);
      if ( wBtn == 1 )
      {
        currentk = 0;
        for ( m = 0; m < 200; ++m )
        {
          if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] == '*')
          {
            if ( currentk == k )
            {
              pScene->by_463 = 0;
              pScene->flt_454 = 0.0;
              pScene->flt_458 = 0.050000001;
              return;
            }
            ++currentk;
          }
          ++pScene->dwPtrCurrent_448;
        }
      }
      return;
    }
  }
}

// DrawDialogueText_4803B5 -- scrolling dialogue-text renderer (glyph half of
// DrawDialogueWindow_480B2D; called with the frame time x speed multiplier).
//
// Global scroll counter g_dwFontRenderFlag (@0x52F1B0, float in a DWORD
// global) advances fFrameTime*30 per call; scroll/100 = number of glyphs
// revealed. Empty scene (no ptrStrData_260) clears by_460/by_461 and bails.
// (The original's NULL-pScene branch writes through the NULL pointer -- a
// crash bug that is never hit; collapsed into the guard here.)
//
// The 4x29 text-sprite grid pScene->textSprites_264 is walked in two passes
// sharing the i/j cursor:
//   PASS 1 (settled glyphs): draw every fully-revealed glyph (alpha =
//   flt_454 * flt_450), 22-px grid cells converted from 640-space, each
//   drawn TWICE (double-alpha compounding), counting v18 = glyph index;
//   stops early (v20 = "still scrolling") when scroll/100 < v18, or at a
//   sprite with bOk_10C == 1 (v25 = page-end marker).
//   PASS 2 (fading tail): up to 5 glyphs per frame fade in with a
//   triangular window fade = |1 - (index - scroll/100)/5| clamped [0,1];
//   each glyph slides into place (right/left edges (1-fade)*10, top/bottom
//   (1-fade)*4/32 px), alpha = flt_450 * fade * flt_454, drawn twice.
//   Quirk kept: hitting the 5-glyph budget mid-row jumps to the NEXT ROW
//   (remaining columns of the current row are deferred a frame).
//
// Cursor sprites (pSprWindowBmp[2] = static base, [1] = bobbing arrow at
// x 624): hidden while text is still scrolling or by_461 == 0; otherwise
// [2]'s alpha ramps fFrameTime/100 up to 1 (copied to [1]) and [1] bobs at
// y = sin(scroll/180)*5 + 432. D3DTSS_ADDRESS is set to 3 (CLAMP) around
// the glyph drawing and restored after.

// === FUNCTIONS ===
void __cdecl hook_DrawDialogueText_4803B5(CHanaSceneT458_S504 *pScene, float fFrameTime)
{
  RECT rc;                               // [esp+50h] [ebp-28h]
  RECT rcGlyph;                          // [esp+60h] [ebp-18h]
  DWORD dwTssAddress;
  float fGlyphIndex, fFade, fFadeCount;  // v18 / v26 / v17
  int i, j, bStillScrolling, bPageEnd;

  ONCELOG("[HNH:SUB] DrawDialogueText_4803B5\n");

  fGlyphIndex = 0.0f;
  fFadeCount = 0.0f;
  bStillScrolling = 0;
  bPageEnd = 0;
  *(float *)&g_dwFontRenderFlag = *(float *)&g_dwFontRenderFlag + fFrameTime * 30.0f;

  // (the original's NULL-pScene / always-true pointer branches are dead
  // code; behaviorally only this guard matters)
  if (!pScene->ptrStrData_260)
  {
    pScene->by_460 = 0;
    pScene->by_461 = 0;
    return;
  }

  g_pD3DDevice_4D6C78->GetTextureStageState(0, D3DTSS_ADDRESS, &dwTssAddress);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 3);   // CLAMP
  SetRect(&rc, 0, 0, 24, 24);

  // --- pass 1: settled glyphs --------------------------------------------
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 29; ++j)
    {
      if (pScene->textSprites_264[i][j])
      {
        SetRect(&rcGlyph, 22 * j, 22 * i, 22 * (j + 1) + 2, 22 * (i + 1) + 2);
        if (*(float *)&g_dwFontRenderFlag / 100.0f < fGlyphIndex)
        {
          bStillScrolling = 1;    // reveal not caught up yet
          break;
        }
        if (pScene->textSprites_264[i][j]->bOk_10C == 1)
        {
          bPageEnd = 1;           // page-end marker glyph
          break;
        }
        fGlyphIndex += 1.0f;
        g_pD3DDevice_4D6C78->SetTexture(0, NULL);
        CHanaObj__SetAlpha3F6(pScene->textSprites_264[i][j],
          pScene->flt_454 * pScene->flt_450);
        SetRect(&rcGlyph,
          ConvertFrom640AsInt(22 * (j + 1)),
          ConvertFrom640AsInt(22 * i + 380),
          ConvertFrom640AsInt(22 * (j + 2) + 2),
          ConvertFrom640AsInt(22 * (i + 1) + 382));
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
        // drawn twice (double alpha compounding), as the original
        DrawSpriteLTRB(pScene->textSprites_264[i][j],
          rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
        DrawSpriteLTRB(pScene->textSprites_264[i][j],
          rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
      }
    }
    if (bStillScrolling == 1 || bPageEnd == 1)
      break;
  }

  // --- pass 2: fading tail (continues the shared i/j cursor) --------------
  while (i < 4)
  {
    if (j >= 29)
    {
      j = 0;                          // row done -> next row
      if (fFadeCount == 5.0f)
        break;                      // glyph budget for this frame
      ++i;
      continue;
    }
    if (pScene->textSprites_264[i][j])
    {
      fFade = fabsf(1.0f - (fGlyphIndex - *(float *)&g_dwFontRenderFlag / 100.0f) / 5.0f);
      if (fFade <= 1.0f)
      {
        if (fFade < 0.0f)
          goto nextGlyph;         // not in the fade window yet
      }
      else
      {
        fFade = 1.0f;
      }
      CHanaObj__SetAlpha3F6(pScene->textSprites_264[i][j],
        pScene->flt_450 * fFade * pScene->flt_454);
      SetRect(&rcGlyph,
        ConvertFrom640AsInt((int)((1.0f - fFade) * 10.0f) + 22 * (j + 1)),
        ConvertFrom640AsInt((int)((1.0f - fFade) * 4.0f) + 22 * i + 380),
        ConvertFrom640AsInt(22 * (j + 2) - (int)((1.0f - fFade) * 10.0f) + 2),
        ConvertFrom640AsInt(22 * (i + 1) + (int)((1.0f - fFade) * 32.0f) + 382));
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
      DrawSpriteLTRB(pScene->textSprites_264[i][j],
        rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
      DrawSpriteLTRB(pScene->textSprites_264[i][j],
        rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
      if (fFadeCount == 5.0f)
      {
        // budget exhausted mid-row: the original jumps to the NEXT
        // row (rest of this row waits a frame)
        j = 0;
        if (fFadeCount == 5.0f)
          break;
        ++i;
        continue;
      }
      fGlyphIndex += 1.0f;
      fFadeCount += 1.0f;
    }
  nextGlyph:
    ++j;
  }

  // --- "more text" cursor sprites ----------------------------------------
  if (pScene->pSprWindowBmp[1] && pScene->pSprWindowBmp[2])
  {
    if (bStillScrolling || !pScene->by_461)
    {
      pScene->pSprWindowBmp[2]->fAlpha_144 = 0.0f;   // hidden
      pScene->pSprWindowBmp[1]->fAlpha_144 = 0.0f;
    }
    else
    {
      pScene->by_460 = 0;
      pScene->pSprWindowBmp[2]->fAlpha_144 =
        fFrameTime / 100.0f + pScene->pSprWindowBmp[2]->fAlpha_144;
      pScene->pSprWindowBmp[1]->fAlpha_144 = pScene->pSprWindowBmp[2]->fAlpha_144;
      if (pScene->pSprWindowBmp[2]->fAlpha_144 > 1.0f)
      {
        pScene->pSprWindowBmp[2]->fAlpha_144 = 1.0f;
        pScene->pSprWindowBmp[1]->fAlpha_144 = 1.0f;
      }
      // static base indicator
      CHanaSpriteT3F6__SetPos_414A5D(pScene->pSprWindowBmp[2],
        (float)(int)ConvertFrom640AsFloat(624.0f),
        (float)(int)ConvertFrom640AsFloat(464.0f));
      DrawSprite2D(pScene->pSprWindowBmp[2]);
      // bobbing arrow
      CHanaSpriteT3F6__SetPos_414A5D(pScene->pSprWindowBmp[1],
        (float)(int)ConvertFrom640AsFloat(624.0f),
        (float)(int)ConvertFrom640AsFloat(
          sinf(*(float *)&g_dwFontRenderFlag / 180.0f) * 5.0f + 432.0f));
      DrawSprite2D(pScene->pSprWindowBmp[1]);
    }
  }

  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, dwTssAddress);
}


// per-channel blend: ((100-w)*dst + w*src)/100, channels isolated by the
// G/R/B bit masks (same expression the original inlines six times)
static DWORD BlendPixel(DWORD dwDst, DWORD dwSrc, int nW, int nInvW)
{
  return (g_dwGBitMask & ((nInvW * (g_dwGBitMask & dwDst) + nW * (g_dwGBitMask & dwSrc)) / 100))
    | (g_dwRBitMask & ((nInvW * (g_dwRBitMask & dwDst) + nW * (g_dwRBitMask & dwSrc)) / 100))
    | (g_dwRGBBitMask & ((nInvW * (g_dwRGBBitMask & dwDst) + nW * (g_dwRGBBitMask & dwSrc)) / 100));
}

// Surface_AlphaBlendRect_41A039 -- software alpha-blend of a rect between
// locked DirectDraw surfaces (used for the scene-background crossfade).
// out = ((100-nBlendWeight)*dst + nBlendWeight*src) / 100 per channel,
// channel masks = g_dwGBitMask / g_dwRBitMask / g_dwRGBBitMask (G/R/B) so
// the same code serves 565/555/888. Bit depth from g_dwPixelFormat:
// 16 (WORD stores), 24 (WORD + byte stores), else 32 (DWORD stores).
// bSkipColorKey & 1: source pixels equal to the surface's DD color key
// (GetColorKey type 8) are left untouched. Same-surface blends lock once
// and unlock once; two-surface blends lock src (Pointer) + dst (pSrcRow)
// and unlock both. Pitches come from each GetSurfaceDesc's lPitch /
// g_dwBytesPerPixel, then scaled to bytes per depth. The 16/24-bit paths
// READ the source as a DWORD (one pixel beyond the rect on the last
// column -- harmless, all channel masks fit the low bits; kept faithful).

void __cdecl hook_Surface_AlphaBlendRect(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY,
  LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect,
  int nBlendWeight, char bSkipColorKey)
{
  DDSURFACEDESC2 ddsd;                   // [ebp-84h]
  DDCOLORKEY colorKey;                   // [ebp-A0h]
  BYTE *pSrcBase, *pDstBase;             // locked pointers
  DWORD dwSrcPitch, dwDstPitch;          // byte pitches
  DWORD dwKey;                           // key color (low bits per depth)
  DWORD dwSrc, dwDst, dwBlend;
  UINT nBpp;                             // bytes per pixel: 2/3/4
  int nInvBlendWeight, nRectHeight, nRectWidthBytes;
  int nSrcLeftBytes, nDestLeftBytes;
  int i, y;
  BOOL bSameSurface;

  ONCELOG("[HNH:SUB] Surface_AlphaBlendRect_41A039\n");

  bSameSurface = 0;
  nInvBlendWeight = 100 - nBlendWeight;

  if (pSrcSurface == pDestSurface)
  {
    ddsd.dwSize = sizeof(DDSURFACEDESC2);              // 124
    pSrcSurface->GetSurfaceDesc(&ddsd);
    dwDstPitch = dwSrcPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
    pSrcSurface->GetColorKey(DDCKEY_SRCBLT /*8*/, &colorKey);
    pSrcBase = (BYTE *)Surface_LockAndGetPointer(pSrcSurface);
    pDstBase = pSrcBase;                               // one lock
    bSameSurface = 1;
  }
  else
  {
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    pDestSurface->GetSurfaceDesc(&ddsd);
    dwDstPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    pSrcSurface->GetSurfaceDesc(&ddsd);
    dwSrcPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
    pSrcSurface->GetColorKey(DDCKEY_SRCBLT, &colorKey);
    pSrcBase = (BYTE *)Surface_LockAndGetPointer(pSrcSurface);
    pDstBase = (BYTE *)Surface_LockAndGetPointer(pDestSurface);
  }

  nRectHeight = pSrcRect->bottom - pSrcRect->top;
  dwKey = colorKey.dwColorSpaceLowValue;

  if (g_dwPixelFormat == 16)
  {
    nBpp = 2;
    nRectWidthBytes = 2 * (pSrcRect->right - pSrcRect->left);
    nSrcLeftBytes   = 2 * pSrcRect->left;
    nDestLeftBytes  = 2 * nDestX;
    dwSrcPitch *= 2;
    dwDstPitch *= 2;
    // (the original's dwColorKeyFlags = 6/8 dance is dead)

    for (y = 0; y < nRectHeight; ++y)
    {
      BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
      BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
      for (i = 0; i < nRectWidthBytes; i += 2)
      {
        // source is READ as a DWORD in the original (straddles into
        // the next pixel; the masks keep only the current one)
        dwSrc = *(DWORD *)(pSrc + i);
        if ((bSkipColorKey & 1) && *(WORD *)(pSrc + i) == (WORD)dwKey)
          continue;                              // key pixel: skip
        dwDst = *(DWORD *)(pDst + i);
        *(WORD *)(pDst + i) = (WORD)BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
      }
    }
  }
  else if (g_dwPixelFormat == 24)
  {
    nRectWidthBytes = 3 * (pSrcRect->right - pSrcRect->left);
    nSrcLeftBytes   = 3 * pSrcRect->left;
    nDestLeftBytes  = 3 * nDestX;
    dwSrcPitch *= 3;
    dwDstPitch *= 3;
    dwKey &= 0xFFFFFF;                                 // 24-bit key

    for (y = 0; y < nRectHeight; ++y)
    {
      BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
      BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
      for (i = 0; i < nRectWidthBytes; i += 3)
      {
        dwSrc = *(DWORD *)(pSrc + i) & 0xFFFFFF;    // dword read (see above)
        if ((bSkipColorKey & 1) && dwSrc == dwKey)
          continue;
        dwDst = *(DWORD *)(pDst + i);
        // 24-bit store: low WORD + third byte
        dwBlend = BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
        *(WORD *)(pDst + i) = (WORD)dwBlend;
        *(BYTE *)(pDst + i + 2) = (BYTE)(dwBlend >> 16);
      }
    }
  }
  else                                                   // 32-bit
  {
    nRectWidthBytes = 4 * (pSrcRect->right - pSrcRect->left);
    nSrcLeftBytes   = 4 * pSrcRect->left;
    nDestLeftBytes  = 4 * nDestX;
    dwSrcPitch *= 4;
    dwDstPitch *= 4;

    for (y = 0; y < nRectHeight; ++y)
    {
      BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
      BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
      for (i = 0; i < nRectWidthBytes; i += 4)
      {
        if ((bSkipColorKey & 1) && *(DWORD *)(pSrc + i) == dwKey)
          continue;
        dwSrc = *(DWORD *)(pSrc + i);
        dwDst = *(DWORD *)(pDst + i);
        *(DWORD *)(pDst + i) = BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
      }
    }
  }

  pSrcSurface->Unlock(NULL);
  if (!bSameSurface)
    pDestSurface->Unlock(NULL);
}

LPVOID __cdecl hook_Surface_LockAndGetPointer(LPDIRECTDRAWSURFACE7 pSurface)
{
  DDSURFACEDESC2 surfaceDesc; // [esp+10h] [ebp-7Ch] BYREF
  ONCELOG("[HNH:SUB] Surface_LockAndGetPointer_4196FD\n");
 
  surfaceDesc.dwSize = 124;
  while ( pSurface->Lock(0, &surfaceDesc, 2049, 0) )
    ;
  return surfaceDesc.lpSurface;
}

void register_PlayOdsDialogueScene_477F90(int cfg) {
  if (cfg == 1) {
    PlayOdsDialogueScene_477F90 = hook_PlayOdsDialogueScene_477F90;
    LoadOds_4843CF = hook_LoadOds_4843CF;
    ResetSceneBodies_4857CA = hook_ResetSceneBodies_4857CA;
    UpdateSceneAtTime_4825D5 = hook_UpdateSceneAtTime_4825D5;
    UpdateSceneTimer_4827FE = hook_UpdateSceneTimer_4827FE;
    PauseSceneAudio_46A2A0 = hook_PauseSceneAudio_46A2A0;
    SetProjectionFov_40DCA3 = hook_SetProjectionFov_40DCA3;
    DrawDialogueWindow_480B2D = hook_DrawDialogueWindow_480B2D;
    DrawAdvanceDialogueText_47FA84 = hook_DrawAdvanceDialogueText_47FA84;
    DrawDialogueText_4803B5 = hook_DrawDialogueText_4803B5;
    CHanaObj__NewSceneT458_S504 = hook_CHanaObj__NewSceneT458_S504;
    CSekBody__CutToAnim_4018E1 = hook_CSekBody__CutToAnim_4018E1;
    CSekBody__UpdateAnim_4019E1 = hook_CSekBody__UpdateAnim_4019E1;
    CHanaAnimSetT3F1__BeginBlend_43A371 = hook_CHanaAnimSetT3F1__BeginBlend_43A371;
    CHanaAnimSetT3F1__BlendStep_43A643 = hook_CHanaAnimSetT3F1__BlendStep_43A643;
    HanaAnim_UpdateBoneTransform = hook_HanaAnim_UpdateBoneTransform;
    SekBody__UpdateTo2_401647 = hook_SekBody__UpdateTo2_401647;
    Surface_AlphaBlendRect = hook_Surface_AlphaBlendRect;
    Surface_LockAndGetPointer = hook_Surface_LockAndGetPointer;
  }

  hnh_HookOff();
    hnh_Hook(0x477F90, hook_PlayOdsDialogueScene_477F90, "PlayOdsDialogueScene_477F90"); // [OK]
    hnh_Hook(0x4843cf, hook_LoadOds_4843CF, "LoadOds_4843CF"); // [OK]
    hnh_Hook(0x47fa84, hook_DrawAdvanceDialogueText_47FA84, "DrawAdvanceDialogueText_47FA84");
    hnh_Hook(0x4019e1, hook_CSekBody__UpdateAnim_4019E1, "CSekBody__UpdateAnim_4019E1"); // [OK]


    hnh_Hook(0x4018e1, hook_CSekBody__CutToAnim_4018E1, "CSekBody__CutToAnim_4018E1"); // [OK]
   
    hnh_Hook(0x401647, hook_SekBody__UpdateTo2_401647, "SekBody__UpdateTo2_401647");  // [OK]
    hnh_Hook(0x43a643, hook_CHanaAnimSetT3F1__BlendStep_43A643, "CHanaAnimSetT3F1__BlendStep_43A643"); // [OK]
    hnh_Hook(0x4857ca, hook_ResetSceneBodies_4857CA, "ResetSceneBodies_4857CA"); // [OK]
    hnh_Hook(0x481c5f, hook_CHanaObj__NewSceneT458_S504, "CHanaObj__NewSceneT458_S504_481C5F"); // [OK]
    hnh_Hook(0x4827fe, hook_UpdateSceneTimer_4827FE, "UpdateSceneTimer_4827FE"); // [OK]
    hnh_Hook(0x46a2a0, hook_PauseSceneAudio_46A2A0, "PauseSceneAudio_46A2A0"); // [OK]
    hnh_Hook(0x40dca3, hook_SetProjectionFov_40DCA3, "SetProjectionFov_40DCA3"); // [OK]
    hnh_Hook(0x480b2d, hook_DrawDialogueWindow_480B2D, "DrawDialogueWindow_480B2D"); // [OK]
    hnh_Hook(0x4825d5, hook_UpdateSceneAtTime_4825D5, "UpdateSceneAtTime_4825D5");  // [OK]
    hnh_Hook(0x4803B5, hook_DrawDialogueText_4803B5, "DrawDialogueText_4803B5"); // [OK]
    hnh_Hook(0x41A039, hook_Surface_AlphaBlendRect, "Surface_AlphaBlendRect_41A039"); // [OK]
    hnh_Hook(0x4196fd, hook_Surface_LockAndGetPointer, "Surface_LockAndGetPointer_4196FD"); // [OK]
    hnh_Hook(0x43a371, hook_CHanaAnimSetT3F1__BeginBlend_43A371, "CHanaAnimSetT3F1__BeginBlend_43A371"); // [OK]
    hnh_Hook(0x43a43f, hook_HanaAnim_UpdateBoneTransform, "HanaAnim_UpdateBoneTransform_43A43F"); // [OK]
  hnh_HookOn();
}
