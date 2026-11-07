// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

#include "vfw.h"

void (__cdecl*PlayVideoWith3DAudio_4791A3)(const char* szMoviePath, int bLoopFlags) =
reinterpret_cast<void(__cdecl*)(const char*, int)>(0x4791A3);
BOOL (__cdecl *CreateAviWnd_43A95B)(HINSTANCE hInstance, HWND hwndParent, LPARAM szMoviePath, int nX, int nY, int nWidth, int nHeight, LPARAM nZoom)
= reinterpret_cast<BOOL (__cdecl*)(HINSTANCE hInstance, HWND hwndParent, LPARAM szMoviePath, int nX, int nY, int nWidth, int nHeight, LPARAM nZoom)>(0x43a95b);
LRESULT (__cdecl *AviGetVolume_43A889)()
= reinterpret_cast<LRESULT (__cdecl*)()>(0x43a889);
void (__cdecl *AviWndSetVolume_43A863)(LPARAM lParam)
= reinterpret_cast<void (__cdecl*)(LPARAM lParam)>(0x43a863);
HWND (__cdecl *GetAviWnd)()
= reinterpret_cast<HWND (__cdecl*)()>(0x43a6f0);
BOOL (__cdecl *CheckIsVideoEndAndFlushMsg_43A6FA)()
= reinterpret_cast<BOOL (__cdecl*)()>(0x43a6fa);
void (__cdecl *AviWnd_Pause)()
= reinterpret_cast<void (__cdecl*)()>(0x43a7d5);
void (__cdecl *AviWnd_Destroy)()
= reinterpret_cast<void (__cdecl*)()>(0x43a7f8);


HWND &g_hwndAvi = *(HWND *)0x5196C0; // WRITABLE
uint32_t &g_nPosAviWnd_5196B8 = *(uint32_t *)0x5196B8; // WRITABLE
uint32_t &g_nAviEnd = *(uint32_t *)0x5196C4; // WRITABLE
uint32_t &g_nPrevPosAviWnd_5196C8 = *(uint32_t *)0x5196C8; // WRITABLE


LPVOID orig_PlayVideoWith3DAudio_4791A3 = NULL;

