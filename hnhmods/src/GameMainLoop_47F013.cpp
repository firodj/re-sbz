#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// GameMainLoop_47F013() -- top-level game loop, entered once from WinMain after
// CheckRunGame / D3D init. One-time intro (war.bmp splash, sound-table reset,
// data0.dat load, op.avi), then forever: InGameMenu -> walk the scene-chapter
// table g_arrSceneNo_4C6F78 dispatching on nSceneType -> autosave data0.dat ->
// back to the menu, until the menu returns 1 (quit).
//
// Scene-chapter table (CSceneSlot[10] @0x4C6F78, {nSceneType, nStrIndex}):
//   step  type  str   meaning
//     1    1     0    ODS dialogue
//     2    1     1    ODS dialogue
//     3    2     1    map battle
//     4    3     1    duel
//     5    4     1    practice
//     6    5     3    ending: ED.avi, step := 0, progress := 2
//     7    0     -    chapter terminator
//     8    6     3    backdrop, step := 0, progress := 1 when still 0
//     9    1     4    ODS dialogue
//    10    0     -    table end (first dword past the [10] array is 0)
// A scene returning 1 ends the chapter; >= 2 replays the SAME step (the scene
// itself moved menuState.bActiveSceneStep); anything else advances until
// nSceneType == 0. g_sceneProgressFlag_530269: 0 = fresh, 1 = backdrop seen,
// 2 = ending seen.

// === PINNED GAME FUNCTIONS ===
LPDIRECTDRAWSURFACE7(__cdecl* GetSurfaceFromBmp_419DE5)(LPCSTR lpFileName) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)(LPCSTR)>(0x419DE5);
int (__cdecl*ConvertFrom640AsInt)(int n) = reinterpret_cast<int(__cdecl*)(int)>(0x47E6AF);
void (__cdecl*StopAndRestartThread)(int bThreadType) =
    reinterpret_cast<void(__cdecl*)(int)>(0x481AE4);
void (__cdecl*ReleaseSoundEffects)(void) = reinterpret_cast<void(__cdecl*)()>(0x47EEEC);
void (__cdecl*PlayVideoWith3DAudio_4791A3)(const char* szMoviePath, int bLoopFlags) =
    reinterpret_cast<void(__cdecl*)(const char*, int)>(0x4791A3);
LPDIRECTDRAWSURFACE7(__cdecl*LoadSoundEffects_47EB7F)(void) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)()>(0x47EB7F);
int (__cdecl*InGameMenu_4799E2)(void) = reinterpret_cast<int(__cdecl*)()>(0x4799E2);
void (__cdecl*MainMenu_47DF0D)(void) = reinterpret_cast<void(__cdecl*)()>(0x47DF0D);
int (__cdecl*PlayOdsDialogueScene_477F90)(int nOdsIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x477F90);
int (__cdecl*PlayMapBattle_45BCAE)(int nStrIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x45BCAE);
int (__cdecl*RunDuelBattle_44C38D)(int nDuelIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x44C38D);
int (__cdecl*RunPracticeScene_4703AE)(int iModeId, int iMenuTab) =
    reinterpret_cast<int(__cdecl*)(int, int)>(0x4703AE);
void (__cdecl*ShowBackdropAndWait_4795CE)(void) = reinterpret_cast<void(__cdecl*)()>(0x4795CE);
void (__cdecl*GameMainLoop_47F013)(void) = reinterpret_cast<void(__cdecl*)()>(0x47F013);
int(__cdecl* BMP_DitherRGB24ToRGB565)(char, char, char, int, int) = reinterpret_cast<int(__cdecl*)(char, char, char, int, int)>(0x421BD5);
void(__cdecl* BMP_Convert24bitTo16bitWithDither)(CBMPFile* pBMPFile) = reinterpret_cast<void(__cdecl*)(CBMPFile* pBMPFile)>(0x421FF4);
HRESULT(__cdecl* GetDC_4188B4)(LPDIRECTDRAWSURFACE7 pDDSurface, HDC* pDC) = reinterpret_cast<HRESULT(__cdecl*)(LPDIRECTDRAWSURFACE7 pDDSurface, HDC* pDC)>(0x4188B4);

