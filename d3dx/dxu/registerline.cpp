#include "d3dx.hpp"
#include "..\ida_defs.hpp"

TArray<LINEENTRY>* ArrayCopySpan;
TArray<LINEENTRY>* ArrayFilterSpan;


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

// type: FnGetSrcSpan
unsigned int* __cdecl GetBiLinearSrcSpan(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  unsigned int BitsPerPixel; // edx
  int vSample; // eax
  int v6; // ecx
  char* v7; // esi
  unsigned int v8; // edx
  int v9; // eax
  int uSample; // edi
  unsigned __int8* v11; // eax
  unsigned int v12; // eax
  unsigned int v13; // edi
  unsigned int v14; // edx
  unsigned int v15; // edi
  unsigned __int8* p[5]; // [esp+Ch] [ebp-68h] BYREF
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
  unsigned int* dstWidtha; // [esp+7Ch] [ebp+8h]
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
  v7 = (char*)bltData->SrcData.Bits + (vSample >> 16) * pitch;
  voutcode = 0;
  v8 = BitsPerPixel >> 3;
  pixelsize = v8;
  vfrac = (unsigned __int16)vSample;
  if (vSample >> 16 < rect.top)
  {
    voutcode = 3;
    if (v6 + 1 < rect.top)
      voutcode = 15;
    vfrac = 0xFFFF;
  }
  if (v6 + 1 >= rect.bottom)
  {
    voutcode |= 0xCu;
    if (v6 >= rect.bottom)
      voutcode = 15;
    vfrac = 0;
  }
  if (voutcode != 15)
  {
    if (dstWidth <= 0)
      return argbsrc;
    v22 = dstWidth;
    for (dstWidtha = argbsrc; ; ++dstWidtha)
    {
      v9 = bltData->uSample >> 16;
      uSample = (unsigned __int16)bltData->uSample;
      outcode = voutcode;
      if (v9 < rect.left)
      {
        outcode = voutcode | 5;
        if (v9 + 1 < rect.left)
          goto LABEL_18;
        uSample = 0xFFFF;
      }
      if (v9 + 1 < rect.right)
        goto LABEL_20;
      outcode |= 0xAu;
      if (v9 < rect.right)
      {
        uSample = 0;
      LABEL_20:
        v11 = (unsigned __int8*)&v7[v8 * v9];
        p[0] = v11;
        bltData->OutCode = outcode;
        p[1] = &v11[v8];
        p[3] = &v11[v8 + pitch];
        p[4] = 0;
        p[2] = &v11[pitch];
        bltData->GetnPixels(p, val, bltData);
        if (bltData->OutCode == 15)
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
            if ((bltData->OutCode & bit) == 0)
            {
              v15 = *(unsigned int*)((char*)val + outcode);
              v21 = *(unsigned int*)((char*)weight + outcode);
              alf_grn += v21 * ((v15 >> 8) & 0xFF00FF);
              v14 = v21 * (v15 & 0xFF00FF) + red_blu;
              red_blu = v14;
            }
            outcode += 4;
            bit *= 2;
          } while ((int)outcode < 16);
          *dstWidtha = alf_grn & 0xFF00FF00 | (v14 >> 8) & 0xFF00FF;
        }
        v8 = pixelsize;
        goto LABEL_28;
      }
    LABEL_18:
      *dstWidtha = 0;
    LABEL_28:
      bltData->uSample += bltData->dudx;
      if (!--v22)
        return argbsrc;
    }
  }
  memset(argbsrc, 0, 4 * dstWidth);
  return argbsrc;
}

