#include "d3dx.hpp"

#include <cstdlib>
#include <cstdio>



CD3duGlobals *g_pGlobals;


int InitCount;
void* Buffer;
int BufferWidth;

TABLEENTRY FnTable[23];



int __cdecl _d3dxCopyAddRef()
{
  TArray<LINEENTRY> *v0; // eax
  TArray<LINEENTRY> *v1; // eax
  // CD3DXException pExceptionObject; // [esp+4h] [ebp-110h] BYREF
  // CD3DXException v4; // [esp+8Ch] [ebp-88h] BYREF

  if ( !InitCount )
  {
    memset(FnTable, 0, sizeof(FnTable));
    v0 = (TArray<LINEENTRY> *)operator new(0xCu);
    if ( v0 )
    {
      v0->Size = 0;
      v0->Count = 0;
      v0->Data = 0;
    }
    else
    {
      v0 = 0;
    }
    ArrayCopySpan = v0;
    v1 = (TArray<LINEENTRY> *)operator new(0xCu);
    if ( v1 )
    {
      v1->Size = 0;
      v1->Count = 0;
      v1->Data = 0;
    }
    else
    {
      v1 = 0;
    }
    ArrayFilterSpan = v1;
    if ( !ArrayCopySpan || !v1 )
    {
      // CD3duAlloc::CD3duAlloc(&v4);
      // v4.error = 0xC8770BB8;
      // strcpy(v4.message, "_d3dxInitConvert - Memory allocation failed.");
      // v4.line = 39;
      // qmemcpy(&pExceptionObject, &v4, sizeof(pExceptionObject));
      // _CxxThrowException(&pExceptionObject, &_TI2_AVCD3DXException__);
      throw new CD3DXException("_d3dxInitConvert - Memory allocation failed.", 0xC8770BB8, 39);
    }
    RegisterLines();
    RegisterBlts();
  }
  ++InitCount;
  return 0;
}

HRESULT WINAPI D3DXInitialize()
{
  CD3duGlobals *v0; // ecx
  CD3duGlobals *v1; // eax
  int v3; // [esp-Ch] [ebp-1B8h] BYREF
  // CD3DXException pExceptionObject; // [esp+88h] [ebp-124h] BYREF
  // CD3DXException v5; // [esp+110h] [ebp-9Ch] BYREF
  void *pv; // [esp+88h] [ebp-14h]
  int *v7; // [esp+8Ch] [ebp-10h]
  BYTE v8; // [esp+98h] [ebp-4h]

  v8 = 0;
  v7 = &v3;
  if ( !g_pGlobals )
  {
    v0 = new CD3duGlobals(); // size=0x18u
    pv = v0;
    v8 = 1;
    if ( v0 )
      v1 = v0;
    else
      v1 = 0;
    v8 = 0;
    g_pGlobals = v1;
    if ( !v1 )
    {
      // CD3duAlloc::CD3duAlloc(&v5);
      // v5.error = 0xC8770BC6;
      // strcpy(v5.message, "D3DXInitialize - Could not start up D3DX.");
      // v5.line = 20;
      // qmemcpy(&pExceptionObject, &v5, sizeof(pExceptionObject));
      // _CxxThrowException(&pExceptionObject, &_TI2_AVCD3DXException__);
      throw CD3DXException("D3DXInitialize - Could not start up D3DX.", 0xC8770BC6, 20);
    }
    _d3dxCopyAddRef();
  }
  return 0;
}

HRESULT WINAPI D3DXUninitialize()
{
  CD3duGlobals *v0; // esi
  int v2; // [esp-Ch] [ebp-1B4h] BYREF
  // CD3DXException pExceptionObject; // [esp+88h] [ebp-120h] BYREF
  // CD3DXException v4; // [esp+110h] [ebp-98h] BYREF
  // void* pv; // [esp+88h] [ebp-14h]
  int *v5; // [esp+8Ch] [ebp-10h]
  int v6; // [esp+98h] [ebp-4h]

  v6 = 0;
  v5 = &v2;
  if ( !g_pGlobals )
  {
    // CD3duAlloc::CD3duAlloc(&v4);
    // v4.error = 0xC8770BC7;
    // strcpy(v4.message, "D3DXUninitialize - Need to call D3DXInitialize first.");
    // v4.line = 39;
    // qmemcpy(&pExceptionObject, &v4, sizeof(pExceptionObject));
    // _CxxThrowException(&pExceptionObject, &_TI2_AVCD3DXException__);
    throw CD3DXException("D3DXUninitialize - Need to call D3DXInitialize first.", 0xC8770BC7, 39);
  }
  v0 = g_pGlobals;
  // CD3duGlobals::~CD3duGlobals(g_pGlobals);
  // CD3duAlloc::operator delete(v0);
  delete g_pGlobals;
  g_pGlobals = 0;
  D3DXDprintf(0, "D3DXUninitialize - Printing memory report:");
  DPF_ShowAllocStats();
  DPF_ListUnfreedMemory();
  _d3dxCopyRelease();
  return 0;
}

