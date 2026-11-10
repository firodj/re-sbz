// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === GLOBAL VARIABLES ===

static DWORD s_dwFogEnable_4D6CF4;
static DWORD s_dwFogRangeEnable_4D6CFC;
static DWORD s_dwFogMode_4B00C8 = 3;
static DWORD s_dwFogTableEnable_4D6CF8;
static DWORD s_dwFogColor_4B00CC = 0xFFFFFFFF;
static BOOL  s_bFogIsActive_4B3480 = TRUE;


#ifdef _USRDLL
DWORD &g_dwFogEnable_4D6CF4      = *(DWORD *)0x4D6CF4; 
DWORD &g_dwFogRangeEnable_4D6CFC = *(DWORD *)0x4D6CFC;
DWORD &g_dwFogMode_4B00C8        = *(DWORD *)0x4B00C8;
DWORD &g_dwFogTableEnable_4D6CF8 = *(DWORD *)0x4D6CF8;
DWORD &g_dwFogColor_4B00CC       = *(DWORD*)0x4B00CC;
BOOL &g_bFogIsActive_4B3480      = *(BOOL*)0x4B3480;
CHAR (&g_szPaddedNum_52EFBC)[20] = *(CHAR (*)[20])0x52EFBC; // WRITABLE

#else
DWORD &g_dwFogEnable_4D6CF4      = s_dwFogEnable_4D6CF4;
DWORD &g_dwFogRangeEnable_4D6CFC = s_dwFogRangeEnable_4D6CFC;
DWORD &g_dwFogMode_4B00C8        = s_dwFogMode_4B00C8;
DWORD &g_dwFogTableEnable_4D6CF8 = s_dwFogTableEnable_4D6CF8;
DWORD &g_dwFogColor_4B00CC       = s_dwFogColor_4B00CC;
BOOL &g_bFogIsActive_4B3480      = s_bFogIsActive_4B3480;

CHAR g_szPaddedNum_52EFBC[20];
#endif

// === GLOBAL FUNCTIONS ===
HRESULT (__cdecl * SetFogEnable_40DFB0)(BOOL bFogEnable, BOOL bFogRangeEnable) = reinterpret_cast<HRESULT(__cdecl*)(BOOL bFogEnable, BOOL bFogRangeEnable)>(0x40DFB0);
HRESULT (__cdecl * SetFogMode_40E02D)(int bFogTableEnable, DWORD dwFogMode) = reinterpret_cast<HRESULT(__cdecl*)(int bFogTableEnable, DWORD dwFogMode)>(0x40E02D);
HRESULT (__cdecl * SetFogColor_40E0B4)(DWORD dwFogColor) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwFogColor)>(0x40E0B4);
HRESULT (__cdecl * SetFogRange_40E0E2)(DWORD dwFogStart, DWORD dwFogEnd) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwFogStart, DWORD dwFogEnd)>(0x40E0E2);