void __cdecl hook_PlayVideoWith3DAudio_4791A3(const char* szMoviePath, int bLoopFlags) {
  DebugLog("[HNH:SUB] %s path=%s loop=%d\n", "PlayVideoWith3DAudio_4791A3", szMoviePath, bLoopFlags);
  // return;

  // reinterpret_cast<void(__cdecl*)(const char* szMoviePath, int bLoopFlags)>(orig_PlayVideoWith3DAudio_4791A3)(szMoviePath, bLoopFlags);
  int v2; // eax
  int v3; // eax
  int v4; // eax
  int v5; // eax
  int nAviW; // eax
  HWND hAviWnd; // eax
  HWND hAviWnd2; // eax
  int v9; // [esp-8h] [ebp-14Ch]
  int v10; // [esp-8h] [ebp-14Ch]
  int v11; // [esp-8h] [ebp-14Ch]
  int v12; // [esp-8h] [ebp-14Ch]
  int nAviH; // [esp-8h] [ebp-14Ch]
  LPARAM nZoom; // [esp-4h] [ebp-148h]
  LRESULT dwOrigVolume; // [esp+0h] [ebp-144h]
  CHAR szRealPath[256]; // [esp+4h] [ebp-140h] BYREF
  DWORD dwAntialias; // [esp+104h] [ebp-40h] BYREF
  LPDIRECTDRAWCLIPPER pClipper; // [esp+108h] [ebp-3Ch] BYREF
  int nMouseY; // [esp+10Ch] [ebp-38h] BYREF
  int nMouseX; // [esp+110h] [ebp-34h] BYREF
  LPDIRECTDRAWCLIPPER pOldClipper = nullptr; // [esp+114h] [ebp-30h] BYREF
  __int16 wBtnMask; // [esp+118h] [ebp-2Ch] BYREF
  _DWORD arrVolSteps[10]; // [esp+11Ch] [ebp-28h] BYREF

  arrVolSteps[0] = 0;
  arrVolSteps[1] = 125;
  arrVolSteps[2] = 250;
  arrVolSteps[3] = 375;
  arrVolSteps[4] = 500;
  arrVolSteps[5] = 1000;
  arrVolSteps[6] = 1000;
  memset(&arrVolSteps[7], 0, 12);
  g_pDD_4D6C7C->CreateClipper(0, &pClipper, 0);
  if ( pClipper )
  {
    pClipper->SetHWnd(0, g_hwndParent);
    g_pPrimaryBuffer_4D6C88->GetClipper(&pOldClipper);
    g_pPrimaryBuffer_4D6C88->SetClipper(pClipper);
  }
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntialias);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
  v9 = ConvertFrom640AsInt(480);
  v2 = ConvertFrom640AsInt(640);
  BltSurface(g_pBackBuffer_4D6C90, 0, 0, v2, v9, 0);
  v10 = ConvertFrom640AsInt(480);
  v3 = ConvertFrom640AsInt(640);
  BltSurface(g_pPrimaryBuffer_4D6C88, 0, 0, v3, v10, 0);
  if ( g_pdsBuffer )
    g_pdsBuffer->Stop();
  if ( g_pds3DListener )
  {
    g_pds3DListener->Release();
    g_pds3DListener = 0;
  }
  HanaDS_Shutdown();
  wsprintfA(szRealPath, "%s", szMoviePath);
  CHanaObj__GetRealPath_486D9A(szRealPath, 1001);
  if ( !IsFileExists_47E8EE(szRealPath) )
  {
    wsprintfA(szRealPath, "%s", szMoviePath);
    CHanaObj__GetRealPath_486D9A(szRealPath, 1000);
  }
  v11 = ConvertFrom640AsInt(480);
  v4 = ConvertFrom640AsInt(640);
  BltSurface(g_pPrimaryBuffer_4D6C88, 0, 0, v4, v11, 0);
  v12 = ConvertFrom640AsInt(480);
  v5 = ConvertFrom640AsInt(640);
  BltSurface(g_pBackBuffer_4D6C90, 0, 0, v5, v12, 0);
  g_pRenderCtx->UpdateFrame(1);
  g_pDD_4D6C7C->FlipToGDISurface();
  nZoom = ConvertFrom640AsInt(100);
  nAviH = ConvertFrom640AsInt(320);
  nAviW = ConvertFrom640AsInt(640);
  if ( CreateAviWnd_43A95B(g_hInstance, g_hwndParent, (LPARAM)szRealPath, 0, 80, nAviW, nAviH, nZoom) == 1 )
  {
    dwOrigVolume = AviGetVolume_43A889();
    AviWndSetVolume_43A863(dwOrigVolume * arrVolSteps[(unsigned __int8)g_nAviVolumeIndex_530266] / 1000u);
    while ( 1 )
    {
      g_pDD_4D6C7C->FlipToGDISurface();
      hAviWnd = GetAviWnd();
      UpdateWindow(hAviWnd);
      hAviWnd2 = GetAviWnd();
      RedrawWindow(hAviWnd2, 0, 0, 0x400u);
      if ( CheckIsVideoEndAndFlushMsg_43A6FA() == 1 )
        break;
      GetCursorPos_4811FA(&nMouseX, &nMouseY, &wBtnMask);
      DoPeekMessages();
      if ( wBtnMask )
      {
        if ( !bLoopFlags )
          break;
      }
      Sleep(5u);
      g_pDD_4D6C7C->FlipToGDISurface();
    }
    AviWnd_Pause();
    Sleep(0x64u);
    AviWndSetVolume_43A863(dwOrigVolume);
    AviWnd_Destroy();
  }
  if ( HanaDS_Init(g_hwndParent) == 1 )
  {
    g_pds3DListener = HanaDS_Create3DListener();
    g_pdsBuffer->Play(0, 0, 1);
  }
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntialias);
  if ( pClipper )
  {
    g_pPrimaryBuffer_4D6C88->SetClipper(pOldClipper);
    if ( pClipper )
    {
      pClipper->Release();
      pClipper = 0;
    }
  }

  Sleep(0xAu);
}

BOOL __cdecl hook_CreateAviWnd_43A95B(
  HINSTANCE hInstance,
  HWND hwndParent,
  LPARAM szMoviePath,
  int nX,
  int nY,
  int nWidth,
  int nHeight,
  LPARAM nZoom)
{
  ONCELOG("[HNH:SUB] CreateAviWnd_43A95B\n");
  
  if ( !IsFileExists_47E8EE((CHAR*)szMoviePath) )
    return 0;
  if ( VideoForWindowsVersion() >> 16 < 0x10A ) // Win32 API
    return 0;
  g_hwndAvi = MCIWndCreateA(hwndParent, hInstance, 0x5000000Au, 0); // Win32 API
  if ( !g_hwndAvi )
    return 0;
  SetForegroundWindow(g_hwndAvi);
  ShowWindow(g_hwndAvi, 5);
  SendMessageA(g_hwndAvi, MCIWNDM_OPENA, 0, szMoviePath);// 0x499 - 0x0400 = 
  MoveWindow(g_hwndAvi, nX, nY, nWidth, nHeight, 1);
  SendMessageA(g_hwndAvi, MCIWNDM_SETZOOM, 0, nZoom);
  SendMessageA(g_hwndAvi, MCIWNDM_SETTIMEFORMATA, 0, (LPARAM)"frames");
  g_nAviEnd = SendMessageA(g_hwndAvi, MCIWNDM_GETEND, 0, 0);
  SendMessageA(g_hwndAvi, MCI_PLAY, 0, 0);
  MoveWindow(g_hwndAvi, nX, nY, nWidth, nHeight, 1);
  return 1;
}

