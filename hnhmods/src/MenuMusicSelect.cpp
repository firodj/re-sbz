#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// MenuMusicSelect_47D163 -- MainMenu state -1: the music/sound selection
// screen over mu.bmp. Three sprites (all GRP dir, 1007): mu.bmp background
// (full screen), mub.bmp button-highlight strip (400x72 = four ~200x36
// frames), mub2.bmp indicator sheet (832x200 = 2x7 grid of 413x28 label
// cells, one per sound).
//
// 14 sounds (g_ascSndFileNames_4C78A0[1..14], String50[200] table) in a 7x2
// grid: cell i hits at x = Conv1024((i-1)/7*468 + 72) (+413) and
// y = Conv1024((i-1)%7*36 + 217) (+27); clicking selects it (nSoundIndex
// starts at 4). Buttons: 0 stop (288..486 x 612..648), 1 play (522..720 x
// 612..648 -- loads the wav through the WAV dir (1006) and plays it LOOPING
// at the volume-table level), 2 prev (341..496 x 666..702, floor 1), 3 next
// (512..667 x 666..702, cap 14). Every click plays the "click" effect
// (arrUIEffect[3], a.k.a. g_pDSBuf_52F174).
// Top bar: the same four 128-wide columns as MenuCharSelect (-1..-4), but
// only nResult < -1 exits here (-1 would re-enter this very screen). The
// menu key opens SystemMenu mode 2; >= 1 from there exits with that value.

// 50-char sound-name table entry (g_ascSndFileNames_4C78A0 is [200] of
// these; only 1..14 are used by this screen)
struct String50 { char str[50]; };

// === PINNED GAME FUNCTIONS ===
int (__cdecl*MenuMusicSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47D163);

// === PINNED GLOBALS ===
String50 (&g_ascSndFileNames_4C78A0)[200] = *(String50(*)[200])0x4C78A0;  // READ-ONLY

