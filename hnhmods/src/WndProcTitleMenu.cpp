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

// === TITLE MENU HELPERS ===

// Exclusive-bounds hit test: x1 < x < x2, y1 < y < y2.
static bool PtInBox(const POINT &Point, int x1, int y1, int x2, int y2)
{
  return x1 < Point.x && Point.x < x2 && y1 < Point.y && Point.y < y2;
}

// Tracks the left mouse button; true only on the frame it goes down.
static int PollLMouseButton()
{
  if ( GetAsyncKeyState(VK_LBUTTON) >> 8 )
  {
    if ( !g_bLMouseClick )
    {
      g_bLMouseClick = 1;
      return 1;
    }
  }
  else
  {
    g_bLMouseClick = 0;
  }
  return 0;
}

static void FreeTitleMenuBitmaps()
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
}

// Blits a whole bitmap with its origin at (x, y).
static void BlitBmp(HDC hDC, int x, int y, CBMPFile *pBmp)
{
  int nWidth = pBmp->pBitmapInfoHeader->biWidth;
  int nHeight = pBmp->pBitmapInfoHeader->biHeight;
  StretchDIBits(hDC, x, y, nWidth, nHeight, 0, 0, nWidth, nHeight, pBmp->pData,
    (BITMAPINFO *)pBmp->pBitmapInfoHeader, 0, SRCCOPY);
}

// Blits a w*h cell of pBmp; srcYFromBottom is measured up from the bottom edge.
static void BlitBmpPart(HDC hDC, int x, int y, int w, int h, CBMPFile *pBmp, int srcX,
  int srcYFromBottom)
{
  StretchDIBits(hDC, x, y, w, h, srcX, pBmp->pBitmapInfoHeader->biHeight - srcYFromBottom,
    w, h, pBmp->pData, (BITMAPINFO *)pBmp->pBitmapInfoHeader, 0, SRCCOPY);
}

// Draws one help/tip line in the fixed 13px cell pitch used by the config screen.
static void DrawTip(HDC hDC, int xBase, int y, LPCSTR pszText)
{
  DrawTextPairs(hDC, xBase, y, 13, (LPSTR)pszText, lstrlenA(pszText));
}

// Stores the picked video mode into the globals the renderer reads, then closes the menu.
static void StartGame(HWND hWnd)
{
  g_bRunGame = 1;
  g_nModeSetIndex = g_nRenderHALMode_518CA0;
  GetNthActiveVideoMode(&g_displayConfig_518C30, g_nRenderHALMode_518CA0,
    g_nResolutionMode_518C80, &g_nWidth, &g_nHeight, &g_nBitness);
  g_nZBufDepthSelValue_518C98 = g_arrZBufDepthsPerDev_518828[16 * g_nModeSetIndex + g_nZBufDepthMode_518C9C];
  DestroyWindow(hWnd);
}

