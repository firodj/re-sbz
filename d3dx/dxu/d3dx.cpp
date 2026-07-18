#include "d3dx.hpp"

#include <cstdlib>
#include <cstdio>

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
  void *pDest;
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
  void *pSrcRow;
  unsigned int OutCode;
  void (__cdecl *GetnPixels)(void *, unsigned int *, BLT_DATA *);
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
    void (__cdecl *FnGetnPixels)(void *, unsigned int *, BLT_DATA *);
    unsigned int *(__cdecl *FnGetSrcSpan)(int, unsigned int *, BLT_DATA *);
    void (__cdecl *FnGetDstSpan)(int, unsigned int *, BLT_DATA *);
    void (__cdecl *FnPutDstSpan)(int, unsigned int *, BLT_DATA *);
    void (__cdecl *FnCopySpan)(int, BLT_DATA *);
  };
};

struct TABLEENTRY
{
  TArray<LINEENTRY> *GetnPixels;
  TArray<LINEENTRY> *GetSrcSpan;
  TArray<LINEENTRY> *PutDstSpan;
  TArray<BLTENTRY> *Copy;
};

CD3duGlobals *g_pGlobals;


int InitCount;
void* Buffer;
int BufferWidth;

TABLEENTRY FnTable[23];
TArray<LINEENTRY> *ArrayCopySpan;
TArray<LINEENTRY> *ArrayFilterSpan;


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
  void *pv; // [esp+88h] [ebp-14h]
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

