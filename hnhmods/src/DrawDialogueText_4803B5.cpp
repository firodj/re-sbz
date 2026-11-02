#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// DrawDialogueText_4803B5 -- scrolling dialogue-text renderer (glyph half of
// DrawDialogueWindow_480B2D; called with the frame time x speed multiplier).
//
// Global scroll counter g_dwFontRenderFlag (@0x52F1B0, float in a DWORD
// global) advances fFrameTime*30 per call; scroll/100 = number of glyphs
// revealed. Empty scene (no ptrStrData_260) clears by_460/by_461 and bails.
// (The original's NULL-pScene branch writes through the NULL pointer -- a
// crash bug that is never hit; collapsed into the guard here.)
//
// The 4x29 text-sprite grid pScene->textSprites_264 is walked in two passes
// sharing the i/j cursor:
//   PASS 1 (settled glyphs): draw every fully-revealed glyph (alpha =
//   flt_454 * flt_450), 22-px grid cells converted from 640-space, each
//   drawn TWICE (double-alpha compounding), counting v18 = glyph index;
//   stops early (v20 = "still scrolling") when scroll/100 < v18, or at a
//   sprite with bOk_10C == 1 (v25 = page-end marker).
//   PASS 2 (fading tail): up to 5 glyphs per frame fade in with a
//   triangular window fade = |1 - (index - scroll/100)/5| clamped [0,1];
//   each glyph slides into place (right/left edges (1-fade)*10, top/bottom
//   (1-fade)*4/32 px), alpha = flt_450 * fade * flt_454, drawn twice.
//   Quirk kept: hitting the 5-glyph budget mid-row jumps to the NEXT ROW
//   (remaining columns of the current row are deferred a frame).
//
// Cursor sprites (pSprWindowBmp[2] = static base, [1] = bobbing arrow at
// x 624): hidden while text is still scrolling or by_461 == 0; otherwise
// [2]'s alpha ramps fFrameTime/100 up to 1 (copied to [1]) and [1] bobs at
// y = sin(scroll/180)*5 + 432. D3DTSS_ADDRESS is set to 3 (CLAMP) around
// the glyph drawing and restored after.

// === PINNED GAME FUNCTIONS ===
void (__cdecl*DrawDialogueText_4803B5)(CHanaSceneT458_S504 *pScene, float fFrameTime) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, float)>(0x4803B5);

