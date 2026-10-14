#include "pch.h"
#include "console.hpp"
#include "shared.hpp"

HANDLE &g_hHeapHandle = *(HANDLE*)0x5187F0;
int &g_nHeapRefCount = *(int*)0x5187F4;
LPDIRECTSOUND &g_ppDS = *(LPDIRECTSOUND*)0x5185C0;
DSCAPS &g_dsCaps_518560 = *(DSCAPS*)0x518560;
int &g_bDSInited_5185C4 = *(int*)0x5185C4;
LPDIRECTSOUNDBUFFER &g_pdsBuffer = *(LPDIRECTSOUNDBUFFER*)0x5185D0;
HGDIOBJ &g_hFont = *(HGDIOBJ*)0x518378;
int &g_dwCharWidth = *(int*)0x4B01B4;
int &g_dwFontHeight = *(int*)0x4B01B8;


float (&g_CosineTable)[360] = *reinterpret_cast<float (*)[360]>(0x517D68);
float (&g_SineTable)[360]   = *reinterpret_cast<float (*)[360]>(0x5177B4);
DDBLTFX &g_ddBltFx          = *reinterpret_cast<DDBLTFX*>(0x518308);
int &g_dwBitDepth           = *reinterpret_cast<int*>(0x517790);
LPDIRECTDRAWSURFACE7 &g_pPrimaryBuffer_4D6C88 = *reinterpret_cast<LPDIRECTDRAWSURFACE7*>(0x4D6C88);
int &g_dwPixelFormat        = *reinterpret_cast<int*>(0x518370);
 
int Renderer_InitializeTrigTablesAndPixelFormat() {

}

char *__cdecl DDErrToString_423CE7(DWORD hr)
{
  if ( hr > 0x88780046 )
  {
    if ( hr > 0x88780096 )
    {
      switch ( hr )
      {
        case 0x887800A0:
          return "DirectSound: この値は過去のものであり使用されない";
        case 0x887800AA:
          return "DirectSound: 他のメソッドが呼び出される前にInitializeメソッドが呼び出されなかったかあるいは呼び出しが成功しなかった";
        case 0u:
          return 0;
      }
    }
    else
    {
      switch ( hr )
      {
        case 0x88780096:
          return "DirectSound: バッファメモリが失われたためリストアしなければならない";
        case 0x88780064:
          return "DirectSound: 指定されたWAVEはサポートされない";
        case 0x88780078:
          return "DirectSound: 使用するサウンドドライバが利用できない";
        case 0x88780082:
          return "DirectSound: オブジェクトはすでに初期化されている";
      }
    }
  }
  else
  {
    if ( hr == 0x88780046 )
      return "DirectSound: 呼び出しは関数が成功するために必要とする優先レベルを持っていない";
    if ( hr > 0x80070057 )
    {
      switch ( hr )
      {
        case 0x8878000A:
          return "DirectSound: 要求は優先レベルのようなリソースが他の呼び出しによってすでに使用中であるため失敗した";
        case 0x8878001E:
          return "DirectSound: 呼び出しによって要求する制御（ボリューム、パンなど）は利用可能でない";
        case 0x88780032:
          return "DirectSound: この関数はこのオブジェクトの現在の状態において有効ではない";
      }
    }
    else
    {
      if ( hr == 0x80070057 )
        return "DirectSound: 無効なパラメータが関数に渡された";
      if ( hr > 0x80040110 )
      {
        if ( hr == 0x8007000E )
          return "DirectSound: DirectSoundサブシステムは呼び出しの要求を完了するために十分なメモリを割り当てることができなかった";
      }
      else
      {
        switch ( hr )
        {
          case 0x80040110:
            return "DirectSound: オブジェクトはCOM集成化をサポートしない";
          case 0x80004001:
            return "DirectSound: 呼び出した関数はここではサポートされない";
          case 0x80004005:
            return "DirectSound: 未確定なエラーがDirectSoundサブシステムの中で起こった";
        }
      }
    }
  }
  return "DirectSound: 不明なエラー";
}

// // HanaDS_SetErrStr - convert a DirectSound error code (DSERR_*/HRESULT) to a human-readable message via DDErrToString_423CE7, cache the resulting message pointer in g_pDDErrStr, and return it. The message points into read-only data (the 'DirectSound ...' strings); DDErrToString_423CE7 returns 0 (NULL) for code 0 (no error). Called by HanaDS_Init on the failure path to record the last error.
char *__cdecl HanaDS_SetErrStr(DWORD hr)
{
  g_pDDErrStr = DDErrToString_423CE7(hr);
  return g_pDDErrStr;
}

// HanaHeap_Create - lazily create the module's private Win32 heap on first use. If g_hHeap is already set, returns 0 (not re-created); otherwise g_hHeap = HeapCreate(HEAP_NO_SERIALIZE=1, dwInitialSize, 0) and returns g_hHeap != 0. dwInitialSize is the initial commit size in bytes.
BOOL __cdecl HanaHeap_Create(SIZE_T dwInitialSize)
{
  if ( g_hHeapHandle )
    return 0;
  g_hHeapHandle = HeapCreate(1u, dwInitialSize, 0);
  return g_hHeapHandle != 0;
}

