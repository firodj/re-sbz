#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"


// === PINNED GAME FUNCTIONS ===
int  (__cdecl*MenuCharSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47A324);
bool (__cdecl*IsMenuKeyPressed_481170)(void) = reinterpret_cast<bool(__cdecl*)()>(0x481170);
HRESULT (__cdecl*Surface_SetColorKeyRGB)(LPDIRECTDRAWSURFACE7 surface, COLORREF color) =
    reinterpret_cast<HRESULT(__cdecl*)(LPDIRECTDRAWSURFACE7, COLORREF)>(0x4198E0);
int  (__cdecl*PlayAssault_4703AE)(int iModeId, int iMenuTab) =
    reinterpret_cast<int(__cdecl*)(int, int)>(0x4703AE);
BOOL (__cdecl * CheckSpecialJpChar_43BFFA)(const char *szText) = reinterpret_cast<BOOL(__cdecl*)(const char *szText)>(0x43BFFA);
void (__cdecl * DrawText_43BCAE)(LPDIRECTDRAWSURFACE7 pSurface, char *szText, int left, int top, int right, int bottom) = reinterpret_cast<void(__cdecl*)(LPDIRECTDRAWSURFACE7 pSurface, char *szText, int left, int top, int right, int bottom)>(0x43BCAE);

int (__cdecl*MenuGallery)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite)
  = reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47AD8D);

int (__cdecl*MenuMusicSelect)(LPDIRECTSOUNDBUFFER *ppDSndBuf, CHanaSpriteT3F6_S1FC *pMouseSprite)
  = reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47D163);

// === GLOBAL VARIABLES ===

LPDIRECTDRAWSURFACE7 &g_lpDDSurface_519CF8 = *(LPDIRECTDRAWSURFACE7 *)0x519CF8; // WRITABLE
char (&g_tempText_519D00)[50] = *(char (*)[50])0x519D00; // WRITABLE
char (&g_arrGalleryUnlocked_5301A2)[150] = *(char(*)[150])0x5301A2;  // WRITABLE

String50 (&g_ascSndFileNames_4C78A0)[200] = *(String50(*)[200])0x4C78A0;  // READ-ONLY

// === FUNCTIONS ===

// MenuCharSelect_47A324 - MainMenu state -3: character select over ss00.bmp
// (1024x768 native). Five portraits ss01..ss05 whose load sizes are exactly
// the on-disk BMP dimensions (244x680, 208x624, 264x536, 232x720, 264x680),
// color-keyed on black. Hover hitboxes live in 1024x768 design space, scaled
// by 0.625 then ConvertFrom640; portrait 1 (ss02) is not hit-testable while
// g_sceneProgressFlag_530269 == 1 (locked until the backdrop was seen).
// Clicking a portrait: "On" effect, BGM stop, then run that character's
// assault pair via PlayAssault_4703AE -- modes 2*selectOrder[ch] and +1
// (selectOrder = {2,4,0,3,1} maps portraits to assault indices); either
// returning 1 exits with nResult=1, else the BGM resumes.
// Top bar: four 128-wide hot columns below y=48 (column 3 right-click only)
// give nResult -1..-4 with the "click" effect; -1/-2/-4 exit to MainMenu
// (-4 exits the whole extra menu, since MainMenu only loops on -1/-2/-3);
// -3 falls through and is effectively a no-op. The menu key opens
// SystemMenu_4655AB in mode 2; >= 1 there exits.
//
// Single xref: MainMenu_47DF0D (state -3), which passes the BGM by pointer.

