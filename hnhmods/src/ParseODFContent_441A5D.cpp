#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*ParseODFContent_441A5D)(LPBYTE pBuf1, CHanaFrameT3EC_S354 *pRootFrameT3EC,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1) =
    reinterpret_cast<int(__cdecl*)(LPBYTE, CHanaFrameT3EC_S354 *, CHanaAnimSetT3F1_S150 *)>(0x441A5D);
CHanaFrameT3EC_S354 *(__cdecl*ParseOdfFRAM_44106A)(LPBYTE *ppBuf,
    CHanaFrameT3EC_S354 *pRootFrameT3EC, void **ppParentList, int nIndex) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(LPBYTE *, CHanaFrameT3EC_S354 *,
      void **, int)>(0x44106A);
int (__cdecl*ParseOdfMESH_44068C)(LPBYTE *ppBuf, CHanaMeshT3F5_S138 **ppOut,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1, CHanaFrameT3EC_S354 *pRootFrameT3EC) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaMeshT3F5_S138 **,
      CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x44068C);
int (__cdecl*ParseOdfMORP_440896)(LPBYTE *ppBuf, CHanaMorpT457_S40C **ppOut,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1, CHanaFrameT3EC_S354 *pRootFrameT3EC) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaMorpT457_S40C **,
      CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x440896);
int (__cdecl*ParseOdfANIM_4414EB)(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSetT3F1,
    CHanaFrameT3EC_S354 *pCurrentFramT3EC) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x4414EB);
int (__cdecl*ParseOdf3DON_44189C)(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pHanaT3F1,
    CHanaFrameT3EC_S354 *pHanaRootT3EC_v1) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x44189C);
void (__cdecl*AssignLigthtToFrame_40539D)(CHanaFrameT3EC_S354 *pHanaT3EC, CHanaLightT3EF_S1A0 *pHanaLight) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaLightT3EF_S1A0 *)>(0x40539D);
int (__cdecl*CHanaObj__NewAnimT3F0)(CHanaAnimT3F0_S240 **ppOut) =
    reinterpret_cast<int(__cdecl*)(CHanaAnimT3F0_S240 **)>(0x401CD0);
int (__cdecl*HanaAnim_GetDuration)(CHanaAnimT3F0_S240 *pHanaAnimT3F0) =
    reinterpret_cast<int(__cdecl*)(CHanaAnimT3F0_S240 *)>(0x4039C9);
void (__cdecl*CHanaAnimT3F0__BakeKeyframesT3F0_402D0E)(CHanaAnimT3F0_S240 *pHanaAnimT3F0) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimT3F0_S240 *)>(0x402D0E);
void (__cdecl*HanaFrame_GetFrameName)(CHanaFrameT3EC_S354 *pHanaFramT3EC, char *pOutFrameName) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, char *)>(0x40BA89);
void (__cdecl*HanaAnim_LinkFrame)(CHanaAnimT3F0_S240 *pHanaAnimT3F0, CHanaFrameT3EC_S354 *pHanaFramT3EC) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimT3F0_S240 *, CHanaFrameT3EC_S354 *)>(0x401FBD);
void (__cdecl*InterpolateHanaAnimFrame)(CHanaAnimT3F0_S240 *pAnim, float fTime) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimT3F0_S240 *, float)>(0x40283C);
bool (__cdecl*CHanaAnimSetT3F1__AddAnimT3F0_403C15)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim) =
    reinterpret_cast<bool(__cdecl*)(CHanaAnimSetT3F1_S150 *, CHanaAnimT3F0_S240 *)>(0x403C15);
int (__cdecl*CHanaAnimSetT3F1__SetTime_403D61)(CHanaAnimSetT3F1_S150 *pHanaT3F1, float t2) =
    reinterpret_cast<int(__cdecl*)(CHanaAnimSetT3F1_S150 *, float)>(0x403D61);
int (__cdecl*CHanaObj__NewMeshT3F5)(CHanaMeshT3F5_S138 **pOut) =
    reinterpret_cast<int(__cdecl*)(CHanaMeshT3F5_S138 **)>(0x40FCA0);
int (__cdecl*ParseVERT_440288)(LPBYTE *ppBuf, CHanaVertT3EA_S1FC **ppVert,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1, CHanaFrameT3EC_S354 *pRootFrameT3EC) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaVertT3EA_S1FC **,
      CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x440288);
int (__cdecl*HanaMesh_AddVertexReference)(CHanaMeshT3F5_S138 *pMeshT3F5, CHanaVertT3EA_S1FC *pVertT3EA) =
    reinterpret_cast<int(__cdecl*)(CHanaMeshT3F5_S138 *, CHanaVertT3EA_S1FC *)>(0x40FCEB);
int (__cdecl*CHanaObj__NewMorpT457)(CHanaMorpT457_S40C **ppOut, CHanaMeshT3F5_S138 *pMeshT3F5) =
    reinterpret_cast<int(__cdecl*)(CHanaMorpT457_S40C **, CHanaMeshT3F5_S138 *)>(0x433DA7);
int (__cdecl*HanaMorp_AddVertexRef)(CHanaMorpT457_S40C *pMorpT457, CHanaVertT3EA_S1FC *pVertT3EA) =
    reinterpret_cast<int(__cdecl*)(CHanaMorpT457_S40C *, CHanaVertT3EA_S1FC *)>(0x434370);
void (__cdecl*HanaMorp_InitSubObject)(CHanaMorpT457_S40C *pMorpT457, int zero,
    int n1, int n0, int n2, int n3, CHanaSubObj_S2C *parrS2C, int n4count,
    const char *sz, int idx) =
    reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *, int, int, int, int, int,
      CHanaSubObj_S2C *, int, const char *, int)>(0x439982);
CHanaFrameT3EC_S354 *(__cdecl*HanaFrame_FindByName)(LPCSTR lpString,
    CHanaFrameT3EC_S354 *pHanaT3EC_v1) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(LPCSTR, CHanaFrameT3EC_S354 *)>(0x4417EB);
int (__cdecl*CalcFrameBound_404B80)(CHanaFrameT3EC_S354 *pFrameT3EC, CHanaMorpT457_S40C *pMorpT3F5) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaMorpT457_S40C *)>(0x404B80);
int (__cdecl*CHanaObj__NewFrameArrayT3EDFromChildT3EC)(CHanaFrameT3EC_S354 *pFrame_a1,
    CHanaFrameArrayT3ED_S138 **ppFrameArray) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameArrayT3ED_S138 **)>(0x405A0A);
int (__cdecl*CHanaObj__GetFramesCountT3ED_408E75)(CHanaFrameArrayT3ED_S138 *pFrameArray) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameArrayT3ED_S138 *)>(0x408E75);
int (__cdecl*CHanaObj__GetFrameAtIndexT3ED)(CHanaFrameArrayT3ED_S138 *pFrameArray,
    unsigned int nIndex, CHanaFrameT3EC_S354 **ppOut) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameArrayT3ED_S138 *, unsigned int,
      CHanaFrameT3EC_S354 **)>(0x408E1B);
D3DXMATRIX *(__cdecl*MtxMult_4069DD)(CHanaFrameT3EC_S354 *pChildFrame,
    CHanaFrameT3EC_S354 *pParentFrame, float x, float y, float z) =
    reinterpret_cast<D3DXMATRIX *(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *,
      float, float, float)>(0x4069DD);
void (__cdecl * ApplyQTS_4035B1)( D3DXMATRIX *pOut, D3DXQUATERNION *pQ, float x, float y, float z, float sx, float sy, float sz) =
  reinterpret_cast<void(__cdecl*)( D3DXMATRIX *pOut, 
    D3DXQUATERNION *pQ, float x, float y, float z, float sx, float sy, float sz)>(0x4035B1);
void (__cdecl * ConfigureLight)( CHanaLightT3EF_S1A0 *pHanaLight, float posX, float posY, float posZ, float dirX, float dirY, float dirZ) = reinterpret_cast<void(__cdecl*)( CHanaLightT3EF_S1A0 *pHanaLight, float posX, float posY, float posZ, float dirX, float dirY, float dirZ)>(0x409510);
void (__cdecl * HanaFrame_GetCenterPosition_408C3E)( CHanaFrameRootT3EC_S2D4 *pFrame, CHanaFrameRootT3EC_S2D4 *pRelFrame, float *pOutX, float *pOutY, float *pOutZ) = reinterpret_cast<void(__cdecl*)( CHanaFrameRootT3EC_S2D4 *pFrame, CHanaFrameRootT3EC_S2D4 *pRelFrame, float *pOutX, float *pOutY, float *pOutZ)>(0x408C3E); 

