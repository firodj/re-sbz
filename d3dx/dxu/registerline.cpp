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

// type: FnPutDstSpan
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

// type: FnGetnPixels
void __cdecl GetnPixels_R8G8B8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  DWORD* v3; // ecx
  unsigned int i; // edx
  unsigned int bit; // [esp+0h] [ebp-4h]

  v3 = (DWORD*)p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
      *(DWORD*)((char*)v3 + ((BYTE*)argbsrc - (_BYTE*)p)) = *(unsigned __int8*)*v3
      | ((*(unsigned __int8*)(*v3 + 1)
        | ((*(unsigned __int8*)(*v3 + 2) | 0xFFFFFF00) << 8)) << 8);
    bit *= 2;
  }
  bltData->OutCode = i;
}

unsigned int* __cdecl GetSrcSpan_A8R8G8B8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int dudx; // esi
  int uSample; // eax
  _DWORD* pSrcRow; // ecx
  unsigned int* result; // eax
  unsigned int* v7; // esi
  int v8; // edi
  unsigned int* v9; // edx
  int v10; // ebx

  dudx = bltData->dudx;
  uSample = bltData->uSample;
  pSrcRow = (_DWORD*)bltData->pSrcRow;
  if (dudx == 0x10000)
  {
    result = &pSrcRow[uSample >> 16];
    if ((((unsigned int)result) & 7) != 0)
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

// type: FnGetnPixels
void __cdecl GetnPixels_A8R8G8B8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // eax
  unsigned int OutCode; // edx
  int i; // ebx
  DWORD v6; // ecx

  v3 = (_DWORD*)p;
  OutCode = bltData->OutCode;
  for (i = 1; *v3; i *= 2)
  {
    if ((i & OutCode) == 0)
    {
      v6 = *(_DWORD*)*v3;
      if (v6)
        *(_DWORD*)((char*)v3 + ((BYTE*)argbsrc - (_BYTE*)p)) = v6;
      else
        OutCode |= i;
    }
    ++v3;
  }
  bltData->OutCode = OutCode;
}

// type: FnGetSrcSpan
unsigned int* __cdecl GetSrcSpan_X8R8G8B8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // edx
  int dudx; // esi
  _DWORD* pSrcRow; // eax
  unsigned int v6; // edi
  unsigned int* v7; // ebx
  int v8; // ecx
  unsigned int prevpixel; // [esp+18h] [ebp+10h]

  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (_DWORD*)bltData->pSrcRow;
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_X8R8G8B8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  qmemcpy(bltData->pDest, argbdst, 4 * dstWidth);
}

