
#include "pch.h"
#include "console.hpp"
#include "shared.hpp"

int &g_nMemCnt_4D6CF0 = *(int*)0x4D6CF0;

// char aDbgPrefix_[104] = {0};
LPSTR &aDbgPrefix = *(LPSTR*)0x4D4718;

typedef std::add_pointer_t<void __stdcall(int hr, size_t strLength, char* str)> D3DXGetErrorStringFn;
const  D3DXGetErrorStringFn D3DXGetErrorString = (D3DXGetErrorStringFn)0x48c80e;

void __cdecl PrintDebugError(int hr)
{
    CHAR str[512]; // [esp+0h] [ebp-200h] BYREF

    D3DXGetErrorString(hr, 0x100u, str);
    strcat_s(str, " ");
    strcat_s(str, aDbgPrefix);
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
LPVOID __cdecl GAlloc80_40DEF7(SIZE_T dwBytes)
{
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
        sprintf(aDbgPrefix, "MemAlloc");
        // Print debug error code (out of memory)
        PrintDebugError(0x8007000E);
        return NULL;
    }
}

void register_GAlloc80_40DEF7() {
    LPVOID addr = (LPVOID)0x40DEF7;
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(GAlloc80_40DEF7), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked GAlloc80_40DEF7; %s\n", MH_StatusToString(res));
}