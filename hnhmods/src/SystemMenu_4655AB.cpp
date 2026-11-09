// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*SystemMenu_4655AB)(LPDIRECTSOUNDBUFFER pBgm, int nVolOffset, int nMenuMode,
    float fCurHP, float fMaxHP, CSceneState sceneState) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER, int, int, float, float, CSceneState)>(0x4655AB);
int (__cdecl*SysMenu_GirlStatusCheck_464919)(LPDIRECTDRAWSURFACE7 *arrSurf,
  CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY,
  int bPressed, int *pnSceneStep, float fCurHP, float fMaxHP, QWORD *pSceneFlags) =
  reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7 *, CHanaSpriteT3F6_S1FC *,
    CHanaSpriteT3F6_S1FC *, int, int, int, int *, float, float, QWORD *)>(0x464919);
void (__cdecl*SysMenu_Graphics_46380D)(int *arrGfxOpts, CSceneState *arrSceneStates,
  LPDIRECTDRAWSURFACE7 *arrSurf, CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
  int nMouseX, int nMouseY, int bPressed) =
  reinterpret_cast<void(__cdecl*)(int *, CSceneState *, LPDIRECTDRAWSURFACE7 *,
    CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x46380D);
void (__cdecl*SysMenu_Options_462FE9)(CSceneState *sceneStates, LPDIRECTDRAWSURFACE7 *arrSurf,
  CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY,
  int bPressed) =
  reinterpret_cast<void(__cdecl*)(CSceneState *, LPDIRECTDRAWSURFACE7 *,
    CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462FE9);
void (__cdecl*SysMenu_SaveMenu_462135)(CSceneState sceneStates[], CHAR *arrSceneName,
  LPDIRECTDRAWSURFACE7 arrSurf[], CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
  int nMouseX, int nMouseY, int bPressed) =
  reinterpret_cast<void(__cdecl*)(CSceneState *, CHAR *, LPDIRECTDRAWSURFACE7 *,
    CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462135);
int (__cdecl*SysMenu_LoadMenu_462851)(CSceneState *arrScene, CHAR *arrSceneName,
  LPDIRECTDRAWSURFACE7 *arrSurf, CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
  int nMouseX, int nMouseY, int bPressed) =
  reinterpret_cast<int(__cdecl*)(CSceneState *, CHAR *, LPDIRECTDRAWSURFACE7 *,
    CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462851);
int (__cdecl*ShowYesNoDialog_461A9E)(CHanaSpriteT3F6_S1FC *pCursor,
  CHanaSpriteT3F6_S1FC *pDialog, LPCSTR szCaption) =
  reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, LPCSTR)>(0x461A9E);


// === PINNED GLOBALS ===
BYTE &g_mapFrameFlag_530264       = *(BYTE *)0x530264;  // WRITABLE
BYTE &g_nEffectLevel_530265       = *(BYTE *)0x530265;  // WRITABLE
BYTE &g_btoggleC_530268           = *(BYTE *)0x530268;  // WRITABLE
DWORD &g_dwUnlockedSaveSlots_530260 = *(DWORD *)0x530260;                   // WRITABLE

// === FUNCTIONS ===