// type: FnGetnPixels
void __cdecl GetnPixels_X8R8G8B8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // ecx
  unsigned int OutCode; // edx
  int i; // ebx

  v3 = (_DWORD*)p;
  OutCode = bltData->OutCode;
  for (i = 1; *v3; i *= 2)
  {
    if ((i & OutCode) == 0)
      *(_DWORD*)((char*)v3 + ((BYTE*)argbsrc - (_BYTE*)p)) = *(_DWORD*)*v3 | 0xFF000000;
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_R5G6B5(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  _WORD* pDest; // ecx
  _DWORD v5; // ax
  _WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = (_WORD *)bltData->pDest;
  v5 = 0;
  prevval = 0;
  if (dstWidth > 0)
  {
    v7 = pDest;
    do
    {
      if (*argbdst != prevval)
      {
        prevval = *argbdst;
        *(_DWORD*)&v5 = (*argbdst & 0xF8 | ((*argbdst & 0xFC00 | (*argbdst >> 3) & 0x1F0000) >> 2)) >> 3;
      }
      *v7 = v5;
      ++argbdst;
      ++v7;
      --v3;
    } while (v3);
  }
}

// type: FnGetnPixels
void __cdecl GetnPixels_R5G6B5(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // edx
  unsigned int v4; // eax
  __int16 v5; // ax
  unsigned int v6; // ecx
  unsigned int v7; // edi
  unsigned int outcode; // [esp+0h] [ebp-8h]
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = (_DWORD*)p;
  bit = 1;
  v4 = bltData->OutCode;
  for (outcode = v4; *v3; ++v3)
  {
    if ((v4 & bit) == 0)
    {
      v5 = *(_WORD*)*v3;
      v6 = 8 * ((32 * (v5 & 0xF800)) | v5 & 0x1F);
      v7 = (32 * (v5 & 0x7E0 | 0xFFF80000)) | (((v6 >> 4) & 0xE000E | v5 & 0x600) >> 1);
      v4 = outcode;
      *(_DWORD*)((char*)v3 + ((BYTE*)argbsrc - (_BYTE*)p)) = v6 | v7;
    }
    bit *= 2;
  }
  bltData->OutCode = v4;
}

// type: FnGetSrcSpan
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_R5G5B5(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = (WORD *)bltData->pDest;
  v5 = 0;
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

// type: FnGetnPixels
void __cdecl GetnPixels_R5G5B5(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // edx
  unsigned int i; // edi
  _WORD v5; // ax
  unsigned int v6; // ecx
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = (_DWORD*)p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      v5 = *(_WORD*)*v3;
      
      // HIBYTE(v5) |= 0x80u;
      v5 = MAKEWORD(LOBYTE(v5), HIBYTE(v5) | 0x80u);

      v6 = 8 * (v5 & 0x1F | (8 * (v5 & 0x3E0 | (8 * (v5 & 0x7C00)))));
      *(_DWORD*)((char*)v3 + ((BYTE*)argbsrc - (_BYTE*)p)) = v6 | (v6 >> 5) & 0x70707 | 0xFF000000;
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

// type: FnGetSrcSpan
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
  tagPALETTEENTRY val; // [esp+Ch] [ebp-4h]
  BLT_DATA** bltDataa; // [esp+20h] [ebp+10h]

  pSrcRow = (unsigned __int8*)bltData->pSrcRow;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  if (dstWidth > 0)
  {
    bltDataa = (BLT_DATA**)argbsrc;
    do
    {
      v6 = pSrcRow[uSample >> 17];
      v7 = uSample;
      uSample += dudx;
      val = bltData->SrcData.ColorTable[(v6 >> ((~v7 >> 14) & 4)) & 0xF];
      // LOBYTE(v8) = val.peBlue;
      // HIBYTE(v8) = val.peGreen;
      v8 = MAKEWORD(val.peBlue, val.peGreen);
      v9 = (BLT_DATA*)bltDataa++;
      v10 = dstWidth-- == 1;
      v9->Stretch = v8 | ((val.peRed | 0xFFFFFF00) << 16);
    } while (!v10);
  }
  return argbsrc;
}

char __cdecl FindClosest_PALETTE4(unsigned int val, tagPALETTEENTRY* colorTable)
{
  unsigned __int8* p_peBlue; // ecx
  __int64 v3; // rax
  int v4; // esi
  __int64 v5; // rax
  int v6; // esi
  char bestVal; // [esp+Ch] [ebp-Ch]
  int bestDiff; // [esp+10h] [ebp-8h]
  int i; // [esp+14h] [ebp-4h]

  bestVal = 0;
  i = 0;
  bestDiff = 1020;
  p_peBlue = &colorTable->peBlue;
  do
  {
    v3 = *(p_peBlue - 2) - BYTE2(val);
    v4 = (HIDWORD(v3) ^ v3) - HIDWORD(v3) + abs32(*p_peBlue - (unsigned __int8)val);
    v5 = *(p_peBlue - 1) - BYTE1(val);
    v6 = (HIDWORD(v5) ^ v5) - HIDWORD(v5) + v4;
    if (v6 < bestDiff)
    {
      bestVal = i;
      if (!v6)
        return bestVal;
      bestDiff = v6;
    }
    p_peBlue += 4;
    ++i;
  } while (i < 15);
  return bestVal;
}

// type: FnPutDstSpan
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

// type: FnGetnPixels
void __cdecl GetnPixels_PALETTE4(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // edx
  unsigned int v5; // eax
  unsigned __int16 v6; // bx
  unsigned int outcode; // [esp+4h] [ebp-8h]
  unsigned int bit; // [esp+8h] [ebp-4h]
  char i; // [esp+1Ch] [ebp+10h]

  v3 = (_DWORD*)p;
  i = 0;
  bit = 1;
  v5 = bltData->OutCode;
  for (outcode = v5; *v3; bit *= 2)
  {
    if ((v5 & bit) == 0)
    {
      BYTE LOBYTE_v6 = BYTE2(*(_DWORD*)&bltData->SrcData.ColorTable[(*(unsigned __int8*)(*v3
        + ((bltData->uSample - 0x7FFF) >> 17)
        + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14)
          & 4))
        & 0xF]);
      BYTE HIBYTE_v6 = BYTE1(*(_DWORD*)&bltData->SrcData.ColorTable[(*(unsigned __int8*)(*v3
        + ((bltData->uSample - 0x7FFF) >> 17)
        + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14)
          & 4))
        & 0xF]);
      v6 = MAKEWORD(LOBYTE_v6, HIBYTE_v6);
      v5 = outcode;
      *(_DWORD*)((char*)v3 + ((_BYTE*)argbsrc - (_BYTE*)p)) = v6
        | ((*(_DWORD*)&bltData->SrcData.ColorTable[(*(unsigned __int8*)(*v3 + ((bltData->uSample - 0x7FFF) >> 17) + (i & 1)) >> ((~(bltData->uSample - 0x7FFF) >> 14) & 4)) & 0xF]
          | 0xFFFFFF00) << 16);
    }
    ++i;
    ++v3;
  }
  bltData->OutCode = v5;
}

// type: FnGetSrcSpan
unsigned int* __cdecl GetSrcSpan_PALETTE8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // ecx
  unsigned __int8* pSrcRow; // esi
  unsigned int* v6; // edi
  unsigned __int16 v7; // bx
  bool v8; // zf
  int dudx; // [esp+4h] [ebp-8h]
  tagPALETTEENTRY val; // [esp+8h] [ebp-4h]
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
      val = bltData->SrcData.ColorTable[pSrcRow[uSample >> 16]];
      // LOBYTE(v7) = val.peBlue;
      // HIBYTE(v7) = val.peGreen;
      v7 = MAKEWORD(val.peBlue, val.peGreen);
      uSample = dudx + u;
      *v6++ = v7 | ((val.peRed | 0xFFFFFF00) << 16);
      v8 = dstWidth-- == 1;
      u += dudx;
    } while (!v8);
  }
  return argbsrc;
}