int __cdecl hook_MenuCharSelect(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
    CHAR szFileName[256];                     // [esp+294h] [ebp-180h]
    LPDIRECTDRAWSURFACE7 arrCharSurfaces[10]; // [esp+268h] [ebp-1ACh]
    LPDIRECTDRAWSURFACE7 pBackgroundSurface;  // [esp+290h] [ebp-184h]
    CSceneState sceneScratch;                 // [ebp-51Ch] SystemMenu arg copy
    RECT rc;                                  // [esp+B4h] [ebp-360h]
    DWORD dwAntiAliasCur;
    char szCharNames[5][25] = { "ss01", "ss02", "ss03", "ss04", "ss05" };
    WORD selectOrder[5] = { 2, 4, 0, 3, 1 };
    // {x, y} pairs in 1024x768 design space (the original interleaves them as
    // DWORD[19] with x in the odd slot, y in the even one):
    const int arrLoadSize[5][2]  = { {244, 680}, {208, 624}, {264, 536}, {232, 720}, {264, 680} }; // == BMP dims
    const int arrPos[5][2]       = { {244, 680}, {208, 622}, {264, 534}, {232, 713}, {264, 678} };
    const int arrHitOffset[5][2] = { { 83,  88}, {248, 146}, {327, 234}, {527,  55}, {687,  90} };
    int nResult, nHoverChar, nChosenChar, i;
    int xmouse, ymouse;
    WORD nMouseBtn = 0, nPrevMouseBtn;

    DebugLog("[HNH:SUB] MenuCharSelect_47A324\n");

    nResult = 0;
    nHoverChar = 0;
    pBackgroundSurface = NULL;
    for (i = 0; i < 9; ++i)                   // original clears 9 of the 10 slots
        arrCharSurfaces[i] = NULL;

    // load the five portraits (scaled, black color key)
    for (i = 0; i < 5; ++i)
    {
        wsprintfA(szFileName, "%s.bmp", szCharNames[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1007);       // 1007 = GRP dir
        arrCharSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
                (int)ConvertFrom1024AsFloat((float)arrLoadSize[i][0]),
                (int)ConvertFrom1024AsFloat((float)arrLoadSize[i][1]));
        Surface_SetColorKeyRGB(arrCharSurfaces[i], 0);
    }

    // ss00 background fills the screen
    wsprintfA(szFileName, "ss00.bmp");                        // aSs00Bmp @0x4C6D04
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    pBackgroundSurface = LoadBmpScaled_4203B3(szFileName, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));

    g_sceneState_52F7E0.bActiveSceneStep = 0;
    StopAndRestartThread(2);

    for (;;)
    {
        DoPeekMessages();

        // menu key -> SystemMenu (mode 2)
        if (IsMenuKeyPressed_481170())
        {
            StopAndRestartThread(0);
            memcpy(&sceneScratch, &g_sceneState_52F7E0, sizeof(sceneScratch));
            nResult = SystemMenu_4655AB(*ppBgmBuffer, -1000, 2, 666.0f, 666.0f,
                                         sceneScratch);
            StopAndRestartThread(2);
            if (nResult >= 1)
                break;
        }

        nPrevMouseBtn = nMouseBtn;
        GetMouseBtn_45A41B(&nMouseBtn);
        GetMouseXY_45A4D8((DWORD *)&xmouse, (DWORD *)&ymouse);

        // hover hit-test: [offset, pos+offset] scaled boxes; portrait 1 locked
        nHoverChar = -1;
        for (i = 0; i < 5; ++i)
        {
            if (i == 1 && g_sceneProgressFlag_530269 != 1)
                continue;
            if ((int)ConvertFrom640AsFloat(arrHitOffset[i][1] * 0.625f) < ymouse
                && ymouse < (int)ConvertFrom640AsFloat((arrPos[i][1] + arrHitOffset[i][1]) * 0.625f)
                && (int)ConvertFrom640AsFloat(arrHitOffset[i][0] * 0.625f) < xmouse
                && xmouse < (int)ConvertFrom640AsFloat((arrPos[i][0] + arrHitOffset[i][0]) * 0.625f))
            {
                nHoverChar = i;
                break;
            }
        }

        // click a portrait -> run the character's assault pair
        if (nHoverChar != -1 && nMouseBtn != nPrevMouseBtn && nMouseBtn == 1)
        {
            nChosenChar = nHoverChar;
            StopAndRestartThread(0);
            if (g_arrUIEffectDSBuffer_52F168[2])              // "On" click (0x52F170)
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[2], 0);
            if (*ppBgmBuffer)
                (*ppBgmBuffer)->Stop();
            if (PlayAssault_4703AE(2 * selectOrder[nChosenChar], 1) == 1)
            {
                nResult = 1;
                break;
            }
            if (PlayAssault_4703AE(2 * selectOrder[nChosenChar] + 1, 1) == 1)
            {
                nResult = 1;
                break;
            }
            // neither assault exited the game: resume BGM
            if (*ppBgmBuffer)
                DSBufferPlay(*ppBgmBuffer, 0);
            StopAndRestartThread(2);
        }

        // top-bar hot columns (any click; column 3 right-click only)
        nResult = 0;
        if (nMouseBtn && nMouseBtn != nPrevMouseBtn)
        {
            for (i = 0; i < 4; ++i)
            {
                if (i != 3 || nMouseBtn != 2)
                {
                    if (xmouse < (int)ConvertFrom1024AsFloat(128.0f) * i)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(128.0f) * (i + 1) <= xmouse)
                        continue;
                    if ((int)ConvertFrom1024AsFloat(48.0f) <= ymouse)
                        continue;
                }
                nResult = -(i + 1);
                if (g_arrUIEffectDSBuffer_52F168[3])          // "click" effect (0x52F174)
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
                break;
            }
        }
        if (nResult < 0 && nResult != -3)                     // -3 is a no-op
            break;

        // render: background + hovered portrait (color-keyed) + cursor
        if (pBackgroundSurface)
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, pBackgroundSurface, NULL, DDBLT_WAIT, NULL);
            if (nHoverChar >= 0 && arrCharSurfaces[nHoverChar])
            {
                SetRect(&rc, 0, 0,
                        (int)ConvertFrom1024AsFloat((float)arrPos[nHoverChar][0]),
                        (int)ConvertFrom1024AsFloat((float)arrPos[nHoverChar][1]));
                g_pBackBuffer_4D6C90->BltFast(
                        (int)ConvertFrom1024AsFloat((float)arrHitOffset[nHoverChar][0]),
                        (int)ConvertFrom1024AsFloat((float)arrHitOffset[nHoverChar][1]),
                        arrCharSurfaces[nHoverChar], &rc,
                        DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
            }
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasCur);
        }
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
            if (pMouseSprite)
            {
                CHanaSpriteT3F6__SetPos_414A5D(pMouseSprite, (float)(xmouse + 16), (float)(ymouse + 16));
                DrawSprite2D(pMouseSprite);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        // nop_4863C3();                                       // disabled, as in InGameMenu.cpp
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    // teardown
    if (pBackgroundSurface)
    {
        pBackgroundSurface->Release();
        pBackgroundSurface = NULL;
    }
    for (i = 0; i < 5; ++i)
    {
        if (arrCharSurfaces[i])
        {
            arrCharSurfaces[i]->Release();
            arrCharSurfaces[i] = NULL;
        }
    }
    StopAndRestartThread(0);
    return nResult;
}

