#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ==

CBMPFile * &g_pbmpDd = *(CBMPFile * *)0x518CB4; // WRITABLE
CBMPFile * &g_pbmpDdBt = *(CBMPFile * *)0x518CB8; // WRITABLE
CBMPFile * &g_pbmpDd2 = *(CBMPFile * *)0x518CBC; // WRITABLE
CBMPFile * &g_pbmpDdBt3 = *(CBMPFile * *)0x518CC0; // WRITABLE
HGDIOBJ &g_hFontTitleMenu_518CCC = *(HGDIOBJ *)0x518CCC; // WRITABLE
uint32_t &g_nTitleMenuHotId_4B348C = *(uint32_t *)0x4B348C; // WRITABLE
uint32_t &g_bLMouseClick = *(uint32_t *)0x518CC8; // WRITABLE
uint32_t &g_bTitleMenuConfigScreen_518CC4 = *(uint32_t *)0x518CC4; // WRITABLE
uint32_t &g_nTitleMenuHotToggleRow_4B3490 = *(uint32_t *)0x4B3490; // WRITABLE

// === GLOBAL FUNCTIONS ===

WNDPROC WndProcTitleMenu_428C11 = (WNDPROC)0x428C11;
BOOL (__cdecl*FreeBmp_4211ED)(CBMPFile *pBMPFile) = reinterpret_cast<BOOL (__cdecl *)(CBMPFile *pBMPFile)>(0x4211ED);
void (__cdecl*GetNthActiveVideoMode)(CDisplayConfig* pDisplayConfig, int modeSetIndex, int nthActiveModeIndex, _DWORD* pWidth, _DWORD* pHeight, unsigned int* pBpp)
= reinterpret_cast<void (__cdecl*)(CDisplayConfig *pDisplayConfig, int modeSetIndex, int nthActiveModeIndex, _DWORD *pWidth, _DWORD *pHeight, unsigned int *pBpp)>(0x42833C);
void (__cdecl*GetNthValidDimensionString)(HWND hWnd, CDisplayConfig *pDisplayConfig, int nIndex, int *pOutIndex, LPSTR pszOutput)
= reinterpret_cast<void(__cdecl*)(HWND hWnd, CDisplayConfig *pDisplayConfig, int nIndex, int *pOutIndex, LPSTR pszOutput)>(0x428A7C);
void (__cdecl*DrawTextPairs)(HDC hdc, int xBase, int y, int xStep, LPSTR pStr, int byteCount)
= reinterpret_cast<void (__cdecl*)(HDC hdc, int xBase, int y, int xStep, LPSTR pStr, int byteCount)>(0x428BC1);
CBMPFile* (__cdecl*LoadBmp_420DF0)(LPCSTR lpFileName) = reinterpret_cast<CBMPFile*(__cdecl*)(LPCSTR lpFileName)>(0x420DF0);
void (__cdecl*UpdateRenderOptionsCheckboxes)(HWND hDlg) = reinterpret_cast<void (__cdecl*)(HWND hDlg)>(0x42889B);
CBMPColor* (__cdecl*CreatePaletteFromBuffer)(BYTE* lpBuffer, int count) = reinterpret_cast<CBMPColor*(__cdecl*)(BYTE *lpBuffer, int count)>(0x421095);

// === FUNCTION IMPLEMENTATIONS ===

int &g_nBMPCounts_518388 = *(int*)0x518388;

BOOL __cdecl hook_FreeBmp_4211ED(CBMPFile *pBMPFile)
{
  if ( !pBMPFile )
    return 0;

  DebugLog("[HNH:SUB] FreeBmp_4211ED\n");

  if ( pBMPFile->pBMPColor )
    GlobalFree(pBMPFile->pBMPColor);
  GlobalFree(pBMPFile->pBitmapFileHeader);
  GlobalFree(pBMPFile);
  --g_nBMPCounts_518388;
  return 1;
}