// SystemMenu_4655AB -- the shared in-game SYSTEM MENU (save / load / girl
// status / options / graphics / back / quit), called from the scene
// functions and every menu sub-screen. pBgm keeps playing (restarted if it
// stopped); after the Options tab, its volume is re-applied as
// nVolOffset + g_fVolumeDbTable[g_nAviVolumeIndex_530266], clamped to
// [-10000, 0] dB.
//
// nMenuMode gates the tabs and strip overlays:
//   0 (scenes)   : all 7 tabs
//   1 (Continue) : start on tab 1 (Load), tabs 1/5 selectable; three
//                  alpha-0.5 menu strips over the scene, positioned at
//                  g_nScreenScale_52F19C/1024 scale -- and the menu exits
//                  early once g_sceneState.bActiveSceneStep advances
//   2 (submenus) : start on tab 3 (Options), tabs 3..6, one strip
// Tabs are 79-wide hit bands across a 30-high top strip:
//   0 Save (SysMenu_SaveMenu_462135), 1 Load (SysMenu_LoadMenu_462851),
//   2 girl status (SysMenu_GirlStatusCheck_464919 -- receives fCurHP/
//     fMaxHP and sceneState.sceneFlags_98), 3 Options (SysMenu_Options_
//     462FE9), 4 Graphics (SysMenu_Graphics_46380D), 5 back (result 0),
//   6 quit: the previous tab is first re-rendered once with the mouse
//     parked at (-100,-100), then ShowYesNoDialog "ゲームを終了しますか？"
//     -> Yes returns 1. Tab switches play the "On" effect; right-click
//     exits with 0 ("click2" effect).
// Assets: Systembk.bmp background (87x87 scaled, arrSurf[9]), mouse.bmp
// cursor, YesNo1.bmp + YesNo2.bmp dialog sprite (SPR dir). Entry captures
// the primary into a temp surface and fades OUT on it; the first loop frame
// fades back IN on the back buffer.
// Graphics options persist through <exe-dir>\Tea.ex (128 bytes: fog,
// specular, antialias, dither, light, shadow) -- read into the g_b* globals
// on entry, written back on exit. Font switched to ＭＳ 明朝 15x15 for the
// menu, restored to 26x26 after. Teardown releases everything, restores
// ANTIALIAS and render states, and FrameTimer_BeginFrame.
int __cdecl hook_SystemMenu_4655AB(LPDIRECTSOUNDBUFFER pBgm, int nVolOffset, int nMenuMode,
                                   float fCurHP, float fMaxHP, CSceneState sceneState)
{
    CHanaSpriteT3F6_S1FC *pCursor;          // [esp+B8h] [ebp-18B8h]
    CHanaSpriteT3F6_S1FC *pMenuStrip;       // [esp+210h] [ebp-1760h]
    CHanaSpriteT3F6_S1FC *pYesNo;           // [esp+330h] [ebp-1640h]
    CHAR szFileName[256];                   // [esp+BCh] [ebp-18B4h]
    CHAR szMaskPath[256];                   // [esp+185Ch] [ebp-114h]
    CSceneState SceneStates[12];            // [esp+344h] [ebp-162Ch]
    char pszTimestamps[600];                // [esp+1604h] [ebp-36Ch]
    LPDIRECTDRAWSURFACE7 arrSurf[20];       // [esp+1C0h] [ebp-17B0h]
    int arrGfxOpts[32];                     // Tea.ex image (128 bytes used)
    CInputState *pInput;
    DWORD dwAntialiasSaved;
    int nResult, nTabCur, nPrevTab, nW;
    int arrH[3];                            // {480, 87, 87} converted
    int i, nMouseX, nMouseY, bPressed, wBtnPrev, wBtnNow = 0, wBtn;
    int pnSceneStep;
    BOOL bFadedIn, bRightClickExit;

    ONCELOG("[HNH:SUB] SystemMenu_4655AB\n");

    bFadedIn = 0;
    bRightClickExit = FALSE;
    pnSceneStep = (BYTE)g_sceneState_52F7E0.bMaxSubStage;
    nResult = -1;
    nTabCur = 0;
    nPrevTab = 0;
    pCursor = NULL;
    pMenuStrip = NULL;

    nW = ConvertFrom640AsInt(640);
    arrH[0] = ConvertFrom640AsInt(480);
    arrH[1] = ConvertFrom640AsInt(87);
    arrH[2] = ConvertFrom640AsInt(87);
    SetUiFont_419BCD(ConvertFrom640AsInt(15), ConvertFrom640AsInt(15),
                     "ＭＳ 明朝");   // aMs_1 @0x4C5960: "ＭＳ 明朝"
    memset(arrSurf, 0, sizeof(arrSurf));
    LoadDataFiles_461557(SceneStates, pszTimestamps);
    if (nMenuMode == 1)
    {
        nPrevTab = 1;
        nTabCur = 1;
    }
    if (nMenuMode == 2)
    {
        nPrevTab = 3;
        nTabCur = 3;
    }

    // graphics options from <exe>\Tea.ex -> the g_b* globals
    wsprintfA(szFileName, "Tea.ex");
    CHanaObj__GetRealPath_486D9A(szFileName, 1001);             // 1001 = exe dir
    if (IsFileExists_47E8EE(szFileName)
        && ReadFileToBuffer(szFileName, arrGfxOpts, 0x80) == 128)
    {
        g_bAntiAliasEnable       = arrGfxOpts[2];
        g_bDitherEnable          = arrGfxOpts[3];
        g_bSpecularEnable_4B3474 = arrGfxOpts[1];
        g_bFogIsActive_4B3480    = arrGfxOpts[0];
        g_bLightEnable_4B3484    = arrGfxOpts[4];
        g_bShadowEnable_518CA8   = arrGfxOpts[5];
    }

    // capture the current screen and fade out on it
    arrSurf[0] = CreateSurfaceWH(nW, arrH[0]);
    if (arrSurf[0])
    {
        BltFastWH(arrSurf[0], 0, 0, g_pPrimaryBuffer_4D6C88, 0, 0, nW, arrH[0], 0);
        PlayScreenFade(0, 0xFF7FFFFF, arrSurf[0], 25.0f, 0);
        arrSurf[0]->Release();
        arrSurf[0] = NULL;
    }

    if (nMenuMode)
        CHanaObj__NewSprite3F6byWH(&pMenuStrip, 0x20, 0x20);    // strip overlay

    // Systembk.bmp (the loop's i=1 entry of the 270-stride name table)
    wsprintfA(szFileName, "Systembk.bmp");                      // g_aSystem01Bmp+270
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);             // 1007 = GRP dir
    arrSurf[9] = LoadBmpScaled_4203B3(szFileName, arrH[1], arrH[2]);

    wsprintfA(szFileName, "mouse.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);             // 1003 = SPR dir
    LoadBmpWithMask_413A5E(&pCursor, szFileName, szFileName);
    wsprintfA(szFileName, "YesNo1.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);
    wsprintfA(szMaskPath, "YesNo2.bmp");
    CHanaObj__GetRealPath_486D9A(szMaskPath, 1003);
    LoadBmpWithMask_413A5E(&pYesNo, szFileName, szMaskPath);

    StopAndRestartThread(2);
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntialiasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);

    for (;;)
    {
        // the BGM must keep playing
        if (pBgm)
        {
            if (pBgm->GetStatus(&g_dwDSStatus_5185C8) != 0 || (g_dwDSStatus_5185C8 & 1) == 0)
                pBgm->Play(0, 0, 0);
        }
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
        DoPeekMessages();
        MutexThreadWait_4815EC(TRUE);
        pInput = GetInputState_481B37();
        wBtn = (BYTE)pInput->mouseBtn_4;
        CHanaSpriteT3F6__SetPos_414A5D(pCursor,
                (float)(pInput->mouseX_0 + 16), (float)(pInput->mouseY_2 + 16));
        wBtnPrev = wBtnNow;
        nMouseX = pInput->mouseX_0;
        nMouseY = pInput->mouseY_2;
        wBtnNow = (BYTE)pInput->mouseBtn_4;
        bPressed = wBtnNow == 1 && !wBtnPrev;

        // top-strip tab hit-test (79-wide bands, 30 high)
        if (wBtn == 1)
        {
            for (i = 0; i < 7; ++i)
            {
                BOOL bTabAllowed = !nMenuMode
                    || (nMenuMode == 1 && (i == 1 || i == 5))
                    || (nMenuMode == 2 && i > 2);
                if (bTabAllowed
                    && ConvertFrom640AsInt(79 * i) <= pInput->mouseX_0
                    && pInput->mouseX_0 < ConvertFrom640AsInt(79 * (i + 1))
                    && pInput->mouseY_2 >= 0
                    && pInput->mouseY_2 < ConvertFrom640AsInt(30))
                {
                    if (nTabCur != i)
                    {
                        if (g_arrUIEffectDSBuffer_52F168[2])    // "On"
                            DSBufferPlay(g_arrUIEffectDSBuffer_52F168[2], 0);
                        nPrevTab = nTabCur;
                        nTabCur = i;
                    }
                    break;
                }
            }
        }
        MutexThreadWait_4815EC(FALSE);

        if (wBtn == 2)                        // right-click: exit with 0
        {
            bRightClickExit = TRUE;
            break;
        }

        switch (nTabCur)
        {
        case 0:                               // Save
            SysMenu_SaveMenu_462135(SceneStates, pszTimestamps, arrSurf, pCursor,
                                     pYesNo, nMouseX, nMouseY, bPressed);
            break;
        case 1:                               // Load
            nResult = SysMenu_LoadMenu_462851(SceneStates, pszTimestamps, arrSurf,
                                              pCursor, pYesNo, nMouseX, nMouseY, bPressed);
            break;
        case 2:                               // girl status
            SysMenu_GirlStatusCheck_464919(arrSurf, pCursor, pYesNo, nMouseX, nMouseY,
                                           bPressed, &pnSceneStep, fCurHP, fMaxHP,
                                           sceneState.sceneFlags_98);
            break;
        case 3:                               // Options
            SysMenu_Options_462FE9(SceneStates, arrSurf, pCursor, pYesNo,
                                   nMouseX, nMouseY, bPressed);
            if (pBgm)                         // re-apply BGM volume after changes
            {
                int nVol = (int)((double)nVolOffset + g_fVolumeDbTable[g_nAviVolumeIndex_530266]);
                if (nVol < -10000)
                    nVol = -10000;
                else if (nVol > 0)
                    nVol = 0;
                pBgm->SetVolume(nVol);
            }
            break;
        case 4:                               // Graphics
            SysMenu_Graphics_46380D(arrGfxOpts, SceneStates, arrSurf, pCursor, pYesNo,
                                    nMouseX, nMouseY, bPressed);
            break;
        case 5:                               // back
            nTabCur = nPrevTab;
            nResult = 0;
            break;
        case 6:                               // quit (with confirm)
            if (nPrevTab <= 9)
            {
                // re-render the previous tab once, mouse parked off-screen
                if (!nPrevTab)
                    SysMenu_SaveMenu_462135(SceneStates, pszTimestamps, arrSurf, pCursor,
                                            pYesNo, -100, -100, 0);
                if (nPrevTab == 1)
                    SysMenu_LoadMenu_462851(SceneStates, pszTimestamps, arrSurf, pCursor,
                                            pYesNo, -100, -100, 0);
                if (nPrevTab == 2)
                    SysMenu_GirlStatusCheck_464919(arrSurf, pCursor, pYesNo, -100, -100, 0,
                                                   &pnSceneStep, fCurHP, fMaxHP,
                                                   sceneState.sceneFlags_98);
                if (nPrevTab == 3)
                    SysMenu_Options_462FE9(SceneStates, arrSurf, pCursor, pYesNo,
                                           -100, -100, 0);
                if (nPrevTab == 4)
                    SysMenu_Graphics_46380D(arrGfxOpts, SceneStates, arrSurf, pCursor,
                                            pYesNo, -100, -100, 0);
                if (ShowYesNoDialog_461A9E(pCursor, pYesNo,
                        "ゲームを終了しますか？")  // asc_4C599C: "ゲームを終了しますか？"
                    == 1)
                    nResult = 1;
                nTabCur = nPrevTab;
            }
            break;
        default:
            break;
        }
        if (nResult != -1)
            break;                             // a tab decided the outcome

        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA

        // overlay strips (menu modes only), at screen-scale/1024
        if (nMenuMode == 1)
        {
            if (g_sceneState_52F7E0.bActiveSceneStep)
                break;                         // the scene advanced under us
            double fScale = (double)g_nScreenScale_52F19C / 1024.0;
            pMenuStrip->fAlpha_144 = 0.5f;
            DrawSpriteLTRB(pMenuStrip, (int)(fScale * 1.0), (int)(fScale * 1.0),
                           (int)(fScale * 126.0), (int)(fScale * 46.0));
            DrawSpriteLTRB(pMenuStrip, (int)(fScale * 257.0), (int)(fScale * 1.0),
                           (int)(fScale * 638.0), (int)(fScale * 46.0));
            DrawSpriteLTRB(pMenuStrip, (int)(fScale * 768.0), 0,
                           (int)(fScale * 895.0), (int)(fScale * 45.0));
        }
        else if (nMenuMode == 2)
        {
            double fScale = (double)g_nScreenScale_52F19C / 1024.0;
            pMenuStrip->fAlpha_144 = 0.5f;
            DrawSpriteLTRB(pMenuStrip, (int)(fScale * 1.0), (int)(fScale * 1.0),
                           (int)(fScale * 384.0), (int)(fScale * 46.0));
        }

        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            DrawSprite2D(pCursor);
            g_pD3DDevice_4D6C78->EndScene();
        }
        if (!bFadedIn)
        {
            bFadedIn = 1;
            PlayScreenFade(1, 0xFF7FFFFF, g_pBackBuffer_4D6C90, 25.0f, 0);
        }
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    if (bRightClickExit)
    {
        if (g_arrUIEffectDSBuffer_52F168[4])   // "click2" (0x52F178)
            DSBufferPlay(g_arrUIEffectDSBuffer_52F168[4], 0);
        nResult = 0;
    }

    // teardown
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntialiasSaved);
    StopAndRestartThread(0);
    if (arrSurf[nTabCur])
    {
        BltFastWH(arrSurf[nTabCur], 0, 0, g_pPrimaryBuffer_4D6C88, 0, 0, nW, arrH[0], 0);
        PlayScreenFade(0, 0, arrSurf[nTabCur], 50.0f, 0);
        arrSurf[nTabCur]->Release();
        arrSurf[nTabCur] = NULL;
    }
    for (i = 0; i < 20; ++i)
    {
        if (arrSurf[i])
        {
            arrSurf[i]->Release();
            arrSurf[i] = NULL;
        }
    }
    if (pCursor)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130*) pCursor);
        pCursor = NULL;
    }
    if (pYesNo)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130*)pYesNo);
        pYesNo = NULL;
    }
    if (pMenuStrip)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130*)pMenuStrip);
        pMenuStrip = NULL;
    }
    SetUiFont_419BCD(26, 26, "ＭＳ 明朝");  // restore ＭＳ 明朝 26x26
    D3DSetRenderStates_4276C0();

    // write graphics options back to Tea.ex
    wsprintfA(szFileName, "Tea.ex");
    CHanaObj__GetRealPath_486D9A(szFileName, 1001);
    if (IsFileExists_47E8EE(szFileName))
    {
        arrGfxOpts[2] = g_bAntiAliasEnable;
        arrGfxOpts[3] = g_bDitherEnable;
        arrGfxOpts[1] = g_bSpecularEnable_4B3474;
        arrGfxOpts[0] = g_bFogIsActive_4B3480;
        arrGfxOpts[4] = g_bLightEnable_4B3484;
        arrGfxOpts[5] = g_bShadowEnable_518CA8;
        WriteBufferToFile(szFileName, arrGfxOpts, 128);
    }
    FrameTimer_BeginFrame();
    return nResult;
}