// === FUNCTIONS ===

typedef char String256[256];

// ParseODFContent_441A5D -- content pass of the ODF (3D object/scene) parser;
// called only by ParseODFFile_441F19 after the header pass. Walks the tag
// stream, keeping the most recent frame as "current":
//   FRAM  ParseOdfFRAM_44106A  new frame; also grows the parallel parent-name
//                              list (one 256-char entry per frame); consecutive
//                              FRAMs chain onto it
//   MESH  ParseOdfMESH_44068C  mesh for the current frame; CalcFrameBound then
//                              frees the temp mesh (typed as morp in the call)
//   MORP  ParseOdfMORP_440896  morph, same treatment
//   ANIM  ParseOdfANIM_4414EB  frame animation for the current frame
//   3DON  ParseOdf3DON_44189C  nested 3D object off the temp root
// An unknown tag breaks the dispatch loop; the scan then steps one byte at a
// time until the Shift-JIS terminator below (or a FRAM re-enters dispatch).
// Afterwards every parsed frame is re-parented: the entry in the parent-name
// list is looked up among ALL frames by szName_8 -> AttachChild + MtxMult;
// unnamed or unmatched frames attach to the caller's root frame instead.
// Always returns 1.

BOOL __cdecl hook_ParseODFContent_441A5D(
  LPBYTE pBuf1,
  CHanaFrameT3EC_S354 *pRootFrameT3EC,
  CHanaAnimSetT3F1_S150 *pAnimSetT3F1)
{
  CHanaFrameT3EC_S354 *pParentFrame; // [esp+0h] [ebp-3Ch] BYREF
  CHanaFrameT3EC_S354 *pChild; // [esp+4h] [ebp-38h] BYREF
  String256 *pParentList; // [esp+8h] [ebp-34h] BYREF
  int nFramIndex; // [esp+10h] [ebp-2Ch]
  int nMorpIndex; // [esp+14h] [ebp-28h]
  unsigned int iParent; // [esp+18h] [ebp-24h]
  CHanaMeshT3F5_S138 *pMeshT3F5; // [esp+1Ch] [ebp-20h] BYREF
  unsigned int iChild; // [esp+20h] [ebp-1Ch]
  CHanaFrameT3EC_S354 *pCurrentFrame; // [esp+24h] [ebp-18h]
  int nMeshIndex; // [esp+28h] [ebp-14h]
  CHanaFrameArrayT3ED_S138 *pChildFramesEnum; // [esp+2Ch] [ebp-10h] BYREF
  CHanaFrameT3EC_S354 *pRootFrameT3EC_v1; // [esp+30h] [ebp-Ch] BYREF
  unsigned int nFramesCount; // [esp+34h] [ebp-8h]
  CHanaMorpT457_S40C *pMorpT457; // [esp+38h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] ParseODFContent_441A5D\n");

  nMeshIndex = 0;
  nMorpIndex = 0;
  nFramIndex = 0;
  pMeshT3F5 = 0;
  pMorpT457 = 0;
  pRootFrameT3EC_v1 = 0;
  pParentList = (String256 *)MemAlloc_40DEF7(0x200u);
  MtxStackStartMultLocal_4074EF();
  CHanaObj__NewFrameT3EC_S354(0, &pRootFrameT3EC_v1);
  pCurrentFrame = 0;
  for ( ;; )
  {
    for ( ;; )                                 // is 'FRAM'
    {
      while ( !memcmp(pBuf1, "FRAM", 4u) )
      {
        pBuf1 += 4;
        pCurrentFrame = ParseOdfFRAM_44106A(&pBuf1, pRootFrameT3EC_v1, (void **)&pParentList, ++nFramIndex);
      }
      if ( !pCurrentFrame )
        break;
      if ( !memcmp(pBuf1, "MESH", 4u) )         // is 'MESH'
      {
        pBuf1 += 4;
        ++nMeshIndex;
        ParseOdfMESH_44068C(&pBuf1, &pMeshT3F5, pAnimSetT3F1, pRootFrameT3EC_v1);
        CalcFrameBound_404B80(pCurrentFrame, (CHanaMorpT457_S40C *)pMeshT3F5);
        if ( pMeshT3F5 )
        {
          CHanaObj__Free_40C1E4(pMeshT3F5);
          pMeshT3F5 = 0;
        }
      }
      else if ( !memcmp(pBuf1, "MORP", 4u) )    // is 'MORP'
      {
        pBuf1 += 4;
        ++nMorpIndex;
        ParseOdfMORP_440896(&pBuf1, &pMorpT457, pAnimSetT3F1, pRootFrameT3EC_v1);
        CalcFrameBound_404B80(pCurrentFrame, pMorpT457);
        if ( pMorpT457 )
        {
          CHanaObj__Free_40C1E4(pMorpT457);
          pMorpT457 = 0;
        }
      }
      else if ( !memcmp(pBuf1, "ANIM", 4u) )    // is 'ANIM'
      {
        pBuf1 += 4;
        ParseOdfANIM_4414EB(&pBuf1, pAnimSetT3F1, pCurrentFrame);
      }
      else if ( !memcmp(pBuf1, "3DON", 4u) )        // is '3DON'
      {
        ParseOdf3DON_44189C(&pBuf1, pAnimSetT3F1, pRootFrameT3EC_v1);
      } else
        break;
    }
    if ( !memcmp(pBuf1, "ファイルは終了", 14u) )
      break;
    ++pBuf1;
  }

  // re-parent: match each frame's recorded parent name against all frames;
  // unnamed or unmatched frames go to the caller's root
    
  pChildFramesEnum = 0;
  CHanaObj__NewFrameArrayT3EDFromChildT3EC(pRootFrameT3EC_v1, &pChildFramesEnum);
  nFramesCount = CHanaObj__GetFramesCountT3ED_408E75(pChildFramesEnum);
  for ( iChild = 0; iChild < nFramesCount; ++iChild )
  {
    BOOL bFoundParent = FALSE;
    pChild = 0;
    CHanaObj__GetFrameAtIndexT3ED(pChildFramesEnum, iChild, &pChild);
    if ( pParentList[iChild][0] )
    { 
      pParentFrame = 0;
      for ( iParent = 0; iParent < nFramesCount; ++iParent )
      {
        CHanaObj__GetFrameAtIndexT3ED(pChildFramesEnum, iParent, &pParentFrame);
        if ( !lstrcmpA(pParentList[iChild], pParentFrame->szName_8) )
        {
          AttachChild_4055FD(pParentFrame, pChild);
          MtxMult_4069DD(pChild, pParentFrame, 0.0, 0.0, 0.0);

          bFoundParent = TRUE; // original encodes this as iParent = -1
        }
        if ( pParentFrame )
        {
          CHanaObj__Free_40C1E4(pParentFrame);
          pParentFrame = 0;
        }
        if (bFoundParent)
          break;
      }
    }
    
    if ( !bFoundParent )
    {
      AttachChild_4055FD(pRootFrameT3EC, pChild);
      MtxMult_4069DD(pChild, pRootFrameT3EC, 0.0, 0.0, 0.0);
    }

    if ( pChild )
    {
      CHanaObj__Free_40C1E4(pChild);
      pChild = 0;
    }
  }
  
  if ( pChildFramesEnum )
  {
    CHanaObj__Free_40C1E4(pChildFramesEnum);
    pChildFramesEnum = 0;
  }
  if ( pRootFrameT3EC_v1 )
  {
    CHanaObj__Free_40C1E4(pRootFrameT3EC_v1);
    pRootFrameT3EC_v1 = 0;
  }
  if ( pParentList )
  {
    MemFree_40DF64(pParentList);
    pParentList = 0;
  }
  return TRUE;
}

