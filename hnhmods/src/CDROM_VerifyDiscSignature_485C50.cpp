#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

#include <VersionHelpers.h>

// --- CD-ROM disc-signature check (the "Disk-2" GM-label loop) -----------------
//
// CDROM_VerifyDiscSignature(chDriveLetter, szExpectedLabel, nLabelSize) reads a raw
// sector off the CD in the given drive and compares the disc's volume-label bytes
// against szExpectedLabel. Return convention: TRUE = disc OK *or* verification
// impossible (every error path fails open); FALSE = definitely the wrong disc.
//
// The caller (OnCheckSystem in WndProcCheckSystem_485FC2.cpp) passes szGMLabel with
// size 4 -- szGMLabel holds "GM" and sits next to szOne ("1") in its frame, so the
// compared bytes are 'G','M','\0','1': the disc label contains an embedded NUL that
// ordinary burning tools and ANSI label APIs (which truncate at NUL) cannot
// reproduce. That is the copy protection.
//
// No MinHook registration: every function in this cluster has exactly 1 xref and is
// reached only via CDROM_VerifyDiscSignature <- WndProcCheckSystem_485FC2.

// 28-byte register block passed to VWIN32_DIOC_DOS_IOCTL (control code 1) on
// \\.\vwin32. (The game's decompiler shows a 6-dword array plus a stray "v4" --
// it is really 7 dwords: v4 == regs[6] == reg_Flags.)
typedef struct _DIOC_REGISTERS {
    DWORD reg_EBX, reg_EDX, reg_ECX, reg_EAX, reg_EDI, reg_ESI, reg_Flags;
} DIOC_REGISTERS;

int(*CDROM_DeviceIoControlVWIN32)(DIOC_REGISTERS* regs)
= reinterpret_cast<int(*)(DIOC_REGISTERS* regs)>(0x485B27);

// Returns 1 on success. reg_Flags bit 0x8000 is an error sentinel: preset here, a
// successful DOS call clears it. (Original leaks hVWIN32 when DeviceIoControl
// fails; it is closed here instead.)
int hook_CDROM_DeviceIoControlVWIN32(DIOC_REGISTERS *regs)
{
    DWORD dwBytesReturned;
    HANDLE hVWIN32;

    DebugLog("[HNH:SUB] CDROM_DeviceIoControlVWIN32_485B27\n");

    regs->reg_Flags = 0x8000;
    hVWIN32 = CreateFileA("\\\\.\\vwin32",   // g_szVWIN32Device @0x4C764C
                          GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hVWIN32 == INVALID_HANDLE_VALUE)
        return 0;
    if (DeviceIoControl(hVWIN32, 1 /* VWIN32_DIOC_DOS_IOCTL */,
                        regs, sizeof(*regs), regs, sizeof(*regs), &dwBytesReturned, NULL))
    {
        CloseHandle(hVWIN32);
        return 1;
    }
    CloseHandle(hVWIN32);
    return 0;
}

BOOL(*CDROM_IoctlReadSectorWin9x)(void* pBuffer, int nDriveNumber)
= reinterpret_cast<BOOL(*)(void* pBuffer, int nDriveNumber)
>(0x485ADA);

// Raw sector read via DOS Int 21h 440Dh (generic block-device IOCTL), category 08h
// (disk) / minor 66h ("read long sector"), tunnelled through \\.\vwin32.
// nDriveNumber is 1-based (1 = A:). The original leaves reg_EDI/reg_ESI as stack
// garbage; they are zeroed here instead.
BOOL hook_CDROM_IoctlReadSectorWin9x(void *pBuffer, int nDriveNumber)
{
    DIOC_REGISTERS regs = { 0 };

    DebugLog("[HNH:SUB] CDROM_IoctlReadSectorWin9x_485ADA\n");

    regs.reg_EBX = (DWORD)nDriveNumber;
    regs.reg_EDX = (DWORD)pBuffer;
    regs.reg_ECX = 0x0866;         // category 08h / minor 66h: read long sector
    regs.reg_EAX = 0x440D;         // Int 21h generic block-device IOCTL
    return CDROM_DeviceIoControlVWIN32(&regs) && (regs.reg_Flags & 0x8000) == 0;
}

int(*CDROM_GetDriveFileSystemType)(CHAR chDriveLetter)
= reinterpret_cast<int(*)(CHAR chDriveLetter)>(0x485A2A);

// Volume FS name -> code map (original string globals @0x4C7630..0x4C764C):
// FAT->1, FAT32->2, NTFS->3, CDFS->4, unknown->0, GetVolumeInformationA-fail->-1.
int hook_CDROM_GetDriveFileSystemType(CHAR chDriveLetter)
{
    static const struct { LPCSTR szName; int nCode; } s_map[] = {
        { "FAT", 1 }, { "FAT32", 2 }, { "NTFS", 3 }, { "CDFS", 4 },
    };
    CHAR szRootPath[4] = "A:\\";
    CHAR szFileSystem[64];
    int i;

    DebugLog("[HNH:SUB] CDROM_GetDriveFileSystemType_485A2A\n");

    szRootPath[0] = chDriveLetter;
    if (!GetVolumeInformationA(szRootPath, NULL, NULL, NULL, NULL, NULL,
                               szFileSystem, sizeof(szFileSystem)))
        return -1;
    for (i = 0; i < 4; ++i)
    {
        if (strcmp(szFileSystem, s_map[i].szName) == 0)
            return s_map[i].nCode;
    }
    return 0;
}

BOOL(__cdecl* CDROM_VerifyNT)(int nDriveIndex, const char* szExpectedLabel, size_t nLabelSize)
= reinterpret_cast<BOOL(__cdecl*)(int nDriveIndex, const char* szExpectedLabel, size_t nLabelSize)
>(0x4858D0);