// type: FnCopySpan
void __cdecl BltSpan_4BPP(int dstWidth, BLT_DATA* bltData)
{
  unsigned int x; // edx
  int uSample; // ecx
  unsigned __int8* v4; // esi
  char v5; // al
  char v6; // cl
  char v7; // bl
  char v8; // al
  bool v9; // zf
  int dudx; // [esp+0h] [ebp-10h]
  unsigned __int8* pSrc; // [esp+4h] [ebp-Ch]
  unsigned __int8* pDest; // [esp+8h] [ebp-8h]
  int u; // [esp+Ch] [ebp-4h]

  x = bltData->x;
  pDest = (unsigned __int8*)bltData->pDest;
  pSrc = (unsigned __int8*)bltData->pSrcRow;
  uSample = bltData->uSample;
  u = uSample;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    do
    {
      v4 = &pDest[x >> 1];
      v5 = pSrc[uSample >> 17] >> ((~uSample >> 14) & 4);
      v6 = 4 * ((x & 1) == 0);
      v7 = 15 << v6;
      v8 = (v5 & 0xF) << v6;
      uSample = dudx + u;
      u += dudx;
      ++x;
      v9 = dstWidth-- == 1;
      *v4 = *v4 & ~v7 | v8;
    } while (!v9);
  }
}

// type: FnCopySpan
void __cdecl BltSpan_8BPP(int dstWidth, BLT_DATA *bltData)
{
  int v2; // esi
  BYTE *pDest; // ecx
  BYTE *pSrcRow; // edx
  int uSample; // edi
  int i; // eax
  int v7; // ebx

  v2 = 0;
  pDest = (BYTE*)bltData->pDest;
  pSrcRow = (BYTE*)bltData->pSrcRow;
  uSample = bltData->uSample;
  for ( i = bltData->dudx; v2 < dstWidth; ++v2 )
  {
    v7 = uSample;
    uSample += i;
    pDest[v2] = pSrcRow[v7 >> 16];
  }
}

// type: FnCopySpan
void __cdecl BltSpan_16BPP(int dstWidth, BLT_DATA* bltData)
{
  int v2; // edi
  WORD* pDest; // edx
  WORD* pSrcRow; // ecx
  int uSample; // esi
  int dudx; // eax
  int v7; // ebx

  v2 = dstWidth;
  pDest = (WORD*)bltData->pDest;
  pSrcRow = (WORD*)bltData->pSrcRow;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    do
    {
      v7 = uSample;
      uSample += dudx;
      *pDest++ = pSrcRow[v7 >> 16];
      --v2;
    } while (v2);
  }
}

// type: FnCopySpan
void __cdecl BltSpan_24BPP(int dstWidth, BLT_DATA* bltData)
{
  char* pDest; // edx
  char* pSrcRow; // esi
  int uSample; // edi
  BYTE* v5; // ecx
  int v6; // eax
  char* v7; // eax
  int dudx; // [esp+Ch] [ebp-4h]

  pDest = (char*)bltData->pDest;
  pSrcRow = (char*)bltData->pSrcRow;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  if (3 * dstWidth > 0)
  {
    v5 = (BYTE*)pDest + 1;
    do
    {
      v6 = uSample;
      uSample += dudx;
      v7 = &pSrcRow[2 * (v6 >> 16) + (v6 >> 16)];
      *(v5 - 1) = *v7;
      *v5 = v7[1];
      v5[1] = v7[2];
      v5 += 3;
    } while ((int)&v5[-1 - (DWORD)pDest] < 3 * dstWidth);
  }
}

// type: FnCopySpan
void __cdecl BltSpan_32BPP(int dstWidth, BLT_DATA* bltData)
{
  int v2; // edi
  DWORD* pDest; // edx
  DWORD* pSrcRow; // ecx
  int uSample; // esi
  int dudx; // eax
  int v7; // ebx

  v2 = dstWidth;
  pDest = (DWORD*)bltData->pDest;
  pSrcRow = (DWORD*)bltData->pSrcRow;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    do
    {
      v7 = uSample;
      uSample += dudx;
      *pDest++ = pSrcRow[v7 >> 16];
      --v2;
    } while (v2);
  }
}