// ParseOdfFRAM_44106A -- parse one FRAM block; only xref is the FRAM dispatch
// in hook_ParseODFContent_441A5D above. Block layout:
//   [DWORD nSize][name]                 frame name; the magic name
//                                       "CameraFrame" adopts g_pCameraFrame
//                                       instead of creating a frame (lets the
//                                       file drive the camera)
//   PARN  [DWORD][DWORD nSize][name]    parent frame name -> stored into the
//                                       grown parent-name list at [nIndex-1]
//                                       (list is reallocated to nIndex*256+256
//                                       bytes -- one entry over-allocated in
//                                       the original -- old (nIndex-1)*256
//                                       bytes copied)
//   LIGH  [8-byte hdr][light payload]   D3DLIGHT7 copied out, NewLightT3EF +
//                                       AssignLigthtToFrame onto this frame;
//                                       payload advances 8+104 bytes
//   FFLG  [8-byte hdr][WORD flags]      bit0 -> flag_218, bit1 -> bFlagAlpha_130,
//                                       bit2 -> flag_134, bit3 -> flag_21C
//   End-Objection (aEndObjection @0x4B411C) ends the attribute scan
//   FMRX  [DWORD][DWORD nSize][matrix]  overwrite of the ROOT frame's
//                                       mat4x4_fmrx
// PARN and FMRX are searched within a 25-byte window byte-by-byte; the
// attribute scan runs up to 25 iterations and handles tags in sequence.
// (VX-N / MH-N / MP-N map to type codes 1002/1013/1111 that are computed and
// never used -- dead stores in the original, dropped here.)
CHanaFrameT3EC_S354 *__cdecl hook_ParseOdfFRAM_44106A(LPBYTE *ppBuf,
        CHanaFrameT3EC_S354 *pRootFrameT3EC, void **ppParentList, int nIndex)
{
    D3DLIGHT7 d3dLight;                      // [esp+10h] [ebp-190h]
    void *pHanaLight;                        // [esp+Ch] [ebp-194h]
    CHanaFrameT3EC_S354 *pFrame;
    CHAR szName[256];                        // [esp+7Ch] [ebp-124h]
    CHAR (*pParentList)[256];                // [esp+17Ch] [ebp-24h]
    int i, nSize;                            // nSize = [ebp-4h]
    LPBYTE pBuf, pScan;                      // [ebp-10h] / [ebp-Ch]
    WORD wFlags;                             // [esp+8h] [ebp-198h]

    DebugLog("[HNH:SUB] ParseOdfFRAM_44106A\n");

    pBuf = *ppBuf;
    pParentList = (CHAR (*)[256])*ppParentList;

    // [DWORD nSize][frame name]
    nSize = *(int *)pBuf;
    pBuf += 4;
    wsprintfA(szName, "%s", (const char *)pBuf);
    pBuf += nSize;

    // create the frame, or adopt the camera for the magic name
    if (!memcmp(szName, "CameraFrame", 11u))
    {
        pFrame = g_pCameraFrame;
        AttachChild_4055FD(pRootFrameT3EC, g_pCameraFrame);
    }
    else
    {
        CHanaObj__NewFrameT3EC_S354(pRootFrameT3EC, &pFrame);
        AttachHanaObj_40BAB8((CHanaBase_S130 *)pFrame, szName);
    }

    // PARN: parent name within 25 bytes
    pScan = pBuf;
    for (i = 0; i < 25; ++i)
    {
        if (!memcmp("PARN", pScan, 4))
        {
            nSize = *((int *)pScan + 1);
            wsprintfA(szName, "%s", (const char *)(pScan + 8));

            // grow the parent-name list; store the name at [nIndex-1]
            CHAR (*pNewList)[256] = (CHAR (*)[256])MemAlloc_40DEF7((nIndex << 8) + 256);
            memcpy(pNewList, pParentList, (nIndex - 1) << 8);
            if (pParentList)
            {
                MemFree_40DF64(pParentList);
                pParentList = NULL;
            }
            pParentList = pNewList;
            wsprintfA(pNewList[nIndex - 1], "%s", szName);

            pBuf = pScan + 8 + nSize;
            break;
        }
        ++pScan;
    }

    // attribute scan: LIGH / FFLG / End-Objection in sequence, 25 rounds max
    pScan = pBuf;
    for (i = 0; i < 25; ++i)
    {
        if (!memcmp("LIGH", pScan, 4))
        {
            pScan += 8;
            memcpy(&d3dLight, pScan, sizeof(d3dLight));
            CHanaObj__NewLightT3EF((CHanaLightT3EF_S1A0 **)&pHanaLight, &d3dLight);
            AssignLigthtToFrame_40539D(pFrame, (CHanaLightT3EF_S1A0 *)pHanaLight);
            if (pHanaLight)
            {
                CHanaObj__Free_40C1E4((CHanaBase_S130 *)pHanaLight);
                pHanaLight = NULL;
            }
            pScan += 104;                    // 8-byte header + 96-byte payload
            pBuf = pScan;
        }
        else if (!memcmp("FFLG", pScan, 4))
        {
            pScan += 8;
            wFlags = *(WORD *)pScan;
            if (wFlags & 1)
                pFrame->flag_218 = 1;
            if (wFlags & 2)
                pFrame->bFlagAlpha_130 = 1;
            if (wFlags & 4)
                pFrame->flag_134 = 1;
            if (wFlags & 8)
                pFrame->flag_21C = 1;
            pScan += 2;
            pBuf = pScan;
        }
        else if (!memcmp("End-Objection", pScan, 13))   // aEndObjection @0x4B411C
        {
            pScan += 13;
            pBuf = pScan;
            break;
        }
        else
        {
            ++pScan;
        }
    }

    // FMRX: root frame transform matrix within 25 bytes
    pScan = pBuf;
    for (i = 0; i < 25; ++i)
    {
        if (!memcmp("FMRX", pScan, 4))
        {
            nSize = *((int *)pScan + 1);
            memcpy(&pRootFrameT3EC->mat4x4_fmrx, pScan + 8, sizeof(pRootFrameT3EC->mat4x4_fmrx));
            pBuf = pScan + 8 + nSize;
            break;
        }
        ++pScan;
    }

    *ppBuf = pBuf;
    *ppParentList = pParentList;
    return pFrame;
}

// ParseOdfANIM_4414EB -- frame animation tag; 7 xrefs (the ANIM dispatch in
// hook_ParseODFContent_441A5D above plus the ODS/OdsEx parsers). Layout:
//   [DWORD nBytes][nKeys = nBytes/56 keys of 56 bytes each]
// Each on-disk key is a CHanaSubObj_S38 (t, pos, scale, quat) expanded into a
// 120-byte CHanaAnimKeyT3F0_S78 key (t, x/y/z, sx/sy/sz, quat, b_2C/b_30/b_34 = 1,
// matrix slot); the key array is allocated with one extra sentinel slot and
// fResolution set to 1e8. After copying: HanaAnim_GetDuration (return value
// ignored), BakeKeyframesT3F0, then -- only when a current frame is given --
// the anim is named "<frameName>_Animation", registered in the object table,
// linked to the frame, interpolated at t=0, added to the anim set, and the set
// seeked to 1.0. The temp anim object is freed afterwards, as everywhere in
// this parser. Always returns TRUE.
BOOL __cdecl hook_ParseOdfANIM_4414EB(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSetT3F1,
                                      CHanaFrameT3EC_S354 *pCurrentFramT3EC)
{
    CHanaAnimT3F0_S240 *pAnimT3F0;           // [esp+0h] [ebp-118h]
    CHAR pszAnimName[256];                   // [esp+4h] [ebp-114h]
    DWORD *pBuf;                             // [esp+10Ch] [ebp-Ch]
    DWORD i, nCount;                         // [ebp-14h] / [ebp-4h]

    DebugLog("[HNH:SUB] ParseOdfANIM_4414EB\n");

    pBuf = (DWORD *)*ppBuf;
    pAnimT3F0 = NULL;
    nCount = *pBuf++ / 56;                   // 56 bytes per on-disk key

    CHanaObj__NewAnimT3F0(&pAnimT3F0);
    MemFree_40DF64(pAnimT3F0->arroS78_13C);  // drop the default key array
    pAnimT3F0->arroS78_13C = (CHanaAnimKeyT3F0_S78 *)MemAlloc_40DEF7(120 * (nCount + 1)); // +1 sentinel
    pAnimT3F0->fResolution_134 = 100000000.0f;
    pAnimT3F0->nCount_138 = nCount;

    for (i = 0; i < nCount; ++i)
    {
        CHanaSubObj_S38 *pKey = (CHanaSubObj_S38 *)pBuf;   // 56-byte on-disk key
        CHanaAnimKeyT3F0_S78 *pOutKey = &pAnimT3F0->arroS78_13C[i];

        pOutKey->t_0 = pKey->t;
        pOutKey->quart_1C = pKey->quat;
        pOutKey->sx_10 = pKey->scale.x;
        pOutKey->sy_14 = pKey->scale.y;
        pOutKey->sz_18 = pKey->scale.z;
        pOutKey->x_4 = pKey->pos.x;
        pOutKey->y_8 = pKey->pos.y;
        pOutKey->z_C = pKey->pos.z;
        pOutKey->b_2C = 1;
        pOutKey->b_30 = 1;
        pOutKey->b_34 = 1;
        pBuf += 14;                          // 14 dwords = 56 bytes
    }

    HanaAnim_GetDuration(pAnimT3F0);         // return value ignored
    CHanaAnimT3F0__BakeKeyframesT3F0_402D0E(pAnimT3F0);

    if (pCurrentFramT3EC)
    {
        HanaFrame_GetFrameName(pCurrentFramT3EC, pszAnimName);
        wsprintfA(pszAnimName, "%s_Animation", pszAnimName);
        AttachHanaObj_40BAB8((CHanaBase_S130 *)pAnimT3F0, pszAnimName);
        HanaAnim_LinkFrame(pAnimT3F0, pCurrentFramT3EC);
        InterpolateHanaAnimFrame(pAnimT3F0, 0.0f);
        CHanaAnimSetT3F1__AddAnimT3F0_403C15(pAnimSetT3F1, pAnimT3F0);
        CHanaAnimSetT3F1__SetTime_403D61(pAnimSetT3F1, 1.0f);
        Sleep(0);
    }

    if (pAnimT3F0)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130 *)pAnimT3F0);
        pAnimT3F0 = NULL;
    }
    *ppBuf = (LPBYTE)pBuf;
    return TRUE;
}

