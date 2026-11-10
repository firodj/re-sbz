// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include <cmath>
#include <corecrt_math_defines.h>

#include "console.hpp"
#include "shared.hpp"
#include "struct.hpp"

// ---
#define WMU_CHECK_SYSTEM 0x0BD0   // posted by WM_CREATE: install/disc/config gate
#define WMU_ENGINE_INIT  0x2B10   // posted on success: sound + renderer init

// === GLOBAL VARIABLES ===

static LPDIRECT3DDEVICE7    s_pD3DDevice_4D6C78;
static D3DDEVICEDESC7       s_d3dDevDesc_4D6980;
static LPD3DXCONTEXT        s_pRenderCtx;
static LPDIRECTDRAWSURFACE7 s_pZBuffer_4D6C94;
static LPDIRECTDRAWSURFACE7 s_pBackBuffer_4D6C90;
static LPDIRECTDRAWSURFACE7 s_pPrimaryBuffer_4D6C88;
static LPDIRECTDRAW7        s_pDD_4D6C7C;
static LPDIRECT3D7          s_pD3D_4D6C84;
static BOOL                 s_bD3Dinited_4D6C6C;

static DDPIXELFORMAT s_ddPixFormat;
static DWORD         s_dwPixelFormat;
static DWORD         s_dwBytesPerPixel;
static DWORD         s_dwRGBBitMask;
static DWORD         s_dwGBitMask;
static DWORD         s_dwRBitMask;

static LPDIRECTSOUND           s_ppDS;
static LPDIRECTSOUND3DLISTENER s_pds3DListener;
static LPDIRECTSOUNDBUFFER     s_pdsBuffer;
static BOOL                    s_bDSInited_5185C4;

static CObjItemRef_S208* s_pObjectTable;
static int               s_nObjectTableCount;
static CObjPoolMgr_S58*  s_pObjPool;

static HGDIOBJ s_hFont;
static int     s_dwCharWidth = 16;
static int     s_dwFontHeight = 16;

static CHanaLightT3EF_S1A0*     s_pLight_4D6CE0;
static CHanaMateT3EE_S184*      s_pMaterial;
static CHanaFrameT3EC_S354*     s_pLightFrame;
static CHanaFrameT3EC_S354*     s_pCameraFrame;
static CHanaFrameRootT3EC_S2D4* s_pSceneFrame;

#ifdef _USRDLL

LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78 = *(LPDIRECT3DDEVICE7*)0x4D6C78;
D3DDEVICEDESC7    &g_d3dDevDesc_4D6980 = *(D3DDEVICEDESC7 *)0x4D6980;
LPD3DXCONTEXT &g_pRenderCtx              = *(LPD3DXCONTEXT*)0x4D6C80; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pZBuffer_4D6C94    = *(LPDIRECTDRAWSURFACE7 *)0x4D6C94; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pBackBuffer_4D6C90 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C90; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pPrimaryBuffer_4D6C88 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C88; // WRITABLE
LPDIRECTDRAW7 &g_pDD_4D6C7C              = *(LPDIRECTDRAW7 *)0x4D6C7C; // WRITABLE
LPDIRECT3D7 &g_pD3D_4D6C84               = *(LPDIRECT3D7 *)0x4D6C84; // WRITABLE
BOOL &g_bD3Dinited_4D6C6C              = *(BOOL *)0x4D6C6C; // WRITABLE

DDPIXELFORMAT &g_ddPixFormat = *(DDPIXELFORMAT *)0x517790; // WRITABLE
DWORD &g_dwPixelFormat = *(DWORD *)0x518370;   // (16/24/32 bpp)
DWORD &g_dwBytesPerPixel = *(DWORD *)0x518374; // WRITABLE
DWORD &g_dwRGBBitMask  = *(DWORD *)0x517788;   // (B channel mask)
DWORD &g_dwGBitMask      = *(DWORD *)0x517D5C; // WRITABLE
DWORD &g_dwRBitMask      = *(DWORD *)0x517D58; // WRITABLE

LPDIRECTSOUND       &g_ppDS          = *(LPDIRECTSOUND*)0x5185C0;
LPDIRECTSOUND3DLISTENER &g_pds3DListener = *(LPDIRECTSOUND3DLISTENER*)0x52F3C4;
LPDIRECTSOUNDBUFFER &g_pdsBuffer     = *(LPDIRECTSOUNDBUFFER*)0x5185D0;
BOOL                 &g_bDSInited_5185C4 = *(BOOL*)0x5185C4;

