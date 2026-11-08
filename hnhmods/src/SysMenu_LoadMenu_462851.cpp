// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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
int (__cdecl*SysMenu_LoadMenu_462851)(CSceneState *arrScene, CHAR *arrSceneName,
    LPDIRECTDRAWSURFACE7 *arrSurf, CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
    int nMouseX, int nMouseY, int bPressed) =
    reinterpret_cast<int(__cdecl*)(CSceneState *, CHAR *, LPDIRECTDRAWSURFACE7 *,
      CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462851);

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
            wsprintfA(szText, "%s", &g_szSceneInfoText_4B5A10[j][i]);
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

void register_SysMenu_LoadMenu_462851() {
    hnh_Hook(0x462851, hook_SysMenu_LoadMenu_462851, "SysMenu_LoadMenu_462851");  // [OK]
}