CBMPFile *__cdecl hook_LoadBmp_420DF0(LPCSTR lpFileName)
{
  CBMPColor *PaletteFromBuffer; // eax
  DWORD dwBytes; // [esp+0h] [ebp-20h]
  DWORD NumberOfBytesRead; // [esp+4h] [ebp-1Ch] BYREF
  CBMPFile *pOut; // [esp+8h] [ebp-18h]
  HANDLE hFile; // [esp+Ch] [ebp-14h]

  int biClrUsed; // [esp+14h] [ebp-Ch]
  LPBYTE lpBuffer; // [esp+18h] [ebp-8h]

  DebugLog("[HNH:SUB] LoadBmp_420DF0\n");

  hFile = CreateFileA(lpFileName, 0x80000000, 1u, 0, 3u, 0x8000000u, 0);
  if ( !hFile || hFile == INVALID_HANDLE_VALUE )
    return 0;
  dwBytes = GetFileSize(hFile, 0);
  lpBuffer = (LPBYTE)GlobalAlloc(0, dwBytes);
  if ( lpBuffer )
  {
    ReadFile(hFile, lpBuffer, dwBytes, &NumberOfBytesRead, 0);
    CloseHandle(hFile);
    pOut = (CBMPFile*)GlobalAlloc(0, 0x10u);
    if ( pOut )
    {
      pOut->pBitmapFileHeader = (BITMAPFILEHEADER*)lpBuffer;
      lpBuffer += 14;
      pOut->pBitmapInfoHeader = (BITMAPINFOHEADER*)lpBuffer;
      lpBuffer += 40;
      pOut->pBitmapFileHeader->bfSize = dwBytes;
      if ( pOut->pBitmapInfoHeader->biBitCount == 8 || pOut->pBitmapInfoHeader->biBitCount == 4 )
      {
        if ( pOut->pBitmapInfoHeader->biClrUsed )
          biClrUsed = pOut->pBitmapInfoHeader->biClrUsed;
        else
          biClrUsed = 1 << pOut->pBitmapInfoHeader->biBitCount;
      }
      else
      {
        biClrUsed = 0;
      }
      if ( biClrUsed <= 0 )
      {
        pOut->pBMPColor = 0;
      }
      else
      {
      
        PaletteFromBuffer = CreatePaletteFromBuffer(lpBuffer, biClrUsed);
        pOut->pBMPColor = PaletteFromBuffer;
      }
      lpBuffer = lpBuffer + 4 * biClrUsed;
      pOut->pData = lpBuffer;
      ++g_nBMPCounts_518388;
      return pOut;
    }
    else
    {
      GlobalFree(lpBuffer);
      return 0;
    }
  }
  else
  {
    CloseHandle(hFile);
    return 0;
  }
}

// WndProcTitleMenu_428C11 at 0x428C11
// Window procedure for the title menu - handles main menu and config screen