void (__cdecl * DrawTextOnSurfaceSpaced_419B05)( LPDIRECTDRAWSURFACE7 pSurface, int a2, int y, int a4, LPCSTR lpString, COLORREF color) = reinterpret_cast<void(__cdecl*)( LPDIRECTDRAWSURFACE7 pSurface, int a2, int y, int a4, LPCSTR lpString, COLORREF color)>(0x419B05);
void (__cdecl * DrawTextOnSurface)( LPDIRECTDRAWSURFACE7 pSurface, int x, int y, int reserved, LPCSTR lpString, COLORREF color) = reinterpret_cast<void(__cdecl*)( LPDIRECTDRAWSURFACE7 pSurface, int x, int y, int reserved, LPCSTR lpString, COLORREF color)>(0x419A4B);
int (__cdecl * IntPow_44AF15)(int nBase, int nExp) = reinterpret_cast<int(__cdecl*)(int nBase, int nExp)>(0x44AF15);
void (__cdecl * CHanaSpriteT3F6__SetScale_414B09)(CHanaSpriteT3F6_S1FC *pSprite, float scale) = reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite, float scale)>(0x414B09);
void (__cdecl * CHanaSpriteT3F6__SetSrcRect_414B28)(CHanaSpriteT3F6_S1FC *pSprite, LPRECT lpRect) = reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite, LPRECT lpRect)>(0x414B28);
double (__cdecl * CHanaSpriteT3F6__GetAlpha_414AFB)(CHanaSpriteT3F6_S1FC *pSprite) = reinterpret_cast<double(__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite)>(0x414AFB);
void (__cdecl * ShowFadeDialog_46164C)(CHanaSpriteT3F6_S1FC *pSprite, CHanaSpriteT3F6_S1FC *pPanelSprite, LPCSTR szText) = reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite, CHanaSpriteT3F6_S1FC *pPanelSprite, LPCSTR szText)>(0x46164C);
CHAR * (__cdecl * ZeroPadNumberToString_4612B0)(int nValue, int nDigits) = reinterpret_cast<CHAR *(__cdecl*)(int nValue, int nDigits)>(0x4612B0);
void (__cdecl * FormatFileLastWriteTime_461397)(CHAR *pszFileName, CHAR *pszOutBuf) = reinterpret_cast<void(__cdecl*)(CHAR *pszFileName, CHAR *pszOutBuf)>(0x461397);
void (__cdecl * LoadDataFiles_461557)(CSceneState *pDataBuffer, char *pszTimestamps) = reinterpret_cast<void(__cdecl*)(CSceneState *pDataBuffer, char *pszTimestamps)>(0x461557);

// === FUNCTION IMPLEMENTATIONS ===
/**
 * Enables or disables fog (and optionally range-based fog) on the D3D7 device.
 * @param bFogEnable FALSE disables fog; any other value enables it.
 * @param bFogRangeEnable Range fog (eye-relative distance) request; only honored
 *                        when the device advertises D3DPRASTERCAPS_FOGRANGE.
 * @return HRESULT of the last SetRenderState call performed.
 */
HRESULT __cdecl hook_SetFogEnable_40DFB0(BOOL bFogEnable, BOOL bFogRangeEnable)
{
  ONCELOG("[HNH:SUB] SetFogEnable_40DFB0\n");

  HRESULT hr; // [esp+0h] [ebp-4h]

  g_dwFogEnable_4D6CF4 = bFogEnable; /*0x40dfbe*/
  hr = g_pD3DDevice_4D6C78->SetRenderState( D3DRENDERSTATE_FOGENABLE  , bFogEnable); /*0x40dfde*/
  if (g_d3dDevDesc_4D6980.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGRANGE) /*0x40dfef*/
  {
    g_dwFogRangeEnable_4D6CFC = bFogRangeEnable; /*0x40dff4*/
    if (bFogRangeEnable) /*0x40dffd*/
    {
      if (g_dwFogEnable_4D6CF4) /*0x40e006*/
        return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, g_dwFogEnable_4D6CF4); /*0x40e023*/
    }
  }
  return hr; /*0x40e029*/
}

/**
* Selects the fog mode (D3DFOG_*) as table (pixel) fog or vertex fog.
* @param bFogTableEnable Requests pixel/table fog; only honored when the device
*                        advertises D3DPRASTERCAPS_FOGTABLE, otherwise vertex
*                        fog is used.
* @param dwFogMode D3DFOG_NONE / D3DFOG_EXP / D3DFOG_EXP2 / D3DFOG_LINEAR.
* @return HRESULT of the SetRenderState call performed.
*/
HRESULT __cdecl hook_SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode)
{
  ONCELOG("[HNH:SUB] SetFogMode_40E02D\n");

  BOOL bTableFog; // [esp+0h] [ebp-8h]

  g_dwFogMode_4B00C8 = dwFogMode; /*0x40e036*/
  bTableFog = bFogTableEnable && (g_d3dDevDesc_4D6980.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0; /*0x40e051*/
  g_dwFogTableEnable_4D6CF8 = bTableFog; /*0x40e064*/
  if (bTableFog) /*0x40e071*/
    return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, dwFogMode); /*0x40e08a*/
  else
    return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, dwFogMode); /*0x40e0aa*/
}

