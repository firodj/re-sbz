// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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

// === PINNED GAME FUNCTIONS ===
void (__cdecl*SysMenu_SaveMenu_462135)(CSceneState sceneStates[], CHAR *arrSceneName,
    LPDIRECTDRAWSURFACE7 arrSurf[], CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
    int nMouseX, int nMouseY, int bPressed) =
    reinterpret_cast<void(__cdecl*)(CSceneState *, CHAR *, LPDIRECTDRAWSURFACE7 *,
      CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x462135);

// === PINNED GLOBALS ===
DWORD &g_dwUnlockedSaveSlots_530260 = *(DWORD *)0x530260;                   // WRITABLE
//int    (&g_arrSceneStepThresholds_4B5970)[40] = *(int (*)[40])0x4B5970;     // READ-ONLY
//char   (&g_szSceneInfoText_4B5A10)[40][20][80] = *(char (*)[40][20][80])0x4B5A10;     // READ-ONLY

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

void register_SysMenu_SaveMenu_462135(int cfg) {
  if (cfg == 1) {
    SysMenu_SaveMenu_462135 = hook_SysMenu_SaveMenu_462135;
  }
  hnh_HookOff();
    hnh_Hook(0x462135, hook_SysMenu_SaveMenu_462135, "SysMenu_SaveMenu_462135");  // [OK]
  hnh_HookOn();
}
