// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===

CHAR (&g_szHnhPath_530270)[256] = *(CHAR (*)[256])0x530270; // WRITABLE
CHAR (&g_szODSPath)[256]        = *(CHAR (*)[256])0x52F6E0; // WRITABLE
CHAR (&g_szDriveLetr_52FC70)[256] = *(CHAR (*)[256])0x52FC70; // WRITABLE
CHAR (&g_szSPRPath)[256]        = *(CHAR (*)[256])0x52FB70; // WRITABLE
CHAR (&g_szStrPath)[256]        = *(CHAR (*)[256])0x52F5E0; // WRITABLE
CHAR (&g_szODFPath)[256]        = *(CHAR (*)[256])0x52FF70; // WRITABLE
CHAR (&g_szWAVPath)[256]        = *(CHAR (*)[256])0x530070; // WRITABLE
CHAR (&g_szGRPPath)[256]        = *(CHAR (*)[256])0x52F970; // WRITABLE
CHAR (&g_szGRP800Path)[256]     = *(CHAR (*)[256])0x52FA70; // WRITABLE
CHAR (&g_szSEPath)[256]         = *(CHAR (*)[256])0x52FD70; // WRITABLE
CHAR (&g_szDATAPath)[256]       = *(CHAR (*)[256])0x52FE70; // WRITABLE
DWORD &g_dwFontRenderFlag       = *(DWORD *)0x52F1B0; // WRITABLE

// === GLOBAL FUNCTIONS ===

BOOL (__cdecl*IsFileExists_47E8EE)(CHAR* pszFilePath) = reinterpret_cast<BOOL (__cdecl*)(CHAR*)>(0x47E8EE);
void (__cdecl * SetupDirPath_486630)() = reinterpret_cast<void(__cdecl*)()>(0x486630);
bool(__cdecl* CheckInstalledDirRoot_4869A7)(HWND hWnd) = reinterpret_cast<bool(__cdecl*)(HWND hWnd)>(0x4869A7);
void (__cdecl * ParseStrChunkText_47FF01)(CHanaSceneT458_S504 *pScene) = reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *pScene)>(0x47FF01);
void (__cdecl * Scene_CreateFontTextureFromStringData)(CHanaSceneT458_S504 *pScene) = reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *pScene)>(0x47F460);
int (__cdecl * FindCharUntil_487D46)(char *pBuf, int charToFind, int charToFind2OrEnd) = reinterpret_cast<int(__cdecl*)(char *pBuf, int charToFind, int charToFind2OrEnd)>(0x487D46);

CHanaFrameT3EC_S354 *(__cdecl*HanaFrame_FindByName)(LPCSTR lpString, CHanaFrameT3EC_S354 *pHanaT3EC_v1)
  = reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(LPCSTR, CHanaFrameT3EC_S354 *)>(0x4417EB);

void (__cdecl*CHanaObj__NewFrameArrayT3EDFromChildT3EC)(CHanaFrameT3EC_S354 *pFrame_a1, CHanaFrameArrayT3ED_S138 **ppFrameArray)
  = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameArrayT3ED_S138 **)>(0x405A0A);
int (__cdecl*CHanaObj__GetFramesCountT3ED_408E75)(CHanaFrameArrayT3ED_S138 *pFrameArray)
  = reinterpret_cast<int(__cdecl*)(CHanaFrameArrayT3ED_S138 *)>(0x408E75);
HRESULT (__cdecl*CHanaObj__GetFrameAtIndexT3ED)(CHanaFrameArrayT3ED_S138 *pFrameArray, unsigned int nIndex, CHanaFrameT3EC_S354 **ppOut)
  = reinterpret_cast<HRESULT(__cdecl*)(CHanaFrameArrayT3ED_S138 *, unsigned int, CHanaFrameT3EC_S354 **)>(0x408E1B);
void (__cdecl * Character_ApplyAnimationParamsToChildren)( CHanaFrameT3EC_S354 *pRootFrame, char bIsEaseAnimation, int nTimeFrom, int nTimeTo, int nEaseFrom, int nEaseTo)
  = reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pRootFrame, char bIsEaseAnimation, int nTimeFrom, int nTimeTo, int nEaseFrom, int nEaseTo)>(0x487267);
void (__cdecl * CHanaObj__NewFrameArrayT3ED)(CHanaFrameArrayT3ED_S138 **ppFrameArray)
  = reinterpret_cast<void(__cdecl*)(CHanaFrameArrayT3ED_S138 **ppFrameArray)>(0x408DD0);

