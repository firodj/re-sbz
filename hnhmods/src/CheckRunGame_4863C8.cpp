#include "dx7.hpp"
#include "console.hpp"
#include "struct.hpp"
#include "shared.hpp"

struct CVideoModeDesc // sizeof=0x18
{
  int nMaxNumVideoModes;
  D3DX_VIDMODEDESC *pModeDesc;
  D3DDEVICEDESC7 *pD3DCaps;
  DDCAPS *pDDHALCaps;
  DDCAPS *pDDHELCaps;
  BOOL *parrbActive;
};

struct CDisplayConfig // sizeof=0x10
{
  unsigned int nDeviceCount;
  int nDeviceIdxHal;
  D3DX_DEVICEDESC *parrD3DXDevDesc;
  CVideoModeDesc *parrVidModeDesc;
};

int &g_bPreferTnLHal_4B33E8 = *(int*)0x4B33E8; // once!

void DisplayConfig_ProcessDriverDesc(CDisplayConfig *pConfig)
{
  DebugLog("[HNH:SUB] DisplayConfig_ProcessDriverDesc_427F8C\n");

  pConfig->nDeviceIdxHal = 0;                          // default device index = 0

  for (unsigned int i = 0; i < pConfig->nDeviceCount; i++)
  {
    D3DX_DEVICEDESC *dev = &pConfig->parrD3DXDevDesc[i];
    char friendly[256] = {0};                        // "Destination"

    // ---- Step 1: bounded scan for '\\' (max 256 chars) ----
    size_t prefixLen = 0;
    while (prefixLen < 256 && dev->driverDesc[prefixLen] != '\\')
      prefixLen++;
    bool found = (prefixLen < 256);

    // ---- Step 2: classify tail (only if separator found) ----
    if (found)
    {
      strncpy(friendly, dev->driverDesc, prefixLen);   // text before '\\'
      const char *tail = &dev->driverDesc[prefixLen + 2]; // skip '\\' + 1 char

      if (!strcmp(tail, "...Transform and Lighting...")) {
        strcat(friendly, "/ Direct3D T&L HAL");
        if (g_bPreferTnLHal_4B33E8)                 // static flag = 1
          pConfig->nDeviceIdxHal = i;
      }
      else if (!strcmp(tail, "...through Direct3D HAL")) {
        strcat(friendly, "/ Direct3D HAL");
        pConfig->nDeviceIdxHal = i;                 // HAL always wins so far
      }
      else if (!strcmp(tail, "...RGB Software Emulation"))
        strcat(friendly, "/ RGB Software Emulation");   // default idx untouched
      else if (!strcmp(tail, "...Reference Rasterizer"))
        strcat(friendly, "/ D3D Refence Rasterizer");   // default idx untouched
      // unknown tail ¨ friendly keeps just the prefix, no tag
    }

    // ---- Step 3: rewrite in place (ALWAYS runs, this was LABEL_17) ----
    memset(dev->driverDesc, 0, sizeof(dev->driverDesc));
    strcpy(dev->driverDesc, friendly);
  }
}

struct CVideoModeWhiteItem
{
  int Width;
  int Height;
  int bpp;
};

/*
A null-terminated table of (width, height, bpp) triples -- the game's officially supported fullscreen modes:

+-----+-------+--------+-----+
|  #  | Width | Height | bpp |
+-----+-------+--------+-----+
| 1   | 640   | 480    | 16  |
+-----+-------+--------+-----+
| 2   | 640   | 480    | 32  |
+-----+-------+--------+-----+
| 3   | 800   | 600    | 16  |
+-----+-------+--------+-----+
| 4   | 800   | 600    | 32  |
+-----+-------+--------+-----+
| 5   | 1024  | 768    | 16  |
+-----+-------+--------+-----+
| 6   | 1024  | 768    | 32  |
+-----+-------+--------+-----+
| 7   | 0     | 0      | 0   |
*/
CVideoModeWhiteItem(&g_arrModeWhitelist_4B33F0)[7] = *(CVideoModeWhiteItem(*)[7])0x4B33F0; // once!