HRESULT __cdecl hook_SetFogColor_40E0B4(DWORD dwFogColor)
{
  ONCELOG("[HNH:SUB] SetFogColor_40E0B4\n");

  g_dwFogColor_4B00CC = dwFogColor;
  return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);
}

HRESULT __cdecl hook_SetFogRange_40E0E2(DWORD dwFogStart, DWORD dwFogEnd)
{
  HRESULT hr; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] SetFogRange_40E0E2\n");

  hr = 0;
  if ( g_dwFogMode_4B00C8 == 3 )
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGSTART, dwFogStart);
    return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGEND, dwFogEnd);
  }
  return hr;
}

// DrawTextOnSurfaceSpaced(pSurf, nX, nY, nCharPitch, lpString, color)
// GDI TextOutA of lpString onto a DD surface's DC in 2-char chunks, selecting g_hFont,
// transparent background. Per-char x-advance = (nCharPitch + g_dwCharWidth)/2, letting the
// caller tighten (nCharPitch<0) or widen the text vs. DrawTextOnSurface_419A4B. Calls GetDC/ReleaseDC.
void __cdecl hook_DrawTextOnSurfaceSpaced_419B05(
  LPDIRECTDRAWSURFACE7 pSurface,
  int a2,
  int y,
  int a4,
  LPCSTR lpString,
  COLORREF color)
{
  HGDIOBJ hOldFont; // [esp+0h] [ebp-10h]
  int nLen; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  HDC hdc; // [esp+Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] DrawTextOnSurfaceSpaced_419B05\n")

  GetDC_4188B4(pSurface, &hdc);
  hOldFont = SelectObject(hdc, g_hFont);
  SetTextColor(hdc, color);
  SetBkMode(hdc, 1);
  nLen = lstrlenA(lpString);
  for ( i = 0; i < nLen; i += 2 )
    TextOutA(hdc, i * a4 / 2 + i * g_dwCharWidth / 2 + a2, y, &lpString[i], 2);
  SelectObject(hdc, hOldFont);
  pSurface->ReleaseDC(hdc);
}

// DrawTextOnSurface(pSurf, x, y, nReserved, lpString, color):
//   GDI-draw lpString onto the D3D surface pSurf at (x, y).
//   - GetDC on pSurf; select global font g_hFont; text color = color; bk mode = TRANSPARENT
//   - fixed-pitch layout: TextOutA in 2-char chunks, x advances by g_dwCharWidth/2
//     per char -> TextOutA(hdc, x + i*g_dwCharWidth/2, y, &lpString[i], 2)
//   - restore previous GDI object (hoPrev); ReleaseDC
//   - nReserved (4th arg) is unused by this routine
void __cdecl hook_DrawTextOnSurface(
  LPDIRECTDRAWSURFACE7 pSurface,
  int x,
  int y,
  int reserved,
  LPCSTR lpString,
  COLORREF color)
{
  HGDIOBJ hoPrev; // [esp+0h] [ebp-10h]
  int nStrLen; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  HDC hdc; // [esp+Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] DrawTextOnSurface_419A4B\n");

  GetDC_4188B4(pSurface, &hdc);
  hoPrev = SelectObject(hdc, g_hFont);
  SetTextColor(hdc, color);
  SetBkMode(hdc, 1);
  nStrLen = lstrlenA(lpString);
  for ( i = 0; i < nStrLen; i += 2 )
    TextOutA(hdc, i * g_dwCharWidth / 2 + x, y, &lpString[i], 2);
  SelectObject(hdc, hoPrev);
  pSurface->ReleaseDC(hdc);
}

int __cdecl hook_IntPow_44AF15(int nBase, int nExp)
{
  int nI; // [esp+0h] [ebp-8h]
  int nBaseCopy; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] IntPow_44AF15\n");

  nBaseCopy = nBase;
  if ( nExp <= 0 )
    return 1;
  for ( nI = 1; nI < nExp; ++nI )
    nBase *= nBaseCopy;
  return nBase;
}