// ParseOdfMESH_44068C -- mesh tag; xrefs: the MESH dispatch in
// hook_ParseODFContent_441A5D above and ParseOdfMORP_440896 (morphs embed a
// mesh block). Layout:
//   [DWORD nHeaderSize -- read, never used][DWORD nVertCount][mesh name ASCIIZ]
// Then scans for up to nVertCount VERT blocks (do-while: one VERT is parsed
// even when the count is 0). Each VERT comes from ParseVERT_440288, is
// referenced into the mesh, and -- when it came back unnamed -- gets a random
// unique name "<nVertIndex>_<meshName>_vertex%03d%03d%03d" built from three
// rand() calls, printed in REVERSE call order (3rd, 2nd, 1st). The temp vert
// object is freed each round. Scanning stops early on ANIM/FRAM/MESH/MORP or
// the Shift-JIS terminator (escape-encoded below to stay codepage-safe).
// Returns 1; *ppOut receives the mesh.
int __cdecl hook_ParseOdfMESH_44068C(LPBYTE *ppBuf, CHanaMeshT3F5_S138 **ppOut,
                                     CHanaAnimSetT3F1_S150 *pAnimSetT3F1,
                                     CHanaFrameT3EC_S354 *pRootFrameT3EC)
{
    CHanaMeshT3F5_S138 *pMeshT3F5;          // [esp+104h] [ebp-18h]
    CHanaVertT3EA_S1FC *pVertT3EA;          // [esp+108h] [ebp-14h]
    CHAR szMeshName[256];                   // [esp+4h] [ebp-118h]
    LPBYTE pBuf;                            // [esp+110h] [ebp-Ch]
    unsigned int nVertCount, nVertIndex, nHeaderSize;
    int nRand1, nRand2, nRand3;
    BOOL bStopped = FALSE;

    DebugLog("[HNH:SUB] ParseOdfMESH_44068C\n");

    pBuf = *ppBuf;
    pVertT3EA = NULL;
    pMeshT3F5 = NULL;
    nVertIndex = 0;
    nHeaderSize = *(unsigned int *)pBuf;    // header size -- read, never used
    pBuf += 4;
    nVertCount = *(unsigned int *)pBuf;
    pBuf += 4;
    wsprintfA(szMeshName, "%s", (const char *)pBuf);
    pBuf += lstrlenA(szMeshName) + 1;

    CHanaObj__NewMeshT3F5(&pMeshT3F5);
    AttachHanaObj_40BAB8((CHanaBase_S130 *)pMeshT3F5, szMeshName);

    do
    {
        while (memcmp(pBuf, "VERT", 4))     // scan for the next VERT
        {
            if (!memcmp(pBuf, "ANIM", 4) || !memcmp(pBuf, "FRAM", 4)
                || !memcmp(pBuf, "MESH", 4) || !memcmp(pBuf, "MORP", 4)
                // Shift-JIS @0x4B416C: "ファイルは終了" ("file ends")
                || !memcmp(pBuf, "\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xCD\x8F\x49\x97\xB9", 14))
            {
                bStopped = TRUE;
                break;
            }
            ++pBuf;
        }
        if (bStopped)
            break;

        pBuf += 4;
        ParseVERT_440288(&pBuf, &pVertT3EA, pAnimSetT3F1, pRootFrameT3EC);
        HanaMesh_AddVertexReference(pMeshT3F5, pVertT3EA);
        if (!pVertT3EA->szName_8[0])        // give unnamed verts a unique name
        {
            nRand1 = rand();                // three rand() calls, printed in
            nRand2 = rand();                // reverse call order (3rd, 2nd, 1st)
            nRand3 = rand();
            wsprintfA(pVertT3EA->szName_8, "%d_%s_vertex%03d%03d%03d",
                      nVertIndex, pMeshT3F5->szName_8, nRand3, nRand2, nRand1);
        }
        if (pVertT3EA)
        {
            CHanaObj__Free_40C1E4((CHanaBase_S130 *)pVertT3EA);
            pVertT3EA = NULL;
        }
        ++nVertIndex;
    } while (nVertCount > nVertIndex);

    *ppBuf = pBuf;
    *ppOut = pMeshT3F5;
    return 1;
}