// type: FnGetSrcSpan
unsigned int* __cdecl GetSrcSpan_R8G8B8(BLT_DATA* dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // esi
  char* pSrcRow; // ecx
  unsigned int* v5; // edx
  int v6; // eax
  char* v7; // eax
  int dudx; // [esp+4h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+18h] [ebp+10h]

  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (char*)bltData->pSrcRow;
  if ((int)dstWidth > 0)
  {
    v5 = argbsrc;
    bltDataa = dstWidth;
    do
    {
      v6 = uSample;
      uSample += dudx;
      v7 = &pSrcRow[2 * (v6 >> 16) + (v6 >> 16)];
      *v5++ = (unsigned __int8)*v7 | (((unsigned __int8)v7[1] | (((unsigned __int8)v7[2] | 0xFFFFFF00) << 8)) << 8);
      bltDataa = (BLT_DATA*)((char*)bltDataa - 1);
    } while (bltDataa);
  }
  return argbsrc;
}

#if 0
void __cdecl PutDstSpan_R8G8B8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  char* pDest; // ecx
  int v6; // eax

  v3 = dstWidth;
  pDest = (char*)bltData->pDest;
  if (dstWidth > 0)
  {
    do
    {
      v6 = *argbdst++;
      *(WORD*)pDest = v6;
      pDest[2] = BYTE2(v6);
      pDest += 3;
      --v3;
    } while (v3);
  }
}

void __cdecl GetnPixels_R8G8B8(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // ecx
  unsigned int i; // edx
  unsigned int bit; // [esp+0h] [ebp-4h]

  v3 = p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(*(unsigned __int8*)*v3
        | ((*((unsigned __int8*)*v3 + 1)
          | ((*((unsigned __int8*)*v3 + 2) | 0xFFFFFF00) << 8)) << 8));
    bit *= 2;
  }
  bltData->OutCode = i;
}

unsigned int* __cdecl GetSrcSpan_A8R8G8B8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int dudx; // esi
  int uSample; // eax
  DWORD* pSrcRow; // ecx
  unsigned int* result; // eax
  unsigned int* v7; // esi
  int v8; // edi
  unsigned int* v9; // edx
  int v10; // ebx

  dudx = bltData->dudx;
  uSample = bltData->uSample;
  pSrcRow = bltData->pSrcRow;
  if (dudx == 0x10000)
  {
    result = &pSrcRow[uSample >> 16];
    if (((unsigned __int8)result & 7) != 0)
    {
      v7 = result;
      result = argbsrc;
      qmemcpy(argbsrc, v7, 4 * dstWidth);
    }
  }
  else
  {
    v8 = dstWidth;
    if (dstWidth > 0)
    {
      v9 = argbsrc;
      do
      {
        v10 = uSample;
        uSample += dudx;
        *v9++ = pSrcRow[v10 >> 16];
        --v8;
      } while (v8);
    }
    return argbsrc;
  }
  return result;
}

void __cdecl PutDstSpan_A8R8G8B8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  qmemcpy(bltData->pDest, argbdst, 4 * dstWidth);
}

void __cdecl GetnPixels_A8R8G8B8(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // eax
  unsigned int OutCode; // edx
  int i; // ebx
  void* v6; // ecx

  v3 = p;
  OutCode = bltData->OutCode;
  for (i = 1; *v3; i *= 2)
  {
    if ((i & OutCode) == 0)
    {
      v6 = *(void**)*v3;
      if (v6)
        *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = v6;
      else
        OutCode |= i;
    }
    ++v3;
  }
  bltData->OutCode = OutCode;
}

unsigned int* __cdecl GetSrcSpan_X8R8G8B8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // edx
  int dudx; // esi
  DWORD* pSrcRow; // eax
  unsigned int v6; // edi
  unsigned int* v7; // ebx
  int v8; // ecx
  unsigned int prevpixel; // [esp+18h] [ebp+10h]

  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = bltData->pSrcRow;
  v6 = -1;
  prevpixel = -1;
  if (dstWidth > 0)
  {
    v7 = argbsrc;
    do
    {
      v8 = pSrcRow[uSample >> 16];
      if (v8 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        v6 = v8 | 0xFF000000;
      }
      *v7++ = v6;
      uSample += dudx;
      --dstWidth;
    } while (dstWidth);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_X8R8G8B8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  qmemcpy(bltData->pDest, argbdst, 4 * dstWidth);
}