char __cdecl FindClosest_PALETTE8(unsigned int val, tagPALETTEENTRY* colorTable)
{
  unsigned __int8* p_peBlue; // ecx
  __int64 v3; // rax
  int v4; // esi
  __int64 v5; // rax
  int v6; // esi
  char bestVal; // [esp+Ch] [ebp-Ch]
  int bestDiff; // [esp+10h] [ebp-8h]
  int i; // [esp+14h] [ebp-4h]

  bestVal = 0;
  i = 0;
  bestDiff = 1020;
  p_peBlue = &colorTable->peBlue;
  do
  {
    v3 = *(p_peBlue - 2) - BYTE2(val);
    v4 = (HIDWORD(v3) ^ v3) - HIDWORD(v3) + abs32(*p_peBlue - (unsigned __int8)val);
    v5 = *(p_peBlue - 1) - BYTE1(val);
    v6 = (HIDWORD(v5) ^ v5) - HIDWORD(v5) + v4;
    if (v6 < bestDiff)
    {
      bestVal = i;
      if (!v6)
        return bestVal;
      bestDiff = v6;
    }
    p_peBlue += 4;
    ++i;
  } while (i < 255);
  return bestVal;
}

// type: FnPutDstSpan
void __cdecl PutDstSpan_PALETTE8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  BYTE* pDest; // esi
  unsigned __int8 Closest_PALETTE8; // al
  int v6; // edi
  unsigned int prevval; // [esp+Ch] [ebp-4h]

  pDest = (BYTE*)bltData->pDest;
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

// type: FnGetnPixels
void __cdecl GetnPixels_PALETTE8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // ecx
  unsigned int v5; // eax
  int v6; // edi
  unsigned int outcode; // [esp+4h] [ebp-4h]
  tagPALETTEENTRY val; // [esp+10h] [ebp+8h]
  unsigned int bit; // [esp+18h] [ebp+10h]

  v3 = (_DWORD*)p;
  bit = 1;
  v5 = bltData->OutCode;
  outcode = v5;
  if (*(_DWORD*)p)
  {
    v6 = (_BYTE*)argbsrc - (_BYTE*)p;
    do
    {
      if ((v5 & bit) == 0)
      {
        val = bltData->SrcData.ColorTable[*(unsigned __int8*)*v3];
        v5 = outcode;
        *(_DWORD*)((char*)v3 + v6) = *(_WORD*)&val.peRed & 0xFF00 | ((val.peRed | 0xFFFFFF00) << 16) | val.peBlue;
      }
      bit *= 2;
      ++v3;
    } while (*v3);
  }
  bltData->OutCode = v5;
}


