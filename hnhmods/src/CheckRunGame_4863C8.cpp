#include "dx7.hpp"
#include "console.hpp"
#include "struct.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===

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

int &g_bPreferTnLHal_4B33E8 = *(int*)0x4B33E8; // once!

uint32_t   &g_dwRGBBitCount_4D6D00 = *(uint32_t *)0x4D6D00; // WRITABLE
D3DXMATRIX &g_mat4x4View_4D4780    = *(D3DXMATRIX *)0x4D4780; // WRITABLE

float &g_fProjZFar   = *(float *)0x4B00C0; // WRITABLE
float &g_fProjZNear  = *(float *)0x4B00BC; // WRITABLE
float &g_fProjAspect = *(float *)0x4B00B8; // WRITABLE
float &g_fProjFov    = *(float *)0x4B00C4; // WRITABLE
D3DXMATRIX  &g_mat4x4Projection_4CE870 = *(D3DXMATRIX *)0x4CE870; // WRITABLE
D3DXVECTOR3 &g_vec3AxisX_4CE8E8 = *(D3DXVECTOR3 *)0x4CE8E8; // WRITABLE
D3DXVECTOR3 &g_vec3AxisY_4CE8F8 = *(D3DXVECTOR3 *)0x4CE8F8; // WRITABLE
D3DXVECTOR3 &g_vec3AxisZ_4CE908 = *(D3DXVECTOR3 *)0x4CE908; // WRITABLE
D3DXVECTOR3 (&g_arrFwdByYaw360_4CD788)[360]   = *(D3DXVECTOR3 (*)[360])0x4CD788; // WRITABLE
D3DXVECTOR3 (&g_arrUpByPitch360_4D47C0)[360]  = *(D3DXVECTOR3 (*)[360])0x4D47C0; // WRITABLE
D3DXVECTOR3 (&g_arrFwdByPitch360_4D58A0)[360] = *(D3DXVECTOR3 (*)[360])0x4D58A0; // WRITABLE
uint32_t &g_bAlphaBlendEnabled_4D6CC0    = *(uint32_t *)0x4D6CC0; // WRITABLE
uint32_t &g_nAlphaOpState_4D6CC4         = *(uint32_t *)0x4D6CC4; // WRITABLE
BOOL     &g_bSpecularEnable              = *(BOOL*)0x4B00A0; // WRITABLE
BOOL     &g_bSpecularEnableMirror_4B00A4 = *(BOOL*)0x4B00A4; // WRITABLE
int      &g_nObjectTableCapacity_4B009C  = *(int*)0x4B009C;
uint32_t &g_nLightningState              = *(uint32_t *)0x4B00A8; // WRITABLE

int (&g_arrZBufDepthCountPerDev_518C40)[16] = *(int(*)[16])0x518C40;
int (&g_arrZBufDepthsPerDev_518828)[256]    = *(int(*)[256])0x518828;


HINSTANCE &g_hInstance_518CB0 = *(HINSTANCE*)0x518CB0;
HWND      &g_hWnd             = *(HWND*)0x518CAC;
BOOL      &g_bRunGame         = *(BOOL*)0x4B3488;

WNDPROC WndProcTitleMenu_428C11 = (WNDPROC)0x428C11;


BOOL     &g_bSpecularEnable_4B3474 = *(BOOL*)0x4B3474;
uint32_t &g_bAntiAliasEnable       = *(uint32_t *)0x4B347C; // WRITABLE
uint32_t &g_bDitherEnable          = *(uint32_t *)0x4B3478; // WRITABLE
uint32_t &g_bLightEnable_4B3484    = *(uint32_t *)0x4B3484; // WRITABLE
uint32_t &g_bShadowEnable_518CA8   = *(uint32_t *)0x518CA8; // WRITABLE
int      &g_nResolutionMode_518C80      = *(int *)0x518C80; // WRITABLE
uint32_t &g_nColorDepthMode_518C9C = *(uint32_t *)0x518C9C; // WRITABLE
int      &g_nRenderHALMode_518CA0       = *(int *)0x518CA0; // WRITABLE
uint32_t &g_nWidth2                = *(uint32_t *)0x518C28; // WRITABLE
uint32_t &g_fScreenScale_52F19C    = *(uint32_t *)0x52F19C; // WRITABLE

// === GLOBAL FUNCTIONS ===