void __cdecl GetnPixels_X8R8G8B8(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // ecx
  unsigned int OutCode; // edx
  int i; // ebx

  v3 = p;
  OutCode = bltData->OutCode;
  for (i = 1; *v3; i *= 2)
  {
    if ((i & OutCode) == 0)
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(*(DWORD*)*v3 | 0xFF000000);
    ++v3;
  }
  bltData->OutCode = OutCode;
}

BLT_DATA* __cdecl GetSrcSpan_R5G6B5(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  unsigned int v3; // edx
  int uSample; // esi
  unsigned __int16* pSrcRow; // eax
  int v6; // ecx
  BLT_DATA* v7; // ecx
  bool v8; // zf
  int dudx; // [esp+4h] [ebp-8h]
  unsigned int prevpixel; // [esp+8h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+1Ch] [ebp+10h]

  prevpixel = -1;
  v3 = 0;
  uSample = bltData->uSample;
  pSrcRow = (unsigned __int16*)bltData->pSrcRow;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v6 = pSrcRow[uSample >> 16];
      if (v6 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        v3 = (32 * (v6 & 0x7E0 | 0xFFF80000))
          | ((((8 * ((32 * (v6 & 0xF800)) | v6 & 0x1Fu)) >> 4) & 0xE000E | v6 & 0x600) >> 1)
          | (8 * ((32 * (v6 & 0xF800)) | v6 & 0x1F));
      }
      v7 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      uSample += dudx;
      v8 = dstWidth-- == 1;
      v7->Stretch = v3;
    } while (!v8);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_R5G6B5(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = bltData->pDest;
  LOWORD(v5) = 0;
  prevval = 0;
  if (dstWidth > 0)
  {
    v7 = pDest;
    do
    {
      if (*argbdst != prevval)
      {
        prevval = *argbdst;
        v5 = (*argbdst & 0xF8 | ((*argbdst & 0xFC00 | (*argbdst >> 3) & 0x1F0000) >> 2)) >> 3;
      }
      *v7 = v5;
      ++argbdst;
      ++v7;
      --v3;
    } while (v3);
  }
}

void __cdecl GetnPixels_R5G6B5(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int v4; // eax
  __int16 v5; // ax
  unsigned int v6; // ecx
  unsigned int v7; // edi
  unsigned int outcode; // [esp+0h] [ebp-8h]
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = p;
  bit = 1;
  v4 = bltData->OutCode;
  for (outcode = v4; *v3; ++v3)
  {
    if ((v4 & bit) == 0)
    {
      v5 = *(WORD*)*v3;
      v6 = 8 * ((32 * (v5 & 0xF800)) | v5 & 0x1F);
      v7 = (32 * (v5 & 0x7E0 | 0xFFF80000)) | (((v6 >> 4) & 0xE000E | v5 & 0x600) >> 1);
      v4 = outcode;
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(v6 | v7);
    }
    bit *= 2;
  }
  bltData->OutCode = v4;
}

BLT_DATA* __cdecl GetSrcSpan_R5G5B5(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  int uSample; // esi
  int dudx; // edi
  unsigned __int16* pSrcRow; // eax
  unsigned int v6; // ecx
  int v7; // edx
  unsigned int v8; // ecx
  BLT_DATA* v9; // edx
  bool v10; // zf
  unsigned int prevpixel; // [esp+8h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+1Ch] [ebp+10h]

  prevpixel = -1;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (unsigned __int16*)bltData->pSrcRow;
  v6 = 0;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v7 = pSrcRow[uSample >> 16];
      if (v7 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        BYTE1(v7) |= 0x80u;
        v8 = 8 * (v7 & 0x1F | (8 * (v7 & 0x3E0 | (8 * (v7 & 0x7C00)))));
        v6 = (v8 >> 5) & 0x70707 | 0xFF000000 | v8;
      }
      v9 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      uSample += dudx;
      v10 = dstWidth-- == 1;
      v9->Stretch = v6;
    } while (!v10);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_R5G5B5(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = bltData->pDest;
  LOWORD(v5) = 0;
  prevval = 0;
  if (dstWidth > 0)
  {
    v7 = pDest;
    do
    {
      if (*argbdst != prevval)
      {
        prevval = *argbdst;
        v5 = (*argbdst & 0xF8 | ((*argbdst & 0xF800 | ((*argbdst & 0xF80000 | 0x1000000) >> 3)) >> 3)) >> 3;
      }
      *v7 = v5;
      ++argbdst;
      ++v7;
      --v3;
    } while (v3);
  }
}

