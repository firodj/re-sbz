
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

HANDLE &g_hThread = *(HANDLE*)0x52F3B0;
bool& g_bRequestThreadStop = *(bool*)0x52F3AC;

CInputState *&g_pInputState1_52F3A4 = *(CInputState**) 0x52F3A4;
CInputState *&g_pInputState2_52F3A8 = *(CInputState**) 0x52F3A8;
CInputState *&g_pInputState_4C7354 = *(CInputState**) 0x4C7354;


CInputState (&g_arrInputState1_52F1C0)[30] = *(CInputState (*)[30])0x52F1C0; // WRITABLE
CInputState (&g_arrInputState2_52F2B0)[30] = *(CInputState (*)[30])0x52F2B0; // WRITABLE
int &g_vkBtnRightAlt_4C7330 = *(int *)0x4C7330; // WRITABLE
int &g_vkBtnLeftAlt_4C7334 = *(int *)0x4C7334; // WRITABLE
int &g_vkBtnMidAlt_4C7338 = *(int *)0x4C7338; // WRITABLE
int &g_vkKbdLeft_4C733C = *(int *)0x4C733C; // WRITABLE
int &g_vkKbdRight_4C7340 = *(int *)0x4C7340; // WRITABLE
int &g_vkKbdUp_4C7344 = *(int *)0x4C7344; // WRITABLE
int &g_vkKbdDown_4C7348 = *(int *)0x4C7348; // WRITABLE
uint32_t &g_vKey_4C734C = *(uint32_t *)0x4C734C; // WRITABLE

int &g_nMouseX_52F1B8 = *(int*)0x52F1B8;
int& g_nMouseY_52F3A0 = *(int*)0x52F3A0;

bool& g_bThreadWait_52F3AD = *(bool*)0x52F3AD;
bool& g_bThreadInWaiting_52F3AE = *(bool*)0x52F3AE;

HANDLE(__cdecl* TerminateThreadGracefully)(HANDLE hThread)
= reinterpret_cast<HANDLE(__cdecl*)(HANDLE hThread)>(0x481A35);
HANDLE(__cdecl* CreateNewThread)(HANDLE hPrevThread, DWORD nThreadType)
= reinterpret_cast<HANDLE(__cdecl *)(HANDLE, DWORD)>(0x4818FD);
void (WINAPI*  InputThread_Relative_481637)(LPVOID lpThreadParameter)
= reinterpret_cast<void(WINAPI  *)(LPVOID lpThreadParameter)>(0x481637);
void ( WINAPI*   InputThread_Absolute_4817B1)(LPVOID lpThreadParameter)
= reinterpret_cast< void(WINAPI   *)(LPVOID lpThreadParameter)>(0x4817B1);
bool(__cdecl*GetInputState_4811FA)(int* pOutX, int* pOutY, __int16* pOutBtn)
= reinterpret_cast<bool(__cdecl*)(int* pOutX, int* pOutY, __int16* pOutBtn)>(0x4811FA);

// StopAndRestartThread - Stop current game thread, optionally start a new one
// Called from all game scenes to manage the worker thread
// 
// Arguments:
//   bThreadType: 0 = stop only (then Sleep 10ms)
//                1 = restart with primary thread (ThreadProc_481637)
//                other = restart with secondary thread (ThreadProc2_4817B1)
// 
// Behavior:
//   1. Always: gracefully terminate current hThread
//   2. If bThreadType != 0: create new thread of given type
//   3. If bThreadType == 0: just sleep 10ms (wind-down)
void __cdecl hook_StopAndRestartThread(DWORD nThreadType)
{
  DebugLog("[HNH:SUB] %s\n", "StopAndRestartThread_481AE4");

  g_hThread = TerminateThreadGracefully(g_hThread); // Store terminated handle (NULL) back to hThread
  if ( nThreadType != 0 )
    g_hThread = CreateNewThread(g_hThread, nThreadType);// Store new thread handle in hThread
  else
    Sleep(10u);                                 // Sleep 10ms to let terminated thread wind down
}

