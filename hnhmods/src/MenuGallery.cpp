#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// MenuGallery_47AD8D -- MainMenu state -2: the CG gallery over gl01/gl02/
// gl03.bmp tab backgrounds (one per tab, full screen). Per tab a 6x4 cell
// grid: cell (col,row) maps to name-table entry 24*nTab + col + 6*row; cell
// x spans Conv640((160*col+37)*0.625)..(160*col+187)*0.625, y likewise
// (122*row+97)..(122*row+210) -- 640-space boxes pre-scaled by 0.625.
//
// The name table holds 72 slots (3 tabs x 24, 20 bytes each); 53 pictures
// are named, in order:
//   G20-G29, G80-G90, G60-G70, G40-G42, G44-G50 (no G43),
//   G00-G03, G05-G10 (no G04), G95
// (the original emits ~230 lines of per-row strcpy/memset plus dead zero
// rows sourced from runtime-zero .data bytes -- collapsed into initializer).
//
// Unlock state: g_arrGalleryUnlocked_5301A2[atoi(name+1)] (char[150] over the
// 'G' number; written by ParseExecStr -- the script engine unlocks CGs).
// Locked cells draw the "glno" placeholder (no color key); only unlocked
// cells are hit-testable, and only entries < 54 open a preview.
//
// Preview: load via GRP dir full screen, PlayScreenFade in, PRIMARY-buffer
// BltFast, pump the mouse until any button, fade out, release. Nav strip
// "glb" (256x48, two 128-wide button frames) at y 393..423 (640-space,
// NOT pre-scaled), x bands (384+128i)..: prev/next clamp nTab to 0..2.
// Thumbnails: "glt" hover highlight (152x120, color-keyed) and "glno"
// (152x120); all sizes via ConvertFrom1024.
// Top bar as usual (-1..-4); exit only on nResult < 0 && != -2 (-2 would
// re-enter this screen). Menu key -> SystemMenu mode 2, >= 1 exits.

// === PINNED GAME FUNCTIONS ===
int (__cdecl*MenuGallery)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47AD8D);

// === PINNED GLOBALS ===
char (&g_arrGalleryUnlocked_5301A2)[150] = *(char(*)[150])0x5301A2;  // WRITABLE