void (* DisplayConfig_ProcessDriverDesc)(CDisplayConfig *pConfig) = reinterpret_cast<void(*)(CDisplayConfig *pConfig)>(0x427F8C);
BOOL (__cdecl * IsVideoModeSupported)(D3DX_VIDMODEDESC *pVidModeDesc) = reinterpret_cast<BOOL(__cdecl*)(D3DX_VIDMODEDESC *pVidModeDesc)>(0x427C51);
void (__cdecl * DisplayConfig_Init)(CDisplayConfig *pDisplayConfig) = reinterpret_cast<void(__cdecl*)(CDisplayConfig *pDisplayConfig)>(0x4278FF);
DWORD (__cdecl * ReadFileToBuffer)(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize) = reinterpret_cast<DWORD(__cdecl*)(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize)>(0x47E858);
void (__cdecl * WriteBufferToFile)(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite) = reinterpret_cast<void(__cdecl*)(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)>(0x47E7F2);
int (__cdecl * ShowTitleMenuWnd_428927)(HINSTANCE hInstance) = reinterpret_cast<int(__cdecl*)(HINSTANCE hInstance)>(0x428927);
BOOL (__cdecl * CheckRunGame_4863C8)(HWND hWnd, BOOL bFullScreen) = reinterpret_cast<BOOL(__cdecl*)(HWND hWnd, BOOL bFullScreen)>(0x4863C8);
HRESULT (__cdecl * SetLightingState)(DWORD dwEnable) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwEnable)>(0x40DD71);
HRESULT (* D3DInitBuffers_40AF60)() = reinterpret_cast<HRESULT(*)()>(0x40AF60);
void (__cdecl * CHanaObj__NewLightT3EF)(CHanaLightT3EF_S1A0 **pOut, D3DLIGHT7 *pD3Dlight) = reinterpret_cast<void(__cdecl*)(CHanaLightT3EF_S1A0 **pOut, D3DLIGHT7 *pD3Dlight)>(0x408F00);
void (__cdecl * CHanaObj__AddObjItemRef_40B848)(LPCSTR szName, CHanaBase_S130 *pObj) = reinterpret_cast<void(__cdecl*)(LPCSTR szName, CHanaBase_S130 *pObj)>(0x40B848);
void (__cdecl * AttachHanaObj_40BAB8)(CHanaBase_S130 *pParent, LPCSTR szName) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130 *pParent, LPCSTR szName)>(0x40BAB8);
D3DXMATRIX * (__cdecl * MatrixRotateCubeMapFace)(CHanaFrameT3EC_S354 *pFrame, int mode, D3DXVECTOR3 *pV, float angle) =
  reinterpret_cast<D3DXMATRIX *(__cdecl*)(CHanaFrameT3EC_S354 *pFrame, int mode, D3DXVECTOR3 *pV, float angle)>(0x4070AB);
void (__cdecl * CalculateLocalRotationMatrix)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pSceneFrame, float *pXpos, float *pYpos, float *pZpos, float *pXYrot, float *pYXrot, float *pZXrot) =
  reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pSceneFrame, float *pXpos, float *pYpos, float *pZpos, float *pXYrot, float *pYXrot, float *pZXrot)>(0x407047);

// === FUNCTION IMPLEMENTATIONS ===

void hook_DisplayConfig_ProcessDriverDesc(CDisplayConfig *pConfig)
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
      // unknown tail → friendly keeps just the prefix, no tag
    }

    // ---- Step 3: rewrite in place (ALWAYS runs, this was LABEL_17) ----
    memset(dev->driverDesc, 0, sizeof(dev->driverDesc));
    strcpy(dev->driverDesc, friendly);
  }
}

BOOL __cdecl hook_IsVideoModeSupported(D3DX_VIDMODEDESC *pVidModeDesc)
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