HANDLE __cdecl hook_TerminateThreadGracefully(HANDLE hThread)
{
  int i; // [esp+0h] [ebp-Ch]
  int bTerminate; // [esp+4h] [ebp-8h]
  DWORD ExitCode; // [esp+8h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] TerminateThreadGracefully_481A35\n");

  bTerminate = 1;
  g_bRequestThreadStop = 1;
  if ( hThread )
  {
    for ( i = 0; i < 1000; ++i )
    {
      Sleep(5u);
      if ( !GetExitCodeThread(hThread, &ExitCode) )
        break;
      if ( ExitCode != STATUS_PENDING )
      {
        bTerminate = 0;
        break;
      }
      SetThreadPriority(hThread, 0);
    }
    if ( bTerminate == 1 )
      TerminateThread(hThread, 1u);
    Sleep(5u);
    CloseHandle(hThread);
  }
  Sleep(10u);
  return 0;
}

LPVOID orig_CreateNewThread = NULL;

HANDLE __cdecl hook_CreateNewThread(HANDLE hPrevThread, DWORD nThreadType)
{
  int i; // [esp+0h] [ebp-8h]
  DWORD ThreadId; // [esp+4h] [ebp-4h] BYREF
  HANDLE hThread; // [esp+10h] [ebp+8h]

  DebugLog("[HNH:SUB] %s\n", "CreateNewThread_4818FD");
  DebugLog("\tcreate new thread 0x%x, %d\n", hPrevThread, nThreadType);
  // return reinterpret_cast<HANDLE(__cdecl*)(HANDLE hPrevThread, BOOL bThreadType)>(orig_CreateNewThread)(hPrevThread, bThreadType);

  for ( i = 0; i < 30; ++i )
  {
    g_arrInputState1_52F1C0[i].mouseX_0 = ConvertFrom640AsInt(320);
    g_arrInputState1_52F1C0[i].mouseY_2 = ConvertFrom640AsInt(240);
    g_arrInputState1_52F1C0[i].mouseBtn_4 = 0;
    g_arrInputState2_52F2B0[i].mouseX_0 = ConvertFrom640AsInt(320);
    g_arrInputState2_52F2B0[i].mouseY_2 = ConvertFrom640AsInt(240);
    g_arrInputState2_52F2B0[i].mouseBtn_4 = 0;
  }
  g_vkBtnRightAlt_4C7330 = 88;
  g_vkBtnLeftAlt_4C7334 = 90;
  g_vkBtnMidAlt_4C7338 = 32;
  g_vkKbdLeft_4C733C = 37;
  g_vkKbdRight_4C7340 = 39;
  g_vkKbdUp_4C7344 = 38;
  g_vkKbdDown_4C7348 = 40;
  g_vKey_4C734C = 27;
  if ( nThreadType == 1 )
    hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InputThread_Relative_481637, 0, 0, &ThreadId);
  else
    hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InputThread_Absolute_4817B1, 0, 0, &ThreadId);
  SetThreadPriority(hThread, 2);
  return hThread;
}

