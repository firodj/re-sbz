#include "pch.h"
#include "console.hpp"
#include "shared.hpp"

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
        vv(g_pds3DListener) = HanaDS_Create3DListener(); /*0x4862ad*/
        vv(g_pdsBuffer)->lpVtbl->Play(vv(g_pdsBuffer), 0, 0, 1); /*0x4862c6*/
      }
      else if ( MessageBoxA(hWnd, aDirectSoundError, aSoundError, 0x1004u) == 6 ) /*0x486293*/
      {
        DestroyWindow(hWnd); /*0x486299*/
        return 0; /*0x4862a1*/
      }
      Renderer_InitializeTrigTablesAndPixelFormat(); /*0x4862c9*/
      if ( !SetFogEnable_40DFB0(vv(g_bFogEnable_4B3480), vv(g_bFogEnable_4B3480)) ) /*0x4862dc*/
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
            if ( MessageBoxA(hWnd, aInsertDisk2, vv(byte_52F5D8), 0x1001u) == 2 ) /*0x48620e*/
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
      if ( vv(g_pdsBuffer) ) /*0x486348*/
        vv(g_pdsBuffer)->lpVtbl->Stop(vv(g_pdsBuffer)); /*0x486359*/
      if ( vv(g_pds3DListener) ) /*0x486363*/
      {
        vv(g_pds3DListener)->lpVtbl->Release(vv(g_pds3DListener)); /*0x486374*/
        vv(g_pds3DListener) = 0; /*0x486377*/
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