void __cdecl hook_DisplayConfig_Init(CDisplayConfig *pDisplayConfig)
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

  DebugLog("[HNH:SUB] DisplayConfig_Init_4278FF\n");

  memset(g_arrZBufDepthCountPerDev_518C40, 0, sizeof(g_arrZBufDepthCountPerDev_518C40));
  memset(g_arrZBufDepthsPerDev_518828, 0, sizeof(g_arrZBufDepthsPerDev_518828));
  D3DXInitialize();
  nDeviceCount = D3DXGetDeviceCount();
  DebugLog("{check} nDeviceCount = %d\n", nDeviceCount);

  pDisplayConfig->nDeviceCount = nDeviceCount;
  pDisplayConfig->parrD3DXDevDesc = (D3DX_DEVICEDESC*) MemAlloc_40DEF7(sizeof(D3DX_DEVICEDESC) * nDeviceCount);
  pD3DXDevDesc = pDisplayConfig->parrD3DXDevDesc;
  pDisplayConfig->parrVidModeDesc = (CVideoModeDesc*) MemAlloc_40DEF7(sizeof(CVideoModeDesc) * nDeviceCount);
  pVidModeDesc = pDisplayConfig->parrVidModeDesc;
  for ( iDevice = 0; iDevice < nDeviceCount; ++iDevice )
  {
    D3DXGetDeviceDescription(iDevice, pD3DXDevDesc);
    nMaxNumVideoModes = D3DXGetMaxNumVideoModes(iDevice, 0);
    DebugLog("{check} %i nMaxNumVideoModes = %d\n", nMaxNumVideoModes);

    pVidModeDesc->nMaxNumVideoModes = nMaxNumVideoModes;
    pVidModeDesc->pModeDesc = (D3DX_VIDMODEDESC*) MemAlloc_40DEF7(sizeof(D3DX_VIDMODEDESC) * nMaxNumVideoModes);
    pVidModeDesc->pD3DCaps  = (D3DDEVICEDESC7*) MemAlloc_40DEF7(sizeof(D3DDEVICEDESC7) * nMaxNumVideoModes);
    pVidModeDesc->pDDHALCaps = (DDCAPS*) MemAlloc_40DEF7(sizeof(DDCAPS) * nMaxNumVideoModes);
    pVidModeDesc->pDDHELCaps = (DDCAPS*) MemAlloc_40DEF7(sizeof(DDCAPS) * nMaxNumVideoModes);
    pVidModeDesc->parrbActive = (BOOL *) MemAlloc_40DEF7(4 * nMaxNumVideoModes);
    pModeDesc = pVidModeDesc->pModeDesc;
    pD3DCaps = pVidModeDesc->pD3DCaps;
    pDDHALCaps = pVidModeDesc->pDDHALCaps;
    pDDHELCaps = pVidModeDesc->pDDHELCaps;
    pbActive = pVidModeDesc->parrbActive;
    for ( jVideoMode = 0; jVideoMode < nMaxNumVideoModes; ++jVideoMode )
    {
      DebugLog("{check} VideoMode %d\n", jVideoMode);
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
DWORD __cdecl hook_ReadFileToBuffer(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize)
{
  HANDLE hFile; // [esp+0h] [ebp-Ch]
  int nNumberOfBytesToRead; // [esp+4h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+8h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] ReadFileToBuffer_47E858\n");

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
void __cdecl hook_WriteBufferToFile(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
  HANDLE hFile; // [esp+0h] [ebp-8h]
  DWORD NumberOfBytesWritten; // [esp+4h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] WriteBufferToFile_47E7F2\n");

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

int __cdecl hook_ShowTitleMenuWnd_428927(HINSTANCE hInstance)
{
  HWND DesktopWindow; // eax
  tagMSG Msg; // [esp+0h] [ebp-58h] BYREF
  int v4; // [esp+1Ch] [ebp-3Ch]
  WNDCLASSA WndClass; // [esp+20h] [ebp-38h] BYREF
  struct tagRECT Rect; // [esp+48h] [ebp-10h] BYREF

  DebugLog("[HNH:SUB] ShowTitleMenuWnd_428927\n");

  v4 = 0;
  DesktopWindow = GetDesktopWindow();
  GetWindowRect(DesktopWindow, &Rect);
  g_hInstance_518CB0 = hInstance;
  WndClass.style = 0;
  WndClass.lpfnWndProc = WndProcTitleMenu_428C11;
  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hInstance = hInstance;
  WndClass.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(0x69));
  WndClass.hCursor = LoadCursorA(hInstance, MAKEINTRESOURCEA(0x68));
  WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  WndClass.lpszMenuName = 0;
  WndClass.lpszClassName = " メニュー ";
  if ( RegisterClassA(&WndClass) )
  {
    g_hWnd = CreateWindowExA(
      0,
      " メニュー ",
      " メニュー ",
      WS_POPUP,
      0,
      0,
      Rect.right - Rect.left,
      Rect.bottom - Rect.top,
      0,
      0,
      hInstance,
      0);
    if ( g_hWnd )
    {
      ShowWindow(g_hWnd, 5);
      UpdateWindow(g_hWnd);
      while ( 1 )
      {
        while ( !PeekMessageA(&Msg, 0, 0, 0, 1u) )
          ;
        if ( Msg.message == WM_QUIT )
          break;
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
      }
      return g_bRunGame;
    }
    else
    {
      v4 = -1;
      return -1;
    }
  }
  else
  {
    v4 = -1;
    return -1;
  }
}

//LPVOID pfnCheckRunGame_4863C8 = NULL;

BOOL __cdecl hook_CheckRunGame_4863C8(HWND hWnd, BOOL bFullScreen)
{
  //auto fn = (BOOL(__cdecl*)(HWND, BOOL))pfnCheckRunGame_4863C8;
  // return fn(hWnd, bFullScreen);

  CHAR szTeaEx[256]; // [esp+0h] [ebp-204h] BYREF
  int bRunGame; // [esp+100h] [ebp-104h]
  _DWORD szTeaConfigData[64]; // [esp+104h] [ebp-100h] BYREF

  DebugLog("[HNH:SUB] CheckRunGame_4863C8\n");

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

HRESULT __cdecl hook_SetLightingState(DWORD dwEnable)
{
  DebugLog("[HNH:SUB] %s\n", "SetLightingState_40DD71");

  g_nLightningState = dwEnable;
  g_nLightningEnabled = dwEnable;
  return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, dwEnable);
}

HRESULT hook_D3DInitBuffers_40AF60()
{
  float radStep;
  float anglea;
  D3DXVECTOR3 pUp;
  D3DLIGHT7 d3dInitLight;
  D3DXVECTOR3 pEye;
  int iAngle;
  int hr;
  DDPIXELFORMAT ddPixelFormat;
  float upAxisX;
  float upAxisY;
  float upAxisZ;
  D3DXVECTOR3 pAt;
  D3DMATERIAL7 d3dDefaultMaterial;

  DebugLog("[HNH:SUB] %s\n", "D3DInitBuffers_40AF60");

  // --- 1. Guard Clauses (Early Exits for Pointers & Init Flags) ---
  if (!g_bInited_4D6C6C) 
    return E_FAIL;

  g_pD3DDevice_4D6C78 = g_pRenderCtx->GetD3DDevice();
  if (!g_pD3DDevice_4D6C78) 
    return E_FAIL;
  g_pD3DDevice_4D6C78->GetCaps(&g_d3dDevDesc_4D6980);

  g_pDD_4D6C7C = g_pRenderCtx->GetDD();
  if (!g_pDD_4D6C7C) 
    return E_FAIL;

  g_pD3D_4D6C84 = g_pRenderCtx->GetD3D();
  if (!g_pD3D_4D6C84) 
    return E_FAIL;

  g_pBackBuffer_4D6C90 = g_pRenderCtx->GetBackBuffer(0);
  if (!g_pBackBuffer_4D6C90) 
    return E_FAIL;

  g_pPrimaryBuffer_4D6C88 = g_pRenderCtx->GetPrimary();
  if (!g_pPrimaryBuffer_4D6C88) 
    return E_FAIL;

  g_pZBuffer_4D6C94 = g_pRenderCtx->GetZBuffer();
  if (!g_pZBuffer_4D6C94) 
    return E_FAIL;

  // --- 2. Setup Pixel Format & Scene Objects ---
  ddPixelFormat.dwSize = 32;
  g_pPrimaryBuffer_4D6C88->GetPixelFormat(&ddPixelFormat);
  g_dwRGBBitCount_4D6D00 = ddPixelFormat.dwRGBBitCount;

  g_pD3DDevice_4D6C78->GetMaterial(&d3dDefaultMaterial);
  CHanaObj__NewMateT3EE_S184(&g_pMaterial);
  ApplyMaterial(g_pMaterial, &d3dDefaultMaterial);

  CHanaObj__NewFrameRootT3EC_S2D4(&g_pSceneFrame);
  AttachHanaObj_40BAB8(g_pSceneFrame, "SceneFrame");

  CHanaObj__NewFrameT3EC_S354((CHanaFrameT3EC_S354*)g_pSceneFrame, &g_pCameraFrame);
  AttachHanaObj_40BAB8(g_pCameraFrame, "CameraFrame");

  CHanaObj__NewFrameT3EC_S354((CHanaFrameT3EC_S354*)g_pSceneFrame, &g_pLightFrame);
  AttachHanaObj_40BAB8(g_pLightFrame, "LightFrame");

  // --- 3. View & Projection Transformations ---
  memset(&pEye, 0, sizeof(pEye));
  pAt.x = 0.0f; pAt.y = 0.0f; pAt.z = 1.0f;
  pUp.x = 0.0f; pUp.y = 1.0f; pUp.z = 0.0f;

  MatrixLookAtLH(&g_mat4x4View_4D4780, &pEye, &pAt, &pUp);
  g_pD3DDevice_4D6C78->SetTransform(D3DTRANSFORMSTATE_VIEW, reinterpret_cast<LPD3DMATRIX>(&g_mat4x4View_4D4780));

  MatrixPerspectiveFovLH(&g_mat4x4Projection_4CE870, g_fProjFov, g_fProjAspect, g_fProjZNear, g_fProjZFar);
  g_pD3DDevice_4D6C78->SetTransform(D3DTRANSFORMSTATE_PROJECTION, reinterpret_cast<LPD3DMATRIX>(&g_mat4x4Projection_4CE870));

  g_vec3AxisX_4CE8E8 = D3DXVECTOR3{1.0f, 0.0f, 0.0f};
  g_vec3AxisY_4CE8F8 = D3DXVECTOR3{0.0f, 1.0f, 0.0f};
  g_vec3AxisZ_4CE908 = D3DXVECTOR3{0.0f, 0.0f, 1.0f};

  // --- 4. Yaw & Pitch Loops ---
  g_arrFwdByYaw360_4CD788[0] = D3DXVECTOR3{0.0f, 0.0f, 1.0f};
  for (iAngle = 1; iAngle < 360; ++iAngle)
  {
    radStep = static_cast<float>(DegreesToRadians(1.0f));
    MatrixRotateCubeMapFace(g_pCameraFrame, 2, &g_vec3AxisY_4CE8F8, radStep);
    CalculateLocalRotationMatrix(
      g_pCameraFrame, (CHanaFrameT3EC_S354*)g_pSceneFrame,
      &g_arrFwdByYaw360_4CD788[iAngle].x,
      &g_arrFwdByYaw360_4CD788[iAngle].y,
      &g_arrFwdByYaw360_4CD788[iAngle].z,
      &upAxisX, &upAxisY, &upAxisZ);
  }

  g_arrUpByPitch360_4D47C0[0]  = D3DXVECTOR3{0.0f, 1.0f, 0.0f};
  g_arrFwdByPitch360_4D58A0[0] = D3DXVECTOR3{0.0f, 0.0f, 1.0f};
  for (iAngle = 1; iAngle < 360; ++iAngle)
  {
    anglea = static_cast<float>(DegreesToRadians(1.0f));
    MatrixRotateCubeMapFace(g_pCameraFrame, 2, &g_vec3AxisX_4CE8E8, anglea);
    CalculateLocalRotationMatrix(
      g_pCameraFrame, (CHanaFrameT3EC_S354*)g_pSceneFrame,
      &g_arrFwdByPitch360_4D58A0[iAngle].x,
      &g_arrFwdByPitch360_4D58A0[iAngle].y,
      &g_arrFwdByPitch360_4D58A0[iAngle].z,
      &g_arrUpByPitch360_4D47C0[iAngle].x,
      &g_arrUpByPitch360_4D47C0[iAngle].y,
      &g_arrUpByPitch360_4D47C0[iAngle].z);
  }

  // --- 5. Flattened Render State & HRESULT Checks ---
  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);
  if (hr < 0) return hr;

  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_CLIPPING, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 2);

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
  if (hr < 0) return hr;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 1);
  if (hr < 0) return hr;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_CULLMODE, 3);
  if (hr < 0) return hr;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  if (hr < 0) return hr;

  g_bAlphaBlendEnabled_4D6CC0 = 1;
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAOP, 4);
  g_nAlphaOpState_4D6CC4 = 0;

  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);

  hr = g_pD3DDevice_4D6C78->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, 1);
  if (hr < 0) return hr;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_COLORKEYBLENDENABLE, 1);
  if (hr < 0) return hr;

  SetLightingState(1u);
  memset(&d3dInitLight, 0, sizeof(d3dInitLight));
  d3dInitLight.dltType = (D3DLIGHTTYPE)-1;
  d3dInitLight.dcvDiffuse.r = 0.5f;
  d3dInitLight.dcvDiffuse.g = 0.5f;
  d3dInitLight.dcvDiffuse.b = 0.5f;
  CHanaObj__NewLightT3EF(&g_pLight_4D6CE0, &d3dInitLight);

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_COLORVERTEX, 1);
  if (hr < 0) return hr;

  g_bSpecularEnable = 1;
  g_bSpecularEnableMirror_4B00A4 = 1;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, 1);
  if (hr < 0) return hr;

  hr = g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SPECULARMATERIALSOURCE, 0);
  if (hr < 0) return hr;

  hr = g_pRenderCtx->SetClearColor(0);
  if ( hr < 0 )
    return hr;
  hr = g_pRenderCtx->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);
  if ( hr >= 0 )
    return S_OK;
  else
    return hr;
}

