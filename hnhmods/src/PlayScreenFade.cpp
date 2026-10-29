#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// PlayScreenFade_4874DE -- the universal screen-fade transition (24 xrefs:
// every scene function, SystemMenu, menus, ParseExecStr, HanaSurface helper).
//
//   bFadeOut:  0 = fade in  (alpha 0 -> 1)
//              1 = fade out (alpha 1 -> 0)
//              >1 = setup only, returns before animating -- and LEAKS the two
//                   sprites and the captured frame (no caller passes > 1)
//   colorref:  solid fade color
//   pDdSurface: optional pre-fade background; NULL = re-render the 3D scene
//               (RenderClear + HanaFrame_Draw over the scene frame)
//   fTimeScale: timer scale for the fade duration (SetTimerScaleFactor)
//   bFlush:    RenderFlush_4874A5() the captured pre-fade frame first
//
// Setup: a 128x128 solid-color tile -> color sprite, plus a second sprite
// textured by CreateTextureFromSurfaceWithAlphaMask(tile, Darken.BMP) for the
// darken overlay. The pre-fade frame is captured into an offscreen surface.
// Each frame: blit the captured frame back, then draw color rect + darken
// overlay twice -- pass 1 DESTBLEND=INVSRCALPHA, pass 2 DESTBLEND=D3DBLEND_ONE
// (SRCALPHA both times). Alpha steps 0.025 * frameTime*2.1 (min 0.01) per
// frame; the darken sprite runs at alpha * 1.25. Teardown frees everything,
// restores Z states, frame timer and timer scale, and ANTIALIAS.

// === PINNED GAME FUNCTIONS ===
LPDIRECTDRAWSURFACE7(__cdecl* PlayScreenFade)(int bisGo, COLORREF colorref,
  LPDIRECTDRAWSURFACE7 pDdSurface, float fFade, char bFlags) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)(int, COLORREF,
      LPDIRECTDRAWSURFACE7, float, char)>(0x4874DE);

DWORD(__cdecl* RGBToSurfaceFormat)(LPDIRECTDRAWSURFACE7 surface, COLORREF color) =
    reinterpret_cast<DWORD(__cdecl*)(LPDIRECTDRAWSURFACE7, COLORREF)>(0x41973F);
int(__cdecl* CHanaObj__NewSpriteT3F6bySurf)(CHanaSpriteT3F6_S1FC **pOut, LPDIRECTDRAWSURFACE7 pSurfaceSrc) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC **, LPDIRECTDRAWSURFACE7)>(0x4137D1);
//void(__cdecl* CHanaObj__NewSprite3F6byWH)(CHanaSpriteT3F6_S1FC **ppOut, DWORD w, DWORD h) =
//    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, DWORD, DWORD)>(0x4133D0);
int(__cdecl* CHanaObj__ReplaceTexture3F6)(CHanaSpriteT3F6_S1FC *pSprite, LPDIRECTDRAWSURFACE7 *pTexture) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, LPDIRECTDRAWSURFACE7 *)>(0x413D78);
void(__cdecl* CHanaObj__SetAlpha3F6)(CHanaSpriteT3F6_S1FC *pSprite, float alpha) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *, float)>(0x414ABA);
int(__cdecl* DrawSpriteLTRB)(CHanaSpriteT3F6_S1FC *pSprite, int left, int top, int right, int bottom) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, int, int, int, int)>(0x414589);
int(__cdecl* BltFastWH)(LPDIRECTDRAWSURFACE7 pSurf, int xDest, int yDest, LPDIRECTDRAWSURFACE7 pSrc,
    int left, int top, int width, int height, int ddbltfastFlags) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7, int, int, LPDIRECTDRAWSURFACE7,
      int, int, int, int, int)>(0x419D16);
void(__cdecl* RenderFlush_4874A5)(void) = reinterpret_cast<void(__cdecl*)()>(0x4874A5);
HRESULT(__cdecl* RenderClear_40BF4F)(DWORD ClearFlags) =
    reinterpret_cast<HRESULT(__cdecl*)(DWORD)>(0x40BF4F);
