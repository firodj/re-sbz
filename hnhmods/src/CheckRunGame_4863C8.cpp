#include "dx7.hpp"
#include "console.hpp"
#include "struct.hpp"
#include "shared.hpp"

// ReadFileToBuffer(lpFileName, lpBuffer, nBufSize): open lpFileName read-only
// (GENERIC_READ/OPEN_EXISTING), cap the read at min(fileSize, nBufSize) to avoid
// overflow, ReadFile into lpBuffer; return bytes actually read (0 on open/read failure).
// Generic whole-file loader used for game assets/config.
DWORD __cdecl ReadFileToBuffer(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize)
{
  HANDLE hFile; // [esp+0h] [ebp-Ch]
  int nNumberOfBytesToRead; // [esp+4h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+8h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] ReadFileToBuffer_47E858");

  hFile = CreateFileA(lpFileName, 0x80000000, 0, 0, 3u, 0, 0);
  if ( !hFile )
    return 0;
  if ( hFile == INVALID_HANDLE_VALUE )
    return 0;
  nNumberOfBytesToRead = GetFileSize(hFile, 0);
  if ( nNumberOfBytesToRead && nNumberOfBytesToRead != -1 )
  {
    if ( nNumberOfBytesToRead > nBufSize )
      nNumberOfBytesToRead = nBufSize;
    ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, 0);
    CloseHandle(hFile);
    return NumberOfBytesRead;
  }
  else
  {
    CloseHandle(hFile);
    return 0;
  }
}

// WriteBufferToFile - Write a whole buffer to a text/config file (Win32)
// // 
// // Arguments:
// //   lpFileName:               destination path (ANSI)
// //   lpBuffer:                 data to write
// //   nNumberOfBytesToWrite:    byte count (e.g. 0x80 = 128-byte render-options block)
// // 
// // Behavior:
// //   1. CreateFileA (GENERIC_WRITE, no sharing, CREATE_ALWAYS)
// //   2. WriteFile + CloseHandle; no error checking on bytes written
// // 
// // Siblings: ReadFileToBuffer (read counterpart), IsFileExists_47E8EE
// // Callers:  CheckRunGame_4863C8 / SystemMenu_4655AB / SysMenu_ScenePage0_462135
// //           (persist fog/specular/AA/dither/light/shadow/resolution options),
// //           GameMainLoop_47F013 (on exit)
void __cdecl WriteBufferToFile(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
  HANDLE hFile; // [esp+0h] [ebp-8h]
  DWORD NumberOfBytesWritten; // [esp+4h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] WriteBufferToFile_47E7F2");

  hFile = CreateFileA(lpFileName, 0x40000000u, 0, 0, 2u, 0, 0);
  NumberOfBytesWritten = 0;
  if ( hFile )
  {
    if ( hFile != INVALID_HANDLE_VALUE )
    {
      WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, 0);
      CloseHandle(hFile);
    }
  }
}

uint32_t &g_bSpecularEnable_4B3474 = *(uint32_t *)0x4B3474; // WRITABLE
uint32_t &g_bAntiAliasEnable       = *(uint32_t *)0x4B347C; // WRITABLE
uint32_t &g_bDitherEnable          = *(uint32_t *)0x4B3478; // WRITABLE
uint32_t &g_bLightEnable_4B3484    = *(uint32_t *)0x4B3484; // WRITABLE
uint32_t &g_bShadowEnable_518CA8   = *(uint32_t *)0x518CA8; // WRITABLE
int &g_nResolutionMode_518C80      = *(int *)0x518C80; // WRITABLE
uint32_t &g_nColorDepthMode_518C9C = *(uint32_t *)0x518C9C; // WRITABLE
int &g_nRenderHALMode_518CA0       = *(int *)0x518CA0; // WRITABLE
uint32_t &g_nWidth2                = *(uint32_t *)0x518C28; // WRITABLE
uint32_t &g_fScreenScale_52F19C    = *(uint32_t *)0x52F19C; // WRITABLE

int (__cdecl *ShowTitleMenu_427DA8)(HWND hWnd, int bFullScreen) = (int (__cdecl*)(HWND, int))0x427DA8;

BOOL __cdecl CheckRunGame_4863C8(HWND hWnd, int bFullScreen)
{
  CHAR szTeaEx[256]; // [esp+0h] [ebp-204h] BYREF
  int bRunGame; // [esp+100h] [ebp-104h]
  _DWORD szTeaConfigData[64]; // [esp+104h] [ebp-100h] BYREF

  DebugLog("[HNH:SUB] CheckRunGame_4863C8");

  memset(szTeaConfigData, 0, 0xFEu);
  szTeaConfigData[0] = 1;
  szTeaConfigData[1] = 1;
  szTeaConfigData[2] = 0;
  szTeaConfigData[3] = 1;
  szTeaConfigData[4] = 1;
  memset(&szTeaConfigData[5], 0, 12);
  szTeaConfigData[8] = 1;
  wsprintfA(szTeaEx, "Tea.ex");
  CHanaObj__GetRealPath_486D9A(szTeaEx, 0x3E9);
  if ( IsFileExists_47E8EE(szTeaEx) )
    ReadFileToBuffer(szTeaEx, szTeaConfigData, 128);
  g_bFogIsActive_4B3480 = szTeaConfigData[0];
  g_bSpecularEnable_4B3474 = szTeaConfigData[1];
  g_bAntiAliasEnable = szTeaConfigData[2];
  g_bDitherEnable = szTeaConfigData[3];
  g_bLightEnable_4B3484 = szTeaConfigData[4];
  g_bShadowEnable_518CA8 = szTeaConfigData[5];
  g_nResolutionMode_518C80 = szTeaConfigData[6];
  g_nColorDepthMode_518C9C = szTeaConfigData[7];
  g_nRenderHALMode_518CA0 = 1;
  bRunGame = ShowTitleMenu_427DA8(hWnd, bFullScreen);
  if ( bRunGame > 0 )
  {
    g_fScreenScale_52F19C = g_nWidth2;
    szTeaConfigData[0] = g_bFogIsActive_4B3480;
    szTeaConfigData[1] = g_bSpecularEnable_4B3474;
    szTeaConfigData[2] = g_bAntiAliasEnable;
    szTeaConfigData[3] = g_bDitherEnable;
    szTeaConfigData[4] = g_bLightEnable_4B3484;
    szTeaConfigData[5] = g_bShadowEnable_518CA8;
    szTeaConfigData[6] = g_nResolutionMode_518C80;
    szTeaConfigData[7] = g_nColorDepthMode_518C9C;
    szTeaConfigData[8] = g_nRenderHALMode_518CA0;
    wsprintfA(szTeaEx, "");
    CHanaObj__GetRealPath_486D9A(szTeaEx, 0x3E9);
    if ( IsFileExists_47E8EE(szTeaEx) )
    {
      wsprintfA(szTeaEx, "Tea.ex");
      CHanaObj__GetRealPath_486D9A(szTeaEx, 0x3E9);
      WriteBufferToFile(szTeaEx, szTeaConfigData, 0x80u);
    }
  }
  return bRunGame;
}

void register_CheckRunGame_4863C8() {
  hnh_Hook(0x4863C8, CheckRunGame_4863C8, "CheckRunGame_4863C8");
  hnh_Hook(0x47E858, ReadFileToBuffer, "ReadFileToBuffer_47E858");
  hnh_Hook(0x47E7F2, WriteBufferToFile, "WriteBufferToFile_47E7F2");
}