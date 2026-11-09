// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// ShowYesNoDialog_461A9E -- modal YES/NO confirmation used by the system
// menu (save/load confirm, quit confirm). pDialog is the YesNo1+YesNo2.bmp
// masked sprite (SystemMenu loads it): sheet rows -- button 1 src
// {0,0,127,31}, button 2 src {0,32,127,63}, dialog panel src
// {0,64,Conv(640),127} drawn over dest (0, Conv(210))..(Conv(640),
// Conv(430)). Button centers (220,350) and (420,350), hit boxes +-64 x,
// +-16 y; the hovered button pulses (alpha sin(t/500)/2+0.75, scale 1.2),
// non-hovered scale 1.0. Font switches to ÇlÇr ñæí© 26x26 for the caption
// (centered at (320,230), white) and back to 15x15 after. The back buffer
// is snapshotted into a 640x480 temp surface and restored each frame.
// Fade: alpha = (now-t0)/500 in, 1-(now-t0)/500 out after a click; the
// loop pumps until a click happened AND the fade-out finished. Mouse input
// is drained from the 30-entry CInputState ring (any button==1 counts as
// the press, all entries cleared). Returns the chosen button: 1 or 2.
// (The "On" effect plays on click.)

// === PINNED GAME FUNCTIONS ===
int (__cdecl*ShowYesNoDialog_461A9E)(CHanaSpriteT3F6_S1FC *pCursor,
    CHanaSpriteT3F6_S1FC *pDialog, LPCSTR szCaption) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, LPCSTR)>(0x461A9E);

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
                     "ÇlÇr ñæí©");  // aMs_3 @0x4C5468: "ÇlÇr ñæí©"

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
                     "ÇlÇr ñæí©");  // aMs_2 @0x4C5474: "ÇlÇr ñæí©"
    return nBtn;                             // 1 or 2
}

void register_ShowYesNoDialog_461A9E(int cfg) {
  if (cfg == 1) {
    ShowYesNoDialog_461A9E = hook_ShowYesNoDialog_461A9E;
  }

  hnh_HookOff();
    hnh_Hook(0x461A9E, hook_ShowYesNoDialog_461A9E, "ShowYesNoDialog_461A9E");  // [OK]
  hnh_HookOn();
}
