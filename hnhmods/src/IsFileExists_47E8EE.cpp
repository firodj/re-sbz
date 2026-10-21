
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

BOOL (__cdecl*IsFileExists_47E8EE)(CHAR* pszFilePath) = reinterpret_cast<BOOL (__cdecl*)(CHAR*)>(0x47E8EE);

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

void register_IsFileExists_47E8EE() {
    hnh_Hook(0x47E8EE, hook_IsFileExists_47E8EE, "IsFileExists_47E8EE");
}