void __cdecl hook_CHanaObj__NewLightT3EF(CHanaLightT3EF_S1A0 **pOut, D3DLIGHT7 *pD3Dlight)
{
  DebugLog("[HNH:SUB] %s\n", "CHanaObj__NewLightT3EF_408F00");

  // Allocate and initialize the light object container
  CHanaLightT3EF_S1A0 *newLight = (CHanaLightT3EF_S1A0 *)MemAlloc_40DEF7(0x1A0u);
  memset(newLight, 0, sizeof(CHanaLightT3EF_S1A0));
  newLight->dwType = 0x3EF;

  // Find and allocate an available light index from the 64-bit bitmask
  int allocatedIndex = 0;
  for (int i = 0; i < 64; ++i)
  {
    if (((1ULL << i) & g_bitsLightIndexes_4D6CB8) == 0)
    {
      g_bitsLightIndexes_4D6CB8 |= (1ULL << i);
      allocatedIndex = i;
      break;
    }
  }
  newLight->dwLightIndex_130 = allocatedIndex;

  // Copy user light settings or apply default directional light parameters
  if (pD3Dlight)
  {
    qmemcpy(&newLight->d3dLight_134, pD3Dlight, sizeof(newLight->d3dLight_134));
  }
  else
  {
    newLight->d3dLight_134.dltType = D3DLIGHT_DIRECTIONAL;
    newLight->d3dLight_134.dcvDiffuse.r = 1.0f;
    newLight->d3dLight_134.dcvDiffuse.g = 1.0f;
    newLight->d3dLight_134.dcvDiffuse.b = 1.0f;
    newLight->d3dLight_134.dcvSpecular.r = 0.1f;
    newLight->d3dLight_134.dcvSpecular.g = 0.1f;
    newLight->d3dLight_134.dcvSpecular.b = 0.1f;
    newLight->d3dLight_134.dcvAmbient.r = 1.0f;
    newLight->d3dLight_134.dcvAmbient.g = 1.0f;
    newLight->d3dLight_134.dcvAmbient.b = 1.0f;
    newLight->d3dLight_134.dvPosition.x = 0.0f;
    newLight->d3dLight_134.dvPosition.y = 0.0f;
    newLight->d3dLight_134.dvPosition.z = 0.0f;
    newLight->d3dLight_134.dvDirection.x = 0.0f;
    newLight->d3dLight_134.dvDirection.y = 0.0f;
    newLight->d3dLight_134.dvDirection.z = 1.0f;
    newLight->d3dLight_134.dvRange = 99999.0f;
    newLight->d3dLight_134.dvFalloff = 1.0f;
    newLight->d3dLight_134.dvAttenuation0 = 0.0f;
    newLight->d3dLight_134.dvAttenuation1 = 0.0f;
    newLight->d3dLight_134.dvAttenuation2 = 0.0f;
    newLight->d3dLight_134.dvTheta = 0.80000001f;
    newLight->d3dLight_134.dvPhi = 1.0f;
  }

  CHanaObj__AddRef_40BFC3(newLight);
  *pOut = newLight;

  // Determine string identifier suffix based on light type
  const char *lightTypeName = "Unk";
  switch (newLight->d3dLight_134.dltType)
  {
  case D3DLIGHT_DIRECTIONAL:
    lightTypeName = "Drc";
    break;
  case (D3DLIGHT_FORCE_DWORD | 0x80000000):
    lightTypeName = "Amb";
    break;
  case D3DLIGHT_SPOT:
    lightTypeName = "Spt";
    break;
  case D3DLIGHT_POINT:
    lightTypeName = "Pnt";
    break;
  }

  // Generate a unique debug name using the light type, index, and random values
  char lightName[256];
  int randSuffix1 = rand() % 100;
  int randSuffix2 = rand() % 100;
  sprintf(lightName, "Light_%s_(%d)%d%d", lightTypeName, newLight->dwLightIndex_130, randSuffix2, randSuffix1);

  AttachHanaObj_40BAB8(newLight, lightName);
  ApplyLightSettings(newLight);
}

