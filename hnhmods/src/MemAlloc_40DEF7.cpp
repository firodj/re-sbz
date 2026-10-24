
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===
int &g_nMemCnt_4D6CF0 = *(int*)0x4D6CF0;
char (&g_szDbgPrefix)[104] = *(char(*)[104])0x4D4718;
int &g_nMinD3DPrintfLevel_534388 = *(int*)0x534388;

// === GLOBAL FUNCTIONS ===
void(__stdcall *D3DXGetErrorString)(int hr, size_t strLength, char* str) = reinterpret_cast<void (__stdcall*)(int hr, size_t strLength, char* str)>(0x48c80e);
void(__cdecl* D3DXDprintf)(int nLevel, LPCSTR szMsg) = reinterpret_cast<void(__cdecl*)(int nLevel, LPCSTR szMsg)>(0x490AC6);
int (__cdecl * MemFree_40DF64)(void *pBuf) = reinterpret_cast<int(__cdecl*)(void *pBuf)>(0x40DF64);
void(__cdecl* PrintDebugError)(int hr) = reinterpret_cast<void(__cdecl*)(int)>(0x40A813);
LPVOID (__cdecl * MemAlloc_40DEF7)(SIZE_T dwBytes) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T dwBytes)>(0x40DEF7);

// === FUNCTION IMPLEMENTATIONS ===

void __cdecl hook_D3DXDprintf(int nLevel, LPCSTR szMsg)
{
  int v2; // eax
  CHAR String[256]; // [esp+0h] [ebp-100h] BYREF

  if ( g_nMinD3DPrintfLevel_534388 >= nLevel )
  {
    wsprintfA(String, "[D3DX] ");
    v2 = lstrlenA(String);
    wsprintfA(&String[v2], szMsg);

    lstrcatA(String, "\n");
    OutputDebugStringA(String);
  }
}

void __cdecl hook_PrintDebugError(int hr)
{
  CHAR str[512]; // [esp+0h] [ebp-200h] BYREF

  D3DXGetErrorString(hr, 0x100u, str);
  strcat_s(str, " ");
  strcat_s(str, g_szDbgPrefix);
  strcat_s(str, "\n");
  OutputDebugStringA("---- D3DXL start ------------------------------------------\n");
  OutputDebugStringA(str);
  OutputDebugStringA("----D3DXL end------------------------------------------\n");
}

/**
 * Allocates memory with zero initialization and tracks allocation count.
 * @param dwBytes Number of bytes to allocate (excluding the extra 128‑byte overhead).
 * @return Pointer to allocated memory, or NULL on failure.
 */
LPVOID __cdecl hook_MemAlloc_40DEF7(SIZE_T dwBytes)
{
  static int log_MemAlloc_40DEF7 = 1;
  log_MemAlloc_40DEF7--;
  if (!log_MemAlloc_40DEF7) {
    DebugLog("[HNH:SUB] MemAlloc_40DEF7\n");
    log_MemAlloc_40DEF7 = 1000;
  }

  LPVOID ptr; // Allocated memory pointer

  // Allocate memory with zero initialization, adding 128 bytes overhead
  ptr = GlobalAlloc(GMEM_ZEROINIT, dwBytes + 128);
  if (ptr)
  {
    // Increment global allocation counter
    ++g_nMemCnt_4D6CF0;
    return ptr;
  }
  else
  {
    // Format error message into Buffer
    sprintf(g_szDbgPrefix, "MemAlloc");
    // Print debug error code (out of memory)
    PrintDebugError(0x8007000E);
    return NULL;
  }
}

int __cdecl hook_MemFree_40DF64(void *pBuf)
{
  static int log_MemFree_40DF64 = 1;
  log_MemFree_40DF64--;
  if (!log_MemFree_40DF64) {
    DebugLog("[HNH:SUB] MemFree_40DF64\n");
    log_MemFree_40DF64 = 1000;
  }

  if ( pBuf )
  {
    GlobalFree(pBuf);
    --g_nMemCnt_4D6CF0;
    return 1;
  }
  else
  {
    sprintf(g_szDbgPrefix, "MemFree Ptr=NULL");
    PrintDebugError(0x80004005);
    return 0;
  }
}

void register_MemAlloc_40DEF7() {
  hnh_Hook(0x40DEF7, hook_MemAlloc_40DEF7, "MemAlloc_40DEF7");
  hnh_Hook(0x40DF64, hook_MemFree_40DF64, "MemFree_40DF64");
  hnh_Hook(0x40A813, hook_PrintDebugError, "PrintDebugError_40A813");

  g_nMinD3DPrintfLevel_534388 = -1;

  hnh_Hook(0x490AC6, hook_D3DXDprintf, "D3DXDprintf_490AC6");
}