BOOL __cdecl IsVideoModeSupported(D3DX_VIDMODEDESC *pVidModeDesc)
{
  int index; // [esp+0h] [ebp-4h]

  DebugLog("[HNH:SUB] IsVideoModeSupported_427C51\n");

  index = 0;
  do
  {
    if ( pVidModeDesc->width  == g_arrModeWhitelist_4B33F0[index].Width
      && pVidModeDesc->height == g_arrModeWhitelist_4B33F0[index].Height
      && pVidModeDesc->bpp    == g_arrModeWhitelist_4B33F0[index].bpp )
    {
      return TRUE;
    }
    ++index;
  }
  while ( g_arrModeWhitelist_4B33F0[index].Width );
  return FALSE;
}

HRESULT (*D3DXInitialize)(void) = (HRESULT(*)(void))0x48B80B; 
DWORD (*D3DXGetDeviceCount)(void) = (DWORD(*)(void))0x48B9DD;
DWORD (*D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags) = (DWORD (*)(DWORD, DWORD))0x48BBF0; 
HRESULT (*D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc) = (HRESULT(*)(DWORD, D3DX_DEVICEDESC*))0x48BA9E; 
HRESULT (*D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList) = (HRESULT (*)(DWORD, DWORD, DWORD, D3DX_VIDMODEDESC*))0x48BCDE;
HRESULT (*D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps) = (HRESULT (*)(DWORD, D3DX_VIDMODEDESC*, D3DDEVICEDESC7*, DDCAPS*, DDCAPS*))0x48C28D; 

int (&g_arrZBufDepthCountPerDev_518C40)[16] = *(int(*)[16])0x518C40;
int (&g_arrZBufDepthsPerDev_518828)[256] = *(int(*)[256])0x518828;

