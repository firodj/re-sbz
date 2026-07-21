#pragma once

#include "d3dxcore.h"

#ifndef __cppobj
#define __cppobj
#endif

#define SKIP_ME 1


template <typename T>
class __cppobj TArray // sizeof=0xC
{
public:
  T* Data;
  int Size;
  int Count;

  int Add(T* Element);
};


struct PIXELCACHE_BLOCK // sizeof=0x50
{
  PIXELCACHE_BLOCK* link;
  unsigned int DXTn;
  int xleft;
  int ytop;
  unsigned int ARGBData[16];
};

struct SURFACE_DATA // sizeof=0x20
{
  int IsDestSurface;
  _D3DX_SURFACEFORMAT Format;
  unsigned int DXTn;
  unsigned int BitsPerPixel;
  int Pitch;
  void* Bits;
  tagPALETTEENTRY* ColorTable;
  PIXELCACHE_BLOCK* PixelCacheBlocks;
};

struct BLT_DATA // sizeof=0xBC
{
  int Stretch;
  int ColorConvert;
  _D3DX_FILTERTYPE FilterType;
  unsigned int FillValue;
  unsigned int ARGBFactors;
  SURFACE_DATA SrcData;
  SURFACE_DATA DstData;
  tagRECT SrcRect;
  tagRECT DstRect;
  int xRef;
  int yRef;
  void* pDest;
  unsigned int x;
  unsigned int y;
  int uRef;
  int vRef;
  int dudx;
  int dvdy;
  int dudy;
  int dvdx;
  int uSpan;
  int vSpan;
  int uSample;
  int vSample;
  void* pSrcRow;
  unsigned int OutCode;
  void(__cdecl* GetnPixels)(void*, unsigned int*, BLT_DATA*);
};

struct BLTENTRY
{
  _D3DX_SURFACEFORMAT SrcFormat;
  _D3DX_SURFACEFORMAT DstFormat;
  _D3DX_FILTERTYPE FilterType;

  union // $77459C7E03FD8447CD04AA93782101B2 // sizeof=0x4
  {
    int(__cdecl* FnFill)(BLT_DATA*);
    int(__cdecl* FnCopy)(BLT_DATA*);
    int(__cdecl* FnBlend)(BLT_DATA*);
  };
};

struct LINEENTRY // sizeof=0x8
{
  unsigned int Desc;
  union // $8CE5F66487672378BB1F76D3E5487019 // sizeof=0x4
  {
    void(__cdecl* FnGetnPixels)(void*, unsigned int*, BLT_DATA*);
    unsigned int* (__cdecl* FnGetSrcSpan)(int, unsigned int*, BLT_DATA*);
    void(__cdecl* FnGetDstSpan)(int, unsigned int*, BLT_DATA*);
    void(__cdecl* FnPutDstSpan)(int, unsigned int*, BLT_DATA*);
    void(__cdecl* FnCopySpan)(int, BLT_DATA*);
  };
};

struct TABLEENTRY
{
  TArray<LINEENTRY>* GetnPixels;
  TArray<LINEENTRY>* GetSrcSpan;
  TArray<LINEENTRY>* PutDstSpan;
  TArray<BLTENTRY>* Copy;
};

struct _ALLOC_STUB // sizeof=0x10
{
  void* pvMem;
  unsigned int dwSize;
  unsigned int dwAllocationNumber;
  _ALLOC_STUB* pNext;
};

class __cppobj CD3duAlloc // sizeof=0x0
{
public:
  CD3duAlloc();

  static void * operator new(size_t size);
  static void operator delete(void *pv);
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
void __cdecl DPF_ShowAllocStats();
void __cdecl DPF_ListUnfreedMemory();

void __cdecl D3DXAssert(const char* szFile, int nLine, const char* szCondition);
int __cdecl _d3dxCopyRelease();
int __cdecl _d3dxCopyAddRef();

extern int g_bInit;
extern _ALLOC_STUB* g_pHead[];
extern unsigned int g_dwNumAllocations;
extern unsigned int g_dwNumFrees;
extern unsigned int g_dwTotalUnfreedMemory;

extern TArray<LINEENTRY>* ArrayCopySpan;
extern TArray<LINEENTRY>* ArrayFilterSpan;