void __cdecl GetnPixels_R5G5B5(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int i; // edi
  __int16 v5; // ax
  unsigned int v6; // ecx
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      v5 = *(WORD*)*v3;
      HIBYTE(v5) |= 0x80u;
      v6 = 8 * (v5 & 0x1F | (8 * (v5 & 0x3E0 | (8 * (v5 & 0x7C00)))));
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(v6 | (v6 >> 5) & 0x70707 | 0xFF000000);
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

BLT_DATA* __cdecl GetSrcSpan_PALETTE4(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  unsigned __int8* pSrcRow; // edi
  int uSample; // esi
  unsigned int v6; // edx
  int v7; // ecx
  unsigned __int16 v8; // bx
  BLT_DATA* v9; // ecx
  bool v10; // zf
  int dudx; // [esp+8h] [ebp-8h]
  unsigned int val; // [esp+Ch] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+20h] [ebp+10h]

  pSrcRow = (unsigned __int8*)bltData->pSrcRow;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v6 = pSrcRow[uSample >> 17];
      v7 = uSample;
      uSample += dudx;
      val = (unsigned int)bltData->SrcData.ColorTable[(v6 >> ((~v7 >> 14) & 4)) & 0xF];
      LOBYTE(v8) = BYTE2(val);
      HIBYTE(v8) = BYTE1(val);
      v9 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      v10 = dstWidth-- == 1;
      v9->Stretch = v8 | ((val | 0xFFFFFF00) << 16);
    } while (!v10);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_PALETTE4(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  char v3; // bl
  unsigned __int8* v4; // esi
  int v5; // edx
  char v6; // al
  unsigned __int8* p; // [esp+4h] [ebp-10h]
  unsigned int prevval; // [esp+8h] [ebp-Ch]
  int i; // [esp+Ch] [ebp-8h]
  unsigned __int8 pixel; // [esp+13h] [ebp-1h]

  p = (unsigned __int8*)bltData->pDest;
  prevval = 0;
  pixel = 0;
  v3 = 4 * (((unsigned __int8)p & 1) == 0);
  for (i = 0; i < dstWidth; *v4 = *v4 & v5 | v6)
  {
    if (*argbdst != prevval)
    {
      prevval = *argbdst;
      pixel = FindClosest_PALETTE4(*argbdst, bltData->DstData.ColorTable);
    }
    v4 = &p[i >> 1];
    v5 = 240 >> v3;
    v6 = pixel << v3;
    ++argbdst;
    v3 ^= 4u;
    ++i;
  }
}

void __cdecl GetnPixels_PALETTE4(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int v5; // eax
  unsigned __int16 v6; // bx
  unsigned int outcode; // [esp+4h] [ebp-8h]
  unsigned int bit; // [esp+8h] [ebp-4h]
  char i; // [esp+1Ch] [ebp+10h]

  v3 = p;
  i = 0;
  bit = 1;
  v5 = bltData->OutCode;
  for (outcode = v5; *v3; bit *= 2)
  {
    if ((v5 & bit) == 0)
    {
      LOBYTE(v6) = BYTE2(*(DWORD*)&bltData->SrcData.ColorTable[(*((unsigned __int8*)*v3
        + ((bltData->uSample - 0x7FFF) >> 17)
        + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14) & 4))
        & 0xF]);
      HIBYTE(v6) = BYTE1(*(DWORD*)&bltData->SrcData.ColorTable[(*((unsigned __int8*)*v3
        + ((bltData->uSample - 0x7FFF) >> 17)
        + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14) & 4))
        & 0xF]);
      v5 = outcode;
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(v6
        | ((*(DWORD*)&bltData->SrcData.ColorTable[(*((unsigned __int8*)*v3 + ((bltData->uSample - 0x7FFF) >> 17) + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14) & 4)) & 0xF]
          | 0xFFFFFF00) << 16));
    }
    ++i;
    ++v3;
  }
  bltData->OutCode = v5;
}