// === FUNCTION IMPLEMENTATIONS ===

void __cdecl hook_SetupDirPath_486630()
{
    CHAR Src[256]; // [esp+0h] [ebp-100h] BYREF

    DebugLog("[HNH:SUB] %s\n", "SetupDirPath_486630");

    wsprintfA(Src, "%s\\ODS", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szODSPath, "%s\\ODS\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szODSPath, "%s\\ODS\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\SPR", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szSPRPath, "%s\\SPR\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szSPRPath, "%s\\SPR\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\Str", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szStrPath, "%s\\Str\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szStrPath, "%s\\Str\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\ODF", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szODFPath, "%s\\ODF\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szODFPath, "%s\\ODF\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\WAV", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szWAVPath, "%s\\WAV\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szWAVPath, "%s\\WAV\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\GRP", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szGRPPath, "%s\\GRP\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szGRPPath, "%s\\GRP\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\GRP800", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szGRP800Path, "%s\\GRP800\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szGRP800Path, "%s\\GRP800\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\SE", g_szHnhPath_530270);
    if ( IsFileExists_47E8EE(Src) )
        wsprintfA(g_szSEPath, "%s\\SE\\", g_szHnhPath_530270);
    else
        wsprintfA(g_szSEPath, "%s\\SE\\", g_szDriveLetr_52FC70);
    wsprintfA(Src, "%s\\DATA", g_szHnhPath_530270);
    if ( !IsFileExists_47E8EE(Src) )
    {
        wsprintfA(g_szDATAPath, "%s\\DATA", g_szHnhPath_530270);
        CreateDirectoryA(g_szDATAPath, 0);
    }
    wsprintfA(g_szDATAPath, "%s\\DATA\\", g_szHnhPath_530270);
}

// CheckInstalledDirRoot_4869A7(HWND hWnd) -> bool
//
// Locate the game's installation drive and the data-CD drive by scanning every drive
// letter 'A'..'Z' for 17-byte marker files:
//
//   install : <X>:\HaNaHiMe\HDFILE    magic: 00 "1192" F0 " d|ddHD031" FF
//   data CD : <X>:\CDFILE             magic: 00 "1192" F0 " d|ddCD031" FF
//
// The game builds the magics at runtime via wsprintfA(buf, "%c1192%c d|ddHD031%c",
// 0, 0xF0, 0xFF) so the raw bytes never appear as one contiguous string in the binary.
// Only the first 16 bytes are compared; byte 16 (0xFF) is a sentinel.
//
// A drive is scanned only when GetDriveType != DRIVE_UNKNOWN and != DRIVE_REMOVABLE
// (fixed / CD-ROM / network / RAM). The CD scan additionally skips the install drive,
// so the data CD must sit in a *different* drive.
//
// Result:
//   success : g_szHnhPath_530270 = "<X>:\HaNaHiMe", g_szDriveLetr_52FC70 = "<Y>:"
//             (or "" when no CDFILE was found -- NOT an error here), process cwd set
//             to the install dir, SetupDirPath_486630() fills the per-asset path
//             globals, returns TRUE.
//   failure : message box, returns FALSE (caller destroys the window).

// 17-byte marker templates; only the first 16 bytes are compared.
static const BYTE HD_MAGIC[17] = { 0x00, '1', '1', '9', '2', 0xF0, ' ', 'd', '|', 'd', 'd', 'H', 'D', '0', '3', '1', 0xFF };
static const BYTE CD_MAGIC[17] = { 0x00, '1', '1', '9', '2', 0xF0, ' ', 'd', '|', 'd', 'd', 'C', 'D', '0', '3', '1', 0xFF };

// Drive usable for the scan: known type and not removable (fixed / CD-ROM / network / RAM).
static bool DriveIsScanTarget(CHAR chDrive)
{
    CHAR szRoot[8];
    UINT uType;

    wsprintfA(szRoot, "%c:", chDrive);
    uType = GetDriveTypeA(szRoot);
    return uType != DRIVE_UNKNOWN && uType != DRIVE_REMOVABLE;
}

// TRUE iff pszPath exists, is exactly 17 bytes, and its first 16 bytes match szMagic.
// (Faithful to the original: the ReadFile byte count is not verified.)
static bool MarkerFileMatches(LPCSTR pszPath, const BYTE szMagic[17])
{
    HANDLE hFile;
    BYTE szData[17];
    DWORD dwSize, dwRead;

    hFile = CreateFileA(pszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false; // original also tested hFile != 0; dead code (CreateFileA never returns NULL)

    dwSize = GetFileSize(hFile, NULL);
    if (dwSize != sizeof(szData)) // original checks 0 / -1 / ==17 separately; same net effect
    {
        CloseHandle(hFile);
        return false;
    }
    ReadFile(hFile, szData, sizeof(szData), &dwRead, NULL);
    CloseHandle(hFile);
    return memcmp(szMagic, szData, 16) == 0;
}

// Scan 'A'..'Z' (skipping chSkip) for "<drive>:<pszTail>"; returns the drive letter or 0.
static CHAR FindMarkerDrive(CHAR chSkip, LPCSTR pszTail, const BYTE szMagic[17])
{
    CHAR szPath[256];

    for (CHAR ch = 'A'; ch <= 'Z'; ++ch)
    {
        if (ch == chSkip || !DriveIsScanTarget(ch))
            continue;
        wsprintfA(szPath, "%c:%s", ch, pszTail); // "<X>:\HaNaHiMe\HDFILE" / "<X>:\CDFILE"
        if (MarkerFileMatches(szPath, szMagic))
            return ch;
    }
    return 0;
}

bool __cdecl hook_CheckInstalledDirRoot_4869A7(HWND hWnd)
{
    CHAR chInstall, chCdRom;

    DebugLog("[HNH:SUB] CheckInstalledDirRoot_4869A7\n");

    // 1) install drive: <X>:\HaNaHiMe\HDFILE
    g_szHnhPath_530270[0] = '\0'; // original zeroes [0..2]
    chInstall = FindMarkerDrive(0, "\\HaNaHiMe\\HDFILE", HD_MAGIC);
    if (!chInstall)
    {
        // Shift-JIS @0x4CB460: "hitsuyou na folder ga mitsukeraremasen" (the required
        // folder could not be found); escape-encoded so this file stays codepage-safe.
        MessageBoxA(hWnd,
            "ïKóvÇ»ÉtÉHÉãÉ_Å[Ç™å©Ç¬ÇØÇÁÇÍÇ‹ÇπÇÒ",
            "", 0);
        return false;
    }
    wsprintfA(g_szHnhPath_530270, "%c:\\HaNaHiMe", chInstall);

    // 2) data-CD drive: <X>:\CDFILE on any OTHER drive (silent when not found -- stays "")
    g_szDriveLetr_52FC70[0] = '\0'; // original zeroes [0..2]
    chCdRom = FindMarkerDrive(chInstall, "\\CDFILE", CD_MAGIC);
    if (chCdRom)
        wsprintfA(g_szDriveLetr_52FC70, "%c:", chCdRom);

    // 2a) Patched, should be error with:
    //  MessageBoxA(hWnd, "ÅuâÿÇÃîﬂñ¬ÅvÇÃÇcÇâÇìÇãÅ|ÇQÇë}ì¸ÇµÇƒâ∫Ç≥Ç¢", "", 1);

    // 3) activate: cwd = install dir, then fill ODS/SPR/Str/ODF/WAV/GRP/GRP800/SE/DATA paths
    SetCurrentDirectoryA(g_szHnhPath_530270);
    SetupDirPath_486630();
    return true;
}

/**
* Checks whether a file or directory exists, via a FindFirstFileA probe.
* @param pszFilePath Path to check; a trailing backslash is stripped so
*                    directory-style paths ("data\") probe the plain name.
* @return TRUE if any matching name exists, FALSE otherwise.
* @note The game's standard existence check -- called from 9 functions
*       (SetupDirPath_486630, CheckRunGame_4863C8, GameMainLoop_47F013,
*       SystemMenu_4655AB, LoadDataFiles_461557, ...).
*/
BOOL __cdecl hook_IsFileExists_47E8EE(CHAR *pszFilePath)
{
  DebugLog("[HNH:SUB] IsFileExists_47E8EE\n");

  WIN32_FIND_DATAA FindFileData; // [esp+0h] [ebp-248h] BYREF
  CHAR szPath[256]; // [esp+140h] [ebp-108h] BYREF
  BOOL bValid; // [esp+240h] [ebp-8h]
  HANDLE hFindFile; // [esp+244h] [ebp-4h]

  hFindFile = 0;
  memcpy(szPath, pszFilePath, 255u); /*0x47e90e*/
  // strip trailing backslash: [ebp+len-0x109] == szPath[len-1]
  // (decompiler mis-anchors this as FindFileData.cAlternateFileName[len+15])
  if (szPath[lstrlenA(szPath) - 1] == '\\') /*0x47e92e*/
    szPath[lstrlenA(szPath) - 1] = 0; /*0x47e93d*/
  memset(&FindFileData, 0, sizeof(FindFileData)); /*0x47e953*/
  hFindFile = FindFirstFileA(szPath, &FindFileData); /*0x47e96f*/
  bValid = hFindFile != INVALID_HANDLE_VALUE; /*0x47e976*/
  FindClose(hFindFile); /*0x47e98c*/
  return bValid; /*0x47e995*/
}


// ParseStrChunkText(pScene) - (note: requested 0x480008 is interior to this function). Parses
// the next dialogue text chunk from pScene->ptrStrData_260 (stops at ';','*', a non-letter,
// or end), rasterizes it onto a 640x200 offscreen surface via DrawText_43BCAE, then rebuilds
// each of the 4x29 text-glyph sprites' textures from 26x26 tiles of that surface (BltFast +
// CreateTextureFromSurfaceWithAlphaMask + ReplaceTexture3F6). Marks the final glyph's
// field_10C when the chunk is <120 chars. Temporarily disables D3D antialiasing.
void __cdecl hook_ParseStrChunkText_47FF01(CHanaSceneT458_S504 *pScene)
{
  int n; // [esp+0h] [ebp-4Ch]
  signed int fullwidt_length; // [esp+4h] [ebp-48h]
  struct IDirectDrawSurface7 *pTextureWholeText; // [esp+8h] [ebp-44h]
  int i; // [esp+Ch] [ebp-40h]
  DWORD dwSavedAntialias; // [esp+10h] [ebp-3Ch] BYREF
  int v6; // [esp+14h] [ebp-38h]
  int j; // [esp+18h] [ebp-34h]
  int k; // [esp+1Ch] [ebp-30h]
  struct IDirectDrawSurface7 *pTexture; // [esp+20h] [ebp-2Ch] BYREF
  RECT rect; // [esp+24h] [ebp-28h] BYREF
  struct tagRECT rc; // [esp+34h] [ebp-18h] BYREF
  LPDIRECTDRAWSURFACE7 pTextureChar; // [esp+44h] [ebp-8h]
  int v13; // [esp+48h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseStrChunkText_47FF01\n");

  v6 = 0;
  rect.left = 1;
  rect.top = 1;
  rect.right = 31;
  rect.bottom = 31;
  fullwidt_length = 0;
  v13 = 0;
  pTexture = 0;
  pTextureWholeText = CreateSurfaceWH(640u, 200u);
  pTextureChar = CreateSurfaceWH(28u, 28u);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 1);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 1);
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwSavedAntialias);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
  if ( pTextureWholeText && pScene && pScene->ptrStrData_260 )
  {
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    BltSurface(pTextureWholeText, 0, 0, 640, 200, 0);
    BltSurface(pTextureChar, 0, 0, 28, 28, 0);
    g_dwFontRenderFlag = 0;
    for ( i = pScene->dwPtrCurrent_448;
      i < pScene->dwPtrSize_44C
      && *((_BYTE *)pScene->ptrStrData_260 + i) != ';'
      && *((_BYTE *)pScene->ptrStrData_260 + i) != '*'
      && (*((char *)pScene->ptrStrData_260 + i) < 'A' || *((char *)pScene->ptrStrData_260 + i) > 'z');
      i += 2 )
    {
      ;
    }
    if ( pScene->dwPtrSize_44C > (unsigned int)pScene->dwPtrPos_444 )
    {
      fullwidt_length = (unsigned int)(i - pScene->dwPtrCurrent_448) >> 1;
      pScene->dwPtrCurrent_448 = i + 1;
      SetRect(&rc, 0, 0, 640, 180);
      BltSurface(pTextureWholeText, 0, 0, 640, 200, 0);
      DrawText_43BCAE(
        pTextureWholeText,
        (char *)pScene->ptrStrData_260 + pScene->dwPtrPos_444,
        rc.left,
        rc.top,
        rc.right,
        rc.bottom);
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
    }
    g_pD3DDevice_4D6C78->SetTexture(0, 0);
    n = 0;
    for ( j = 0; j < 4; ++j )
    {
      for ( k = 0; k < 29; ++k )
      {
        if ( pScene->textSprites_264[j][k] )
        {
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->textSprites_264[j][k], &rect);
          CHanaObj__SetAlpha3F6(pScene->textSprites_264[j][k], 0.0);
          SetRect(&rc, 26 * (n % 24), 26 * (n / 24), 26 * (n % 24 + 1) + 2, 26 * (n / 24 + 1) + 2);
          ++n;
          pTextureChar->BltFast(0, 0, pTextureWholeText, &rc, 16);
          pTexture = CreateTextureFromSurfaceWithAlphaMask(pTextureChar, pTextureChar);
          if ( pTexture )
          {
            CHanaObj__ReplaceTexture3F6(pScene->textSprites_264[j][k], &pTexture);
            pScene->textSprites_264[j][k]->bOk_10C = 0;
            pTexture = 0;
          }
        }
      }
    }
    if ( fullwidt_length < 120 && pScene->textSprites_264[0][fullwidt_length] )
      pScene->textSprites_264[0][fullwidt_length]->bOk_10C = 1;
  }
  if ( pTextureChar )
    pTextureChar->Release();
  pTextureChar = 0;
  if ( pTextureWholeText )
    pTextureWholeText->Release();
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 2);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwSavedAntialias);
}