// === FUNCTIONS ===

// SysMenu_GirlStatusCheck_464919 -- SystemMenu tab 2 (girl status). Per-girl
// backgrounds System31..System35.bmp (30-stride name table g_aSystem01Bmp
// +360 @0x4B5840), cached in arrSurf[12..16]; button/lock sheet
// sys3bt2.bmp (160x50) into arrSurf[17]. pYesNo is passed but never used.
//
// Prev/next arrows (80x30 cells of the sheet, hit zones y 427.5..457.5,
// x 442.5..522.5 / 522.5..602.5) CYCLE *pnSceneStep 0..bMaxSubStage
// (wrap-around, "click" SFX); bMaxSubStage is clamped to 4 on entry.
// Per girl-step, locked items draw the 90x17 lock bar (sheet src (0,30)):
//   - option rows: v82 = {0,3,3,4,4} rows; row i gated by bit i of
//     pSceneFlags[step], lock position (69, 500+40i) -- the loop runs
//     count-1 times, so step 0 draws none, steps 1-2 draw 2, 3-4 draw 3
//   - girl-status slots: v93 = {12,14,14,15,15} slots; slot i gated by bit
//     10+i, lock positions (x,y) walking (713,94),(763,126),(813,158)...
//     y+32 per slot, x cycling 713/763/813
//   - the whole girl is locked (bit step-1 of g_dwUnlockedSaveSlots_530260,
//     step != 0) -> one lock at (69,460)
//   (all positions 1024x768 design space, scaled *0.625 then ConvertFrom640)
//
// HP readout "cur/max" at (153.125, 53.125): full-width digits via the
// standard patch (wsprintf "０" then szText[1] += digit), 5 digits each,
// right-to-left (x = 153.125 - (i+1)*9), max digits offset by its digit
// count; cur digits WHITE while curHP >= maxHP/3, else RED (0xFF).
// fCurHP/fMaxHP are pre-divided by 100. Three early-return paths: bg not
// loaded yet (load+blt, return), sheet not loaded yet (load, return),
// otherwise draw all + set blend states. ALWAYS returns -1 (stay in menu).


int __cdecl hook_SysMenu_GirlStatusCheck_464919(LPDIRECTDRAWSURFACE7 arrSurf[],
  CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo /*unused*/, int nMouseX,
  int nMouseY, int bPressed, int *pnSceneStep, float fCurHP, float fMaxHP,
  QWORD *pSceneFlags)
{
  CHAR szText[256];                      // [esp+5Ch] [ebp-210h]
  int nSubGirlStep, nZone;
  int i, j, nDigit, nCurHP, nMaxHP, nMaxDigits;

  // (tables interleaved as DWORD[n] with x in the odd slot in the original)
  static const int arrOptionRows[5]  = { 0, 3, 3, 4, 4 };   // v82
  static const int arrGirlSlotCnt[5] = { 12, 14, 14, 15, 15 }; // v93
  static const int arrOptionLockPos[4][2] = { {69, 500}, {69, 540}, {69, 580}, {69, 620} };
  static const int arrGirlLockPos[18][2] = {
    {713,  94}, {763, 126}, {813, 158}, {763, 190}, {813, 222}, {763, 254},
    {713, 286}, {763, 318}, {813, 350}, {763, 382}, {713, 414}, {763, 446},
    {813, 478}, {763, 510}, {713, 542}, {763, 574}, {813, 606}, {763, 638},
  };

  ONCELOG("[HNH:SUB] SysMenu_GirlStatusCheck_464919\n");

  nSubGirlStep = *pnSceneStep;
  nZone = -1;
  CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

  if ((BYTE)g_sceneState_52F7E0.bMaxSubStage >= 4)
    g_sceneState_52F7E0.bMaxSubStage = 4;

  // prev/next arrows: cycle the girl step (wrap-around)
  if ((int)ConvertFrom640AsFloat(427.5f) < nMouseY && nMouseY < (int)ConvertFrom640AsFloat(457.5f))
  {
    if ((int)ConvertFrom640AsFloat(442.5f) < nMouseX && nMouseX < (int)ConvertFrom640AsFloat(522.5f))
    {
      nZone = 0;                       // prev
      if (bPressed == 1)
      {
        if (--nSubGirlStep < 0)
          nSubGirlStep = (BYTE)g_sceneState_52F7E0.bMaxSubStage;
        if (g_arrUIEffectDSBuffer_52F168[3])          // "click"
          DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      }
    }
    if ((int)ConvertFrom640AsFloat(522.5f) < nMouseX && nMouseX < (int)ConvertFrom640AsFloat(602.5f))
    {
      nZone = 1;                       // next
      if (bPressed == 1)
      {
        if (++nSubGirlStep > (BYTE)g_sceneState_52F7E0.bMaxSubStage)
          nSubGirlStep = 0;
        if (g_arrUIEffectDSBuffer_52F168[3])
          DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      }
    }
  }
  *pnSceneStep = nSubGirlStep;

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

  // girl background not cached yet -> load it (System31..35.bmp), then return
  if (!arrSurf[nSubGirlStep + 12])
  {
    static const char *const arrGirlBg[5] = { "System31.bmp", "System32.bmp",
      "System33.bmp", "System34.bmp", "System35.bmp" };
    wsprintfA(szText, "%s", arrGirlBg[nSubGirlStep]);     // g_aSystem01Bmp+360+30*n
    CHanaObj__GetRealPath_486D9A(szText, 1007);           // 1007 = GRP dir
    arrSurf[nSubGirlStep + 12] = LoadBmpScaled_4203B3(szText,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[nSubGirlStep + 12], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    return -1;
  }

  BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[nSubGirlStep + 12], 0, 0,
    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);

  // button/lock sheet not cached yet -> load it, then return
  if (!arrSurf[17])
  {
    wsprintfA(szText, "%s", "sys3bt2.bmp");               // nav buttons + lock bars
    CHanaObj__GetRealPath_486D9A(szText, 1007);
    arrSurf[17] = LoadBmpScaled_4203B3(szText,
      (int)ConvertFrom640AsFloat(160.0f), (int)ConvertFrom640AsFloat(50.0f));
    return -1;
  }

  // hovered prev/next arrow
  if (nZone == 0)
    BltFastWH(g_pBackBuffer_4D6C90,
      (int)ConvertFrom640AsFloat(442.5f), (int)ConvertFrom640AsFloat(427.5f),
      arrSurf[17], 0, 0, ConvertFrom640AsInt(80), ConvertFrom640AsInt(30), 0);
  if (nZone == 1)
    BltFastWH(g_pBackBuffer_4D6C90,
      (int)ConvertFrom640AsFloat(522.5f), (int)ConvertFrom640AsFloat(427.5f),
      arrSurf[17], ConvertFrom640AsInt(80), 0,
      ConvertFrom640AsInt(80), ConvertFrom640AsInt(30), 0);

  // whole girl locked -> lock bar at (69,460)
  if (nSubGirlStep && ((1 << (nSubGirlStep - 1)) & g_dwUnlockedSaveSlots_530260) == 0)
    BltFastWH(g_pBackBuffer_4D6C90,
      (int)ConvertFrom640AsFloat(69.0f * 0.625f),
      (int)ConvertFrom640AsFloat(460.0f * 0.625f),
      arrSurf[17], 0, ConvertFrom640AsInt(30),
      (int)ConvertFrom640AsFloat(90.625f), (int)ConvertFrom640AsFloat(17.5f), 0);

  // locked option rows (bit i of pSceneFlags[step]); count-1 iterations
  for (i = 0; i < arrOptionRows[nSubGirlStep] - 1; ++i)
  {
    if (((QWORD)1 << i & pSceneFlags[nSubGirlStep]) == 0)
      BltFastWH(g_pBackBuffer_4D6C90,
        (int)ConvertFrom640AsFloat((float)arrOptionLockPos[i][0] * 0.625f),
        (int)ConvertFrom640AsFloat((float)arrOptionLockPos[i][1] * 0.625f),
        arrSurf[17], 0, ConvertFrom640AsInt(30),
        (int)ConvertFrom640AsFloat(90.625f), (int)ConvertFrom640AsFloat(17.5f), 0);
  }

  // locked girl-status slots (bit 10+i of pSceneFlags[step])
  for (i = 0; i < arrGirlSlotCnt[nSubGirlStep]; ++i)
  {
    if (((QWORD)1 << (i + 10) & pSceneFlags[nSubGirlStep]) == 0)
      BltFastWH(g_pBackBuffer_4D6C90,
        (int)ConvertFrom640AsFloat((float)arrGirlLockPos[i][0] * 0.625f),
        (int)ConvertFrom640AsFloat((float)arrGirlLockPos[i][1] * 0.625f),
        arrSurf[17], 0, ConvertFrom640AsInt(30),
        (int)ConvertFrom640AsFloat(90.625f), (int)ConvertFrom640AsFloat(17.5f), 0);
  }

  // HP readout: cur/max, 5 full-width digits each, right-to-left
  nCurHP = (int)fCurHP / 100;
  nMaxHP = (int)fMaxHP / 100;
  DrawTextOnSurface(g_pBackBuffer_4D6C90,
    (int)ConvertFrom640AsFloat(153.125f), (int)ConvertFrom640AsFloat(53.125f),
    1, "／", 0xC4C4C4);              // "／" @0x4C5954
  if (nCurHP < 1)
    nCurHP = 1;
  for (i = 0; i < 5; ++i)
  {
    if (nCurHP >= IntPow_44AF15(10, i))
    {
      nDigit = nCurHP % IntPow_44AF15(10, i + 1) / IntPow_44AF15(10, i);
      wsprintfA(szText, "０");                    // "０" @0x4C5958
      szText[1] += (CHAR)nDigit;                        // full-width digit
      DrawTextOnSurface(g_pBackBuffer_4D6C90,
        (int)ConvertFrom640AsFloat(153.125f) - (i + 1) * ConvertFrom640AsInt(9),
        (int)ConvertFrom640AsFloat(53.125f), 1, szText,
        nCurHP >= nMaxHP / 3 ? 0xFFFFFF : 0x0000FF);  // white / red
    }
  }
  for (j = 0; j < 5 && nMaxHP >= IntPow_44AF15(10, j); ++j)
    ;
  nMaxDigits = j;
  for (i = 0; i < 5; ++i)
  {
    if (nMaxHP >= IntPow_44AF15(10, i))
    {
      nDigit = nMaxHP % IntPow_44AF15(10, i + 1) / IntPow_44AF15(10, i);
      wsprintfA(szText, "０");
      szText[1] += (CHAR)nDigit;
      DrawTextOnSurface(g_pBackBuffer_4D6C90,
        (int)ConvertFrom640AsFloat(153.125f) + (nMaxDigits + 1) * ConvertFrom640AsInt(9)
        - (i + 1) * ConvertFrom640AsInt(9),
        (int)ConvertFrom640AsFloat(53.125f), 1, szText, 0xFFFFFF);
    }
  }

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
  return -1;                              // always stay in the menu
}