void __cdecl hook_CHanaObj__AddObjItemRef_40B848(LPCSTR szName, CHanaBase_S130 *pObj)
{
  CObjItemRef_S208 *pObjItemRef; // [esp+0h] [ebp-14h]
  int v3; // [esp+4h] [ebp-10h]
  int j; // [esp+8h] [ebp-Ch]
  int k; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]
  CObjItemRef_S208 *pStruct208; // [esp+10h] [ebp-4h]
  CObjItemRef_S208 *pDestItemRef; // [esp+10h] [ebp-4h]

  DebugLog("[HNH:SUB] %s\n", "CHanaObj__AddObjItemRef_40B848");

  if ( szName && *szName )
  {
    v3 = lstrlenA(szName);
    for ( i = 0; i < g_nObjectTableCount; ++i )
    {
      if ( !g_pObjectTable[i].szName_0[0] )
      {
        pStruct208 = &g_pObjectTable[i];
        strcpy(pStruct208->szName_0, szName);
        for ( j = 0; j < v3; ++j )
        {
          if ( szName[j] == 32 )
            strcpy(pStruct208->szName_100, &szName[j + 1]);
        }
        pStruct208->pObj_200 = pObj;
        pStruct208->dwObjType_204 = pObj->dwType;
        pObj->nIndex_108 = i;
        return;
      }
    }
    pDestItemRef = &g_pObjectTable[g_nObjectTableCount];
    strcpy(pDestItemRef->szName_0, szName);
    for ( k = 0; k < v3; ++k )
    {
      if ( szName[k] == 32 )
        strcpy(pDestItemRef->szName_100, &szName[k + 1]);
    }
    pDestItemRef->pObj_200 = pObj;
    pDestItemRef->dwObjType_204 = pObj->dwType;
    pObj->nIndex_108 = g_nObjectTableCount++;
    if ( g_nObjectTableCount >= g_nObjectTableCapacity_4B009C )// If above capacity, do Realloc
    {
      pObjItemRef = g_pObjectTable;
      g_pObjectTable = (CObjItemRef_S208*) MemAlloc_40DEF7(520 * (g_nObjectTableCapacity_4B009C + 256));
      memset(g_pObjectTable, 0, sizeof(CObjItemRef_S208) * (g_nObjectTableCapacity_4B009C + 256));
      memcpy(g_pObjectTable, pObjItemRef, sizeof(CObjItemRef_S208) * g_nObjectTableCapacity_4B009C);
      g_nObjectTableCapacity_4B009C += 256;
      MemFree_40DF64(pObjItemRef);
    }
  }
}

