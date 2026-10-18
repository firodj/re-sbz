#include "dx7.hpp"

#include <cmath>
#include <corecrt_math_defines.h>

#include "console.hpp"
#include "shared.hpp"
#include "struct.hpp"

HANDLE &g_hHeapHandle   = *(HANDLE*)0x5187F0;
int &g_nHeapRefCount    = *(int*)0x5187F4;
LPDIRECTSOUND &g_ppDS   = *(LPDIRECTSOUND*)0x5185C0;
DSCAPS &g_dsCaps_518560 = *(DSCAPS*)0x518560;
int &g_bDSInited_5185C4 = *(int*)0x5185C4;
LPDIRECTSOUNDBUFFER &g_pdsBuffer = *(LPDIRECTSOUNDBUFFER*)0x5185D0;
HGDIOBJ &g_hFont    = *(HGDIOBJ*)0x518378;
int &g_dwCharWidth  = *(int*)0x4B01B4;
int &g_dwFontHeight = *(int*)0x4B01B8;

LPDIRECTSOUND3DLISTENER &g_pds3DListener = *(LPDIRECTSOUND3DLISTENER*)0x52F3C4;

float (&g_CosineTable)[360] = *(float (*)[360])0x517D68; // WRITABLE
float (&g_SineTable)[360]   = *(float (*)[360])0x5177B4; // WRITABLE
DDBLTFX &g_ddBltFx          = *(DDBLTFX *)0x518308; // WRITABLE
DDPIXELFORMAT &g_ddPixFormat = *(DDPIXELFORMAT *)0x517790; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pPrimaryBuffer_4D6C88 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C88; // WRITABLE
uint32_t &dword_51779C      = *(uint32_t *)0x51779C; // WRITABLE
uint32_t &g_dwPixelFormat   = *(uint32_t *)0x518370; // WRITABLE
uint32_t &g_dwBytesPerPixel = *(uint32_t *)0x518374; // WRITABLE
uint32_t &dword_5177A0      = *(uint32_t *)0x5177A0; // WRITABLE
uint32_t &g_dwRGBBitMask    = *(uint32_t *)0x517788; // WRITABLE
uint32_t &dword_5177A4      = *(uint32_t *)0x5177A4; // WRITABLE
uint32_t &g_dwRBitMask      = *(uint32_t *)0x517D58; // WRITABLE
uint32_t &dword_5177A8      = *(uint32_t *)0x5177A8; // WRITABLE
uint32_t &g_dwGBitMask      = *(uint32_t *)0x517D5C; // WRITABLE
uint32_t &g_dwBBitMask      = *(uint32_t *)0x517D60; // WRITABLE
uint32_t &g_nBBitCount      = *(uint32_t *)0x4B01C4; // WRITABLE
uint32_t &g_nRBitCount      = *(uint32_t *)0x4B01C0; // WRITABLE
uint32_t &g_nRGBBitCount    = *(uint32_t *)0x4B01BC; // WRITABLE
uint32_t &g_dwLowestBBit    = *(uint32_t *)0x518384; // WRITABLE
uint32_t &g_dwLowestRBit    = *(uint32_t *)0x518380; // WRITABLE
uint32_t &g_dwLowestRGBBit  = *(uint32_t *)0x51837C; // WRITABLE
CHAR &g_szFontFaceName      = *(CHAR *)0x4B01C8; // WRITABLE
LPSTR &g_pDDErrStr = *(LPSTR*)0x5185CC;

int __cdecl SetUiFont_419BCD(int nCharW, int nFontH, LPCSTR szFaceName);

// ---
BOOL (__cdecl* CheckRunGame_4863C8)(HWND hWnd, int bFullScreen) = reinterpret_cast<BOOL (__cdecl *)(HWND, int)>(0x4863C8);
void __cdecl D3DDeinit();

uint32_t &g_bInited_4D6C6C              = *(uint32_t *)0x4D6C6C; // WRITABLE
CHanaLightT3EF_S1A0 * &g_pLight_4D6CE0  = *(CHanaLightT3EF_S1A0 * *)0x4D6CE0; // WRITABLE
CHanaMateT3EE_S184 * &g_pMaterial       = *(CHanaMateT3EE_S184 * *)0x4D6CDC; // WRITABLE
HGLOBAL &g_pSekHeader                   = *(HGLOBAL *)0x4D6CCC; // WRITABLE
CObjPoolMgr_S58 * &g_pObjPool           = *(CObjPoolMgr_S58 * *)0x4D6CD0; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pZBuffer_4D6C94    = *(LPDIRECTDRAWSURFACE7 *)0x4D6C94; // WRITABLE
LPDIRECTDRAWSURFACE7 &g_pBackBuffer_4D6C90 = *(LPDIRECTDRAWSURFACE7 *)0x4D6C90; // WRITABLE
IUnknown * &g_pUnk_4D6C8C                  = *(IUnknown * *)0x4D6C8C; // WRITABLE