// HanaDS_InitHeap - initialize the DirectSound/audio subsystem heap to a fixed 40000-byte (0x9C40) initial size via HanaHeap_Create(40000). Returns 1 unconditionally (the heap-create result is not propagated to the caller).
int HanaDS_InitHeap()
{
  HanaHeap_Create(40000u);
  return 1;
}

// HanaDS_Init - initialize the global DirectSound device. Ensures the audio heap is up (HanaDS_InitHeap); then DirectSoundCreate(0, &g_ppDS, 0) and SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE=3). On any failure it records the DS error string (sub_423CCC -> g_szDDErr) and returns 0. On success it Compact()s the device, zero-fills and queries capabilities into g_dsCaps (dwSize=96), sets g_bDSInited_5185C4=1, and returns 1.
int __cdecl HanaDS_Init(HWND hwnd)
{
  HRESULT hr; // [esp+0h] [ebp-4h]

  if ( !HanaDS_InitHeap() )
    return 0;
  hr = DirectSoundCreate(0, &g_ppDS, 0);
  if ( hr || (hr = g_ppDS->lpVtbl->SetCooperativeLevel(g_ppDS, hwnd, 3)) != 0 )
  {
    HanaDS_SetErrStr(hr);
    return 0;
  }
  else
  {
    g_ppDS->lpVtbl->Compact(g_ppDS);
    memset(&g_dsCaps_518560, 0, sizeof(g_dsCaps_518560));
    g_dsCaps_518560.dwSize = 96;
    g_ppDS->lpVtbl->GetCaps(g_ppDS, &g_dsCaps_518560);
    g_bDSInited_5185C4 = 1;
    return 1;
  }
}

