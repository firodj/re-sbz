#include "d3dx.hpp"
#include "windows.h"
#include <cstring>


_ALLOC_STUB *g_pHead[4093];
unsigned int g_dwNumAllocations;
unsigned int g_dwNumFrees;
unsigned int g_dwTotalUnfreedMemory;
int g_bInit;
unsigned int g_dwDebugLevel;

unsigned int __cdecl CD3duAllocHash(void* pv)
{
  return (unsigned int)pv % 0xFFD;
}

CD3duAlloc::CD3duAlloc()
{
  if ( !g_bInit )
  {
    memset(g_pHead, 0, sizeof(g_pHead));
    g_dwNumAllocations = 0;
    g_dwNumFrees = 0;
    g_dwTotalUnfreedMemory = 0;
    g_bInit = 1;
  }
  //return this;
}

void *__cdecl CD3duAlloc::operator new(unsigned int size)
{
  _ALLOC_STUB *v1; // esi
  void *v2; // eax
  void *v3; // edi
  _ALLOC_STUB **v4; // eax
  _ALLOC_STUB *v5; // ecx

  v1 = new _ALLOC_STUB(); //(_ALLOC_STUB *)operator new(0x10u);
  v2 = operator new(size);
  v3 = v2;
  if ( v1 )
  {
    if ( v2 )
    {
      g_dwTotalUnfreedMemory += size;
      v1->dwAllocationNumber = ++g_dwNumAllocations;
      v4 = &g_pHead[CD3duAllocHash(v2)];
      v5 = *v4;
      v1->pvMem = v3;
      v1->pNext = v5;
      v1->dwSize = size;
      *v4 = v1;
      return v1->pvMem;
    }
    operator delete(v1);
  }
  if ( v3 )
    operator delete(v3);
  D3DXDprintf(0, "CD3duAlloc::new - Memory allocation failed!");
  return 0;
}

void __cdecl CD3duAlloc::operator delete(void *pv)
{
  _ALLOC_STUB *v1; // ebx
  unsigned int v2; // eax
  _ALLOC_STUB *v3; // esi
  _ALLOC_STUB **v4; // edi

  v1 = 0;
  if ( pv )
  {
    v2 = CD3duAllocHash(pv);
    v3 = g_pHead[v2];
    v4 = &g_pHead[v2];
    while ( v3 )
    {
      if ( pv == v3->pvMem )
      {
        operator delete(pv);
        if ( v1 )
          v1->pNext = v3->pNext;
        if ( *v4 == v3 )
          *v4 = v3->pNext;
        g_dwTotalUnfreedMemory -= v3->dwSize;
        ++g_dwNumFrees;
        operator delete(v3);
        return;
      }
      v1 = v3;
      v3 = v3->pNext;
    }
  }
}

CD3DXException::CD3DXException(const char* _message, int _error, int _line) {
  error = _error;
  strcpy_s(message, sizeof(message), _message);
  line = _line;
}

void __cdecl D3DXDprintf(unsigned int lvl, const char *szFormat)
{
  int v2; // eax
  char str[256]; // [esp+0h] [ebp-100h] BYREF

  if ( g_dwDebugLevel >= lvl )
  {
    wsprintfA(str, (LPCSTR)"D3DX: ");
    v2 = lstrlenA(str);
    wsprintfA(&str[v2], szFormat);
    lstrcatA(str, (LPCSTR)"\r\n");
    OutputDebugStringA(str);
  }
}