void __cdecl hook_AttachHanaObj_40BAB8(CHanaBase_S130 *pParent, LPCSTR szName)
{
  int lenStr; // [esp+0h] [ebp-14h]
  int j; // [esp+4h] [ebp-10h]
  CObjItemRef_S208 *pStruct208; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]

  DebugLog("[HNH:SUB] AttachHanaObj_40BAB8\n");

  if ( pParent && szName && *szName )
  {
    lenStr = lstrlenA(szName);
    if ( lenStr > 63 )
      lenStr = 63;
    pStruct208 = g_pObjectTable;
    for ( i = 0; ; ++i )
    {
      if ( i >= g_nObjectTableCount )
      {
        strcpy(pParent->szName_8, szName);
        CHanaObj__AddObjItemRef_40B848(szName, pParent);
        return;
      }
      if ( pParent == pStruct208->pObj_200 )
        break;
      ++pStruct208;
    }
    strcpy(pStruct208->szName_0, szName);
    strcpy(pParent->szName_8, szName);
    for ( j = 0; j < lenStr; ++j )
    {
      if ( szName[j] == ' ' )
        strcpy(pStruct208->szName_100, &szName[j + 1]);
    }
    pStruct208->dwObjType_204 = pParent->dwType;
    pParent->nIndex_108 = i;
  }
}

