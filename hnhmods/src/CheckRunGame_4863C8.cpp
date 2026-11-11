// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"
#include "console.hpp"
#include "struct.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

#include <cmath>
#include <corecrt_math_defines.h> // define: M_PI

// === GLOBAL VARIABLES ===

D3DXMATRIX s_mat4x4View_4D4780;
D3DXMATRIX s_mat4x4Projection_4CE870;

float s_fProjZFar  = 100000.0;
float s_fProjZNear = 1.0;
float s_fProjAspect = 0.75;
float s_fProjFov    = 0.5;

int s_bAlphaBlendEnabled_4D6CC0;
int s_nAlphaOpState_4D6CC4;

BOOL s_bSpecularEnable_4B00A0 = 1;
BOOL s_bSpecularEnable_4B3474 = 1;
int  s_nObjectTableCapacity_4B009C = 64;
int s_arrZBufDepthCountPerDev_518C40[16];
int s_arrZBufDepthsPerDev_518828[256];

HWND s_hWnd;
BOOL s_bRunGame = 0xFFFFFFFF;

BOOL s_bAntiAliasEnable    = 1;
BOOL s_bDitherEnable       = 1;
BOOL s_bLightEnable_4B3484 = 1;
BOOL s_nLightningEnabled = 1;

BOOL s_bShadowEnable_518CA8;
int  s_nResolutionMode_518C80;
int  s_nZBufDepthMode_518C9C;
int  s_nRenderHALMode_518CA0;
int  s_nWidth2;
int  s_nWidth3;

#ifdef _USRDLL
D3DXMATRIX &g_mat4x4View_4D4780       = *(D3DXMATRIX *)0x4D4780; // WRITABLE
D3DXMATRIX &g_mat4x4Projection_4CE870 = *(D3DXMATRIX *)0x4CE870; // WRITABLE

float &g_fProjZFar   = *(float *)0x4B00C0; // WRITABLE default: 100000.0
float &g_fProjZNear  = *(float *)0x4B00BC; // WRITABLE default: 1.0
float &g_fProjAspect = *(float *)0x4B00B8; // WRITABLE default: 0.75
float &g_fProjFov    = *(float *)0x4B00C4; // WRITABLE default: 0.5

int &g_bAlphaBlendEnabled_4D6CC0    = *(int *)0x4D6CC0; // WRITABLE
int &g_nAlphaOpState_4D6CC4         = *(int *)0x4D6CC4; // WRITABLE
BOOL &g_bSpecularEnable_4B00A0      = *(BOOL*)0x4B00A0; // WRITABLE default: 1
BOOL &g_bSpecularEnable_4B3474 = *(BOOL*)0x4B3474; //  default: 1
int  &g_nObjectTableCapacity_4B009C  = *(int*)0x4B009C; // default: 64
int (&g_arrZBufDepthCountPerDev_518C40)[16] = *(int(*)[16])0x518C40;
int (&g_arrZBufDepthsPerDev_518828)[256]    = *(int(*)[256])0x518828;

HWND &g_hWnd             = *(HWND*)0x518CAC;
BOOL &g_bRunGame         = *(BOOL*)0x4B3488; // default: 0xFFFFFFFF

BOOL &g_bAntiAliasEnable       = *(BOOL *)0x4B347C; // WRITABLE, default: 1
BOOL &g_bDitherEnable          = *(BOOL *)0x4B3478; // WRITABLE, default: 1
BOOL &g_bLightEnable_4B3484    = *(BOOL *)0x4B3484; // WRITABLE, default: 1
BOOL &g_bShadowEnable_518CA8   = *(BOOL *)0x518CA8; // WRITABLE
BOOL &g_nLightningEnabled      = *(BOOL *)0x4B00AC; // WRITABLE, default: 1

int  &g_nResolutionMode_518C80 = *(int *)0x518C80; // WRITABLE
int  &g_nZBufDepthMode_518C9C  = *(int *)0x518C9C; // WRITABLE
int  &g_nRenderHALMode_518CA0  = *(int *)0x518CA0; // WRITABLE
int  &g_nWidth2                = *(int *)0x518C28; // WRITABLE
int  &g_nWidth3    = *(int *)0x52F19C; // WRITABLE

