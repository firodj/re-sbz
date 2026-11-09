// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*SystemMenu_4655AB)(LPDIRECTSOUNDBUFFER pBgm, int nVolOffset, int nMenuMode,
    float fCurHP, float fMaxHP, CSceneState sceneState) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER, int, int, float, float, CSceneState)>(0x4655AB);

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
//     parked at (-100,-100), then ShowYesNoDialog "ÉQÅ[ÉÄÇèIóπÇµÇ‹Ç∑Ç©ÅH"
//     -> Yes returns 1. Tab switches play the "On" effect; right-click
//     exits with 0 ("click2" effect).
// Assets: Systembk.bmp background (87x87 scaled, arrSurf[9]), mouse.bmp
// cursor, YesNo1.bmp + YesNo2.bmp dialog sprite (SPR dir). Entry captures
// the primary into a temp surface and fades OUT on it; the first loop frame
// fades back IN on the back buffer.
// Graphics options persist through <exe-dir>\Tea.ex (128 bytes: fog,
// specular, antialias, dither, light, shadow) -- read into the g_b* globals
// on entry, written back on exit. Font switched to ÇlÇr ñæí© 15x15 for the
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
                     "ÇlÇr ñæí©");   // aMs_1 @0x4C5960: "ÇlÇr ñæí©"
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
                        "ÉQÅ[ÉÄÇèIóπÇµÇ‹Ç∑Ç©ÅH")  // asc_4C599C: "ÉQÅ[ÉÄÇèIóπÇµÇ‹Ç∑Ç©ÅH"
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
    SetUiFont_419BCD(26, 26, "ÇlÇr ñæí©");  // restore ÇlÇr ñæí© 26x26
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

void register_SystemMenu_4655AB() {
  SystemMenu_4655AB = hook_SystemMenu_4655AB;

  hnh_HookOff();
    hnh_Hook(0x4655AB, hook_SystemMenu_4655AB, "SystemMenu_4655AB"); // [OK]
  hnh_HookOn();
}
