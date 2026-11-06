// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// SysMenu_Graphics_46380D -- SystemMenu tab 4 (graphics options). Background
// System05.bmp (lazy into arrSurf[4]) + sheet kanbt.bmp (184x215, lazy into
// arrSurf[10]: 6 unchecked-box tiles of 183.125x23.125 plus three 120x24
// preset highlights at src y 142+24i). arrGfxOpts / arrSceneStates / pYesNo
// are passed but never used (Tea.ex writing is done by the SystemMenu
// caller from the same globals).
//
// 6 checkbox rows toggle (hit zones x 203/419 +216*(i%2), 184 wide,
// y 92+40*(i/2)..+23):
//   0 antialias, 1 dither, 2 specular, 3 fog, 4 light, 5 shadow
//   (each = (old == 0), i.e. a boolean NOT). Unchecked boxes redraw the
//   empty-box tile from kanbt src (arrCheckXY[i]*0.625, from the table
//   {(0,0),(0,114),(0,38),(0,152),(0,76),(0,190)}); the checked state is
//   part of the background image.
// 3 preset buttons (x 202+140*i, 120 wide, y 225..249) apply:
//   0 speed:     all off
//   1 standard:  dither+specular+fog+light on, AA/shadow off
//   2 quality:   all on
//   The hovered/selected preset highlight is drawn, and a per-option /
//   per-preset help line (DrawTextOnSurfaceSpaced, x 180, y 350/370/390,
//   spacing -2) explains each switch in Japanese.
// Clicks play the "click" effect. Blend states re-set after drawing.

// === PINNED GAME FUNCTIONS ===
void (__cdecl*SysMenu_Graphics_46380D)(int *arrGfxOpts, CSceneState *arrSceneStates,
    LPDIRECTDRAWSURFACE7 *arrSurf, CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo,
    int nMouseX, int nMouseY, int bPressed) =
    reinterpret_cast<void(__cdecl*)(int *, CSceneState *, LPDIRECTDRAWSURFACE7 *,
      CHanaSpriteT3F6_S1FC *, CHanaSpriteT3F6_S1FC *, int, int, int)>(0x46380D);

// === FUNCTIONS ===
// one help line: (180, y) spaced-text
static void DrawTip(const char *szText, int nY)
{
    DrawTextOnSurfaceSpaced_419B05(g_pBackBuffer_4D6C90,
        ConvertFrom640AsInt(180), ConvertFrom640AsInt(nY),
        ConvertFrom640AsInt(-2), const_cast<char *>(szText), 0);
}