D3DXMATRIX *__cdecl hook_MatrixRotateCubeMapFace(CHanaFrameT3EC_S354 *pFrame, int mode, D3DXVECTOR3 *pV, float angle)
{
  D3DXMATRIX rotationMatrix;
  D3DXMATRIX strippedMatrix;

  DebugLog("[HNH:SUB] %s\n", "MatrixRotateCubeMapFace_4070AB");

  // Variables to preserve the translation components (m[3][0], m[3][1], m[3][2])
  __int64 savedTransXY;
  float savedTransZ;

  switch (mode)
  {
  case 0:
    // Direct rotation overwrite
    D3DXMatrixRotationAxis(&pFrame->mat4x4_fmrx, pV, angle);
    break;

  case 1:
    // Pre-multiplication: rotation * original (with translation preserved)
    D3DXMatrixRotationAxis(&rotationMatrix, pV, angle);

    qmemcpy(&strippedMatrix, &pFrame->mat4x4_fmrx, sizeof(strippedMatrix));
    memset(strippedMatrix.m[3], 0, 12); // Zero out translation row

    savedTransXY = *(__int64*)&pFrame->mat4x4_fmrx.m[3][0];
    savedTransZ = pFrame->mat4x4_fmrx._43;

    D3DXMatrixMultiply(&pFrame->mat4x4_fmrx, &rotationMatrix, &strippedMatrix);

    *(__int64*)&pFrame->mat4x4_fmrx.m[3][0] = savedTransXY;
    pFrame->mat4x4_fmrx._43 = savedTransZ;
    break;

  default:
    // Post-multiplication: original * rotation (with translation preserved)
    D3DXMatrixRotationAxis(&rotationMatrix, pV, angle);

    qmemcpy(&strippedMatrix, &pFrame->mat4x4_fmrx, sizeof(strippedMatrix));
    memset(strippedMatrix.m[3], 0, 12); // Zero out translation row

    savedTransXY = *(__int64*)&pFrame->mat4x4_fmrx.m[3][0];
    savedTransZ = pFrame->mat4x4_fmrx._43;

    D3DXMatrixMultiply(&pFrame->mat4x4_fmrx, &strippedMatrix, &rotationMatrix);

    *(__int64*)&pFrame->mat4x4_fmrx.m[3][0] = savedTransXY;
    pFrame->mat4x4_fmrx._43 = savedTransZ;
    break;
  }

  // Final combination matrix multiplication
  return D3DXMatrixMultiply(&pFrame->mat4x4_after, &pFrame->mat4x4_fmrx, &pFrame->mat4x4_before);
}