int __cdecl RegisterBlts()
{
  return 0;
}



void __cdecl DPF_ShowAllocStats()
{
  char buff[256]; // [esp+0h] [ebp-100h] BYREF

  sprintf_s(buff, sizeof(buff), "DPF_ShowAllocStats(): %d calls to CD3duAlloc::new.", g_dwNumAllocations);
  D3DXDprintf(0, buff);
  sprintf_s(buff, sizeof(buff), "DPF_ShowAllocStats(): %d calls to CD3duAlloc::delete (on valid memory).", g_dwNumFrees);
  D3DXDprintf(0, buff);
  sprintf_s(buff, sizeof(buff), "DPF_ShowAllocStats(): %d bytes of CD3duAlloc memory not freed by D3DX.", g_dwTotalUnfreedMemory);
  D3DXDprintf(0, buff);
}

void __cdecl DPF_ListUnfreedMemory()
{
  _ALLOC_STUB **v0; // edi
  _ALLOC_STUB *i; // esi
  char buff[256]; // [esp+8h] [ebp-100h] BYREF

  v0 = g_pHead;
  do
  {
    for ( i = *v0; i; i = i->pNext )
    {
      sprintf_s(
        buff,
        sizeof(buff),
        "DPF_ListUnfreedMemory(): Alloc #%d not freed! Address: %.8x, Size: %d bytes.",
        i->dwAllocationNumber,
        (unsigned int)i->pvMem,
        i->dwSize);
      D3DXDprintf(0, buff);
    }
    ++v0;
  }
  while ( v0 < (_ALLOC_STUB **)&g_bInit );
}


int dword_519598;
int dword_519708[39];
extern TArray<LINEENTRY>* ArrayCopySpan;
extern TArray<LINEENTRY>* ArrayFilterSpan;

int __cdecl _d3dxCopyRelease()
{
  bool v0; // zf
  TArray<LINEENTRY> *v1; // esi
  TArray<LINEENTRY> *v2; // esi
  int *v3; // edi
  void **v4; // esi
  void **v5; // esi
  void **v6; // esi
  void **v7; // esi

  v0 = InitCount == 1;
  if ( InitCount < 1 )
  {
    D3DXAssert("D:\\DirectX.CHK\\koolaid\\direct3d\\dxu\\core\\convert.cpp", 52, "InitCount >= 1");
    v0 = InitCount == 1;
  }
  if ( v0 )
  {
    if ( ArrayCopySpan )
    {
      v1 = ArrayCopySpan;
      if ( ArrayCopySpan->Data )
        free(ArrayCopySpan->Data);
      delete v1;
    }
    if ( ArrayFilterSpan )
    {
      v2 = ArrayFilterSpan;
      if ( ArrayFilterSpan->Data )
        free(ArrayFilterSpan->Data);
      delete v2;
    }
    v3 = &dword_519598;
    do
    {
      v4 = (void **)*(v3 - 2);
      if ( v4 )
      {
        if ( *v4 )
          free(*v4);
        delete v4;
      }
      v5 = (void **)*(v3 - 1);
      if ( v5 )
      {
        if ( *v5 )
          free(*v5);
        delete v5;
      }
      v6 = (void **)*v3;
      if ( *v3 )
      {
        if ( *v6 )
          free(*v6);
        delete v6;
      }
      v7 = (void **)v3[1];
      if ( v7 )
      {
        if ( *v7 )
          free(*v7);
        delete v7;
      }
      v3 += 4;
    }
    while ( (int)v3 < (int)dword_519708 );
    memset(FnTable, 0, sizeof(FnTable));
    free(Buffer);
    Buffer = 0;
    BufferWidth = 0;
  }
  --InitCount;
  return 0;
}