static void DrawTip(const char *szText, int nY)
{
  DrawTextOnSurfaceSpaced_419B05(g_pBackBuffer_4D6C90,
    ConvertFrom640AsInt(180), ConvertFrom640AsInt(nY),
    ConvertFrom640AsInt(-2), const_cast<char *>(szText), 0);
}

void __cdecl hook_SysMenu_Graphics_46380D(int *arrGfxOpts /*unused*/,
  CSceneState *arrSceneStates /*unused*/, LPDIRECTDRAWSURFACE7 arrSurf[],
  CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo /*unused*/,
  int nMouseX, int nMouseY, int bPressed)
{
  CHAR szText[256];                      // [esp+40h] [ebp-158h]
  POINT arrCheckXY[10];                  // [esp+148h] [ebp-50h] unchecked-tile src
  int nCheck, nPreset, i;

  static const int arrChkSrcY[6] = { 0, 114, 38, 152, 76, 190 }; // *0.625 in the original

  ONCELOG("[HNH:SUB] SysMenu_Graphics_46380D\n");

  memset(arrCheckXY, 0, sizeof(arrCheckXY));
  for (i = 0; i < 6; ++i)
    arrCheckXY[i].y = arrChkSrcY[i];

  CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

  // checkbox hit-test (6 rows; plays the click on hover-with-press)
  nCheck = -1;
  for (i = 0; i < 6; ++i)
  {
    if (ConvertFrom640AsInt(216 * (i % 2) + 203) < nMouseX
      && nMouseX < ConvertFrom640AsInt(216 * (i % 2) + 387)
      && ConvertFrom640AsInt(40 * (i / 2) + 92) < nMouseY
      && nMouseY < ConvertFrom640AsInt(40 * (i / 2) + 115))
    {
      if (bPressed == 1 && g_arrUIEffectDSBuffer_52F168[3])  // "click"
        DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      nCheck = i;
      break;
    }
  }
  if (bPressed == 1)
  {
    switch (nCheck)                     // boolean NOT of each flag
    {
    case 0: g_bAntiAliasEnable       = g_bAntiAliasEnable == 0;       break;
    case 1: g_bDitherEnable          = g_bDitherEnable == 0;          break;
    case 2: g_bSpecularEnable_4B3474 = g_bSpecularEnable_4B3474 == 0; break;
    case 3: g_bFogIsActive_4B3480    = !g_bFogIsActive_4B3480;        break;
    case 4: g_bLightEnable_4B3484    = g_bLightEnable_4B3484 == 0;    break;
    case 5: g_bShadowEnable_518CA8   = g_bShadowEnable_518CA8 == 0;   break;
    default: break;
    }
  }

  // preset buttons: 0 speed / 1 standard / 2 quality
  nPreset = -1;
  for (i = 0; i < 3; ++i)
  {
    if (ConvertFrom640AsInt(140 * i + 202) < nMouseX
      && nMouseX < ConvertFrom640AsInt(140 * i + 322)
      && ConvertFrom640AsInt(225) < nMouseY
      && nMouseY < ConvertFrom640AsInt(249))
    {
      nPreset = i;
      if (bPressed == 1)
      {
        if (g_arrUIEffectDSBuffer_52F168[3])
          DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
        if (nPreset == 0)           // speed: all off
        {
          g_bAntiAliasEnable       = 0;
          g_bDitherEnable          = 0;
          g_bSpecularEnable_4B3474 = 0;
          g_bFogIsActive_4B3480    = 0;
          g_bLightEnable_4B3484    = 0;
          g_bShadowEnable_518CA8   = 0;
        }
        else if (nPreset == 1)      // standard
        {
          g_bAntiAliasEnable       = 0;
          g_bDitherEnable          = 1;
          g_bSpecularEnable_4B3474 = 1;
          g_bFogIsActive_4B3480    = 1;
          g_bLightEnable_4B3484    = 1;
          g_bShadowEnable_518CA8   = 0;
        }
        else if (nPreset == 2)      // quality: all on
        {
          g_bAntiAliasEnable       = 1;
          g_bDitherEnable          = 1;
          g_bSpecularEnable_4B3474 = 1;
          g_bFogIsActive_4B3480    = 1;
          g_bLightEnable_4B3484    = 1;
          g_bShadowEnable_518CA8   = 1;
        }
      }
      break;
    }
  }

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

  // background not cached yet -> load System05.bmp, blt, return
  if (!arrSurf[4])
  {
    wsprintfA(szText, "%s", "System05.bmp");
    CHanaObj__GetRealPath_486D9A(szText, 1007);           // 1007 = GRP dir
    arrSurf[4] = LoadBmpScaled_4203B3(szText,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[4], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    return;
  }
  BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[4], 0, 0,
    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);

  // sheet not cached yet -> load kanbt.bmp, return
  if (!arrSurf[10])
  {
    wsprintfA(szText, "%s", "kanbt.bmp");
    CHanaObj__GetRealPath_486D9A(szText, 1007);
    arrSurf[10] = LoadBmpScaled_4203B3(szText,
      ConvertFrom640AsInt(184), ConvertFrom640AsInt(215));
    return;
  }

  // unchecked boxes: redraw the empty-box tile over the background image
  for (i = 0; i < 6; ++i)
  {
    if ((!g_bAntiAliasEnable && !i)
      || (!g_bDitherEnable && i == 1)
      || (!g_bSpecularEnable_4B3474 && i == 2)
      || (!g_bFogIsActive_4B3480 && i == 3)
      || (!g_bLightEnable_4B3484 && i == 4)
      || (!g_bShadowEnable_518CA8 && i == 5))
    {
      BltFastWH(g_pBackBuffer_4D6C90,
        (int)ConvertFrom640AsFloat((double)(i & 1) * 216.25 + 204.375),
        (int)ConvertFrom640AsFloat((double)(i / 2) * 40.0 + 93.125),
        arrSurf[10],
        (int)ConvertFrom640AsFloat((float)arrCheckXY[i].x * 0.625f),
        (int)ConvertFrom640AsFloat((float)arrCheckXY[i].y * 0.625f),
        (int)ConvertFrom640AsFloat(183.125f),
        (int)ConvertFrom640AsFloat(23.125f), 0);
    }
  }
  // hovered/selected preset highlight
  if (nPreset >= 0)
    BltFastWH(g_pBackBuffer_4D6C90,
      (int)ConvertFrom640AsFloat((double)nPreset * 140.0 + 202.5),
      (int)ConvertFrom640AsFloat(225.625f),
      arrSurf[10], 0, ConvertFrom640AsInt(24 * nPreset + 142),
      ConvertFrom640AsInt(120), ConvertFrom640AsInt(24), 0);

  // per-option help text (SJIS byte-exact from .rdata, escape-encoded)
  switch (nCheck)
  {
  case 0:    // "　このスイッチをオンにすると、ポリゴンの角を滑らかにします。"
    DrawTip("　このスイッチをオンにすると、ポリゴンの角を滑らかにします。", 350);
    break;
  case 1:
    DrawTip("　このスイッチをオンにすると、テクスチャーの色を滑らかにします。", 350);
    DrawTip("＊　一部のグラフィックカードのドライバーで T&Lモードの時に、この", 370);
    DrawTip("　スイッチをオンにすると物体の一部が変色してしまう場合があります", 390);
    break;
  case 2:
    DrawTip("　このスイッチをオンにすると、特定の物体に対して光沢を使用します。", 350);
    DrawTip("＊　ライトスイッチがオンの時のみ、このスイッチは効果をあらわします。", 370);
    break;
  case 3:    // fog
    DrawTip("　このスイッチをオンにすると、画面に霧の効果をつけます。", 350);
    break;
  case 4:    // light
    DrawTip("　このスイッチをオンにすると、光と陰の効果をつけます。", 350);
    break;
  case 5:    // shadow
    DrawTip("　このスイッチをオンにすると、影などの効果を描画します。", 350);
    break;
  default:
    break;
  }
  // preset help
  if (nPreset == 0)
    DrawTip("　環境設定スイッチが速度を最優先にした設定になります。", 350);
  else if (nPreset == 1)
    DrawTip("　環境設定スイッチが標準設定になります。", 350);
  else if (nPreset == 2)
    DrawTip("　環境設定スイッチが画質を最優先にした設定になります。", 350);

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
}