LRESULT __stdcall hook_WndProcTitleMenu_428C11(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam)
{
  int cchDriverDesc1; // eax
  int cchDriverDesc2; // eax
  unsigned int nDevNameX; // esi
  int cchDevNameCenter; // eax
  int cchDimStr; // eax
  int cchTipClose; // eax
  int cchTipDevMode1; // eax
  int cchTipDevMode2; // eax
  int cchTipDevMode3; // eax
  int cchTipRes; // eax
  int cchTipPresetSpeed; // eax
  int cchTipPresetStd; // eax
  int cchTipPresetQual; // eax
  int cchTipTglAA; // eax
  int cchTipTglDither1; // eax
  int cchTipTglDither2; // eax
  int cchTipTglDither3; // eax
  int cchTipTglSpec1; // eax
  int cchTipTglSpec2; // eax
  int cchTipTglFog; // eax
  int cchTipTglLight; // eax
  int cchTipTglShadow; // eax
  int nDevNameY; // [esp-Ch] [ebp-2A0h]
  const CHAR *driverDesc; // [esp-8h] [ebp-29Ch]
  int cchDevName; // [esp-4h] [ebp-298h]
  HBITMAP hbmPaint; // [esp+8h] [ebp-28Ch]
  tagPAINTSTRUCT Paint; // [esp+Ch] [ebp-288h] BYREF
  CHAR String[512]; // [esp+4Ch] [ebp-248h] BYREF
  HGDIOBJ hOldBitmap; // [esp+24Ch] [ebp-48h]
  HGDIOBJ hOldFont; // [esp+250h] [ebp-44h]
  int nPrevHotId; // [esp+254h] [ebp-40h]
  HDC hdc; // [esp+258h] [ebp-3Ch]
  int i; // [esp+25Ch] [ebp-38h]
  HBRUSH hbr; // [esp+260h] [ebp-34h]
  HDC hDC; // [esp+264h] [ebp-30h]
  int nImgOffY; // [esp+268h] [ebp-2Ch]
  int nImgOffX; // [esp+26Ch] [ebp-28h]
  struct tagPOINT Point; // [esp+270h] [ebp-24h] BYREF
  D3DX_DEVICEDESC *parrD3DXDevDesc; // [esp+278h] [ebp-1Ch]
  int nMainMenuItem; // [esp+27Ch] [ebp-18h]
  struct tagRECT Rect; // [esp+280h] [ebp-14h] BYREF
  int bMouseClicked; // [esp+290h] [ebp-4h]

  DebugLog("[HNH:SUB] hook_WndProcTitleMenu_428C11\n");

  hOldBitmap = 0;
  hbr = 0;
  hdc = 0;
  hDC = 0;
  nImgOffX = 0;
  nImgOffY = 0;
  bMouseClicked = 0;
  nMainMenuItem = 0;
  g_hWnd = hWnd;
  if ( Msg > WM_PAINT )
  {
    if ( Msg == WM_CLOSE )
    {
      if ( g_pbmpDd )
        FreeBmp_4211ED(g_pbmpDd);
      if ( g_pbmpDdBt )
        FreeBmp_4211ED(g_pbmpDdBt);
      if ( g_pbmpDd2 )
        FreeBmp_4211ED(g_pbmpDd2);
      if ( g_pbmpDdBt3 )
        FreeBmp_4211ED(g_pbmpDdBt3);
      g_pbmpDdBt3 = 0;
      g_pbmpDd2 = 0;
      g_pbmpDdBt = 0;
      g_pbmpDd = 0;
      DestroyWindow(hWnd);
    }
    else if ( Msg == WM_TIMER )
    {
      nPrevHotId = g_nTitleMenuHotId_4B348C;
      g_nTitleMenuHotId_4B348C = -1;
      GetClientRect(hWnd, &Rect);
      if ( GetAsyncKeyState(VK_ESCAPE) >> 8 )
      {
        DestroyWindow(hWnd);
        return 0;
      }
      GetCursorPos(&Point);
      if ( GetAsyncKeyState(VK_LBUTTON) >> 8 )
      {
        if ( !g_bLMouseClick )
        {
          g_bLMouseClick = 1;
          bMouseClicked = 1;
        }
      }
      else
      {
        g_bLMouseClick = 0;
      }
      if ( g_bTitleMenuConfigScreen_518CC4 )
      {
        i = 6; // fix msvc warning
        if ( g_pbmpDd2 )
        {
          nImgOffX = (Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) / 2;
          nImgOffY = (Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) / 2;
        }
        if ( nImgOffX + 590 < Point.x
          && Point.x < nImgOffX + 618
          && nImgOffY + 258 < Point.y
          && Point.y < nImgOffY + 286
          || GetAsyncKeyState(2) >> 8 )
        {
          g_nTitleMenuHotId_4B348C = 0;
          if ( bMouseClicked == 1 || GetAsyncKeyState(VK_RBUTTON) >> 8 )
          {
            bMouseClicked = 1;
            g_nTitleMenuHotId_4B348C = -1;
            nPrevHotId = -2;
            InvalidateRect(hWnd, &Rect, 1);
            UpdateWindow(hWnd);
            g_bTitleMenuConfigScreen_518CC4 = 0;
          }
        }
        else
        {
          for ( i = 0; i < 6; ++i )
          {
            if ( nImgOffX + 216 * (i % 2) + 164 < Point.x
              && Point.x < nImgOffX + 216 * (i % 2) + 344
              && nImgOffY + 40 * (i / 2) + 136 < Point.y
              && Point.y < nImgOffY + 40 * (i / 2) + 160 )
            {
              if ( g_nTitleMenuHotToggleRow_4B3490 == -1 || g_nTitleMenuHotToggleRow_4B3490 != i )
              {
                g_nTitleMenuHotToggleRow_4B3490 = i;
                InvalidateRect(hWnd, &Rect, 0);
                UpdateWindow(hWnd);
              }
              if ( bMouseClicked == 1 )
              {
                if ( !i )
                  g_bAntiAliasEnable = g_bAntiAliasEnable == 0;
                if ( i == 1 )
                  g_bDitherEnable = g_bDitherEnable == 0;
                if ( i == 2 )
                  g_bSpecularEnable_4B3474 = g_bSpecularEnable_4B3474 == 0;
                if ( i == 3 )
                  g_bFogIsActive_4B3480 = !g_bFogIsActive_4B3480;
                if ( i == 4 )
                  g_bLightEnable_4B3484 = g_bLightEnable_4B3484 == 0;
                if ( i == 5 )
                  g_bShadowEnable_518CA8 = g_bShadowEnable_518CA8 == 0;
                InvalidateRect(hWnd, &Rect, 0);
                UpdateWindow(hWnd);
              }
              break;
            }
          }
        }
        if ( i == 6 && g_nTitleMenuHotToggleRow_4B3490 != -1 ) // i maybe undefined
        {
          g_nTitleMenuHotToggleRow_4B3490 = -1;
          InvalidateRect(hWnd, &Rect, 0);
          UpdateWindow(hWnd);
        }
        for ( i = 0; i < 3; ++i )
        {
          if ( nImgOffX + 140 * i + 160 < Point.x
            && Point.x < nImgOffX + 140 * (i + 1) + 140
            && nImgOffY + 264 < Point.y
            && Point.y < nImgOffY + 288 )
          {
            g_nTitleMenuHotId_4B348C = i + 7;
            if ( bMouseClicked == 1 )
            {
              g_nTitleMenuHotId_4B348C = -1;
              if ( i )
              {
                if ( i == 1 )
                {
                  g_bAntiAliasEnable = 0;
                  g_bDitherEnable = 1;
                  g_bSpecularEnable_4B3474 = 1;
                  g_bFogIsActive_4B3480 = 1;
                  g_bLightEnable_4B3484 = 1;
                  g_bShadowEnable_518CA8 = 0;
                }
                else
                {
                  g_bAntiAliasEnable = 1;
                  g_bDitherEnable = 1;
                  g_bSpecularEnable_4B3474 = 1;
                  g_bFogIsActive_4B3480 = 1;
                  g_bLightEnable_4B3484 = 1;
                  g_bShadowEnable_518CA8 = 1;
                }
              }
              else
              {
                g_bAntiAliasEnable = 0;
                g_bDitherEnable = 0;
                g_bSpecularEnable_4B3474 = 0;
                g_bFogIsActive_4B3480 = 0;
                g_bLightEnable_4B3484 = 0;
                g_bShadowEnable_518CA8 = 0;
              }
            }
          }
        }
        if ( nImgOffX + 326 >= Point.x
          || Point.x >= nImgOffX + 346
          || nImgOffY + 50 >= Point.y
          || Point.y >= nImgOffY + 70 )
        {
          if ( nImgOffX + 382 >= Point.x
            || Point.x >= nImgOffX + 402
            || nImgOffY + 50 >= Point.y
            || Point.y >= nImgOffY + 70 )
          {
            if ( nImgOffX + 506 >= Point.x
              || Point.x >= nImgOffX + 526
              || nImgOffY + 98 >= Point.y
              || Point.y >= nImgOffY + 118 )
            {
              if ( nImgOffX + 562 >= Point.x
                || Point.x >= nImgOffX + 582
                || nImgOffY + 98 >= Point.y
                || Point.y >= nImgOffY + 118 )
              {
                if ( nImgOffX + 146 >= Point.x
                  || Point.x >= nImgOffX + 166
                  || nImgOffY + 96 >= Point.y
                  || Point.y >= nImgOffY + 116 )
                {
                  if ( nImgOffX + 202 < Point.x
                    && Point.x < nImgOffX + 222
                    && nImgOffY + 96 < Point.y
                    && Point.y < nImgOffY + 116 )
                  {
                    g_nTitleMenuHotId_4B348C = 6;
                    if ( bMouseClicked == 1 )
                    {
                      ++g_nResolutionMode_518C80;
                      g_nTitleMenuHotId_4B348C = -1;
                    }
                  }
                }
                else
                {
                  g_nTitleMenuHotId_4B348C = 5;
                  if ( bMouseClicked == 1 )
                  {
                    --g_nResolutionMode_518C80;
                    g_nTitleMenuHotId_4B348C = -1;
                  }
                }
              }
              else
              {
                g_nTitleMenuHotId_4B348C = 4;
                if ( bMouseClicked == 1 )
                {
                  ++g_nZBufDepthMode_518C9C;
                  g_nTitleMenuHotId_4B348C = -1;
                }
              }
            }
            else
            {
              g_nTitleMenuHotId_4B348C = 3;
              if ( bMouseClicked == 1 )
              {
                --g_nZBufDepthMode_518C9C;
                g_nTitleMenuHotId_4B348C = -1;
              }
            }
          }
          else
          {
            g_nTitleMenuHotId_4B348C = 2;
            if ( bMouseClicked == 1 )
            {
              ++g_nRenderHALMode_518CA0;
              g_nTitleMenuHotId_4B348C = -1;
            }
          }
        }
        else
        {
          g_nTitleMenuHotId_4B348C = 1;
          if ( bMouseClicked == 1 )
          {
            --g_nRenderHALMode_518CA0;
            g_nTitleMenuHotId_4B348C = -1;
          }
        }
        if ( g_nRenderHALMode_518CA0 < 0 )
          g_nRenderHALMode_518CA0 = 0;
        if ( g_nRenderHALMode_518CA0 >= g_displayConfig_518C30.nDeviceCount )
          g_nRenderHALMode_518CA0 = g_displayConfig_518C30.nDeviceCount - 1;
        if ( g_nZBufDepthMode_518C9C < 0 )
          g_nZBufDepthMode_518C9C = 0;
        if ( g_nZBufDepthMode_518C9C >= g_arrZBufDepthCountPerDev_518C40[g_nRenderHALMode_518CA0] )
          g_nZBufDepthMode_518C9C = g_arrZBufDepthCountPerDev_518C40[g_nRenderHALMode_518CA0] - 1;
      }
      else
      {
        if ( g_pbmpDd )
        {
          nImgOffX = (Rect.right - g_pbmpDd->pBitmapInfoHeader->biWidth) / 2;
          nImgOffY = (Rect.bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) / 2;
        }
        nMainMenuItem = -1;
        for ( i = 0; i < 3; ++i )
        {
          if ( nImgOffX + 469 < Point.x
            && Point.x < nImgOffX + 597
            && nImgOffY + 160 * i + 104 < Point.y
            && Point.y < nImgOffY + 160 * i + 136 )
          {
            nMainMenuItem = i;
            break;
          }
        }
        if ( nMainMenuItem )
        {
          if ( nMainMenuItem == 2 )
          {
            g_nTitleMenuHotId_4B348C = 1;
            if ( bMouseClicked == 1 )
            {
              g_bRunGame = 0;
              DestroyWindow(hWnd);
              return 0;
            }
          }
          else if ( nMainMenuItem == 1 )
          {
            g_nTitleMenuHotId_4B348C = 2;
            if ( bMouseClicked == 1 )
            {
              g_nTitleMenuHotId_4B348C = -1;
              InvalidateRect(hWnd, &Rect, 1);
              UpdateWindow(hWnd);
              g_bTitleMenuConfigScreen_518CC4 = 1;
            }
          }
        }
        else
        {
          g_nTitleMenuHotId_4B348C = 0;
          if ( bMouseClicked == 1 )
          {
            g_bRunGame = 1;
            g_nModeSetIndex = g_nRenderHALMode_518CA0;
            GetNthActiveVideoMode(
              &g_displayConfig_518C30,
              g_nRenderHALMode_518CA0,
              g_nResolutionMode_518C80,
              &g_nWidth,
              &g_nHeight,
              &g_nBitness);
            g_nZBufDepthSelValue_518C98 = g_arrZBufDepthsPerDev_518828[16 * g_nModeSetIndex + g_nZBufDepthMode_518C9C];
            DestroyWindow(hWnd);
            return 0;
          }
        }
      }
      if ( g_nTitleMenuHotId_4B348C != nPrevHotId )
      {
        InvalidateRect(hWnd, &Rect, 0);
        UpdateWindow(hWnd);
      }
    }
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
  switch ( Msg )
  {
  case WM_PAINT:
    GetClientRect(hWnd, &Rect);
    hdc = BeginPaint(hWnd, &Paint);
    hDC = CreateCompatibleDC(hdc);
    hbmPaint = CreateCompatibleBitmap(hdc, Rect.right - Rect.left, Rect.bottom - Rect.top);
    hOldBitmap = SelectObject(hDC, hbmPaint);
    hbr = CreateSolidBrush(0xFFFFFFu);
    FillRect(hDC, &Rect, hbr);
    DeleteObject(hbr);
    if ( g_bTitleMenuConfigScreen_518CC4 )
    {
      if ( g_pbmpDd2 )
      {
        StretchDIBits(
          hDC,
          (Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1,
          (Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1,
          g_pbmpDd2->pBitmapInfoHeader->biWidth,
          g_pbmpDd2->pBitmapInfoHeader->biHeight,
          0,
          0,
          g_pbmpDd2->pBitmapInfoHeader->biWidth,
          g_pbmpDd2->pBitmapInfoHeader->biHeight,
          g_pbmpDd2->pData,
          (BITMAPINFO*)g_pbmpDd2->pBitmapInfoHeader,
          0,
          0xCC0020u);
        if ( g_pbmpDdBt )
        {
          if ( !g_nTitleMenuHotId_4B348C )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 590,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 258,
              28,
              28,
              120,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 192,
              28,
              28,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          if ( g_nTitleMenuHotId_4B348C == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 326,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 50,
              20,
              20,
              120,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 164,
              20,
              20,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          if ( g_nTitleMenuHotId_4B348C == 2 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 382,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 50,
              20,
              20,
              140,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 164,
              20,
              20,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          if ( g_nTitleMenuHotId_4B348C == 5 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 146,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 97,
              20,
              20,
              120,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 164,
              20,
              20,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          if ( g_nTitleMenuHotId_4B348C == 6 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 202,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 97,
              20,
              20,
              140,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 164,
              20,
              20,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 0;
          if ( g_nTitleMenuHotId_4B348C == 7 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 164,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 264,
              120,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 168,
              120,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 1;
          if ( g_nTitleMenuHotId_4B348C == 8 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 304,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 264,
              120,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 192,
              120,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 2;
          if ( g_nTitleMenuHotId_4B348C == 9 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 445,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 264,
              120,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 216,
              120,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 0;
          if ( g_bAntiAliasEnable == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 164,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 136,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 24,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 1;
          if ( g_bDitherEnable == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 380,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 136,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 48,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 2;
          if ( g_bSpecularEnable_4B3474 == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 164,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 176,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 72,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 3;
          if ( g_bFogIsActive_4B3480 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 380,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 176,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 96,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 4;
          if ( g_bLightEnable_4B3484 == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 164,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 216,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 120,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
          i = 5;
          if ( g_bShadowEnable_518CA8 == 1 )
            StretchDIBits(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 380,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 216,
              183,
              24,
              0,
              g_pbmpDdBt->pBitmapInfoHeader->biHeight - 144,
              183,
              24,
              g_pbmpDdBt->pData,
              (BITMAPINFO*)g_pbmpDdBt->pBitmapInfoHeader,
              0,
              0xCC0020u);
        }
      }
      parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;
      hOldFont = SelectObject(hDC, g_hFontTitleMenu_518CCC);
      SetTextColor(hDC, 0);
      SetBkMode(hDC, 1);
      for ( i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
      {
        if ( g_nRenderHALMode_518CA0 == i )
        {
          cchDevName = lstrlenA(parrD3DXDevDesc->driverDesc);
          driverDesc = parrD3DXDevDesc->driverDesc;
          nDevNameY = ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 27;
          nDevNameX = ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 364;
          cchDevNameCenter = lstrlenA(parrD3DXDevDesc->driverDesc);
          TextOutA(hDC, nDevNameX - 14 * (cchDevNameCenter / 2) / 2, nDevNameY, driverDesc, cchDevName);
          break;
        }
        ++parrD3DXDevDesc;
      }
      GetNthValidDimensionString(
        hWnd,
        &g_displayConfig_518C30,
        g_nRenderHALMode_518CA0,
        &g_nResolutionMode_518C80,
        String);
      cchDimStr = lstrlenA(String);
      TextOutA(
        hDC,
        ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 121,
        ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 76,
        String,
        cchDimStr);
      if ( g_nTitleMenuHotId_4B348C )
      {
        if ( g_nTitleMenuHotId_4B348C == 1 || g_nTitleMenuHotId_4B348C == 2 )
        {
          wsprintfA(String, "　このボタンを押すことで動作モードを変えるができます。");
          cchTipDevMode1 = lstrlenA(String);
          DrawTextPairs(
            hDC,
            ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
            ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
            13,
            String,
            cchTipDevMode1);
          wsprintfA(
            String,
            "　 Emulationモードはソフトウエアのみで動作させるので安定した動作");
          cchTipDevMode2 = lstrlenA(String);
          DrawTextPairs(
            hDC,
            ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
            ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 370,
            13,
            String,
            cchTipDevMode2);
          wsprintfA(
            String,
            "　を行いますが大変低速です。できるだけ他のモードで動かして下さい。");
          cchTipDevMode3 = lstrlenA(String);
          DrawTextPairs(
            hDC,
            ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
            ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 390,
            13,
            String,
            cchTipDevMode3);
        }
        else if ( g_nTitleMenuHotId_4B348C != 3 && g_nTitleMenuHotId_4B348C != 4 )
        {
          switch ( g_nTitleMenuHotId_4B348C )
          {
          case 5:
          case 6:
            wsprintfA(String, "　このボタンを押すことで画面の解像度を変えられます。");
            cchTipRes = lstrlenA(String);
            DrawTextPairs(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
              13,
              String,
              cchTipRes);
            break;
          case 7:
            wsprintfA(String, "　環境設定スイッチが速度を最優先にした設定になります。");
            cchTipPresetSpeed = lstrlenA(String);
            DrawTextPairs(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
              13,
              String,
              cchTipPresetSpeed);
            break;
          case 8:
            wsprintfA(String, "　環境設定スイッチが標準設定になります。");
            cchTipPresetStd = lstrlenA(String);
            DrawTextPairs(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
              13,
              String,
              cchTipPresetStd);
            break;
          case 9:
            wsprintfA(String, "　環境設定スイッチが画質を最優先にした設定になります。");
            cchTipPresetQual = lstrlenA(String);
            DrawTextPairs(
              hDC,
              ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
              ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
              13,
              String,
              cchTipPresetQual);
            break;
          default:
            if ( g_nTitleMenuHotToggleRow_4B3490 )
            {
              switch ( g_nTitleMenuHotToggleRow_4B3490 )
              {
              case 1:
                wsprintfA(
                  String,
                  "　このスイッチをオンにすると、テクスチャーの色を滑らかにします。");
                cchTipTglDither1 = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                  13,
                  String,
                  cchTipTglDither1);
                wsprintfA(
                  String,
                  "＊　一部のグラフィックカードのドライバーで T&Lモードの時に、この");
                cchTipTglDither2 = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 370,
                  13,
                  String,
                  cchTipTglDither2);
                wsprintfA(
                  String,
                  "　スイッチをオンにすると物体の一部が変色してしまう場合があります。");
                cchTipTglDither3 = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 390,
                  13,
                  String,
                  cchTipTglDither3);
                break;
              case 2:
                wsprintfA(
                  String,
                  "　このスイッチをオンにすると、特定の物体に対して光沢を使用します。");
                cchTipTglSpec1 = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                  13,
                  String,
                  cchTipTglSpec1);
                wsprintfA(
                  String,
                  "＊　ライトスイッチがオンの時のみ、このスイッチは効果をあらわします。");
                cchTipTglSpec2 = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 370,
                  13,
                  String,
                  cchTipTglSpec2);
                break;
              case 3:
                wsprintfA(
                  String,
                  "　このスイッチをオンにすると、画面に霧の効果をつけます。");
                cchTipTglFog = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                  13,
                  String,
                  cchTipTglFog);
                break;
              case 4:
                wsprintfA(
                  String,
                  "　このスイッチをオンにすると、光と陰の効果をつけます。");
                cchTipTglLight = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                  13,
                  String,
                  cchTipTglLight);
                break;
              case 5:
                wsprintfA(
                  String,
                  "　このスイッチをオンにすると、影などの効果を描画します");
                cchTipTglShadow = lstrlenA(String);
                DrawTextPairs(
                  hDC,
                  ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                  ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                  13,
                  String,
                  cchTipTglShadow);
                break;
              }
            }
            else
            {
              wsprintfA(String, "　このスイッチをオンにすると、ポリゴンの角を滑らかにします。");
              cchTipTglAA = lstrlenA(String);
              DrawTextPairs(
                hDC,
                ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
                ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
                13,
                String,
                cchTipTglAA);
            }
            break;
          }
        }
      }
      else
      {
        wsprintfA(String, "　このボタンを押すと環境設定画面から出ます。");
        cchTipClose = lstrlenA(String);
        DrawTextPairs(
          hDC,
          ((Rect.right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1) + 180,
          ((Rect.bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1) + 350,
          13,
          String,
          cchTipClose);
      }
      SelectObject(hDC, hOldFont);
    }
    else if ( g_pbmpDd )
    {
      StretchDIBits(
        hDC,
        (Rect.right - g_pbmpDd->pBitmapInfoHeader->biWidth) >> 1,
        (Rect.bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) >> 1,
        g_pbmpDd->pBitmapInfoHeader->biWidth,
        g_pbmpDd->pBitmapInfoHeader->biHeight,
        0,
        0,
        g_pbmpDd->pBitmapInfoHeader->biWidth,
        g_pbmpDd->pBitmapInfoHeader->biHeight,
        g_pbmpDd->pData,
        (BITMAPINFO*)g_pbmpDd->pBitmapInfoHeader,
        0,
        0xCC0020u);
      if ( g_pbmpDdBt3 )
      {
        if ( !g_nTitleMenuHotId_4B348C )
          StretchDIBits(
            hDC,
            ((Rect.right - g_pbmpDd->pBitmapInfoHeader->biWidth) >> 1) + 469,
            ((Rect.bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) >> 1) + 104,
            128,
            32,
            0,
            g_pbmpDdBt3->pBitmapInfoHeader->biHeight - 32,
            128,
            32,
            g_pbmpDdBt3->pData,
            (BITMAPINFO*)g_pbmpDdBt3->pBitmapInfoHeader,
            0,
            0xCC0020u);
        if ( g_nTitleMenuHotId_4B348C == 2 )
          StretchDIBits(
            hDC,
            ((Rect.right - g_pbmpDd->pBitmapInfoHeader->biWidth) >> 1) + 469,
            ((Rect.bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) >> 1) + 264,
            128,
            32,
            0,
            g_pbmpDdBt3->pBitmapInfoHeader->biHeight - 64,
            128,
            32,
            g_pbmpDdBt3->pData,
            (BITMAPINFO*)g_pbmpDdBt3->pBitmapInfoHeader,
            0,
            0xCC0020u);
        if ( g_nTitleMenuHotId_4B348C == 1 )
          StretchDIBits(
            hDC,
            ((Rect.right - g_pbmpDd->pBitmapInfoHeader->biWidth) >> 1) + 469,
            ((Rect.bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) >> 1) + 424,
            128,
            32,
            0,
            g_pbmpDdBt3->pBitmapInfoHeader->biHeight - 96,
            128,
            32,
            g_pbmpDdBt3->pData,
            (BITMAPINFO*)g_pbmpDdBt3->pBitmapInfoHeader,
            0,
            0xCC0020u);
      }
    }
    BitBlt(hdc, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, hDC, 0, 0, 0xCC0020u);
    EndPaint(hWnd, &Paint);
    SelectObject(hDC, hOldBitmap);
    DeleteObject(hbmPaint);
    DeleteObject(hDC);
    ReleaseDC(hWnd, hdc);
    return 0;
  case WM_CREATE:
    g_hFontTitleMenu_518CCC = CreateFontA(14, 7, 0, 0, 400, 0, 0, 0, 0x80u, 0, 0, 2u, 0x30u, "ＭＳ 明朝");
    SetTimer(hWnd, 0x113u, 0x32u, 0);
    wsprintfA(String, "DD.bmp");
    CHanaObj__GetRealPath_486D9A(String, 1007);
    g_pbmpDd = LoadBmp_420DF0(String);
    wsprintfA(String, "DD_BT.bmp");
    CHanaObj__GetRealPath_486D9A(String, 1007);
    g_pbmpDdBt = LoadBmp_420DF0(String);
    wsprintfA(String, "DD2.bmp");
    CHanaObj__GetRealPath_486D9A(String, 1007);
    g_pbmpDd2 = LoadBmp_420DF0(String);
    wsprintfA(String, "DD_BT3.bmp");
    CHanaObj__GetRealPath_486D9A(String, 1007);
    g_pbmpDdBt3 = LoadBmp_420DF0(String);
    UpdateRenderOptionsCheckboxes(hWnd);
    g_nRenderHALMode_518CA0 = 0;
    parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;
    for ( i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
    {
      cchDriverDesc1 = lstrlenA(parrD3DXDevDesc->driverDesc);
      if ( !memcmp(&parrD3DXDevDesc->ddDeviceID.Data4[cchDriverDesc1 + 2], "3D HAL", 6u) )
      {
        g_nRenderHALMode_518CA0 = i;
        break;
      }
      ++parrD3DXDevDesc;
    }
    if ( !g_nRenderHALMode_518CA0 )
    {
      parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;
      for ( i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
      {
        cchDriverDesc2 = lstrlenA(parrD3DXDevDesc->driverDesc);
        if ( !memcmp(&parrD3DXDevDesc->ddDeviceID.Data3 + cchDriverDesc2 + 1, "Emulation", 9u) )
        {
          g_nRenderHALMode_518CA0 = i;
          return 0;
        }
        ++parrD3DXDevDesc;
      }
    }
    return 0;
  case WM_DESTROY:
    if ( g_pbmpDd )
      FreeBmp_4211ED(g_pbmpDd);
    if ( g_pbmpDdBt )
      FreeBmp_4211ED(g_pbmpDdBt);
    if ( g_pbmpDd2 )
      FreeBmp_4211ED(g_pbmpDd2);
    if ( g_pbmpDdBt3 )
      FreeBmp_4211ED(g_pbmpDdBt3);
    g_pbmpDdBt3 = 0;
    g_pbmpDd2 = 0;
    g_pbmpDdBt = 0;
    g_pbmpDd = 0;
    Sleep(0x64u);
    PostQuitMessage(0);
    return 1;
  default:
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
}

// Register hook if needed
void register_WndProcTitleMenu_428C11() {
    hnh_Hook(0x428C11, hook_WndProcTitleMenu_428C11, "WndProcTitleMenu_428C11");
    hnh_Hook(0x4211ED, hook_FreeBmp_4211ED, "FreeBmp_4211ED");
    hnh_Hook(0x420DF0, hook_LoadBmp_420DF0, "LoadBmp_420DF0");
}