CHanaFrameT3EC_S354 * &g_pLightFrame     = *(CHanaFrameT3EC_S354 * *)0x4D6CB0; // WRITABLE
CHanaFrameT3EC_S354 * &g_pCameraFrame    = *(CHanaFrameT3EC_S354 * *)0x4D6CAC; // WRITABLE
CHanaFrameRootT3EC_S2D4 * &g_pSceneFrame = *(CHanaFrameRootT3EC_S2D4 * *)0x4D6CA8; // WRITABLE
CObjItemRef_S208 * &g_pObjectTable       = *(CObjItemRef_S208 * *)0x4D6C98; // WRITABLE
int& g_nObjectTableCount                 = *(int*)0x4D6C9C;
LPDIRECTDRAW7 &g_pDD_4D6C7C              = *(LPDIRECTDRAW7 *)0x4D6C7C; // WRITABLE
LPDIRECT3D7 &g_pD3D_4D6C84               = *(LPDIRECT3D7 *)0x4D6C84; // WRITABLE
D3DXContext * &g_pRenderCtx              = *(D3DXContext * *)0x4D6C80; // WRITABLE
uint32_t &g_nMemCnt_4D6CF0               = *(uint32_t *)0x4D6CF0; // WRITABLE

BOOL &g_bFogIsActive_4B3480 = *(BOOL*)0x4B3480;

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
void __cdecl D3DDeinit()
{
    if ( g_bInited_4D6C6C )
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
        if ( g_pSekHeader )
        {
            GlobalFree_40DF64(g_pSekHeader);
            g_pSekHeader = 0;
        }
        if ( g_pObjPool )
        {
            GlobalFree_40DF64(g_pObjPool);
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
            GlobalFree_40DF64(g_pObjectTable);
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
        g_bInited_4D6C6C = 0;
        D3DXUninitialize();
        if ( g_nMemCnt_4D6CF0 )
        {
            sprintf(g_szDbgPrefix, "MemCnt=%d", g_nMemCnt_4D6CF0);
            PrintDebugError(-1);
        }
    }
}

int Renderer_InitializeTrigTablesAndPixelFormat()
{
  int i; // [esp+8h] [ebp-8h]
  int j; // [esp+8h] [ebp-8h]
  int k; // [esp+8h] [ebp-8h]
  int bitMask; // [esp+Ch] [ebp-4h]

  DebugLog("[HNH:SUB] Renderer_InitializeTrigTablesAndPixelFormat_418620\n");

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
  return 0;
}

// HanaDS_SetErrStr - convert a DirectSound error code (DSERR_*/HRESULT) to a human-readable message via DDErrToString_423CE7, cache the resulting message pointer in g_pDDErrStr, and return it. The message points into read-only data (the 'DirectSound ...' strings); DDErrToString_423CE7 returns 0 (NULL) for code 0 (no error). Called by HanaDS_Init on the failure path to record the last error.
const char *__cdecl HanaDS_SetErrStr(DWORD hr)
{
  DebugLog("[HNH:SUB] HanaHeap_Create_423CCC\n");

  g_pDDErrStr = (LPSTR)DDErrToString_423CE7(hr);
  return g_pDDErrStr;
}

// HanaHeap_Create - lazily create the module's private Win32 heap on first use. If g_hHeap is already set, returns 0 (not re-created); otherwise g_hHeap = HeapCreate(HEAP_NO_SERIALIZE=1, dwInitialSize, 0) and returns g_hHeap != 0. dwInitialSize is the initial commit size in bytes.
BOOL __cdecl HanaHeap_Create(SIZE_T dwInitialSize)
{
  DebugLog("[HNH:SUB] HanaHeap_Create_427520\n");
  
  if ( g_hHeapHandle )
    return 0;
  g_hHeapHandle = HeapCreate(1u, dwInitialSize, 0);
  return g_hHeapHandle != 0;
}

// HanaDS_InitHeap - initialize the DirectSound/audio subsystem heap to a fixed 40000-byte (0x9C40) initial size via HanaHeap_Create(40000). Returns 1 unconditionally (the heap-create result is not propagated to the caller).
int HanaDS_InitHeap()
{
  DebugLog("[HNH:SUB] HanaDS_InitHeap_424886\n");
  
  HanaHeap_Create(40000u);
  return 1;
}