CObjItemRef_S208 * &g_pObjectTable       = *(CObjItemRef_S208 * *)0x4D6C98; // WRITABLE
int&                g_nObjectTableCount                 = *(int*)0x4D6C9C;
CObjPoolMgr_S58 *  &g_pObjPool           = *(CObjPoolMgr_S58 * *)0x4D6CD0; // WRITABLE

HGDIOBJ             &g_hFont         = *(HGDIOBJ*)0x518378;
int                 &g_dwCharWidth   = *(int*)0x4B01B4;
int                 &g_dwFontHeight  = *(int*)0x4B01B8;

CHanaLightT3EF_S1A0 * &g_pLight_4D6CE0  = *(CHanaLightT3EF_S1A0 * *)0x4D6CE0; // WRITABLE
CHanaMateT3EE_S184 * &g_pMaterial       = *(CHanaMateT3EE_S184 * *)0x4D6CDC; // WRITABLE
CHanaFrameT3EC_S354 * &g_pLightFrame     = *(CHanaFrameT3EC_S354 * *)0x4D6CB0; // WRITABLE
CHanaFrameT3EC_S354 * &g_pCameraFrame    = *(CHanaFrameT3EC_S354 * *)0x4D6CAC; // WRITABLE
CHanaFrameRootT3EC_S2D4 * &g_pSceneFrame = *(CHanaFrameRootT3EC_S2D4 * *)0x4D6CA8; // WRITABLE

// ---

DSCAPS              &g_dsCaps_518560 = *(DSCAPS*)0x518560;

float (&g_CosineTable)[360] = *(float (*)[360])0x517D68; // WRITABLE
float (&g_SineTable)[360]   = *(float (*)[360])0x5177B4; // WRITABLE
DDBLTFX &g_ddBltFx          = *(DDBLTFX *)0x518308; // WRITABLE
DWORD &g_dwBBitMask      = *(DWORD *)0x517D60; // WRITABLE

uint32_t &g_nBBitCount      = *(uint32_t *)0x4B01C4; // WRITABLE
uint32_t &g_nRBitCount      = *(uint32_t *)0x4B01C0; // WRITABLE
uint32_t &g_nRGBBitCount    = *(uint32_t *)0x4B01BC; // WRITABLE
DWORD &g_dwLowestBBit    = *(DWORD *)0x518384; // WRITABLE
DWORD &g_dwLowestRBit    = *(DWORD *)0x518380; // WRITABLE
DWORD &g_dwLowestRGBBit  = *(DWORD *)0x51837C; // WRITABLE
CHAR &g_szFontFaceName      = *(CHAR *)0x4B01C8; // WRITABLE
LPSTR &g_pDDErrStr = *(LPSTR*)0x5185CC;

D3DX_DEVICEDESC * &g_pDevicesDesc = *(D3DX_DEVICEDESC**)0x4D6CCC; // WRITABLE
DWORD &g_nDeviceCount = *(DWORD *)0x4D6CC8; // WRITABLE

IUnknown * &g_pUnk_4D6C8C                  = *(IUnknown * *)0x4D6C8C; // WRITABLE

#else

LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78        = s_pD3DDevice_4D6C78;
D3DDEVICEDESC7 &g_d3dDevDesc_4D6980           = s_d3dDevDesc_4D6980;
LPD3DXCONTEXT &g_pRenderCtx                   = s_pRenderCtx;
LPDIRECTDRAWSURFACE7 &g_pZBuffer_4D6C94       = s_pZBuffer_4D6C94;
LPDIRECTDRAWSURFACE7 &g_pBackBuffer_4D6C90    = s_pBackBuffer_4D6C90;
LPDIRECTDRAWSURFACE7 &g_pPrimaryBuffer_4D6C88 = s_pPrimaryBuffer_4D6C88;
LPDIRECTDRAW7 &g_pDD_4D6C7C                   = s_pDD_4D6C7C;
LPDIRECT3D7 &g_pD3D_4D6C84                    = s_pD3D_4D6C84;
BOOL &g_bD3Dinited_4D6C6C                     = s_bD3Dinited_4D6C6C;

DDPIXELFORMAT &g_ddPixFormat = s_ddPixFormat;
DWORD &g_dwPixelFormat   = s_dwPixelFormat;
DWORD &g_dwBytesPerPixel = s_dwBytesPerPixel;
DWORD &g_dwRGBBitMask    = s_dwRGBBitMask;
DWORD &g_dwGBitMask      = s_dwGBitMask;
DWORD &g_dwRBitMask      = s_dwRBitMask;

LPDIRECTSOUND &g_ppDS                    = s_ppDS;
LPDIRECTSOUND3DLISTENER &g_pds3DListener = s_pds3DListener;
LPDIRECTSOUNDBUFFER &g_pdsBuffer         = s_pdsBuffer;
BOOL &g_bDSInited_5185C4                 = s_bDSInited_5185C4;

