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
void (__cdecl*CalcFrameBound_404B80)(CHanaFrameT3EC_S354 *pFrameT3EC, CHanaMorpT457_S40C *pMorpT3F5) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaMorpT457_S40C *)>(0x404B80);
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
BOOL (__cdecl * GetSoundBufferAverageLevel)(LPDIRECTSOUNDBUFFER pDSBuffer, float *pfPlayPos) = reinterpret_cast<BOOL(__cdecl*)(LPDIRECTSOUNDBUFFER pDSBuffer, float *pfPlayPos)>(0x4248F2);

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
//   FFLG  [8-byte hdr][WORD flags]      bit0 -> bUpdateBone_218, bit1 -> bAlphaDeferred_130,
//                                       bit2 -> bDisableFog_134, bit3 -> bFlagFFLG3_21C
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
    WORD wFrameFlags;                             // [esp+8h] [ebp-198h]

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
            wFrameFlags = *(WORD *)pScan;
            if (wFrameFlags & 1)
                pFrame->bUpdateBone_218 = 1;        // FFLG bit0: bone transform
            if (wFrameFlags & 2)
                pFrame->bAlphaDeferred_130 = 1;   // FFLG bit1: alpha-deferred render
            if (wFrameFlags & 4)
                pFrame->bDisableFog_134 = 1;        // FFLG bit2: fog off for this frame
            if (wFrameFlags & 8)
                pFrame->bFlagFFLG3_21C = 1;          // FFLG bit3: meaning unknown
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
// 120-byte CHanaAnimKeyT3F0_S78 key (t, x/y/z, sx/sy/sz, quat, bKeyPos/b_30/bKeyRot = 1,
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
  MemFree_40DF64(pAnimT3F0->arrKeys_13C);  // drop the default key array
  pAnimT3F0->arrKeys_13C = (CHanaAnimKeyT3F0_S78 *)MemAlloc_40DEF7(120 * (nCount + 1)); // +1 sentinel
  pAnimT3F0->fDuration_134 = 100000000.0f;
  pAnimT3F0->nKeyCount_138 = nCount;

  for (i = 0; i < nCount; ++i)
  {
    CHanaSubObj_S38 *pKey = (CHanaSubObj_S38 *)pBuf;   // 56-byte on-disk key
    CHanaAnimKeyT3F0_S78 *pOutKey = &pAnimT3F0->arrKeys_13C[i];

    pOutKey->fTime = pKey->t;
    pOutKey->quatRot = pKey->quat;
    pOutKey->fScaleX = pKey->scale.x;
    pOutKey->fScaleY = pKey->scale.y;
    pOutKey->fScaleZ = pKey->scale.z;
    pOutKey->fPosX = pKey->pos.x;
    pOutKey->fPosY = pKey->pos.y;
    pOutKey->fPosZ = pKey->pos.z;
    pOutKey->bKeyPos = 1;
    pOutKey->bKeyScale = 1;
    pOutKey->bKeyRot = 1;
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
        pMorpT457->arrMKeyT3EA_34C[idx] = (CHanaMKey_S8*)MemAlloc_40DEF7(
          8 * pMorpT457->arrMKeyT3EASize_348[idx] + 80);
        for (k = 0; k < (unsigned int)pMorpT457->arrMKeyT3EASize_348[idx]; ++k)
        {
          pMorpT457->arrMKeyT3EA_34C[idx][k].nParam = *(int *)pBuf;
          pBuf += 4;
          pMorpT457->arrMKeyT3EA_34C[idx][k].nVtxIdx = *(int *)pBuf;
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
    pMorpT457->nEaseFrom_35C    = *(int *)pBuf;    pBuf += 4;
    pMorpT457->nEaseTo_360      = *(int *)pBuf;    pBuf += 4;
    pMorpT457->nTimeState_36C   = *(int *)pBuf;    pBuf += 4;
    pMorpT457->nTimeFrom_370    = *(int *)pBuf;    pBuf += 4;
    pMorpT457->nTimeTo_374      = *(int *)pBuf;    pBuf += 4;
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
  static bool _debugged = true;
  if (_debugged) {
    _debugged = false;
    DebugLog("[HNH:SUB] ConfigureLight_409510\n");
  }

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
  CHanaAnimKeyT3F0_S78 *arrKeys_13C; // eax
  DebugLog("[HNH:SUB] CHanaObj__NewAnimT3F0_401CD0\n");
  pOut = (CHanaAnimT3F0_S240 *)MemAlloc_40DEF7(0x240u);
  memset(pOut, 0, sizeof(CHanaAnimT3F0_S240));
  pOut->dwType = 0x3F0;
  pOut->nKeyCount_138 = 1;
  pOut->arrKeys_13C = (CHanaAnimKeyT3F0_S78 *)MemAlloc_40DEF7(0x78u);
  pOut->arrKeys_13C->bKeyRot = 1;
  arrKeys_13C = pOut->arrKeys_13C;
  arrKeys_13C->quatRot.z = 0.0;
  arrKeys_13C->quatRot.y = 0.0;
  arrKeys_13C->quatRot.x = 0.0;
  arrKeys_13C->quatRot.w = 1.0;
  pOut->arrKeys_13C->quatRot.w = pOut->arrKeys_13C->quatRot.w * -1.0;
  pOut->arrKeys_13C->bKeyPos = 1;
  pOut->arrKeys_13C->fPosX = 0.0;
  pOut->arrKeys_13C->fPosY = 0.0;
  pOut->arrKeys_13C->fPosZ = 0.0;
  pOut->arrKeys_13C->bKeyScale = 1;
  pOut->arrKeys_13C->fScaleX = 1.0;
  pOut->arrKeys_13C->fScaleY = 1.0;
  pOut->arrKeys_13C->fScaleZ = 1.0;
  pOut->fDuration_134 = 0.0;
  CHanaObj__AddRef_40BFC3(pOut);
  *ppOut = pOut;
  pOut->keyCur_14C.quatRot = pOut->arrKeys_13C->quatRot;
  pOut->keyCur_14C.fPosX = pOut->arrKeys_13C->fPosX;
  pOut->keyCur_14C.fPosY = pOut->arrKeys_13C->fPosY;
  pOut->keyCur_14C.fPosZ = pOut->arrKeys_13C->fPosZ;
  pOut->keyCur_14C.fScaleX = pOut->arrKeys_13C->fScaleX;
  pOut->keyCur_14C.fScaleY = pOut->arrKeys_13C->fScaleY;
  pOut->keyCur_14C.fScaleZ = pOut->arrKeys_13C->fScaleZ;
  return 0;
}

int __cdecl hook_HanaAnim_GetDuration(CHanaAnimT3F0_S240 *pHanaAnimT3F0)
{
  int result; // eax
  DebugLog("[HNH:SUB] HanaAnim_GetDuration_004039C9\n");
  result = LODWORD(pHanaAnimT3F0->arrKeys_13C[pHanaAnimT3F0->nKeyCount_138 - 1].fTime);
  LODWORD(pHanaAnimT3F0->fDuration_134) = result;
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
  CHanaAnimKeyT3F0_S78 *arrKeys_13C; // [esp+18h] [ebp-38h]
  CHanaAnimKeyT3F0_S78 *pKeyFirst; // [esp+18h] [ebp-38h]
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
  float fScaleX; // [esp+44h] [ebp-Ch]
  float fPosX; // [esp+44h] [ebp-Ch]
  float fScaleY; // [esp+48h] [ebp-8h]
  float fPosY; // [esp+48h] [ebp-8h]
  float fScaleZ; // [esp+4Ch] [ebp-4h]
  float fPosZ; // [esp+4Ch] [ebp-4h]
  DebugLog("[HNH:SUB] CHanaAnimT3F0__BakeKeyframesT3F0_402D0E\n");
  arrKeys_13C = pHanaAnimT3F0->arrKeys_13C;
  pobjS78_v19 = arrKeys_13C;
  pobjS78_v4 = 0;
  for ( i = 0; i < pHanaAnimT3F0->nKeyCount_138; ++i )
  {
    if ( arrKeys_13C[i].bKeyRot == 1 )
    {
      pobjS78_v19 = &arrKeys_13C[i];
      pobjS78_v4 = 0;
    }
    else
    {
      for ( j = i + 1; j < pHanaAnimT3F0->nKeyCount_138; ++j )
      {
        if ( arrKeys_13C[j].bKeyRot == 1 )
        {
          pobjS78_v4 = &arrKeys_13C[j];
          break;
        }
      }
      if ( j >= pHanaAnimT3F0->nKeyCount_138 )
        pobjS78_v4 = arrKeys_13C;
      for ( k = i; k < j; ++k )
      {
        if ( pobjS78_v4->fTime - pobjS78_v19->fTime == 0.0 )
        {
          arrKeys_13C[k].quatRot = pobjS78_v19->quatRot;
        }
        else
        {
          b = (arrKeys_13C[k].fTime - pobjS78_v19->fTime) / (pobjS78_v4->fTime - pobjS78_v19->fTime);
          D3DXQuaternionSlerp(&arrKeys_13C[k].quatRot, &pobjS78_v19->quatRot, &pobjS78_v4->quatRot, b);
        }
      }
    }
  }
  pobjS78_v20 = arrKeys_13C;
  pobjS78_v2 = 0;
  for ( m = 0; m < pHanaAnimT3F0->nKeyCount_138; ++m )
  {
    if ( arrKeys_13C[m].bKeyScale == 1 )
    {
      pobjS78_v20 = &arrKeys_13C[m];
      pobjS78_v2 = 0;
    }
    else
    {
      for ( n = m + 1; n < pHanaAnimT3F0->nKeyCount_138; ++n )
      {
        if ( arrKeys_13C[n].bKeyScale == 1 )
        {
          pobjS78_v2 = &arrKeys_13C[n];
          break;
        }
      }
      if ( n >= pHanaAnimT3F0->nKeyCount_138 )
        pobjS78_v2 = arrKeys_13C;
      fScaleX = pobjS78_v20->fScaleX;
      fScaleY = pobjS78_v20->fScaleY;
      fScaleZ = pobjS78_v20->fScaleZ;
      sx_v22 = pobjS78_v2->fScaleX;
      sy_v24 = pobjS78_v2->fScaleY;
      sz_v26 = pobjS78_v2->fScaleZ;
      for ( ii = m; ii < n; ++ii )
      {
        if ( pobjS78_v2->fTime - pobjS78_v20->fTime == 0.0 )
        {
          arrKeys_13C[ii].fScaleX = fScaleX;
          arrKeys_13C[ii].fScaleY = fScaleY;
          arrKeys_13C[ii].fScaleZ = fScaleZ;
        }
        else
        {
          dt_v3 = (arrKeys_13C[ii].fTime - pobjS78_v20->fTime) / (pobjS78_v2->fTime - pobjS78_v20->fTime);
          sx_v28 = (sx_v22 - fScaleX) * dt_v3 + fScaleX;
          sy_v30 = (sy_v24 - fScaleY) * dt_v3 + fScaleY;
          sz_v32 = (sz_v26 - fScaleZ) * dt_v3 + fScaleZ;
          arrKeys_13C[ii].fScaleX = sx_v28;
          arrKeys_13C[ii].fScaleY = sy_v30;
          arrKeys_13C[ii].fScaleZ = sz_v32;
        }
      }
    }
  }
  pobjS78_v21 = arrKeys_13C;
  pobjS78_v6 = 0;
  for ( jj = 0; jj < pHanaAnimT3F0->nKeyCount_138; ++jj )
  {
    if ( arrKeys_13C[jj].bKeyPos == 1 )
    {
      pobjS78_v21 = &arrKeys_13C[jj];
      pobjS78_v6 = 0;
    }
    else
    {
      for ( kk = jj + 1; kk < pHanaAnimT3F0->nKeyCount_138; ++kk )
      {
        if ( arrKeys_13C[kk].bKeyPos == 1 )
        {
          pobjS78_v6 = &arrKeys_13C[kk];
          break;
        }
      }
      if ( kk >= pHanaAnimT3F0->nKeyCount_138 )
        pobjS78_v6 = arrKeys_13C;
      fPosX = pobjS78_v21->fPosX;
      fPosY = pobjS78_v21->fPosY;
      fPosZ = pobjS78_v21->fPosZ;
      x_v23 = pobjS78_v6->fPosX;
      y_v25 = pobjS78_v6->fPosY;
      z_v27 = pobjS78_v6->fPosZ;
      for ( mm = jj; mm < kk; ++mm )
      {
        if ( pobjS78_v6->fTime - pobjS78_v21->fTime == 0.0 )
        {
          arrKeys_13C[mm].fPosX = fPosX;
          arrKeys_13C[mm].fPosY = fPosY;
          arrKeys_13C[mm].fPosZ = fPosZ;
        }
        else
        {
          v2 = (arrKeys_13C[mm].fTime - pobjS78_v21->fTime) / (pobjS78_v6->fTime - pobjS78_v21->fTime);
          x_v30 = (x_v23 - fPosX) * v2 + fPosX;
          y_v32 = (y_v25 - fPosY) * v2 + fPosY;
          z_v34 = (z_v27 - fPosZ) * v2 + fPosZ;
          arrKeys_13C[mm].fPosX = x_v30;
          arrKeys_13C[mm].fPosY = y_v32;
          arrKeys_13C[mm].fPosZ = z_v34;
        }
      }
    }
  }
  pKeyFirst = pHanaAnimT3F0->arrKeys_13C;
  for ( nn = 0; nn < pHanaAnimT3F0->nKeyCount_138; ++nn )
    ApplyQTS_4035B1(
      &pKeyFirst[nn].matLocal,
      &pKeyFirst[nn].quatRot,
      pKeyFirst[nn].fPosX,
      pKeyFirst[nn].fPosY,
      pKeyFirst[nn].fPosZ,
      pKeyFirst[nn].fScaleX,
      pKeyFirst[nn].fScaleY,
      pKeyFirst[nn].fScaleZ);
  memcpy(&pHanaAnimT3F0->keyPrev_1C4, &pHanaAnimT3F0->keyCur_14C, sizeof(pHanaAnimT3F0->keyPrev_1C4));
  pHanaAnimT3F0->keyCur_14C.fTime = 0.0;
  pHanaAnimT3F0->keyPrev_1C4.fTime = 0.0;
  pHanaAnimT3F0->keyCur_14C.fPosX = pKeyFirst->fPosX;
  pHanaAnimT3F0->keyPrev_1C4.fPosX = pHanaAnimT3F0->keyCur_14C.fPosX;
  pHanaAnimT3F0->keyCur_14C.fPosY = pKeyFirst->fPosY;
  pHanaAnimT3F0->keyPrev_1C4.fPosY = pHanaAnimT3F0->keyCur_14C.fPosY;
  pHanaAnimT3F0->keyCur_14C.fPosZ = pKeyFirst->fPosZ;
  pHanaAnimT3F0->keyPrev_1C4.fPosZ = pHanaAnimT3F0->keyCur_14C.fPosZ;
  pHanaAnimT3F0->keyCur_14C.fScaleX = pKeyFirst->fScaleX;
  pHanaAnimT3F0->keyPrev_1C4.fScaleX = pHanaAnimT3F0->keyCur_14C.fScaleX;
  pHanaAnimT3F0->keyCur_14C.fScaleY = pKeyFirst->fScaleY;
  pHanaAnimT3F0->keyPrev_1C4.fScaleY = pHanaAnimT3F0->keyCur_14C.fScaleY;
  pHanaAnimT3F0->keyCur_14C.fScaleZ = pKeyFirst->fScaleZ;
  pHanaAnimT3F0->keyPrev_1C4.fScaleZ = pHanaAnimT3F0->keyCur_14C.fScaleZ;
  pHanaAnimT3F0->keyCur_14C.quatRot = pKeyFirst->quatRot;
  pHanaAnimT3F0->keyPrev_1C4.quatRot = pHanaAnimT3F0->keyCur_14C.quatRot;
  qmemcpy(&pHanaAnimT3F0->keyCur_14C.matLocal, &pKeyFirst->matLocal, sizeof(pHanaAnimT3F0->keyCur_14C.matLocal));
  qmemcpy(
    &pHanaAnimT3F0->keyPrev_1C4.matLocal,
    &pHanaAnimT3F0->keyCur_14C.matLocal,
    sizeof(pHanaAnimT3F0->keyPrev_1C4.matLocal));
}

void __cdecl hook_HanaFrame_GetFrameName(CHanaFrameT3EC_S354 *pFrame, char *pOutName)
{
  static bool _debugged = true;
  if (_debugged) {
    _debugged = false;
    DebugLog("[HNH:SUB] HanaFrame_GetFrameName_40BA89\n");
  }

  if ( pFrame )
  {
    if ( pOutName )
      strcpy(pOutName, pFrame->szName_8);
  }
}

void __cdecl hook_HanaAnim_LinkFrame(CHanaAnimT3F0_S240 *pAnim, CHanaFrameT3EC_S354 *pFrame)
{
  static bool _debugged = true;
  if (_debugged) {
    _debugged = false;
    DebugLog("[HNH:SUB] HanaAnim_LinkFrame_401FBD\n");
  }

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

  static bool _debugged = true;
  if (_debugged) {
    _debugged = false;
    DebugLog("[HNH:SUB] ApplyQTS_4035B1\n");
  }

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
  if ( pAnimSet->fRes_13C < (double)pAnim->fDuration_134 )// 6. Track maximum animation resolution across the set
    pAnimSet->fRes_13C = pAnim->fDuration_134;
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

  static bool _debugged = true;
  if (_debugged) {
    _debugged = false;
    DebugLog("[HNH:SUB] HanaFrame_GetCenterPosition_408C3E\n");
  }

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

BOOL __cdecl hook_GetSoundBufferAverageLevel(LPDIRECTSOUNDBUFFER pDSBuffer, float *pfPlayPos)
{
  double pSample; // st7
  int wBitsPerSample; // [esp+Ch] [ebp-5Ch]
  __int16 *ppvAudioPtr2; // [esp+10h] [ebp-58h] BYREF
  int i; // [esp+14h] [ebp-54h]
  WAVEFORMATEX waveFormat; // [esp+18h] [ebp-50h] BYREF
  void *pvAudioPtr1; // [esp+2Ch] [ebp-3Ch]
  int nChannels; // [esp+30h] [ebp-38h]
  signed int pdwAudioBytes1; // [esp+34h] [ebp-34h] BYREF
  signed int pdwAudioBytes2; // [esp+38h] [ebp-30h] BYREF
  DSBCAPS dsBufCaps; // [esp+3Ch] [ebp-2Ch] BYREF
  DWORD curPlayCursor; // [esp+50h] [ebp-18h] BYREF
  DWORD v15; // [esp+54h] [ebp-14h]
  __int16 *ppvAudioPtr1; // [esp+58h] [ebp-10h] BYREF
  void *pvAudioPtr2; // [esp+5Ch] [ebp-Ch]
  int totalSampleLevels; // [esp+60h] [ebp-8h]
  DWORD sampleBlokSize; // [esp+64h] [ebp-4h]

  DebugLog("[HNH:SUB] GetSoundBufferAverageLevel_4248F2\n");

  totalSampleLevels = 0;
  if ( !g_bDSInited_5185C4 )
    return 0;
  *pfPlayPos = 0.0;
  if ( !pDSBuffer )
    return 0;
  if ( pDSBuffer->GetStatus(&g_dwDSStatus_5185C8) != S_OK || (g_dwDSStatus_5185C8 & 1) == 0 )
    return 0;
  memset(&waveFormat, 0, sizeof(waveFormat));
  pDSBuffer->GetFormat(&waveFormat, 18, 0);
  pDSBuffer->GetFormat(&waveFormat, 18, 0);
  wBitsPerSample = waveFormat.wBitsPerSample;
  v15 = waveFormat.nSamplesPerSec / 1000;
  nChannels = waveFormat.nChannels;
  sampleBlokSize = waveFormat.nChannels * waveFormat.wBitsPerSample / 8 * (waveFormat.nSamplesPerSec / 1000);
  dsBufCaps.dwSize = 20;
  pDSBuffer->GetCaps(&dsBufCaps);
  pDSBuffer->GetCurrentPosition(&curPlayCursor, 0);
  curPlayCursor -= sampleBlokSize >> 1;
  if ( (curPlayCursor & 0x80000000) != 0 )
    curPlayCursor = 0;
  if ( curPlayCursor > dsBufCaps.dwBufferBytes - sampleBlokSize )
    return 0;
  if ( pDSBuffer->Lock(
    curPlayCursor,
    sampleBlokSize,
    (LPVOID *)&ppvAudioPtr1,
    (LPDWORD)&pdwAudioBytes1,
    (LPVOID *)&ppvAudioPtr2,
    (LPDWORD)&pdwAudioBytes2,
    DSBLOCK_FROMWRITECURSOR) )
  {
    return 0;
  }
  totalSampleLevels = 0;
  pvAudioPtr1 = ppvAudioPtr1;
  pvAudioPtr2 = ppvAudioPtr2;
  if ( wBitsPerSample / 8 == 1 )                // 8-bit audio
  {
    for ( i = 0; i < pdwAudioBytes1; ++i )
    {
      totalSampleLevels += abs(*(char *)ppvAudioPtr1);
      ppvAudioPtr1 = (__int16 *)((char *)ppvAudioPtr1 + 1);
    }
    for ( i = 0; i < pdwAudioBytes2; ++i )
    {
      totalSampleLevels += abs(*(char *)ppvAudioPtr2);
      ppvAudioPtr2 = (__int16 *)((char *)ppvAudioPtr2 + 1);
    }
  }
  else                                          // 16-bit audio
  {
    pdwAudioBytes1 = (unsigned int)pdwAudioBytes1 >> 1;
    pdwAudioBytes2 = (unsigned int)pdwAudioBytes2 >> 1;
    for ( i = 0; i < pdwAudioBytes1; ++i )
      totalSampleLevels += abs(*ppvAudioPtr1++);
    for ( i = 0; i < pdwAudioBytes2; ++i )
      totalSampleLevels += abs(*ppvAudioPtr2++);
  }
  pDSBuffer->Unlock(pvAudioPtr1, 0, pvAudioPtr2, 0);
  *pfPlayPos = (double)totalSampleLevels / (double)(unsigned int)pdwAudioBytes1;
  if ( wBitsPerSample / 8 == 1 )
    pSample = *pfPlayPos / 128.0;
  else
    pSample = *pfPlayPos / 32767.0;
  *pfPlayPos = pSample;
  return 1;
}

void __cdecl hook_CalcFrameBound_404B80(CHanaFrameT3EC_S354 *pFrame, CHanaMorpT457_S40C *pVertOrMeshOrMorp)
{
  D3DVERTEX *pData_1; // [esp+8h] [ebp-4Ch] BYREF
  DWORD numVertices; // [esp+Ch] [ebp-48h] BYREF
  unsigned int m; // [esp+10h] [ebp-44h]
  CHanaVertT3EA_S1FC *pVert; // [esp+14h] [ebp-40h]
  CHanaMorpT457_S40C *pMorp; // [esp+18h] [ebp-3Ch]
  int k; // [esp+1Ch] [ebp-38h]
  unsigned int dwNumChild; // [esp+20h] [ebp-34h]
  CHanaMeshT3F5_S138 *pMesh; // [esp+24h] [ebp-30h]
  D3DVERTEX *ppData; // [esp+28h] [ebp-2Ch] BYREF
  DWORD dwNumVertices_v2; // [esp+2Ch] [ebp-28h] BYREF
  unsigned int j; // [esp+30h] [ebp-24h]
  CHanaVertT3EA_S1FC *pVert_v2; // [esp+34h] [ebp-20h]
  int nIndex; // [esp+38h] [ebp-1Ch]
  unsigned int dwCount_v16; // [esp+3Ch] [ebp-18h]
  CHanaMeshT3F5_S138 *pMesh_v2; // [esp+40h] [ebp-14h]
  D3DVERTEX *pData; // [esp+44h] [ebp-10h] BYREF
  DWORD dwNumVertices; // [esp+48h] [ebp-Ch] BYREF
  unsigned int i; // [esp+4Ch] [ebp-8h]
  CHanaVertT3EA_S1FC *pVert_v3; // [esp+50h] [ebp-4h]

  ONCELOG("[HNH:SUB] CalcFrameBound_404B80\n");

  if ( pFrame->pAttachedObj_20C )
    CHanaObj__Free_40C1E4(pFrame->pAttachedObj_20C);
  pFrame->pAttachedObj_20C = pVertOrMeshOrMorp;
  CHanaObj__AddRef_40BFC3(pVertOrMeshOrMorp);
  if ( CHanaObj__GetType_40BBF4(pVertOrMeshOrMorp) == 0x3EA )
  {
    pVert_v3 = (CHanaVertT3EA_S1FC *)pVertOrMeshOrMorp;
    CHanaObj__LockVtxBufT3EA_418544((CHanaVertT3EA_S1FC *)pVertOrMeshOrMorp, &dwNumVertices, (LPVOID *)&pData);
    pFrame->vertBoundMin_22C.x = pData->x;
    pFrame->vertBoundMax_220.x = pFrame->vertBoundMin_22C.x;
    pFrame->vertBoundMin_22C.y = pData->y;
    pFrame->vertBoundMax_220.y = pFrame->vertBoundMin_22C.y;
    pFrame->vertBoundMin_22C.z = pData->z;
    pFrame->vertBoundMax_220.z = pFrame->vertBoundMin_22C.z;
    for ( i = 1; i < dwNumVertices; ++i )
    {
      if ( pData[i].x < (double)pFrame->vertBoundMax_220.x )
        pFrame->vertBoundMax_220.x = pData[i].x;
      if ( pData[i].y < (double)pFrame->vertBoundMax_220.y )
        pFrame->vertBoundMax_220.y = pData[i].y;
      if ( pData[i].z < (double)pFrame->vertBoundMax_220.z )
        pFrame->vertBoundMax_220.z = pData[i].z;
      if ( pData[i].x > (double)pFrame->vertBoundMin_22C.x )
        pFrame->vertBoundMin_22C.x = pData[i].x;
      if ( pData[i].y > (double)pFrame->vertBoundMin_22C.y )
        pFrame->vertBoundMin_22C.y = pData[i].y;
      if ( pData[i].z > (double)pFrame->vertBoundMin_22C.z )
        pFrame->vertBoundMin_22C.z = pData[i].z;
    }
    CHanaObj__UnlockVrxBufT3EA_41858E(pVert_v3);
  }
  else if ( CHanaObj__GetType_40BBF4(pVertOrMeshOrMorp) == 0x3F5 )
  {
    pMesh_v2 = (CHanaMeshT3F5_S138 *)pVertOrMeshOrMorp;
    dwCount_v16 = CHanaObj__GetChildCountT3F5_40FF0F((CHanaMeshT3F5_S138 *)pVertOrMeshOrMorp);
    for ( nIndex = 0; nIndex < dwCount_v16; ++nIndex )// find vertix bound min,max
    {
      pVert_v2 = CHanaObj__GetVertsAtT3F5_40FF1D(pMesh_v2, nIndex);
      CHanaObj__LockVtxBufT3EA_418544(pVert_v2, &dwNumVertices_v2, (LPVOID *)&ppData);
      if ( !nIndex )
      {
        pFrame->vertBoundMin_22C.x = ppData->x;
        pFrame->vertBoundMax_220.x = pFrame->vertBoundMin_22C.x;
        pFrame->vertBoundMin_22C.y = ppData->y;
        pFrame->vertBoundMax_220.y = pFrame->vertBoundMin_22C.y;
        pFrame->vertBoundMin_22C.z = ppData->z;
        pFrame->vertBoundMax_220.z = pFrame->vertBoundMin_22C.z;
      }
      for ( j = 0; j < dwNumVertices_v2; ++j )
      {
        if ( ppData[j].x < (double)pFrame->vertBoundMax_220.x )
          pFrame->vertBoundMax_220.x = ppData[j].x;
        if ( ppData[j].y < (double)pFrame->vertBoundMax_220.y )
          pFrame->vertBoundMax_220.y = ppData[j].y;
        if ( ppData[j].z < (double)pFrame->vertBoundMax_220.z )
          pFrame->vertBoundMax_220.z = ppData[j].z;
        if ( ppData[j].x > (double)pFrame->vertBoundMin_22C.x )
          pFrame->vertBoundMin_22C.x = ppData[j].x;
        if ( ppData[j].y > (double)pFrame->vertBoundMin_22C.y )
          pFrame->vertBoundMin_22C.y = ppData[j].y;
        if ( ppData[j].z > (double)pFrame->vertBoundMin_22C.z )
          pFrame->vertBoundMin_22C.z = ppData[j].z;
      }
      CHanaObj__UnlockVrxBufT3EA_41858E(pVert_v2);
      CHanaObj__Free_40C1E4(pVert_v2);
    }
  }
  else if ( CHanaObj__GetType_40BBF4(pVertOrMeshOrMorp) == 0x457 )
  {
    pMorp = pVertOrMeshOrMorp;
    pMesh = pVertOrMeshOrMorp->pMeshT3F5_138;
    dwNumChild = CHanaObj__GetChildCountT3F5_40FF0F(pMesh);
    for ( k = 0; k < dwNumChild; ++k )
    {
      pVert = CHanaObj__GetVertsAtT3F5_40FF1D(pMesh, k);
      CHanaObj__LockVtxBufT3EA_418544(pVert, &numVertices, (LPVOID *)&pData_1);
      if ( !k )
      {
        pFrame->vertBoundMin_22C.x = pData_1->x;
        pFrame->vertBoundMax_220.x = pFrame->vertBoundMin_22C.x;
        pFrame->vertBoundMin_22C.y = pData_1->y;
        pFrame->vertBoundMax_220.y = pFrame->vertBoundMin_22C.y;
        pFrame->vertBoundMin_22C.z = pData_1->z;
        pFrame->vertBoundMax_220.z = pFrame->vertBoundMin_22C.z;
      }
      for ( m = 0; m < numVertices; ++m )
      {
        if ( pData_1[m].x < (double)pFrame->vertBoundMax_220.x )
          pFrame->vertBoundMax_220.x = pData_1[m].x;
        if ( pData_1[m].y < (double)pFrame->vertBoundMax_220.y )
          pFrame->vertBoundMax_220.y = pData_1[m].y;
        if ( pData_1[m].z < (double)pFrame->vertBoundMax_220.z )
          pFrame->vertBoundMax_220.z = pData_1[m].z;
        if ( pData_1[m].x > (double)pFrame->vertBoundMin_22C.x )
          pFrame->vertBoundMin_22C.x = pData_1[m].x;
        if ( pData_1[m].y > (double)pFrame->vertBoundMin_22C.y )
          pFrame->vertBoundMin_22C.y = pData_1[m].y;
        if ( pData_1[m].z > (double)pFrame->vertBoundMin_22C.z )
          pFrame->vertBoundMin_22C.z = pData_1[m].z;
      }
      CHanaObj__UnlockVrxBufT3EA_41858E(pVert);
      CHanaObj__Free_40C1E4(pVert);
    }
  }
  pFrame->vertCenter_2B4.x = (pFrame->vertBoundMax_220.x + pFrame->vertBoundMin_22C.x) / 2.0;
  pFrame->vertCenter_2B4.y = (pFrame->vertBoundMax_220.y + pFrame->vertBoundMin_22C.y) / 2.0;
  pFrame->vertCenter_2B4.z = (pFrame->vertBoundMax_220.z + pFrame->vertBoundMin_22C.z) / 2.0;
  pFrame->vertMagnitude_2B0 = sqrt(
    (pFrame->vertBoundMax_220.x - pFrame->vertCenter_2B4.x)
    * (pFrame->vertBoundMax_220.x - pFrame->vertCenter_2B4.x)
    + (pFrame->vertBoundMax_220.y - pFrame->vertCenter_2B4.y)
    * (pFrame->vertBoundMax_220.y - pFrame->vertCenter_2B4.y)
    + (pFrame->vertBoundMax_220.z - pFrame->vertCenter_2B4.z)
    * (pFrame->vertBoundMax_220.z - pFrame->vertCenter_2B4.z));
}

void register_ParseODFContent_441A5D() {
  hnh_Hook(0x44106A, hook_ParseOdfFRAM_44106A, "ParseOdfFRAM_44106A"); // [OK]
  hnh_Hook(0x4414EB, hook_ParseOdfANIM_4414EB, "ParseOdfANIM_4414EB"); // [OK]
  hnh_Hook(0x44068C, hook_ParseOdfMESH_44068C, "ParseOdfMESH_44068C"); // [OK]
  hnh_Hook(0x440896, hook_ParseOdfMORP_440896, "ParseOdfMORP_440896"); // [OK]
  hnh_Hook(0x44189C, hook_ParseOdf3DON_44189C, "ParseOdf3DON_44189C"); // [OK]
 
  hnh_HookOff();
    hnh_Hook(0x441A5D, hook_ParseODFContent_441A5D, "ParseODFContent_441A5D"); // [OK] 
    hnh_Hook(0x409510, hook_ConfigureLight,"ConfigureLight_409510"); // [OK] 
    hnh_Hook(0x40539D, hook_AssignLigthtToFrame_40539D, "AssignLigthtToFrame_40539D"); // [OK] 
    hnh_Hook(0x401CD0, hook_CHanaObj__NewAnimT3F0,"CHanaObj__NewAnimT3F0_401CD0"); // [OK] 
    hnh_Hook(0x4039C9, hook_HanaAnim_GetDuration, "HanaAnim_GetDuration_004039C9"); // [OK] 
    hnh_Hook(0x402D0E, hook_CHanaAnimT3F0__BakeKeyframesT3F0_402D0E, "CHanaAnimT3F0__BakeKeyframesT3F0_402D0E"); // [OK] 
    hnh_Hook(0x40BA89, hook_HanaFrame_GetFrameName, "HanaFrame_GetFrameName_40BA89"); // [OK] 
    hnh_Hook(0x401FBD, hook_HanaAnim_LinkFrame, "HanaAnim_LinkFrame_401FBD"); // [OK] 
    hnh_Hook(0x4035B1, hook_ApplyQTS_4035B1, "ApplyQTS_4035B1"); // [OK] 
    hnh_Hook(0x403C15, hook_CHanaAnimSetT3F1__AddAnimT3F0_403C15, "CHanaAnimSetT3F1__AddAnimT3F0_403C15"); // [OK] 
    hnh_Hook(0x408C3E, hook_HanaFrame_GetCenterPosition_408C3E, "HanaFrame_GetCenterPosition_408C3E"); // [OK] 
    hnh_Hook(0x4248F2, hook_GetSoundBufferAverageLevel, "GetSoundBufferAverageLevel_4248F2"); // [OK]
    hnh_Hook(0x404B80, hook_CalcFrameBound_404B80, "CalcFrameBound_404B80"); // [OK]
  hnh_HookOn();
}