// === PINNED GLOBALS ===
LPDIRECTSOUNDBUFFER (&g_arrUIEffectDSBuffer_52F168)[5] = *(LPDIRECTSOUNDBUFFER(*)[5])0x52F168; // WRITABLE
int  (&g_arrFootstepSound_52F17C)[4] = *(int(*)[4])0x52F17C; // WRITABLE
int  (&g_arrFootstepSound_52F18C)[4] = *(int(*)[4])0x52F18C; // WRITABLE
BYTE (&g_byData0Dat_530170)[256]     = *(BYTE(*)[256])0x530170; // WRITABLE
BYTE &g_nAviVolumeIndex_530266       = *(BYTE*)0x530266; // WRITABLE
BYTE &g_nAviFrameMultipler_530267    = *(BYTE*)0x530267; // WRITABLE
BYTE &g_sceneProgressFlag_530269     = *(BYTE*)0x530269; // WRITABLE
CSceneState &g_sceneState_52F7E0     = *(CSceneState*)0x52F7E0; // WRITABLE
CSceneSlot (&g_arrSceneNo_4C6F78)[10] = *(CSceneSlot(*)[10])0x4C6F78; // READ-ONLY
// Vestigial save-name format string. Past .data's raw image (runtime zero-fill,
// IDA shows 0xFF filler) and never written -- this file holds its only xref --
// so it is always the empty string. The guard below really tests the DATA
// directory, which SetupDirPath_486630 just created -> the autosave always fires.
CHAR *g_pszSaveNameFmt_52F1AC = (CHAR*)0x52F1AC; // WRITABLE (never written)

const char* &g_szLastDDError_51836C = *(const char* *)0x51836C; // WRITABLE