// === FUNCTIONS ===
void __cdecl hook_DrawDialogueText_4803B5(CHanaSceneT458_S504 *pScene, float fFrameTime)
{
    RECT rc;                               // [esp+50h] [ebp-28h]
    RECT rcGlyph;                          // [esp+60h] [ebp-18h]
    DWORD dwTssAddress;
    float fGlyphIndex, fFade, fFadeCount;  // v18 / v26 / v17
    int i, j, bStillScrolling, bPageEnd;

    ONCELOG("[HNH:SUB] DrawDialogueText_4803B5\n");

    fGlyphIndex = 0.0f;
    fFadeCount = 0.0f;
    bStillScrolling = 0;
    bPageEnd = 0;
    *(float *)&g_dwFontRenderFlag = *(float *)&g_dwFontRenderFlag + fFrameTime * 30.0f;

    // (the original's NULL-pScene / always-true pointer branches are dead
    // code; behaviorally only this guard matters)
    if (!pScene->ptrStrData_260)
    {
        pScene->by_460 = 0;
        pScene->by_461 = 0;
        return;
    }

    g_pD3DDevice_4D6C78->GetTextureStageState(0, D3DTSS_ADDRESS, &dwTssAddress);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 3);   // CLAMP
    SetRect(&rc, 0, 0, 24, 24);

    // --- pass 1: settled glyphs --------------------------------------------
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 29; ++j)
        {
            if (pScene->textSprites_264[i][j])
            {
                SetRect(&rcGlyph, 22 * j, 22 * i, 22 * (j + 1) + 2, 22 * (i + 1) + 2);
                if (*(float *)&g_dwFontRenderFlag / 100.0f < fGlyphIndex)
                {
                    bStillScrolling = 1;    // reveal not caught up yet
                    break;
                }
                if (pScene->textSprites_264[i][j]->bOk_10C == 1)
                {
                    bPageEnd = 1;           // page-end marker glyph
                    break;
                }
                fGlyphIndex += 1.0f;
                g_pD3DDevice_4D6C78->SetTexture(0, NULL);
                CHanaObj__SetAlpha3F6(pScene->textSprites_264[i][j],
                                       pScene->flt_454 * pScene->flt_450);
                SetRect(&rcGlyph,
                        ConvertFrom640AsInt(22 * (j + 1)),
                        ConvertFrom640AsInt(22 * i + 380),
                        ConvertFrom640AsInt(22 * (j + 2) + 2),
                        ConvertFrom640AsInt(22 * (i + 1) + 382));
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
                // drawn twice (double alpha compounding), as the original
                DrawSpriteLTRB(pScene->textSprites_264[i][j],
                               rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
                DrawSpriteLTRB(pScene->textSprites_264[i][j],
                               rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
            }
        }
        if (bStillScrolling == 1 || bPageEnd == 1)
            break;
    }

    // --- pass 2: fading tail (continues the shared i/j cursor) --------------
    while (i < 4)
    {
        if (j >= 29)
        {
            j = 0;                          // row done -> next row
            if (fFadeCount == 5.0f)
                break;                      // glyph budget for this frame
            ++i;
            continue;
        }
        if (pScene->textSprites_264[i][j])
        {
            fFade = fabsf(1.0f - (fGlyphIndex - *(float *)&g_dwFontRenderFlag / 100.0f) / 5.0f);
            if (fFade <= 1.0f)
            {
                if (fFade < 0.0f)
                    goto nextGlyph;         // not in the fade window yet
            }
            else
            {
                fFade = 1.0f;
            }
            CHanaObj__SetAlpha3F6(pScene->textSprites_264[i][j],
                                   pScene->flt_450 * fFade * pScene->flt_454);
            SetRect(&rcGlyph,
                    ConvertFrom640AsInt((int)((1.0f - fFade) * 10.0f) + 22 * (j + 1)),
                    ConvertFrom640AsInt((int)((1.0f - fFade) * 4.0f) + 22 * i + 380),
                    ConvertFrom640AsInt(22 * (j + 2) - (int)((1.0f - fFade) * 10.0f) + 2),
                    ConvertFrom640AsInt(22 * (i + 1) + (int)((1.0f - fFade) * 32.0f) + 382));
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
            DrawSpriteLTRB(pScene->textSprites_264[i][j],
                           rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
            DrawSpriteLTRB(pScene->textSprites_264[i][j],
                           rcGlyph.left, rcGlyph.top, rcGlyph.right, rcGlyph.bottom);
            if (fFadeCount == 5.0f)
            {
                // budget exhausted mid-row: the original jumps to the NEXT
                // row (rest of this row waits a frame)
                j = 0;
                if (fFadeCount == 5.0f)
                    break;
                ++i;
                continue;
            }
            fGlyphIndex += 1.0f;
            fFadeCount += 1.0f;
        }
nextGlyph:
        ++j;
    }

    // --- "more text" cursor sprites ----------------------------------------
    if (pScene->pSprWindowBmp[1] && pScene->pSprWindowBmp[2])
    {
        if (bStillScrolling || !pScene->by_461)
        {
            pScene->pSprWindowBmp[2]->fAlpha_144 = 0.0f;   // hidden
            pScene->pSprWindowBmp[1]->fAlpha_144 = 0.0f;
        }
        else
        {
            pScene->by_460 = 0;
            pScene->pSprWindowBmp[2]->fAlpha_144 =
                fFrameTime / 100.0f + pScene->pSprWindowBmp[2]->fAlpha_144;
            pScene->pSprWindowBmp[1]->fAlpha_144 = pScene->pSprWindowBmp[2]->fAlpha_144;
            if (pScene->pSprWindowBmp[2]->fAlpha_144 > 1.0f)
            {
                pScene->pSprWindowBmp[2]->fAlpha_144 = 1.0f;
                pScene->pSprWindowBmp[1]->fAlpha_144 = 1.0f;
            }
            // static base indicator
            CHanaSpriteT3F6__SetPos_414A5D(pScene->pSprWindowBmp[2],
                    (float)(int)ConvertFrom640AsFloat(624.0f),
                    (float)(int)ConvertFrom640AsFloat(464.0f));
            DrawSprite2D(pScene->pSprWindowBmp[2]);
            // bobbing arrow
            CHanaSpriteT3F6__SetPos_414A5D(pScene->pSprWindowBmp[1],
                    (float)(int)ConvertFrom640AsFloat(624.0f),
                    (float)(int)ConvertFrom640AsFloat(
                        sinf(*(float *)&g_dwFontRenderFlag / 180.0f) * 5.0f + 432.0f));
            DrawSprite2D(pScene->pSprWindowBmp[1]);
        }
    }

    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, dwTssAddress);
}

void register_DrawDialogueText_4803B5() {
    hnh_Hook(0x4803B5, hook_DrawDialogueText_4803B5, "DrawDialogueText_4803B5");
}
