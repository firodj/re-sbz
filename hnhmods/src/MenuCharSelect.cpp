#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// MenuCharSelect_47A324 - MainMenu state -3: character select over ss00.bmp
// (1024x768 native). Five portraits ss01..ss05 whose load sizes are exactly
// the on-disk BMP dimensions (244x680, 208x624, 264x536, 232x720, 264x680),
// color-keyed on black. Hover hitboxes live in 1024x768 design space, scaled
// by 0.625 then ConvertFrom640; portrait 1 (ss02) is not hit-testable while
// g_sceneProgressFlag_530269 == 1 (locked until the backdrop was seen).
// Clicking a portrait: "On" effect, BGM stop, then run that character's
// assault pair via PlayAssault_4703AE -- modes 2*selectOrder[ch] and +1
// (selectOrder = {2,4,0,3,1} maps portraits to assault indices); either
// returning 1 exits with nResult=1, else the BGM resumes.
// Top bar: four 128-wide hot columns below y=48 (column 3 right-click only)
// give nResult -1..-4 with the "click" effect; -1/-2/-4 exit to MainMenu
// (-4 exits the whole extra menu, since MainMenu only loops on -1/-2/-3);
// -3 falls through and is effectively a no-op. The menu key opens
// SystemMenu_4655AB in mode 2; >= 1 there exits.
//
// Single xref: MainMenu_47DF0D (state -3), which passes the BGM by pointer.

// === PINNED GAME FUNCTIONS ===
int  (__cdecl*MenuCharSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47A324);
bool (__cdecl*IsMenuKeyPressed_481170)(void) = reinterpret_cast<bool(__cdecl*)()>(0x481170);
HRESULT (__cdecl*Surface_SetColorKeyRGB)(LPDIRECTDRAWSURFACE7 surface, COLORREF color) =
    reinterpret_cast<HRESULT(__cdecl*)(LPDIRECTDRAWSURFACE7, COLORREF)>(0x4198E0);
int  (__cdecl*PlayAssault_4703AE)(int iModeId, int iMenuTab) =
    reinterpret_cast<int(__cdecl*)(int, int)>(0x4703AE);