void __cdecl hook_Scene_CreateFontTextureFromStringData(CHanaSceneT458_S504 *pScene)
{
  struct IDirectDrawSurface7 *pFontSurface; // [esp+4h] [ebp-244h]
  int i; // [esp+8h] [ebp-240h]
  int nSrcIndex; // [esp+Ch] [ebp-23Ch]
  int nCharIndex; // [esp+Ch] [ebp-23Ch]
  int nSpriteIndex; // [esp+Ch] [ebp-23Ch]
  int j; // [esp+14h] [ebp-234h]
  int k; // [esp+18h] [ebp-230h]
  LPDIRECTDRAWSURFACE7 pTempSurface; // [esp+1Ch] [ebp-22Ch] BYREF
  struct tagRECT rcChar; // [esp+20h] [ebp-228h] BYREF
  struct tagRECT rcDest; // [esp+30h] [ebp-218h] BYREF
  LPDIRECTDRAWSURFACE7 pCharSurface; // [esp+40h] [ebp-208h]
  int nCurrentPos; // [esp+44h] [ebp-204h]
  char szTextBuffer[512]; // [esp+48h] [ebp-200h] BYREF

  ONCELOG("[HNH:SUB] Scene_CreateFontTextureFromStringData_47F460\n");

  rcChar.left = 1;
  rcChar.top = 1;
  rcChar.right = 31;
  rcChar.bottom = 31;
  nCurrentPos = 0;
  pTempSurface = 0;
  pFontSurface = CreateSurfaceWH(0x280u, 0xC8u);
  pCharSurface = CreateSurfaceWH(0x1Cu, 0x1Cu);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 1);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 1);
  if ( pFontSurface && pScene && pScene->ptrStrData_260 )
  {
    BltSurface(pFontSurface, 0, 0, 640, 200, 0);
    BltSurface(pCharSurface, 0, 0, 28, 28, 0);
    g_dwFontRenderFlag = 0;
    memset(szTextBuffer, 538976288, 0xFAu);
    szTextBuffer[251] = 0;
    nSrcIndex = 0;
    for ( i = pScene->dwPtrCurrent_448; i < pScene->dwPtrSize_44C; i += 2 )
    {
      if ( *((_BYTE *)pScene->ptrStrData_260 + i) == ',' )
      {
        nSrcIndex += 54 - nSrcIndex % 54;
        --i;
      }
      else
      {
        if ( *((_BYTE *)pScene->ptrStrData_260 + i) == ';'
          || *((_BYTE *)pScene->ptrStrData_260 + i) == '*'
          || *((char *)pScene->ptrStrData_260 + i) >= 'A' && *((char *)pScene->ptrStrData_260 + i) <= 'z' )
        {
          break;
        }
        szTextBuffer[nSrcIndex] = *((_BYTE *)pScene->ptrStrData_260 + i);
        nCharIndex = nSrcIndex + 1;
        szTextBuffer[nCharIndex] = *((_BYTE *)pScene->ptrStrData_260 + i + 1);
        nSrcIndex = nCharIndex + 1;
      }
    }
    pScene->dwPtrCurrent_448 = i + 1;
    SetRect(&rcDest, 0, 0, 640, 180);
    BltSurface(pFontSurface, 0, 0, 640, 200, 0);
    DrawText_43BCAE(pFontSurface, szTextBuffer, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
    pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
    g_pD3DDevice_4D6C78->SetTexture(0, 0);
    nSpriteIndex = 0;
    for ( j = 0; j < 4; ++j )
    {
      for ( k = 0; k < 29; ++k )
      {
        if ( pScene->textSprites_264[j][k] )
        {
          CHanaSpriteT3F6__SetSrcRect_414B28(pScene->textSprites_264[j][k], &rcChar);
          CHanaObj__SetAlpha3F6(pScene->textSprites_264[j][k], 0.0);
          SetRect(
            &rcDest,
            26 * (nSpriteIndex % 24),
            26 * (nSpriteIndex / 24),
            26 * (nSpriteIndex % 24 + 1) + 2,
            26 * (nSpriteIndex / 24 + 1) + 2);
          ++nSpriteIndex;
          pCharSurface->BltFast(0, 0, pFontSurface, &rcDest, 16);
          pTempSurface = CreateTextureFromSurfaceWithAlphaMask(pCharSurface, pCharSurface);
          if ( pTempSurface )
          {
            CHanaObj__ReplaceTexture3F6(pScene->textSprites_264[j][k], &pTempSurface);
            pScene->textSprites_264[j][k]->bOk_10C = 0;
            pTempSurface = 0;
          }
        }
      }
    }
  }
  if ( pCharSurface )
    pCharSurface->Release();
  pCharSurface = 0;
  if ( pFontSurface )
    pFontSurface->Release();
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 2);
  CHanaObj__SetAlpha3F6(pScene->pSprWindowBmp[3], 0.0);
}