CObjItemRef_S208 * &g_pObjectTable       = s_pObjectTable;
int&                g_nObjectTableCount  = s_nObjectTableCount;
CObjPoolMgr_S58* &g_pObjPool = s_pObjPool;

HGDIOBJ             &g_hFont         = s_hFont;
int                 &g_dwCharWidth   = s_dwCharWidth;
int                 &g_dwFontHeight  = s_dwFontHeight;

CHanaLightT3EF_S1A0 * &g_pLight_4D6CE0  = s_pLight_4D6CE0;
CHanaMateT3EE_S184 * &g_pMaterial       = s_pMaterial;
CHanaFrameT3EC_S354 * &g_pLightFrame     = s_pLightFrame;
CHanaFrameT3EC_S354 * &g_pCameraFrame    = s_pCameraFrame;
CHanaFrameRootT3EC_S2D4 * &g_pSceneFrame = s_pSceneFrame;

// ---

DSCAPS g_dsCaps_518560;

float g_CosineTable[360];
float g_SineTable[360];
DDBLTFX g_ddBltFx;
DWORD g_dwBBitMask;

uint32_t g_nBBitCount;
uint32_t g_nRBitCount;
uint32_t g_nRGBBitCount;
DWORD g_dwLowestBBit;
DWORD g_dwLowestRBit;
DWORD g_dwLowestRGBBit;
CHAR  g_szFontFaceName;
LPSTR g_pDDErrStr;

D3DX_DEVICEDESC* g_pDevicesDesc;
DWORD g_nDeviceCount;
IUnknown * g_pUnk_4D6C8C;

#endif

// === FUNCTION POINTERS ===

void (__cdecl * D3DDeinit)() = reinterpret_cast<void(__cdecl*)()>(0x40AC7C);
void (* Renderer_InitializeTrigTablesAndPixelFormat)() = reinterpret_cast<void(*)()>(0x418620);
const char * (__cdecl * HanaDS_SetErrStr)(DWORD hr) = reinterpret_cast<const char *(__cdecl*)(DWORD hr)>(0x423CCC);
BOOL (__cdecl * HanaHeap_Create)(SIZE_T dwInitialSize) = reinterpret_cast<BOOL(__cdecl*)(SIZE_T dwInitialSize)>(0x427520);
int (* HanaDS_InitHeap)() = reinterpret_cast<int(*)()>(0x424886);
int (__cdecl * HanaDS_Init)(HWND hwnd) = reinterpret_cast<int(__cdecl*)(HWND hwnd)>(0x422D50);
int (* HanaHeap_Destroy)() = reinterpret_cast<int(*)()>(0x427551);
int (* HanaDS_DestroyHeap)() = reinterpret_cast<int(*)()>(0x42489D);
void (* HanaDS_Shutdown)() = reinterpret_cast<void(*)()>(0x422E14);
LPDIRECTSOUND3DLISTENER (*HanaDS_Create3DListener)() = reinterpret_cast<LPDIRECTSOUND3DLISTENER(*)()>(0x422690);
int (* Renderer_CleanupFont)() = reinterpret_cast<int(*)()>(0x41888E);
void (__cdecl * SetUiFont_419BCD)(int nCharW, int nFontH, LPCSTR szFaceName) = reinterpret_cast<void(__cdecl*)(int nCharW, int nFontH, LPCSTR szFaceName)>(0x419BCD);
LRESULT (__stdcall * WndProcCheckSystem_485FC2)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = reinterpret_cast<LRESULT(__stdcall*)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>(0x485FC2);
void (*AllocateDevicesDescArray)(void) = reinterpret_cast<void(*)(void)>(0x40AEFB);