unsigned int* __cdecl GetSrcSpan_PALETTE8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // ecx
  unsigned __int8* pSrcRow; // esi
  unsigned int* v6; // edi
  unsigned __int16 v7; // bx
  bool v8; // zf
  int dudx; // [esp+4h] [ebp-8h]
  unsigned int val; // [esp+8h] [ebp-4h]
  int u; // [esp+1Ch] [ebp+10h]

  uSample = bltData->uSample;
  pSrcRow = (unsigned __int8*)bltData->pSrcRow;
  dudx = bltData->dudx;
  u = uSample;
  if (dstWidth > 0)
  {
    v6 = argbsrc;
    do
    {
      val = (unsigned int)bltData->SrcData.ColorTable[pSrcRow[uSample >> 16]];
      LOBYTE(v7) = BYTE2(val);
      HIBYTE(v7) = BYTE1(val);
      uSample = dudx + u;
      *v6++ = v7 | ((val | 0xFFFFFF00) << 16);
      v8 = dstWidth-- == 1;
      u += dudx;
    } while (!v8);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_PALETTE8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  BYTE* pDest; // esi
  unsigned __int8 Closest_PALETTE8; // al
  int v6; // edi
  unsigned int prevval; // [esp+Ch] [ebp-4h]

  pDest = bltData->pDest;
  Closest_PALETTE8 = 0;
  v6 = 0;
  for (prevval = 0; v6 < dstWidth; ++v6)
  {
    if (*argbdst != prevval)
    {
      prevval = *argbdst;
      Closest_PALETTE8 = FindClosest_PALETTE8(*argbdst, bltData->DstData.ColorTable);
    }
    ++argbdst;
    pDest[v6] = Closest_PALETTE8;
  }
}

void __cdecl GetnPixels_PALETTE8(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // ecx
  unsigned int v5; // eax
  int v6; // edi
  unsigned int outcode; // [esp+4h] [ebp-4h]
  unsigned int val; // [esp+10h] [ebp+8h]
  unsigned int bit; // [esp+18h] [ebp+10h]

  v3 = p;
  bit = 1;
  v5 = bltData->OutCode;
  outcode = v5;
  if (*p)
  {
    v6 = (char*)argbsrc - (char*)p;
    do
    {
      if ((v5 & bit) == 0)
      {
        val = (unsigned int)bltData->SrcData.ColorTable[*(unsigned __int8*)*v3];
        v5 = outcode;
        *(void**)((char*)v3 + v6) = (void*)(val & 0xFF00 | ((val | 0xFFFFFF00) << 16) | BYTE2(val));
      }
      bit *= 2;
      ++v3;
    } while (*v3);
  }
  bltData->OutCode = v5;
}

BLT_DATA* __cdecl GetSrcSpan_A1R5G5B5(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  unsigned int v3; // ecx
  int uSample; // esi
  int dudx; // edi
  __int16* pSrcRow; // eax
  int v7; // edx
  int v8; // ecx
  BLT_DATA* v9; // edx
  bool v10; // zf
  int prevpixel; // [esp+8h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+1Ch] [ebp+10h]

  v3 = 0;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (__int16*)bltData->pSrcRow;
  prevpixel = 0;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v7 = pSrcRow[uSample >> 16];
      if (v7 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        v8 = v7 & 0x1F | (8 * (v7 & 0x3E0 | (8 * (v7 & 0x7C00))));
        v3 = v7 & 0xFF000000 | ((unsigned int)(8 * v8) >> 5) & 0x70707 | (8 * v8);
      }
      v9 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      uSample += dudx;
      v10 = dstWidth-- == 1;
      v9->Stretch = v3;
    } while (!v10);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_A1R5G5B5(BLT_DATA* dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  unsigned int v4; // ecx
  WORD* pDest; // esi
  BLT_DATA* bltDataa; // [esp+18h] [ebp+10h]

  v3 = 0;
  LOWORD(v4) = 0;
  if ((int)dstWidth > 0)
  {
    pDest = bltData->pDest;
    bltDataa = dstWidth;
    do
    {
      if (*argbdst != v3)
      {
        LOWORD(v4) = 0;
        v3 = *argbdst;
        if (*argbdst)
          v4 = (*argbdst & 0xF8 | ((*argbdst & 0xF800 | (((*argbdst >> 7) & 0x1000000 | *argbdst & 0xF80000) >> 3)) >> 3)) >> 3;
      }
      *pDest = v4;
      ++argbdst;
      ++pDest;
      bltDataa = (BLT_DATA*)((char*)bltDataa - 1);
    } while (bltDataa);
  }
}