void __cdecl hook_SysMenu_Graphics_46380D(int *arrGfxOpts /*unused*/,
    CSceneState *arrSceneStates /*unused*/, LPDIRECTDRAWSURFACE7 arrSurf[],
    CHanaSpriteT3F6_S1FC *pCursor, CHanaSpriteT3F6_S1FC *pYesNo /*unused*/,
    int nMouseX, int nMouseY, int bPressed)
{
    CHAR szText[256];                      // [esp+40h] [ebp-158h]
    POINT arrCheckXY[10];                  // [esp+148h] [ebp-50h] unchecked-tile src
    int nCheck, nPreset, i;

    static const int arrChkSrcY[6] = { 0, 114, 38, 152, 76, 190 }; // *0.625 in the original

    ONCELOG("[HNH:SUB] SysMenu_Graphics_46380D\n");

    memset(arrCheckXY, 0, sizeof(arrCheckXY));
    for (i = 0; i < 6; ++i)
        arrCheckXY[i].y = arrChkSrcY[i];

    CHanaSpriteT3F6__SetPos_414A5D(pCursor, (float)(nMouseX + 16), (float)(nMouseY + 16));

    // checkbox hit-test (6 rows; plays the click on hover-with-press)
    nCheck = -1;
    for (i = 0; i < 6; ++i)
    {
        if (ConvertFrom640AsInt(216 * (i % 2) + 203) < nMouseX
            && nMouseX < ConvertFrom640AsInt(216 * (i % 2) + 387)
            && ConvertFrom640AsInt(40 * (i / 2) + 92) < nMouseY
            && nMouseY < ConvertFrom640AsInt(40 * (i / 2) + 115))
        {
            if (bPressed == 1 && g_arrUIEffectDSBuffer_52F168[3])  // "click"
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
            nCheck = i;
            break;
        }
    }
    if (bPressed == 1)
    {
        switch (nCheck)                     // boolean NOT of each flag
        {
        case 0: g_bAntiAliasEnable       = g_bAntiAliasEnable == 0;       break;
        case 1: g_bDitherEnable          = g_bDitherEnable == 0;          break;
        case 2: g_bSpecularEnable_4B3474 = g_bSpecularEnable_4B3474 == 0; break;
        case 3: g_bFogIsActive_4B3480    = !g_bFogIsActive_4B3480;        break;
        case 4: g_bLightEnable_4B3484    = g_bLightEnable_4B3484 == 0;    break;
        case 5: g_bShadowEnable_518CA8   = g_bShadowEnable_518CA8 == 0;   break;
        default: break;
        }
    }

    // preset buttons: 0 speed / 1 standard / 2 quality
    nPreset = -1;
    for (i = 0; i < 3; ++i)
    {
        if (ConvertFrom640AsInt(140 * i + 202) < nMouseX
            && nMouseX < ConvertFrom640AsInt(140 * i + 322)
            && ConvertFrom640AsInt(225) < nMouseY
            && nMouseY < ConvertFrom640AsInt(249))
        {
            nPreset = i;
            if (bPressed == 1)
            {
                if (g_arrUIEffectDSBuffer_52F168[3])
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                if (nPreset == 0)           // speed: all off
                {
                    g_bAntiAliasEnable       = 0;
                    g_bDitherEnable          = 0;
                    g_bSpecularEnable_4B3474 = 0;
                    g_bFogIsActive_4B3480    = 0;
                    g_bLightEnable_4B3484    = 0;
                    g_bShadowEnable_518CA8   = 0;
                }
                else if (nPreset == 1)      // standard
                {
                    g_bAntiAliasEnable       = 0;
                    g_bDitherEnable          = 1;
                    g_bSpecularEnable_4B3474 = 1;
                    g_bFogIsActive_4B3480    = 1;
                    g_bLightEnable_4B3484    = 1;
                    g_bShadowEnable_518CA8   = 0;
                }
                else if (nPreset == 2)      // quality: all on
                {
                    g_bAntiAliasEnable       = 1;
                    g_bDitherEnable          = 1;
                    g_bSpecularEnable_4B3474 = 1;
                    g_bFogIsActive_4B3480    = 1;
                    g_bLightEnable_4B3484    = 1;
                    g_bShadowEnable_518CA8   = 1;
                }
            }
            break;
        }
    }

    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

    // background not cached yet -> load System05.bmp, blt, return
    if (!arrSurf[4])
    {
        wsprintfA(szText, "%s", "System05.bmp");
        CHanaObj__GetRealPath_486D9A(szText, 1007);           // 1007 = GRP dir
        arrSurf[4] = LoadBmpScaled_4203B3(szText,
                ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
        BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[4], 0, 0,
                  ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);
        return;
    }
    BltFastWH(g_pBackBuffer_4D6C90, 0, 0, arrSurf[4], 0, 0,
              ConvertFrom640AsInt(640), ConvertFrom640AsInt(480), 0);

    // sheet not cached yet -> load kanbt.bmp, return
    if (!arrSurf[10])
    {
        wsprintfA(szText, "%s", "kanbt.bmp");
        CHanaObj__GetRealPath_486D9A(szText, 1007);
        arrSurf[10] = LoadBmpScaled_4203B3(szText,
                ConvertFrom640AsInt(184), ConvertFrom640AsInt(215));
        return;
    }

    // unchecked boxes: redraw the empty-box tile over the background image
    for (i = 0; i < 6; ++i)
    {
        if ((!g_bAntiAliasEnable && !i)
            || (!g_bDitherEnable && i == 1)
            || (!g_bSpecularEnable_4B3474 && i == 2)
            || (!g_bFogIsActive_4B3480 && i == 3)
            || (!g_bLightEnable_4B3484 && i == 4)
            || (!g_bShadowEnable_518CA8 && i == 5))
        {
            BltFastWH(g_pBackBuffer_4D6C90,
                      (int)ConvertFrom640AsFloat((double)(i & 1) * 216.25 + 204.375),
                      (int)ConvertFrom640AsFloat((double)(i / 2) * 40.0 + 93.125),
                      arrSurf[10],
                      (int)ConvertFrom640AsFloat((float)arrCheckXY[i].x * 0.625f),
                      (int)ConvertFrom640AsFloat((float)arrCheckXY[i].y * 0.625f),
                      (int)ConvertFrom640AsFloat(183.125f),
                      (int)ConvertFrom640AsFloat(23.125f), 0);
        }
    }
    // hovered/selected preset highlight
    if (nPreset >= 0)
        BltFastWH(g_pBackBuffer_4D6C90,
                  (int)ConvertFrom640AsFloat((double)nPreset * 140.0 + 202.5),
                  (int)ConvertFrom640AsFloat(225.625f),
                  arrSurf[10], 0, ConvertFrom640AsInt(24 * nPreset + 142),
                  ConvertFrom640AsInt(120), ConvertFrom640AsInt(24), 0);

    // per-option help text (SJIS byte-exact from .rdata, escape-encoded)
    switch (nCheck)
    {
    case 0:    // "　このスイッチをオンにすると、ポリゴンの角を滑らかにします。"
        DrawTip("　このスイッチをオンにすると、ポリゴンの角を滑らかにします。", 350);
        break;
    case 1:
        DrawTip("　このスイッチをオンにすると、テクスチャーの色を滑らかにします。", 350);
        DrawTip("＊　一部のグラフィックカードのドライバーで T&Lモードの時に、この", 370);
        DrawTip("　スイッチをオンにすると物体の一部が変色してしまう場合があります", 390);
        break;
    case 2:
        DrawTip("　このスイッチをオンにすると、特定の物体に対して光沢を使用します。", 350);
        DrawTip("＊　ライトスイッチがオンの時のみ、このスイッチは効果をあらわします。", 370);
        break;
    case 3:    // fog
        DrawTip("　このスイッチをオンにすると、画面に霧の効果をつけます。", 350);
        break;
    case 4:    // light
        DrawTip("　このスイッチをオンにすると、光と陰の効果をつけます。", 350);
        break;
    case 5:    // shadow
        DrawTip("　このスイッチをオンにすると、影などの効果を描画します。", 350);
        break;
    default:
        break;
    }
    // preset help
    if (nPreset == 0)
        DrawTip("　環境設定スイッチが速度を最優先にした設定になります。", 350);
    else if (nPreset == 1)
        DrawTip("　環境設定スイッチが標準設定になります。", 350);
    else if (nPreset == 2)
        DrawTip("　環境設定スイッチが画質を最優先にした設定になります。", 350);

    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
}

void register_SysMenu_Graphics_46380D() {
    hnh_Hook(0x46380D, hook_SysMenu_Graphics_46380D, "SysMenu_Graphics_46380D");  // [OK]
}