// CHanaSpriteT3F6::SetScale(pSprite, fScale)
// Sets the sprite uniform scale (fScale_148 at 0x148). fScale is a float bit-cast via int
// (called with 1.0, 1.2, or 0x40000000 = 0.5f). Returns pSprite.
void __cdecl hook_CHanaSpriteT3F6__SetScale_414B09(CHanaSpriteT3F6_S1FC *pSprite, float scale)
{
  ONCELOG("[HNH:SUB] CHanaSpriteT3F6__SetScale_414B09\n");
  pSprite->fScale_148 = scale;
}

// CHanaSpriteT3F6::SetSrcRect(pSprite, lpRect)
// Sets the sprite source/clip rect: copies *lpRect into pSprite->rect_15C and points pRect_16C at it;
// if lpRect is NULL, clears pRect_16C (draw full texture). Returns pSprite.
void __cdecl hook_CHanaSpriteT3F6__SetSrcRect_414B28(CHanaSpriteT3F6_S1FC *pSprite, LPRECT lpRect)
{
  if (!pSprite) return;

  ONCELOG("[HNH:SUB] CHanaSpriteT3F6__SetSrcRect_414B28\n");

  if ( lpRect )
  {
    pSprite->rect_15C = *lpRect;
    pSprite->pRect_16C = &pSprite->rect_15C;
  }
  else
  {
    pSprite->pRect_16C = 0;
  }
}
// CHanaSpriteT3F6::GetAlpha(pSprite)
// Returns the sprite's current alpha (fAlpha_144 at 0x144) as a float/double.
// Used by fade loops to read/decay alpha per frame.
double __cdecl hook_CHanaSpriteT3F6__GetAlpha_414AFB(CHanaSpriteT3F6_S1FC *pSprite)
{
  ONCELOG("[HNH:SUB] CHanaSpriteT3F6__GetAlpha_414AFB\n");
  return pSprite->fAlpha_144;
}