// === FUNCTION ===
void __cdecl hook_GameMainLoop_47F013(void)
{
    CHAR szPathBuf[152];               // [esp+4h] [ebp-B4h]
    LPDIRECTDRAWSURFACE7 pWarSurface;  // [esp+9Ch] [ebp-1Ch]
    int nMenuResult;                   // [esp+A4h] [ebp-14h]
    RECT rc;                           // [esp+A8h] [ebp-10h] BYREF

    DebugLog("[HNH:SUB] GameMainLoop_47F013\n");

    nMenuResult = 0;

    // --- one-time intro ----------------------------------------------------
    // war.bmp splash stretched over the primary surface (640x480 design
    // resolution, scaled to the current mode).
    wsprintfA(szPathBuf, "war.bmp");                  // aWarBmp @0x4C72F8
    CHanaObj__GetRealPath_486D9A(szPathBuf, 1007);    // 1007 = GRP dir
    pWarSurface = GetSurfaceFromBmp_419DE5(szPathBuf);
    if (pWarSurface)
    {
        SetRect(&rc, 0, 0, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
        g_pPrimaryBuffer_4D6C88->Blt(&rc, pWarSurface, NULL, DDBLT_WAIT, NULL);
        pWarSurface->Release();
    }

    // reset the UI-effect DirectSound buffers and both footstep-sound tables
    memset(g_arrUIEffectDSBuffer_52F168, 0, 0x14);
    memset(g_arrFootstepSound_52F18C, 0, sizeof(g_arrFootstepSound_52F18C));
    memset(g_arrFootstepSound_52F17C, 0, sizeof(g_arrFootstepSound_52F17C));

    // reset the data0.dat save image, default AVI volume (2), byte_530267 := 1
    memset(g_byData0Dat_530170, 0, 0x100);
    g_nAviVolumeIndex_530266 = 2;
    g_nAviFrameMultipler_530267 = 1;

    // load the previous save if present: <DATA>\data0.dat
    wsprintfA(szPathBuf, "data0.dat");                // aData0Dat @0x4C7300
    CHanaObj__GetRealPath_486D9A(szPathBuf, 1010);    // 1010 = DATA dir
    if (IsFileExists_47E8EE(szPathBuf))
        ReadFileToBuffer(szPathBuf, g_byData0Dat_530170, 256);

    StopAndRestartThread(0);                          // twice, as in the original
    StopAndRestartThread(0);
    ReleaseSoundEffects();
    PlayVideoWith3DAudio_4791A3("op.avi", 0);         // aOpAvi @0x4C730C
    LoadSoundEffects_47EB7F();

    // --- outer loop: menu <-> scene chapter <-> autosave ---------------------
    for (;;)
    {
        memset(&g_sceneState_52F7E0, 0, sizeof(g_sceneState_52F7E0));
        g_sceneState_52F7E0.field_C4 = 66600.0f;
        g_sceneState_52F7E0.field_C0 = 66600.0f;

        // In-game menu. 999 -> title menu (then replay the opening); the replay
        // loops back through the menu until it answers with neither -1 nor 999.
        for (;;)
        {
            nMenuResult = InGameMenu_4799E2();
            if (nMenuResult == 999)
            {
                MainMenu_47DF0D();
                nMenuResult = -1;
            }
            if (nMenuResult != -1)
                break;
            ReleaseSoundEffects();
            PlayVideoWith3DAudio_4791A3("op.avi", 0); // aOpAvi_0 @0x4C7314
            LoadSoundEffects_47EB7F();
        }

        if (nMenuResult != 1)                         // 1 = quit, no chapter run
        {
            for (;;)                                  // walk the chapter table
            {
                D3DSetRenderStates_4276C0();
                int nStep = g_sceneState_52F7E0.menuState.bActiveSceneStep;

                switch (g_arrSceneNo_4C6F78[nStep].nSceneType)
                {
                case 1:
                    nMenuResult = PlayOdsDialogueScene_477F90(g_arrSceneNo_4C6F78[nStep].nStrIndex);
                    break;
                case 2:
                    nMenuResult = PlayMapBattle_45BCAE(g_arrSceneNo_4C6F78[nStep].nStrIndex);
                    break;
                case 3:
                    nMenuResult = RunDuelBattle_44C38D(g_arrSceneNo_4C6F78[nStep].nStrIndex);
                    break;
                case 4:
                    nMenuResult = RunPracticeScene_4703AE(g_arrSceneNo_4C6F78[nStep].nStrIndex, 0);
                    break;
                case 5:                               // ending
                    nMenuResult = 1;
                    ReleaseSoundEffects();
                    PlayVideoWith3DAudio_4791A3("ED.avi", 0);  // aEdAvi @0x4C731C
                    LoadSoundEffects_47EB7F();
                    g_sceneState_52F7E0.menuState.bActiveSceneStep = 0;
                    g_sceneProgressFlag_530269 = 2;    // ending seen
                    break;
                case 6:                               // backdrop
                    nMenuResult = 1;
                    ShowBackdropAndWait_4795CE();
                    g_sceneState_52F7E0.menuState.bActiveSceneStep = 0;
                    if (!g_sceneProgressFlag_530269)
                        g_sceneProgressFlag_530269 = 1; // backdrop seen
                    break;
                default:
                    break;
                }

                if (nMenuResult == 1)                 // chapter finished
                {
                    nMenuResult = 0;
                    break;
                }
                if (nMenuResult < 2)                  // advance to the next step
                {
                    ++g_sceneState_52F7E0.menuState.bActiveSceneStep;
                    if (!g_arrSceneNo_4C6F78[g_sceneState_52F7E0.menuState.bActiveSceneStep].nSceneType)
                        break;                        // fell off the chapter table
                }
                // nMenuResult >= 2: replay the (scene-selected) current step
            }
            nMenuResult = 0;
        }

        // autosave: <DATA>\data0.dat -- the format buffer is always "" (see
        // g_pszSaveNameFmt_52F1AC above), so the guard tests the DATA
        // directory itself and the save always writes.
        wsprintfA(szPathBuf, g_pszSaveNameFmt_52F1AC);
        CHanaObj__GetRealPath_486D9A(szPathBuf, 1010);
        if (IsFileExists_47E8EE(szPathBuf))
        {
            wsprintfA(szPathBuf, "data0.dat");
            CHanaObj__GetRealPath_486D9A(szPathBuf, 1010);
            WriteBufferToFile(szPathBuf, g_byData0Dat_530170, 0x100);
        }

        if (nMenuResult == 1)                         // menu said quit
            break;
    }
    ReleaseSoundEffects();
}

LPDIRECTDRAWSURFACE7 __cdecl hook_GetSurfaceFromBmp_419DE5(LPCSTR lpFileName)
{
  CBMPFile *pBMPFile; // [esp+0h] [ebp-8Ch]
  HDC hdc; // [esp+4h] [ebp-88h] BYREF
  LPDIRECTDRAWSURFACE7 pDDSurface; // [esp+8h] [ebp-84h] BYREF
  HRESULT hr; // [esp+Ch] [ebp-80h]
  DDSURFACEDESC2 ddSurfaceDesc; // [esp+10h] [ebp-7Ch] BYREF

  DebugLog("[HNH:SUB] GetSurfaceFromBmp_419DE5\n");

  pBMPFile = LoadBmp_420DF0(lpFileName);
  if ( !pBMPFile )
    return 0;
  if ( g_dwBytesPerPixel == 2 )
    BMP_Convert24bitTo16bitWithDither(pBMPFile);
  memset(&ddSurfaceDesc, 0, sizeof(ddSurfaceDesc));
  ddSurfaceDesc.dwSize = 124;
  ddSurfaceDesc.dwFlags = 7;
  ddSurfaceDesc.ddsCaps.dwCaps = 2112;
  ddSurfaceDesc.dwWidth = pBMPFile->pBitmapInfoHeader->biWidth;
  ddSurfaceDesc.dwHeight = pBMPFile->pBitmapInfoHeader->biHeight;
  hr = g_pDD_4D6C7C->CreateSurface(&ddSurfaceDesc, & pDDSurface, 0);
  if ( hr )
  {
    FreeBmp_4211ED(pBMPFile);
    g_szLastDDError_51836C = DDErrToString_41898E(hr);
    return 0;
  }
  else
  {
    hr = GetDC_4188B4(pDDSurface, &hdc);
    if ( hr != S_OK )
    {
      if ( pDDSurface )
      {
        pDDSurface->Release();
        pDDSurface = 0;
      }
      FreeBmp_4211ED(pBMPFile);
      g_szLastDDError_51836C = DDErrToString_41898E(hr);
      return 0;
    }
    else
    {
      StretchDIBits(
        hdc,
        0,
        0,
        pBMPFile->pBitmapInfoHeader->biWidth,
        pBMPFile->pBitmapInfoHeader->biHeight,
        0,
        0,
        pBMPFile->pBitmapInfoHeader->biWidth,
        pBMPFile->pBitmapInfoHeader->biHeight,
        pBMPFile->pData,
        (BITMAPINFO*)pBMPFile->pBitmapInfoHeader,
        0,
        0xCC0020u);
      FreeBmp_4211ED(pBMPFile);
      pDDSurface->ReleaseDC(hdc);
      return pDDSurface;
    }
  }
}

HRESULT __cdecl hook_GetDC_4188B4(LPDIRECTDRAWSURFACE7 pDDSurface, HDC *pDC)
{
  int nRetry; // [esp+0h] [ebp-8h]
  HRESULT hResult; // [esp+4h] [ebp-4h]

  DebugLog("[HNH:SUB] GetDC_4188B4\n");

  nRetry = 0;
  while ( 1 )
  {
    hResult = pDDSurface->GetDC(pDC);
    if ( hResult == S_OK )
      break;
    if ( ++nRetry > 30 )
    {
      *pDC = 0;
      return hResult;
    }
    Sleep(1u);
  }
  return hResult;
}

char (&g_BayerDitherMatrix8x8)[] = *(char (*)[])0x4B23B0;

int __cdecl hook_BMP_DitherRGB24ToRGB565(char b, char g, char r, int x, int y)
{
  int ditherY; // eax
  int ditherX; // ecx
  unsigned __int16 g6; // [esp+0h] [ebp-18h]
  unsigned __int16 b5; // [esp+Ch] [ebp-Ch]
  unsigned __int16 r5; // [esp+10h] [ebp-8h]

  DebugLog("[HNH:SUB] %s\n", "BMP_DitherRGB24ToRGB565_421BD5");

  ditherY = y % 8;
  ditherX = x % 8;
  r5 = b & 0xF8;
  if ( g_BayerDitherMatrix8x8[72 * (b & 7) + 9 * (y % 8) + x % 8] == '1' )
  {
    r5 += 8;
    if ( r5 > 0xF8u )
      r5 = 0xF8;
  }
  g6 = g & 0xF8;
  if ( g_BayerDitherMatrix8x8[72 * (g & 7) + 9 * ditherY + ditherX] == '1' )
  {
    g6 += 8;
    if ( g6 > 0xF8u )
      g6 = 0xF8;
  }
  b5 = r & 0xF8;
  if ( g_BayerDitherMatrix8x8[72 * (r & 7) + 9 * ditherY + ditherX] == '1' )
  {
    b5 += 8;
    if ( b5 > 0xF8u )
      b5 = 0xF8;
  }
  return (r5 >> 3) | (4 * g6) | (b5 << 7);
}

int __cdecl hook_ConvertFrom640AsInt(int v)
{
  static int log_ConvertFrom640AsInt = 1;
  log_ConvertFrom640AsInt--;
  if (!log_ConvertFrom640AsInt) {
    DebugLog("[HNH:SUB] %s\n", "ConvertFrom640AsInt_47E6AF");
    log_ConvertFrom640AsInt = 1000;
  }

  return g_fScreenScale_52F19C * v / 640;
}

void register_GameMainLoop_47F013() {
    hnh_Hook(0x47F013, hook_GameMainLoop_47F013, "GameMainLoop_47F013");
    hnh_Hook(0x419DE5, hook_GetSurfaceFromBmp_419DE5, "GetSurfaceFromBmp_419DE5");
    hnh_Hook(0x4188B4, hook_GetDC_4188B4, "GetDC_4188B4");
    hnh_Hook(0x421BD5, hook_BMP_DitherRGB24ToRGB565, "BMP_DitherRGB24ToRGB565");
    hnh_Hook(0x47E6AF, hook_ConvertFrom640AsInt, "ConvertFrom640AsInt_47E6AF");
}