void __cdecl hook_CalculateLocalRotationMatrix(
  CHanaFrameT3EC_S354 *pCameraFrame,
  CHanaFrameT3EC_S354 *pSceneFrame,
  float *pXpos,
  float *pYpos,
  float *pZpos,
  float *pXYrot,
  float *pYXrot,
  float *pZXrot)
{
  D3DXMATRIX pOut; // [esp+0h] [ebp-40h] BYREF

  static int log_CalculateLocalRotationMatrix_407047 = 1;
  log_CalculateLocalRotationMatrix_407047--;
  if (!log_CalculateLocalRotationMatrix_407047) {
    DebugLog("[HNH:SUB] %s\n", "CalculateLocalRotationMatrix_407047");
    log_CalculateLocalRotationMatrix_407047 = 1000;
  }

  D3DXMatrixInverse(&pOut, 0, &pSceneFrame->mat4x4_after);
  D3DXMatrixMultiply(&pOut, &pCameraFrame->mat4x4_after, &pOut);
  *pXYrot = pOut._12;
  *pYXrot = pOut._22;
  *pZXrot = pOut._32;
  *pXpos = pOut._13;
  *pYpos = pOut._23;
  *pZpos = pOut._33;
}

void register_CheckRunGame_4863C8() {
  hnh_Hook(0x4863C8, hook_CheckRunGame_4863C8, "CheckRunGame_4863C8");
  hnh_Hook(0x47E858, hook_ReadFileToBuffer, "ReadFileToBuffer_47E858");
  hnh_Hook(0x47E7F2, hook_WriteBufferToFile, "WriteBufferToFile_47E7F2");
  hnh_Hook(0x4278FF, hook_DisplayConfig_Init, "DisplayConfig_Init_4278FF");
  hnh_Hook(0x427F8C, hook_DisplayConfig_ProcessDriverDesc, "DisplayConfig_ProcessDriverDesc_427F8C");
  hnh_Hook(0x427C51, hook_IsVideoModeSupported, "IsVideoModeSupported_427C51");
  hnh_Hook(0x428927, hook_ShowTitleMenuWnd_428927, "ShowTitleMenuWnd_428927");
  hnh_Hook(0x40DD71, hook_SetLightingState, "SetLightingState_40DD71");
  hnh_Hook(0x40AF60, hook_D3DInitBuffers_40AF60, "D3DInitBuffers_40AF60");
  hnh_Hook(0x408F00, hook_CHanaObj__NewLightT3EF, "CHanaObj__NewLightT3EF");
  hnh_Hook(0x40B848, hook_CHanaObj__AddObjItemRef_40B848, "CHanaObj__AddObjItemRef_40B848");
  hnh_Hook(0x40BAB8, hook_AttachHanaObj_40BAB8, "AttachHanaObj_40BAB8");
  hnh_Hook(0x4070AB, hook_MatrixRotateCubeMapFace, "MatrixRotateCubeMapFace");
  hnh_Hook(0x407047, hook_CalculateLocalRotationMatrix, "CalculateLocalRotationMatrix");
} 