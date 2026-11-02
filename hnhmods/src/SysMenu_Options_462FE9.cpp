#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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
void (__cdecl*SysMenu_Options_462FE9)(CSceneState *sceneStates, LPDIRECTDRAWSURFACE7 *arrSurf,
    CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY,
    int bPressed) =
    reinterpret_cast<void(__cdecl*)(CSceneState *, LPDIRECTDRAWSURFACE7 *,
      CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462FE9);

// === PINNED GLOBALS ===
BYTE &g_mapFrameFlag_530264       = *(BYTE *)0x530264;  // WRITABLE
BYTE &g_nEffectLevel_530265       = *(BYTE *)0x530265;  // WRITABLE
BYTE &g_btoggleC_530268           = *(BYTE *)0x530268;  // WRITABLE

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

void register_SysMenu_Options_462FE9() {
    hnh_Hook(0x462FE9, hook_SysMenu_Options_462FE9, "SysMenu_Options_462FE9");  // [OK]
}