void __cdecl GetnPixels_A1R5G5B5(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int i; // esi
  int v5; // ecx
  int v6; // eax
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      v5 = *(__int16*)*v3;
      if (*(WORD*)*v3)
      {
        v6 = 8 * (v5 & 0x3E0 | (8 * (v5 & 0x7C00)));
        *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)((8 * (v5 & 0x1F | v6))
          | v5 & 0xFF000000
          | ((8 * (v5 & 0x1F | (unsigned int)v6)) >> 5)
          & 0x70707);
      }
      else
      {
        i |= bit;
      }
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

BLT_DATA* __cdecl GetSrcSpan_X4R4G4B4(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  int uSample; // esi
  int dudx; // edi
  unsigned __int16* pSrcRow; // eax
  unsigned int v6; // ecx
  int v7; // edx
  int v8; // ecx
  BLT_DATA* v9; // edx
  bool v10; // zf
  unsigned int prevpixel; // [esp+8h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+1Ch] [ebp+10h]

  prevpixel = -1;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (unsigned __int16*)bltData->pSrcRow;
  v6 = 0;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v7 = pSrcRow[uSample >> 16];
      if (v7 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        BYTE1(v7) |= 0xF0u;
        v8 = v7 & 0xF | (16 * ((16 * (v7 & 0xF00)) | v7 & 0xF0));
        v6 = (16 * (v8 | 0xFFF00000)) | v8;
      }
      v9 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      uSample += dudx;
      v10 = dstWidth-- == 1;
      v9->Stretch = v6;
    } while (!v10);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_X4R4G4B4(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = bltData->pDest;
  LOWORD(v5) = 0;
  prevval = 0;
  if (dstWidth > 0)
  {
    v7 = pDest;
    do
    {
      if (*argbdst != prevval)
      {
        prevval = *argbdst;
        v5 = (*argbdst & 0xF0 | ((*argbdst & 0xF000 | (*argbdst >> 4) & 0xF0000) >> 4)) >> 4;
      }
      *v7 = v5;
      ++argbdst;
      ++v7;
      --v3;
    } while (v3);
  }
}