// ParseOdfMORP_440896 -- morph tag; only xref is the MORP dispatch in
// hook_ParseODFContent_441A5D above. Block layout:
//   [DWORD][DWORD] morp name      (both header dwords are read, never used)
//   MESH ...                       embedded base mesh (scanned within 50 bytes,
//                                  parsed by ParseOdfMESH_44068C), then the
//                                  temp mesh ref freed after NewMorpT457
//   MPPL [DWORD nCount] VERT*      vertex palette: nCount rounds, each VERT
//                                  parsed + referenced into the morph
//   Mkey* per vertex               for each mesh vertex (nCountVertT3EA):
//                                  [DWORD nKeys][nKeys x {DWORD x, DWORD y}]
//                                  into arrMKeyT3EASize/arrMKeyT3EA
//                                  (8 bytes per key, +80 over-allocated)
//   ParM [DWORD nCount] records    per record: 20-byte header {f0,f4,f8,fC,
//                                  nCount}, ASCIIZ name, then nCount x 44-byte
//                                  CHanaSubObj_S2C blobs -> HanaMorp_InitSub-
//                                  Object(field_4, field_0, field_8, field_C,
//                                  ... -- note the swapped first two args)
//   PAKU                           6 dwords -> dwEaseEnabled/fEaseFrom/fEaseTo/
//                                  nTimeState/fTimeFrom/fTimeTo; resets
//                                  fEaseProgress and dwClockStart; an optional
//                                  7th dword (fEaseRate) follows unless the
//                                  next tag is "Mani"
// Cursor quirks kept: the MPPL VERT scan uses pScan to peek but advances pBuf
// by a fixed +4 (they stay in sync only when VERT is at the scan start), and
// the MPPL stop tags (Mkey/FRAM/MESH/MORP/terminator) are tested at pBuf, not
// at the scan cursor. The terminator is escape-encoded to stay codepage-safe.
// Returns 1; *ppOut receives the morph.
int __cdecl hook_ParseOdfMORP_440896(LPBYTE *ppBuf, CHanaMorpT457_S40C **ppOut,
                                     CHanaAnimSetT3F1_S150 *pAnimSetT3F1,
                                     CHanaFrameT3EC_S354 *pRootFrameT3EC)
{
    CHanaVertT3EA_S1FC *pVertT3EA;          // [esp+Ch] [ebp-230h]
    CHanaMeshT3F5_S138 *pMeshT3F5;          // [esp+230h] [ebp-Ch]
    CHanaMorpT457_S40C *pMorpT457;          // [esp+118h] [ebp-124h]
    CHAR szMorpName[256];                   // [esp+14h] [ebp-228h]
    CHAR szSubObjName[256];                 // [esp+120h] [ebp-11Ch]
    DWORD arrHdr[5];                        // ParM 20-byte record
    CHanaSubObj_S2C *parrS2C;
    LPBYTE pBuf, pScan;                     // [ebp-14h] / [ebp-10h] (Buf1)
    DWORD nCount, nParMCount;
    int i, j;
    unsigned int k, idx;

    DebugLog("[HNH:SUB] ParseOdfMORP_440896\n");

    pBuf = *ppBuf;
    pMorpT457 = NULL;
    pVertT3EA = NULL;
    pMeshT3F5 = NULL;

    pBuf += 8;                              // [DWORD][DWORD]: read, never used
    wsprintfA(szMorpName, "%s", (const char *)pBuf);
    pBuf += lstrlenA(szMorpName) + 1;

    // embedded MESH within 50 bytes
    for (idx = 0; idx < 50; ++idx)
    {
        if (!memcmp(pBuf, "MESH", 4))
        {
            pBuf += 4;
            ParseOdfMESH_44068C(&pBuf, &pMeshT3F5, pAnimSetT3F1, pRootFrameT3EC);
            break;
        }
        ++pBuf;
    }
    CHanaObj__NewMorpT457(&pMorpT457, pMeshT3F5);
    if (pMeshT3F5)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130 *)pMeshT3F5);
        pMeshT3F5 = NULL;
    }
    AttachHanaObj_40BAB8((CHanaBase_S130 *)pMorpT457, szMorpName);

    // MPPL: vertex palette within 25 bytes
    pScan = pBuf;
    for (i = 0; i < 25; ++i)
    {
        if (!memcmp(pScan, "MPPL", 4))
        {
            pBuf = pScan;
            break;
        }
        ++pScan;
    }
    if (!memcmp(pBuf, "MPPL", 4))
    {
        pBuf += 4;
        nCount = *(DWORD *)pBuf;
        pBuf += 4;
        pScan = pBuf;
        for (idx = 0; idx < nCount; ++idx)
        {
            for (j = 0; j < 20 && memcmp(pScan, "VERT", 4); ++j)
                ++pScan;
            if (!memcmp(pScan, "VERT", 4))
            {
                pBuf += 4;                  // skips from pBuf, NOT pScan
                ParseVERT_440288(&pBuf, &pVertT3EA, pAnimSetT3F1, pRootFrameT3EC);
                HanaMorp_AddVertexRef(pMorpT457, pVertT3EA);
                if (pVertT3EA)
                {
                    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pVertT3EA);
                    pVertT3EA = NULL;
                }
                pScan = pBuf;
            }
            else
            {
                // stop tags tested at pBuf (not the scan cursor) -- original
                if (!memcmp(pBuf, "Mkey", 4) || !memcmp(pBuf, "FRAM", 4)
                    || !memcmp(pBuf, "MESH", 4) || !memcmp(pBuf, "MORP", 4)
                    // Shift-JIS @0x4B416C: "ファイルは終了" ("file ends")
                    || !memcmp(pBuf, "\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xCD\x8F\x49\x97\xB9", 14))
                    break;
                ++pBuf;
            }
        }
    }

    // Mkey tracks: one per mesh vertex (scan window 50 bytes each)
    pScan = pBuf;
    for (idx = 0; idx < pMorpT457->pMeshT3F5_138->nCountVertT3EA_130; ++idx)
    {
        for (k = 0; k < 50 && memcmp("Mkey", pScan, 4); ++k)
            ++pScan;
        if (k != 50)
        {
            pScan += 4;
            pMorpT457->arrMKeyT3EASize_348[idx] = *(int *)pScan;
            pScan += 4;
            pBuf = pScan;
            if (pMorpT457->arrMKeyT3EASize_348[idx])
            {
                pMorpT457->arrMKeyT3EA_34C[idx] = (CHanaVector2 *)MemAlloc_40DEF7(
                    8 * pMorpT457->arrMKeyT3EASize_348[idx] + 80);
                for (k = 0; k < (unsigned int)pMorpT457->arrMKeyT3EASize_348[idx]; ++k)
                {
                    pMorpT457->arrMKeyT3EA_34C[idx][k].x = *(int *)pBuf;
                    pBuf += 4;
                    pMorpT457->arrMKeyT3EA_34C[idx][k].y = *(int *)pBuf;
                    pBuf += 4;
                }
            }
            pScan = pBuf;
        }
    }

    // ParM: sub-object table within 50 bytes
    pScan = pBuf;
    for (k = 0; k < 50 && memcmp("ParM", pScan, 4); ++k)
        ++pScan;
    if (k != 50)
    {
        pScan += 4;
        nParMCount = *(DWORD *)pScan;
        pScan += 4;
        pBuf = pScan;
        for (idx = 0; idx < nParMCount; ++idx)
        {
            memcpy(arrHdr, pBuf, 20);       // {field_0, field_4, field_8, field_C, nCount_10}
            pBuf += 20;
            wsprintfA(szSubObjName, "%s", (const char *)pBuf);
            pBuf += lstrlenA(szSubObjName) + 1;
            if ((int)arrHdr[4] <= 0)
                break;
            parrS2C = (CHanaSubObj_S2C *)MemAlloc_40DEF7(44 * arrHdr[4] + 128);
            memcpy(parrS2C, pBuf, 44 * arrHdr[4]);
            HanaMorp_InitSubObject(pMorpT457, 0,
                                   (int)arrHdr[1],          // field_4
                                   (int)arrHdr[0],          // field_0
                                   (int)arrHdr[2],          // field_8
                                   (int)arrHdr[3],          // field_C
                                   parrS2C, (int)arrHdr[4], // nCount_10
                                   szSubObjName, (int)idx);
            pBuf += 44 * arrHdr[4];
        }
    }

    // PAKU: ease/timing block within 25 bytes
    pScan = pBuf;
    for (k = 0; k < 0x19 && memcmp("PAKU", pScan, 4); ++k)
        ++pScan;
    if (!memcmp("PAKU", pScan, 4))
    {
        pScan += 4;
        pBuf = pScan;
        pMorpT457->dwEaseEnabled_358 = *(int *)pBuf;   pBuf += 4;
        pMorpT457->fEaseFrom_35C    = *(int *)pBuf;    pBuf += 4;
        pMorpT457->fEaseTo_360      = *(int *)pBuf;    pBuf += 4;
        pMorpT457->nTimeState_36C   = *(int *)pBuf;    pBuf += 4;
        pMorpT457->fTimeFrom_370    = *(int *)pBuf;    pBuf += 4;
        pMorpT457->fTimeTo_374      = *(int *)pBuf;    pBuf += 4;
        pMorpT457->fEaseProgress_384 = 0;
        pMorpT457->dwClockStart_380 = 0;
        if (memcmp("Mani", pBuf, 4))       // optional 7th dword unless "Mani"
        {
            pMorpT457->fEaseRate_388 = *(int *)pBuf;
            pBuf += 4;
        }
    }

    *ppBuf = pBuf;
    *ppOut = pMorpT457;
    return 1;
}

// ParseOdf3DON_44189C -- 3DON tag: the anim set's table of spatialized 3D
// sound emitters; only xref is the 3DON dispatch in hook_ParseODFContent_
// 441A5D above. Layout:
//   [DWORD skipped -- tag size][DWORD nCount]
//   nCount x { float flt_104, float flt_108, DWORD field_10C,
//              ASCIIZ wavName, ASCIIZ frameName }
// The array lands in pHanaT3F1->arroS118_14C (CHanaObj_S118 = 280 bytes each,
// allocated with +128 slack): each entry loads its wav through the 3D loader
// LoadWavToDS3DBuffer_42277E -- NOTE with no GetRealPath resolution, the name
// is used as-is (relative to the process cwd, which the game points at the
// install dir) -- and binds pFrame_114 by looking the SECOND name up in the
// frame tree (HanaFrame_FindByName). So a 3DON entry pins a looping 3D sound
// to a named frame of the object. Returns 1.
int __cdecl hook_ParseOdf3DON_44189C(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pHanaT3F1,
                                     CHanaFrameT3EC_S354 *pHanaRootT3EC_v1)
{
    LPBYTE pBuf, pWavName, pFrameName;      // one reused scan cursor [ebp-8h]
    unsigned int i;

    DebugLog("[HNH:SUB] ParseOdf3DON_44189C\n");

    pBuf = *ppBuf + 4;                      // skip the tag-size dword
    pHanaT3F1->nCount_148 = *(int *)pBuf;
    pBuf += 4;
    pHanaT3F1->arroS118_14C = (CHanaObj_S118 *)MemAlloc_40DEF7(
        280 * pHanaT3F1->nCount_148 + 128);

    for (i = 0; i < (unsigned int)pHanaT3F1->nCount_148; ++i)
    {
        pHanaT3F1->arroS118_14C[i].flt_104  = *(float *)pBuf;
        pBuf += 4;
        pHanaT3F1->arroS118_14C[i].flt_108  = *(float *)pBuf;
        pBuf += 4;
        pHanaT3F1->arroS118_14C[i].field_10C = *(int *)pBuf;
        pBuf += 4;

        pWavName = pBuf;                    // ASCIIZ wav name
        memcpy(pHanaT3F1->arroS118_14C[i].sz_name_4, pWavName,
               lstrlenA((LPCSTR)pWavName) + 1);
        pFrameName = pWavName + lstrlenA((LPCSTR)pWavName) + 1;   // ASCIIZ frame name

        pHanaT3F1->arroS118_14C[i].pSnd_0 =
            LoadWavToDS3DBuffer_42277E(pHanaT3F1->arroS118_14C[i].sz_name_4);
        pHanaT3F1->arroS118_14C[i].pFrame_114 =
            HanaFrame_FindByName((LPCSTR)pFrameName, pHanaRootT3EC_v1);

        pBuf = pFrameName + lstrlenA((LPCSTR)pFrameName) + 1;
    }
    *ppBuf = pBuf;
    return 1;
}