int __cdecl RegisterLines()
{
  int v1; // [esp-Ch] [ebp-A4h] BYREF
  int *v2; // [esp+88h] [ebp-10h]
  int v3; // [esp+94h] [ebp-4h]

  v2 = &v1;
  v3 = 0;
  RegisterLine(&ArrayFilterSpan, 2u, GetBiLinearSrcSpan);

#if SKIP_ME != 1
  RegisterLine(&ArrayCopySpan, 4u, BltSpan_4BPP);
  RegisterLine(&ArrayCopySpan, 8u, BltSpan_8BPP);
  RegisterLine(&ArrayCopySpan, 0x10u, BltSpan_16BPP);
  RegisterLine(&ArrayCopySpan, 0x18u, BltSpan_24BPP);
  RegisterLine(&ArrayCopySpan, 0x20u, BltSpan_32BPP);
  RegisterLine(&array, 0, GetSrcSpan_R8G8B8);
  RegisterLine(&dword_5195A8, 0, PutDstSpan_R8G8B8);
  RegisterLine(&dword_5195A0, 0, GetnPixels_R8G8B8);
  RegisterLine(&dword_5195B4, 0, GetSrcSpan_A8R8G8B8);
  RegisterLine(&dword_5195B8, 0, PutDstSpan_A8R8G8B8);
  RegisterLine(&dword_5195B0, 0, GetnPixels_A8R8G8B8);
  RegisterLine(&dword_5195C4, 0, GetSrcSpan_X8R8G8B8);
  RegisterLine(&dword_5195C8, 0, PutDstSpan_X8R8G8B8);
  RegisterLine(&dword_5195C0, 0, GetnPixels_X8R8G8B8);
  RegisterLine(&dword_5195D4, 0, GetSrcSpan_R5G6B5);
  RegisterLine(&dword_5195D8, 0, PutDstSpan_R5G6B5);
  RegisterLine(&dword_5195D0, 0, GetnPixels_R5G6B5);
  RegisterLine(&dword_5195E4, 0, GetSrcSpan_R5G5B5);
  RegisterLine(&dword_5195E8, 0, PutDstSpan_R5G5B5);
  RegisterLine(&dword_5195E0, 0, GetnPixels_R5G5B5);
  RegisterLine(&dword_5195F4, 0, GetSrcSpan_PALETTE4);
  RegisterLine(&dword_5195F8, 0, PutDstSpan_PALETTE4);
  RegisterLine(&dword_5195F0, 0, GetnPixels_PALETTE4);
  RegisterLine(&dword_519604, 0, GetSrcSpan_PALETTE8);
  RegisterLine(&dword_519608, 0, PutDstSpan_PALETTE8);
  RegisterLine(&dword_519600, 0, GetnPixels_PALETTE8);
  RegisterLine(&dword_519614, 0, GetSrcSpan_A1R5G5B5);
  RegisterLine(&dword_519618, 0, PutDstSpan_A1R5G5B5);
  RegisterLine(&dword_519610, 0, GetnPixels_A1R5G5B5);
  RegisterLine(&dword_519624, 0, GetSrcSpan_X4R4G4B4);
  RegisterLine(&dword_519628, 0, PutDstSpan_X4R4G4B4);
  RegisterLine(&dword_519620, 0, GetnPixels_X4R4G4B4);
  RegisterLine(&dword_519634, 0, GetSrcSpan_A4R4G4B4);
  RegisterLine(&dword_519638, 0, PutDstSpan_A4R4G4B4);
  RegisterLine(&dword_519630, 0, GetnPixels_A4R4G4B4);
  RegisterLine(&dword_519644, 0, GetSrcSpan_L8);
  RegisterLine(&dword_519648, 0, PutDstSpan_L8);
  RegisterLine(&dword_519640, 0, GetnPixels_L8);
  RegisterLine(&dword_5196F4, 0, GetSrcSpan_A8);
  RegisterLine(&dword_5196F8, 0, PutDstSpan_A8);
  RegisterLine(&dword_5196F0, 0, GetnPixels_A8);
  RegisterLine(&dword_519654, 0, GetSrcSpan_A8L8);
  RegisterLine(&dword_519658, 0, PutDstSpan_A8L8);
  RegisterLine(&dword_519650, 0, GetnPixels_A8L8);
  RegisterLine(&dword_5196B4, 0, GetSrcSpan_DXTn);
  RegisterLine(&dword_5196B8, 0, PutDstSpan_DXTn);
  RegisterLine(&dword_5196B0, 0, GetnPixels_DXTn);
  RegisterLine(&dword_5196C4, 0, GetSrcSpan_DXTn);
  RegisterLine(&dword_5196C8, 0, PutDstSpan_DXTn);
  RegisterLine(&dword_5196C0, 0, GetnPixels_DXTn);
  RegisterLine(&dword_5196D4, 0, GetSrcSpan_DXTn);
  RegisterLine(&dword_5196D8, 0, PutDstSpan_DXTn);
  RegisterLine(&dword_5196D0, 0, GetnPixels_DXTn);
  RegisterLine(&dword_5196E4, 0, GetSrcSpan_R3G3B2);
  RegisterLine(&dword_5196E8, 0, PutDstSpan_R3G3B2);
  RegisterLine(&dword_5196E0, 0, GetnPixels_R3G3B2);
  RegisterLine(&dword_519684, 0, GetSrcSpan_U8V8L8);
  RegisterLine(&dword_519688, 0, PutDstSpan_U8V8L8);
  RegisterLine(&dword_519680, 0, GetnPixels_U8V8L8);
  RegisterLine(&dword_519674, 0, GetSrcSpan_U5V5L6);
  RegisterLine(&dword_519678, 0, PutDstSpan_U5V5L6);
  RegisterLine(&dword_519670, 0, GetnPixels_U5V5L6);
  RegisterLine(&dword_519664, 0, GetSrcSpan_U8V8);
  RegisterLine(&dword_519668, 0, PutDstSpan_U8V8);
  RegisterLine(&dword_519660, 0, GetnPixels_U8V8);
  RegisterLine(&dword_5196A4, 0, GetSrcSpan_YUY2);
  RegisterLine(&dword_5196A8, 0, PutDstSpan_YUY2);
  RegisterLine(&dword_5196A0, 0, GetnPixels_YUY2);
  RegisterLine(&dword_519694, 0, GetSrcSpan_UYVY);
  RegisterLine(&dword_519698, 0, PutDstSpan_UYVY);
  RegisterLine(&dword_519690, 0, GetnPixels_UYVY);
#endif
  return 0;
}