int __cdecl hook_FindCharUntil_487D46(char *pBuf, int charToFind, int charToFind2OrEnd)
{
  int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindCharUntil_487D46\n");

  for ( i = 0; i < 20 && pBuf[i] != charToFind && pBuf[i] != charToFind2OrEnd; ++i )
    ;
  return i;
}

// CreateObject (sets type 0x3ED, AddRef)  
void __cdecl hook_CHanaObj__NewFrameArrayT3ED(CHanaFrameArrayT3ED_S138 **ppFrameArray)
{
  CHanaFrameArrayT3ED_S138 *pFrameArray; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] CHanaObj__NewFrameArrayT3ED_408DD0\n");

  pFrameArray = (CHanaFrameArrayT3ED_S138 *)MemAlloc_40DEF7(0x138u);
  memset(pFrameArray, 0, sizeof(CHanaFrameArrayT3ED_S138));
  pFrameArray->dwType = 0x3ED;
  CHanaObj__AddRef_40BFC3(pFrameArray);
  *ppFrameArray = pFrameArray;
}

// Idiomatic with IDirect3DRMFrame::GetChildren() - walks pFirstChildNode linked list,
// flattens nCountChild children into array; fails (NULL) if no children
void __cdecl hook_CHanaObj__NewFrameArrayT3EDFromChildT3EC(
  CHanaFrameT3EC_S354 *pFrame,
  CHanaFrameArrayT3ED_S138 **ppFrameArray)
{
  unsigned int i; // [esp+0h] [ebp-Ch]
  CHanaFrameArrayT3ED_S138 *pFrameArray; // [esp+4h] [ebp-8h] BYREF
  CHanaNode *pFirstChildNode; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__NewFrameArrayT3EDFromChildT3EC_405A0A\n");

  pFrameArray = 0;
  if ( pFrame->nCountChild )
  {
    CHanaObj__NewFrameArrayT3ED(&pFrameArray);
    pFrameArray->nFrameCount = pFrame->nCountChild;
    pFrameArray->arrFrames = (CHanaFrameT3EC_S354 **)MemAlloc_40DEF7(4 * pFrameArray->nFrameCount);
    pFirstChildNode = pFrame->pFirstChildNode;
    for ( i = 0; i < pFrame->nCountChild; ++i )
    {
      pFrameArray->arrFrames[i] = pFirstChildNode->pData;
      pFirstChildNode = pFirstChildNode->pNext;
    }
    *ppFrameArray = pFrameArray;
  }
  else
  {
    *ppFrameArray = 0;
  }
}

