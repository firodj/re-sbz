// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"


// === PINNED GAME FUNCTIONS ===
int (__cdecl*SysMenu_GirlStatusCheck_464919)(LPDIRECTDRAWSURFACE7 *arrSurf,
    CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo, int nMouseX, int nMouseY,
    int bPressed, int *pnSceneStep, float fCurHP, float fMaxHP, QWORD *pSceneFlags) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7 *, CHanaSpriteT3F6_S1FC *,
      CHanaSpriteT3F6_S1FC *, int, int, int, int *, float, float, QWORD *)>(0x464919);

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
// standard patch (wsprintf "ÇO" then szText[1] += digit), 5 digits each,
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
                      1, "Å^", 0xC4C4C4);              // "Å^" @0x4C5954
    if (nCurHP < 1)
        nCurHP = 1;
    for (i = 0; i < 5; ++i)
    {
        if (nCurHP >= IntPow_44AF15(10, i))
        {
            nDigit = nCurHP % IntPow_44AF15(10, i + 1) / IntPow_44AF15(10, i);
            wsprintfA(szText, "ÇO");                    // "ÇO" @0x4C5958
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
            wsprintfA(szText, "ÇO");
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

void register_SysMenu_GirlStatusCheck_464919() {
  SysMenu_GirlStatusCheck_464919 = hook_SysMenu_GirlStatusCheck_464919;
  hnh_HookOff();
    hnh_Hook(0x464919, hook_SysMenu_GirlStatusCheck_464919, "SysMenu_GirlStatusCheck_464919");  // [OK]
  hnh_HookOn();
}