LRESULT hook_AviGetVolume_43A889()
{
  ONCELOG("[HNH:SUB] AviGetVolume_43A889\n");
  if ( g_hwndAvi )
    return SendMessageA(g_hwndAvi, MCIWNDM_GETVOLUME, 0, 0);
  else
    return 0;
}

void __cdecl hook_AviWndSetVolume_43A863(LPARAM lParam)
{
  ONCELOG("[HNH:SUB] AviWndSetVolume_43A863\n");
 
  if ( g_hwndAvi )
    SendMessageA(g_hwndAvi, MCIWNDM_SETVOLUME, 0, lParam);
}

// // GetAviWnd - return the MCI AVI window handle (g_hwndAvi).
HWND hook_GetAviWnd()
{
  ONCELOG("[HNH:SUB] GetAviWnd_43A6F0\n");
  
  return g_hwndAvi;
}

#ifndef WM_YOMICHAR
#define WM_YOMICHAR             0x0108      /* ;Internal JAPAN */
#endif

BOOL hook_CheckIsVideoEndAndFlushMsg_43A6FA()
{
  struct tagMSG Msg; // [esp+0h] [ebp-1Ch] BYREF

  ONCELOG("[HNH:SUB] CheckIsVideoEndAndFlushMsg_43A6FA\n");
 
  g_nPosAviWnd_5196B8 = SendMessageA(g_hwndAvi, MCIWNDM_GETPOSITIONA, 0, 0);
  if ( g_nPosAviWnd_5196B8 < g_nAviEnd - 1 )
  {
    if ( g_nPosAviWnd_5196B8 == g_nPrevPosAviWnd_5196C8 )
      SendMessageA(g_hwndAvi, MCI_PLAY, 0, 0);
    g_nPrevPosAviWnd_5196C8 = g_nPosAviWnd_5196B8;
    PeekMessageA(&Msg, 0, WM_KEYDOWN, WM_YOMICHAR, PM_REMOVE);
    PeekMessageA(&Msg, 0, WM_MOUSEFIRST, WM_MBUTTONDBLCLK, PM_REMOVE);
    PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE);
    PeekMessageA(&Msg, g_hwndAvi, 0, 0, PM_REMOVE);
    Sleep(5u);
    return 0;
  }
  else
  {
    g_nPrevPosAviWnd_5196C8 = 0;
    return 1;
  }
}

// // AviWnd_Pause - send MCI_PAUSE to the MCI AVI window (g_hwndAvi); returns the SendMessage result (0 if no AVI window).
void hook_AviWnd_Pause()
{
  ONCELOG("[HNH:SUB] AviWnd_Pause_43A7D5\n");

  if ( g_hwndAvi )
    SendMessageA(g_hwndAvi, MCI_PAUSE, 0, 0);
}

// // AviWnd_Destroy - close/destroy the MCI AVI window: send the custom cleanup messages 0x804, 0x806, 0x808 then WM_CLOSE (0x10) to g_hwndAvi, then clear g_hwndAvi. Returns the final SendMessage result.
void hook_AviWnd_Destroy()
{
  ONCELOG("[HNH:SUB] AviWnd_Destroy_43A7F8\n");
 
  if ( g_hwndAvi )
  {
    SendMessageA(g_hwndAvi, MCI_PLAY, 0, 0);
    SendMessageA(g_hwndAvi, MCI_STOP, 0, 0);
    SendMessageA(g_hwndAvi, MCI_CLOSE, 0, 0);
    SendMessageA(g_hwndAvi, MCI_TRACK, 0, 0);
  }
  g_hwndAvi = 0;
}

void register_PlayVideoWith3DAudio_4791A3() {
  orig_PlayVideoWith3DAudio_4791A3 = hnh_Hook(0x4791A3, hook_PlayVideoWith3DAudio_4791A3, "PlayVideoWith3DAudio_4791A3"); // [OK]

  hnh_HookOff();
  hnh_Hook(0x43a95b, hook_CreateAviWnd_43A95B, "CreateAviWnd_43A95B"); // [OK]
  hnh_Hook(0x43a889, hook_AviGetVolume_43A889, "AviGetVolume_43A889"); // [OK]
  hnh_Hook(0x43a863, hook_AviWndSetVolume_43A863, "AviWndSetVolume_43A863"); // [OK]
  hnh_Hook(0x43a6f0, hook_GetAviWnd, "GetAviWnd_43A6F0"); // [OK]
  hnh_Hook(0x43a6fa, hook_CheckIsVideoEndAndFlushMsg_43A6FA, "CheckIsVideoEndAndFlushMsg_43A6FA"); // [OK]
  hnh_Hook(0x43a7d5, hook_AviWnd_Pause, "AviWnd_Pause_43A7D5"); // [OK]
  hnh_Hook(0x43a7f8, hook_AviWnd_Destroy, "AviWnd_Destroy_43A7F8"); // [OK]
  hnh_HookOn();

}