// === FUNCTION IMPLEMENTATIONS ===
// 
// D3DDeinit(): full D3D/DDraw teardown, the mirror of D3DInit.
//   Runs only if g_bInited. Frees/releases, in order:
//     - CHanaObj light g_Light & default g_pMaterial (CHanaObj::Free)
//     - pSekHeader (SEK data header, HGLOBAL) & obj16000 (0x16000 3D-object pool) via GlobalFree
//     - D3D buffers g_pZBuffer / g_pBackBuffer / (vestigial) dword_4D6C8C / g_pPrimaryBuffer
//     - scene-graph frames g_pLightFrame / g_pCameraFrame / g_pSceneFrame (CHanaObj::Free)
//     - g_pObjTable (name-keyed frame table, GlobalFree)
//     - COM: g_pDD (IDirectDraw7), g_pD3DDevice, g_pD3D, g_pRenderCtx (D3DXContext)
//   - g_bInited=0; D3DXUninitialize(); if g_nMemCnt!=0 log "MemCnt=N" via PrintDebugError
//   Called on init failure (D3DInit), lost-device wrong-mode (RecoverLostDevice),
//   and app shutdown (WndProcCheckSystem).
void __cdecl hook_D3DDeinit()
{
  ONCELOG("[HNH:SUB] D3DDeinit_40AC7C\n");

  if ( g_bD3Dinited_4D6C6C )
  {
    if ( g_pLight_4D6CE0 )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130*)g_pLight_4D6CE0);
      g_pLight_4D6CE0 = 0;
    }
    if ( g_pMaterial )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130*)g_pMaterial);
      g_pMaterial = 0;
    }
    if ( g_pDevicesDesc )
    {
      MemFree_40DF64(g_pDevicesDesc);
      g_pDevicesDesc = 0;
    }
    if ( g_pObjPool )
    {
      MemFree_40DF64(g_pObjPool);
      g_pObjPool = 0;
    }
    if ( g_pZBuffer_4D6C94 )
    {
      g_pZBuffer_4D6C94->Release();
      g_pZBuffer_4D6C94 = 0;
    }
    if ( g_pBackBuffer_4D6C90 )
    {
      g_pBackBuffer_4D6C90->Release();
      g_pBackBuffer_4D6C90 = 0;
    }
    if ( g_pUnk_4D6C8C )
    {
      g_pUnk_4D6C8C->Release();
      g_pUnk_4D6C8C = 0;
    }
    if ( g_pPrimaryBuffer_4D6C88 )
    {
      g_pPrimaryBuffer_4D6C88->Release();
      g_pPrimaryBuffer_4D6C88 = 0;
    }
    if ( g_pLightFrame )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130*)g_pLightFrame);
      g_pLightFrame = 0;
    }
    if ( g_pCameraFrame )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130*)g_pCameraFrame);
      g_pCameraFrame = 0;
    }
    if ( g_pSceneFrame )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130*)g_pSceneFrame);
      g_pSceneFrame = 0;
    }
    if ( g_pObjectTable )
    {
      MemFree_40DF64(g_pObjectTable);
      g_pObjectTable = 0;
    }
    if ( g_pDD_4D6C7C )
    {
      g_pDD_4D6C7C->Release();
      g_pDD_4D6C7C = 0;
    }
    if ( g_pD3DDevice_4D6C78 )
    {
      g_pD3DDevice_4D6C78->Release();
      g_pD3DDevice_4D6C78 = 0;
    }
    if ( g_pD3D_4D6C84 )
    {
      g_pD3D_4D6C84->Release();
      g_pD3D_4D6C84 = 0;
    }
    if ( g_pRenderCtx )
    {
      g_pRenderCtx->Release();
      g_pRenderCtx = 0;
    }
    g_bD3Dinited_4D6C6C = 0;
    D3DXUninitialize();
    if ( g_nMemCnt_4D6CF0 )
    {
      sprintf(g_szDbgPrefix, "MemCnt=%d", g_nMemCnt_4D6CF0);
      PrintDebugError(-1);
    }
  }
}

void hook_Renderer_InitializeTrigTablesAndPixelFormat()
{
  int i; // [esp+8h] [ebp-8h]
  int j; // [esp+8h] [ebp-8h]
  int k; // [esp+8h] [ebp-8h]
  int bitMask; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] Renderer_InitializeTrigTablesAndPixelFormat_418620\n");

  float two_pi_a = 2.0f * static_cast<float>(M_PI);

  for ( i = 0; i < 360; ++i )
  {
    g_CosineTable[i] = ::cos(i / 360.0f * two_pi_a);
    g_SineTable[i] = ::sin(i / 360.0f * two_pi_a);
  }
  memset(&g_ddBltFx, 0, sizeof(g_ddBltFx));
  g_ddBltFx.dwSize = 100;
  g_ddBltFx.dwROP = 0xCC0020;
  g_ddPixFormat.dwSize = 32;
  g_pPrimaryBuffer_4D6C88->GetPixelFormat(&g_ddPixFormat);
  g_dwPixelFormat = g_ddPixFormat.dwRGBBitCount;
  g_dwBytesPerPixel = g_ddPixFormat.dwRGBBitCount / 8;
  g_dwRGBBitMask = g_ddPixFormat.dwRBitMask;
  g_dwRBitMask = g_ddPixFormat.dwGBitMask;
  g_dwGBitMask = g_ddPixFormat.dwBBitMask;
  g_dwBBitMask = g_ddPixFormat.dwBBitMask | g_ddPixFormat.dwGBitMask | g_ddPixFormat.dwRBitMask;
  g_nBBitCount = 0;
  g_nRBitCount = 0;
  g_nRGBBitCount = 0;
  for ( j = 0; j < 32; ++j )
  {
    if ( ((1 << j) & g_dwRGBBitMask) != 0 )
      ++g_nRGBBitCount;
    if ( ((1 << j) & g_dwRBitMask) != 0 )
      ++g_nRBitCount;
    if ( ((1 << j) & g_dwGBitMask) != 0 )
      ++g_nBBitCount;
  }
  g_dwLowestBBit = 0;
  g_dwLowestRBit = 0;
  g_dwLowestRGBBit = 0;
  bitMask = 1;
  for ( k = 0; k < 32; ++k )
  {
    if ( !g_dwLowestRGBBit && (bitMask & g_dwRGBBitMask) != 0 )
      g_dwLowestRGBBit = bitMask;
    if ( !g_dwLowestRBit && (bitMask & g_dwRBitMask) != 0 )
      g_dwLowestRBit = bitMask;
    if ( !g_dwLowestBBit && (bitMask & g_dwGBitMask) != 0 )
      g_dwLowestBBit = bitMask;
    bitMask *= 2;
  }
  SetUiFont_419BCD(26, 26, &g_szFontFaceName);
}