void __cdecl GetnPixels_X4R4G4B4(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int i; // edi
  __int16 v5; // ax
  int v6; // ecx
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      v5 = *(WORD*)*v3;
      HIBYTE(v5) |= 0xF0u;
      v6 = v5 & 0xF | (16 * ((16 * (v5 & 0xF00)) | *(WORD*)*v3 & 0xF0));
      *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(v6 | (16 * (v6 | 0xFFF00000)));
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

BLT_DATA* __cdecl GetSrcSpan_A4R4G4B4(int dstWidth, BLT_DATA* argbsrc, BLT_DATA* bltData)
{
  int v3; // ecx
  int uSample; // esi
  int dudx; // edi
  unsigned __int16* pSrcRow; // eax
  int v7; // edx
  int v8; // ecx
  BLT_DATA* v9; // edx
  bool v10; // zf
  unsigned int prevpixel; // [esp+8h] [ebp-4h]
  BLT_DATA* bltDataa; // [esp+1Ch] [ebp+10h]

  v3 = 0;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (unsigned __int16*)bltData->pSrcRow;
  prevpixel = 0;
  if (dstWidth > 0)
  {
    bltDataa = argbsrc;
    do
    {
      v7 = pSrcRow[uSample >> 16];
      if (v7 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        v8 = v7 & 0xF | (16 * (v7 & 0xF0 | (16 * ((16 * (v7 & 0xF000)) | v7 & 0xF00))));
        v3 = (16 * v8) | v8;
      }
      v9 = bltDataa;
      bltDataa = (BLT_DATA*)((char*)bltDataa + 4);
      uSample += dudx;
      v10 = dstWidth-- == 1;
      v9->Stretch = v3;
    } while (!v10);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_A4R4G4B4(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = bltData->pDest;
  LOWORD(v5) = 0;
  prevval = 0;
  if (dstWidth > 0)
  {
    v7 = pDest;
    do
    {
      if (*argbdst != prevval)
      {
        prevval = *argbdst;
        v5 = (*argbdst & 0xF0 | ((*argbdst & 0xF000 | (((*argbdst >> 4) & 0xF000000 | *argbdst & 0xF00000) >> 4)) >> 4)) >> 4;
      }
      *v7 = v5;
      ++argbdst;
      ++v7;
      --v3;
    } while (v3);
  }
}



void __cdecl GetnPixels_A4R4G4B4(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  void** v3; // edx
  unsigned int i; // esi
  int v5; // eax
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      if (*(WORD*)*v3)
      {
        v5 = *(WORD*)*v3 & 0xF
          | (16 * (*(WORD*)*v3 & 0xF0 | (16 * ((16 * (*(WORD*)*v3 & 0xF000)) | *(WORD*)*v3 & 0xF00))));
        *(void**)((char*)v3 + (char*)argbsrc - (char*)p) = (void*)(v5 | (16 * v5));
      }
      else
      {
        i |= bit;
      }
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

unsigned int* __cdecl GetSrcSpan_L8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // edx
  int dudx; // edi
  unsigned __int8* pSrcRow; // eax
  unsigned int v7; // ecx
  unsigned int* v8; // ebx
  int v9; // esi
  unsigned int prevpixel; // [esp+18h] [ebp+10h]

  prevpixel = -1;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (unsigned __int8*)bltData->pSrcRow;
  v7 = 0;
  if (dstWidth > 0)
  {
    v8 = argbsrc;
    do
    {
      v9 = pSrcRow[uSample >> 16];
      if (v9 != prevpixel)
      {
        prevpixel = pSrcRow[uSample >> 16];
        v7 = v9 | ((v9 | ((v9 | 0xFFFFFF00) << 8)) << 8);
      }
      *v8++ = v7;
      uSample += dudx;
      --dstWidth;
    } while (dstWidth);
  }
  return argbsrc;
}

#endif

void __cdecl RegisterLine(
  TArray<LINEENTRY>** array,
  unsigned int desc,
  void* pfn)
{
  void(__cdecl * fn)(void*, unsigned int*, BLT_DATA*) = reinterpret_cast<void(__cdecl*)(void*, unsigned int*, BLT_DATA*)>(pfn);

  TArray<LINEENTRY>* v3; // eax
  TArray<LINEENTRY>* v4; // ecx
  //CD3DXException pExceptionObject; // [esp+Ch] [ebp-118h] BYREF
  //CD3DXException v6; // [esp+94h] [ebp-90h] BYREF
  LINEENTRY l; // [esp+11Ch] [ebp-8h] BYREF

  if (!*array)
  {
    v3 = (TArray<LINEENTRY> *)operator new(0xCu);
    if (v3)
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
    if (!v3)
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



int __cdecl RegisterLines()
{
    int v1; // [esp-Ch] [ebp-A4h] BYREF
    int* v2; // [esp+88h] [ebp-10h]
    int v3; // [esp+94h] [ebp-4h]

    v2 = &v1;
    v3 = 0;
    RegisterLine(&ArrayFilterSpan, 2u, GetBiLinearSrcSpan);
    RegisterLine(&ArrayCopySpan, 4u, BltSpan_4BPP);
    RegisterLine(&ArrayCopySpan, 8u, BltSpan_8BPP);

#if SKIP_ME != 1
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
