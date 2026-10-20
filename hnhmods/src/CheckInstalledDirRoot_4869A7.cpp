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

// === FUNCTION IMPLEMENTATIONS ===

void __cdecl SetupDirPath_486630()
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

bool __cdecl CheckInstalledDirRoot_4869A7(HWND hWnd)
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

void register_CheckInstalledDirRoot_4869A7() {
    hnh_Hook(0x4869A7, CheckInstalledDirRoot_4869A7, "CheckInstalledDirRoot_4869A7");
    hnh_Hook(0x486630, SetupDirPath_486630, "SetupDirPath_486630");
}