// --- internal
//CVideoModeWhiteItem(&g_arrModeWhitelist_4B33F0)[7] = *(CVideoModeWhiteItem(*)[7])0x4B33F0; // once!

int &g_bPreferTnLHal_4B33E8 = *(int*)0x4B33E8; // once! default: 1
unsigned int   &g_dwRGBBitCount_4D6D00 = *(unsigned int *)0x4D6D00; // WRITABLE

D3DXVECTOR3 &g_vec3AxisX_4CE8E8 = *(D3DXVECTOR3 *)0x4CE8E8; // WRITABLE
D3DXVECTOR3 &g_vec3AxisY_4CE8F8 = *(D3DXVECTOR3 *)0x4CE8F8; // WRITABLE
D3DXVECTOR3 &g_vec3AxisZ_4CE908 = *(D3DXVECTOR3 *)0x4CE908; // WRITABLE
D3DXVECTOR3 (&g_arrFwdByYaw360_4CD788)[360]   = *(D3DXVECTOR3 (*)[360])0x4CD788; // WRITABLE
D3DXVECTOR3 (&g_arrUpByPitch360_4D47C0)[360]  = *(D3DXVECTOR3 (*)[360])0x4D47C0; // WRITABLE
D3DXVECTOR3 (&g_arrFwdByPitch360_4D58A0)[360] = *(D3DXVECTOR3 (*)[360])0x4D58A0; // WRITABLE

BOOL      &g_bSpecularEnableMirror_4B00A4 = *(BOOL*)0x4B00A4; // WRITABLE, default: 1
HINSTANCE &g_hInstance_518CB0 = *(HINSTANCE*)0x518CB0;
int       &g_nLightningState  = *(int *)0x4B00A8; // WRITABLE, default: 1

#else

D3DXMATRIX &g_mat4x4View_4D4780 = s_mat4x4View_4D4780;
D3DXMATRIX &g_mat4x4Projection_4CE870= s_mat4x4Projection_4CE870;

float &g_fProjZFar  = s_fProjZFar;
float &g_fProjZNear = s_fProjZNear;
float &g_fProjAspect= s_fProjAspect;
float &g_fProjFov   = s_fProjFov;

int &g_bAlphaBlendEnabled_4D6CC0   = s_bAlphaBlendEnabled_4D6CC0;
int &g_nAlphaOpState_4D6CC4        = s_nAlphaOpState_4D6CC4;
BOOL &g_bSpecularEnable_4B00A0     = s_bSpecularEnable_4B00A0;
BOOL &g_bSpecularEnable_4B3474     = s_bSpecularEnable_4B3474;
int  &g_nObjectTableCapacity_4B009C = s_nObjectTableCapacity_4B009C ;
int(&g_arrZBufDepthCountPerDev_518C40)[16] = s_arrZBufDepthCountPerDev_518C40;
int(&g_arrZBufDepthsPerDev_518828)[256] = s_arrZBufDepthsPerDev_518828;

HWND &g_hWnd            = s_hWnd;
BOOL &g_bRunGame        = s_bRunGame;

BOOL &g_bAntiAliasEnable      = s_bAntiAliasEnable;
BOOL &g_bDitherEnable         = s_bDitherEnable;
BOOL &g_bLightEnable_4B3484   = s_bLightEnable_4B3484;
BOOL &g_bShadowEnable_518CA8  = s_bShadowEnable_518CA8;
BOOL &g_nLightningEnabled     = s_nLightningEnabled;

int  &g_nResolutionMode_518C80= s_nResolutionMode_518C80;
int  &g_nZBufDepthMode_518C9C = s_nZBufDepthMode_518C9C;
int  &g_nRenderHALMode_518CA0 = s_nRenderHALMode_518CA0;
int  &g_nWidth2               = s_nWidth2;
int  &g_nWidth3               = s_nWidth3;

// --- internal
//CVideoModeWhiteItem(&g_arrModeWhitelist_4B33F0)[7] = *(CVideoModeWhiteItem(*)[7])0x4B33F0; // once!

int            g_bPreferTnLHal_4B33E8 = 1;
unsigned int   g_dwRGBBitCount_4D6D00;