// HanaDS_SetErrStr - convert a DirectSound error code (DSERR_*/HRESULT) to a human-readable message via DSErrToString_423CE7, cache the resulting message pointer in g_pDDErrStr, and return it. The message points into read-only data (the 'DirectSound ...' strings); DSErrToString_423CE7 returns 0 (NULL) for code 0 (no error). Called by HanaDS_Init on the failure path to record the last error.
const char *__cdecl hook_HanaDS_SetErrStr(DWORD hr)
{
  ONCELOG("[HNH:SUB] HanaDS_SetErrStr_423CCC\n");

  g_pDDErrStr = (LPSTR)DSErrToString_423CE7(hr);
  return g_pDDErrStr;
}

// HanaHeap_Create - lazily create the module's private Win32 heap on first use. If g_hHeap is already set, returns 0 (not re-created); otherwise g_hHeap = HeapCreate(HEAP_NO_SERIALIZE=1, dwInitialSize, 0) and returns g_hHeap != 0. dwInitialSize is the initial commit size in bytes.
BOOL __cdecl hook_HanaHeap_Create(SIZE_T dwInitialSize)
{
  ONCELOG("[HNH:SUB] HanaHeap_Create_427520\n");
  
  if ( g_hHeapHandle )
    return 0;
  g_hHeapHandle = HeapCreate(1u, dwInitialSize, 0);
  return g_hHeapHandle != 0;
}

// HanaDS_InitHeap - initialize the DirectSound/audio subsystem heap to a fixed 40000-byte (0x9C40)
// initial size via HanaHeap_Create(40000). Returns 1 unconditionally (the heap-create result is not =
// propagated to the caller).
int hook_HanaDS_InitHeap()
{
  ONCELOG("[HNH:SUB] HanaDS_InitHeap_424886\n");
  
  HanaHeap_Create(40000u);
  return 1;
}

// HanaDS_Init - initialize the global DirectSound device. Ensures the audio heap is up (HanaDS_InitHeap);
// then DirectSoundCreate(0, &g_ppDS, 0) and SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE=3). 
// On any failure it records the DS error string (sub_423CCC -> g_szDDErr) and returns 0.
// On success it Compact()s the device, zero-fills and queries capabilities into g_dsCaps (dwSize=96), 
// sets g_bDSInited_5185C4=1, and returns 1.
int __cdecl hook_HanaDS_Init(HWND hwnd)
{
  HRESULT hr; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaDS_Init_422D50\n");

  if ( !HanaDS_InitHeap() )
    return 0;
  hr = DirectSoundCreate(0, &g_ppDS, 0);
  if ( hr || (hr = g_ppDS->SetCooperativeLevel(hwnd, 3)) != 0 )
  {
    HanaDS_SetErrStr(hr);
    return 0;
  }
  else
  {
    g_ppDS->Compact();
    memset(&g_dsCaps_518560, 0, sizeof(g_dsCaps_518560));
    g_dsCaps_518560.dwSize = 96;
    g_ppDS->GetCaps(&g_dsCaps_518560);
    g_bDSInited_5185C4 = 1;
    return 1;
  }
}