// type: FnGetSrcSpan
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_A1R5G5B5(BLT_DATA* dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  unsigned int v4; // ecx
  WORD* pDest; // esi
  BLT_DATA* bltDataa; // [esp+18h] [ebp+10h]

  v3 = 0;
  v4 = 0;
  if ((int)dstWidth > 0)
  {
    pDest = (WORD*)bltData->pDest;
    bltDataa = dstWidth;
    do
    {
      if (*argbdst != v3)
      {
        v4 = 0;
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

// type: FnGetnPixels
void __cdecl GetnPixels_A1R5G5B5(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* ppPixel; // edx
  unsigned int outCode; // esi
  int pixel; // ecx
  int v6; // eax
  unsigned int bit; // [esp+4h] [ebp-4h]

  ppPixel = (_DWORD *)p;
  bit = 1;
  for (outCode = bltData->OutCode; *ppPixel; ++ppPixel)
  {
    if ((outCode & bit) == 0)
    {
      pixel = *(__int16*)*ppPixel;
      if (*(_WORD*)*ppPixel)
      {
        v6 = 8 * (pixel & 0x3E0 | (8 * (pixel & 0x7C00)));
        *(_DWORD*)((char*)ppPixel + ((char*)argbsrc - (char*)p)) = (8 * (pixel & 0x1F | v6))
          | pixel & 0xFF000000
          | ((8 * (pixel & 0x1F | (unsigned int)v6)) >> 5)
          & 0x70707;
      }
      else
      {
        outCode |= bit;
      }
    }
    bit *= 2;
  }
  bltData->OutCode = outCode;
}

// type: FnGetSrcSpan
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_X4R4G4B4(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = (WORD*)bltData->pDest;
  v5 = 0;
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

// type: FnGetnPixels
void __cdecl GetnPixels_X4R4G4B4(void** p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // edx
  unsigned int i; // edi
  __int16 v5; // ax
  int v6; // ecx
  unsigned int bit; // [esp+4h] [ebp-4h]

  v3 = (_DWORD *)p;
  bit = 1;
  for (i = bltData->OutCode; *v3; ++v3)
  {
    if ((i & bit) == 0)
    {
      v5 = *(_WORD*)*v3;
      // HIBYTE(v5) |= 0xF0u
      v5 = MAKEWORD(LOBYTE(v5), HIBYTE(v5) | 0xF0u);
      v6 = v5 & 0xF | (16 * ((16 * (v5 & 0xF00)) | *(_WORD*)*v3 & 0xF0));
      *(_DWORD*)((char*)v3 + ((char*)argbsrc - (char*)p)) = v6 | (16 * (v6 | 0xFFF00000));
    }
    bit *= 2;
  }
  bltData->OutCode = i;
}

// type: FnGetSrcSpan
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_A4R4G4B4(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // edi
  WORD* pDest; // ecx
  unsigned int v5; // eax
  WORD* v7; // esi
  unsigned int prevval; // [esp+14h] [ebp+10h]

  v3 = dstWidth;
  pDest = (WORD*)bltData->pDest;
  v5 = 0;
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

// type: FnGetnPixels
void __cdecl GetnPixels_A4R4G4B4(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* ppPixel; // edx
  unsigned int outCode; // esi
  int v5; // eax
  unsigned int bit; // [esp+4h] [ebp-4h]

  ppPixel = (_DWORD*)p;
  bit = 1;
  for (outCode = bltData->OutCode; *ppPixel; ++ppPixel)
  {
    if ((outCode & bit) == 0)
    {
      if (*(_WORD*)*ppPixel)
      {
        v5 = *(_WORD*)*ppPixel & 0xF
          | (16
            * (*(_WORD*)*ppPixel & 0xF0 | (16 * ((16 * (*(_WORD*)*ppPixel & 0xF000)) | *(_WORD*)*ppPixel & 0xF00))));
        *(_DWORD*)((char*)ppPixel + ((char*)argbsrc - (char*)p)) = v5 | (16 * v5);
      }
      else
      {
        outCode |= bit;
      }
    }
    bit *= 2;
  }
  bltData->OutCode = outCode;
}

// type: FnGetSrcSpan
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

// type: FnPutDstSpan
void __cdecl PutDstSpan_L8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  unsigned int v3; // ecx
  _BYTE* pDest; // esi
  int v5; // edi
  unsigned int v7; // eax
  unsigned int prevval; // [esp+18h] [ebp+10h]

  v3 = 0;
  pDest = (_BYTE *)bltData->pDest;
  v5 = 0;
  for (prevval = 0; v5 < dstWidth; ++argbdst)
  {
    if (*argbdst != prevval)
    {
      prevval = *argbdst;
      v7 = 76 * (unsigned __int8)BYTE2(*argbdst)
        + 28 * (unsigned __int8)*argbdst
        + 151 * (unsigned __int8)BYTE1(*argbdst);
      v3 = (v7 + (v7 >> 8)) >> 8;
    }
    pDest[v5++] = v3;
  }
}

void __cdecl GetnPixels_L8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* ppPixel; // eax
  unsigned int outCode; // edx
  unsigned int bit; // [esp+14h] [ebp+10h]

  ppPixel = (_DWORD*)p;
  bit = 1;
  for (outCode = bltData->OutCode; *ppPixel; ++ppPixel)
  {
    if ((outCode & bit) == 0)
      *(_DWORD*)((char*)ppPixel + ((char*)argbsrc - (char*)p)) = *(unsigned __int8*)*ppPixel
      | ((*(unsigned __int8*)*ppPixel
        | ((*(unsigned __int8*)*ppPixel | 0xFFFFFF00) << 8)) << 8);
    bit *= 2;
  }
  bltData->OutCode = outCode;
}

unsigned int* __cdecl GetSrcSpan_A8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // edx
  int dudx; // esi
  unsigned __int8* pSrcRow; // eax
  unsigned int v7; // edi
  unsigned int* v8; // ebx
  int v9; // ecx
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
        v7 = v9 << 24;
      }
      *v8++ = v7;
      uSample += dudx;
      --dstWidth;
    } while (dstWidth);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_A8(int dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // ebx
  int v4; // edx
  _BYTE* pDest; // ecx
  unsigned int v6; // eax

  v3 = 0;
  v4 = 0;
  pDest = (_BYTE*)bltData->pDest;
  for (v6 = 0; v4 < dstWidth; ++argbdst)
  {
    if (*argbdst != v3)
    {
      v3 = *argbdst;
      v6 = HIBYTE(*argbdst);
    }
    pDest[v4++] = v6;
  }
}

void __cdecl GetnPixels_A8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* ppPixel; // ecx
  unsigned int OutCode; // edx
  int outCode; // ebx

  ppPixel = (_DWORD*)p;
  OutCode = bltData->OutCode;
  for (outCode = 1; *ppPixel; outCode *= 2)
  {
    if ((outCode & OutCode) == 0)
      *(_DWORD*)((char*)ppPixel + ((char*)argbsrc - (char*)p)) = *(unsigned __int8*)*ppPixel << 24;
    ++ppPixel;
  }
  bltData->OutCode = OutCode;
}

unsigned int* __cdecl GetSrcSpan_A8L8(int dstWidth, unsigned int* argbsrc, BLT_DATA* bltData)
{
  int uSample; // edx
  int dudx; // edi
  unsigned __int16* pSrcRow; // eax
  int v7; // esi
  unsigned int* v8; // ebx
  int v9; // ecx
  unsigned int prevpixel; // [esp+18h] [ebp+10h]

  prevpixel = -1;
  uSample = bltData->uSample;
  dudx = bltData->dudx;
  pSrcRow = (unsigned __int16*)bltData->pSrcRow;
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
        v7 = (unsigned __int8)v9 | (((unsigned __int8)v9 | (v9 << 8)) << 8);
      }
      *v8++ = v7;
      uSample += dudx;
      --dstWidth;
    } while (dstWidth);
  }
  return argbsrc;
}

void __cdecl PutDstSpan_A8L8(BLT_DATA* dstWidth, unsigned int* argbdst, BLT_DATA* bltData)
{
  int v3; // ebx
  unsigned int v4; // ecx
  _WORD* pDest; // edi
  int v7; // edx
  BLT_DATA* bltDataa; // [esp+14h] [ebp+10h]

  v3 = 0;
  v4 = 0;
  if ((int)dstWidth > 0)
  {
    pDest = (_WORD*)bltData->pDest;
    bltDataa = dstWidth;
    do
    {
      if (*argbdst != v3)
      {
        v3 = *argbdst;
        v7 = *argbdst;
        v4 = (28 * (unsigned __int8)v7
          + 76 * BYTE2(v7)
          + 151 * BYTE1(v7)
          + ((28 * (unsigned __int8)v7 + 76 * BYTE2(v7) + 151 * (unsigned int)BYTE1(v7)) >> 8)) >> 8;
      }
      *pDest = v4;
      ++argbdst;
      ++pDest;
      bltDataa = (BLT_DATA*)((char*)bltDataa - 1);
    } while (bltDataa);
  }
}