// === FUNCTIONS ===
int __cdecl hook_MenuGallery(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
    LPDIRECTDRAWSURFACE7 bgSurfaces[10];    // [ebp-98Ch] 3 used (per tab)
    LPDIRECTDRAWSURFACE7 thumbSurfaces[6];  // [ebp-964h] [2] = nav strip "glb"
    LPDIRECTDRAWSURFACE7 pPreviewSurface;   // [ebp-B88h]
    CHAR szFileName[256];                   // [ebp-93Ch]
    CSceneState sceneScratch;               // [ebp-DA4h] SystemMenu arg copy
    DWORD dwAntiAliasCur;                   // [ebp-BC8h]
    char gpNameTable[72][20];               // [ebp-824h] 53 named slots
    int nResult, nHoverPic, nHoverNav, nTab, i, j;
    int xMouse, yMouse;                     // [ebp-828h] / [ebp-82Ch]
    WORD nMouseBtn = 0, nPrevMouseBtn;          // [ebp-4h] / [ebp-83Ch]

    DebugLog("[HNH:SUB] MenuGallery_47AD8D\n");

    static const char *const arrPicNames[53] = {
        "G20.GP","G21.GP","G22.GP","G23.GP","G24.GP","G25.GP","G26.GP","G27.GP","G28.GP","G29.GP",
        "G80.GP","G81.GP","G82.GP","G83.GP","G84.GP","G85.GP","G86.GP","G87.GP","G88.GP","G89.GP",
        "G90.GP","G60.GP","G61.GP","G62.GP","G63.GP","G64.GP","G65.GP","G66.GP","G67.GP","G68.GP",
        "G69.GP","G70.GP","G40.GP","G41.GP","G42.GP","G44.GP","G45.GP","G46.GP","G47.GP","G48.GP",
        "G49.GP","G50.GP","G00.GP","G01.GP","G02.GP","G03.GP","G05.GP","G06.GP","G07.GP","G08.GP",
        "G09.GP","G10.GP","G95.GP",
    };
    static const char arrBgNames[3][25]    = { "gl01", "gl02", "gl03" };
    static const char arrThumbNames[3][25] = { "glt", "glno", "glb" };
    static const int  arrThumbSize[3][2]   = { {152, 120}, {152, 120}, {256, 48} };

    nResult = 0;
    pPreviewSurface = NULL;
    nTab = 0;
    memset(gpNameTable, 0, sizeof(gpNameTable));
    for (i = 0; i < 53; ++i)
        strcpy(gpNameTable[i], arrPicNames[i]);
    for (i = 0; i < 9; ++i)                 // original clears 9 of the slots
    {
        thumbSurfaces[i] = NULL;
        bgSurfaces[i] = NULL;
    }

    for (i = 0; i < 3; ++i)
    {
        wsprintfA(szFileName, "%s.bmp", arrBgNames[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1007);        // 1007 = GRP dir
        bgSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
                ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
        wsprintfA(szFileName, "%s.bmp", arrThumbNames[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1007);
        thumbSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
                (int)ConvertFrom1024AsFloat((float)arrThumbSize[i][0]),
                (int)ConvertFrom1024AsFloat((float)arrThumbSize[i][1]));
        Surface_SetColorKeyRGB(thumbSurfaces[i], 0);
    }

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

        // grid hit-test: only unlocked cells answer
        nHoverPic = -1;
        for (j = 0; j < 4; ++j)
        {
            for (i = 0; i < 6; ++i)
            {
                if (g_arrGalleryUnlocked_5301A2[atoi(&gpNameTable[24 * nTab + 6 * j + i][1])]
                    && (int)ConvertFrom640AsFloat((float)(122 * j + 97) * 0.625f) < yMouse
                    && yMouse < (int)ConvertFrom640AsFloat((float)(122 * j + 210) * 0.625f)
                    && (int)ConvertFrom640AsFloat((float)(160 * i + 37) * 0.625f) < xMouse
                    && xMouse < (int)ConvertFrom640AsFloat((float)(160 * i + 187) * 0.625f))
                {
                    nHoverPic = 24 * nTab + i + 6 * j;
                    break;                   // inner loop only, as the original
                }
            }
        }

        // click a picture -> full-screen preview
        if (nHoverPic < 54 && nHoverPic != -1 && nMouseBtn != nPrevMouseBtn && nMouseBtn == 1)
        {
            if (g_arrUIEffectDSBuffer_52F168[3])               // "click"
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
            wsprintfA(szFileName, "%s", gpNameTable[nHoverPic]);
            CHanaObj__GetRealPath_486D9A(szFileName, 1007);
            pPreviewSurface = LoadBmpScaled_4203B3(szFileName,
                    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
            if (pPreviewSurface)
            {
                PlayScreenFade(1, 0xFFFF7FFF, pPreviewSurface, 25.0f, 0);
                g_pPrimaryBuffer_4D6C88->BltFast(0, 0, pPreviewSurface, NULL, DDBLTFAST_WAIT);
                do
                {
                    DoPeekMessages();
                    GetMouseBtn_45A41B(&nMouseBtn);
                } while (!nMouseBtn);
                PlayScreenFade(0, 0xFFFF7FFF, pPreviewSurface, 25.0f, 0);
                nPrevMouseBtn = nMouseBtn;   // consume the preview-dismiss click
            }
            if (pPreviewSurface)
            {
                pPreviewSurface->Release();
                pPreviewSurface = NULL;
            }
        }

        // nav buttons: prev / next (clamped tab switch)
        nHoverNav = -1;
        for (i = 0; i < 2; ++i)
        {
            if ((int)ConvertFrom640AsFloat(393.125f) < yMouse
                && yMouse < (int)ConvertFrom640AsFloat(423.125f)
                && (int)ConvertFrom640AsFloat((float)((i << 7) + 384) * 0.625f) < xMouse
                && xMouse < (int)ConvertFrom640AsFloat((float)(((i + 1) << 7) + 384) * 0.625f))
            {
                nHoverNav = i;
                if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
                {
                    if (i == 1)
                    {
                        if (++nTab > 2)
                            nTab = 2;
                    }
                    else
                    {
                        if (--nTab < 0)
                            nTab = 0;
                    }
                    if (g_arrUIEffectDSBuffer_52F168[3])
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
                if (g_arrUIEffectDSBuffer_52F168[3])
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                break;
            }
        }
        if (nResult < 0 && nResult != -2)    // -2 would re-enter this screen
            break;

        // (nav geometry is re-initialized every frame in the original)
        RECT arrNavBtnRect[2] = {
            { 0, 0, (int)ConvertFrom640AsFloat(80.0f),  (int)ConvertFrom640AsFloat(30.0f) },
            { (int)ConvertFrom640AsFloat(80.0f), 0, (int)ConvertFrom640AsFloat(160.0f), (int)ConvertFrom640AsFloat(30.0f) },
        };
        POINT arrNavPos[2] = {
            { (int)ConvertFrom640AsFloat(240.0f), (int)ConvertFrom640AsFloat(393.125f) },
            { (int)ConvertFrom640AsFloat(320.0f), (int)ConvertFrom640AsFloat(393.125f) },
        };

        if (bgSurfaces[nTab])
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, bgSurfaces[nTab], NULL, DDBLT_WAIT, NULL);

            if (nHoverNav >= 0 && thumbSurfaces[2])            // "glb" strip
                g_pBackBuffer_4D6C90->BltFast(arrNavPos[nHoverNav].x, arrNavPos[nHoverNav].y,
                        thumbSurfaces[2], &arrNavBtnRect[nHoverNav], DDBLTFAST_WAIT);

            // locked cells: "glno" placeholder (no color key)
            for (i = 0; i < 24; ++i)
            {
                if (gpNameTable[24 * nTab + i][0]
                    && !g_arrGalleryUnlocked_5301A2[atoi(&gpNameTable[24 * nTab + i][1])])
                    g_pBackBuffer_4D6C90->BltFast(
                            (int)ConvertFrom640AsFloat((float)(160 * (i % 6) + 37) * 0.625f),
                            (int)ConvertFrom640AsFloat((float)(122 * (i / 6) + 97) * 0.625f),
                            thumbSurfaces[1], NULL, DDBLTFAST_WAIT);
            }
            // hovered cell: "glt" highlight (color-keyed)
            if (nHoverPic >= 0 && thumbSurfaces[0] && gpNameTable[nHoverPic][0])
                g_pBackBuffer_4D6C90->BltFast(
                        (int)ConvertFrom640AsFloat((float)(160 * (nHoverPic % 24 % 6) + 37) * 0.625f),
                        (int)ConvertFrom640AsFloat((float)(122 * (nHoverPic % 24 / 6) + 97) * 0.625f),
                        thumbSurfaces[0], NULL, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
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
    for (i = 0; i < 5; ++i)                  // original releases 5 slots
    {
        if (bgSurfaces[i])
        {
            bgSurfaces[i]->Release();
            bgSurfaces[i] = NULL;
        }
        if (thumbSurfaces[i])
        {
            thumbSurfaces[i]->Release();
            thumbSurfaces[i] = NULL;
        }
    }
    StopAndRestartThread(0);
    return nResult;
}

void register_MenuGallery() {
    hnh_Hook(0x47AD8D, hook_MenuGallery, "MenuGallery_47AD8D");
}