HRESULT(__cdecl* HanaFrame_Draw)(CHanaFrameT3EC_S354 *pFrame) =
    reinterpret_cast<HRESULT(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x405C06);
double(__cdecl* GetTimerScaleFactor)(void) = reinterpret_cast<double(__cdecl*)()>(0x47E78F);
void(__cdecl* SetTimerScaleFactor)(float fTimeScale) =
    reinterpret_cast<void(__cdecl*)(float)>(0x47E782);
int(__cdecl* FrameTimer_BeginFrame)(void) = reinterpret_cast<int(__cdecl*)()>(0x47E768);
DWORD(__cdecl* FrameTimer_EndFrame)(void) = reinterpret_cast<DWORD(__cdecl*)()>(0x47E7D5);
double(__cdecl* GetFrameTime)(void) = reinterpret_cast<double(__cdecl*)()>(0x47E79A);

// === FUNCTIONS ===
void __cdecl hook_PlayScreenFade(int bFadeOut, COLORREF colorref,
                                 LPDIRECTDRAWSURFACE7 pDdSurface, float fTimeScale, char bFlush)
{
    CHanaSpriteT3F6_S1FC *pDarkenSprite;    // [esp+D4h] [ebp-4h]
    CHanaSpriteT3F6_S1FC *pColorSprite;     // [esp+94h] [ebp-44h]
    LPDIRECTDRAWSURFACE7 pCapturedBg;       // [esp+9Ch] [ebp-3Ch]
    LPDIRECTDRAWSURFACE7 pColorSurface;     // [esp+A8h] [ebp-30h]
    LPDIRECTDRAWSURFACE7 pTexture;          // [esp+B8h] [ebp-20h]
    DWORD dwColor, w, h, dwAntiAliasSaved;  // color / [ebp-24h] / [ebp-18h] / [ebp-34h]
    float fAlpha, fAlphaDarken, fTimeAlpha, fTimerScaleSaved;
    int left, top, right, bottom, right2, bottom2;
    BYTE bDone;
    HRESULT hr;

    DebugLog("[HNH:SUB] PlayScreenFade_4874DE dir=%d scale=%f\n", bFadeOut, fTimeScale);

    pDarkenSprite = NULL;
    pColorSprite = NULL;
    pTexture = NULL;
    pCapturedBg = NULL;
    bDone = 0;
    left = -1;
    top = -1;
    right  = ConvertFrom640AsInt(641);            // one past the screen edges
    bottom = ConvertFrom640AsInt(481);
    right2  = ConvertFrom640AsInt(640) + 100;     // darken overlay overscan
    bottom2 = ConvertFrom640AsInt(480) + 100;

    // 128x128 solid-color tile -> color sprite
    pColorSurface = CreateSurfaceWH(0x80, 0x80);
    if (!pColorSurface)
        return;
    dwColor = RGBToSurfaceFormat(pColorSurface, colorref);
    BltSurface(pColorSurface, 0, 0, 128, 128, (int)dwColor);
    CHanaObj__NewSpriteT3F6bySurf(&pColorSprite, pColorSurface);
    if (!pColorSprite)
    {
        pColorSurface->Release();
        return;
    }
    // second sprite: the tile re-textured with darken.bmp as alpha mask
    CHanaObj__NewSprite3F6byWH(&pDarkenSprite, 128, 128);
    if (!pDarkenSprite)
    {
        CHanaObj__Free_40C1E4(&pColorSprite->base_0);
        pColorSprite = NULL;
        pColorSurface->Release();
        return;
    }
    pTexture = CreateTextureFromSurfaceWithAlphaMask(pColorSurface, g_pSurfaceDarkenBmp_52F1A8);
    CHanaObj__ReplaceTexture3F6(pDarkenSprite, &pTexture);
    pTexture = NULL;
    pColorSurface->Release();
    pColorSurface = NULL;

    // capture the pre-fade frame: supplied surface, or a fresh 3D render
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    if (pDdSurface)
    {
        FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
        GetSurfaceWH(pDdSurface, &w, &h);
        BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pDdSurface, 0, 0, (int)w, (int)h, 0);
        GetSurfaceWH(g_pBackBuffer_4D6C90, &w, &h);
        pCapturedBg = CreateSurfaceWH(w, h);
        if (bFlush)
            RenderFlush_4874A5();
        BltFastWH(pCapturedBg, 0, 0, g_pBackBuffer_4D6C90, 0, 0, (int)w, (int)h, 0);
    }
    else
    {
        RenderClear_40BF4F(3);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
        HanaVert_DrawBatch(0);
        MtxStackStartMultLocal_4074EF();
        hr = g_pD3DDevice_4D6C78->BeginScene();
        if (hr >= 0)
        {
            HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
            HanaVert_DrawBatch(1);
            g_pD3DDevice_4D6C78->EndScene();
        }
        if (bFlush)
            RenderFlush_4874A5();
        GetSurfaceWH(g_pBackBuffer_4D6C90, &w, &h);
        pCapturedBg = CreateSurfaceWH(w, h);
        BltFastWH(pCapturedBg, 0, 0, g_pBackBuffer_4D6C90, 0, 0, (int)w, (int)h, 0);
        BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pCapturedBg, 0, 0, (int)w, (int)h, 0);
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasSaved);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

    // (the original re-tests pDarkenSprite here; it cannot be NULL anymore)

    // bFadeOut > 1: setup only -- returns WITHOUT freeing the sprites or the
    // captured frame (original leaks them; no caller passes > 1)
    if (bFadeOut != 0 && bFadeOut != 1)
        return;
    fAlpha = (bFadeOut == 1) ? 1.0f : 0.0f;

    fTimerScaleSaved = (float)GetTimerScaleFactor();
    SetTimerScaleFactor(fTimeScale);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();

    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    while (!bDone)
    {
        fTimeAlpha = (float)(GetFrameTime() * 2.1 * 5); // default 2.1
        if (fTimeAlpha < 0.01f)
            fTimeAlpha = 0.01f;
        if (bFadeOut == 0)
            fAlpha += 0.025f * fTimeAlpha;
        else
            fAlpha -= 0.025f * fTimeAlpha;
        FrameTimer_EndFrame();

        // restore the captured pre-fade frame
        if (pCapturedBg)
        {
            FlushD3DAndTouchSurface(g_pPrimaryBuffer_4D6C88);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            GetSurfaceWH(pCapturedBg, &w, &h);
            BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pCapturedBg, 0, 0, (int)w, (int)h, 0);
        }

        // clamp for drawing / detect the end
        float alpha = fAlpha;
        if (fAlpha >= 1.0f)
        {
            alpha = 1.0f;
            bDone = 1;
        }
        if (alpha <= 0.0f)
        {
            alpha = 0.0f;
            bDone = 1;
        }

        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
        fAlphaDarken = alpha * 1.25f;
        CHanaObj__SetAlpha3F6(pDarkenSprite, fAlphaDarken);
        CHanaObj__SetAlpha3F6(pColorSprite, alpha);

        hr = g_pD3DDevice_4D6C78->BeginScene();
        if (hr >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
            DrawSpriteLTRB(pColorSprite, left, top, right, bottom);
            DrawSpriteLTRB(pDarkenSprite, -100, -100, right2, bottom2);

            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
            DrawSpriteLTRB(pColorSprite, left, top, right, bottom);
            DrawSpriteLTRB(pDarkenSprite, -100, -100, right2, bottom2);
            g_pD3DDevice_4D6C78->EndScene();
        }
        PresentFrame();
        HanaVert_DrawBatch(0);
    }
    Sleep(10);

    // teardown / restore
    if (pDarkenSprite)
    {
        CHanaObj__Free_40C1E4(&pDarkenSprite->base_0);
        pDarkenSprite = NULL;
    }
    if (pColorSprite)
    {
        CHanaObj__Free_40C1E4(&pColorSprite->base_0);
        pColorSprite = NULL;
    }
    if (pCapturedBg)
        pCapturedBg->Release();
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();
    SetTimerScaleFactor(fTimerScaleSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasSaved);
}

void register_PlayScreenFade_4874DE() {
    hnh_Hook(0x4874DE, hook_PlayScreenFade, "PlayScreenFade_4874DE"); // [OK]
}