void __cdecl hook_ShowFadeDialog_46164C(CHanaSpriteT3F6_S1FC *pSprite, CHanaSpriteT3F6_S1FC *pPanelSprite, LPCSTR szText)
{
  DWORD surfH; // eax
  int srcW; // eax
  int fontH; // eax
  int capSrcW; // eax
  int nCenterX; // esi
  int nTextLen; // edi
  int nCharW; // eax
  int font2W; // eax
  int nTextTop; // [esp-8h] [ebp-88h]
  int srcH; // [esp+0h] [ebp-80h]
  int fontW; // [esp+0h] [ebp-80h]
  int capSrcH; // [esp+0h] [ebp-80h]
  int font2H; // [esp+0h] [ebp-80h]
  DWORD surfW; // [esp+4h] [ebp-7Ch]
  float alpha; // [esp+4h] [ebp-7Ch]
  DWORD dwNow; // [esp+20h] [ebp-60h]
  struct IDirectDrawSurface7 *pHddCapture; // [esp+24h] [ebp-5Ch]
  int i; // [esp+28h] [ebp-58h]
  int bMouseHeld; // [esp+2Ch] [ebp-54h]
  CInputState *pInputState; // [esp+30h] [ebp-50h]
  int bPhaseOut; // [esp+34h] [ebp-4Ch]
  int nPanelTop; // [esp+3Ch] [ebp-44h]
  int nPanelRight; // [esp+40h] [ebp-40h]
  int nPanelBottom; // [esp+44h] [ebp-3Ch]
  struct tagRECT rcSrcRect; // [esp+68h] [ebp-18h] BYREF
  DWORD dwStartTime; // [esp+78h] [ebp-8h]
  unsigned int dwElapsedMs; // [esp+7Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] ShowFadeDialog_46164C\n");

  bPhaseOut = 0;
  nPanelTop = ConvertFrom640AsInt(210);
  nPanelRight = ConvertFrom640AsInt(640);
  nPanelBottom = ConvertFrom640AsInt(280);
  rcSrcRect.left = 0;
  rcSrcRect.top = 64;
  rcSrcRect.right = ConvertFrom640AsInt(640);
  rcSrcRect.bottom = 84;
  surfW = ConvertFrom640AsInt(480);
  surfH = ConvertFrom640AsInt(640);
  pHddCapture = CreateSurfaceWH(surfH, surfW);
  srcH = ConvertFrom640AsInt(480);
  srcW = ConvertFrom640AsInt(640);
  BltFastWH(pHddCapture, 0, 0, g_pBackBuffer_4D6C90, 0, 0, srcW, srcH, 0);
  fontW = ConvertFrom640AsInt(26);
  fontH = ConvertFrom640AsInt(26);
  SetUiFont_419BCD(fontH, fontW, "ÇlÇr ñæí©");
  dwStartTime = timeGetTime();
  do
  {
    DoPeekMessages();
    dwNow = timeGetTime();
    dwElapsedMs = dwNow - dwStartTime;
    MutexThreadWait_4815EC(1);
    pInputState = GetInputState_481B37();
    bMouseHeld = 0;
    for ( i = 0; i < 30; ++i )
    {
      if ( pInputState[i].mouseBtn_4 == 1 )
        bMouseHeld = 1;
      pInputState[i].mouseBtn_4 = 0;
    }
    CHanaSpriteT3F6__SetPos_414A5D(pSprite, -1000.0, 1000.0);
    if ( pPanelSprite->fAlpha_144 == 1.0 && !bPhaseOut && bMouseHeld == 1 || dwElapsedMs > 0xBB8 )
    {
      bPhaseOut = 1;
      dwStartTime = dwNow;
    }
    MutexThreadWait_4815EC(0);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    if ( pHddCapture )
    {
      capSrcH = ConvertFrom640AsInt(480);
      capSrcW = ConvertFrom640AsInt(640);
      BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pHddCapture, 0, 0, capSrcW, capSrcH, 0);
    }
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      if ( bPhaseOut )
        alpha = 1.0 - (double)(dwNow - dwStartTime) / 500.0;
      else
        alpha = (double)(dwNow - dwStartTime) / 500.0;
      CHanaObj__SetAlpha3F6(pPanelSprite, alpha);
      CHanaSpriteT3F6__SetSrcRect_414B28(pPanelSprite, &rcSrcRect);
      DrawSpriteLTRB(pPanelSprite, 0, nPanelTop, nPanelRight, nPanelBottom);
      g_pD3DDevice_4D6C78->EndScene();
    }
    if ( pPanelSprite->fAlpha_144 == 1.0 )
    {
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      nTextTop = ConvertFrom640AsInt(230);
      nCenterX = ConvertFrom640AsInt(320);
      nTextLen = lstrlenA(szText);
      nCharW = ConvertFrom640AsInt(26);
      DrawTextOnSurface(g_pBackBuffer_4D6C90, nCenterX - nTextLen * nCharW / 4, nTextTop, 1, szText, 0xFFFFFFu);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    }
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      DrawSprite2D(pSprite);
      g_pD3DDevice_4D6C78->EndScene();
    }
    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  while ( bPhaseOut != 1 || pPanelSprite->fAlpha_144 > 0.0 );
  if ( pHddCapture )
    pHddCapture->Release();
  font2H = ConvertFrom640AsInt(15);
  font2W = ConvertFrom640AsInt(15);
  SetUiFont_419BCD(font2W, font2H, "ÇlÇr ñæí©");
}