// Idiomatic with IDirect3DRMFrameArray::GetSize()
int __cdecl hook_CHanaObj__GetFramesCountT3ED_408E75(CHanaFrameArrayT3ED_S138 *pFrameArray)
{
  ONCELOG("[HNH:SUB] CHanaObj__GetFramesCountT3ED_408E75\n");

  if ( pFrameArray )
    return pFrameArray->nFrameCount;
  else
    return 0;
}

// Idiomatic with IDirect3DRMFrameArray::GetElement() - bounds-checked, AddRefs returned frame;
// NULL input ?ÅN NULL out, success (D3DRM returns S_FALSE there) 
HRESULT __cdecl hook_CHanaObj__GetFrameAtIndexT3ED(
  CHanaFrameArrayT3ED_S138 *pFrameArray,
  unsigned int nIndex,
  CHanaFrameT3EC_S354 **ppOut)
{
  ONCELOG("[HNH:SUB] hook_CHanaObj__GetFrameAtIndexT3ED_408E1B\n");

  if ( pFrameArray )
  {
    if ( pFrameArray->nFrameCount >= nIndex )
    {
      *ppOut = pFrameArray->arrFrames[nIndex];
      CHanaObj__AddRef_40BFC3(*ppOut);
      return 0;
    }
    else
    {
      *ppOut = 0;
      return E_FAIL;
    }
  }
  else
  {
    *ppOut = 0;
    return 0;
  }
}