void __cdecl hook_Surface_SetColorKeyRGB(LPDIRECTDRAWSURFACE7 surface, COLORREF color)
{
  DWORD colorKey; // [esp+0h] [ebp-8h] BYREF
  DWORD surfaceColor; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] Surface_SetColorKeyRGB_4198E0\n");

  surfaceColor = RGBToSurfaceFormat(surface, color);
  colorKey = surfaceColor;
  surface->SetColorKey(8, (LPDDCOLORKEY)&colorKey);
}

BOOL __cdecl hook_CheckSpecialJpChar_43BFFA(const char *szText)
{
  // Ensure valid pointer and minimum 2 bytes available
  if (!szText || szText[0] == '\0' || szText[1] == '\0')
    return FALSE;

  ONCELOG("[HNH:SUB] CheckSpecialJpChar_43BFFA\n");

  // Combine lead and trail bytes into a 16-bit big-endian value matching Shift-JIS bytes
  // szText[0] = lead byte (e.g., 0x81), szText[1] = trail byte
  unsigned short ch = ((unsigned char)szText[0] << 8) | (unsigned char)szText[1];

  switch (ch)
  {
  case 0x8149: // '！' (Full-width Exclamation Mark)
  case 0x8142: // '。' (Ideographic Period)
  case 0x8141: // '、' (Ideographic Comma)
  case 0x8143: // '，' (Full-width Comma)
  case 0x8144: // '．' (Full-width Period)
  case 0x8176: // '』' (Right White Corner Bracket)
  case 0x8178: // '】' (Right Black Tortoise Shell Bracket)
    return TRUE;
  default:
    return FALSE;
  }
}

void __cdecl hook_DrawText_43BCAE(LPDIRECTDRAWSURFACE7 pSurface, char *szText, int left, int top, int right, int bottom)
{
  int y; // [esp+0h] [ebp-14h]
  int p; // [esp+4h] [ebp-10h]
  HDC hdc; // [esp+8h] [ebp-Ch] BYREF
  HGDIOBJ h; // [esp+Ch] [ebp-8h]
  int x; // [esp+10h] [ebp-4h]

  ONCELOG("[HNH:SUB] DrawText_43BCAE\n");

  x = 0;
  p = 0;
  y = 0;
  if ( g_lpDDSurface_519CF8 )
  {
    g_lpDDSurface_519CF8->Release();
    g_lpDDSurface_519CF8 = 0;
  }
  g_lpDDSurface_519CF8 = CreateSurfaceWH(right - left, bottom - top);
  Surface_SetColorKeyRGB(g_lpDDSurface_519CF8, 0);
  BltSurface(g_lpDDSurface_519CF8, 0, 0, right - left, bottom - top, 0);
  memcpy(&g_tempText_519D00, szText, sizeof(g_tempText_519D00));
  g_lpDDSurface_519CF8->GetDC(&hdc);
  h = SelectObject(hdc, g_hFont);
  SetTextColor(hdc, 0xFFFFFFu);
  SetBkMode(hdc, 1);
  do
  {
    x = 0;
    do
    {
      if ( x >= right - left - 20 )
        break;
      if ( szText[p] == ';' || szText[p] == ',' || szText[p] >= 'A' && szText[p] <= 'z' )
      {
        ++p;                                    // skip halfwidth
      }
      else
      {
        TextOutA(hdc, x, y, &szText[p], 2);
        TextOutA(hdc, x + 1, y, &szText[p], 2);
        x += 26;
        p += 2;                                 // only accept FullWidth shift-jis
      }
    }
    while ( CheckSpecialJpChar_43BFFA(&szText[p]) || szText[p] && szText[p] != '\r' && szText[p] != '\n' );
    y += 26;
  }
  while ( szText[p] && szText[p] != '\r' && szText[p] != '\n' && (szText[p] < 'A' || szText[p] > 'z') && y <= bottom );
  SelectObject(hdc, h);
  g_lpDDSurface_519CF8->ReleaseDC(hdc);
  if ( g_lpDDSurface_519CF8 )
  {
    BltFastWH(pSurface, left, top + 2, g_lpDDSurface_519CF8, 0, 0, right - left, bottom - top, 1);
    g_lpDDSurface_519CF8->Release();
    g_lpDDSurface_519CF8 = 0;
  }
}

uint32_t &g_keyC = *(uint32_t *)0x4C7350; // WRITABLE
uint8_t &g_btoggleC = *(uint8_t *)0x530268; // WRITABLE

// Keyboard poll for the scene/menu layer. Returns true when ESC (0x1B) is down 
// (bEsc, via GetAsyncKeyState after Sleep(0)). 
// Also tracks the 'C' key (0x43): first press latches dword_4C7350=1 and 
// toggles the global byte_530268; release clears the latch.
bool hook_IsMenuKeyPressed_481170()
{
  bool bEsc; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] IsMenuKeyPressed_481170\n");

  Sleep(0);
  bEsc = (unsigned __int16)GetAsyncKeyState(VK_ESCAPE) >> 8 != 0;
  if ( (unsigned __int16)GetAsyncKeyState('C') >> 8 )
  {
    if ( !g_keyC )
    {
      g_btoggleC = g_btoggleC == 0;
      g_keyC = 1;
    }
  }
  else
  {
    g_keyC = 0;
  }
  return bEsc;
}