// NT: compare the ISO9660 Primary Volume Descriptor's Volume Identifier
// (LBA 16, offset 0x28) against the expected label. Every error path returns TRUE.
BOOL __cdecl hook_CDROM_VerifyNT(int nDriveIndex /* 0 = 'A' */, const char *szExpectedLabel, size_t nLabelSize)
{
    CHAR szRootPath[4]   = "A:\\";
    CHAR szVolumePath[8] = "\\\\.\\A:";
    void *pSector;
    DWORD dwBytesPerSector, dwBytesRead;
    HANDLE hVolume;
    BOOL bResult;

    DebugLog("[HNH:SUB] CDROM_VerifyNT_4858D0\n");

    if (CDROM_GetDriveFileSystemType((CHAR)(nDriveIndex + 'A')) != 4)  // CDFS only
        return TRUE;
    szRootPath[0] = (CHAR)(nDriveIndex + 'A');
    if (!GetDiskFreeSpaceA(szRootPath, NULL, &dwBytesPerSector, NULL, NULL))
        return TRUE;

    szVolumePath[4] = (CHAR)(nDriveIndex + 'A');
    hVolume = CreateFileA(szVolumePath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                          OPEN_EXISTING, 0, NULL);
    if (hVolume == INVALID_HANDLE_VALUE)
        return TRUE;

    pSector = GlobalAlloc(GMEM_FIXED, dwBytesPerSector);
    if (!pSector)
    {
        CloseHandle(hVolume);      // original leaks the handle here
        return TRUE;
    }

    SetFilePointer(hVolume, 16 * dwBytesPerSector, NULL, FILE_BEGIN);  // LBA 16 = PVD
    bResult = ReadFile(hVolume, pSector, dwBytesPerSector, &dwBytesRead, NULL);
    CloseHandle(hVolume);
    if (!bResult || dwBytesRead != dwBytesPerSector)
    {
        GlobalFree(pSector);       // original leaks the buffer here
        return TRUE;
    }

    bResult = _memicmp((const char *)pSector + 0x28, szExpectedLabel, nLabelSize) == 0;
    GlobalFree(pSector);
    return bResult;
}

BOOL(__cdecl* CDROM_VerifyWin9x)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize)
= reinterpret_cast<BOOL(__cdecl*)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize)
>(0x485BA8);

// Win9x: raw sector read via the DOS ioctl above, then two label layouts in the
// 28-byte result:
//   pass 1: buf[7+2i] == label[i] (exact)     -- wide/UCS-2 layout, low bytes at odd offsets
//   pass 2: _memicmp(&buf[6], label, n) == 0  -- narrow layout at +6
BOOL __cdecl hook_CDROM_VerifyWin9x(int chDriveLetter, const char *szExpectedLabel, size_t nLabelSize)
{
    BYTE buf[28];
    BOOL bWideMatch;
    size_t i;

    DebugLog("[HNH:SUB] CDROM_VerifyWin9x_485BA8\n");

    memset(buf, 0, 25);            // original zeroes 25 of the 28 bytes
    if (!CDROM_IoctlReadSectorWin9x(buf, chDriveLetter - 'A' + 1))
        return TRUE;               // raw read unavailable: fail open

    bWideMatch = TRUE;
    for (i = 0; i < nLabelSize; ++i)
    {
        if (buf[7 + 2 * i] != (BYTE)szExpectedLabel[i])
        {
            bWideMatch = FALSE;
            break;
        }
    }
    return bWideMatch || _memicmp((const char *)&buf[6], szExpectedLabel, nLabelSize) == 0;
}

BOOL(__cdecl* CDROM_VerifyDiscSignature)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize)
= reinterpret_cast<BOOL(__cdecl*)(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize)
>(0x485C50);

// OS dispatcher. NOTE the convention mismatch is deliberate: Win9x gets the ASCII
// drive letter, NT a 0-based drive index ('A' = 0). Unknown platform -> "verified".
BOOL __cdecl hook_CDROM_VerifyDiscSignature(int chDriveLetter, const char* szExpectedLabel, size_t nLabelSize)
{
    DebugLog("[HNH:SUB] CDROM_VerifyDiscSignature_485C50\n");
#if 0
    OSVERSIONINFOA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(osvi);

    // Disable deprecation warnings specifically for this legacy detection logic
#pragma warning(suppress : 4996)
    GetVersionExA(&osvi);
    switch (osvi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_WINDOWS:  // 1
        return CDROM_VerifyWin9x(chDriveLetter, szExpectedLabel, nLabelSize);
    case VER_PLATFORM_WIN32_NT:       // 2
        return CDROM_VerifyNT(chDriveLetter - 'A', szExpectedLabel, nLabelSize);
    }
    return TRUE;               // fail open
#else
    if (IsWindowsXPOrGreater()) {
        return CDROM_VerifyNT(chDriveLetter - 'A', szExpectedLabel, nLabelSize);
    }
    return CDROM_VerifyWin9x(chDriveLetter, szExpectedLabel, nLabelSize);
#endif
}

void register_CDROM_VerifyDiscSignature_485C50() {
  // [OK] hnh_Hook(0x485C50, hook_CDROM_VerifyDiscSignature, "CDROM_VerifyDiscSignature_485C50");
  hnh_Hook(0x4858D0, hook_CDROM_VerifyNT, "CDROM_VerifyNT_4858D0"); // [OK]
  // [OK] hnh_Hook(0x485A2A, hook_CDROM_GetDriveFileSystemType, "CDROM_GetDriveFileSystemType_485A2A");
}