void __cdecl GetnPixels_A8L8(void* p, unsigned int* argbsrc, BLT_DATA* bltData)
{
  _DWORD* v3; // ecx
  unsigned int outCode; // esi
  unsigned int bit; // [esp+18h] [ebp+10h]

  v3 = (_DWORD*)p;
  bit = 1;
  for (outCode = bltData->OutCode; *v3; ++v3)
  {
    if ((outCode & bit) == 0)
      *(_DWORD*)((char*)v3 + ((char*)argbsrc - (char*)p)) = *(unsigned __int8*)*v3
      | ((*(unsigned __int8*)*v3 | (*(unsigned __int8*)*v3 << 8)) << 8);
    bit *= 2;
  }
  bltData->OutCode = outCode;
}

/////

void __cdecl ColorToRGB(S3TC_COLOR* pcolor, unsigned __int16* prgb)
{
  *prgb = (pcolor->rgba[0] >> 3) | (32 * ((pcolor->rgba[1] >> 2) | (pcolor->rgba[2] >> 3 << 6)));
}

void __cdecl RGBToColor(unsigned __int16* prgb, S3TC_COLOR* pcolor)
{
  unsigned __int16 v2; // ax
  char v3; // dl

  v2 = *prgb;
  v3 = *prgb >> 5;
  S3TC_COLOR out = { 0 };
  out.rgba[0] = ((unsigned __int8)(8 * v2) >> 5) | (8 * v2);
  out.rgba[1] = ((unsigned __int8)(4 * v3) >> 6) | (4 * v3);
  out.rgba[2] = ((unsigned __int8)(8 * (v2 >> 11)) >> 5) | (8 * (v2 >> 11));
  *pcolor = out;
}

void __cdecl AllSame(S3TC_COLOR** pcolor, S3TCBlockRGB* pblock, unsigned __int16 wAlpha)
{
  S3TCBlockRGB* v3; // esi
  S3TC_COLOR** v4; // edi
  unsigned __int16 rgb0; // ax
  bool v6; // zf
  __int16 v7; // cx
  int v8; // eax
  int v9; // edx
  S3TC_COLOR* v10; // [esp-8h] [ebp-10h]

  v3 = pblock;
  v4 = pcolor;
  v10 = (S3TC_COLOR*)pcolor;
  pcolor = (S3TC_COLOR**)*pcolor;
  ColorToRGB(v10, &pblock->rgb0);
  rgb0 = v3->rgb0;
  v3->pixbm = 0;
  v6 = wAlpha == 0xFFFF;
  v3->rgb1 = rgb0;
  if (!v6)
  {
    v7 = 1;
    v8 = 3;
    v9 = 16;
    do
    {
      if ((wAlpha & (unsigned __int16)v7) != 0)
        pcolor = (S3TC_COLOR**)*v4;
      else
        v3->pixbm |= v8;
      ++v4;
      v7 *= 2;
      v8 *= 4;
      --v9;
    } while (v9);
    ColorToRGB((S3TC_COLOR*)&pcolor, &v3->rgb0);
    v3->rgb1 = v3->rgb0;
  }
}

float wtPrimary[3] = { 0.082000002, 0.60939997, 0.30860001 };

void __cdecl ColorToFcolor(S3TC_COLOR* pcolor, FCOLOR* pfcolor)
{
  FCOLOR* v2; // eax
  int v3; // ecx
  int v4; // edx
  FCOLOR* pfcolora; // [esp+10h] [ebp+Ch]

  v2 = pfcolor;
  v3 = 0;
  v4 = (char*)wtPrimary - (char*)pfcolor;
  do
  {
    pfcolora = (FCOLOR*)pcolor->rgba[v3++];
    v2->rgba[0] = (double)(int)pfcolora * *(float*)((char*)v2->rgba + v4) * 0.0039215689;
    v2 = (FCOLOR*)((char*)v2 + 4);
  } while (v3 < 3);
}

void __cdecl Square3x3(float (*m)[3][3], float (*m2)[3][3])
{
  (*m2)[0][0] = (*m)[0][1] * (*m)[0][1] + (*m)[0][2] * (*m)[0][2] + (*m)[0][0] * (*m)[0][0];
  (*m2)[0][1] = ((*m)[0][0] + (*m)[1][1]) * (*m)[0][1] + (*m)[1][2] * (*m)[0][2];
  (*m2)[0][2] = ((*m)[0][0] + (*m)[2][2]) * (*m)[0][2] + (*m)[1][2] * (*m)[0][1];
  (*m2)[1][1] = (*m)[0][1] * (*m)[0][1] + (*m)[1][1] * (*m)[1][1] + (*m)[1][2] * (*m)[1][2];
  (*m2)[1][2] = ((*m)[2][2] + (*m)[1][1]) * (*m)[1][2] + (*m)[0][2] * (*m)[0][1];
  (*m2)[2][2] = (*m)[0][2] * (*m)[0][2] + (*m)[1][2] * (*m)[1][2] + (*m)[2][2] * (*m)[2][2];
}