// MenuGallery_47AD8D -- MainMenu state -2: the CG gallery over gl01/gl02/
// gl03.bmp tab backgrounds (one per tab, full screen). Per tab a 6x4 cell
// grid: cell (col,row) maps to name-table entry 24*nTab + col + 6*row; cell
// x spans Conv640((160*col+37)*0.625)..(160*col+187)*0.625, y likewise
// (122*row+97)..(122*row+210) -- 640-space boxes pre-scaled by 0.625.
//
// The name table holds 72 slots (3 tabs x 24, 20 bytes each); 53 pictures
// are named, in order:
//   G20-G29, G80-G90, G60-G70, G40-G42, G44-G50 (no G43),
//   G00-G03, G05-G10 (no G04), G95
// (the original emits ~230 lines of per-row strcpy/memset plus dead zero
// rows sourced from runtime-zero .data bytes -- collapsed into initializer).
//
// Unlock state: g_arrGalleryUnlocked_5301A2[atoi(name+1)] (char[150] over the
// 'G' number; written by ParseExecStr -- the script engine unlocks CGs).
// Locked cells draw the "glno" placeholder (no color key); only unlocked
// cells are hit-testable, and only entries < 54 open a preview.
//
// Preview: load via GRP dir full screen, PlayScreenFade in, PRIMARY-buffer
// BltFast, pump the mouse until any button, fade out, release. Nav strip
// "glb" (256x48, two 128-wide button frames) at y 393..423 (640-space,
// NOT pre-scaled), x bands (384+128i)..: prev/next clamp nTab to 0..2.
// Thumbnails: "glt" hover highlight (152x120, color-keyed) and "glno"
// (152x120); all sizes via ConvertFrom1024.
// Top bar as usual (-1..-4); exit only on nResult < 0 && != -2 (-2 would
// re-enter this screen). Menu key -> SystemMenu mode 2, >= 1 exits.