// === FUNCTIONS ===
int __cdecl hook_MenuCharSelect(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
    CHAR szFileName[256];                     // [esp+294h] [ebp-180h]
    LPDIRECTDRAWSURFACE7 arrCharSurfaces[10]; // [esp+268h] [ebp-1ACh]
    LPDIRECTDRAWSURFACE7 pBackgroundSurface;  // [esp+290h] [ebp-184h]
    CSceneState sceneScratch;                 // [ebp-51Ch] SystemMenu arg copy
    RECT rc;                                  // [esp+B4h] [ebp-360h]
    DWORD dwAntiAliasCur;
    char szCharNames[5][25] = { "ss01", "ss02", "ss03", "ss04", "ss05" };
    WORD selectOrder[5] = { 2, 4, 0, 3, 1 };
    // {x, y} pairs in 1024x768 design space (the original interleaves them as
    // DWORD[19] with x in the odd slot, y in the even one):
    const int arrLoadSize[5][2]  = { {244, 680}, {208, 624}, {264, 536}, {232, 720}, {264, 680} }; // == BMP dims
    const int arrPos[5][2]       = { {244, 680}, {208, 622}, {264, 534}, {232, 713}, {264, 678} };
    const int arrHitOffset[5][2] = { { 83,  88}, {248, 146}, {327, 234}, {527,  55}, {687,  90} };
    int nResult, nHoverChar, nChosenChar, i;
    int xmouse, ymouse;
    WORD nMouseBtn = 0, nPrevMouseBtn;

    DebugLog("[HNH:SUB] MenuCharSelect_47A324\n");

    nResult = 0;
    nHoverChar = 0;
    pBackgroundSurface = NULL;
    for (i = 0; i < 9; ++i)                   // original clears 9 of the 10 slots
        arrCharSurfaces[i] = NULL;

    // load the five portraits (scaled, black color key)
    for (i = 0; i < 5; ++i)
    {
        wsprintfA(szFileName, "%s.bmp", szCharNames[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1007);       // 1007 = GRP dir
        arrCharSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
                (int)ConvertFrom1024AsFloat((float)arrLoadSize[i][0]),
                (int)ConvertFrom1024AsFloat((float)arrLoadSize[i][1]));
        Surface_SetColorKeyRGB(arrCharSurfaces[i], 0);
    }

    // ss00 background fills the screen
    wsprintfA(szFileName, "ss00.bmp");                        // aSs00Bmp @0x4C6D04
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    pBackgroundSurface = LoadBmpScaled_4203B3(szFileName, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));

    g_sceneState_52F7E0.menuState.bActiveSceneStep = 0;
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
        GetMouseXY_45A4D8((DWORD *)&xmouse, (DWORD *)&ymouse);

        // hover hit-test: [offset, pos+offset] scaled boxes; portrait 1 locked
        nHoverChar = -1;
        for (i = 0; i < 5; ++i)
        {
            if (i == 1 && g_sceneProgressFlag_530269 != 1)
                continue;
            if ((int)ConvertFrom640AsFloat(arrHitOffset[i][1] * 0.625f) < ymouse
                && ymouse < (int)ConvertFrom640AsFloat((arrPos[i][1] + arrHitOffset[i][1]) * 0.625f)
                && (int)ConvertFrom640AsFloat(arrHitOffset[i][0] * 0.625f) < xmouse
                && xmouse < (int)ConvertFrom640AsFloat((arrPos[i][0] + arrHitOffset[i][0]) * 0.625f))
            {
                nHoverChar = i;
                break;
            }
        }

        // click a portrait -> run the character's assault pair
        if (nHoverChar != -1 && nMouseBtn != nPrevMouseBtn && nMouseBtn == 1)
        {
            nChosenChar = nHoverChar;
            StopAndRestartThread(0);
            if (g_arrUIEffectDSBuffer_52F168[2])              // "On" click (0x52F170)
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[2], 0);
            if (*ppBgmBuffer)
                (*ppBgmBuffer)->Stop();
            if (PlayAssault_4703AE(2 * selectOrder[nChosenChar], 1) == 1)
            {
                nResult = 1;
                break;
            }
            if (PlayAssault_4703AE(2 * selectOrder[nChosenChar] + 1, 1) == 1)
            {
                nResult = 1;
                break;
            }
            // neither assault exited the game: resume BGM
            if (*ppBgmBuffer)
                DSBufferPlay(*ppBgmBuffer, 0);
            StopAndRestartThread(2);
        }

        // top-bar hot columns (any click; column 3 right-click only)
        nResult = 0;
        if (nMouseBtn && nMouseBtn != nPrevMouseBtn)
        {
            for (i = 0; i < 4; ++i)
            {
                if (i != 3 || nMouseBtn != 2)
                {
                    if (xmouse < (int)ConvertFrom1024AsFloat(128.0f) * i)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(128.0f) * (i + 1) <= xmouse)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(48.0f) <= ymouse)
                        continue;
                }
                nResult = -(i + 1);
                if (g_arrUIEffectDSBuffer_52F168[3])          // "click" effect (0x52F174)
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                break;
            }
        }
        if (nResult < 0 && nResult != -3)                     // -3 is a no-op
            break;

        // render: background + hovered portrait (color-keyed) + cursor
        if (pBackgroundSurface)
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, pBackgroundSurface, NULL, DDBLT_WAIT, NULL);
            if (nHoverChar >= 0 && arrCharSurfaces[nHoverChar])
            {
                SetRect(&rc, 0, 0,
                        (int)ConvertFrom1024AsFloat((float)arrPos[nHoverChar][0]),
                        (int)ConvertFrom1024AsFloat((float)arrPos[nHoverChar][1]));
                g_pBackBuffer_4D6C90->BltFast(
                        (int)ConvertFrom1024AsFloat((float)arrHitOffset[nHoverChar][0]),
                        (int)ConvertFrom1024AsFloat((float)arrHitOffset[nHoverChar][1]),
                        arrCharSurfaces[nHoverChar], &rc,
                        DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
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
                CHanaSpriteT3F6__SetPos_414A5D(pMouseSprite, (float)(xmouse + 16), (float)(ymouse + 16));
                DrawSprite2D(pMouseSprite);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        // nop_4863C3();                                       // disabled, as in InGameMenu.cpp
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    // teardown
    if (pBackgroundSurface)
    {
        pBackgroundSurface->Release();
        pBackgroundSurface = NULL;
    }
    for (i = 0; i < 5; ++i)
    {
        if (arrCharSurfaces[i])
        {
            arrCharSurfaces[i]->Release();
            arrCharSurfaces[i] = NULL;
        }
    }
    StopAndRestartThread(0);
    return nResult;
}

void register_MenuCharSelect_47A324() {
    hnh_Hook(0x47A324, hook_MenuCharSelect, "MenuCharSelect_47A324");
}