// SysMenu_Options_462FE9 -- SystemMenu tab 3 (options). Background
// System04.bmp (lazy into arrSurf[3]) + symbol sheet syoubt.bmp (200x80,
// lazy into arrSurf[11]; 40x20/60x20 symbol cells). sceneStates and pYesNo
// are passed but never used. On a press, hit-tests five option rows and
// writes the picked index straight into the global option bytes:
//   g_mapFrameFlag_530264     2 cells, x 405+60.625*i   y 151.875..171.875
//   g_nEffectLevel_530265     5 cells, x 68+42*i        y 151.875..171.875
//   g_nAviVolumeIndex_530266  5 cells, x 68+42*i        y 336.875..356.875
//   g_nAviFrameMultipler_530267 3 cells, x 363+42*(i+1) y 336.875..356.875
//   g_btoggleC_530268         2 cells, x Conv(413+101i)*0.625,
//                                                   y 244.375..264.375
// any change plays the "click" effect once. The selected symbol of each row
// is redrawn every frame from the sheet (effect/volume/frame rows blit 40x20
// cells at their hit positions; map-frame row a 60x20 cell at 405; toggle a
// 60x20 cell scaled). Note the option globals persist only via Tea.ex for
// the six GRAPHICS flags -- these five are runtime-only (saved games carry
// them inside the 400-byte data<N>.dat instead).

// === PINNED GAME FUNCTIONS ===