int __cdecl hook_MenuGallery(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
  LPDIRECTDRAWSURFACE7 bgSurfaces[9];    // [ebp-98Ch] 3 used (per tab)
  LPDIRECTDRAWSURFACE7 thumbSurfaces[9];  // [ebp-964h] [2] = nav strip "glb"
  LPDIRECTDRAWSURFACE7 pPreviewSurface;   // [ebp-B88h]
  CHAR szFileName[256];                   // [ebp-93Ch]
  CSceneState sceneScratch;               // [ebp-DA4h] SystemMenu arg copy
  DWORD dwAntiAliasCur;                   // [ebp-BC8h]
  char gpNameTable[72][20];               // [ebp-824h] 53 named slots
  int nResult, nHoverPic, nHoverNav, nTab, i, j;
  int xMouse, yMouse;                     // [ebp-828h] / [ebp-82Ch]
  WORD nMouseBtn = 0, nPrevMouseBtn;          // [ebp-4h] / [ebp-83Ch]

  DebugLog("[HNH:SUB] MenuGallery_47AD8D\n");

  static const char *const arrPicNames[53] = {
    "G20.GP","G21.GP","G22.GP","G23.GP","G24.GP","G25.GP","G26.GP","G27.GP","G28.GP","G29.GP",
    "G80.GP","G81.GP","G82.GP","G83.GP","G84.GP","G85.GP","G86.GP","G87.GP","G88.GP","G89.GP",
    "G90.GP","G60.GP","G61.GP","G62.GP","G63.GP","G64.GP","G65.GP","G66.GP","G67.GP","G68.GP",
    "G69.GP","G70.GP","G40.GP","G41.GP","G42.GP","G44.GP","G45.GP","G46.GP","G47.GP","G48.GP",
    "G49.GP","G50.GP","G00.GP","G01.GP","G02.GP","G03.GP","G05.GP","G06.GP","G07.GP","G08.GP",
    "G09.GP","G10.GP","G95.GP",
  };
  static const char arrBgNames[3][25]    = { "gl01", "gl02", "gl03" };
  static const char arrThumbNames[3][25] = { "glt", "glno", "glb" };
  static const int  arrThumbSize[3][2]   = { {152, 120}, {152, 120}, {256, 48} };

  nResult = 0;
  pPreviewSurface = NULL;
  nTab = 0;
  memset(gpNameTable, 0, sizeof(gpNameTable));
  for (i = 0; i < 53; ++i)
    strcpy(gpNameTable[i], arrPicNames[i]);
  for (i = 0; i < 9; ++i)                 // original clears 9 of the slots
  {
    thumbSurfaces[i] = NULL;
    bgSurfaces[i] = NULL;
  }

  for (i = 0; i < 3; ++i)
  {
    wsprintfA(szFileName, "%s.bmp", arrBgNames[i]);
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);        // 1007 = GRP dir
    bgSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
      ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    wsprintfA(szFileName, "%s.bmp", arrThumbNames[i]);
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    thumbSurfaces[i] = LoadBmpScaled_4203B3(szFileName,
      (int)ConvertFrom1024AsFloat((float)arrThumbSize[i][0]),
      (int)ConvertFrom1024AsFloat((float)arrThumbSize[i][1]));
    Surface_SetColorKeyRGB(thumbSurfaces[i], 0);
  }

  g_sceneState_52F7E0.bActiveSceneStep = 0;
  StopAndRestartThread(2);

  for (;;)
  {
    DoPeekMessages();

    // menu key -> SystemMenu (mode 2)
    if (IsMenuKeyPressed_481170())
    {
      StopAndRestartThread(0);
      memcpy(&sceneScratch, &g_sceneState_52F7E0, sizeof(sceneScratch));
      nResult = SystemMenu_4655AB(*ppBgmBuffer, -1000, 2, 666.0f, 666.0f,
        sceneScratch);
      StopAndRestartThread(2);
      if (nResult >= 1)
        break;
    }

    nPrevMouseBtn = nMouseBtn;
    GetMouseBtn_45A41B(&nMouseBtn);
    GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

    // grid hit-test: only unlocked cells answer
    nHoverPic = -1;
    for (j = 0; j < 4; ++j)
    {
      for (i = 0; i < 6; ++i)
      {
        if (g_arrGalleryUnlocked_5301A2[atoi(&gpNameTable[24 * nTab + 6 * j + i][1])]
          && (int)ConvertFrom640AsFloat((float)(122 * j + 97) * 0.625f) < yMouse
          && yMouse < (int)ConvertFrom640AsFloat((float)(122 * j + 210) * 0.625f)
          && (int)ConvertFrom640AsFloat((float)(160 * i + 37) * 0.625f) < xMouse
          && xMouse < (int)ConvertFrom640AsFloat((float)(160 * i + 187) * 0.625f))
        {
          nHoverPic = 24 * nTab + i + 6 * j;
          break;                   // inner loop only, as the original
        }
      }
    }

    // click a picture -> full-screen preview
    if (nHoverPic < 54 && nHoverPic != -1 && nMouseBtn != nPrevMouseBtn && nMouseBtn == 1)
    {
      if (g_arrUIEffectDSBuffer_52F168[3])               // "click"
        DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      wsprintfA(szFileName, "%s", gpNameTable[nHoverPic]);
      CHanaObj__GetRealPath_486D9A(szFileName, 1007);
      pPreviewSurface = LoadBmpScaled_4203B3(szFileName,
        ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
      if (pPreviewSurface)
      {
        PlayScreenFade(1, 0xFFFF7FFF, pPreviewSurface, 25.0f, 0);
        g_pPrimaryBuffer_4D6C88->BltFast(0, 0, pPreviewSurface, NULL, DDBLTFAST_WAIT);
        do
        {
          DoPeekMessages();
          GetMouseBtn_45A41B(&nMouseBtn);
        } while (!nMouseBtn);
        PlayScreenFade(0, 0xFFFF7FFF, pPreviewSurface, 25.0f, 0);
        nPrevMouseBtn = nMouseBtn;   // consume the preview-dismiss click
      }
      if (pPreviewSurface)
      {
        pPreviewSurface->Release();
        pPreviewSurface = NULL;
      }
    }

    // nav buttons: prev / next (clamped tab switch)
    nHoverNav = -1;
    for (i = 0; i < 2; ++i)
    {
      if ((int)ConvertFrom640AsFloat(393.125f) < yMouse
        && yMouse < (int)ConvertFrom640AsFloat(423.125f)
        && (int)ConvertFrom640AsFloat((float)((i << 7) + 384) * 0.625f) < xMouse
        && xMouse < (int)ConvertFrom640AsFloat((float)(((i + 1) << 7) + 384) * 0.625f))
      {
        nHoverNav = i;
        if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
        {
          if (i == 1)
          {
            if (++nTab > 2)
              nTab = 2;
          }
          else
          {
            if (--nTab < 0)
              nTab = 0;
          }
          if (g_arrUIEffectDSBuffer_52F168[3])
            DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
        }
      }
    }

    // top-bar hot columns (any click; column 3 right-click only)
    nResult = 0;
    if (nMouseBtn && nMouseBtn != nPrevMouseBtn)
    {
      for (i = 0; i < 4; ++i)
      {
        if (i != 3 || nMouseBtn != 2)
        {
          if (xMouse < (int)ConvertFrom1024AsFloat(128.0f) * i)
            continue;
          if ((int)ConvertFrom1024AsFloat(128.0f) * (i + 1) <= xMouse)
            continue;
          if ((int)ConvertFrom1024AsFloat(48.0f) <= yMouse)
            continue;
        }
        nResult = -(i + 1);
        if (g_arrUIEffectDSBuffer_52F168[3])
          DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
        break;
      }
    }
    if (nResult < 0 && nResult != -2)    // -2 would re-enter this screen
      break;

    // (nav geometry is re-initialized every frame in the original)
    RECT arrNavBtnRect[2] = {
      { 0, 0, (int)ConvertFrom640AsFloat(80.0f),  (int)ConvertFrom640AsFloat(30.0f) },
      { (int)ConvertFrom640AsFloat(80.0f), 0, (int)ConvertFrom640AsFloat(160.0f), (int)ConvertFrom640AsFloat(30.0f) },
    };
    POINT arrNavPos[2] = {
      { (int)ConvertFrom640AsFloat(240.0f), (int)ConvertFrom640AsFloat(393.125f) },
      { (int)ConvertFrom640AsFloat(320.0f), (int)ConvertFrom640AsFloat(393.125f) },
    };

    if (bgSurfaces[nTab])
    {
      g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pBackBuffer_4D6C90->Blt(NULL, bgSurfaces[nTab], NULL, DDBLT_WAIT, NULL);

      if (nHoverNav >= 0 && thumbSurfaces[2])            // "glb" strip
        g_pBackBuffer_4D6C90->BltFast(arrNavPos[nHoverNav].x, arrNavPos[nHoverNav].y,
          thumbSurfaces[2], &arrNavBtnRect[nHoverNav], DDBLTFAST_WAIT);

      // locked cells: "glno" placeholder (no color key)
      for (i = 0; i < 24; ++i)
      {
        if (gpNameTable[24 * nTab + i][0]
          && !g_arrGalleryUnlocked_5301A2[atoi(&gpNameTable[24 * nTab + i][1])])
          g_pBackBuffer_4D6C90->BltFast(
            (int)ConvertFrom640AsFloat((float)(160 * (i % 6) + 37) * 0.625f),
            (int)ConvertFrom640AsFloat((float)(122 * (i / 6) + 97) * 0.625f),
            thumbSurfaces[1], NULL, DDBLTFAST_WAIT);
      }
      // hovered cell: "glt" highlight (color-keyed)
      if (nHoverPic >= 0 && thumbSurfaces[0] && gpNameTable[nHoverPic][0])
        g_pBackBuffer_4D6C90->BltFast(
          (int)ConvertFrom640AsFloat((float)(160 * (nHoverPic % 24 % 6) + 37) * 0.625f),
          (int)ConvertFrom640AsFloat((float)(122 * (nHoverPic % 24 / 6) + 97) * 0.625f),
          thumbSurfaces[0], NULL, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasCur);
    }
    if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
      if (pMouseSprite)
      {
        CHanaSpriteT3F6__SetPos_414A5D(pMouseSprite, (float)(xMouse + 16), (float)(yMouse + 16));
        DrawSprite2D(pMouseSprite);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }
    // nop_4863C3();                                       // disabled, as elsewhere
    PresentFrame();
    HanaVert_DrawBatch(0);
  }

  // teardown
  for (i = 0; i < 5; ++i)                  // original releases 5 slots
  {
    if (bgSurfaces[i])
    {
      bgSurfaces[i]->Release();
      bgSurfaces[i] = NULL;
    }
    if (thumbSurfaces[i])
    {
      thumbSurfaces[i]->Release();
      thumbSurfaces[i] = NULL;
    }
  }
  StopAndRestartThread(0);
  return nResult;
}