D3DXVECTOR3 g_vec3AxisX_4CE8E8;
D3DXVECTOR3 g_vec3AxisY_4CE8F8;
D3DXVECTOR3 g_vec3AxisZ_4CE908;
D3DXVECTOR3 g_arrFwdByYaw360_4CD788[360];
D3DXVECTOR3 g_arrUpByPitch360_4D47C0[360];
D3DXVECTOR3 g_arrFwdByPitch360_4D58A0[360];

BOOL g_bSpecularEnableMirror_4B00A4 = 1;
HINSTANCE g_hInstance_518CB0;
int  g_nLightningState = 1;

#endif

// === GLOBAL FUNCTIONS ===

BOOL (__cdecl * CheckRunGame_4863C8)(HWND hWnd, BOOL bFullScreen) = reinterpret_cast<BOOL(__cdecl*)(HWND hWnd, BOOL bFullScreen)>(0x4863C8);
void (* DisplayConfig_ProcessDriverDesc)(CDisplayConfig *pConfig) = reinterpret_cast<void(*)(CDisplayConfig *pConfig)>(0x427F8C);
BOOL (__cdecl * IsVideoModeSupported)(D3DX_VIDMODEDESC *pVidModeDesc) = reinterpret_cast<BOOL(__cdecl*)(D3DX_VIDMODEDESC *pVidModeDesc)>(0x427C51);
void (__cdecl * DisplayConfig_Init)(CDisplayConfig *pDisplayConfig) = reinterpret_cast<void(__cdecl*)(CDisplayConfig *pDisplayConfig)>(0x4278FF);
DWORD (__cdecl * ReadFileToBuffer)(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize) = reinterpret_cast<DWORD(__cdecl*)(LPCSTR lpFileName, LPVOID lpBuffer, int nBufSize)>(0x47E858);
void (__cdecl * WriteBufferToFile)(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite) = reinterpret_cast<void(__cdecl*)(LPCSTR lpFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)>(0x47E7F2);
int (__cdecl * ShowTitleMenuWnd_428927)(HINSTANCE hInstance) = reinterpret_cast<int(__cdecl*)(HINSTANCE hInstance)>(0x428927);
HRESULT (__cdecl * SetLightingState)(DWORD dwEnable) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwEnable)>(0x40DD71);
HRESULT (* D3DInitBuffers_40AF60)() = reinterpret_cast<HRESULT(*)()>(0x40AF60);
void (__cdecl * CHanaObj__NewLightT3EF)(CHanaLightT3EF_S1A0 **pOut, D3DLIGHT7 *pD3Dlight) = reinterpret_cast<void(__cdecl*)(CHanaLightT3EF_S1A0 **pOut, D3DLIGHT7 *pD3Dlight)>(0x408F00);
void (__cdecl * CHanaObj__AddObjItemRef_40B848)(LPCSTR szName, CHanaBase_S130 *pObj) = reinterpret_cast<void(__cdecl*)(LPCSTR szName, CHanaBase_S130 *pObj)>(0x40B848);
void (__cdecl * AttachHanaObj_40BAB8)(CHanaBase_S130 *pParent, LPCSTR szName) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130 *pParent, LPCSTR szName)>(0x40BAB8);
D3DXMATRIX * (__cdecl * MatrixRotateCubeMapFace)(CHanaFrameT3EC_S354 *pFrame, int mode, D3DXVECTOR3 *pV, float angle) =
reinterpret_cast<D3DXMATRIX *(__cdecl*)(CHanaFrameT3EC_S354 *pFrame, int mode, D3DXVECTOR3 *pV, float angle)>(0x4070AB);
void (__cdecl * CalculateLocalRotationMatrix)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pSceneFrame, float *pXpos, float *pYpos, float *pZpos, float *pXYrot, float *pYXrot, float *pZXrot) =
reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pSceneFrame, float *pXpos, float *pYpos, float *pZpos, float *pXYrot, float *pYXrot, float *pZXrot)>(0x407047);
double (__cdecl * DegreesToRadians)(float deg) = reinterpret_cast<double(__cdecl*)(float deg)>(0x40DDF3);
HRESULT (__cdecl * MatrixPerspectiveFovLH)(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf) = reinterpret_cast<HRESULT(__cdecl*)(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf)>(0x40CB94);
HRESULT (__cdecl * MatrixLookAtLH)(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp) = reinterpret_cast<HRESULT(__cdecl*)(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)>(0x40C64A);
D3DXMATRIX * (__cdecl * MatIdentity)(D3DXMATRIX *pOut) = reinterpret_cast<D3DXMATRIX *(__cdecl*)(D3DXMATRIX *pOut)>(0x40E450);
double (__cdecl * Vec3LengthSq)(const D3DXVECTOR3 *pV) = reinterpret_cast<double(__cdecl*)(const D3DXVECTOR3 *pV)>(0x40E3C0);