int __cdecl RegisterBlts()
{
  return 0;
}

void __cdecl RegisterLine(
        TArray<LINEENTRY> **array,
        unsigned int desc,
        void *pfn)
{
  void(__cdecl * fn)(void*, unsigned int*, BLT_DATA*) = reinterpret_cast<void(__cdecl*)(void*, unsigned int*, BLT_DATA*)>(pfn);

  TArray<LINEENTRY> *v3; // eax
  TArray<LINEENTRY> *v4; // ecx
  //CD3DXException pExceptionObject; // [esp+Ch] [ebp-118h] BYREF
  //CD3DXException v6; // [esp+94h] [ebp-90h] BYREF
  LINEENTRY l; // [esp+11Ch] [ebp-8h] BYREF

  if ( !*array )
  {
    v3 = (TArray<LINEENTRY> *)operator new(0xCu);
    if ( v3 )
    {
      v3->Size = 0;
      v3->Count = 0;
      v3->Data = 0;
    }
    else
    {
      v3 = 0;
    }
    *array = v3;
    if ( !v3 )
    {
      // CD3duAlloc::CD3duAlloc(&v6);
      // v6.error = 0xC8770BB8;
      // strcpy(v6.message, "RegisterLine - out of memory");
      // v6.line = 21;
      // qmemcpy(&pExceptionObject, &v6, sizeof(pExceptionObject));
      // _CxxThrowException(&pExceptionObject, &_TI2_AVCD3DXException__);
      throw CD3DXException("RegisterLine - out of memory", 0xC8770BB8, 21);
    }
  }
  v4 = *array;
  l.Desc = desc;
  l.FnGetnPixels = fn;
  v4->Add(&l);
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

void __cdecl D3DXAssert(const char *szFile, int nLine, const char *szCondition)
{
  char buffer[512]; // [esp+0h] [ebp-200h] BYREF

  wsprintfA(buffer, (LPCSTR)"ASSERTION FAILED! File %s Line %d: %s", szFile, nLine, szCondition);
  D3DXDprintf(0, buffer);
  DebugBreak();
}

int dword_519598;
int dword_519708[39];

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

unsigned int *__cdecl GetBiLinearSrcSpan(int dstWidth, unsigned int *argbsrc, BLT_DATA *bltData)
{
  unsigned int BitsPerPixel; // edx
  int vSample; // eax
  int v6; // ecx
  char *v7; // esi
  unsigned int v8; // edx
  int v9; // eax
  int uSample; // edi
  unsigned __int8 *v11; // eax
  unsigned int v12; // eax
  unsigned int v13; // edi
  unsigned int v14; // edx
  unsigned int v15; // edi
  unsigned __int8 *p[5]; // [esp+Ch] [ebp-68h] BYREF
  unsigned int val[4]; // [esp+20h] [ebp-54h] BYREF
  unsigned int weight[4]; // [esp+30h] [ebp-44h]
  tagRECT rect; // [esp+40h] [ebp-34h]
  int v21; // [esp+50h] [ebp-24h]
  int v22; // [esp+54h] [ebp-20h]
  unsigned int pixelsize; // [esp+58h] [ebp-1Ch]
  unsigned int red_blu; // [esp+5Ch] [ebp-18h]
  unsigned int pitch; // [esp+60h] [ebp-14h]
  unsigned int alf_grn; // [esp+64h] [ebp-10h]
  unsigned int bit; // [esp+68h] [ebp-Ch]
  unsigned int vfrac; // [esp+6Ch] [ebp-8h]
  unsigned int outcode; // [esp+70h] [ebp-4h]
  unsigned int *dstWidtha; // [esp+7Ch] [ebp+8h]
  unsigned int voutcode; // [esp+84h] [ebp+10h]

  pitch = bltData->SrcData.Pitch;
  rect.left = bltData->SrcRect.left;
  rect.top = bltData->SrcRect.top;
  bltData->uSample -= 0x7FFF;
  BitsPerPixel = bltData->SrcData.BitsPerPixel;
  bltData->vSample -= 0x7FFF;
  vSample = bltData->vSample;
  rect.right = bltData->SrcRect.right;
  rect.bottom = bltData->SrcRect.bottom;
  v6 = vSample >> 16;
  v7 = (char *)bltData->SrcData.Bits + (vSample >> 16) * pitch;
  voutcode = 0;
  v8 = BitsPerPixel >> 3;
  pixelsize = v8;
  vfrac = (unsigned __int16)vSample;
  if ( vSample >> 16 < rect.top )
  {
    voutcode = 3;
    if ( v6 + 1 < rect.top )
      voutcode = 15;
    vfrac = 0xFFFF;
  }
  if ( v6 + 1 >= rect.bottom )
  {
    voutcode |= 0xCu;
    if ( v6 >= rect.bottom )
      voutcode = 15;
    vfrac = 0;
  }
  if ( voutcode != 15 )
  {
    if ( dstWidth <= 0 )
      return argbsrc;
    v22 = dstWidth;
    for ( dstWidtha = argbsrc; ; ++dstWidtha )
    {
      v9 = bltData->uSample >> 16;
      uSample = (unsigned __int16)bltData->uSample;
      outcode = voutcode;
      if ( v9 < rect.left )
      {
        outcode = voutcode | 5;
        if ( v9 + 1 < rect.left )
          goto LABEL_18;
        uSample = 0xFFFF;
      }
      if ( v9 + 1 < rect.right )
        goto LABEL_20;
      outcode |= 0xAu;
      if ( v9 < rect.right )
      {
        uSample = 0;
LABEL_20:
        v11 = (unsigned __int8 *)&v7[v8 * v9];
        p[0] = v11;
        bltData->OutCode = outcode;
        p[1] = &v11[v8];
        p[3] = &v11[v8 + pitch];
        p[4] = 0;
        p[2] = &v11[pitch];
        bltData->GetnPixels(p, val, bltData);
        if ( bltData->OutCode == 15 )
        {
          *dstWidtha = 0;
        }
        else
        {
          v12 = (vfrac * uSample) >> 16;
          v13 = uSample - v12;
          weight[1] = v13 >> 8;
          bit = 1;
          weight[0] = (0x10000 - vfrac - v13) >> 8;
          v14 = 0;
          weight[2] = (vfrac - v12) >> 8;
          weight[3] = v12 >> 8;
          red_blu = 0;
          alf_grn = 0;
          outcode = 0;
          do
          {
            if ( (bltData->OutCode & bit) == 0 )
            {
              v15 = *(unsigned int *)((char *)val + outcode);
              v21 = *(unsigned int *)((char *)weight + outcode);
              alf_grn += v21 * ((v15 >> 8) & 0xFF00FF);
              v14 = v21 * (v15 & 0xFF00FF) + red_blu;
              red_blu = v14;
            }
            outcode += 4;
            bit *= 2;
          }
          while ( (int)outcode < 16 );
          *dstWidtha = alf_grn & 0xFF00FF00 | (v14 >> 8) & 0xFF00FF;
        }
        v8 = pixelsize;
        goto LABEL_28;
      }
LABEL_18:
      *dstWidtha = 0;
LABEL_28:
      bltData->uSample += bltData->dudx;
      if ( !--v22 )
        return argbsrc;
    }
  }
  memset(argbsrc, 0, 4 * dstWidth);
  return argbsrc;
}

template <typename T>
int TArray<T>::Add(T *Element)
{
  int Size; // ecx
  int *p_Size; // eax

  Size = this->Size;
  p_Size = &this->Size;
  if ( this->Count == Size )
  {
    if ( Size )
      *p_Size = 2 * Size;
    else
      *p_Size = 2;
    this->Data = (T *)realloc(this->Data, 8 * *p_Size);
  }
  memmove((unsigned __int8 *)&this->Data[this->Count], (unsigned __int8 *)Element, 8u);
  return this->Count++;
}