// HanaHeap_Destroy - destroy the module's private heap (g_hHeap) via HeapDestroy and null g_hHeap. Refuses (returns 0) while g_nHeapRefCount > 0 (heap still has live references); otherwise destroys, clears g_nHeapRefCount and returns 1. Counterpart to HanaHeap_Create.
int hook_HanaHeap_Destroy()
{
  ONCELOG("[HNH:SUB] HanaHeap_Destroy_427551\n");
  
  if ( g_nHeapRefCount > 0 )
     return 0;
  if ( g_hHeapHandle )
    HeapDestroy(g_hHeapHandle);
  g_hHeapHandle = 0;
  g_nHeapRefCount = 0;
  return 1;
}

// HanaDS_DestroyHeap - audio-subsystem hook that tears down the private module heap by calling
// HanaHeap_Destroy(); returns its result. Called as the final step of HanaDS_Shutdown.
int hook_HanaDS_DestroyHeap()
{
  ONCELOG("[HNH:SUB] HanaDS_DestroyHeap_42489D\n");
  
  return HanaHeap_Destroy();
}

// HanaDS_Shutdown - tear down the DirectSound subsystem if initialized: release g_pdsBuffer and g_ppDS,
// null both, clear g_bDSInited_5185C4, then call sub_42489D() (further 3D/audio cleanup) and
// return its result. Returns 0 when not initialized.
void hook_HanaDS_Shutdown()
{
  ONCELOG("[HNH:SUB] HanaDS_Shutdown_422E14\n");

  if ( g_bDSInited_5185C4 )
  {
    if ( g_pdsBuffer )
      g_pdsBuffer->Release();
    if ( g_ppDS )
      g_ppDS->Release();
    g_pdsBuffer = 0;
    g_ppDS = 0;
    g_bDSInited_5185C4 = 0;
    HanaDS_DestroyHeap();
  }
}

// HanaDS_Create3DListener - obtain the 3D sound listener. Requires g_bDSInited. If g_pdsBuffer is not yet created, create it (DSBUFFERDESC dwSize=36, dwFlags=17) via g_ppDS->CreateSoundBuffer; on failure record the error (HanaDS_SetErrStr) and return 0. Then QueryInterface IDirectSound3DListener - on failure record the error, release the buffer, return 0; on success SetDistanceFactor(1.0f) and SetRolloffFactor(~0.1f) and return the listener.
LPDIRECTSOUND3DLISTENER hook_HanaDS_Create3DListener()
{
  DSBUFFERDESC dsbDesc; // [esp+0h] [ebp-2Ch] BYREF
  DWORD hr; // [esp+24h] [ebp-8h]
  LPDIRECTSOUND3DLISTENER pListener; // [esp+28h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] HanaDS_Create3DListener_422690\n");

  if ( !g_bDSInited_5185C4 )
    return 0;
  if ( g_pdsBuffer
    || (memset(&dsbDesc, 0, sizeof(dsbDesc)),
        dsbDesc.dwSize = 36,
        dsbDesc.dwFlags = 17,
        (hr = g_ppDS->CreateSoundBuffer(&dsbDesc, &g_pdsBuffer, 0)) == 0) )
  {
    hr = g_pdsBuffer->QueryInterface(IID_IDirectSound3DListener, reinterpret_cast<void**>(&pListener));
    if ( hr )
    {
      HanaDS_SetErrStr(hr);
      g_pdsBuffer->Release();
      g_pdsBuffer = 0;
      return 0;
    }
    else
    {
      pListener->SetDistanceFactor(1.0f, 1u);
      pListener->SetRolloffFactor(0.001f, 1u);
      return pListener;
    }
  }
  else
  {
    HanaDS_SetErrStr(hr);
    return 0;
  }
}

int hook_Renderer_CleanupFont()
{
    ONCELOG("[HNH:SUB] Renderer_CleanupFont_41888E\n");

    if ( g_hFont )
    {
        DeleteObject(g_hFont);
        g_hFont = 0;
    }
    return 0;
}

// SetUiFont_419BCD(int nCharW, int nFontH, const char *szFaceName)
// 
// Recreate the global UI font: DeleteObject(g_hFont), CreateFontA(nFontH, nCharW/2, weight 400,
// FIXED_PITCH 0x80, DEFAULT_CHARSET, ps 2, CP 0x30, szFaceName), store the char width in
// g_dwCharWidth and the height in g_dwFontHeight. Returns nCharW. Caller then uses
// g_hFont / g_dwCharWidth for all menu text.
void __cdecl hook_SetUiFont_419BCD(int nCharW, int nFontH, LPCSTR szFaceName)
{
  ONCELOG("[HNH:SUB] SetUiFont_419BCD\n");

  if ( g_hFont )
  {
    DeleteObject(g_hFont);
    g_hFont = 0;
  }
  g_hFont = CreateFontA(nFontH, nCharW / 2, 0, 0, 400, 0, 0, 0, 0x80u, 0, 0, 2u, 0x30u, szFaceName);
  g_dwCharWidth = nCharW;
  g_dwFontHeight = nFontH;
}