void __cdecl Quantize(FCOLOR* pfcolor0, FCOLOR* pfcolor1, S3TCBlockRGB* pblock, int cOpaque)
{
  unsigned __int16 rgb0; // ax
  S3TC_COLOR color; // [esp+Ch] [ebp-4h] BYREF

  FcolorToColor(pfcolor0, &color);
  ColorToRGB(&color, &pblock->rgb0);
  FcolorToColor(pfcolor1, &color);
  ColorToRGB(&color, &pblock->rgb1);
  rgb0 = pblock->rgb0;
  if ((cOpaque == 16) != pblock->rgb1 < pblock->rgb0)
  {
    pblock->rgb0 = pblock->rgb1;
    pblock->rgb1 = rgb0;
  }
  RGBToColor(&pblock->rgb0, &color);
  ColorToFcolor(&color, pfcolor0);
  RGBToColor(&pblock->rgb1, &color);
  ColorToFcolor(&color, pfcolor1);
}

void __cdecl ClipExtrema(FCOLOR* plower, FCOLOR* pupper)
{
  FCOLOR* v3; // ecx
  int v4; // ebx
  double v5; // st5
  FCOLOR* v6; // eax
  float* v7; // esi
  double v8; // st5
  FCOLOR* v9; // eax
  int v10; // [esp+Ch] [ebp-8h]
  char* v11; // [esp+10h] [ebp-4h]
  FCOLOR* plowera; // [esp+1Ch] [ebp+8h]

  v3 = plower;
  v4 = (char*)pupper - (char*)plower;
  plowera = (FCOLOR*)((char*)wtPrimary - (char*)plower);
  v11 = (char*)((char*)pupper - (char*)v3);
  v10 = 3;
  do
  {
    if (v3->rgba[0] < 0.0 != *(float*)((char*)v3->rgba + v4) < 0.0)
    {
      v5 = -(v3->rgba[0] / (*(float*)((char*)v3->rgba + v4) - v3->rgba[0]));
      if (v3->rgba[0] >= 0.0)
      {
        v6 = pupper;
        v5 = v5 - 1.0;
      }
      else
      {
        v6 = plower;
      }
      v6->rgba[2] = (pupper->rgba[2] - plower->rgba[2]) * v5 + v6->rgba[2];
      v6->rgba[1] = (pupper->rgba[1] - plower->rgba[1]) * v5 + v6->rgba[1];
      v6->rgba[0] = (pupper->rgba[0] - plower->rgba[0]) * v5 + v6->rgba[0];
    }
    v7 = (float*)((int)v3->rgba + (_DWORD)plowera);
    if (v3->rgba[0] > (double)*(float*)((char*)v3->rgba + (_DWORD)plowera) != *(float*)((char*)v3->rgba + v4) > (double)*(float*)((char*)v3->rgba + (_DWORD)plowera))
    {
      v4 = (int)v11;
      v8 = (*v7 - v3->rgba[0]) / (*(float*)((char*)v3->rgba + (_DWORD)v11) - v3->rgba[0]);
      if (v3->rgba[0] <= (double)*v7)
      {
        v9 = pupper;
        v8 = v8 - 1.0;
      }
      else
      {
        v9 = plower;
      }
      v9->rgba[2] = (pupper->rgba[2] - plower->rgba[2]) * v8 + v9->rgba[2];
      v9->rgba[1] = (pupper->rgba[1] - plower->rgba[1]) * v8 + v9->rgba[1];
      v9->rgba[0] = (pupper->rgba[0] - plower->rgba[0]) * v8 + v9->rgba[0];
    }
    else
    {
      v4 = (int)v11;
    }
    v3 = (FCOLOR*)((char*)v3 + 4);
    --v10;
  } while (v10);
}

void __cdecl FcolorToColor(FCOLOR* pfcolor, S3TC_COLOR* pcolor)
{
  float* v2; // esi
  int v3; // ebx
  __int64 v4; // rax

  v2 = wtPrimary;
  v3 = 0;
  do
  {
    v4 = (__int64)(*(float*)((char*)v2 + ((char*)pfcolor - (char*)wtPrimary)) * 255.0 / *v2);
    ++v2;
    pcolor->rgba[v3++] = v4;
  } while (v2 < &wtPrimary[ARRAY_LENGTH(wtPrimary)]);
}

