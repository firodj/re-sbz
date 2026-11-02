#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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

// 20-char path-table entry (IDA: String20)
struct String20 { char str[20]; };

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
BOOL (__cdecl*ParseExecStr_487D89)(CHanaSceneT458_S504 *pScene, float *pfExecTime) =
    reinterpret_cast<BOOL(__cdecl*)(CHanaSceneT458_S504 *, float *)>(0x487D89);
void (__cdecl*PauseSceneAudio_46A2A0)(CHanaSceneT458_S504 *pScene, int bPause) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, int)>(0x46A2A0);
void (__cdecl*SetProjectionFov_40DCA3)(float fFov) =
    reinterpret_cast<void(__cdecl*)(float)>(0x40DCA3);
void (__cdecl*DrawDialogueWindow_480B2D)(CHanaSceneT458_S504 *pScene,
    float frameTimeMultiplied, int xMouse, int yMouse, short btnMouse) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, float, int, int, short)>(0x480B2D);
void(__cdecl*Surface_AlphaBlendRect)(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY, LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect, int nBlendWeight, char bSkipColorKey)
  = reinterpret_cast<void(__cdecl*)(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY, LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect, int nBlendWeight, char bSkipColorKey)>(0x41A039);


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


// === FUNCTIONS ===
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

    DebugLog("[HNH:SUB] PlayOdsDialogueScene_477F90\n");

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
    StopAndRestartThread(2);

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
            StopAndRestartThread(0);
            memcpy(&stcScene, &g_sceneState_52F7E0, sizeof(stcScene));
            nResult = SystemMenu_4655AB(pHanaScene->pSndBuf_46C,
                                        pHanaScene->field_470, 0,
                                        g_sceneState_52F7E0.protCurrentHP_C0,
                                        g_sceneState_52F7E0.protMaximumHP_C4,
                                        stcScene);
            StopAndRestartThread(2);
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
    StopAndRestartThread(0);
    if (pHanaScene)
    {
        CHanaObj__Free_40C1E4(pHanaScene);
        pHanaScene = NULL;
    }
    if (pCursor)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130*)&pCursor);
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
  wsprintfA(String1, "新フォーマットＯＤＳ%c%c%c", 13, 10, 0);
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
  ApplyQTS_4035B1(&pAnim->pHanaFram_130->mat4x4_fmrx, &resultQuat, startX, startY, startZ, endX, endY, endZ);
  pAnim->keyCur_14C.fPosX = startX;
  pAnim->keyCur_14C.fPosY = startY;
  pAnim->keyCur_14C.fPosZ = startZ;
  pAnim->keyCur_14C.fScaleX = endX;
  pAnim->keyCur_14C.fScaleY = endY;
  pAnim->keyCur_14C.fScaleZ = endZ;
  pAnim->keyCur_14C.quatRot = resultQuat;
  qmemcpy(&pAnim->keyCur_14C.matLocal, &pAnim->pHanaFram_130->mat4x4_fmrx, sizeof(pAnim->keyCur_14C.matLocal));
}

void register_PlayOdsDialogueScene_477F90() {
    hnh_Hook(0x477F90, hook_PlayOdsDialogueScene_477F90, "PlayOdsDialogueScene_477F90"); // [OK]
    hnh_Hook(0x43a371, hook_CHanaAnimSetT3F1__BeginBlend_43A371, "CHanaAnimSetT3F1__BeginBlend_43A371");
    hnh_Hook(0x43a43f, hook_HanaAnim_UpdateBoneTransform, "HanaAnim_UpdateBoneTransform_43A43F");

    hnh_HookOff();
    hnh_Hook(0x4843cf, hook_LoadOds_4843CF, "LoadOds_4843CF"); // [OK]
    hnh_Hook(0x4018e1, hook_CSekBody__CutToAnim_4018E1, "CSekBody__CutToAnim_4018E1"); // [OK]
    hnh_Hook(0x4019e1, hook_CSekBody__UpdateAnim_4019E1, "CSekBody__UpdateAnim_4019E1"); // [OK]
    hnh_Hook(0x43a643, hook_CHanaAnimSetT3F1__BlendStep_43A643, "CHanaAnimSetT3F1__BlendStep_43A643"); // [OK]
    hnh_Hook(0x4857ca, hook_ResetSceneBodies_4857CA, "ResetSceneBodies_4857CA"); // [OK]
    hnh_Hook(0x481c5f, hook_CHanaObj__NewSceneT458_S504, "CHanaObj__NewSceneT458_S504_481C5F"); // [OK]
    hnh_HookOn();
}