// shared "quit" epilogue (replaces LABEL_14 and the WM_CLOSE duplication)
static LRESULT QuitWindow(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  DebugLog("[HNH] WM_CLOSE\n");
  DestroyWindow(hWnd);
  return DefWindowProcA(hWnd, msg, wp, lp);
}

static LRESULT OnCheckSystem(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  CHAR szExe[256], szText[256], szCdRoot[256];

  DebugLog("[HNH] OnCheckSystem\n");

  if (!CheckInstalledDirRoot_4869A7(hWnd))
    return QuitWindow(hWnd, msg, wp, lp);

  lstrcpyA(szExe, "HaNaHiMe.exe");
  CHanaObj__GetRealPath_486D9A(szExe, 1001);
  if (!IsFileExists_47E8EE(szExe))
  {
    SetWindowTextA(hWnd, "インストール失敗");
    szExe[0] = '\0';
    CHanaObj__GetRealPath_486D9A(szExe, 1001);
    szExe[lstrlenA(szExe) - 1] = '\0';           // strip trailing '\'
    wsprintfA(szText, "  「華の悲鳴」のインストールは失敗しています。\n"
      "新たに作成された『 %s 』フォルダーを削除してから\n"
      "もう一度インストールしてください。", szExe);
    MessageBoxA(g_hwndParent, szText, "", MB_OK);
    return QuitWindow(hWnd, msg, wp, lp);
  }

  for (;;)                                          // Disk-2 'GM' signature loop
  {
    char szGMLabel[3] = "GM";
    szCdRoot[0] = '\0';
    CHanaObj__GetRealPath_486D9A(szCdRoot, 1000);
    if (CDROM_VerifyDiscSignature(szCdRoot[0], szGMLabel, 4))
      break;
    if (MessageBoxA(hWnd, "「華の悲鳴」のＤｉｓｋ－２を挿入して下さい",
      "", MB_SYSTEMMODAL | MB_OKCANCEL) == IDCANCEL)
    {
      DestroyWindow(hWnd);                     // LABEL_19 path: returns 0,
      return 0;                                // NOT DefWindowProc
    }
  }

  if (!CheckRunGame_4863C8(hWnd, 1))               // title menu + D3D init
  {
    DestroyWindow(hWnd);
    return 0;
  }
  PostMessageA(hWnd, WMU_ENGINE_INIT, 0, 0);
  return DefWindowProcA(hWnd, msg, wp, lp);
}

static LRESULT OnEngineInit(HWND hWnd)
{
    DebugLog("[HNH] OnEngineInit\n");
    if (HanaDS_Init(hWnd) == 1)
    {
      g_pds3DListener = HanaDS_Create3DListener();
      g_pdsBuffer->Play(0, 0, 1);
    }
    else if (MessageBoxA(hWnd,
      "ダイレクトサウンドの初期化に失敗しました。\nゲームを終了します。よろしいですか？",
      "サウンド機能のエラーです。",
      MB_SYSTEMMODAL | MB_YESNO) == IDYES)
    {
      DestroyWindow(hWnd);
      return 0;
    }                                                 // IDNO: continue silently

    Renderer_InitializeTrigTablesAndPixelFormat();
    if (SetFogEnable_40DFB0(g_bFogIsActive_4B3480, g_bFogIsActive_4B3480) == S_OK)
    {
      SetFogMode_40E02D(TRUE, D3DFOG_LINEAR);       // table fog, linear
      SetFogColor_40E0B4(0);                        // black
      SetFogRange_40E0E2(0x40A00000u, 0x437A0000u); // 5.0f .. 250.0f
    }
    ShowCursor(FALSE);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
}

