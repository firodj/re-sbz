// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===

static int s_nMemCnt_4D6CF0;
static char s_szDbgPrefix[104];
static int s_nMinD3DPrintfLevel_534388;
static HANDLE s_hHeapHandle;
static int s_nHeapRefCount;

#ifdef _USRDLL
int   &g_nMemCnt_4D6CF0 = *(int*)0x4D6CF0;
char (&g_szDbgPrefix)[104] = *(char(*)[104])0x4D4718;
int   &g_nMinD3DPrintfLevel_534388 = *(int*)0x534388;

HANDLE  &g_hHeapHandle   = *(HANDLE*)0x5187F0;
int     &g_nHeapRefCount = *(int*)0x5187F4;
#else

int    &g_nMemCnt_4D6CF0 = s_nMemCnt_4D6CF0;
char   (&g_szDbgPrefix)[104] = s_szDbgPrefix;
int    &g_nMinD3DPrintfLevel_534388 = s_nMinD3DPrintfLevel_534388;
HANDLE &g_hHeapHandle = s_hHeapHandle;
int    &g_nHeapRefCount = s_nHeapRefCount;

#endif

// === GLOBAL FUNCTIONS ===
void(__cdecl* D3DXDprintf)(int nLevel, LPCSTR szMsg) = reinterpret_cast<void(__cdecl*)(int nLevel, LPCSTR szMsg)>(0x490AC6);
int (__cdecl * MemFree_40DF64)(void *pBuf) = reinterpret_cast<int(__cdecl*)(void *pBuf)>(0x40DF64);
void(__cdecl* PrintDebugError)(int hr) = reinterpret_cast<void(__cdecl*)(int)>(0x40A813);
LPVOID (__cdecl * MemAlloc_40DEF7)(SIZE_T dwBytes) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T dwBytes)>(0x40DEF7);
LPVOID(__cdecl* AllocMem)(SIZE_T size) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T)>(0x4248A7);
BOOL(__cdecl* SafeFree)(LPVOID lpMem) = reinterpret_cast<BOOL(__cdecl*)(LPVOID)>(0x4248D2);
LPVOID(__cdecl* HeapAllocRef)(SIZE_T size) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T size)>(0x427591);
BOOL(__cdecl* HeapFreeRef)(LPVOID lpMem) = reinterpret_cast<BOOL(__cdecl*)(LPVOID lpMem)>(0x4275DF);

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
 * @param dwBytes Number of bytes to allocate (excluding the extra 128‚Äëbyte overhead).
 * @return Pointer to allocated memory, or NULL on failure.
 */
LPVOID __cdecl hook_MemAlloc_40DEF7(SIZE_T dwBytes)
{
  ONCELOG("[HNH:SUB] MemAlloc_40DEF7\n");
  

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

LPVOID __cdecl hook_HeapAllocRef(SIZE_T size)
{
  LPVOID pMem; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] HeapAllocRef_427591\n");
 

  if ( !g_hHeapHandle )
    return 0;
  pMem = HeapAlloc(g_hHeapHandle, 1u, size);
  if ( !pMem )
    return 0;
  ++g_nHeapRefCount;
  return pMem;
}

BOOL __cdecl hook_HeapFreeRef(LPVOID lpMem)
{
  ONCELOG("[HNH:SUB] HeapFreeRef_4275DF\n");
 

  if ( !lpMem )
    return 0;
  if ( !g_nHeapRefCount )
    return 0;
  HeapFree(g_hHeapHandle, 1u, lpMem);
  --g_nHeapRefCount;
  return 1;
}


LPVOID __cdecl hook_AllocMem(SIZE_T size)
{
  LPVOID pMem; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] AllocMem_4248A7\n");
  

  pMem = HeapAllocRef(size);
  if ( pMem )
    return pMem;
  else
    return 0;
}

BOOL __cdecl hook_SafeFree(LPVOID lpMem)
{
  ONCELOG("[HNH:SUB] SafeFree_4248D2\n");
 
  if ( !lpMem )
    return 0;
  HeapFreeRef(lpMem);
  return 1;
}

int __cdecl hook_MemFree_40DF64(void *pBuf)
{
  ONCELOG("[HNH:SUB] MemFree_40DF64\n");
 

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

void register_MemAlloc_40DEF7(int cfg) {

  g_nMinD3DPrintfLevel_534388 = -1;

  if (cfg == 1) {
    D3DXDprintf = hook_D3DXDprintf;
    PrintDebugError = hook_PrintDebugError;
    
    MemFree_40DF64 = hook_MemFree_40DF64;
    MemAlloc_40DEF7 = hook_MemAlloc_40DEF7;
    AllocMem = hook_AllocMem;
    SafeFree = hook_SafeFree;
    HeapAllocRef = hook_HeapAllocRef;
    HeapFreeRef = hook_HeapFreeRef;
  }
  else {
    hnh_Hook(0x490AC6, hook_D3DXDprintf, "D3DXDprintf_490AC6");  // [OK] 
    hnh_Hook(0x40A813, hook_PrintDebugError, "PrintDebugError_40A813"); 
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x40DEF7, hook_MemAlloc_40DEF7, "MemAlloc_40DEF7"); // [OK] 
  hnh_Hook(0x40DF64, hook_MemFree_40DF64, "MemFree_40DF64"); // [OK] 
  hnh_Hook(0x4248A7, hook_AllocMem, "AllocMem_4248A7"); // [OK] 
  hnh_Hook(0x4248D2, hook_SafeFree, "SafeFree_4248D2"); // [OK] 
  hnh_Hook(0x427591, hook_HeapAllocRef, "HeapAllocRef_427591"); // [OK] 
  hnh_Hook(0x4275DF, hook_HeapFreeRef, "HeapFreeRef_4275DF"); // [OK] 
  hnh_HookOn();
}