void __cdecl hook_ConfigureLight(
        CHanaLightT3EF_S1A0 *pHanaLight,
        float posX,
        float posY,
        float posZ,
        float dirX,
        float dirY,
        float dirZ)
{
  DebugLog("[HNH:SUB] ConfigureLight_409510\n");
  switch ( pHanaLight->d3dLight_134.dltType )
  {
    case D3DLIGHT_DIRECTIONAL:
      pHanaLight->d3dLight_134.dvDirection.x = -dirX;
      pHanaLight->d3dLight_134.dvDirection.y = -dirY;
      pHanaLight->d3dLight_134.dvDirection.z = dirZ;
      ApplyLightSettings(pHanaLight);
      break;
    case D3DLIGHT_POINT:
      pHanaLight->d3dLight_134.dvPosition.x = posX;
      pHanaLight->d3dLight_134.dvPosition.y = posY;
      pHanaLight->d3dLight_134.dvPosition.z = posZ;
      ApplyLightSettings(pHanaLight);
      break;
    case D3DLIGHT_SPOT:
      pHanaLight->d3dLight_134.dvDirection.x = -dirX;
      pHanaLight->d3dLight_134.dvDirection.y = -dirY;
      pHanaLight->d3dLight_134.dvDirection.z = dirZ;
      pHanaLight->d3dLight_134.dvPosition.x = posX;
      pHanaLight->d3dLight_134.dvPosition.y = posY;
      pHanaLight->d3dLight_134.dvPosition.z = posZ;
      ApplyLightSettings(pHanaLight);
      break;
  }
}

void __cdecl hook_AssignLigthtToFrame_40539D(CHanaFrameT3EC_S354 *pHanaT3EC, CHanaLightT3EF_S1A0 *pHanaLight)
{
  D3DXMATRIX mat_v3; // [esp+8h] [ebp-40h] BYREF
  DebugLog("[HNH:SUB] AssignLigthtToFrame_40539D\n");
  if ( pHanaLight )
  {
    pHanaT3EC->pHanaLight_210 = pHanaLight;
    pHanaLight->pHanaT3EC_19C = pHanaT3EC;
    pHanaT3EC->bHasLight_214 = 1;
    qmemcpy(&mat_v3, &pHanaT3EC->mat4x4_after, sizeof(mat_v3));
    ConfigureLight(pHanaT3EC->pHanaLight_210, mat_v3._41, mat_v3._42, mat_v3._43, mat_v3._13, mat_v3._23, mat_v3._33);
    CHanaObj__AddRef_40BFC3(pHanaLight);
  }
}

int __cdecl hook_CHanaObj__NewAnimT3F0(CHanaAnimT3F0_S240 **ppOut)
{
  CHanaAnimT3F0_S240 *pOut; // edi
  CHanaAnimKeyT3F0_S78 *arroS78_13C; // eax
  DebugLog("[HNH:SUB] CHanaObj__NewAnimT3F0_401CD0\n");
  pOut = (CHanaAnimT3F0_S240 *)MemAlloc_40DEF7(0x240u);
  memset(pOut, 0, sizeof(CHanaAnimT3F0_S240));
  pOut->dwType = 0x3F0;
  pOut->nCount_138 = 1;
  pOut->arroS78_13C = (CHanaAnimKeyT3F0_S78 *)MemAlloc_40DEF7(0x78u);
  pOut->arroS78_13C->b_34 = 1;
  arroS78_13C = pOut->arroS78_13C;
  arroS78_13C->quart_1C.z = 0.0;
  arroS78_13C->quart_1C.y = 0.0;
  arroS78_13C->quart_1C.x = 0.0;
  arroS78_13C->quart_1C.w = 1.0;
  pOut->arroS78_13C->quart_1C.w = pOut->arroS78_13C->quart_1C.w * -1.0;
  pOut->arroS78_13C->b_2C = 1;
  pOut->arroS78_13C->x_4 = 0.0;
  pOut->arroS78_13C->y_8 = 0.0;
  pOut->arroS78_13C->z_C = 0.0;
  pOut->arroS78_13C->b_30 = 1;
  pOut->arroS78_13C->sx_10 = 1.0;
  pOut->arroS78_13C->sy_14 = 1.0;
  pOut->arroS78_13C->sz_18 = 1.0;
  pOut->fResolution_134 = 0.0;
  CHanaObj__AddRef_40BFC3(pOut);
  *ppOut = pOut;
  pOut->objS78_14C.quart_1C = pOut->arroS78_13C->quart_1C;
  pOut->objS78_14C.x_4 = pOut->arroS78_13C->x_4;
  pOut->objS78_14C.y_8 = pOut->arroS78_13C->y_8;
  pOut->objS78_14C.z_C = pOut->arroS78_13C->z_C;
  pOut->objS78_14C.sx_10 = pOut->arroS78_13C->sx_10;
  pOut->objS78_14C.sy_14 = pOut->arroS78_13C->sy_14;
  pOut->objS78_14C.sz_18 = pOut->arroS78_13C->sz_18;
  return 0;
}

int __cdecl hook_HanaAnim_GetDuration(CHanaAnimT3F0_S240 *pHanaAnimT3F0)
{
  int result; // eax
  DebugLog("[HNH:SUB] hook_HanaAnim_GetDuration_004039C9\n");
  result = LODWORD(pHanaAnimT3F0->arroS78_13C[pHanaAnimT3F0->nCount_138 - 1].t_0);
  LODWORD(pHanaAnimT3F0->fResolution_134) = result;
  return result;
}