void __cdecl hook_Character_ApplyAnimationParamsToChildren(
  CHanaFrameT3EC_S354 *pRootFrame,
  char bIsEaseAnimation,
  int nTimeFrom,
  int nTimeTo,
  int nEaseFrom,
  int nEaseTo)
{
  CHanaMorpT457_S40C *pMorp; // [esp+8h] [ebp-14h]
  CHanaFrameArrayT3ED_S138 *pFrameLists; // [esp+Ch] [ebp-10h] BYREF
  unsigned int i; // [esp+10h] [ebp-Ch]
  CHanaFrameT3EC_S354 *pChildFrame; // [esp+14h] [ebp-8h] BYREF
  unsigned int FramesCountT3ED_408E75; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] Character_ApplyAnimationParamsToChildren_487267\n");

  CHanaObj__NewFrameArrayT3EDFromChildT3EC(pRootFrame, &pFrameLists);
  FramesCountT3ED_408E75 = CHanaObj__GetFramesCountT3ED_408E75(pFrameLists);
  for ( i = 0; i < FramesCountT3ED_408E75; ++i )
  {
    CHanaObj__GetFrameAtIndexT3ED(pFrameLists, i, &pChildFrame);
    if ( pChildFrame->pAttachedObj_20C && pChildFrame->pAttachedObj_20C->dwType == 0x457 )
    {
      pMorp = pChildFrame->pAttachedObj_20C;
      if ( bIsEaseAnimation == 1 && pMorp->dwEaseEnabled_358 )
      {
        pMorp->nEaseFrom_35C = nEaseFrom;
        pMorp->nEaseTo_360 = nEaseTo;
        pMorp->dwEaseEnabled_358 = 2;
        pMorp->field_364 = nEaseFrom;
        pMorp->field_368 = nEaseTo;
        pMorp->fEaseProgress_384 = (float)timeGetTime();
      }
      else if ( !bIsEaseAnimation && pMorp->nTimeState_36C )
      {
        pMorp->nTimeFrom_370 = nTimeFrom;
        pMorp->nTimeTo_374 = nTimeTo;
        pMorp->dwClockStart_380 = timeGetTime();
        pMorp->nTimeState_36C = 2;
        pMorp->nNextFrom_378 = nEaseFrom;
        pMorp->nNextTo_37C = nEaseTo;
      }
    }
    Character_ApplyAnimationParamsToChildren(pChildFrame, bIsEaseAnimation, nTimeFrom, nTimeTo, nEaseFrom, nEaseTo);
    if ( pChildFrame )
    {
      CHanaObj__Free_40C1E4(pChildFrame);
      pChildFrame = 0;
    }
  }
  if ( pFrameLists )
    CHanaObj__Free_40C1E4(pFrameLists);
}