// HanaDS_Init - initialize the global DirectSound device. Ensures the audio heap is up (HanaDS_InitHeap); then DirectSoundCreate(0, &g_ppDS, 0) and SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE=3). On any failure it records the DS error string (sub_423CCC -> g_szDDErr) and returns 0. On success it Compact()s the device, zero-fills and queries capabilities into g_dsCaps (dwSize=96), sets g_bDSInited_5185C4=1, and returns 1.
int __cdecl HanaDS_Init(HWND hwnd)
{
  HRESULT hr; // [esp+0h] [ebp-4h]

  DebugLog("[HNH:SUB] HanaDS_Init_422D50\n");

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
int HanaHeap_Destroy()
{
    if ( g_nHeapRefCount > 0 )
        return 0;
    if ( g_hHeapHandle )
        HeapDestroy(g_hHeapHandle);
    g_hHeapHandle = 0;
    g_nHeapRefCount = 0;
    return 1;
}

// HanaDS_DestroyHeap - audio-subsystem hook that tears down the private module heap by calling HanaHeap_Destroy(); returns its result. Called as the final step of HanaDS_Shutdown.
int HanaDS_DestroyHeap()
{
    return HanaHeap_Destroy();
}

// HanaDS_Shutdown - tear down the DirectSound subsystem if initialized: release g_pdsBuffer and g_ppDS, null both, clear g_bDSInited_5185C4, then call sub_42489D() (further 3D/audio cleanup) and return its result. Returns 0 when not initialized.
void HanaDS_Shutdown()
{
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
IDirectSound3DListener *HanaDS_Create3DListener()
{
  DSBUFFERDESC dsbDesc; // [esp+0h] [ebp-2Ch] BYREF
  DWORD hr; // [esp+24h] [ebp-8h]
  IDirectSound3DListener *pListener; // [esp+28h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] HanaDS_Create3DListener_422690\n");

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

int Renderer_CleanupFont()
{
    DebugLog("[HNH:SUB] Renderer_CleanupFont_0x41888E\n");

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
int __cdecl SetUiFont_419BCD(int nCharW, int nFontH, LPCSTR szFaceName)
{
  int result; // eax

  DebugLog("[HNH:SUB] SetUiFont_419BCD\n");

  if ( g_hFont )
  {
    DeleteObject(g_hFont);
    g_hFont = 0;
  }
  g_hFont = CreateFontA(nFontH, nCharW / 2, 0, 0, 400, 0, 0, 0, 0x80u, 0, 0, 2u, 0x30u, szFaceName);
  result = nCharW;
  g_dwCharWidth = nCharW;
  g_dwFontHeight = nFontH;
  return result;
}

HRESULT __cdecl SetFogColor_40E0B4(DWORD dwFogColor)
{
    DebugLog("[HNH:SUB] SetFogColor_40E0B4\n");

    g_dwFogColor_4B00CC = dwFogColor;
    return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);
}

HRESULT __cdecl SetFogRange_40E0E2(DWORD dwFogStart, DWORD dwFogEnd)
{
    HRESULT hr; // [esp+0h] [ebp-4h]

    DebugLog("[HNH:SUB] SetFogRange_40E0E2\n");

    hr = 0;
    if ( g_dwFogMode_4B00C8 == 3 )
    {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGSTART, dwFogStart);
        return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGEND, dwFogEnd);
    }
    return hr;
}

// ---
#define WMU_CHECK_SYSTEM 0x0BD0   // posted by WM_CREATE: install/disc/config gate
#define WMU_ENGINE_INIT  0x2B10   // posted on success: sound + renderer init

// shared "quit" epilogue (replaces LABEL_14 and the WM_CLOSE duplication)
static LRESULT QuitWindow(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    DestroyWindow(hWnd);
    return DefWindowProcA(hWnd, msg, wp, lp);
}

static LRESULT OnCheckSystem(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    CHAR szExe[256], szText[256], szCdRoot[256];

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

LRESULT __stdcall WndProcCheckSystem_485FC2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    static bool notyet = true;
    if (notyet) {
        DebugLog("[HNH:SUB] WndProcCheckSystem_485FC2\n"); notyet = false;
    }

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

// ---

void register_WndProcCheckSystem_485FC2() {
    LPVOID addr = (LPVOID)0x485FC2; // <- the function address
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(WndProcCheckSystem_485FC2), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked WndProcCheckSystem_485FC2; %s\n", MH_StatusToString(res));
}