// === FUNCTIONS ===
void __cdecl hook_SysMenu_Options_462FE9(CSceneState *sceneStates /*unused*/,
  LPDIRECTDRAWSURFACE7 arrSurf[], CHanaSpriteT3F6_S1FC *pCursor,
  CHanaSpriteT3F6_S1FC *pYesNo /*unused*/, int nMouseX, int nMouseY, int bPressed)
{
  CHAR szFileName[256];                  // [esp+38h] [ebp-110h]
  int i, bChanged;

  ONCELOG("[HNH:SUB] SysMenu_Options_462FE9\n");

  bChanged = 0;
  CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

  if (bPressed == 1)
  {
    // effect level: 5 cells
    for (i = 0; i < 5; ++i)
    {
      if (ConvertFrom640AsInt(42 * i + 68) < nMouseX
        && nMouseX < ConvertFrom640AsInt(42 * (i + 1) + 68)
        && (int)ConvertFrom640AsFloat(151.875f) < nMouseY
        && nMouseY < (int)ConvertFrom640AsFloat(171.875f))
      {
        g_nEffectLevel_530265 = (BYTE)i;
        bChanged = 1;
      }
    }
    // map frame flag: 2 cells
    for (i = 0; i < 2; ++i)
    {
      if (ConvertFrom640AsInt((int)((double)i * 60.625) + 405) < nMouseX
        && nMouseX < ConvertFrom640AsInt((int)((double)(i + 1) * 60.625) + 405)
        && (int)ConvertFrom640AsFloat(151.875f) < nMouseY
        && nMouseY < (int)ConvertFrom640AsFloat(171.875f))
      {
        g_mapFrameFlag_530264 = (BYTE)i;
        bChanged = 1;
      }
    }
    // AVI/BGM volume: 5 cells
    for (i = 0; i < 5; ++i)
    {
      if (ConvertFrom640AsInt(42 * i + 68) < nMouseX
        && nMouseX < ConvertFrom640AsInt(42 * (i + 1) + 68)
        && (int)ConvertFrom640AsFloat(336.875f) < nMouseY
        && nMouseY < (int)ConvertFrom640AsFloat(356.875f))
      {
        g_nAviVolumeIndex_530266 = (BYTE)i;
        bChanged = 1;
      }
    }
    // AVI frame multiplier: 3 cells
    for (i = 0; i < 3; ++i)
    {
      if (ConvertFrom640AsInt(42 * (i + 1) + 363) < nMouseX
        && nMouseX < ConvertFrom640AsInt(42 * (i + 2) + 363)
        && (int)ConvertFrom640AsFloat(336.875f) < nMouseY
        && nMouseY < (int)ConvertFrom640AsFloat(356.875f))
      {
        g_nAviFrameMultipler_530267 = (BYTE)i;
        bChanged = 1;
      }
    }
    // toggle C: 2 cells (1024-space x, scaled 0.625 then ConvertFrom640)
    for (i = 0; i < 2; ++i)
    {
      if ((int)(ConvertFrom640AsFloat((double)i * 101.0 + 413.0) * 0.625) < nMouseX
        && nMouseX < (int)(ConvertFrom640AsFloat((double)(i + 1) * 101.0 + 413.0) * 0.625)
        && (int)ConvertFrom640AsFloat(244.375f) < nMouseY
        && nMouseY < (int)ConvertFrom640AsFloat(264.375f))
      {
        g_btoggleC_530268 = (BYTE)i;
        bChanged = 1;
      }
    }
    if (bChanged == 1 && g_arrUIEffectDSBuffer_52F168[3])   // "click"
      DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
  }

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

  // background not cached yet -> load System04.bmp, blt, return
  if (!arrSurf[3])
  {
    wsprintfA(szFileName, "%s", "System04.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);        // 1007 = GRP dir
    arrSurf[3] = LoadBmpScaled_4203B3(szFileName,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[3], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    return;
  }
  BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[3], 0, 0,
    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);

  // symbol sheet not cached yet -> load it, return
  if (!arrSurf[11])
  {
    wsprintfA(szFileName, "%s", "syoubt.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    arrSurf[11] = LoadBmpScaled_4203B3(szFileName,
      (int)ConvertFrom640AsFloat(200.0f), (int)ConvertFrom640AsFloat(80.0f));
    return;
  }

  // redraw the selected symbol of each option row
  BltFastWH(g_pBackBuffer_4D6C90,                         // effect level (40x20)
    ConvertFrom640AsInt(42 * g_nEffectLevel_530265 + 68),
    (int)ConvertFrom640AsFloat(151.25f),
    arrSurf[11], ConvertFrom640AsInt(40 * g_nEffectLevel_530265), 0,
    ConvertFrom640AsInt(40), ConvertFrom640AsInt(20), 0);
  BltFastWH(g_pBackBuffer_4D6C90,                         // map frame (60x20 @405)
    (int)(ConvertFrom640AsFloat(62.5f) * g_mapFrameFlag_530264)
    + ConvertFrom640AsInt(405),
    (int)ConvertFrom640AsFloat(151.25f),
    arrSurf[11], (int)ConvertFrom640AsFloat(61.25f * g_mapFrameFlag_530264),
    ConvertFrom640AsInt(40), (int)ConvertFrom640AsFloat(60.0f),
    ConvertFrom640AsInt(20), 0);
  BltFastWH(g_pBackBuffer_4D6C90,                         // volume (40x20)
    ConvertFrom640AsInt(42 * g_nAviVolumeIndex_530266 + 68),
    (int)ConvertFrom640AsFloat(336.25f),
    arrSurf[11], ConvertFrom640AsInt(40 * g_nAviVolumeIndex_530266), 0,
    ConvertFrom640AsInt(40), ConvertFrom640AsInt(20), 0);
  BltFastWH(g_pBackBuffer_4D6C90,                         // frame multiplier (40x20)
    ConvertFrom640AsInt(42 * (g_nAviFrameMultipler_530267 + 1) + 362),
    (int)ConvertFrom640AsFloat(336.25f),
    arrSurf[11], ConvertFrom640AsInt(40 * g_nAviFrameMultipler_530267), 0,
    ConvertFrom640AsInt(40), ConvertFrom640AsInt(20), 0);
  BltFastWH(g_pBackBuffer_4D6C90,                         // toggle C (60x20, scaled)
    (int)(ConvertFrom640AsFloat((double)g_btoggleC_530268 * 101.0 + 413.0) * 0.625),
    (int)ConvertFrom640AsFloat(244.375f),
    arrSurf[11], ConvertFrom640AsInt(60 * g_btoggleC_530268),
    ConvertFrom640AsInt(60), ConvertFrom640AsInt(60),
    ConvertFrom640AsInt(20), 0);

  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
  if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
  {
    DrawSprite2D(pCursor);              // this tab also draws the cursor itself
    g_pD3DDevice_4D6C78->EndScene();
  }
}

// SysMenu_SaveMenu_462135 -- SystemMenu tab 0 (save). 5x2 grid of 10 diary
// slots over System01.bmp (lazy-loaded into arrSurf[0]); slots 7..10 are
// gated by g_dwUnlockedSaveSlots_530260 bits (locked cells are blacked out
// with BltSurface after drawing). Hover highlights the 87x87 cell
// (arrSurf[9]); the hovered diary's name is centered at y 326 and 7 info
// lines at y 339+13i -- the line block is chosen by the first
// g_arrSceneStepThresholds_4B5970 entry exceeding the slot's
// bActiveSceneStep (g_szSceneInfoText_4B5A10: 1600-byte blocks, 80-byte
// rows); a status line at y 430 shows （探索中）/（狩猟中）/（暴行中） by
// scene type 2/3/4. Click: "click" SFX + ShowYesNoDialog
// "＜N＞番目の日記にセーブしますか？" -- the full-width digit is patched by
// adding the slot number to szText[1] ('０' low byte 0x4F); slot 10 has its
// own literal. On Yes: data<N>.dat (400 bytes of g_sceneState) written to
// the DATA dir, the slot copy synced, LoadDataFiles re-run, and the fade
// dialog "セーブが完了しました。" shown. (SJIS strings escape-encoded.)


// === FUNCTIONS ===
void __cdecl hook_SysMenu_SaveMenu_462135(CSceneState sceneStates[], CHAR *arrSceneName,
  LPDIRECTDRAWSURFACE7 arrSurf[], CHanaSpriteT3F6_S1FC *pCursor,
  CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY, int bPressed)
{
  CHAR szText[256];                      // [esp+14h] [ebp-10Ch]
  int nHoverCell, nConfirm;              // v44 / v45
  int i, j;

  ONCELOG("[HNH:SUB] SysMenu_SaveMenu_462135\n");

  nHoverCell = 0;
  nConfirm = 0;
  CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

  // 5x2 slot grid; slots 7..10 unlocked by g_dwUnlockedSaveSlots bits
  for (i = 0; i < 10; ++i)
  {
    if ((i < 6 || ((1 << (i - 6)) & g_dwUnlockedSaveSlots_530260) != 0)
      && ConvertFrom640AsInt(((i % 5) << 7) + 20) <= nMouseX
      && nMouseX < ConvertFrom640AsInt(((i % 5) << 7) + 107)
      && ConvertFrom640AsInt(124 * (i / 5) + 59) <= nMouseY
      && nMouseY < ConvertFrom640AsInt(124 * (i / 5) + 146))
    {
      nHoverCell = i + 1;            // 1-based diary slot
      break;
    }
  }

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
  if (arrSurf[0])
  {
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[0], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    if (nHoverCell && arrSurf[9])      // hovered-cell highlight
      BltFastWH(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt((((nHoverCell - 1) % 5) << 7) + 20),
        ConvertFrom640AsInt(124 * ((nHoverCell - 1) / 5) + 59),
        arrSurf[9], 0, 0,
        ConvertFrom640AsInt(87), ConvertFrom640AsInt(87), 0);
  }
  else
  {
    // first visit: load the tab background
    wsprintfA(szText, "%s", "System01.bmp");
    CHanaObj__GetRealPath_486D9A(szText, 1007);        // 1007 = GRP dir
    arrSurf[0] = LoadBmpScaled_4203B3(szText,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[0], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
  }

  if (nHoverCell > 0)
  {
    // diary name, centered at y 326
    DrawTextOnSurface(g_pBackBuffer_4D6C90,
      ConvertFrom640AsInt(320)
      - (lstrlenA(&arrSceneName[50 * nHoverCell - 50]) / 2)
      * ConvertFrom640AsInt(15) / 2,
      ConvertFrom640AsInt(326), 1,
      &arrSceneName[50 * nHoverCell - 50], 0xF0F0F);

    // 7 info lines; block picked by the slot's scene step
    for (i = 0; i < 7; ++i)
    {
      for (j = 0; j < 39
        && g_arrSceneStepThresholds_4B5970[j] <= sceneStates[nHoverCell - 1].bActiveSceneStep;
        ++j)
        ;
      wsprintfA(szText, "%s", g_szSceneInfoText_4B5A10[j][i]);
      DrawTextOnSurface(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(320)
        - (lstrlenA(szText) / 2) * ConvertFrom640AsInt(15) / 2,
        ConvertFrom640AsInt(13 * i + 339), 1, szText, 0xF0F0F);
    }

    // status line at y 430 by scene type
    szText[0] = '\0';
    switch (g_arrSceneNo_4C6F78[sceneStates[nHoverCell - 1].bActiveSceneStep].nSceneType)
    {
    case 2:                             // "（探索中）" exploring
      wsprintfA(szText, "　　　　　　　　　　　（探索中）");
      break;
    case 3:                             // "（狩猟中）" hunting
      wsprintfA(szText, "　　　　　　　　　　　（狩猟中）");
      break;
    case 4:                             // "（暴行中）" assault
      wsprintfA(szText, "　　　　　　　　　　　（暴行中）");
      break;
    default:
      break;
    }
    DrawTextOnSurface(g_pBackBuffer_4D6C90,
      ConvertFrom640AsInt(320)
      - (lstrlenA(szText) / 2) * ConvertFrom640AsInt(15) / 2,
      ConvertFrom640AsInt(430), 1, szText, 0xF0F0F);
  }

  // black out the locked slots 7..10
  for (i = 6; i < 10; ++i)
  {
    if (((1 << (i - 6)) & g_dwUnlockedSaveSlots_530260) == 0)
      BltSurface(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(((i % 5) << 7) + 20),
        ConvertFrom640AsInt(124 * (i / 5) + 59),
        ConvertFrom640AsInt(((i % 5) << 7) + 20) + ConvertFrom640AsInt(87),
        ConvertFrom640AsInt(124 * (i / 5) + 59) + ConvertFrom640AsInt(87),
        0);
  }
  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

  // click a slot: confirm, then save
  if (bPressed == 1 && nHoverCell)
  {
    if (g_arrUIEffectDSBuffer_52F168[3])              // "click"
      DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
    if (nHoverCell == 10)
    {
      // "１０番目の日記にセーブしますか？"
      wsprintfA(szText, "１０番目の日記にセーブしますか？");
    }
    else
    {
      // "０番目の日記にセーブしますか？" -- then patch the full-width
      // digit: szText[1] ('０' low byte 0x4F) += slot number
      wsprintfA(szText, "０番目の日記にセーブしますか？");
      szText[1] += (CHAR)nHoverCell;
    }
    nConfirm = ShowYesNoDialog_461A9E(pCursor, pYesNo, szText);
  }

  if (nConfirm == 1)
  {
    wsprintfA(szText, "data%d.dat", nHoverCell);
    CHanaObj__GetRealPath_486D9A(szText, 1010);        // 1010 = DATA dir
    WriteBufferToFile(szText, &g_sceneState_52F7E0, 400);
    memcpy(&sceneStates[nHoverCell - 1], &g_sceneState_52F7E0, sizeof(CSceneState));
    nConfirm = 0;
    LoadDataFiles_461557(sceneStates, arrSceneName);
    // "セーブが完了しました。"
    ShowFadeDialog_46164C(pCursor, pYesNo,
      "セーブが完了しました。");
  }
}

// SysMenu_LoadMenu_462851 -- SystemMenu tab 1 (load). Structurally the twin
// of hook_SysMenu_SaveMenu_462135 (same 5x2 grid, same slot gating via
// g_dwUnlockedSaveSlots_530260, same info-line/status rendering), with:
//   - background System02.bmp lazy-loaded into arrSurf[1] (and the alpha
//     blend render states re-set after the lazy load)
//   - clicking an EMPTY slot (bActiveSceneStep == 0) only plays the "click2"
//     error effect -- no dialog; non-empty slots get the "click" effect +
//     ShowYesNoDialog "＜N＞番目の日記をロードしますか？" (same full-width
//     digit patch: szText[1] ('０' low byte 0x4F) += slot; slot 10 literal)
//   - on Yes: data<N>.dat READ into g_sceneState (400 bytes), the slot copy
//     synced, fade dialog "ロードが完了しました。", and the function
//     RETURNS 2 = scene selected (SystemMenu exits with it); not confirmed
//     returns -1 (keep drawing). No LoadDataFiles refresh (unlike save).
// (SJIS strings escape-encoded.)

// === PINNED GAME FUNCTIONS ===

// === FUNCTIONS ===
int __cdecl hook_SysMenu_LoadMenu_462851(CSceneState arrScene[], CHAR arrSceneName[],
  LPDIRECTDRAWSURFACE7 arrSurf[], CHanaSpriteT3F6_S1FC *pCursor,
  CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY, int bPressed)
{
  CHAR szText[256];                      // [esp+14h] [ebp-10Ch]
  int nHoverCell, nConfirm;              // saveSlot / v46
  int i, j;

  ONCELOG("[HNH:SUB] SysMenu_LoadMenu_462851\n");

  nHoverCell = 0;
  nConfirm = 0;
  CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

  // 5x2 slot grid; slots 7..10 unlocked by g_dwUnlockedSaveSlots bits
  for (i = 0; i < 10; ++i)
  {
    if ((i < 6 || ((1 << (i - 6)) & g_dwUnlockedSaveSlots_530260) != 0)
      && ConvertFrom640AsInt(((i % 5) << 7) + 20) <= nMouseX
      && nMouseX < ConvertFrom640AsInt(((i % 5) << 7) + 107)
      && ConvertFrom640AsInt(124 * (i / 5) + 59) <= nMouseY
      && nMouseY < ConvertFrom640AsInt(124 * (i / 5) + 146))
    {
      nHoverCell = i + 1;            // 1-based diary slot
      break;
    }
  }

  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
  if (arrSurf[1])
  {
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[1], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    if (nHoverCell && arrSurf[9])      // hovered-cell highlight
      BltFastWH(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt((((nHoverCell - 1) % 5) << 7) + 20),
        ConvertFrom640AsInt(124 * ((nHoverCell - 1) / 5) + 59),
        arrSurf[9], 0, 0,
        ConvertFrom640AsInt(87), ConvertFrom640AsInt(87), 0);
  }
  else
  {
    // first visit: load the tab background
    wsprintfA(szText, "%s", "System02.bmp");
    CHanaObj__GetRealPath_486D9A(szText, 1007);        // 1007 = GRP dir
    arrSurf[1] = LoadBmpScaled_4203B3(szText,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[1], 0, 0,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
  }

  if (nHoverCell > 0)
  {
    // diary name, centered at y 326
    DrawTextOnSurface(g_pBackBuffer_4D6C90,
      ConvertFrom640AsInt(320)
      - (lstrlenA(&arrSceneName[50 * nHoverCell - 50]) / 2)
      * ConvertFrom640AsInt(15) / 2,
      ConvertFrom640AsInt(326), 1,
      &arrSceneName[50 * nHoverCell - 50], 0xF0F0F);

    // 7 info lines; block picked by the slot's scene step
    for (i = 0; i < 7; ++i)
    {
      for (j = 0; j < 39
        && g_arrSceneStepThresholds_4B5970[j] <= arrScene[nHoverCell - 1].bActiveSceneStep;
        ++j)
        ;
      wsprintfA(szText, "%s", g_szSceneInfoText_4B5A10[j][i]);
      DrawTextOnSurface(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(320)
        - (lstrlenA(szText) / 2) * ConvertFrom640AsInt(15) / 2,
        ConvertFrom640AsInt(13 * i + 339), 1, szText, 0xF0F0F);
    }

    // status line at y 430 by scene type
    szText[0] = '\0';
    switch (g_arrSceneNo_4C6F78[arrScene[nHoverCell - 1].bActiveSceneStep].nSceneType)
    {
    case 2:                             // "（探索中）" exploring
      wsprintfA(szText, "　　　　　　　　　　　（探索中）");
      break;
    case 3:                             // "（狩猟中）" hunting
      wsprintfA(szText, "　　　　　　　　　　　（狩猟中）");
      break;
    case 4:                             // "（暴行中）" assault
      wsprintfA(szText, "　　　　　　　　　　　（暴行中）");
      break;
    default:
      break;
    }
    DrawTextOnSurface(g_pBackBuffer_4D6C90,
      ConvertFrom640AsInt(320)
      - (lstrlenA(szText) / 2) * ConvertFrom640AsInt(15) / 2,
      ConvertFrom640AsInt(430), 1, szText, 0xF0F0F);
  }

  // black out the locked slots 7..10
  for (i = 6; i < 10; ++i)
  {
    if (((1 << (i - 6)) & g_dwUnlockedSaveSlots_530260) == 0)
      BltSurface(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(((i % 5) << 7) + 20),
        ConvertFrom640AsInt(124 * (i / 5) + 59),
        ConvertFrom640AsInt(((i % 5) << 7) + 20) + ConvertFrom640AsInt(87),
        ConvertFrom640AsInt(124 * (i / 5) + 59) + ConvertFrom640AsInt(87),
        0);
  }
  FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

  // click a slot: empty -> error effect; filled -> confirm, then load
  if (bPressed == 1 && nHoverCell)
  {
    if (arrScene[nHoverCell - 1].bActiveSceneStep)
    {
      if (g_arrUIEffectDSBuffer_52F168[3])          // "click"
        DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      if (nHoverCell == 10)
      {
        // "１０番目の日記をロードしますか？"
        wsprintfA(szText, "１０番目の日記をロードしますか？");
      }
      else
      {
        // "０番目の日記をロードしますか？" -- then patch the
        // full-width digit: szText[1] ('０' low byte 0x4F) += slot
        wsprintfA(szText, "０番目の日記をロードしますか？");
        szText[1] += (CHAR)nHoverCell;
      }
      nConfirm = ShowYesNoDialog_461A9E(pCursor, pYesNo, szText);
    }
    else if (g_arrUIEffectDSBuffer_52F168[4])         // "click2" (0x52F178)
    {
      DSBufferPlay(g_arrUIEffectDSBuffer_52F168[4], 0);
    }
  }

  if (nConfirm != 1)
    return -1;                          // keep drawing

  wsprintfA(szText, "data%d.dat", nHoverCell);
  CHanaObj__GetRealPath_486D9A(szText, 1010);            // 1010 = DATA dir
  ReadFileToBuffer(szText, &g_sceneState_52F7E0, 400);
  memcpy(&arrScene[nHoverCell - 1], &g_sceneState_52F7E0, sizeof(CSceneState));
  nConfirm = 0;
  // "ロードが完了しました。"
  ShowFadeDialog_46164C(pCursor, pYesNo,
    "ロードが完了しました。");
  return 2;                               // scene selected
}

// ShowYesNoDialog_461A9E -- modal YES/NO confirmation used by the system
// menu (save/load confirm, quit confirm). pDialog is the YesNo1+YesNo2.bmp
// masked sprite (SystemMenu loads it): sheet rows -- button 1 src
// {0,0,127,31}, button 2 src {0,32,127,63}, dialog panel src
// {0,64,Conv(640),127} drawn over dest (0, Conv(210))..(Conv(640),
// Conv(430)). Button centers (220,350) and (420,350), hit boxes +-64 x,
// +-16 y; the hovered button pulses (alpha sin(t/500)/2+0.75, scale 1.2),
// non-hovered scale 1.0. Font switches to ＭＳ 明朝 26x26 for the caption
// (centered at (320,230), white) and back to 15x15 after. The back buffer
// is snapshotted into a 640x480 temp surface and restored each frame.
// Fade: alpha = (now-t0)/500 in, 1-(now-t0)/500 out after a click; the
// loop pumps until a click happened AND the fade-out finished. Mouse input
// is drained from the 30-entry CInputState ring (any button==1 counts as
// the press, all entries cleared). Returns the chosen button: 1 or 2.
// (The "On" effect plays on click.)

// === FUNCTIONS ===
int __cdecl hook_ShowYesNoDialog_461A9E(CHanaSpriteT3F6_S1FC *pCursor,
  CHanaSpriteT3F6_S1FC *pDialog, LPCSTR szCaption)
{
  LPDIRECTDRAWSURFACE7 pSnapSurf;        // [esp+40h] [ebp-70h]
  RECT arrBtnRect[2];                    // [esp+7Ch] [ebp-34h] button src rects
  RECT rcDialog;                         // [esp+9Ch] [ebp-14h] panel src rect
  CInputState *pInput;
  DWORD dwTimeStart, dwTimeNow;
  int dlgT, dlgR, dlgB;                  // dialog dest rect (L = 0)
  POINT arrBtnCenter[2];
  float fSavedDlgAlpha, fFadeAlpha;
  int i, j, k, nBtn, bClicked, bAnyBtnDown;

  static const int arrBtnSrc[2][4] = {
    { 0,  0, 127, 31 },                // (interleaved as RECT + 4 ints in the original)
    { 0, 32, 127, 63 },
  };

  ONCELOG("[HNH:SUB] ShowYesNoDialog_461A9E\n");

  bClicked = 0;
  nBtn = 0;
  dlgT = ConvertFrom640AsInt(210);
  dlgR = ConvertFrom640AsInt(640);
  dlgB = ConvertFrom640AsInt(430);
  arrBtnCenter[0].x = ConvertFrom640AsInt(220);
  arrBtnCenter[0].y = ConvertFrom640AsInt(350);
  arrBtnCenter[1].x = ConvertFrom640AsInt(420);
  arrBtnCenter[1].y = ConvertFrom640AsInt(350);
  for (k = 0; k < 2; ++k)
  {
    arrBtnRect[k].left   = arrBtnSrc[k][0];
    arrBtnRect[k].top    = arrBtnSrc[k][1];
    arrBtnRect[k].right  = arrBtnSrc[k][2];
    arrBtnRect[k].bottom = arrBtnSrc[k][3];
  }
  rcDialog.left   = 0;
  rcDialog.top    = 64;
  rcDialog.right  = ConvertFrom640AsInt(640);
  rcDialog.bottom = 127;

  // snapshot the current frame; dialog font
  pSnapSurf = CreateSurfaceWH(ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
  BltFastWH(pSnapSurf, 0, 0, g_pBackBuffer_4D6C90, 0, 0,
    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
  SetUiFont_419BCD(ConvertFrom640AsInt(26), ConvertFrom640AsInt(26),
    "ＭＳ 明朝");  // aMs_3 @0x4C5468: "ＭＳ 明朝"

  dwTimeStart = timeGetTime();
  do
  {
    DoPeekMessages();
    dwTimeNow = timeGetTime();

    // drain the 30-entry input ring: any button press counts
    MutexThreadWait_4815EC(TRUE);
    pInput = GetInputState_481B37();
    bAnyBtnDown = 0;
    for (i = 0; i < 30; ++i)
    {
      if (pInput[i].mouseBtn_4 == 1)
        bAnyBtnDown = 1;
      pInput[i].mouseBtn_4 = 0;
    }
    CHanaSpriteT3F6__SetPos_414A5D(pCursor,
      (float)(pInput->mouseX_0 + 16), (float)(pInput->mouseY_2 + 16));

    // button hit-test (only while fully faded in, before any click)
    if (pDialog->fAlpha_144 == 1.0f && !bClicked)
    {
      nBtn = 0;
      for (j = 0; j < 2; ++j)
      {
        if (arrBtnCenter[j].x - 64 < pInput->mouseX_0
          && pInput->mouseX_0 < arrBtnCenter[j].x + 64
          && arrBtnCenter[j].y - 16 < pInput->mouseY_2
          && pInput->mouseY_2 < arrBtnCenter[j].y + 16)
        {
          nBtn = j + 1;
          if (bAnyBtnDown == 1)
          {
            if (g_arrUIEffectDSBuffer_52F168[2])  // "On" click
              DSBufferPlay(g_arrUIEffectDSBuffer_52F168[2], 0);
            bClicked = 1;
            dwTimeStart = dwTimeNow;             // restart the fade timer
          }
        }
      }
    }
    MutexThreadWait_4815EC(FALSE);

    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    if (pSnapSurf)                       // restore the snapshot underneath
      BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pSnapSurf, 0, 0,
        ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

    if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
    {
      // dialog panel: fade in (t/500) or out (1 - t/500) after the click
      if (bClicked)
        fFadeAlpha = 1.0f - (float)(dwTimeNow - dwTimeStart) / 500.0f;
      else
        fFadeAlpha = (float)(dwTimeNow - dwTimeStart) / 500.0f;
      CHanaObj__SetAlpha3F6(pDialog, fFadeAlpha);
      CHanaSpriteT3F6__SetSrcRect_414B28(pDialog, &rcDialog);
      DrawSpriteLTRB(pDialog, 0, dlgT, dlgR, dlgB);

      // the two buttons; the hovered one pulses
      for (k = 0; k < 2; ++k)
      {
        fSavedDlgAlpha = (float)CHanaSpriteT3F6__GetAlpha_414AFB(pDialog);
        if (nBtn - 1 != k || bClicked)
        {
          CHanaSpriteT3F6__SetScale_414B09(pDialog,
            (float)(int)ConvertFrom640AsFloat(1.0f));
        }
        else
        {
          CHanaObj__SetAlpha3F6(pDialog,
            (float)(sin((double)(dwTimeNow - dwTimeStart) / 500.0) / 2.0 + 0.75));
          CHanaSpriteT3F6__SetScale_414B09(pDialog,
            (float)(int)ConvertFrom640AsFloat(1.2f));
        }
        CHanaSpriteT3F6__SetPos_414A5D(pDialog,
          (float)arrBtnCenter[k].x, (float)arrBtnCenter[k].y);
        CHanaSpriteT3F6__SetSrcRect_414B28(pDialog, &arrBtnRect[k]);
        DrawSprite2D(pDialog);
        CHanaObj__SetAlpha3F6(pDialog, fSavedDlgAlpha);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }

    // caption once fully visible
    if (pDialog->fAlpha_144 == 1.0f)
    {
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      DrawTextOnSurface(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(320) - lstrlenA(szCaption) * ConvertFrom640AsInt(26) / 4,
        ConvertFrom640AsInt(230), 1, szCaption, 0xFFFFFF);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    }

    // cursor on top while anything is visible
    if (pDialog->fAlpha_144 > 0.0f && g_pD3DDevice_4D6C78->BeginScene() >= 0)
    {
      DrawSprite2D(pCursor);
      g_pD3DDevice_4D6C78->EndScene();
    }
    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  while (bClicked != 1 || pDialog->fAlpha_144 > 0.0f);

  if (pSnapSurf)
    pSnapSurf->Release();
  SetUiFont_419BCD(ConvertFrom640AsInt(15), ConvertFrom640AsInt(15),
    "ＭＳ 明朝");  // aMs_2 @0x4C5474: "ＭＳ 明朝"
  return nBtn;                             // 1 or 2
}


void register_SystemMenu_4655AB(int cfg) {
  if (cfg == 1) {
    SystemMenu_4655AB = hook_SystemMenu_4655AB;
    SysMenu_GirlStatusCheck_464919 = hook_SysMenu_GirlStatusCheck_464919;
    SysMenu_Graphics_46380D = hook_SysMenu_Graphics_46380D;
    SysMenu_Options_462FE9 = hook_SysMenu_Options_462FE9;
    SysMenu_SaveMenu_462135 = hook_SysMenu_SaveMenu_462135;
    SysMenu_LoadMenu_462851 = hook_SysMenu_LoadMenu_462851;
    ShowYesNoDialog_461A9E = hook_ShowYesNoDialog_461A9E;

  }

  hnh_HookOff();
  hnh_Hook(0x4655AB, hook_SystemMenu_4655AB, "SystemMenu_4655AB"); // [OK]
  hnh_Hook(0x464919, hook_SysMenu_GirlStatusCheck_464919, "SysMenu_GirlStatusCheck_464919");  // [OK]
  hnh_Hook(0x46380D, hook_SysMenu_Graphics_46380D, "SysMenu_Graphics_46380D");  // [OK]
  hnh_Hook(0x462FE9, hook_SysMenu_Options_462FE9, "SysMenu_Options_462FE9");  // [OK]
  hnh_Hook(0x462135, hook_SysMenu_SaveMenu_462135, "SysMenu_SaveMenu_462135");  // [OK]
  hnh_Hook(0x462851, hook_SysMenu_LoadMenu_462851, "SysMenu_LoadMenu_462851");  // [OK]
  hnh_Hook(0x461A9E, hook_ShowYesNoDialog_461A9E, "ShowYesNoDialog_461A9E");  // [OK]

  hnh_HookOn();
}