void __cdecl EncodeBlockRGBColorKey(S3TC_COLOR* colorSrc, S3TCBlockRGB* pblockDst, S3TC_COLOR colorLo, int colorHi)
{
  int v4; // edx
  unsigned __int8* v5; // eax
  int v6; // esi
  unsigned __int8 v7; // cl
  unsigned __int8 v8; // cl
  int v9; // ecx
  S3TC_COLOR* v10; // eax
  S3TC_COLOR* v11; // edi
  FCOLOR* v12; // esi
  int v13; // ebx
  int i; // ecx
  float* v15; // eax
  __int16 v16; // si
  float* v17; // edx
  int v18; // edi
  int j; // ecx
  float* v20; // eax
  int v21; // edx
  FCOLOR* v22; // edi
  float* v23; // eax
  int v24; // ecx
  float* v25; // edx
  float* v26; // esi
  int k; // edi
  double v28; // st7
  double v29; // st7
  int v30; // edx
  float* v31; // esi
  float* v32; // ecx
  int v33; // eax
  int v34; // edx
  int v35; // esi
  long double v36; // st7
  float* v37; // ecx
  float* v38; // eax
  FCOLOR* p_axis; // ecx
  int v40; // edx
  long double v41; // st7
  long double v42; // st6
  double v43; // st7
  FCOLOR* v44; // eax
  int v45; // ecx
  double v46; // st6
  double v47; // st5
  __int16 v48; // di
  FCOLOR* v49; // edx
  double v50; // st6
  FCOLOR* v51; // eax
  float* v52; // ecx
  int v53; // esi
  double v54; // st5
  double v55; // st6
  int m; // eax
  double v57; // st7
  float* v58; // ecx
  float* v59; // edx
  int v60; // eax
  double v61; // st7
  double v62; // st6
  __int16 v63; // ax
  FCOLOR* v64; // ebx
  double v65; // st3
  float* v66; // edx
  int n; // eax
  double v68; // st2
  float* v69; // ecx
  double v70; // st3
  double v71; // st3
  unsigned int pixbm; // edi
  int v73; // eax
  double v74; // st3
  unsigned int v75; // eax
  bool v76; // zf
  S3TCBlockRGB* v77; // [esp-8h] [ebp-1B0h]
  unsigned __int16 v78; // [esp-4h] [ebp-1ACh]
  FCOLOR c[16]; // [esp+Ch] [ebp-19Ch] BYREF
  float tt[3][3]; // [esp+10Ch] [ebp-9Ch] BYREF
  FCOLOR mean; // [esp+130h] [ebp-78h] BYREF
  FCOLOR axis; // [esp+140h] [ebp-68h] BYREF
  FCOLOR fcolor1; // [esp+150h] [ebp-58h] BYREF
  FCOLOR fcolor0; // [esp+160h] [ebp-48h] BYREF
  float t[3][3]; // [esp+170h] [ebp-38h] BYREF
  float* v86; // [esp+194h] [ebp-14h]
  FCOLOR* v87; // [esp+198h] [ebp-10h]
  int v88; // [esp+19Ch] [ebp-Ch]
  int cOpaque; // [esp+1A0h] [ebp-8h]
  int wAlpha; // [esp+1A4h] [ebp-4h]
  int colorSrca; // [esp+1B0h] [ebp+8h]
  int wMin; // [esp+1B8h] [ebp+10h]
  float wMina; // [esp+1B8h] [ebp+10h]
  unsigned __int16 wMinb; // [esp+1B8h] [ebp+10h]
  int primary; // [esp+1BCh] [ebp+14h]
  float primarya; // [esp+1BCh] [ebp+14h]

  wAlpha = 0;
  if (pblockDst)
  {
    v4 = 16;
    cOpaque = 0;
    v5 = (unsigned __int8*)&colorSrc[15] + 1;
    v6 = 1;
    do
    {
      v7 = v5[1];
      wAlpha *= 2;
      if (colorLo.rgba[2] > v7
        || v7 > BYTE2(colorHi)
        || colorLo.rgba[1] > *v5
        || *v5 > BYTE1(colorHi)
        || (v8 = *(v5 - 1), colorLo.rgba[0] > v8)
        || v8 > (unsigned __int8)colorHi)
      {
        wAlpha |= 1u;
        ++cOpaque;
      }
      else
      {
        wAlpha &= 0xFFFEu;
      }
      v5 -= 4;
      --v4;
    } while (v4);
    if (!cOpaque)
    {
      pblockDst->rgb0 = 0;
      pblockDst->rgb1 = -1;
      pblockDst->pixbm = -1;
      return;
    }
    v9 = 0;
    v10 = colorSrc;
    do
    {
      if (v6
        && v9 > 0
        && (v10->rgba[2] != v10[-1].rgba[2] || v10->rgba[1] != v10[-1].rgba[1] || v10->rgba[0] != v10[-1].rgba[0]))
      {
        v6 = 0;
      }
      ++v9;
      ++v10;
    } while (v9 < 16);
    if (v6)
    {
      AllSame(colorSrc, pblockDst, wAlpha);
      return;
    }
    v11 = colorSrc;
    v12 = c;
    v13 = 16;
    do
    {
      ColorToFcolor(v11++, v12++);
      --v13;
    } while (v13);
    for (i = 0; i < 3; ++i)
    {
      v15 = &mean.rgba[i];
      v16 = 1;
      v17 = &c[0].rgba[i];
      mean.rgba[i] = 0.0;
      v18 = 16;
      do
      {
        if (((unsigned __int16)wAlpha & (unsigned __int16)v16) != 0)
          *v15 = *v15 + *v17;
        v16 *= 2;
        v17 += 4;
        --v18;
      } while (v18);
      *v15 = *v15 / (double)cOpaque;
    }
    for (j = 0; j < 3; ++j)
    {
      v20 = &c[0].rgba[j];
      v21 = 16;
      do
      {
        *v20 = *v20 - mean.rgba[j];
        v20 += 4;
        --v21;
      } while (v21);
    }
    primary = 0;
    v22 = c;
    v86 = t[0];
    do
    {
      v23 = v86;
      v24 = 3 - primary;
      v87 = v22;
      do
      {
        v25 = (float*)v87;
        wMin = 1;
        v26 = (float*)v22;
        v88 = 16;
        *v23 = 0.0;
        do
        {
          if (((unsigned __int16)wAlpha & (unsigned __int16)wMin) != 0)
            *v23 = *v26 * *v25 + *v23;
          wMin *= 2;
          v25 += 4;
          v26 += 4;
          --v88;
        } while (v88);
        v87 = (FCOLOR*)((char*)v87 + 4);
        ++v23;
        --v24;
      } while (v24);
      ++primary;
      v86 += 4;
      v22 = (FCOLOR*)((char*)v22 + 4);
    } while (primary < 3);
    for (k = 0; k < 9; ++k)
    {
      Square3x3(t, tt);
      Square3x3(tt, t);
      v28 = t[0][0] + t[1][1] + t[2][2];
      if (v28 == 0.0)
        goto LABEL_60;
      v29 = 3.0 / v28;
      v30 = 0;
      v31 = t[0];
      do
      {
        v32 = v31;
        v33 = 3 - v30;
        do
        {
          *v32 = v29 * *v32;
          ++v32;
          --v33;
        } while (v33);
        ++v30;
        v31 += 4;
      } while (v30 < 3);
    }
    v34 = (int)pblockDst;
    *(_QWORD*)&t[2][0] = __PAIR64__(LODWORD(t[1][2]), LODWORD(t[0][2]));
    t[1][0] = t[0][1];
    v35 = 0;
    v36 = 0.0;
    v37 = t[0];
    do
    {
      if (v36 < *v37)
      {
        v36 = *v37;
        v34 = v35;
      }
      ++v35;
      v37 += 4;
    } while (v35 < 3);
    v38 = &t[0][v34];
    p_axis = &axis;
    v40 = 3;
    v41 = 1.0 / sqrt(v36);
    do
    {
      v42 = v41 * *v38;
      v38 += 3;
      p_axis->rgba[0] = v42;
      p_axis = (FCOLOR*)((char*)p_axis + 4);
      --v40;
    } while (v40);
    v43 = 0.0;
    v44 = &axis;
    v45 = 3;
    do
    {
      v46 = v44->rgba[0];
      v44 = (FCOLOR*)((char*)v44 + 4);
      --v45;
      v47 = v46 * v46 + v43;
      v43 = v47;
    } while (v45);
    if (v47 == 0.0)
    {
    LABEL_60:
      v78 = wAlpha;
      v77 = pblockDst;
    LABEL_77:
      AllSame(colorSrc, v77, v78);
      return;
    }
    primarya = -99999.0;
    v48 = 1;
    wMina = 99999.0;
    v49 = c;
    v88 = 16;
    do
    {
      if (((unsigned __int16)wAlpha & (unsigned __int16)v48) != 0)
      {
        v50 = 0.0;
        v51 = &axis;
        v52 = (float*)v49;
        v53 = 3;
        do
        {
          v54 = *v52++ * v51->rgba[0];
          v51 = (FCOLOR*)((char*)v51 + 4);
          --v53;
          v50 = v50 + v54;
        } while (v53);
        v55 = v50 / v43;
        if (v55 < wMina)
          wMina = v55;
        if (v55 > primarya)
          primarya = v55;
      }
      v48 *= 2;
      ++v49;
      --v88;
    } while (v88);
    for (m = 0; m < 3; axis.rgba[m + 3] = primarya * *v58 + *v59)
    {
      v57 = wMina * axis.rgba[m];
      v58 = &axis.rgba[m];
      v59 = &mean.rgba[m++];
      fcolor1.rgba[m + 3] = v57 + *v59;
    }
    ClipExtrema(&fcolor0, &fcolor1);
    Quantize(&fcolor0, &fcolor1, pblockDst, cOpaque);
    v60 = 0;
    v61 = 0.0;
    do
    {
      v62 = fcolor1.rgba[v60] - fcolor0.rgba[v60];
      ++v60;
      v61 = v62 * v62 + v61;
    } while (v60 < 3);
    if (v61 == 0.0 && cOpaque == 16)
    {
      v78 = wAlpha;
      v77 = pblockDst;
      goto LABEL_77;
    }
    v63 = 0x8000;
    wMinb = 0x8000;
    v64 = &c[15];
    colorSrca = 16;
    while (1)
    {
      if (((unsigned __int16)wAlpha & (unsigned __int16)v63) != 0)
      {
        v65 = 0.0;
        v66 = (float*)v64;
        for (n = 0; n < 3; v65 = (axis.rgba[n + 3] - *v69) * (v68 - *v69) + v65)
        {
          v68 = mean.rgba[n] + *v66;
          v69 = &fcolor0.rgba[n++];
          *v66++ = v68;
        }
        v70 = v65 / v61;
        if (cOpaque == 16)
        {
          v71 = v70 * 4.0;
          if (v71 >= 0.0)
          {
            if (v71 >= 4.0)
              v71 = 3.0;
          }
          else
          {
            v71 = 0.0;
          }
          pblockDst->pixbm *= 4;
          pixbm = pblockDst->pixbm;
          v73 = mapRGB4[(unsigned int)(__int64)v71];
        }
        else
        {
          v74 = v70 * 3.0;
          if (v74 >= 0.0)
          {
            if (v74 >= 3.0)
              v74 = 2.0;
          }
          else
          {
            v74 = 0.0;
          }
          pblockDst->pixbm *= 4;
          pixbm = pblockDst->pixbm;
          v73 = mapRGB3[(unsigned int)(__int64)v74];
        }
        v75 = pixbm | v73;
      }
      else
      {
        v75 = 4 * pblockDst->pixbm;
        LOBYTE(v75) = v75 | 3;
      }
      wMinb >>= 1;
      --v64;
      v76 = colorSrca-- == 1;
      pblockDst->pixbm = v75;
      if (v76)
        break;
      v63 = wMinb;
    }
  }
}

/////

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
    RegisterLine(&ArrayCopySpan, 0x10u, BltSpan_16BPP);
    RegisterLine(&ArrayCopySpan, 0x18u, BltSpan_24BPP);
    RegisterLine(&ArrayCopySpan, 0x20u, BltSpan_32BPP);
#if SKIP_ME != 1
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