// MenuMusicSelect_47D163 -- MainMenu state -1: the music/sound selection
// screen over mu.bmp. Three sprites (all GRP dir, 1007): mu.bmp background
// (full screen), mub.bmp button-highlight strip (400x72 = four ~200x36
// frames), mub2.bmp indicator sheet (832x200 = 2x7 grid of 413x28 label
// cells, one per sound).
//
// 14 sounds (g_ascSndFileNames_4C78A0[1..14], String50[200] table) in a 7x2
// grid: cell i hits at x = Conv1024((i-1)/7*468 + 72) (+413) and
// y = Conv1024((i-1)%7*36 + 217) (+27); clicking selects it (nSoundIndex
// starts at 4). Buttons: 0 stop (288..486 x 612..648), 1 play (522..720 x
// 612..648 -- loads the wav through the WAV dir (1006) and plays it LOOPING
// at the volume-table level), 2 prev (341..496 x 666..702, floor 1), 3 next
// (512..667 x 666..702, cap 14). Every click plays the "click" effect
// (arrUIEffect[3], a.k.a. g_pDSBuf_52F174).
// Top bar: the same four 128-wide columns as MenuCharSelect (-1..-4), but
// only nResult < -1 exits here (-1 would re-enter this very screen). The
// menu key opens SystemMenu mode 2; >= 1 from there exits with that value.