CHAR *__cdecl hook_ZeroPadNumberToString_4612B0(int nValue, int nDigits)
{
  char cDigit; // [esp+0h] [ebp-14h]
  int i; // [esp+4h] [ebp-10h]
  int nPlaceValue; // [esp+Ch] [ebp-8h]
  CHAR chChar[4]; // [esp+10h] [ebp-4h] BYREF

  chChar[2] = 0;
  memset(g_szPaddedNum_52EFBC, 0, 0x12u);
  while ( nDigits > 0 )
  {
    nPlaceValue = 1;
    for ( i = 0; i < nDigits - 1; ++i )
      nPlaceValue *= 10;
    wsprintfA(chChar, "ÇO");
    cDigit = nValue % (10 * nPlaceValue) / nPlaceValue;
    if ( cDigit > 10 )
      cDigit -= nValue % (100 * nPlaceValue) / (10 * nPlaceValue);
    chChar[1] += cDigit;
    wsprintfA(g_szPaddedNum_52EFBC, "%s%s", g_szPaddedNum_52EFBC, chChar);
    --nDigits;
  }
  return g_szPaddedNum_52EFBC;
}

void __cdecl hook_FormatFileLastWriteTime_461397(CHAR *pszFileName, CHAR *pszOutBuf)
{
  CHAR *pszYear; // eax
  CHAR *pszMonth; // eax
  CHAR *pszDay; // eax
  CHAR *pszHour; // eax
  CHAR *pszMin; // eax
  WORD wDosTime; // [esp+0h] [ebp-28h] BYREF
  HANDLE hFile; // [esp+4h] [ebp-24h]
  struct _FILETIME ftLocal; // [esp+8h] [ebp-20h] BYREF
 
  WORD wDosDate; // [esp+14h] [ebp-14h] BYREF
  DWORD FileSize; // [esp+18h] [ebp-10h]
  struct _FILETIME ftLastWrite; // [esp+1Ch] [ebp-Ch] BYREF

  ONCELOG("[HNH:SUB] FormatFileLastWriteTime_461397\n");

  if ( IsFileExists_47E8EE(pszFileName) )
  {
    hFile = CreateFileA(pszFileName, 0x80000000, 0, 0, 3u, 0, 0);

    if ( hFile )
    {
      if ( hFile != (HANDLE)-1 )
      {
        FileSize = GetFileSize(hFile, 0);
        if ( FileSize && FileSize != -1 )
        {
          GetFileTime(hFile, 0, 0, &ftLastWrite);
          FileTimeToLocalFileTime(&ftLastWrite, &ftLocal);
          FileTimeToDosDateTime(&ftLocal, &wDosDate, &wDosTime);
          CloseHandle(hFile);
          pszYear = ZeroPadNumberToString_4612B0((wDosDate >> 9) + 1980, 4);
          wsprintfA(pszOutBuf, "%s%sîN", pszOutBuf, pszYear);
          pszMonth = ZeroPadNumberToString_4612B0((wDosDate >> 5) & 0xF, 2);
          wsprintfA(pszOutBuf, "%s%såé", pszOutBuf, pszMonth);
          pszDay = ZeroPadNumberToString_4612B0(wDosDate & 0x1F, 2);
          wsprintfA(pszOutBuf, "%s%sì˙", pszOutBuf, pszDay);
          pszHour = ZeroPadNumberToString_4612B0(wDosTime >> 11, 2);
          wsprintfA(pszOutBuf, "%s%séû", pszOutBuf, pszHour);
          pszMin = ZeroPadNumberToString_4612B0((wDosTime >> 5) & 0x3F, 2);
          wsprintfA(pszOutBuf, "%s%sï™", pszOutBuf, pszMin);
        }
        else
        {
          CloseHandle(hFile);
        }
      }
    }
  }
}

void __cdecl hook_LoadDataFiles_461557(CSceneState *pDataBuffer, char *pszTimestamps)
{
  CHAR szFilePath[256]; // [esp+0h] [ebp-104h] BYREF
  int i; // [esp+100h] [ebp-4h]

  ONCELOG("[HNH:SUB] LoadDataFiles_461557\n");
    
  memset(pDataBuffer, 0, 4000u);
  memset(pszTimestamps, 0, 500u);
  for ( i = 0; i < 10; ++i )
  {
    wsprintfA(szFilePath, "data%d.dat", i + 1);
    CHanaObj__GetRealPath_486D9A(szFilePath, 1010);
    if ( IsFileExists_47E8EE(szFilePath) )
    {
      ReadFileToBuffer(szFilePath, &pDataBuffer[i], 400);
      FormatFileLastWriteTime_461397(szFilePath, &pszTimestamps[50 * i]);
    }
    else
    {
      memset(&pDataBuffer[i], 0, sizeof(CSceneState));
    }
  }
}