union {
  void(__thiscall * member)(D3DXVECTOR3* This, float x, float y, float z);
  void(__fastcall* fast)(D3DXVECTOR3* This, int unused_edx, float x, float y, float z);
} Vec3Set = { .member = reinterpret_cast<void(__thiscall *)(D3DXVECTOR3 *This, float x, float y, float z)>(0x40E420) };

// void (__thiscall * Vec3Set)(D3DXVECTOR3 *This, float x, float y, float z) = reinterpret_cast<void(__thiscall*)(D3DXVECTOR3 *This, float x, float y, float z)>(0x40E420);

double (__cdecl * Vec3Dot)(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2) = reinterpret_cast<double(__cdecl*)(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)>(0x40E3F0);


// === FUNCTION IMPLEMENTATIONS ===

void hook_DisplayConfig_ProcessDriverDesc(CDisplayConfig *pConfig)
{
  ONCELOG("[HNH:SUB] DisplayConfig_ProcessDriverDesc_427F8C\n");

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

      if (!strcmp(tail, "Microsoft Direct3D Hardware Transform and Lighting acceleration capable device")) {
        strcat(friendly, "/ Direct3D T&L HAL");
        if (g_bPreferTnLHal_4B33E8)                 // static flag = 1
          pConfig->nDeviceIdxHal = i;
      }
      else if (!strcmp(tail, "Microsoft Direct3D Hardware acceleration through Direct3D HAL")) {
        strcat(friendly, "/ Direct3D HAL");
        pConfig->nDeviceIdxHal = i;                 // HAL always wins so far
      }
      else if (!strcmp(tail, "Microsoft Direct3D RGB Software Emulation"))
        strcat(friendly, "/ RGB Software Emulation");   // default idx untouched
      else if (!strcmp(tail, "Microsoft Direct3D Reference Rasterizer"))
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

  ONCELOG("[HNH:SUB] IsVideoModeSupported_427C51\n");

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

  ONCELOG("[HNH:SUB] DisplayConfig_Init_4278FF\n");

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

  ONCELOG("[HNH:SUB] ReadFileToBuffer_47E858\n");

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

  ONCELOG("[HNH:SUB] WriteBufferToFile_47E7F2\n");

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
  HWND hDesktopWnd; // eax
  MSG Msg; // [esp+0h] [ebp-58h] BYREF
  int v4; // [esp+1Ch] [ebp-3Ch]
  WNDCLASSA WndClass; // [esp+20h] [ebp-38h] BYREF
  RECT Rect; // [esp+48h] [ebp-10h] BYREF

  ONCELOG("[HNH:SUB] ShowTitleMenuWnd_428927\n");

  v4 = 0;
  hDesktopWnd = GetDesktopWindow();
  GetWindowRect(hDesktopWnd, &Rect);
  //Rect.right = 1280;
  //Rect.bottom = 960;

  g_hInstance_518CB0 = hInstance;
  WndClass.style = 0;
  WndClass.lpfnWndProc = WndProcTitleMenu_428C11;
  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hInstance = hInstance;
  WndClass.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(0x69)); // 105
  WndClass.hCursor = LoadCursorA(hInstance, MAKEINTRESOURCEA(0x68)); // 104
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

  ONCELOG("[HNH:SUB] CheckRunGame_4863C8\n");

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
  g_nZBufDepthMode_518C9C = szTeaConfigData[7];
  g_nRenderHALMode_518CA0 = 1;
  bRunGame = ShowTitleMenu_427DA8(hWnd, bFullScreen);
  if ( bRunGame > 0 )
  {
    g_nWidth3 = g_nWidth2;
    szTeaConfigData[0] = g_bFogIsActive_4B3480;
    szTeaConfigData[1] = g_bSpecularEnable_4B3474;
    szTeaConfigData[2] = g_bAntiAliasEnable;
    szTeaConfigData[3] = g_bDitherEnable;
    szTeaConfigData[4] = g_bLightEnable_4B3484;
    szTeaConfigData[5] = g_bShadowEnable_518CA8;
    szTeaConfigData[6] = g_nResolutionMode_518C80;
    szTeaConfigData[7] = g_nZBufDepthMode_518C9C;
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
  ONCELOG("[HNH:SUB] %s\n", "SetLightingState_40DD71");

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

  ONCELOG("[HNH:SUB] %s\n", "D3DInitBuffers_40AF60");

  // --- 1. Guard Clauses (Early Exits for Pointers & Init Flags) ---
  if (!g_bD3Dinited_4D6C6C) 
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

  g_bSpecularEnable_4B00A0 = 1;
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
  ONCELOG("[HNH:SUB] %s\n", "CHanaObj__NewLightT3EF_408F00");

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
  int len; // [esp+4h] [ebp-10h]
  int j; // [esp+8h] [ebp-Ch]
  int k; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]
  CObjItemRef_S208 *pStruct208; // [esp+10h] [ebp-4h]
  CObjItemRef_S208 *pDestItemRef; // [esp+10h] [ebp-4h]

  ONCELOG("[HNH:SUB] %s\n", "CHanaObj__AddObjItemRef_40B848");

  if ( szName && *szName )
  {
    len = lstrlenA(szName);
    for ( i = 0; i < g_nObjectTableCount; ++i )
    {
      if ( !g_pObjectTable[i].szName_0[0] )
      {
        pStruct208 = &g_pObjectTable[i];
        strcpy(pStruct208->szName_0, szName);
        for ( j = 0; j < len; ++j )
        {
          if ( szName[j] == ' ')
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
    for ( k = 0; k < len; ++k )
    {
      if ( szName[k] == ' ')
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
  int len; // [esp+0h] [ebp-14h]
  int j; // [esp+4h] [ebp-10h]
  CObjItemRef_S208 *pStruct208; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]

  ONCELOG("[HNH:SUB] AttachHanaObj_40BAB8\n");

  if ( pParent && szName && *szName )
  {
    len = lstrlenA(szName);
    if ( len > 63 )
      len = 63;
    pStruct208 = g_pObjectTable;
    for ( i = 0; ; ++i )
    {
      if ( i >= g_nObjectTableCount )
      {
        strcpy_s(pParent->szName_8, sizeof(pParent->szName_8), szName);
        CHanaObj__AddObjItemRef_40B848(szName, pParent);
        return;
      }
      if ( pParent == pStruct208->pObj_200 )
        break;
      ++pStruct208;
    }
    strcpy(pStruct208->szName_0, szName);
    strcpy(pParent->szName_8, szName);
    for ( j = 0; j < len; ++j )
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

  ONCELOG("[HNH:SUB] %s\n", "MatrixRotateCubeMapFace_4070AB");

  // Variables to preserve the translation components (m[3][0], m[3][1], m[3][2])
  __int64 savedTransXY;
  float savedTransZ;

  switch (mode)
  {
  case 0:
    // Direct rotation overwrite
    D3DXMatrixRotationAxis(&pFrame->matLocal_138, pV, angle);
    break;

  case 1:
    // Pre-multiplication: rotation * original (with translation preserved)
    D3DXMatrixRotationAxis(&rotationMatrix, pV, angle);

    qmemcpy(&strippedMatrix, &pFrame->matLocal_138, sizeof(strippedMatrix));
    memset(strippedMatrix.m[3], 0, 12); // Zero out translation row

    savedTransXY = *(__int64*)&pFrame->matLocal_138.m[3][0];
    savedTransZ = pFrame->matLocal_138._43;

    D3DXMatrixMultiply(&pFrame->matLocal_138, &rotationMatrix, &strippedMatrix);

    *(__int64*)&pFrame->matLocal_138.m[3][0] = savedTransXY;
    pFrame->matLocal_138._43 = savedTransZ;
    break;

  default:
    // Post-multiplication: original * rotation (with translation preserved)
    D3DXMatrixRotationAxis(&rotationMatrix, pV, angle);

    qmemcpy(&strippedMatrix, &pFrame->matLocal_138, sizeof(strippedMatrix));
    memset(strippedMatrix.m[3], 0, 12); // Zero out translation row

    savedTransXY = *(__int64*)&pFrame->matLocal_138.m[3][0];
    savedTransZ = pFrame->matLocal_138._43;

    D3DXMatrixMultiply(&pFrame->matLocal_138, &strippedMatrix, &rotationMatrix);

    *(__int64*)&pFrame->matLocal_138.m[3][0] = savedTransXY;
    pFrame->matLocal_138._43 = savedTransZ;
    break;
  }

  // Final combination matrix multiplication
  return D3DXMatrixMultiply(&pFrame->matWorld_178, &pFrame->matLocal_138, &pFrame->matParentWorld_1B8);
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

  ONCELOG("[HNH:SUB] %s\n", "CalculateLocalRotationMatrix_407047");

  D3DXMatrixInverse(&pOut, 0, &pSceneFrame->matWorld_178);
  D3DXMatrixMultiply(&pOut, &pCameraFrame->matWorld_178, &pOut);
  *pXYrot = pOut._12;
  *pYXrot = pOut._22;
  *pZXrot = pOut._32;
  *pXpos = pOut._13;
  *pYpos = pOut._23;
  *pZpos = pOut._33;
}

double __cdecl hook_DegreesToRadians(float deg)
{
  float sign = 1.0f;
  float accumulatedPi = 0.0f;

  ONCELOG("[HNH:SUB] %s\n", "DegreesToRadians_40DDF3");

  if (deg < 0.0f)
  {
    sign = -1.0f;
    deg = -deg;
  }

  while (deg > 180.0f)
  {
    deg -= 180.0f;
    accumulatedPi += (float)M_PI;
  }

  return (deg * (float)(M_PI / 180.0) + accumulatedPi) * sign;
}

HRESULT __cdecl hook_MatrixPerspectiveFovLH(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf)
{
  double X; // st7
  float v7; // [esp+18h] [ebp-24h]
  float v8; // [esp+20h] [ebp-1Ch]
  float v9; // [esp+30h] [ebp-Ch]
  float v10; // [esp+34h] [ebp-8h]
  float v11; // [esp+38h] [ebp-4h]

  ONCELOG("[HNH:SUB] %s\n", "MatrixPerspectiveFovLH_40CB94");

  if ( fabs(zf - zn) < 0.009999999776482582 )
    return E_INVALIDARG;
  X = sin(fovy / 2.0);
  if ( fabs(X) < 0.009999999776482582 )
    return E_INVALIDARG;
  v8 = sin(fovy / 2.0);
  v10 = cos(fovy / 2.0) / v8 * aspect;
  v7 = sin(fovy / 2.0);
  v9 = cos(fovy / 2.0) / v7 * 1.0;
  v11 = zf / (zf - zn);
  memset(pMat, 0, sizeof(D3DXMATRIX));
  pMat->_11 = v10;
  pMat->_22 = v9;
  pMat->_33 = v11;
  pMat->_34 = 1.0;
  pMat->_43 = -v11 * zn;
  return S_OK;
}

HRESULT __cdecl hook_MatrixLookAtLH(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
{
  float lenSq;
  float invLen;
  float dotProduct;
  D3DXVECTOR3 forward; // Formerly v60
  D3DXVECTOR3 up;      // Formerly v64
  D3DXVECTOR3 right;   // Formerly v61
  D3DXVECTOR3 tempVec;

  ONCELOG("[HNH:SUB] %s\n", "MatrixLookAtLH_40C64A");

  // --- 1. Compute and normalize the forward vector (Z-axis) ---
  float dx = pAt->x - pEye->x;
  float dy = pAt->y - pEye->y;
  float dz = pAt->z - pEye->z;

  Vec3Set.member(&tempVec, dx, dy, dz);
  forward = tempVec;

  lenSq = Vec3LengthSq(&forward);
  invLen = sqrt(lenSq);
  if (invLen < 0.000001f)
    return E_INVALIDARG;

  float forwardLenInv = 1.0f / invLen;
  forward.x *= forwardLenInv;
  forward.y *= forwardLenInv;
  forward.z *= forwardLenInv;

  // --- 2. Orthogonalize the up vector (Y-axis) with fallbacks ---
  dotProduct = forward.z * pUp->z + forward.y * pUp->y + forward.x * pUp->x;
  Vec3Set.member(&tempVec, dotProduct * forward.x, dotProduct * forward.y, dotProduct * forward.z);

  Vec3Set.member(&up, pUp->x - tempVec.x, pUp->y - tempVec.y, pUp->z - tempVec.z);
  lenSq = Vec3LengthSq(&up);
  invLen = sqrt(lenSq);

  if (invLen < 0.000001f)
  {
    // Fallback 1: Try alignment using Y world axis
    float cy = forward.y;
    Vec3Set.member(&tempVec, forward.x * cy, forward.y * cy, forward.z * cy);
    Vec3Set.member(&up, 0.0f - tempVec.x, 1.0f - tempVec.y, 0.0f - tempVec.z);
    lenSq = Vec3LengthSq(&up);
    invLen = sqrt(lenSq);

    if (invLen < 0.000001f)
    {
      // Fallback 2: Try alignment using Z world axis
      float cz = forward.z;
      Vec3Set.member(&tempVec, forward.x * cz, forward.y * cz, forward.z * cz);
      Vec3Set.member(&up, 0.0f - tempVec.x, 0.0f - tempVec.y, 1.0f - tempVec.z);
      lenSq = Vec3LengthSq(&up);
      invLen = sqrt(lenSq);

      if (invLen < 0.000001f)
        return E_INVALIDARG;
    }
  }

  float upLenInv = 1.0f / invLen;
  up.x *= upLenInv;
  up.y *= upLenInv;
  up.z *= upLenInv;

  // --- 3. Compute the right vector (X-axis) using cross product (up x forward) ---
  right.x = up.y * forward.z - up.z * forward.y;
  right.y = up.z * forward.x - up.x * forward.z;
  right.z = up.x * forward.y - up.y * forward.x;

  // --- 4. Assemble the final view matrix ---
  MatIdentity(pMat);

  // Rotation components
  pMat->_11 = right.x;   pMat->_12 = up.x;   pMat->_13 = forward.x;
  pMat->_21 = right.y;   pMat->_22 = up.y;   pMat->_23 = forward.y;
  pMat->_31 = right.z;   pMat->_32 = up.z;   pMat->_33 = forward.z;

  // Translation components (dot product with eye position)
  pMat->_41 = Vec3Dot(pEye, &right);
  pMat->_42 = Vec3Dot(pEye, &up);
  pMat->_43 = Vec3Dot(pEye, &forward);

  return S_OK;
}


D3DXMATRIX *__cdecl hook_MatIdentity(D3DXMATRIX *pOut)
{
  D3DXMATRIX *result; // eax

  ONCELOG("[HNH:SUB] %s\n", "MatIdentity_40E450");


  result = pOut;
  pOut->m[3][2] = 0.0;
  pOut->m[3][1] = 0.0;
  pOut->m[3][0] = 0.0;
  pOut->m[2][3] = 0.0;
  pOut->m[2][1] = 0.0;
  pOut->m[2][0] = 0.0;
  pOut->m[1][3] = 0.0;
  pOut->m[1][2] = 0.0;
  pOut->m[1][0] = 0.0;
  pOut->m[0][3] = 0.0;
  pOut->m[0][2] = 0.0;
  pOut->m[0][1] = 0.0;
  pOut->m[3][3] = 1.0;
  pOut->m[2][2] = 1.0;
  pOut->m[1][1] = 1.0;
  pOut->m[0][0] = 1.0;
  return result;
}

double __cdecl hook_Vec3LengthSq(const D3DXVECTOR3 *pV)
{
  ONCELOG("[HNH:SUB] Vec3LengthSq_40E3C0\n");
  return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

void __fastcall hook_Vec3Set(D3DXVECTOR3 *This, int unused_edx, float x, float y, float z)
{
  ONCELOG("[HNH:SUB] hook_Vec3Set_40E420\n");
  This->Vec3Set(x, y, z);
}

double __cdecl hook_Vec3Dot(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
  ONCELOG("[HNH:SUB] Vec3Dot_40E3F0\n");
  return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}


void register_CheckRunGame_4863C8(int cfg) {
  if (cfg == 1) {
    CheckRunGame_4863C8 = hook_CheckRunGame_4863C8;
    DisplayConfig_ProcessDriverDesc = hook_DisplayConfig_ProcessDriverDesc;
    IsVideoModeSupported = hook_IsVideoModeSupported;
    DisplayConfig_Init = hook_DisplayConfig_Init;
    ReadFileToBuffer = hook_ReadFileToBuffer;
    WriteBufferToFile = hook_WriteBufferToFile;
    ShowTitleMenuWnd_428927 = hook_ShowTitleMenuWnd_428927;
    SetLightingState = hook_SetLightingState;
    D3DInitBuffers_40AF60 = hook_D3DInitBuffers_40AF60;
    CHanaObj__NewLightT3EF = hook_CHanaObj__NewLightT3EF;
    CHanaObj__AddObjItemRef_40B848 = hook_CHanaObj__AddObjItemRef_40B848;
    AttachHanaObj_40BAB8 = hook_AttachHanaObj_40BAB8;
    MatrixRotateCubeMapFace = hook_MatrixRotateCubeMapFace;
    CalculateLocalRotationMatrix = hook_CalculateLocalRotationMatrix;

    MatrixLookAtLH = hook_MatrixLookAtLH;
    DegreesToRadians = hook_DegreesToRadians;
    MatrixPerspectiveFovLH = hook_MatrixPerspectiveFovLH;
    MatIdentity = hook_MatIdentity;

    Vec3LengthSq = hook_Vec3LengthSq;
    Vec3Set.fast = hook_Vec3Set;
    Vec3Dot = hook_Vec3Dot;
  }

  if (cfg != 2) hnh_HookOff();

  hnh_Hook(0x40DD71, hook_SetLightingState, "SetLightingState_40DD71"); // [OK] 
  hnh_Hook(0x40AF60, hook_D3DInitBuffers_40AF60, "D3DInitBuffers_40AF60"); // [OK] 
  hnh_Hook(0x4863C8, hook_CheckRunGame_4863C8, "CheckRunGame_4863C8"); // [OK] 
  hnh_Hook(0x47E858, hook_ReadFileToBuffer, "ReadFileToBuffer_47E858"); // [OK] 
  hnh_Hook(0x47E7F2, hook_WriteBufferToFile, "WriteBufferToFile_47E7F2"); // [OK] 
  hnh_Hook(0x4278FF, hook_DisplayConfig_Init, "DisplayConfig_Init_4278FF"); // [OK] 
  hnh_Hook(0x427F8C, hook_DisplayConfig_ProcessDriverDesc, "DisplayConfig_ProcessDriverDesc_427F8C"); // [OK]

  hnh_Hook(0x427C51, hook_IsVideoModeSupported, "IsVideoModeSupported_427C51"); // [OK] 
  hnh_Hook(0x428927, hook_ShowTitleMenuWnd_428927, "ShowTitleMenuWnd_428927"); // [OK] 
  hnh_Hook(0x408F00, hook_CHanaObj__NewLightT3EF, "CHanaObj__NewLightT3EF"); // [OK] 
  hnh_Hook(0x40B848, hook_CHanaObj__AddObjItemRef_40B848, "CHanaObj__AddObjItemRef_40B848"); // [OK] 
  hnh_Hook(0x40BAB8, hook_AttachHanaObj_40BAB8, "AttachHanaObj_40BAB8"); // [OK] 
  hnh_Hook(0x4070AB, hook_MatrixRotateCubeMapFace, "MatrixRotateCubeMapFace"); // [OK] 
  hnh_Hook(0x407047, hook_CalculateLocalRotationMatrix, "CalculateLocalRotationMatrix"); // [OK] 

  hnh_Hook(0x40C64A, hook_MatrixLookAtLH, "MatrixLookAtLH_40C64A"); // [OK] 
  hnh_Hook(0x40DDF3, hook_DegreesToRadians, "DegreesToRadians_40DDF3"); // [OK} 
  hnh_Hook(0x40CB94, hook_MatrixPerspectiveFovLH, "MatrixPerspectiveFovLH_40CB94"); // [OK] 
  hnh_Hook(0x40E450, hook_MatIdentity, "MatIdentity_40E450"); // [OK] 
  hnh_Hook(0x40E3C0, hook_Vec3LengthSq, "Vec3LengthSq_40E3C0"); // [OK] 
  hnh_Hook(0x40E420, hook_Vec3Set, "Vec3Set_40E420"); // [OK] 
  hnh_Hook(0x40E3F0, hook_Vec3Dot, "Vec3Dot_40E3F0"); // [OK] 

  hnh_HookOn();
} 