void __cdecl hook_CHanaAnimT3F0__BakeKeyframesT3F0_402D0E(CHanaAnimT3F0_S240 *pHanaAnimT3F0)
{
  float b; // [esp+0h] [ebp-50h]
  float v2; // [esp+Ch] [ebp-44h]
  float dt_v3; // [esp+10h] [ebp-40h]
  CHanaAnimKeyT3F0_S78 *pobjS78_v4; // [esp+14h] [ebp-3Ch]
  CHanaAnimKeyT3F0_S78 *pobjS78_v2; // [esp+14h] [ebp-3Ch]
  CHanaAnimKeyT3F0_S78 *pobjS78_v6; // [esp+14h] [ebp-3Ch]
  CHanaAnimKeyT3F0_S78 *arroS78_13C; // [esp+18h] [ebp-38h]
  CHanaAnimKeyT3F0_S78 *v8; // [esp+18h] [ebp-38h]
  unsigned int k; // [esp+1Ch] [ebp-34h]
  unsigned int ii; // [esp+1Ch] [ebp-34h]
  unsigned int mm; // [esp+1Ch] [ebp-34h]
  unsigned int i; // [esp+20h] [ebp-30h]
  unsigned int m; // [esp+20h] [ebp-30h]
  unsigned int jj; // [esp+20h] [ebp-30h]
  unsigned int nn; // [esp+20h] [ebp-30h]
  unsigned int j; // [esp+24h] [ebp-2Ch]
  unsigned int n; // [esp+24h] [ebp-2Ch]
  unsigned int kk; // [esp+24h] [ebp-2Ch]
  CHanaAnimKeyT3F0_S78 *pobjS78_v19; // [esp+28h] [ebp-28h]
  CHanaAnimKeyT3F0_S78 *pobjS78_v20; // [esp+28h] [ebp-28h]
  CHanaAnimKeyT3F0_S78 *pobjS78_v21; // [esp+28h] [ebp-28h]
  float sx_v22; // [esp+2Ch] [ebp-24h]
  float x_v23; // [esp+2Ch] [ebp-24h]
  float sy_v24; // [esp+30h] [ebp-20h]
  float y_v25; // [esp+30h] [ebp-20h]
  float sz_v26; // [esp+34h] [ebp-1Ch]
  float z_v27; // [esp+34h] [ebp-1Ch]
  float sx_v28; // [esp+38h] [ebp-18h]
  float x_v30; // [esp+38h] [ebp-18h]
  float sy_v30; // [esp+3Ch] [ebp-14h]
  float y_v32; // [esp+3Ch] [ebp-14h]
  float sz_v32; // [esp+40h] [ebp-10h]
  float z_v34; // [esp+40h] [ebp-10h]
  float sx_10; // [esp+44h] [ebp-Ch]
  float x_4; // [esp+44h] [ebp-Ch]
  float sy_14; // [esp+48h] [ebp-8h]
  float y_8; // [esp+48h] [ebp-8h]
  float sz_18; // [esp+4Ch] [ebp-4h]
  float z_C; // [esp+4Ch] [ebp-4h]
  DebugLog("[HNH:SUB] CHanaAnimT3F0__BakeKeyframesT3F0_402D0E\n");
  arroS78_13C = pHanaAnimT3F0->arroS78_13C;
  pobjS78_v19 = arroS78_13C;
  pobjS78_v4 = 0;
  for ( i = 0; i < pHanaAnimT3F0->nCount_138; ++i )
  {
    if ( arroS78_13C[i].b_34 == 1 )
    {
      pobjS78_v19 = &arroS78_13C[i];
      pobjS78_v4 = 0;
    }
    else
    {
      for ( j = i + 1; j < pHanaAnimT3F0->nCount_138; ++j )
      {
        if ( arroS78_13C[j].b_34 == 1 )
        {
          pobjS78_v4 = &arroS78_13C[j];
          break;
        }
      }
      if ( j >= pHanaAnimT3F0->nCount_138 )
        pobjS78_v4 = arroS78_13C;
      for ( k = i; k < j; ++k )
      {
        if ( pobjS78_v4->t_0 - pobjS78_v19->t_0 == 0.0 )
        {
          arroS78_13C[k].quart_1C = pobjS78_v19->quart_1C;
        }
        else
        {
          b = (arroS78_13C[k].t_0 - pobjS78_v19->t_0) / (pobjS78_v4->t_0 - pobjS78_v19->t_0);
          D3DXQuaternionSlerp(&arroS78_13C[k].quart_1C, &pobjS78_v19->quart_1C, &pobjS78_v4->quart_1C, b);
        }
      }
    }
  }
  pobjS78_v20 = arroS78_13C;
  pobjS78_v2 = 0;
  for ( m = 0; m < pHanaAnimT3F0->nCount_138; ++m )
  {
    if ( arroS78_13C[m].b_30 == 1 )
    {
      pobjS78_v20 = &arroS78_13C[m];
      pobjS78_v2 = 0;
    }
    else
    {
      for ( n = m + 1; n < pHanaAnimT3F0->nCount_138; ++n )
      {
        if ( arroS78_13C[n].b_30 == 1 )
        {
          pobjS78_v2 = &arroS78_13C[n];
          break;
        }
      }
      if ( n >= pHanaAnimT3F0->nCount_138 )
        pobjS78_v2 = arroS78_13C;
      sx_10 = pobjS78_v20->sx_10;
      sy_14 = pobjS78_v20->sy_14;
      sz_18 = pobjS78_v20->sz_18;
      sx_v22 = pobjS78_v2->sx_10;
      sy_v24 = pobjS78_v2->sy_14;
      sz_v26 = pobjS78_v2->sz_18;
      for ( ii = m; ii < n; ++ii )
      {
        if ( pobjS78_v2->t_0 - pobjS78_v20->t_0 == 0.0 )
        {
          arroS78_13C[ii].sx_10 = sx_10;
          arroS78_13C[ii].sy_14 = sy_14;
          arroS78_13C[ii].sz_18 = sz_18;
        }
        else
        {
          dt_v3 = (arroS78_13C[ii].t_0 - pobjS78_v20->t_0) / (pobjS78_v2->t_0 - pobjS78_v20->t_0);
          sx_v28 = (sx_v22 - sx_10) * dt_v3 + sx_10;
          sy_v30 = (sy_v24 - sy_14) * dt_v3 + sy_14;
          sz_v32 = (sz_v26 - sz_18) * dt_v3 + sz_18;
          arroS78_13C[ii].sx_10 = sx_v28;
          arroS78_13C[ii].sy_14 = sy_v30;
          arroS78_13C[ii].sz_18 = sz_v32;
        }
      }
    }
  }
  pobjS78_v21 = arroS78_13C;
  pobjS78_v6 = 0;
  for ( jj = 0; jj < pHanaAnimT3F0->nCount_138; ++jj )
  {
    if ( arroS78_13C[jj].b_2C == 1 )
    {
      pobjS78_v21 = &arroS78_13C[jj];
      pobjS78_v6 = 0;
    }
    else
    {
      for ( kk = jj + 1; kk < pHanaAnimT3F0->nCount_138; ++kk )
      {
        if ( arroS78_13C[kk].b_2C == 1 )
        {
          pobjS78_v6 = &arroS78_13C[kk];
          break;
        }
      }
      if ( kk >= pHanaAnimT3F0->nCount_138 )
        pobjS78_v6 = arroS78_13C;
      x_4 = pobjS78_v21->x_4;
      y_8 = pobjS78_v21->y_8;
      z_C = pobjS78_v21->z_C;
      x_v23 = pobjS78_v6->x_4;
      y_v25 = pobjS78_v6->y_8;
      z_v27 = pobjS78_v6->z_C;
      for ( mm = jj; mm < kk; ++mm )
      {
        if ( pobjS78_v6->t_0 - pobjS78_v21->t_0 == 0.0 )
        {
          arroS78_13C[mm].x_4 = x_4;
          arroS78_13C[mm].y_8 = y_8;
          arroS78_13C[mm].z_C = z_C;
        }
        else
        {
          v2 = (arroS78_13C[mm].t_0 - pobjS78_v21->t_0) / (pobjS78_v6->t_0 - pobjS78_v21->t_0);
          x_v30 = (x_v23 - x_4) * v2 + x_4;
          y_v32 = (y_v25 - y_8) * v2 + y_8;
          z_v34 = (z_v27 - z_C) * v2 + z_C;
          arroS78_13C[mm].x_4 = x_v30;
          arroS78_13C[mm].y_8 = y_v32;
          arroS78_13C[mm].z_C = z_v34;
        }
      }
    }
  }
  v8 = pHanaAnimT3F0->arroS78_13C;
  for ( nn = 0; nn < pHanaAnimT3F0->nCount_138; ++nn )
    ApplyQTS_4035B1(
      &v8[nn].mat4x4_38,
      &v8[nn].quart_1C,
      v8[nn].x_4,
      v8[nn].y_8,
      v8[nn].z_C,
      v8[nn].sx_10,
      v8[nn].sy_14,
      v8[nn].sz_18);
  memcpy(&pHanaAnimT3F0->objS78_1C4, &pHanaAnimT3F0->objS78_14C, sizeof(pHanaAnimT3F0->objS78_1C4));
  pHanaAnimT3F0->objS78_14C.t_0 = 0.0;
  pHanaAnimT3F0->objS78_1C4.t_0 = 0.0;
  pHanaAnimT3F0->objS78_14C.x_4 = v8->x_4;
  pHanaAnimT3F0->objS78_1C4.x_4 = pHanaAnimT3F0->objS78_14C.x_4;
  pHanaAnimT3F0->objS78_14C.y_8 = v8->y_8;
  pHanaAnimT3F0->objS78_1C4.y_8 = pHanaAnimT3F0->objS78_14C.y_8;
  pHanaAnimT3F0->objS78_14C.z_C = v8->z_C;
  pHanaAnimT3F0->objS78_1C4.z_C = pHanaAnimT3F0->objS78_14C.z_C;
  pHanaAnimT3F0->objS78_14C.sx_10 = v8->sx_10;
  pHanaAnimT3F0->objS78_1C4.sx_10 = pHanaAnimT3F0->objS78_14C.sx_10;
  pHanaAnimT3F0->objS78_14C.sy_14 = v8->sy_14;
  pHanaAnimT3F0->objS78_1C4.sy_14 = pHanaAnimT3F0->objS78_14C.sy_14;
  pHanaAnimT3F0->objS78_14C.sz_18 = v8->sz_18;
  pHanaAnimT3F0->objS78_1C4.sz_18 = pHanaAnimT3F0->objS78_14C.sz_18;
  pHanaAnimT3F0->objS78_14C.quart_1C = v8->quart_1C;
  pHanaAnimT3F0->objS78_1C4.quart_1C = pHanaAnimT3F0->objS78_14C.quart_1C;
  qmemcpy(&pHanaAnimT3F0->objS78_14C.mat4x4_38, &v8->mat4x4_38, sizeof(pHanaAnimT3F0->objS78_14C.mat4x4_38));
  qmemcpy(
    &pHanaAnimT3F0->objS78_1C4.mat4x4_38,
    &pHanaAnimT3F0->objS78_14C.mat4x4_38,
    sizeof(pHanaAnimT3F0->objS78_1C4.mat4x4_38));
}