void register_SetFogEnable_40DFB0(int cfg) {
  if (cfg == 1) {
    SetFogEnable_40DFB0 = hook_SetFogEnable_40DFB0;
    SetFogMode_40E02D = hook_SetFogMode_40E02D;
    SetFogColor_40E0B4 = hook_SetFogColor_40E0B4;
    SetFogRange_40E0E2 = hook_SetFogRange_40E0E2;

    DrawTextOnSurfaceSpaced_419B05 = hook_DrawTextOnSurfaceSpaced_419B05;
    DrawTextOnSurface = hook_DrawTextOnSurface;
    IntPow_44AF15 = hook_IntPow_44AF15;
    CHanaSpriteT3F6__SetScale_414B09 = hook_CHanaSpriteT3F6__SetScale_414B09;
    CHanaSpriteT3F6__SetSrcRect_414B28 = hook_CHanaSpriteT3F6__SetSrcRect_414B28;
    CHanaSpriteT3F6__GetAlpha_414AFB = hook_CHanaSpriteT3F6__GetAlpha_414AFB;
    ShowFadeDialog_46164C = hook_ShowFadeDialog_46164C;
    ZeroPadNumberToString_4612B0 = hook_ZeroPadNumberToString_4612B0;
    FormatFileLastWriteTime_461397 = hook_FormatFileLastWriteTime_461397;
    LoadDataFiles_461557 = hook_LoadDataFiles_461557;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x4612B0, hook_ZeroPadNumberToString_4612B0, "ZeroPadNumberToString_4612B0");

  hnh_Hook(0x40DFB0, hook_SetFogEnable_40DFB0, "SetFogEnable_40DFB0"); // [OK] 
  hnh_Hook(0x40E02D, hook_SetFogMode_40E02D, "SetFogMode_40E02D"); // [OK] 
  hnh_Hook(0x40E0B4, hook_SetFogColor_40E0B4, "SetFogColor_40E0B4"); // [OK] 
  hnh_Hook(0x40E0E2, hook_SetFogRange_40E0E2, "SetFogRange_40E0E2"); // [OK]
  hnh_Hook(0x419A4B, hook_DrawTextOnSurface, "DrawTextOnSurface_419A4B"); // [OK]
  hnh_Hook(0x44AF15, hook_IntPow_44AF15, "IntPow_44AF15"); // [OK]
  hnh_Hook(0x414B09, hook_CHanaSpriteT3F6__SetScale_414B09, "CHanaSpriteT3F6__SetScale_414B09"); // [OK]
  hnh_Hook(0x414B28, hook_CHanaSpriteT3F6__SetSrcRect_414B28, "CHanaSpriteT3F6__SetSrcRect_414B28"); // [OK]
  hnh_Hook(0x414AFB, hook_CHanaSpriteT3F6__GetAlpha_414AFB, "CHanaSpriteT3F6__GetAlpha_414AFB"); // [OK]
  hnh_Hook(0x461397, hook_FormatFileLastWriteTime_461397, "FormatFileLastWriteTime_461397"); // [OK]
  hnh_Hook(0x461557, hook_LoadDataFiles_461557, "LoadDataFiles_461557"); // [OK]
  hnh_Hook(0x46164C, hook_ShowFadeDialog_46164C, "ShowFadeDialog_46164C"); // [OK]
  hnh_Hook(0x419B05, hook_DrawTextOnSurfaceSpaced_419B05, "DrawTextOnSurfaceSpaced_419B05");  // [OK]

  hnh_HookOn();
}