// HanaFrame_FindByName - recursively search the frame tree (starting at pFrame) for a frame whose szName_8 matches pName. Comparison is case-sensitive and full-length (lstrlenA + 1, so it includes the terminator). On a match it increments the frame's reference count (CHanaObj_AddRef_S130_40BFC3) and returns that frame; otherwise it recurses through every child (pChild->pData) and returns the first descendant found, or NULL if none match. Used by HanaMorpT457_SetupFuns to resolve a sub-object's source frame from its stored source name.
CHanaFrameT3EC_S354 *__cdecl hook_HanaFrame_FindByName(LPCSTR pName, CHanaFrameT3EC_S354 *pFrame)
{
  int nLen; // eax
  unsigned int i; // [esp+0h] [ebp-10h]
  CHanaFrameT3EC_S354 *pFound; // [esp+8h] [ebp-8h]
  CHanaNode *pChild; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaFrame_FindByName_4417EB\n");

  pFound = 0;
  nLen = lstrlenA(pName);
  if ( !memcmp(pFrame->szName_8, pName, nLen + 1) )
  {
    CHanaObj__AddRef_40BFC3(pFrame);
    return pFrame;
  }
  else
  {
    pChild = pFrame->pFirstChildNode;
    for ( i = 0; i < pFrame->nCountChild; ++i )
    {
      if ( pChild->pData )
        pFound = HanaFrame_FindByName(pName, pChild->pData);
      if ( pFound )
        break;
      pChild = pChild->pNext;
    }
    return pFound;
  }
}

