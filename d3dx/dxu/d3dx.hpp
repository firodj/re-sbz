#pragma once

#include "d3dxcore.h"

#ifndef __cppobj
#define __cppobj
#endif

#define SKIP_ME 1

struct LINEENTRY;
struct BLT_DATA;

struct _ALLOC_STUB // sizeof=0x10
{
    void* pvMem;
    unsigned int dwSize;
    unsigned int dwAllocationNumber;
    _ALLOC_STUB* pNext;
};

template <typename T>
class __cppobj TArray // sizeof=0xC
{
public:
    T* Data;
    int Size;
    int Count;

    int Add(T* Element);
};

class __cppobj CD3duAlloc // sizeof=0x0
{
public:
    CD3duAlloc();

    static void  * operator new(size_t size);
    static void  operator delete(void *pv);
};

class __cppobj CD3DXException: public CD3duAlloc // sizeof=0x88
{
public:
  CD3DXException(const char* _message, int _error, int _line);
  
  char message[128];
  int error;
  int line;
};

class CHelInfo;
class CDDrawDeviceList;
class CD3duDeviceList;
class CD3duContextList;
class CCapsFile;

class __cppobj CD3duGlobals: public CD3duAlloc // sizeof=0x18
{
public:
  CD3duGlobals();
  ~CD3duGlobals();

  int m_bInitialized;
  CHelInfo *m_pHelInfo;
  CDDrawDeviceList *m_pDDrawDeviceList;
  CD3duDeviceList *m_pD3duDeviceList;
  CD3duContextList *m_pD3duContextList;
  CCapsFile *m_pCapsFile;
};

void __cdecl D3DXDprintf(unsigned int lvl, const char* szFormat);
int __cdecl RegisterLines();
int __cdecl RegisterBlts();
void __cdecl RegisterLine(
    TArray<LINEENTRY>** array,
    unsigned int desc,
    void* pfn);
    //void(__cdecl* fn)(void*, unsigned int*, BLT_DATA*));
void __cdecl DPF_ShowAllocStats();
void __cdecl DPF_ListUnfreedMemory();

void __cdecl D3DXAssert(const char* szFile, int nLine, const char* szCondition);
int __cdecl _d3dxCopyRelease();
int __cdecl _d3dxCopyAddRef();

unsigned int* __cdecl GetBiLinearSrcSpan(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData);

extern int g_bInit;
extern _ALLOC_STUB* g_pHead[];
extern unsigned int g_dwNumAllocations;
extern unsigned int g_dwNumFrees;
extern unsigned int g_dwTotalUnfreedMemory;