void __cdecl hook_HanaFrame_GetFrameName(CHanaFrameT3EC_S354 *pFrame, char *pOutName)
{
  DebugLog("[HNH:SUB] HanaFrame_GetFrameName_40BA89\n");
  if ( pFrame )
  {
    if ( pOutName )
      strcpy(pOutName, pFrame->szName_8);
  }
}

void __cdecl hook_HanaAnim_LinkFrame(CHanaAnimT3F0_S240 *pAnim, CHanaFrameT3EC_S354 *pFrame)
{
  DebugLog("[HNH:SUB] HanaAnim_LinkFrame_401FBD\n");
  if ( pFrame )
  {
    pAnim->pHanaFram_130 = pFrame;
    pFrame->pHanaAnim_2C0 = pAnim;
  }
}

void __cdecl hook_ApplyQTS_4035B1(
        D3DXMATRIX *pOut,
        D3DXQUATERNION *pQ,
        float x,
        float y,
        float z,
        float sx,
        float sy,
        float sz)
{
  D3DXMATRIX pM1; // [esp+0h] [ebp-C0h] BYREF
  D3DXMATRIX pM2; // [esp+40h] [ebp-80h] BYREF
  D3DXMATRIX pM3; // [esp+80h] [ebp-40h] BYREF
  DebugLog("[HNH:SUB] ApplyQTS_4035B1\n");
  D3DXMatrixRotationQuaternion(&pM2, pQ);
  D3DXMatrixTranslation(&pM3, x, y, z);
  D3DXMatrixScaling(&pM1, sx, sy, sz);
  D3DXMatrixMultiply(pOut, &pM1, &pM2);
  D3DXMatrixMultiply(pOut, pOut, &pM3);
}

bool __cdecl hook_CHanaAnimSetT3F1__AddAnimT3F0_403C15(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)
{
  CHanaAnimT3F0_S240 **pOldAnimRefs; // [esp+14h] [ebp-8h]
  unsigned int i; // [esp+18h] [ebp-4h]
  DebugLog("[HNH:SUB] CHanaAnimSetT3F1__AddAnimT3F0_403C15\n");
  for ( i = 0; i < pAnimSet->nAnimCount_134; ++i )// 1. Duplicate check ? O(n) linear scan
  {
    if ( pAnimSet->refsAnimT3F0_138[i] == pAnim )
      return 0;                                 // <- already present
  }
  ++pAnimSet->nAnimCount_134;                   // 2. Grow count
  if ( pAnimSet->refsAnimT3F0_138 )             // 3. (Re)allocate the pointer array
                                                //    existing array → realloc+copy
  {
    pOldAnimRefs = pAnimSet->refsAnimT3F0_138;
    pAnimSet->refsAnimT3F0_138 = (CHanaAnimT3F0_S240 **)MemAlloc_40DEF7(4 * pAnimSet->nAnimCount_134);
    qmemcpy(pAnimSet->refsAnimT3F0_138, pOldAnimRefs, 4 * ((unsigned int)(4 * pAnimSet->nAnimCount_134 - 4) >> 2));// copy old (count-1) entries
    MemFree_40DF64(pOldAnimRefs);
  }
  else                                          // first entry → alloc 4 bytes
  {
    pAnimSet->refsAnimT3F0_138 = (CHanaAnimT3F0_S240 **)MemAlloc_40DEF7(4u);
  }
  pAnimSet->refsAnimT3F0_138[pAnimSet->nAnimCount_134 - 1] = pAnim;// 4. Append the new anim at the tail
  CHanaObj__AddRef_40BFC3(pAnim);               //  5. Take ownership: refcount++ and bake keyframes
  CHanaAnimT3F0__BakeKeyframesT3F0_402D0E(pAnim);// slerp/lerp fill + build matrices
  if ( pAnimSet->fRes_13C < (double)pAnim->fResolution_134 )// 6. Track maximum animation resolution across the set
    pAnimSet->fRes_13C = pAnim->fResolution_134;
  return 1;
}

// HanaFrame_GetCenterPosition - returns pFrame's vertCenter_2B4 expressed in pRelFrame's space. If pRelFrame is g_pSceneFrame: out = Translate(center)*mat4x4_after. Otherwise also premultiplies by the inverse of pRelFrame->mat4x4_after. Writes x/y/z into px/py/pz; zeroes them when pFrame is NULL.
void __cdecl hook_HanaFrame_GetCenterPosition_408C3E(
        CHanaFrameRootT3EC_S2D4 *pFrame,
        CHanaFrameRootT3EC_S2D4 *pRelFrame,
        float *pOutX,
        float *pOutY,
        float *pOutZ)
{
  D3DXMATRIX pOut; // [esp+0h] [ebp-80h] BYREF
  D3DXMATRIX pM1; // [esp+40h] [ebp-40h] BYREF
  DebugLog("[HNH:SUB] HanaFrame_GetCenterPosition_408C3E\n");
  if ( pFrame )
  {
    if ( pRelFrame == g_pSceneFrame )
    {
      D3DXMatrixTranslation(&pOut, pFrame->vertCenter_2B4.x, pFrame->vertCenter_2B4.y, pFrame->vertCenter_2B4.z);
      D3DXMatrixMultiply(&pOut, &pOut, &pFrame->mat4x4_after);
    }
    else
    {
      D3DXMatrixTranslation(&pOut, pFrame->vertCenter_2B4.x, pFrame->vertCenter_2B4.y, pFrame->vertCenter_2B4.z);
      D3DXMatrixMultiply(&pM1, &pOut, &pFrame->mat4x4_after);
      D3DXMatrixInverse(&pOut, 0, &pRelFrame->mat4x4_after);
      D3DXMatrixMultiply(&pOut, &pM1, &pOut);
    }
    *pOutX = pOut._41;
    *pOutY = pOut._42;
    *pOutZ = pOut._43;
  }
  else
  {
    *pOutZ = 0.0;
    *pOutY = 0.0;
    *pOutX = 0.0;
  }
}

void register_ParseODFContent_441A5D() {
    // [OK] hnh_Hook(0x441A5D, hook_ParseODFContent_441A5D, "ParseODFContent_441A5D");
    hnh_Hook(0x44106A, hook_ParseOdfFRAM_44106A, "ParseOdfFRAM_44106A"); // [OK]
    hnh_Hook(0x4414EB, hook_ParseOdfANIM_4414EB, "ParseOdfANIM_4414EB"); // [OK]
    hnh_Hook(0x44068C, hook_ParseOdfMESH_44068C, "ParseOdfMESH_44068C"); // [OK]
    hnh_Hook(0x440896, hook_ParseOdfMORP_440896, "ParseOdfMORP_440896"); // [OK]
    hnh_Hook(0x44189C, hook_ParseOdf3DON_44189C, "ParseOdf3DON_44189C"); // [OK]
    hnh_Hook(0x409510, hook_ConfigureLight,"ConfigureLight_409510");
    hnh_Hook(0x40539D, hook_AssignLigthtToFrame_40539D, "AssignLigthtToFrame_40539D");
    hnh_Hook(0x401CD0, hook_CHanaObj__NewAnimT3F0,"CHanaObj__NewAnimT3F0_401CD0");
    hnh_Hook(0x4039C9, hook_HanaAnim_GetDuration, "HanaAnim_GetDuration_004039C9");
    hnh_Hook(0x402D0E, hook_CHanaAnimT3F0__BakeKeyframesT3F0_402D0E, "CHanaAnimT3F0__BakeKeyframesT3F0_402D0E");
    hnh_Hook(0x40BA89, hook_HanaFrame_GetFrameName, "HanaFrame_GetFrameName_40BA89");
    hnh_Hook(0x401FBD, hook_HanaAnim_LinkFrame, "HanaAnim_LinkFrame_401FBD");
    hnh_Hook(0x4035B1, hook_ApplyQTS_4035B1, "ApplyQTS_4035B1");
    hnh_Hook(0x403C15, hook_CHanaAnimSetT3F1__AddAnimT3F0_403C15, "CHanaAnimSetT3F1__AddAnimT3F0_403C15");
    hnh_Hook(0x408C3E, hook_HanaFrame_GetCenterPosition_408C3E, "HanaFrame_GetCenterPosition_408C3E");
}