int __cdecl hook_MenuMusicSelect(LPDIRECTSOUNDBUFFER *ppBgmBuffer, CHanaSpriteT3F6_S1FC *pMouseSprite)
{
  LPDIRECTDRAWSURFACE7 pBmpIndicator;     // [esp+190h] [ebp-124h]
  LPDIRECTDRAWSURFACE7 pBmpBackground;    // [esp+194h] [ebp-120h]
  LPDIRECTDRAWSURFACE7 pBmpHighlight;     // [esp+198h] [ebp-11Ch]
  CHAR szFileName[256];                   // [esp+19Ch] [ebp-118h]
  CSceneState sceneScratch;               // [ebp-394h] SystemMenu arg copy
  RECT rc;                                // [ebp-18Ch] indicator source cell
  RECT arrHighlightRects[5];              // [ebp-17Ch] mub.bmp frames
  DWORD dwAntiAliasCur;                   // [ebp-1B8h]
  int nResult, nHoverCtrl, nSoundIndex, i;
  int xMouse, yMouse;                     // [ebp-8h] / [ebp-Ch]
  WORD nMouseBtn = 0, nPrevMouseBtn;          // [ebp-4h] / [ebp-18h]

  DebugLog("[HNH:SUB] MenuMusicSelect_47D163\n");

  nResult = 0;
  nSoundIndex = 4;

  wsprintfA(szFileName, "mu.bmp");
  CHanaObj__GetRealPath_486D9A(szFileName, 1007);            // 1007 = GRP dir
  pBmpBackground = LoadBmpScaled_4203B3(szFileName,
    ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
  wsprintfA(szFileName, "mub.bmp");
  CHanaObj__GetRealPath_486D9A(szFileName, 1007);
  pBmpHighlight = LoadBmpScaled_4203B3(szFileName,
    (int)ConvertFrom1024AsFloat(400.0f), (int)ConvertFrom1024AsFloat(72.0f));
  wsprintfA(szFileName, "mub2.bmp");
  CHanaObj__GetRealPath_486D9A(szFileName, 1007);
  pBmpIndicator = LoadBmpScaled_4203B3(szFileName,
    (int)ConvertFrom1024AsFloat(832.0f), (int)ConvertFrom1024AsFloat(200.0f));

  g_sceneState_52F7E0.bActiveSceneStep = 0;
  StopAndRestartThread(2);

  for (;;)
  {
    DoPeekMessages();

    // menu key -> SystemMenu (mode 2)
    if (IsMenuKeyPressed_481170())
    {
      StopAndRestartThread(0);
      memcpy(&sceneScratch, &g_sceneState_52F7E0, sizeof(sceneScratch));
      nResult = SystemMenu_4655AB(*ppBgmBuffer, -1000, 2, 666.0f, 666.0f,
        sceneScratch);
      StopAndRestartThread(2);
      if (nResult >= 1)
        break;
    }

    nPrevMouseBtn = nMouseBtn;
    GetMouseBtn_45A41B(&nMouseBtn);
    GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

    // button hitboxes (1024x768 design space)
    nHoverCtrl = -1;
    if (yMouse >= (int)ConvertFrom1024AsFloat(612.0f) && yMouse < (int)ConvertFrom1024AsFloat(648.0f))
    {
      if (xMouse >= (int)ConvertFrom1024AsFloat(288.0f) && xMouse < (int)ConvertFrom1024AsFloat(486.0f))
        nHoverCtrl = 0;                                // stop
      else if (xMouse >= (int)ConvertFrom1024AsFloat(522.0f) && xMouse < (int)ConvertFrom1024AsFloat(720.0f))
        nHoverCtrl = 1;                                // play
    }
    else if (yMouse >= (int)ConvertFrom1024AsFloat(666.0f) && yMouse < (int)ConvertFrom1024AsFloat(702.0f))
    {
      if (xMouse >= (int)ConvertFrom1024AsFloat(341.0f) && xMouse < (int)ConvertFrom1024AsFloat(496.0f))
        nHoverCtrl = 2;                                // prev
      else if (xMouse >= (int)ConvertFrom1024AsFloat(512.0f) && xMouse < (int)ConvertFrom1024AsFloat(667.0f))
        nHoverCtrl = 3;                                // next
    }

    if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
    {
      if (nHoverCtrl != -1 && g_arrUIEffectDSBuffer_52F168[3])  // "click"
        DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
      if (nHoverCtrl == 2 && --nSoundIndex < 1)
        nSoundIndex = 1;
      if (nHoverCtrl == 3 && ++nSoundIndex > 14)
        nSoundIndex = 14;
      if (nHoverCtrl == 1)                               // play: swap the BGM
      {
        if (*ppBgmBuffer)
        {
          (*ppBgmBuffer)->Stop();
          if (*ppBgmBuffer)                          // original re-tests
          {
            (*ppBgmBuffer)->Release();
            *ppBgmBuffer = NULL;
          }
        }
        wsprintfA(szFileName, "%s", g_ascSndFileNames_4C78A0[nSoundIndex].str);
        CHanaObj__GetRealPath_486D9A(szFileName, 1006);  // 1006 = WAV dir
        *ppBgmBuffer = LoadWav_422E7B(szFileName);
        if (*ppBgmBuffer)
        {
          int nVol = (int)(g_fVolumeDbTable[g_nAviVolumeIndex_530266] - 1000.0f);
          if (nVol < -10000)
            nVol = -10000;
          else if (nVol > 0)
            nVol = 0;
          (*ppBgmBuffer)->SetVolume(nVol);
          DSBufferPlay(*ppBgmBuffer, 1);             // loop
        }
      }
      if (nHoverCtrl == 0 && *ppBgmBuffer)               // stop
        (*ppBgmBuffer)->Stop();
    }

    // 7x2 sound grid: click selects
    if (nMouseBtn == 1 && nMouseBtn != nPrevMouseBtn)
    {
      for (i = 1; i < 15; ++i)
      {
        if ((int)ConvertFrom1024AsFloat((float)((i - 1) / 7) * 468.0f + 72.0f) <= xMouse
          && xMouse < (int)ConvertFrom1024AsFloat((float)((i - 1) / 7) * 468.0f + 485.0f)
          && (int)ConvertFrom1024AsFloat((float)((i - 1) % 7) * 36.0f + 217.0f) <= yMouse
          && yMouse < (int)ConvertFrom1024AsFloat((float)((i - 1) % 7) * 36.0f + 244.0f))
        {
          nSoundIndex = i;
          if (g_arrUIEffectDSBuffer_52F168[3])      // "click" effect
            DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
        }
      }
    }

    // top-bar hot columns (any click; column 3 right-click only)
    nResult = 0;
    if (nMouseBtn && nMouseBtn != nPrevMouseBtn)
    {
      for (i = 0; i < 4; ++i)
      {
        if (i != 3 || nMouseBtn != 2)
        {
          if (xMouse < (int)ConvertFrom1024AsFloat(128.0f) * i)
            continue;
          if ((int)ConvertFrom1024AsFloat(128.0f) * (i + 1) <= xMouse)
            continue;
          if ((int)ConvertFrom1024AsFloat(48.0f) <= yMouse)
            continue;
        }
        nResult = -(i + 1);
        if (g_arrUIEffectDSBuffer_52F168[3])          // "click" effect
          DSBufferPlay(g_arrUIEffectDSBuffer_52F168[3], 0);
        break;
      }
    }
    if (nResult < -1)                       // -1 would re-enter this screen
      break;

    // (both tables below are re-initialized every frame in the original;
    //  ctrlPos was an interleaved {x,y} DWORD[9] there)
    int arrCtrlXY[4][2] = { {288, 612}, {522, 612}, {341, 666}, {512, 666} };
    arrHighlightRects[0] = { 0, 0, ConvertFrom1024AsInt(198), ConvertFrom1024AsInt(36) };
    arrHighlightRects[1] = { ConvertFrom1024AsInt(198), 0, ConvertFrom1024AsInt(396), ConvertFrom1024AsInt(36) };
    arrHighlightRects[2] = { 0, ConvertFrom1024AsInt(36), ConvertFrom1024AsInt(155), ConvertFrom1024AsInt(72) };
    arrHighlightRects[3] = { ConvertFrom1024AsInt(155), ConvertFrom1024AsInt(36), ConvertFrom1024AsInt(310), ConvertFrom1024AsInt(72) };
    arrHighlightRects[4] = { 0, 0, 0, 0 };

    if (pBmpBackground)
    {
      g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pBackBuffer_4D6C90->Blt(NULL, pBmpBackground, NULL, DDBLT_WAIT, NULL);

      // hovered-button highlight frame from mub.bmp
      if (nHoverCtrl >= 0 && pBmpHighlight)
        g_pBackBuffer_4D6C90->BltFast(
          (int)ConvertFrom1024AsFloat((float)arrCtrlXY[nHoverCtrl][0]),
          (int)ConvertFrom1024AsFloat((float)arrCtrlXY[nHoverCtrl][1]),
          pBmpHighlight, &arrHighlightRects[nHoverCtrl], DDBLTFAST_WAIT);

      // selected-sound indicator cell from mub2.bmp (2x7 cells of 413x28)
      if (nSoundIndex > 0 && pBmpIndicator)
      {
        SetRect(&rc,
          (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) / 7) * 413.0f),
          (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) % 7) * 28.0f),
          (int)ConvertFrom1024AsFloat((float)(((nSoundIndex - 1) / 7) + 1) * 413.0f),
          (int)ConvertFrom1024AsFloat((float)(((nSoundIndex - 1) % 7) + 1) * 28.0f));
        g_pBackBuffer_4D6C90->BltFast(
          (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) / 7) * 468.0f + 72.0f),
          (int)ConvertFrom1024AsFloat((float)((nSoundIndex - 1) % 7) * 36.0f + 217.0f),
          pBmpIndicator, &rc, DDBLTFAST_WAIT);
      }
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasCur);
    }
    if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
      if (pMouseSprite)
      {
        CHanaSpriteT3F6__SetPos_414A5D(pMouseSprite, (float)(xMouse + 16), (float)(yMouse + 16));
        DrawSprite2D(pMouseSprite);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }
    // nop_4863C3();                                       // disabled, as elsewhere
    PresentFrame();
    HanaVert_DrawBatch(0);
  }

  // teardown
  if (pBmpBackground)
  {
    pBmpBackground->Release();
    pBmpBackground = NULL;
  }
  if (pBmpHighlight)
  {
    pBmpHighlight->Release();
    pBmpHighlight = NULL;
  }
  if (pBmpIndicator)
  {
    pBmpIndicator->Release();
    pBmpIndicator = NULL;
  }
  StopAndRestartThread(0);
  return nResult;
}

void register_MenuCharSelect_47A324() {
  hnh_Hook(0x47A324, hook_MenuCharSelect, "MenuCharSelect_47A324"); // [OK]
  hnh_Hook(0x47AD8D, hook_MenuGallery, "MenuGallery_47AD8D"); // [OK]
  hnh_Hook(0x47D163, hook_MenuMusicSelect, "MenuMusicSelect_47D163"); // [OK]

  hnh_HookOff();
  hnh_Hook(0x43BFFA, hook_CheckSpecialJpChar_43BFFA, "CheckSpecialJpChar_43BFFA"); // [OK]
  hnh_Hook(0x4198E0, hook_Surface_SetColorKeyRGB, "Surface_SetColorKeyRGB_4198E0"); // [OK]
  hnh_Hook(0x43BCAE, hook_DrawText_43BCAE, "DrawText_43BCAE");  // [OK]
  hnh_Hook(0x481170, hook_IsMenuKeyPressed_481170, "IsMenuKeyPressed_481170"); // [OK]
  hnh_HookOn();
}