void __cdecl DisplayConfig_Init(CDisplayConfig *pDisplayConfig)
{
  DDCAPS *pDDHALCaps; // [esp+0h] [ebp-38h]
  DDCAPS *pDDHELCaps; // [esp+4h] [ebp-34h]
  int nModeBitDepth; // [esp+8h] [ebp-30h]
  unsigned int iDevice; // [esp+Ch] [ebp-2Ch]
  CVideoModeDesc *pVidModeDesc; // [esp+14h] [ebp-24h]
  BOOL *pbActive; // [esp+18h] [ebp-20h]
  unsigned int nMaxNumVideoModes; // [esp+1Ch] [ebp-1Ch]
  D3DX_DEVICEDESC *pD3DXDevDesc; // [esp+20h] [ebp-18h]
  unsigned int jVideoMode; // [esp+24h] [ebp-14h]
  unsigned int nDeviceCount; // [esp+28h] [ebp-10h]
  D3DDEVICEDESC7 *pD3DCaps; // [esp+2Ch] [ebp-Ch]
  DWORD nZBufBitDepth; // [esp+30h] [ebp-8h]
  D3DX_VIDMODEDESC *pModeDesc; // [esp+34h] [ebp-4h]

  DebugLog("[HNH:SUB] DisplayConfig_Init_4278FF");

  memset(g_arrZBufDepthCountPerDev_518C40, 0, sizeof(g_arrZBufDepthCountPerDev_518C40));
  memset(g_arrZBufDepthsPerDev_518828, 0, sizeof(g_arrZBufDepthsPerDev_518828));
  D3DXInitialize();
  nDeviceCount = D3DXGetDeviceCount();
  pDisplayConfig->nDeviceCount = nDeviceCount;
  pDisplayConfig->parrD3DXDevDesc = (D3DX_DEVICEDESC*) GAlloc80_40DEF7(sizeof(D3DX_DEVICEDESC) * nDeviceCount);
  pD3DXDevDesc = pDisplayConfig->parrD3DXDevDesc;
  pDisplayConfig->parrVidModeDesc = (CVideoModeDesc*) GAlloc80_40DEF7(sizeof(CVideoModeDesc) * nDeviceCount);
  pVidModeDesc = pDisplayConfig->parrVidModeDesc;
  for ( iDevice = 0; iDevice < nDeviceCount; ++iDevice )
  {
    D3DXGetDeviceDescription(iDevice, pD3DXDevDesc);
    nMaxNumVideoModes = D3DXGetMaxNumVideoModes(iDevice, 0);
    pVidModeDesc->nMaxNumVideoModes = nMaxNumVideoModes;
    pVidModeDesc->pModeDesc = (D3DX_VIDMODEDESC*) GAlloc80_40DEF7(16 * nMaxNumVideoModes);
    pVidModeDesc->pD3DCaps  = (D3DDEVICEDESC7*) GAlloc80_40DEF7(236 * nMaxNumVideoModes);
    pVidModeDesc->pDDHALCaps = (DDCAPS*) GAlloc80_40DEF7(sizeof(DDCAPS) * nMaxNumVideoModes);
    pVidModeDesc->pDDHELCaps = (DDCAPS*) GAlloc80_40DEF7(sizeof(DDCAPS) * nMaxNumVideoModes);
    pVidModeDesc->parrbActive = (BOOL *) GAlloc80_40DEF7(4 * nMaxNumVideoModes);
    pModeDesc = pVidModeDesc->pModeDesc;
    pD3DCaps = pVidModeDesc->pD3DCaps;
    pDDHALCaps = pVidModeDesc->pDDHALCaps;
    pDDHELCaps = pVidModeDesc->pDDHELCaps;
    pbActive = pVidModeDesc->parrbActive;
    for ( jVideoMode = 0; jVideoMode < nMaxNumVideoModes; ++jVideoMode )
    {
      D3DXGetVideoMode(iDevice, 0, jVideoMode, pModeDesc);
      *pbActive = FALSE;
      if ( IsVideoModeSupported(pModeDesc) == 1 )
      {
        pDDHALCaps->dwSize = 380;
        pDDHELCaps->dwSize = 380;
        D3DXGetDeviceCaps(iDevice, pModeDesc, pD3DCaps, pDDHALCaps, pDDHELCaps);
        nZBufBitDepth = pD3DCaps->dwDeviceZBufferBitDepth;
        switch ( pModeDesc->bpp )
        {
        case 0x10u:
          nModeBitDepth = 0x400;
          break;
        case 0x18u:
          nModeBitDepth = 0x200;
          break;
        case 0x20u:
          nModeBitDepth = 0x100;
          break;
        }
        if ( (nModeBitDepth & pD3DCaps->dwDeviceRenderBitDepth) != 0 )
          *pbActive = TRUE;
      }
      ++pModeDesc;
      ++pD3DCaps;
      ++pDDHALCaps;
      ++pDDHELCaps;
      ++pbActive;
    }
    if ( (nZBufBitDepth & 0x400) != 0 )
      g_arrZBufDepthsPerDev_518828[16 * iDevice + g_arrZBufDepthCountPerDev_518C40[iDevice]++] = 16;
    if ( (nZBufBitDepth & 0x200) != 0 )
      g_arrZBufDepthsPerDev_518828[16 * iDevice + g_arrZBufDepthCountPerDev_518C40[iDevice]++] = 24;
    if ( (nZBufBitDepth & 0x100) != 0 )
      g_arrZBufDepthsPerDev_518828[16 * iDevice + g_arrZBufDepthCountPerDev_518C40[iDevice]++] = 32;
    ++pD3DXDevDesc;
    ++pVidModeDesc;
  }
  DisplayConfig_ProcessDriverDesc(pDisplayConfig);
}

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
//    
//    Arguments:
//      lpFileName:               destination path (ANSI)
//      lpBuffer:                 data to write
//      nNumberOfBytesToWrite:    byte count (e.g. 0x80 = 128-byte render-options block)
//    
//    Behavior:
//      1. CreateFileA (GENERIC_WRITE, no sharing, CREATE_ALWAYS)
//      2. WriteFile + CloseHandle; no error checking on bytes written
//    
//    Siblings: ReadFileToBuffer (read counterpart), IsFileExists_47E8EE
//    Callers:  CheckRunGame_4863C8 / SystemMenu_4655AB / SysMenu_ScenePage0_462135
//              (persist fog/specular/AA/dither/light/shadow/resolution options),
//              GameMainLoop_47F013 (on exit)
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
int      &g_nResolutionMode_518C80      = *(int *)0x518C80; // WRITABLE
uint32_t &g_nColorDepthMode_518C9C = *(uint32_t *)0x518C9C; // WRITABLE
int      &g_nRenderHALMode_518CA0       = *(int *)0x518CA0; // WRITABLE
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
  hnh_Hook(0x4278FF, DisplayConfig_Init, "DisplayConfig_Init_4278FF");
  hnh_Hook(0x427F8C, DisplayConfig_ProcessDriverDesc, "DisplayConfig_ProcessDriverDesc_427F8C");
  hnh_Hook(0x427C51, IsVideoModeSupported, "IsVideoModeSupported_427C51");
}