// Mouse-look input thread (relative / cursor-pinned mode), ~120Hz (Sleep(8)/poll).
// Per iteration: honors g_bThreadWait_52F3AD (busy-wait with g_bThreadInWaiting_52F3AE)
// and g_bRequestThreadStop -> ExitThread; flips the published pointer g_pInputState_4C7354
// between the two 0xE8-byte backing blocks (inputState1_52F1C0 / inputState2_52F2B0,
// aliased by g_pInputState1_52F3A4 / g_pInputState2_52F3A8) and memcpy's 0xE8 bytes into
// the newly-published one so both stay coherent; polls GetInputState_4811FA into
// nTmpX/nTmpY/wTmpBtn and stores them to mouseX_0/mouseY_2/mouseBtn_4/kbdMove_5,
// then sets true_6=1 (fresh-data flag); SetCursorPos(nCenterX,nCenterY) re-pins
// the OS cursor to the virtual center so relative movement is unbounded.
// nCenterX/Y = ConvertFrom640AsInt(320/240).
__noreturn void __stdcall  hook_InputThread_Relative_481637(LPVOID lpThreadParameter)
{
  char bKbdMove; // al
  int nTmpY; // [esp+8h] [ebp-14h] BYREF
  int nTmpX; // [esp+Ch] [ebp-10h] BYREF
  __int16 wTmpBtn; // [esp+10h] [ebp-Ch] BYREF
  int nCenterY; // [esp+14h] [ebp-8h]
  int nCenterX; // [esp+18h] [ebp-4h]

  DebugLog("[HNH:SUB] %s\n", "InputThread_Relative_481637");

  nCenterX = ConvertFrom640AsInt(320);
  nCenterY = ConvertFrom640AsInt(240);
  g_pInputState2_52F3A8 = g_arrInputState1_52F1C0;
  g_pInputState1_52F3A4 = g_arrInputState2_52F2B0;
  g_bRequestThreadStop = 0;
  while ( 1 )
  {
    while ( g_bThreadWait_52F3AD )
    {
      g_bThreadInWaiting_52F3AE = 1;
      if ( g_bRequestThreadStop )
        break;
      Sleep(1u);
    }
    g_bThreadInWaiting_52F3AE = 0;
    if ( g_bRequestThreadStop )
    {
      g_bRequestThreadStop = 0;
      ExitThread(1u);
    }
    if ( g_pInputState_4C7354 == g_pInputState2_52F3A8 )
    {
      g_pInputState_4C7354 = g_pInputState1_52F3A4;
      memcpy(&g_pInputState1_52F3A4[1], g_pInputState2_52F3A8, 0xE8u);
    }
    else
    {
      g_pInputState_4C7354 = g_pInputState2_52F3A8;
      memcpy(&g_pInputState2_52F3A8[1], g_pInputState1_52F3A4, 0xE8u);
    }
    bKbdMove = GetInputState_4811FA(&nTmpX, &nTmpY, &wTmpBtn);
    g_pInputState_4C7354->mouseX_0 = nTmpX;
    g_pInputState_4C7354->mouseY_2 = nTmpY;
    g_pInputState_4C7354->mouseBtn_4 = wTmpBtn;
    g_pInputState_4C7354->kbdMove_5 = bKbdMove;
    g_pInputState_4C7354->true_6 = 1;
    SetCursorPos(nCenterX, nCenterY);
    g_bThreadInWaiting_52F3AE = 1;
    Sleep(8u);
    g_bThreadInWaiting_52F3AE = 0;
  }
}

// Absolute / free-cursor input thread (used for menus & UI where absolute mouse position matters). Identical double-buffer ping-pong + GetInputState_4811FA poll as InputThread_Relative_481637, but WITHOUT the SetCursorPos re-pinning, so the OS cursor moves freely.
__noreturn void __stdcall hook_InputThread_Absolute_4817B1(LPVOID lpThreadParameter)
{
  char bKbdMove; // al
  int nTmpY; // [esp+8h] [ebp-Ch] BYREF
  int nTmpX; // [esp+Ch] [ebp-8h] BYREF
  __int16 wTmpBtn; // [esp+10h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] %s\n", "InputThread_Absolute_4817B1");

  g_pInputState2_52F3A8 = g_arrInputState1_52F1C0;
  g_pInputState1_52F3A4 = g_arrInputState2_52F2B0;
  g_bRequestThreadStop = 0;
  while ( 1 )
  {
    while ( g_bThreadWait_52F3AD )
    {
      g_bThreadInWaiting_52F3AE = 1;
      if ( g_bRequestThreadStop )
        break;
      Sleep(1u);
    }
    g_bThreadInWaiting_52F3AE = 0;
    if ( g_bRequestThreadStop )
    {
      g_bRequestThreadStop = 0;
      ExitThread(1u);
    }
    if ( g_pInputState_4C7354 == g_pInputState2_52F3A8 )
    {
      g_pInputState_4C7354 = g_pInputState1_52F3A4;
      memcpy(&g_pInputState1_52F3A4[1], g_pInputState2_52F3A8, 0xE8u);
    }
    else
    {
      g_pInputState_4C7354 = g_pInputState2_52F3A8;
      memcpy(&g_pInputState2_52F3A8[1], g_pInputState1_52F3A4, 0xE8u);
    }
    bKbdMove = GetInputState_4811FA(&nTmpX, &nTmpY, &wTmpBtn);
    g_pInputState_4C7354->mouseX_0 = nTmpX;
    g_pInputState_4C7354->mouseY_2 = nTmpY;
    g_pInputState_4C7354->mouseBtn_4 = wTmpBtn;
    g_pInputState_4C7354->kbdMove_5 = bKbdMove;
    g_pInputState_4C7354->true_6 = 1;
    g_bThreadInWaiting_52F3AE = 1;
    Sleep(8u);
    g_bThreadInWaiting_52F3AE = 0;
  }
}