// === FUNCTIONS ===
int __cdecl hook_MenuMusicSelect(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
    LPDIRECTDRAWSURFACE7 pBmpIndicator;     // [esp+190h] [ebp-124h]
    LPDIRECTDRAWSURFACE7 pBmpBackground;    // [esp+194h] [ebp-120h]
    LPDIRECTDRAWSURFACE7 pBmpHighlight;     // [esp+198h] [ebp-11Ch]
    CHAR szFileName[256];                   // [esp+19Ch] [ebp-118h]
    CSceneState sceneScratch;               // [ebp-394h] SystemMenu arg copy
    RECT rc;                                // [ebp-18Ch] indicator source cell
    RECT arrHighlightRects[5];              // [ebp-17Ch] mub.bmp frames
    DWORD dwAntiAliasCur;                   // [ebp-1B8h]
    int nResult, nHoverCtrl, nSoundIndex, i;
    int xMouse, yMouse;                     // [ebp-8h] / [ebp-Ch]
    WORD nMouseBtn = 0, nPrevMouseBtn;          // [ebp-4h] / [ebp-18h]

    DebugLog("[HNH:SUB] MenuMusicSelect_47D163\n");

    nResult = 0;
    nSoundIndex = 4;

    wsprintfA(szFileName, "mu.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);            // 1007 = GRP dir
    pBmpBackground = LoadBmpScaled_4203B3(szFileName,
            ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    wsprintfA(szFileName, "mub.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    pBmpHighlight = LoadBmpScaled_4203B3(szFileName,
            (int)ConvertFrom1024AsFloat(400.0f), (int)ConvertFrom1024AsFloat(72.0f));
    wsprintfA(szFileName, "mub2.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    pBmpIndicator = LoadBmpScaled_4203B3(szFileName,
            (int)ConvertFrom1024AsFloat(832.0f), (int)ConvertFrom1024AsFloat(200.0f));

    g_sceneState_52F7E0.bActiveSceneStep = 0;
    StopAndRestartThread(2);

    for (;;)
    {
        DoPeekMessages();

        // menu key -> SystemMenu (mode 2)
        if (IsMenuKeyPressed_481170())
        {
            StopAndRestartThread(0);
            memcpy(&sceneScratch, &g_sceneState_52F7E0, sizeof(sceneScratch));
            nResult = SystemMenu_4655AB(*ppBgmBuffer, -1000, 2, 666.0f, 666.0f,
                                         sceneScratch);
            StopAndRestartThread(2);
            if (nResult >= 1)
                break;
        }

        nPrevMouseBtn = nMouseBtn;
        GetMouseBtn_45A41B(&nMouseBtn);
        GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

        // button hitboxes (1024x768 design space)
        nHoverCtrl = -1;
        if (yMouse >= (int)ConvertFrom1024AsFloat(612.0f) && yMouse < (int)ConvertFrom1024AsFloat(648.0f))
        {
            if (xMouse >= (int)ConvertFrom1024AsFloat(288.0f) && xMouse < (int)ConvertFrom1024AsFloat(486.0f))
                nHoverCtrl = 0;                                // stop
            else if (xMouse >= (int)ConvertFrom1024AsFloat(522.0f) && xMouse < (int)ConvertFrom1024AsFloat(720.0f))
                nHoverCtrl = 1;                                // play
        }
        else if (yMouse >= (int)ConvertFrom1024AsFloat(666.0f) && yMouse < (int)ConvertFrom1024AsFloat(702.0f))
        {
            if (xMouse >= (int)ConvertFrom1024AsFloat(341.0f) && xMouse < (int)ConvertFrom1024AsFloat(496.0f))
                nHoverCtrl = 2;                                // prev
            else if (xMouse >= (int)ConvertFrom1024AsFloat(512.0f) && xMouse < (int)ConvertFrom1024AsFloat(667.0f))
                nHoverCtrl = 3;                                // next
        }

        if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
        {
            if (nHoverCtrl != -1 && g_arrUIEffectDSBuffer_52F168[3])  // "click"
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
            if (nHoverCtrl == 2 && --nSoundIndex < 1)
                nSoundIndex = 1;
            if (nHoverCtrl == 3 && ++nSoundIndex > 14)
                nSoundIndex = 14;
            if (nHoverCtrl == 1)                               // play: swap the BGM
            {
                if (*ppBgmBuffer)
                {
                    (*ppBgmBuffer)->Stop();
                    if (*ppBgmBuffer)                          // original re-tests
                    {
                        (*ppBgmBuffer)->Release();
                        *ppBgmBuffer = NULL;
                    }
                }
                wsprintfA(szFileName, "%s", g_ascSndFileNames_4C78A0[nSoundIndex].str);
                CHanaObj__GetRealPath_486D9A(szFileName, 1006);  // 1006 = WAV dir
                *ppBgmBuffer = LoadWav_422E7B(szFileName);
                if (*ppBgmBuffer)
                {
                    int nVol = (int)(g_fVolumeDbTable[g_nAviVolumeIndex_530266] - 1000.0f);
                    if (nVol < -10000)
                        nVol = -10000;
                    else if (nVol > 0)
                        nVol = 0;
                    (*ppBgmBuffer)->SetVolume(nVol);
                    DSBufferPlay(*ppBgmBuffer, 1);             // loop
                }
            }
            if (nHoverCtrl == 0 && *ppBgmBuffer)               // stop
                (*ppBgmBuffer)->Stop();
        }

        // 7x2 sound grid: click selects
        if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
        {
            for (i = 1; i < 15; ++i)
            {
                if ((int)ConvertFrom1024AsFloat((float)((i - 1) / 7) * 468.0f + 72.0f) <= xMouse
                    && xMouse < (int)ConvertFrom1024AsFloat((float)((i - 1) / 7) * 468.0f + 485.0f)
                    && (int)ConvertFrom1024AsFloat((float)((i - 1) % 7) * 36.0f + 217.0f) <= yMouse
                    && yMouse < (int)ConvertFrom1024AsFloat((float)((i - 1) % 7) * 36.0f + 244.0f))
                {
                    nSoundIndex = i;
                    if (g_arrUIEffectDSBuffer_52F168[3])      // "click" effect
                        DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                }
            }
        }

        // top-bar hot columns (any click; column 3 right-click only)
        nResult = 0;
        if (nMouseBtn && nMouseBtn != nPrevMouseBtn)
        {
            for (i = 0; i < 4; ++i)
            {
                if (i != 3 || nMouseBtn != 2)
                {
                    if (xMouse < (int)ConvertFrom1024AsFloat(128.0f) * i)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(128.0f) * (i + 1) <= xMouse)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(48.0f) <= yMouse)
                        continue;
                }
                nResult = -(i + 1);
                if (g_arrUIEffectDSBuffer_52F168[3])          // "click" effect
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                break;
            }
        }
        if (nResult < -1)                       // -1 would re-enter this screen
            break;

        // (both tables below are re-initialized every frame in the original;
        //  ctrlPos was an interleaved {x,y} DWORD[9] there)
        int arrCtrlXY[4][2] = { {288, 612}, {522, 612}, {341, 666}, {512, 666} };
        arrHighlightRects[0] = { 0, 0, ConvertFrom1024AsInt(198), ConvertFrom1024AsInt(36) };
        arrHighlightRects[1] = { ConvertFrom1024AsInt(198), 0, ConvertFrom1024AsInt(396), ConvertFrom1024AsInt(36) };
        arrHighlightRects[2] = { 0, ConvertFrom1024AsInt(36), ConvertFrom1024AsInt(155), ConvertFrom1024AsInt(72) };
        arrHighlightRects[3] = { ConvertFrom1024AsInt(155), ConvertFrom1024AsInt(36), ConvertFrom1024AsInt(310), ConvertFrom1024AsInt(72) };
        arrHighlightRects[4] = { 0, 0, 0, 0 };

        if (pBmpBackground)
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, pBmpBackground, NULL, DDBLT_WAIT, NULL);

            // hovered-button highlight frame from mub.bmp
            if (nHoverCtrl >= 0 && pBmpHighlight)
                g_pBackBuffer_4D6C90->BltFast(
                        (int)ConvertFrom1024AsFloat((float)arrCtrlXY[nHoverCtrl][0]),
                        (int)ConvertFrom1024AsFloat((float)arrCtrlXY[nHoverCtrl][1]),
                        pBmpHighlight, &arrHighlightRects[nHoverCtrl], DDBLTFAST_WAIT);

            // selected-sound indicator cell from mub2.bmp (2x7 cells of 413x28)
            if (nSoundIndex > 0 && pBmpIndicator)
            {
                SetRect(&rc,
                        (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) / 7) * 413.0f),
                        (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) % 7) * 28.0f),
                        (int)ConvertFrom1024AsFloat((float)(((nSoundIndex - 1) / 7) + 1) * 413.0f),
                        (int)ConvertFrom1024AsFloat((float)(((nSoundIndex - 1) % 7) + 1) * 28.0f));
                g_pBackBuffer_4D6C90->BltFast(
                        (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) / 7) * 468.0f + 72.0f),
                        (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) % 7) * 36.0f + 217.0f),
                        pBmpIndicator, &rc, DDBLTFAST_WAIT);
            }
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasCur);
        }
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
            if (pMouseSprite)
            {
                CHanaSpriteT3F6__SetPos_414A5D(pMouseSprite, (float)(xMouse + 16), (float)(yMouse + 16));
                DrawSprite2D(pMouseSprite);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        // nop_4863C3();                                       // disabled, as elsewhere
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    // teardown
    if (pBmpBackground)
    {
        pBmpBackground->Release();
        pBmpBackground = NULL;
    }
    if (pBmpHighlight)
    {
        pBmpHighlight->Release();
        pBmpHighlight = NULL;
    }
    if (pBmpIndicator)
    {
        pBmpIndicator->Release();
        pBmpIndicator = NULL;
    }
    StopAndRestartThread(0);
    return nResult;
}

void register_MenuMusicSelect() {
    hnh_Hook(0x47D163, hook_MenuMusicSelect, "MenuMusicSelect_47D163"); // [OK]
}
