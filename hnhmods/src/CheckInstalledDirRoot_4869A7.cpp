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
uint32_t &g_dwFontRenderFlag = *(uint32_t *)0x52F1B0; // WRITABLE

// === GLOBAL FUNCTIONS ===

BOOL (__cdecl*IsFileExists_47E8EE)(CHAR* pszFilePath) = reinterpret_cast<BOOL (__cdecl*)(CHAR*)>(0x47E8EE);
void (__cdecl * SetupDirPath_486630)() = reinterpret_cast<void(__cdecl*)()>(0x486630);
bool(__cdecl* CheckInstalledDirRoot_4869A7)(HWND hWnd) = reinterpret_cast<bool(__cdecl*)(HWND hWnd)>(0x4869A7);

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
            "\x95\x4B\x97\x76\x82\xC8\x83\x74\x83\x48\x83\x8B\x83\x5F\x81\x5B"
            "\x82\xAA\x8C\xA9\x82\xC2\x82\xAF\x82\xE7\x82\xEA\x82\xDC\x82\xB9\x82\xF1",
            "", 0);
        return false;
    }
    wsprintfA(g_szHnhPath_530270, "%c:\\HaNaHiMe", chInstall);

    // 2) data-CD drive: <X>:\CDFILE on any OTHER drive (silent when not found -- stays "")
    g_szDriveLetr_52FC70[0] = '\0'; // original zeroes [0..2]
    chCdRom = FindMarkerDrive(chInstall, "\\CDFILE", CD_MAGIC);
    if (chCdRom)
        wsprintfA(g_szDriveLetr_52FC70, "%c:", chCdRom);

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



void register_CheckInstalledDirRoot_4869A7() {
  hnh_Hook(0x47FF01, hook_ParseStrChunkText_47FF01, "ParseStrChunkText_47FF01");

  hnh_HookOff();
  hnh_Hook(0x4869A7, hook_CheckInstalledDirRoot_4869A7, "CheckInstalledDirRoot_4869A7"); // [OK] 
  hnh_Hook(0x486630, hook_SetupDirPath_486630, "SetupDirPath_486630"); // [OK] 
  hnh_Hook(0x47E8EE, hook_IsFileExists_47E8EE, "IsFileExists_47E8EE"); // [OK] 
  hnh_HookOn();
}