// Poll the OS mouse once. GetCursorPos -> *pOutX/*pOutY (ptCursor). Build *pOutBtn as a button mask via GetAsyncKeyState: OR 1 = left button or keybind g_vkBtnLeftAlt_4C7334; OR 2 = right or g_vkBtnRightAlt_4C7330; OR 3 = middle or g_vkBtnMidAlt_4C7338. Keyboard-relative-mouse fallback: if the physical cursor did NOT move this frame (g_nMouseX_52F1B8==ptCursor.x && g_nMouseY_52F3A0==ptCursor.y), each of the 4 keybinds g_vkKbdLeft/Right/Up/Down (4C733C/340/344/348) nudges the virtual position by +/-9 and SetCursorPos applies it. Returns bKbdMove (1 if any keyboard nudge fired, else 0). Caches last-seen cursor in g_nMouseX_52F1B8/g_nMouseY_52F3A0.
bool __cdecl hook_GetInputState_4811FA(int *pOutX, int *pOutY, __int16 *pOutBtn)
{
  __int16 bLeftBtn; // dx
  __int16 bRightBtn; // ax
  __int16 bMidBtn; // cx
  POINT ptCursor; // [esp+0h] [ebp-Ch] BYREF
  bool bKbdMove; // [esp+8h] [ebp-4h]

  static int log_GetInputState_4811FA = 1;
  log_GetInputState_4811FA--;
  if (!log_GetInputState_4811FA) {
    DebugLog("[HNH:SUB] %s\n", "GetInputState_4811FA");
    log_GetInputState_4811FA = 1000;
  }

  bKbdMove = 0;
  Sleep(0);
  GetCursorPos(&ptCursor);
  *pOutX = ptCursor.x;
  *pOutY = ptCursor.y;
  *pOutBtn = 0;
  if ( GetAsyncKeyState(VK_LBUTTON) >> 8 || GetAsyncKeyState(g_vkBtnLeftAlt_4C7334) >> 8 )
  {
    bLeftBtn = *pOutBtn;
    LOBYTE_(bLeftBtn) = *pOutBtn | 1;
    *pOutBtn = bLeftBtn;
  }
  if ( GetAsyncKeyState(VK_RBUTTON) >> 8 || GetAsyncKeyState(g_vkBtnRightAlt_4C7330) >> 8 )
  {
    bRightBtn = *pOutBtn;
    LOBYTE_(bRightBtn) = *pOutBtn | 2;
    *pOutBtn = bRightBtn;
  }
  if ( GetAsyncKeyState(VK_MBUTTON) >> 8 || GetAsyncKeyState(g_vkBtnMidAlt_4C7338) >> 8 )
  {
    bMidBtn = *pOutBtn;
    LOBYTE_(bMidBtn) = *pOutBtn | 3;
    *pOutBtn = bMidBtn;
  }
  if ( g_nMouseX_52F1B8 == ptCursor.x && g_nMouseY_52F3A0 == ptCursor.y )
  {
    if ( GetAsyncKeyState(g_vkKbdLeft_4C733C) >> 8 )
    {
      *pOutX -= 9;
      bKbdMove = 1;
    }
    if ( GetAsyncKeyState(g_vkKbdRight_4C7340) >> 8 )
    {
      *pOutX += 9;
      bKbdMove = 1;
    }
    if ( GetAsyncKeyState(g_vkKbdUp_4C7344) >> 8 )
    {
      *pOutY -= 9;
      bKbdMove = 1;
    }
    if ( GetAsyncKeyState(g_vkKbdDown_4C7348) >> 8 )
    {
      *pOutY += 9;
      bKbdMove = 1;
    }
    SetCursorPos(*pOutX, *pOutY);
  }
  g_nMouseX_52F1B8 = ptCursor.x;
  g_nMouseY_52F3A0 = ptCursor.y;
  return bKbdMove;
}

void register_StopAndRestartThread_481AE4() {
  hnh_Hook(0x481AE4, hook_StopAndRestartThread, "StopAndRestartThread_481AE4");
  hnh_Hook(0x481A35, hook_TerminateThreadGracefully, "TerminateThreadGracefully_481A35");
  orig_CreateNewThread = hnh_Hook(0x4818FD, hook_CreateNewThread, "CreateNewThread_4818FD");
  hnh_Hook(0x481637, hook_InputThread_Relative_481637, "InputThread_Relative_481637");
  hnh_Hook(0x4817B1, hook_InputThread_Absolute_4817B1, "InputThread_Absolute_4817B1");
  hnh_Hook(0x4811FA, hook_GetInputState_4811FA, "GetInputState_4811FA");
}