void register_CheckInstalledDirRoot_4869A7() {
  IsFileExists_47E8EE = hook_IsFileExists_47E8EE;
  SetupDirPath_486630 = hook_SetupDirPath_486630;
  CheckInstalledDirRoot_4869A7 = hook_CheckInstalledDirRoot_4869A7;
  ParseStrChunkText_47FF01 = hook_ParseStrChunkText_47FF01;
  Scene_CreateFontTextureFromStringData = hook_Scene_CreateFontTextureFromStringData;
  FindCharUntil_487D46 = hook_FindCharUntil_487D46;
  HanaFrame_FindByName = hook_HanaFrame_FindByName;
  CHanaObj__NewFrameArrayT3EDFromChildT3EC = hook_CHanaObj__NewFrameArrayT3EDFromChildT3EC;
  CHanaObj__GetFramesCountT3ED_408E75 = hook_CHanaObj__GetFramesCountT3ED_408E75;
  CHanaObj__GetFrameAtIndexT3ED = hook_CHanaObj__GetFrameAtIndexT3ED;
  Character_ApplyAnimationParamsToChildren = hook_Character_ApplyAnimationParamsToChildren;
  CHanaObj__NewFrameArrayT3ED = hook_CHanaObj__NewFrameArrayT3ED;

  hnh_HookOff();
  hnh_Hook(0x47F460, hook_Scene_CreateFontTextureFromStringData, "Scene_CreateFontTextureFromStringData_47F460");


  hnh_Hook(0x4869A7, hook_CheckInstalledDirRoot_4869A7, "CheckInstalledDirRoot_4869A7"); // [OK] 
  hnh_Hook(0x486630, hook_SetupDirPath_486630, "SetupDirPath_486630"); // [OK] 
  hnh_Hook(0x47E8EE, hook_IsFileExists_47E8EE, "IsFileExists_47E8EE"); // [OK] 
  hnh_Hook(0x408E75, hook_CHanaObj__GetFramesCountT3ED_408E75, "CHanaObj__GetFramesCountT3ED_408E75"); // [OK]
  hnh_Hook(0x405A0A, hook_CHanaObj__NewFrameArrayT3EDFromChildT3EC, "CHanaObj__NewFrameArrayT3EDFromChildT3EC_405A0A"); // [OK]
  hnh_Hook(0x408DD0, hook_CHanaObj__NewFrameArrayT3ED, "CHanaObj__NewFrameArrayT3ED_408DD0"); // [OK]
  hnh_Hook(0x47FF01, hook_ParseStrChunkText_47FF01, "ParseStrChunkText_47FF01"); // [OK]
  hnh_Hook(0x487D46, hook_FindCharUntil_487D46, "FindCharUntil_487D46"); // [OK]
  hnh_Hook(0x487267, hook_Character_ApplyAnimationParamsToChildren, "Character_ApplyAnimationParamsToChildren_487267"); // [OK]
  hnh_Hook(0x408E1B, hook_CHanaObj__GetFrameAtIndexT3ED, "CHanaObj__GetFrameAtIndexT3ED_408E1B"); // [OK]
  hnh_Hook(0x4417EB, hook_HanaFrame_FindByName, "HanaFrame_FindByName_4417EB"); // [OK]
  hnh_HookOn();
}