LRESULT __stdcall hook_WndProcCheckSystem_485FC2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  ONCELOG("[HNH:SUB] WndProcCheckSystem_485FC2\n");
 
  g_hwndParent = hWnd;
  LCID lcid;

  switch (Msg)
  {
  case WM_CREATE:
    lcid = GetSystemDefaultLCID();
    DebugLog("[HNH:LCID] lcid = 0x%X\n", (DWORD)lcid);
    if ((lcid & 0xFF) == 0x11)           // Japanese system
    {
      srand(timeGetTime());
      PostMessageA(hWnd, WMU_CHECK_SYSTEM, 0, 0);
    }
    else
    {
      MessageBoxA(hWnd, "このゲームは日本語版Windowsでのみ動作致します。\n"
        "  (This software operates with only the Japanese edition Windows )",
        "誠に申し訳ありません。  ", MB_ICONEXCLAMATION);
      DestroyWindow(hWnd);
    }
    return 0;

  case WMU_CHECK_SYSTEM: return OnCheckSystem(hWnd, Msg, wParam, lParam);
  case WMU_ENGINE_INIT:  return OnEngineInit(hWnd);
  case WM_CLOSE:         return QuitWindow(hWnd, Msg, wParam, lParam);

  case WM_DESTROY:
    DebugLog("[HNH] WM_DESTROY\n");
    ShowCursor(TRUE);
    if (g_pdsBuffer)
      g_pdsBuffer->Stop();
    if (g_pds3DListener)
    {
      g_pds3DListener->Release();
      g_pds3DListener = NULL;
    }
    HanaDS_Shutdown();
    Renderer_CleanupFont();
    D3DDeinit();
    Sleep(100);
    PostQuitMessage(0);
    return 1;

  default:
     return DefWindowProcA(hWnd, Msg, wParam, lParam);
  }
}

void hook_AllocateDevicesDescArray()
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] %s\n", "AllocateDevicesDescArray_40AEFB");

  g_nDeviceCount = D3DXGetDeviceCount();
  g_pDevicesDesc = (D3DX_DEVICEDESC*)MemAlloc_40DEF7(sizeof(D3DX_DEVICEDESC) * g_nDeviceCount);
  for ( i = 0; i < g_nDeviceCount; ++i )
    D3DXGetDeviceDescription(i, &g_pDevicesDesc[i]);
}


// ---

void register_WndProcCheckSystem_485FC2(int cfg) {
  if (cfg == 1) {
    WndProcCheckSystem_485FC2 = hook_WndProcCheckSystem_485FC2;

    D3DDeinit = hook_D3DDeinit;
    Renderer_InitializeTrigTablesAndPixelFormat = hook_Renderer_InitializeTrigTablesAndPixelFormat;
    HanaDS_SetErrStr = hook_HanaDS_SetErrStr;
    HanaHeap_Create = hook_HanaHeap_Create;
    HanaDS_InitHeap = hook_HanaDS_InitHeap;
    HanaDS_Init = hook_HanaDS_Init;
    HanaHeap_Destroy = hook_HanaHeap_Destroy;
    HanaDS_DestroyHeap = hook_HanaDS_DestroyHeap;
    HanaDS_Shutdown = hook_HanaDS_Shutdown;
    HanaDS_Create3DListener = hook_HanaDS_Create3DListener;
    Renderer_CleanupFont = hook_Renderer_CleanupFont;
    SetUiFont_419BCD = hook_SetUiFont_419BCD;
    AllocateDevicesDescArray = hook_AllocateDevicesDescArray;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x40AC7C, hook_D3DDeinit, "D3DDeinit_40AC7C"); // [OK] 
  hnh_Hook(0x418620, hook_Renderer_InitializeTrigTablesAndPixelFormat, "Renderer_InitializeTrigTablesAndPixelFormat_418620"); // [OK] 
  hnh_Hook(0x423CCC, hook_HanaDS_SetErrStr, "HanaDS_SetErrStr_423CCC");
  hnh_Hook(0x485FC2, hook_WndProcCheckSystem_485FC2, "WndProcCheckSystem_485FC2"); // [OK] 
  hnh_Hook(0x427520, hook_HanaHeap_Create, "HanaHeap_Create_427520"); // [OK] 
  hnh_Hook(0x424886, hook_HanaDS_InitHeap, "HanaDS_InitHeap_424886"); // [OK] 
  hnh_Hook(0x422D50, hook_HanaDS_Init, "HanaDS_Init_422D50"); // [OK] 
  hnh_Hook(0x427551, hook_HanaHeap_Destroy, "HanaHeap_Destroy_427551"); // [OK] 
  hnh_Hook(0x42489D, hook_HanaDS_DestroyHeap, "HanaDS_DestroyHeap_42489D"); // [OK] 
  hnh_Hook(0x422E14, hook_HanaDS_Shutdown, "HanaDS_Shutdown_422E14"); // [OK] 
  hnh_Hook(0x422690, hook_HanaDS_Create3DListener, "HanaDS_Create3DListener_422690"); // [OK] 
  hnh_Hook(0x419BCD, hook_SetUiFont_419BCD, "SetUiFont_419BCD"); // [OK] 
  hnh_Hook(0x41888E, hook_Renderer_CleanupFont, "Renderer_CleanupFont_41888E"); // [OK]
  hnh_Hook(0x40AEFB, hook_AllocateDevicesDescArray, "AllocateDevicesDescArray_40AEFB"); // [OK] 
  hnh_HookOn();
}