// // HanaDS_Create3DListener - obtain the 3D sound listener. Requires g_bDSInited. If g_pdsBuffer is not yet created, create it (DSBUFFERDESC dwSize=36, dwFlags=17) via g_ppDS->CreateSoundBuffer; on failure record the error (HanaDS_SetErrStr) and return 0. Then QueryInterface IDirectSound3DListener - on failure record the error, release the buffer, return 0; on success SetDistanceFactor(1.0f) and SetRolloffFactor(~0.1f) and return the listener.
IDirectSound3DListener *HanaDS_Create3DListener()
{
  DSBUFFERDESC dsbDesc; // [esp+0h] [ebp-2Ch] BYREF
  DWORD hr; // [esp+24h] [ebp-8h]
  IDirectSound3DListener *pListener; // [esp+28h] [ebp-4h] BYREF

  if ( !g_bDSInited_5185C4 )
    return 0;
  if ( g_pdsBuffer
    || (memset(&dsbDesc, 0, sizeof(dsbDesc)),
        dsbDesc.dwSize = 36,
        dsbDesc.dwFlags = 17,
        (hr = g_ppDS->lpVtbl->CreateSoundBuffer(g_ppDS, &dsbDesc, &g_pdsBuffer, 0)) == 0) )
  {
    hr = g_pdsBuffer->lpVtbl->QueryInterface(g_pdsBuffer, &IID_IDirectSound3DListener, &pListener);
    if ( hr )
    {
      HanaDS_SetErrStr(hr);
      g_pdsBuffer->lpVtbl->Release(g_pdsBuffer);
      g_pdsBuffer = 0;
      return 0;
    }
    else
    {
      pListener->lpVtbl->SetDistanceFactor(pListener, 1.0, 1u);
      pListener->lpVtbl->SetRolloffFactor(pListener, 0.001, 1u);
      return pListener;
    }
  }
  else
  {
    HanaDS_SetErrStr(hr);
    return 0;
  }
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

LRESULT __stdcall WndProcCheckSystem_485FC2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  DWORD Time;
  char szGMLabel[3];
  char v8[253];
  CHAR v9[256];
  CHAR Text[256];
  CHAR String[256];

  g_hwndParent = hWnd; /*0x485fce*/
  if ( Msg > 0x10 ) /*0x485fe3*/
  {
    if ( Msg != 0xBD0 ) /*0x486017*/
    {
      if ( Msg != 0x2B10 ) /*0x486023*/
        return DefWindowProcA(hWnd, Msg, wParam, lParam); /*0x486023*/
      if ( HanaDS_Init(hWnd) == 1 ) /*0x486275*/
      {
        g_pds3DListener = HanaDS_Create3DListener(); /*0x4862ad*/
        g_pdsBuffer->lpVtbl->Play(g_pdsBuffer, 0, 0, 1); /*0x4862c6*/
      }
      else if ( MessageBoxA(hWnd, aDirectSoundError, aSoundError, 0x1004u) == 6 ) /*0x486293*/
      {
        DestroyWindow(hWnd); /*0x486299*/
        return 0; /*0x4862a1*/
      }
      Renderer_InitializeTrigTablesAndPixelFormat(); /*0x4862c9*/
      if ( !SetFogEnable_40DFB0(g_bFogEnable_4B3480, g_bFogEnable_4B3480) ) /*0x4862dc*/
      {
        SetFogMode_40E02D(1, 3u); /*0x4862ec*/
        SetFogColor_40E0B4(0); /*0x4862f6*/
        SetFogRange_40E0E2(0x40A00000u, 0x437A0000u); /*0x486308*/
      }
      ShowCursor(0); /*0x486312*/
      InvalidateRect(hWnd, 0, 0); /*0x486320*/
      return 0; /*0x486328*/
    }
    else // Msg == 0xBD0
    {
      if ( !CheckInstalledDirRoot_4869A7(hWnd) ) /*0x486099*/
      {
        DestroyWindow(hWnd); /*0x4860aa*/
        return DefWindowProcA(hWnd, Msg, wParam, lParam); /*0x4863b7*/
      }
      else
      {
        wsprintfA(String, aHanahimeExe); /*0x4860c5*/
        CHanaObj::GetRealPath_486D9A(String, 1001); /*0x4860da*/
        if ( IsFileExists_47E8EE(String) ) /*0x4860e9*/
        {
          bool diskVerified = false;
          while ( 1 ) /*0x48618c*/
          {
            strcpy(szGMLabel, "GM"); /*0x48618c*/
            strcpy(v8, "1"); /*0x4861a1*/
            v9[0] = 0; /*0x4861af*/
            CHanaObj::GetRealPath_486D9A(v9, 1000); /*0x4861c2*/
            if ( CDROM_VerifyDiscSignature(v9[0], szGMLabel, 4u) == 1 ) /*0x4861e6*/
            {
              diskVerified = true;
              break; /*0x4861e6*/
            }
            if ( MessageBoxA(hWnd, aInsertDisk2, byte_52F5D8, 0x1001u) == 2 ) /*0x48620e*/
            {
              break; /*0x48620e*/
            }
          }
          if ( !diskVerified || CheckRunGame_4863C8(hWnd, 1) != 1 ) /*0x48623b*/
          {
            DestroyWindow(hWnd); /*0x486210*/
            return 0; /*0x48621c*/
          }
          PostMessageA(hWnd, 0x2B10u, 0, 0); /*0x48625b*/
        }
        else
        {
          SetWindowTextA(hWnd, aInstallFailed); /*0x486103*/
          String[0] = 0; /*0x486109*/
          CHanaObj::GetRealPath_486D9A(String, 1001); /*0x48611c*/
          Text[lstrlenA(String) + 255] = 0; /*0x486131*/
          wsprintfA( /*0x48614c*/
            Text,
              "  「華の悲鳴」のインストールは失敗しています。\n"
              "新たに作成された『 %s 』フォルダーを削除してから\n"
              "もう一度インストールしてください。",
            String);
          MessageBoxA(g_hwndParent, Text, vv(byte_52F5D4), 0); /*0x48616a*/
          DestroyWindow(hWnd); /*0x486174*/
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam); /*0x48617a*/
      }
    }
  }
  switch ( Msg ) /*0x485fec*/
  {
    case WM_CLOSE: /*0x485fec*/
      DestroyWindow(hWnd); /*0x4860aa*/
      return DefWindowProcA(hWnd, Msg, wParam, lParam); /*0x4863b7*/
    case WM_CREATE: /*0x485fec*/
      if ( GetSystemDefaultLCID() == 17 )       // Check JAPANESE System /*0x486043*/
      {
        Time = timeGetTime(); /*0x48606c*/
        srand(Time); /*0x486073*/
        PostMessageA(hWnd, 0xBD0u, 0, 0); /*0x486088*/
      }
      else
      {
        MessageBoxA(hWnd, aWindowsThisSof, aIamTrulySorry, 0x30u); /*0x486055*/
        DestroyWindow(hWnd); /*0x48605f*/
      }
      return 0; /*0x486065*/
    case WM_DESTROY: /*0x485fec*/
      ShowCursor(1); /*0x48633b*/
      if ( g_pdsBuffer ) /*0x486348*/
        g_pdsBuffer->lpVtbl->Stop(g_pdsBuffer); /*0x486359*/
      if ( g_pds3DListener ) /*0x486363*/
      {
        g_pds3DListener->lpVtbl->Release(g_pds3DListener); /*0x486374*/
        g_pds3DListener = 0; /*0x486377*/
      }
      HanaDS_Shutdown(); /*0x486381*/
      Renderer_CleanupFont(); /*0x486386*/
      D3DDeinit(); /*0x48638b*/
      Sleep(0x64u); /*0x486392*/
      PostQuitMessage(0); /*0x48639a*/
      return 1; /*0x4863a0*/
    default:
      return DefWindowProcA(hWnd, Msg, wParam, lParam); /*0x486002*/
  }
}

void register_WndProcCheckSystem_485FC2() {
    LPVOID addr = (LPVOID)0x485FC2; // <- the function address
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(WndProcCheckSystem_485FC2), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked WndProcCheckSystem_485FC2; %s\n", MH_StatusToString(res));
}