// Prefers the first device whose description mentions "3D HAL", else "Emulation".
static void PickDefaultRenderHALMode()
{
  D3DX_DEVICEDESC *parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;

  g_nRenderHALMode_518CA0 = 0;
  for ( int i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
  {
    int cchDriverDesc = lstrlenA(parrD3DXDevDesc->driverDesc);
    if ( !memcmp(&parrD3DXDevDesc->ddDeviceID.Data4[cchDriverDesc + 2], "3D HAL", 6u) )
    {
      g_nRenderHALMode_518CA0 = i;
      return;
    }
    ++parrD3DXDevDesc;
  }
  parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;
  for ( int i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
  {
    int cchDriverDesc = lstrlenA(parrD3DXDevDesc->driverDesc);
    if ( !memcmp(&parrD3DXDevDesc->ddDeviceID.Data3 + cchDriverDesc + 1, "Emulation", 9u) )
    {
      g_nRenderHALMode_518CA0 = i;
      return;
    }
    ++parrD3DXDevDesc;
  }
}

// Keeps renderer/z-buffer selections inside their per-device ranges.
static void ClampConfigSelections()
{
  if ( g_nRenderHALMode_518CA0 < 0 )
    g_nRenderHALMode_518CA0 = 0;
  if ( g_nRenderHALMode_518CA0 >= g_displayConfig_518C30.nDeviceCount )
    g_nRenderHALMode_518CA0 = g_displayConfig_518C30.nDeviceCount - 1;
  if ( g_nZBufDepthMode_518C9C < 0 )
    g_nZBufDepthMode_518C9C = 0;
  if ( g_nZBufDepthMode_518C9C >= g_arrZBufDepthCountPerDev_518C40[g_nRenderHALMode_518CA0] )
    g_nZBufDepthMode_518C9C = g_arrZBufDepthCountPerDev_518C40[g_nRenderHALMode_518CA0] - 1;
}

// WM_TIMER input handling while the config screen is shown.
static void UpdateConfigScreenInput(HWND hWnd, const RECT *pRect, const POINT &Point,
  int bMouseClicked, int *pnPrevHotId)
{
  // the six render option toggles, in hot-row order
  static BOOL *const s_toggleFlags[6] = {
    &g_bAntiAliasEnable,
    &g_bDitherEnable,
    &g_bSpecularEnable_4B3474,
    &g_bFogIsActive_4B3480,
    &g_bLightEnable_4B3484,
    &g_bShadowEnable_518CA8,
  };
  int nImgOffX = 0;
  int nImgOffY = 0;
  if ( g_pbmpDd2 )
  {
    nImgOffX = (pRect->right - g_pbmpDd2->pBitmapInfoHeader->biWidth) / 2;
    nImgOffY = (pRect->bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) / 2;
  }

  // "close" box in the corner; a right click anywhere also leaves the screen
  if ( PtInBox(Point, nImgOffX + 590, nImgOffY + 258, nImgOffX + 618, nImgOffY + 286)
    || GetAsyncKeyState(VK_RBUTTON) >> 8 )
  {
    g_nTitleMenuHotId_4B348C = 0;
    if ( bMouseClicked == 1 || GetAsyncKeyState(VK_RBUTTON) >> 8 )
    {
      bMouseClicked = 1;
      g_nTitleMenuHotId_4B348C = -1;
      *pnPrevHotId = -2; // force a redraw of the hot state
      InvalidateRect(hWnd, pRect, 1);
      UpdateWindow(hWnd);
      g_bTitleMenuConfigScreen_518CC4 = 0;
    }
  }
  else
  {
    // toggles: 2 columns x 3 rows of 180x24 boxes
    int nHotToggleRow = -1;
    for ( int i = 0; i < 6; ++i )
    {
      int x = nImgOffX + 216 * (i % 2) + 164;
      int y = nImgOffY + 40 * (i / 2) + 136;
      if ( !PtInBox(Point, x, y, x + 180, y + 24) )
        continue;
      nHotToggleRow = i;
      if ( g_nTitleMenuHotToggleRow_4B3490 != i )
      {
        g_nTitleMenuHotToggleRow_4B3490 = i;
        InvalidateRect(hWnd, pRect, 0);
        UpdateWindow(hWnd);
      }
      if ( bMouseClicked == 1 )
      {
        *s_toggleFlags[i] = *s_toggleFlags[i] == 0;
        InvalidateRect(hWnd, pRect, 0);
        UpdateWindow(hWnd);
      }
      break;
    }
    if ( nHotToggleRow == -1 && g_nTitleMenuHotToggleRow_4B3490 != -1 )
    {
      g_nTitleMenuHotToggleRow_4B3490 = -1;
      InvalidateRect(hWnd, pRect, 0);
      UpdateWindow(hWnd);
    }
  }

  // three quality presets along the bottom edge
  static const BOOL s_presetFlags[3][6] = {
    { 0, 0, 0, 0, 0, 0 }, // speed: everything off
    { 0, 1, 1, 1, 1, 0 }, // standard
    { 1, 1, 1, 1, 1, 1 }, // quality: everything on
  };
  for ( int i = 0; i < 3; ++i )
  {
    int x = nImgOffX + 140 * i + 160;
    if ( !PtInBox(Point, x, nImgOffY + 264, x + 120, nImgOffY + 288) )
      continue;
    g_nTitleMenuHotId_4B348C = i + 7;
    if ( bMouseClicked == 1 )
    {
      g_nTitleMenuHotId_4B348C = -1;
      for ( int j = 0; j < 6; ++j )
        *s_toggleFlags[j] = s_presetFlags[i][j];
    }
  }

  // selection arrows: renderer, z-buffer depth, resolution
  if ( PtInBox(Point, nImgOffX + 326, nImgOffY + 50, nImgOffX + 346, nImgOffY + 70) )
  {
    g_nTitleMenuHotId_4B348C = 1; // renderer <
    if ( bMouseClicked == 1 )
    {
      --g_nRenderHALMode_518CA0;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }
  else if ( PtInBox(Point, nImgOffX + 382, nImgOffY + 50, nImgOffX + 402, nImgOffY + 70) )
  {
    g_nTitleMenuHotId_4B348C = 2; // renderer >
    if ( bMouseClicked == 1 )
    {
      ++g_nRenderHALMode_518CA0;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }
  else if ( PtInBox(Point, nImgOffX + 506, nImgOffY + 98, nImgOffX + 526, nImgOffY + 118) )
  {
    g_nTitleMenuHotId_4B348C = 3; // z-buffer <
    if ( bMouseClicked == 1 )
    {
      --g_nZBufDepthMode_518C9C;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }
  else if ( PtInBox(Point, nImgOffX + 562, nImgOffY + 98, nImgOffX + 582, nImgOffY + 118) )
  {
    g_nTitleMenuHotId_4B348C = 4; // z-buffer >
    if ( bMouseClicked == 1 )
    {
      ++g_nZBufDepthMode_518C9C;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }
  else if ( PtInBox(Point, nImgOffX + 146, nImgOffY + 96, nImgOffX + 166, nImgOffY + 116) )
  {
    g_nTitleMenuHotId_4B348C = 5; // resolution <
    if ( bMouseClicked == 1 )
    {
      --g_nResolutionMode_518C80;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }
  else if ( PtInBox(Point, nImgOffX + 202, nImgOffY + 96, nImgOffX + 222, nImgOffY + 116) )
  {
    g_nTitleMenuHotId_4B348C = 6; // resolution >
    if ( bMouseClicked == 1 )
    {
      ++g_nResolutionMode_518C80;
      g_nTitleMenuHotId_4B348C = -1;
    }
  }

  ClampConfigSelections();
}

// WM_TIMER input handling while the main menu is shown.
// Returns true if the window was destroyed.
static bool UpdateMainMenuInput(HWND hWnd, const RECT *pRect, const POINT &Point, int bMouseClicked)
{
  int nImgOffX = 0;
  int nImgOffY = 0;
  if ( g_pbmpDd )
  {
    nImgOffX = (pRect->right - g_pbmpDd->pBitmapInfoHeader->biWidth) / 2;
    nImgOffY = (pRect->bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) / 2;
  }

  // three stacked entries on the right side: 0=start, 1=config, 2=quit
  int nMainMenuItem = -1;
  for ( int i = 0; i < 3; ++i )
  {
    if ( PtInBox(Point, nImgOffX + 469, nImgOffY + 160 * i + 104, nImgOffX + 597,
      nImgOffY + 160 * i + 136) )
    {
      nMainMenuItem = i;
      break;
    }
  }

  switch ( nMainMenuItem )
  {
  case 0:
    g_nTitleMenuHotId_4B348C = 0;
    if ( bMouseClicked == 1 )
    {
      StartGame(hWnd);
      return true;
    }
    break;
  case 1:
    g_nTitleMenuHotId_4B348C = 2;
    if ( bMouseClicked == 1 )
    {
      g_nTitleMenuHotId_4B348C = -1;
      InvalidateRect(hWnd, pRect, 1);
      UpdateWindow(hWnd);
      g_bTitleMenuConfigScreen_518CC4 = 1;
    }
    break;
  case 2:
    g_nTitleMenuHotId_4B348C = 1;
    if ( bMouseClicked == 1 )
    {
      g_bRunGame = 0;
      DestroyWindow(hWnd);
      return true;
    }
    break;
  }
  return false;
}

// WM_TIMER: polls input at 50ms and drives the hot-highlight/click state.
static void TitleMenu_OnTimer(HWND hWnd)
{
  RECT Rect;
  POINT Point;
  int nPrevHotId = g_nTitleMenuHotId_4B348C;

  g_nTitleMenuHotId_4B348C = -1;
  GetClientRect(hWnd, &Rect);
  if ( GetAsyncKeyState(VK_ESCAPE) >> 8 )
  {
    DestroyWindow(hWnd);
    return;
  }
  GetCursorPos(&Point);
  int bMouseClicked = PollLMouseButton();

  if ( g_bTitleMenuConfigScreen_518CC4 )
    UpdateConfigScreenInput(hWnd, &Rect, Point, bMouseClicked, &nPrevHotId);
  else if ( UpdateMainMenuInput(hWnd, &Rect, Point, bMouseClicked) )
    return; // window destroyed
  if ( g_nTitleMenuHotId_4B348C != nPrevHotId )
  {
    InvalidateRect(hWnd, &Rect, 0);
    UpdateWindow(hWnd);
  }
}

// Hot highlight cells on the config screen, all taken from g_pbmpDdBt.
static void PaintConfigHotButtons(HDC hDC, int nImgOffX, int nImgOffY)
{
  static const struct
  {
    uint32_t nHotId;
    int x, y, w, h;
    int srcX, srcYFromBottom;
  } s_buttons[] = {
    { 0, 590, 258, 28, 28, 120, 192 }, // close box
    { 1, 326, 50, 20, 20, 120, 164 },  // renderer <
    { 2, 382, 50, 20, 20, 140, 164 },  // renderer >
    { 5, 146, 97, 20, 20, 120, 164 },  // resolution <
    { 6, 202, 97, 20, 20, 140, 164 },  // resolution >
    { 7, 164, 264, 120, 24, 0, 168 },  // speed preset
    { 8, 304, 264, 120, 24, 0, 192 },  // standard preset
    { 9, 445, 264, 120, 24, 0, 216 },  // quality preset
  };
  for ( int i = 0; i < sizeof(s_buttons) / sizeof(s_buttons[0]); ++i )
  {
    if ( g_nTitleMenuHotId_4B348C == s_buttons[i].nHotId )
      BlitBmpPart(hDC, nImgOffX + s_buttons[i].x, nImgOffY + s_buttons[i].y, s_buttons[i].w,
        s_buttons[i].h, g_pbmpDdBt, s_buttons[i].srcX, s_buttons[i].srcYFromBottom);
  }
}

// "enabled" overlays for the six option toggles, all taken from g_pbmpDdBt.
static void PaintConfigToggleOverlays(HDC hDC, int nImgOffX, int nImgOffY)
{
  static const struct
  {
    BOOL *pbEnabled;
    int x, y;
    int srcYFromBottom;
  } s_overlays[] = {
    { &g_bAntiAliasEnable, 164, 136, 24 },
    { &g_bDitherEnable, 380, 136, 48 },
    { &g_bSpecularEnable_4B3474, 164, 176, 72 },
    { &g_bFogIsActive_4B3480, 380, 176, 96 },
    { &g_bLightEnable_4B3484, 164, 216, 120 },
    { &g_bShadowEnable_518CA8, 380, 216, 144 },
  };
  for ( int i = 0; i < sizeof(s_overlays) / sizeof(s_overlays[0]); ++i )
  {
    if ( *s_overlays[i].pbEnabled )
      BlitBmpPart(hDC, nImgOffX + s_overlays[i].x, nImgOffY + s_overlays[i].y, 183, 24,
        g_pbmpDdBt, 0, s_overlays[i].srcYFromBottom);
  }
}

// Device name, resolution string and the hovered control's help text.
static void PaintConfigInfoText(HWND hWnd, HDC hDC, int nImgOffX, int nImgOffY)
{
  CHAR String[512];
  int nTipX = nImgOffX + 180;

  HGDIOBJ hOldFont = SelectObject(hDC, g_hFontTitleMenu_518CCC);
  SetTextColor(hDC, 0);
  SetBkMode(hDC, TRANSPARENT);

  // name of the selected renderer, centered at (nImgOffX+364, nImgOffY+27)
  D3DX_DEVICEDESC *parrD3DXDevDesc = g_displayConfig_518C30.parrD3DXDevDesc;
  for ( int i = 0; i < g_displayConfig_518C30.nDeviceCount; ++i )
  {
    if ( g_nRenderHALMode_518CA0 == i )
    {
      int cchDriverDesc = lstrlenA(parrD3DXDevDesc->driverDesc);
      TextOutA(hDC, nImgOffX + 364 - 14 * (cchDriverDesc / 2) / 2, nImgOffY + 27,
        parrD3DXDevDesc->driverDesc, cchDriverDesc);
      break;
    }
    ++parrD3DXDevDesc;
  }

  // current resolution string (also clamps g_nResolutionMode_518C80)
  GetNthValidDimensionString(hWnd, &g_displayConfig_518C30, g_nRenderHALMode_518CA0,
    &g_nResolutionMode_518C80, String);
  TextOutA(hDC, nImgOffX + 121, nImgOffY + 76, String, lstrlenA(String));

  // help text for the hovered control
  if ( !g_nTitleMenuHotId_4B348C )
  {
    DrawTip(hDC, nTipX, nImgOffY + 350, "　このボタンを押すと環境設定画面から出ます。");
  }
  else if ( g_nTitleMenuHotId_4B348C == 1 || g_nTitleMenuHotId_4B348C == 2 )
  {
    DrawTip(hDC, nTipX, nImgOffY + 350, "　このボタンを押すことで動作モードを変えるができます。");
    DrawTip(hDC, nTipX, nImgOffY + 370, "　 Emulationモードはソフトウエアのみで動作させるので安定した動作");
    DrawTip(hDC, nTipX, nImgOffY + 390, "　を行いますが大変低速です。できるだけ他のモードで動かして下さい。");
  }
  else if ( g_nTitleMenuHotId_4B348C != 3 && g_nTitleMenuHotId_4B348C != 4 )
  {
    // 3/4 (z-buffer arrows) have no tip
    switch ( g_nTitleMenuHotId_4B348C )
    {
    case 5:
    case 6:
      DrawTip(hDC, nTipX, nImgOffY + 350, "　このボタンを押すことで画面の解像度を変えられます。");
      break;
    case 7:
      DrawTip(hDC, nTipX, nImgOffY + 350, "　環境設定スイッチが速度を最優先にした設定になります。");
      break;
    case 8:
      DrawTip(hDC, nTipX, nImgOffY + 350, "　環境設定スイッチが標準設定になります。");
      break;
    case 9:
      DrawTip(hDC, nTipX, nImgOffY + 350, "　環境設定スイッチが画質を最優先にした設定になります。");
      break;
    default:
      // hovered one of the six option toggles
      if ( !g_nTitleMenuHotToggleRow_4B3490 )
      {
        DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、ポリゴンの角を滑らかにします。");
      }
      else
      {
        switch ( g_nTitleMenuHotToggleRow_4B3490 )
        {
        case 1:
          DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、テクスチャーの色を滑らかにします。");
          DrawTip(hDC, nTipX, nImgOffY + 370, "＊　一部のグラフィックカードのドライバーで T&Lモードの時に、この");
          DrawTip(hDC, nTipX, nImgOffY + 390, "　スイッチをオンにすると物体の一部が変色してしまう場合があります。");
          break;
        case 2:
          DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、特定の物体に対して光沢を使用します。");
          DrawTip(hDC, nTipX, nImgOffY + 370, "＊　ライトスイッチがオンの時のみ、このスイッチは効果をあらわします。");
          break;
        case 3:
          DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、画面に霧の効果をつけます。");
          break;
        case 4:
          DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、光と陰の効果をつけます。");
          break;
        case 5:
          DrawTip(hDC, nTipX, nImgOffY + 350, "　このスイッチをオンにすると、影などの効果を描画します");
          break;
        }
      }
      break;
    }
  }

  SelectObject(hDC, hOldFont);
}

static void PaintConfigScreen(HWND hWnd, HDC hDC, const RECT *pRect)
{
  int nImgOffX = 0;
  int nImgOffY = 0;
  if ( g_pbmpDd2 )
  {
    nImgOffX = (pRect->right - g_pbmpDd2->pBitmapInfoHeader->biWidth) >> 1;
    nImgOffY = (pRect->bottom - g_pbmpDd2->pBitmapInfoHeader->biHeight) >> 1;
    BlitBmp(hDC, nImgOffX, nImgOffY, g_pbmpDd2);
  }
  if ( g_pbmpDdBt )
  {
    PaintConfigHotButtons(hDC, nImgOffX, nImgOffY);
    PaintConfigToggleOverlays(hDC, nImgOffX, nImgOffY);
  }
  PaintConfigInfoText(hWnd, hDC, nImgOffX, nImgOffY);
}

static void PaintMainMenu(HDC hDC, const RECT *pRect)
{
  int nImgOffX = (pRect->right - g_pbmpDd->pBitmapInfoHeader->biWidth) >> 1;
  int nImgOffY = (pRect->bottom - g_pbmpDd->pBitmapInfoHeader->biHeight) >> 1;
  // highlighted menu entry cells, taken from g_pbmpDdBt3
  static const struct
  {
    uint32_t nHotId;
    int y;
    int srcYFromBottom;
  } s_buttons[] = {
    { 0, 104, 32 }, // start
    { 2, 264, 64 }, // config
    { 1, 424, 96 }, // quit
  };

  BlitBmp(hDC, nImgOffX, nImgOffY, g_pbmpDd);
  if ( !g_pbmpDdBt3 )
    return;
  for ( int i = 0; i < sizeof(s_buttons) / sizeof(s_buttons[0]); ++i )
  {
    if ( g_nTitleMenuHotId_4B348C == s_buttons[i].nHotId )
      BlitBmpPart(hDC, nImgOffX + 469, nImgOffY + s_buttons[i].y, 128, 32, g_pbmpDdBt3, 0,
        s_buttons[i].srcYFromBottom);
  }
}

// WM_PAINT: renders into a compatible DC, then blits it to the window.
static int TitleMenu_OnPaint(HWND hWnd)
{
  PAINTSTRUCT Paint;
  RECT Rect;
  HBRUSH hbr;

  GetClientRect(hWnd, &Rect);
  HDC hdc = BeginPaint(hWnd, &Paint);
  HDC hDC = CreateCompatibleDC(hdc);
  HBITMAP hbmPaint = CreateCompatibleBitmap(hdc, Rect.right - Rect.left, Rect.bottom - Rect.top);
  HGDIOBJ hOldBitmap = SelectObject(hDC, hbmPaint);
  hbr = CreateSolidBrush(0xFFFFFFu);
  FillRect(hDC, &Rect, hbr);
  DeleteObject(hbr);

  if ( g_bTitleMenuConfigScreen_518CC4 )
    PaintConfigScreen(hWnd, hDC, &Rect);
  else if ( g_pbmpDd )
    PaintMainMenu(hDC, &Rect);

  BitBlt(hdc, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, hDC, 0, 0,
    SRCCOPY);
  EndPaint(hWnd, &Paint);
  SelectObject(hDC, hOldBitmap);
  DeleteObject(hbmPaint);
  DeleteObject(hDC);
  ReleaseDC(hWnd, hdc);
  return 0;
}

// WM_CREATE: loads the menu bitmaps and picks the default renderer.
static int TitleMenu_OnCreate(HWND hWnd)
{
  CHAR String[512];
  const struct
  {
    LPCSTR pszFile;
    CBMPFile **ppBmp;
  } bitmaps[4] = {
    { "DD.bmp", &g_pbmpDd },
    { "DD_BT.bmp", &g_pbmpDdBt },
    { "DD2.bmp", &g_pbmpDd2 },
    { "DD_BT3.bmp", &g_pbmpDdBt3 },
  };

  g_hFontTitleMenu_518CCC = CreateFontA(14, 7, 0, 0, 400, 0, 0, 0, 0x80u, 0, 0, 2u, 0x30u,
    "ＭＳ 明朝");
  SetTimer(hWnd, 0x113u, 0x32u, 0);
  for ( int i = 0; i < 4; ++i )
  {
    wsprintfA(String, bitmaps[i].pszFile);
    CHanaObj__GetRealPath_486D9A(String, 1007);
    *bitmaps[i].ppBmp = LoadBmp_420DF0(String);
  }
  UpdateRenderOptionsCheckboxes(hWnd);
  PickDefaultRenderHALMode();
  return 0;
}

// WM_DESTROY
static int TitleMenu_OnDestroy(HWND)
{
  FreeTitleMenuBitmaps();
  Sleep(0x64u);
  PostQuitMessage(0);
  return 1;
}

// WndProcTitleMenu_428C11 at 0x428C11
// Window procedure for the title menu - handles main menu and config screen
LRESULT __stdcall hook_WndProcTitleMenu_428C11(HWND hWnd, unsigned int Msg, WPARAM wParam,
  LPARAM lParam)
{
  DebugLog("[HNH:SUB] hook_WndProcTitleMenu_428C11\n");

  g_hWnd = hWnd;
  if ( Msg > WM_PAINT ) // decompiler artifact of the original switch: WM_CLOSE / WM_TIMER
  {
    if ( Msg == WM_CLOSE )
    {
      FreeTitleMenuBitmaps();
      DestroyWindow(hWnd);
    }
    else if ( Msg == WM_TIMER )
    {
      TitleMenu_OnTimer(hWnd);
    }
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }

  switch ( Msg )
  {
  case WM_PAINT:
    return TitleMenu_OnPaint(hWnd);
  case WM_CREATE:
    return TitleMenu_OnCreate(hWnd);
  case WM_DESTROY:
    return TitleMenu_OnDestroy(hWnd);
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