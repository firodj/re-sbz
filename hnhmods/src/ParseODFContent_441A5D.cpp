// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
BOOL (__cdecl*ParseODFContent_441A5D)(LPBYTE pBuf1, CHanaFrameT3EC_S354 *pRootFrameT3EC,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1) =
    reinterpret_cast<BOOL(__cdecl*)(LPBYTE, CHanaFrameT3EC_S354 *, CHanaAnimSetT3F1_S150 *)>(0x441A5D);
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
void (__cdecl*CHanaAnimSetT3F1__SetTime_403D61)(CHanaAnimSetT3F1_S150 *pHanaT3F1, float t2) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 *, float)>(0x403D61);

void (__cdecl*ParseVERT_440288)(LPBYTE *ppBuf, CHanaVertT3EA_S1FC **ppVert,
    CHanaAnimSetT3F1_S150 *pAnimSetT3F1, CHanaFrameT3EC_S354 *pRootFrameT3EC) =
    reinterpret_cast<void(__cdecl*)(LPBYTE *, CHanaVertT3EA_S1FC **,
      CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x440288);

int (__cdecl*CHanaObj__NewMorpT457)(CHanaMorpT457_S40C **ppOut, CHanaMeshT3F5_S138 *pMeshT3F5) =
    reinterpret_cast<int(__cdecl*)(CHanaMorpT457_S40C **, CHanaMeshT3F5_S138 *)>(0x433DA7);
void (__cdecl*HanaMorp_AddVertexRef)(CHanaMorpT457_S40C *pMorpT457, CHanaVertT3EA_S1FC *pVertT3EA) =
    reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *, CHanaVertT3EA_S1FC *)>(0x434370);
void (__cdecl*HanaMorp_InitSubObject)(CHanaMorpT457_S40C *pMorpT457, int zero,
    int n1, int n0, int n2, int n3, CHanaSubObj_S2C *parrS2C, int n4count,
    const char *sz, int idx) =
    reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *, int, int, int, int, int,
      CHanaSubObj_S2C *, int, const char *, int)>(0x439982);
void (__cdecl*CalcFrameBound_404B80)(CHanaFrameT3EC_S354 *pFrameT3EC, CHanaMorpT457_S40C *pMorpT3F5) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaMorpT457_S40C *)>(0x404B80);

void (__cdecl*SetFramePosRel_4069DD)(CHanaFrameT3EC_S354 *pChildFrame, CHanaFrameT3EC_S354 *pParentFrame, float x, float y, float z) =
    reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *, float, float, float)>(0x4069DD);
void (__cdecl * ApplyQTS_4035B1)( D3DXMATRIX *pOut, D3DXQUATERNION *pQ, float x, float y, float z, float sx, float sy, float sz) =
  reinterpret_cast<void(__cdecl*)( D3DXMATRIX *pOut, 
    D3DXQUATERNION *pQ, float x, float y, float z, float sx, float sy, float sz)>(0x4035B1);
void (__cdecl * ConfigureLight)( CHanaLightT3EF_S1A0 *pHanaLight, float posX, float posY, float posZ, float dirX, float dirY, float dirZ) = reinterpret_cast<void(__cdecl*)( CHanaLightT3EF_S1A0 *pHanaLight, float posX, float posY, float posZ, float dirX, float dirY, float dirZ)>(0x409510);
void (__cdecl * HanaFrame_GetCenterPosition_408C3E)( CHanaFrameRootT3EC_S2D4 *pFrame, CHanaFrameRootT3EC_S2D4 *pRelFrame, float *pOutX, float *pOutY, float *pOutZ) = reinterpret_cast<void(__cdecl*)( CHanaFrameRootT3EC_S2D4 *pFrame, CHanaFrameRootT3EC_S2D4 *pRelFrame, float *pOutX, float *pOutY, float *pOutZ)>(0x408C3E); 
BOOL (__cdecl * GetSoundBufferAverageLevel)(LPDIRECTSOUNDBUFFER pDSBuffer, float *pfPlayPos) = reinterpret_cast<BOOL(__cdecl*)(LPDIRECTSOUNDBUFFER pDSBuffer, float *pfPlayPos)>(0x4248F2);
BOOL (__cdecl * HanaAnim_FindInterpolationFrame)( CHanaAnimT3F0_S240 *pHanaAnimT3F0, float t2, CHanaAnimKeyT3F0_S78 **ppOutS78, CHanaAnimKeyT3F0_S78 **ppOutS78v2, int *pCount) = reinterpret_cast<BOOL(__cdecl*)( CHanaAnimT3F0_S240 *pHanaAnimT3F0, float t2, CHanaAnimKeyT3F0_S78 **ppOutS78, CHanaAnimKeyT3F0_S78 **ppOutS78v2, int *pCount)>(0x40344A);
BOOL (__cdecl * FindMateAnimKeyframeBounds)( CHanaMateAnimT3F7_S148 *pMateAnim, float fTime, CHanaMateAnimKeyT3F7_S50 **ppOutStart, CHanaMateAnimKeyT3F7_S50 **ppOutEnd) = reinterpret_cast<BOOL(__cdecl*)( CHanaMateAnimT3F7_S148 *pMateAnim, float fTime, CHanaMateAnimKeyT3F7_S50 **ppOutStart, CHanaMateAnimKeyT3F7_S50 **ppOutEnd)>(0x40EF23);

void(__cdecl*ApplyMateAnimMaterial)(CHanaMateAnimT3F7_S148* pMateAnim, float fNum)
= reinterpret_cast<void(__cdecl*)(CHanaMateAnimT3F7_S148 * pMateAnim, float fNum)>(0x40E969);

HRESULT (__cdecl * CHanaObj__FillVertexBufferT3EA)( CHanaVertT3EA_S1FC *pHanaT3EA, const void *pBufVertices, int dwNumVertices, WORD *pVtxIndices, unsigned int dwNumIndices) = reinterpret_cast<HRESULT(__cdecl*)( CHanaVertT3EA_S1FC *pHanaT3EA, const void *pBufVertices, int dwNumVertices, WORD *pVtxIndices, unsigned int dwNumIndices)>(0x416E65);
HRESULT (__cdecl * CHanaObj__NewVertT3EA)(CHanaVertT3EA_S1FC **ppOut, DWORD fvf, DWORD dwNumVertices) = reinterpret_cast<HRESULT(__cdecl*)(CHanaVertT3EA_S1FC **ppOut, DWORD fvf, DWORD dwNumVertices)>(0x416CE0);
CHanaVertT3EA_S1FC * (__cdecl * GetVertByName_4400C3)(LPCSTR lpName, CHanaFrameT3EC_S354 *pMeshOrMorp) = reinterpret_cast<CHanaVertT3EA_S1FC *(__cdecl*)(LPCSTR lpName, CHanaFrameT3EC_S354 *pMeshOrMorp)>(0x4400C3);
void (__cdecl * ParseOdfMeshMATE_43F8FE)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA) = reinterpret_cast<void(__cdecl*)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA)>(0x43F8FE);
void (__cdecl * HanaVert_SetMaterial)(CHanaVertT3EA_S1FC *pVertT3EA, CHanaMateT3EE_S184 *pMateT3EE) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVertT3EA, CHanaMateT3EE_S184 *pMateT3EE)>(0x418221);
void (__cdecl * FreeMate_41828C)(CHanaVertT3EA_S1FC *pHanaT3EA) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pHanaT3EA)>(0x41828C);
void (__cdecl * HanaMateAnim_InsertKeyframe)(CHanaMateAnimT3F7_S148 *pHanaT3F7, float fNum, D3DMATERIAL7 *pD3DMaterial) = reinterpret_cast<void(__cdecl*)(CHanaMateAnimT3F7_S148 *pHanaT3F7, float fNum, D3DMATERIAL7 *pD3DMaterial)>(0x40E7DA);
void (__cdecl * ParseOdfMeshMANM_43FBD1)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA, CHanaAnimSetT3F1_S150 *pAnimSetT3F1) = reinterpret_cast<void(__cdecl*)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA, CHanaAnimSetT3F1_S150 *pAnimSetT3F1)>(0x43FBD1);
void (__cdecl * HanaMateAnim_SetMaterial)(CHanaMateAnimT3F7_S148 *pMatAnim, CHanaMateT3EE_S184 *pMate) = reinterpret_cast<void(__cdecl*)(CHanaMateAnimT3F7_S148 *pMatAnim, CHanaMateT3EE_S184 *pMate)>(0x40E782);
void (__cdecl * CHanaObj__NewMateAnimT3F7)(CHanaMateAnimT3F7_S148 **ppOut) = reinterpret_cast<void(__cdecl*)(CHanaMateAnimT3F7_S148 **ppOut)>(0x40E500);
void (__cdecl * HanaAnimSet_AddMaterialAnimRef)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaMateAnimT3F7_S148 *pMatAnim) = reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaMateAnimT3F7_S148 *pMatAnim)>(0x4044D9);
void (__cdecl * ParseOdfMeshTEXT_43F460)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA) = reinterpret_cast<void(__cdecl*)(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA)>(0x43F460);
void (__cdecl * HanaVert_SetTexture)(CHanaVertT3EA_S1FC *pVert, CHanaTextureT3EB_S2CC *pTex) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVert, CHanaTextureT3EB_S2CC *pTex)>(0x418103);
void (__cdecl * HanaVert_ReleaseTexture)(CHanaVertT3EA_S1FC *pHanaT3EA) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pHanaT3EA)>(0x4181B5);
CHanaBase_S130 * (__cdecl * FindObjByType_40BC13)(char *szName, int dwType) = reinterpret_cast<CHanaBase_S130 *(__cdecl*)(char *szName, int dwType)>(0x40BC13);
void (__cdecl * ParseOdfMeshVFLG_43FDEC)(char **ppBuf, CHanaVertT3EA_S1FC *pHanaT3EA) = reinterpret_cast<void(__cdecl*)(char **ppBuf, CHanaVertT3EA_S1FC *pHanaT3EA)>(0x43FDEC);

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
          SetFramePosRel_4069DD(pChild, pParentFrame, 0.0, 0.0, 0.0);

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
      SetFramePosRel_4069DD(pChild, pRootFrameT3EC, 0.0, 0.0, 0.0);
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
//                                       matLocal_138
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

    ONCELOG("[HNH:SUB] ParseOdfFRAM_44106A\n");

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
            memcpy(&pRootFrameT3EC->matLocal_138, pScan + 8, sizeof(pRootFrameT3EC->matLocal_138));
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

  ONCELOG("[HNH:SUB] ParseOdfANIM_4414EB\n");

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

  ONCELOG("[HNH:SUB] ParseOdfMESH_44068C\n");

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

  ONCELOG("[HNH:SUB] ParseOdfMORP_440896\n");

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

  ONCELOG("[HNH:SUB] ParseOdf3DON_44189C\n");

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
    qmemcpy(&mat_v3, &pHanaT3EC->matWorld_178, sizeof(mat_v3));
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

// HanaFrame_GetCenterPosition - returns pFrame's vertCenter_2B4 expressed in pRelFrame's space. If pRelFrame is g_pSceneFrame: out = Translate(center)*matWorld_178. Otherwise also premultiplies by the inverse of pRelFrame->matWorld_178. Writes x/y/z into px/py/pz; zeroes them when pFrame is NULL.
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
      D3DXMatrixMultiply(&pOut, &pOut, &pFrame->matWorld_178);
    }
    else
    {
      D3DXMatrixTranslation(&pOut, pFrame->vertCenter_2B4.x, pFrame->vertCenter_2B4.y, pFrame->vertCenter_2B4.z);
      D3DXMatrixMultiply(&pM1, &pOut, &pFrame->matWorld_178);
      D3DXMatrixInverse(&pOut, 0, &pRelFrame->matWorld_178);
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

void __cdecl hook_InterpolateHanaAnimFrame(CHanaAnimT3F0_S240 *pAnim, float fTime)
{
  float interpTx; // [esp+Ch] [ebp-90h]
  float interpTy; // [esp+10h] [ebp-8Ch]
  float interpTz; // [esp+14h] [ebp-88h]
  CHanaAnimKeyT3F0_S78 *pEndKeyframe; // [esp+18h] [ebp-84h] BYREF
  CHanaAnimKeyT3F0_S78 *pStartKeyframe; // [esp+1Ch] [ebp-80h] BYREF
  int iResult; // [esp+20h] [ebp-7Ch] BYREF
  float fInterpolant; // [esp+24h] [ebp-78h]
  float interpSx; // [esp+28h] [ebp-74h]
  float interpSy; // [esp+2Ch] [ebp-70h]
  float interpSz; // [esp+30h] [ebp-6Ch]
  float endTx; // [esp+34h] [ebp-68h]
  float endTy; // [esp+38h] [ebp-64h]
  float endTz; // [esp+3Ch] [ebp-60h]
  D3DXQUATERNION pOut; // [esp+40h] [ebp-5Ch] BYREF
  float startTx; // [esp+50h] [ebp-4Ch]
  float startTy; // [esp+54h] [ebp-48h]
  float startTz; // [esp+58h] [ebp-44h]

  ONCELOG("[HNH:SUB] InterpolateHanaAnimFrame_40283C\n");

  pStartKeyframe = 0;
  pEndKeyframe = 0;
  if ( pAnim->nKeyCount_138 && pAnim->pHanaFram_130 )
  {
    pAnim->pHanaFram_130->pHanaAnim_2C0 = pAnim;
    if ( !pAnim->field_144 )
    {
      CHanaAnimT3F0__BakeKeyframesT3F0_402D0E(pAnim);
      pAnim->field_144 = 1;
    }
    if ( pAnim->field_140 && pAnim->fDuration_134 >= 1.0 && pAnim->fDuration_134 <= (double)fTime )
      fTime = (float)((__int64)fTime % (int)((__int64)pAnim->fDuration_134 + 1));
    if ( HanaAnim_FindInterpolationFrame(pAnim, fTime, &pStartKeyframe, &pEndKeyframe, &iResult)
      || !pEndKeyframe && pStartKeyframe )
    {
      qmemcpy(&pAnim->pHanaFram_130->matLocal_138, &pStartKeyframe->matLocal, sizeof(pAnim->pHanaFram_130->matLocal_138));
      pAnim->keyCur_14C.fTime = fTime;
      pAnim->keyCur_14C.fPosX = pStartKeyframe->fPosX;
      pAnim->keyCur_14C.fPosY = pStartKeyframe->fPosY;
      pAnim->keyCur_14C.fPosZ = pStartKeyframe->fPosZ;
      pAnim->keyCur_14C.fScaleX = pStartKeyframe->fScaleX;
      pAnim->keyCur_14C.fScaleY = pStartKeyframe->fScaleY;
      pAnim->keyCur_14C.fScaleZ = pStartKeyframe->fScaleZ;
      pAnim->keyCur_14C.quatRot = pStartKeyframe->quatRot;
      qmemcpy(&pAnim->keyCur_14C.matLocal, &pAnim->pHanaFram_130->matLocal_138, sizeof(pAnim->keyCur_14C.matLocal));
    }
    else if ( pStartKeyframe )
    {
      fInterpolant = 0.0;
      if ( pEndKeyframe->fTime - pStartKeyframe->fTime != 0.0 )
        fInterpolant = (fTime - pStartKeyframe->fTime) / (pEndKeyframe->fTime - pStartKeyframe->fTime);
      D3DXQuaternionSlerp(&pOut, &pStartKeyframe->quatRot, &pEndKeyframe->quatRot, fInterpolant);
      startTx = pStartKeyframe->fPosX;
      startTy = pStartKeyframe->fPosY;
      startTz = pStartKeyframe->fPosZ;
      endTx = pEndKeyframe->fPosX;
      endTy = pEndKeyframe->fPosY;
      endTz = pEndKeyframe->fPosZ;
      interpTx = (endTx - startTx) * fInterpolant + startTx;
      interpTy = (endTy - startTy) * fInterpolant + startTy;
      interpTz = (endTz - startTz) * fInterpolant + startTz;
      startTx = pStartKeyframe->fScaleX;
      startTy = pStartKeyframe->fScaleY;
      startTz = pStartKeyframe->fScaleZ;
      endTx = pEndKeyframe->fScaleX;
      endTy = pEndKeyframe->fScaleY;
      endTz = pEndKeyframe->fScaleZ;
      interpSx = (endTx - startTx) * fInterpolant + startTx;
      interpSy = (endTy - startTy) * fInterpolant + startTy;
      interpSz = (endTz - startTz) * fInterpolant + startTz;
      ApplyQTS_4035B1(
        &pAnim->pHanaFram_130->matLocal_138,
        &pOut,
        interpTx,
        interpTy,
        interpTz,
        interpSx,
        interpSy,
        interpSz);
      pAnim->keyCur_14C.fTime = fTime;
      pAnim->keyCur_14C.fPosX = interpTx;
      pAnim->keyCur_14C.fPosY = interpTy;
      pAnim->keyCur_14C.fPosZ = interpTz;
      pAnim->keyCur_14C.fScaleX = interpSx;
      pAnim->keyCur_14C.fScaleY = interpSy;
      pAnim->keyCur_14C.fScaleZ = interpSz;
      pAnim->keyCur_14C.quatRot = pOut;
      qmemcpy(&pAnim->keyCur_14C.matLocal, &pAnim->pHanaFram_130->matLocal_138, sizeof(pAnim->keyCur_14C.matLocal));
    }
  }
}

/**
 * HanaAnim_FindInterpolationFrame_40344A - binary-searches the key array by fTime:
 * - t2 > fDuration -> clamp: out1 = last key, out2 = NULL, return 0
 * - Exact hit -> out1 = &keys[i], out2 = &keys[i+1] (bracketing pair), return 1
 * - Miss -> out1 = first key >= t2, out2 = last key <= t2 (maintained during descent), return 0 - the caller slerps/lerps
 * between out2 -> out1
 */
BOOL __cdecl hook_HanaAnim_FindInterpolationFrame(
  CHanaAnimT3F0_S240 *pHanaAnimT3F0,
  float t2,
  CHanaAnimKeyT3F0_S78 **ppOutS78,
  CHanaAnimKeyT3F0_S78 **ppOutS78v2,
  int *pCount)
{
  CHanaAnimKeyT3F0_S78 *pFrames_test; // [esp+0h] [ebp-10h]
  int index = -1; // [esp+4h] [ebp-Ch]
  int low; // [esp+8h] [ebp-8h]
  int high; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaAnim_FindInterpolationFrame_40344A\n");

  *pCount = pHanaAnimT3F0->nKeyCount_138;
  if ( pHanaAnimT3F0->fDuration_134 >= (double)t2 )
  {
    pFrames_test = pHanaAnimT3F0->arrKeys_13C;
    *ppOutS78 = pFrames_test;
    *ppOutS78v2 = 0;
    low = 0;
    high = pHanaAnimT3F0->nKeyCount_138 - 1;
    while ( 1 )
    {
      if ( low > high )
      {
        *pCount = index;
        return 0;
      }
      index = (high + low) / 2;
      if ( pFrames_test[index].fTime == t2 )
        break;
      if ( pFrames_test[index].fTime >= (double)t2 )
      {
        high = index - 1;
        *ppOutS78v2 = &pFrames_test[index - 1];
      }
      else
      {
        low = index + 1;
        *ppOutS78 = &pFrames_test[index + 1];
      }
    }
    *pCount = index;
    *ppOutS78 = &pFrames_test[index];
    if ( index < pHanaAnimT3F0->nKeyCount_138 )
      *ppOutS78v2 = &pFrames_test[index + 1];
    return 1;
  }
  else
  {
    *pCount = pHanaAnimT3F0->nKeyCount_138 - 1;
    *ppOutS78 = &pHanaAnimT3F0->arrKeys_13C[pHanaAnimT3F0->nKeyCount_138 - 1];
    *ppOutS78v2 = 0;
    return 0;
  }
}

// CHanaAnimSetT3F1::SetTime(pAnimSet, fTime) - drive the whole anim-set to pose time fTime and sync
// its DirectSound effects. Stores pAnimSet->field_130 = fTime. For each of nCount_14C sound/morph entries
// (arroS118_14C): if fTime is outside the entry's active window [flt_104, flt_108] stop + rewind its DS
// buffer; otherwise accumulate the play offset (sub_4248F2 -> fPosAcc) - and animates the entry's morph
// frame via HanaMorpT457_Animate(pFrame_114, fTime, fPosAcc, -1). Then advances every sub-anim ref
// (refsAnimT3F0_138: sub_40283C + HanaMorpT457_Animate) and every secondary 'manm' set (pRefsManmT3F7:
// sub_40E969). A final reverse pass re-arms in-window sounds (Play + SetPosition to the frame's 3D
// position via sub_408C3E), respecting the repeat limit field_10C vs field_110. Returns pAnimSet.
void __cdecl hook_CHanaAnimSetT3F1__SetTime_403D61(CHanaAnimSetT3F1_S150 *pHanaT3F1, float t2)
{
  BOOL bOk; // [esp+4h] [ebp-20h]
  float fPosAcc; // [esp+8h] [ebp-1Ch]
  signed int i; // [esp+Ch] [ebp-18h]
  int j; // [esp+Ch] [ebp-18h]
  signed int k; // [esp+Ch] [ebp-18h]
  signed int m; // [esp+Ch] [ebp-18h]
  int nRandTick; // [esp+10h] [ebp-14h]
  float pfAvgLevel; // [esp+14h] [ebp-10h] BYREF
  float pz; // [esp+18h] [ebp-Ch] BYREF
  float py; // [esp+1Ch] [ebp-8h] BYREF
  float px; // [esp+20h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] CHanaAnimSetT3F1__SetTime_403D61\n");

  nRandTick = 0;
  pHanaT3F1->field_130 = t2;
  if ( g_pds3DListener )
  {
    fPosAcc = -0.000099999997;
    for ( i = 0; i < (signed int)pHanaT3F1->nCount_148; ++i )
    {
      if ( pHanaT3F1->arroS118_14C[i].pSnd_0 )
      {
        if ( pHanaT3F1->arroS118_14C[i].flt_104 > (double)t2 || t2 > (double)pHanaT3F1->arroS118_14C[i].flt_108 )
        {
          pHanaT3F1->arroS118_14C[i].field_110 = 0;
          pHanaT3F1->arroS118_14C[i].pSnd_0->pDSBuffer_0->Stop();
          pHanaT3F1->arroS118_14C[i].pSnd_0->pDSBuffer_0->SetCurrentPosition(
            0);
        }
        else if ( GetSoundBufferAverageLevel(pHanaT3F1->arroS118_14C[i].pSnd_0->pDSBuffer_0, &pfAvgLevel) )
        {
          fPosAcc = fPosAcc + pfAvgLevel;
        }
      }
      HanaMorpT457_Animate(pHanaT3F1->arroS118_14C[i].pFrame_114, t2, fPosAcc, -1);
    }
  }
  if ( !(rand() % 31) )
    nRandTick = 1;
  for ( j = 0; j < pHanaT3F1->nAnimCount_134; ++j )
  {
    InterpolateHanaAnimFrame(pHanaT3F1->refsAnimT3F0_138[j], t2);
    HanaMorpT457_Animate(pHanaT3F1->refsAnimT3F0_138[j]->pHanaFram_130, t2, -1.0, nRandTick);
  }
  for ( k = 0; k < (signed int)pHanaT3F1->nManmCount_140; ++k )
    ApplyMateAnimMaterial(pHanaT3F1->pRefsManmT3F7[k], t2);
  if ( g_pds3DListener )
  {
    MtxStackStartMultLocal_4074EF();
    for ( m = pHanaT3F1->nCount_148 - 1; m >= 0; --m )
    {
      if ( pHanaT3F1->arroS118_14C[m].pSnd_0 )
      {
        if ( pHanaT3F1->arroS118_14C[m].pFrame_114 )
        {
          if ( pHanaT3F1->arroS118_14C[m].flt_104 <= (double)t2 && t2 <= (double)pHanaT3F1->arroS118_14C[m].flt_108 )
          {
            bOk = pHanaT3F1->arroS118_14C[m].pSnd_0->pDSBuffer_0
              && !pHanaT3F1->arroS118_14C[m].pSnd_0->pDSBuffer_0->GetStatus(
                &g_dwDSStatus_5185C8)
              && (g_dwDSStatus_5185C8 & 1) != 0;
            if ( !bOk
              && (pHanaT3F1->arroS118_14C[m].field_110 < pHanaT3F1->arroS118_14C[m].field_10C
                || pHanaT3F1->arroS118_14C[m].field_10C < 0) )
            {
              pHanaT3F1->arroS118_14C[m].pSnd_0->pDSBuffer_0->Play(
                0,
                0,
                0);
              ++pHanaT3F1->arroS118_14C[m].field_110;
            }
          }
          HanaFrame_GetCenterPosition_408C3E(pHanaT3F1->arroS118_14C[m].pFrame_114, g_pSceneFrame, &px, &py, &pz);
          pHanaT3F1->arroS118_14C[m].pSnd_0->pDS3DBuffer_4->SetPosition(
            px,
            py,
            pz,
            1);
        }
        else
        {
          pHanaT3F1->arroS118_14C[m].pSnd_0->pDSBuffer_0->Stop();
        }
      }
    }
  }
}

void __cdecl hook_ApplyMateAnimMaterial(CHanaMateAnimT3F7_S148 *pMateAnim, float fTime)
{
  float fT_alpha; // [esp+8h] [ebp-94h]
  float fT_specular; // [esp+Ch] [ebp-90h]
  float fT_emissive; // [esp+10h] [ebp-8Ch]
  float fT_diffuse; // [esp+14h] [ebp-88h]
  float fT_alphaWrap; // [esp+18h] [ebp-84h]
  float fT_specWrap; // [esp+1Ch] [ebp-80h]
  float fT_emisWrap; // [esp+20h] [ebp-7Ch]
  float fT_diffWrap; // [esp+24h] [ebp-78h]
  D3DMATERIAL7 outMaterial; // [esp+2Ch] [ebp-70h] BYREF
  CHanaMateAnimKeyT3F7_S50 *pKeyEnd; // [esp+70h] [ebp-2Ch] BYREF
  CHanaMateAnimKeyT3F7_S50 *pKeyStart; // [esp+74h] [ebp-28h] BYREF
  float fEndR; // [esp+78h] [ebp-24h]
  float fEndG; // [esp+7Ch] [ebp-20h]
  float fEndB; // [esp+80h] [ebp-1Ch]
  __int64 qLerpRG; // [esp+84h] [ebp-18h]
  float fLerpB; // [esp+8Ch] [ebp-10h]
  float fStartR; // [esp+90h] [ebp-Ch]
  float fStartG; // [esp+94h] [ebp-8h]
  float fStartB; // [esp+98h] [ebp-4h]

  ONCELOG("[HNH:SUB] ApplyMateAnimMaterial_40E969\n");

  pKeyStart = 0;
  pKeyEnd = 0;
  if ( pMateAnim->fLoopEnd_134 >= 1.0 && pMateAnim->fLoopEnd_134 <= (double)fTime )
    fTime = (float)((__int64)fTime % (int)((__int64)pMateAnim->fLoopEnd_134 + 1));
  if ( pMateAnim->nKeyCount_138 >= 2u )
  {
    if ( FindMateAnimKeyframeBounds(pMateAnim, fTime, &pKeyStart, &pKeyEnd) )
    {
      ApplyMaterial(pMateAnim->pMateT3EE, &pKeyStart->d3dMat7_4);
    }
    else
    {
      if ( pKeyEnd )
      {
        fStartR = pKeyStart->d3dMat7_4.diffuse.r;
        fStartG = pKeyStart->d3dMat7_4.diffuse.g;
        fStartB = pKeyStart->d3dMat7_4.diffuse.b;
        fEndR = pKeyEnd->d3dMat7_4.diffuse.r;
        fEndG = pKeyEnd->d3dMat7_4.diffuse.g;
        fEndB = pKeyEnd->d3dMat7_4.diffuse.b;
        fT_diffuse = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_diffuse + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_diffuse + fStartG;
        fLerpB = (fEndB - fStartB) * fT_diffuse + fStartB;
        *(_QWORD *)&outMaterial.diffuse.r = qLerpRG;
        outMaterial.diffuse.b = fLerpB;
        fStartR = pKeyStart->d3dMat7_4.emissive.r;
        fStartG = pKeyStart->d3dMat7_4.emissive.g;
        fStartB = pKeyStart->d3dMat7_4.emissive.b;
        fEndR = pKeyEnd->d3dMat7_4.emissive.r;
        fEndG = pKeyEnd->d3dMat7_4.emissive.g;
        fEndB = pKeyEnd->d3dMat7_4.emissive.b;
        fT_emissive = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_emissive + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_emissive + fStartG;
        fLerpB = (fEndB - fStartB) * fT_emissive + fStartB;
        *(_QWORD *)&outMaterial.emissive.r = qLerpRG;
        outMaterial.emissive.b = fLerpB;
        fStartR = pKeyStart->d3dMat7_4.specular.r;
        fStartG = pKeyStart->d3dMat7_4.specular.g;
        fStartB = pKeyStart->d3dMat7_4.specular.b;
        fEndR = pKeyEnd->d3dMat7_4.specular.r;
        fEndG = pKeyEnd->d3dMat7_4.specular.g;
        fEndB = pKeyEnd->d3dMat7_4.specular.b;
        fT_specular = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_specular + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_specular + fStartG;
        fLerpB = (fEndB - fStartB) * fT_specular + fStartB;
        *(_QWORD *)&outMaterial.specular.r = qLerpRG;
        outMaterial.specular.b = fLerpB;
        *(_QWORD *)&outMaterial.ambient.r = 0x3F8000003F800000LL;
        *(_QWORD *)&outMaterial.dcvAmbient.b = 1065353216;
        fStartR = pKeyStart->d3dMat7_4.diffuse.a;
        fStartG = 0.0;
        fStartB = 0.0;
        fEndR = pKeyEnd->d3dMat7_4.diffuse.a;
        fEndG = 0.0;
        fEndB = 0.0;
        fT_alpha = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_alpha + fStartR;
        *((float *)&qLerpRG + 1) = (0.0 - 0.0) * fT_alpha + 0.0;
      }
      else
      {
        pKeyEnd = pMateAnim->pKeyHead_13C;
        fStartR = pKeyStart->d3dMat7_4.diffuse.r;
        fStartG = pKeyStart->d3dMat7_4.diffuse.g;
        fStartB = pKeyStart->d3dMat7_4.diffuse.b;
        fEndR = pKeyEnd->d3dMat7_4.diffuse.r;
        fEndG = pKeyEnd->d3dMat7_4.diffuse.g;
        fEndB = pKeyEnd->d3dMat7_4.diffuse.b;
        fT_diffWrap = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_diffWrap + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_diffWrap + fStartG;
        fLerpB = (fEndB - fStartB) * fT_diffWrap + fStartB;
        *(_QWORD *)&outMaterial.diffuse.r = qLerpRG;
        outMaterial.diffuse.b = fLerpB;
        fStartR = pKeyStart->d3dMat7_4.emissive.r;
        fStartG = pKeyStart->d3dMat7_4.emissive.g;
        fStartB = pKeyStart->d3dMat7_4.emissive.b;
        fEndR = pKeyEnd->d3dMat7_4.emissive.r;
        fEndG = pKeyEnd->d3dMat7_4.emissive.g;
        fEndB = pKeyEnd->d3dMat7_4.emissive.b;
        fT_emisWrap = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_emisWrap + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_emisWrap + fStartG;
        fLerpB = (fEndB - fStartB) * fT_emisWrap + fStartB;
        *(_QWORD *)&outMaterial.emissive.r = qLerpRG;
        outMaterial.emissive.b = fLerpB;
        fStartR = pKeyStart->d3dMat7_4.specular.r;
        fStartG = pKeyStart->d3dMat7_4.specular.g;
        fStartB = pKeyStart->d3dMat7_4.specular.b;
        fEndR = pKeyEnd->d3dMat7_4.specular.r;
        fEndG = pKeyEnd->d3dMat7_4.specular.g;
        fEndB = pKeyEnd->d3dMat7_4.specular.b;
        fT_specWrap = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_specWrap + fStartR;
        *((float *)&qLerpRG + 1) = (fEndG - fStartG) * fT_specWrap + fStartG;
        fLerpB = (fEndB - fStartB) * fT_specWrap + fStartB;
        *(_QWORD *)&outMaterial.specular.r = qLerpRG;
        outMaterial.specular.b = fLerpB;
        *(_QWORD *)&outMaterial.ambient.r = 0x3F8000003F800000LL;
        *(_QWORD *)&outMaterial.dcvAmbient.b = 1065353216;
        fStartR = pKeyStart->d3dMat7_4.diffuse.a;
        fStartG = 0.0;
        fStartB = 0.0;
        fEndR = pKeyEnd->d3dMat7_4.diffuse.a;
        fEndG = 0.0;
        fEndB = 0.0;
        fT_alphaWrap = (fTime - pKeyStart->fTime) / (pKeyEnd->fTime - pKeyStart->fTime);
        *(float *)&qLerpRG = (fEndR - fStartR) * fT_alphaWrap + fStartR;
        *((float *)&qLerpRG + 1) = (0.0 - 0.0) * fT_alphaWrap + 0.0;
      }
      fLerpB = *((float *)&qLerpRG + 1);
      LODWORD(outMaterial.diffuse.a) = qLerpRG;
      ApplyMaterial(pMateAnim->pMateT3EE, &outMaterial);
    }
  }
}

BOOL __cdecl hook_FindMateAnimKeyframeBounds(
  CHanaMateAnimT3F7_S148 *pMateAnim,
  float fTime,
  CHanaMateAnimKeyT3F7_S50 **ppOutStart,
  CHanaMateAnimKeyT3F7_S50 **ppOutEnd)
{
  CHanaMateAnimKeyT3F7_S50 *pKeyWalk; // [esp+0h] [ebp-8h]
  unsigned int iKey; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindMateAnimKeyframeBounds_40EF23\n");

  if ( pMateAnim->fLoopEnd_134 >= (double)fTime )
  {
    pKeyWalk = pMateAnim->pKeyHead_13C;
    *ppOutStart = pKeyWalk;
    *ppOutEnd = 0;
    for ( iKey = 0; iKey < pMateAnim->nKeyCount_138; ++iKey )
    {
      if ( pKeyWalk->fTime == fTime )
      {
        *ppOutStart = pKeyWalk;
        return 1;
      }
      if ( pKeyWalk->fTime < (double)fTime )
        *ppOutStart = pKeyWalk;
      if ( pKeyWalk->fTime > (double)fTime )
      {
        *ppOutEnd = pKeyWalk;
        return 0;
      }
      pKeyWalk = pKeyWalk->pNextKey;
      if ( !pKeyWalk )
        return 0;
    }
    return 0;
  }
  else
  {
    *ppOutStart = pMateAnim->pKeyTail_140;
    *ppOutEnd = 0;
    return 0;
  }
}

// SetFramePosRel: place pFrameDst's origin at the world-space position of pFrameSrc's
// local point (x,y,z) -- the engine's standard "move frame to point" primitive. Frame
// transforms obey matWorld_178 = matLocal_138 * matParentWorld_1B8 (see
// MtxStackRecursiveMulLocal_4075B0), so this is just:
//   1. P = world position of pFrameSrc's local point (x,y,z)
//   2. new local translation = P expressed in pFrameDst's parent space
//   3. recompose the world matrix
// Only the local translation row is written: matLocal_138's rotation/scale stay as-is,
// so with identity rotation this is a pure translation (orientation preserved).
// pFrameSrc == pFrameDst means "offset in my own local space" (-Z = forward); used as a
// teleport, e.g. IsFoeWithinHeroRange_448B78's save / probe-2.5-ahead / restore pattern.
void __cdecl hook_SetFramePosRel_4069DD(
  CHanaFrameT3EC_S354 *pFrameDst,
  CHanaFrameT3EC_S354 *pFrameSrc,
  float x,
  float y,
  float z)
{
  D3DXMATRIX matPoint;         // T(x,y,z) * matWorld_178, then parent-inverted
  D3DXMATRIX matParentWorldInv;

  ONCELOG("[HNH:SUB] SetFramePosRel_4069DD\n");

  // 1) P = world-space position of the local point (x,y,z) in pFrameSrc's space
  D3DXMatrixTranslation(&matPoint, x, y, z);
  D3DXMatrixMultiply(&matPoint, &matPoint, &pFrameSrc->matWorld_178);

  // 2) re-express P in pFrameDst's parent space -> that IS the new local translation.
  //    (The original built [dstWorld with row4 = P] * inv(parentWorld) and kept only
  //    row 4 -- row 4 of a row-vector product ignores the left side's rotation, so
  //    skipping the dstWorld copy is bit-identical.)
  D3DXMatrixInverse(&matParentWorldInv, 0, &pFrameDst->matParentWorld_1B8);
  D3DXMatrixMultiply(&matPoint, &matPoint, &matParentWorldInv);

  pFrameDst->matLocal_138._41 = matPoint._41;
  pFrameDst->matLocal_138._42 = matPoint._42;
  pFrameDst->matLocal_138._43 = matPoint._43;

  // 3) recompose: world = local * parentWorld (rotation/scale of matLocal_138 untouched)
  D3DXMatrixMultiply(&pFrameDst->matWorld_178, &pFrameDst->matLocal_138, &pFrameDst->matParentWorld_1B8);
}

void __cdecl hook_HanaMorp_InitSubObject(
  CHanaMorpT457_S40C *pMorp,
  int unused,
  int n1,
  int n0,
  int n2,
  int n3,
  CHanaSubObj_S2C *parrS2C,
  int nCountS2C,
  const char *pszName,
  int index)
{
  if (!pMorp) return;

  ONCELOG("[HNH:SUB] HanaMorp_InitSubObject_439982\n");
 
  if ( !pMorp->arrSubObj_144[index].pSubObj_0 )
    pMorp->arrSubObj_144[index].pSubObj_0 = (CHanaSubObj_S120 *)MemAlloc_40DEF7(0x120u);
  if ( pMorp->arrSubObj_144[index].pSubObj_0->parrS2C )
  {
    MemFree_40DF64(pMorp->arrSubObj_144[index].pSubObj_0->parrS2C);
    pMorp->arrSubObj_144[index].pSubObj_0->parrS2C = 0;
  }
  pMorp->arrSubObj_144[index].pSubObj_0->n1 = n1;
  pMorp->arrSubObj_144[index].pSubObj_0->n0 = n0;
  pMorp->arrSubObj_144[index].pSubObj_0->n2 = n2;
  pMorp->arrSubObj_144[index].pSubObj_0->n3 = n3;
  wsprintfA(pMorp->arrSubObj_144[index].pSubObj_0->szName_18, "%s", pszName);
  pMorp->arrSubObj_144[index].pSubObj_0->parrS2C = parrS2C;
  pMorp->arrSubObj_144[index].pSubObj_0->nCountS2C_0 = nCountS2C;
  pMorp->arrSubObj_144[index].pSubObj_0->pVert_14 = 0;
  ++pMorp->nCount_344;
}

void __cdecl hook_HanaMorp_AddVertexRef(CHanaMorpT457_S40C *pMorp, CHanaVertT3EA_S1FC *pVert)
{
  CHanaVertT3EA_S1FC **ppOldVertexRefs; // [esp+0h] [ebp-Ch]
  int loopVar; // [esp+4h] [ebp-8h]

  ONCELOG("[HNH:SUB] HanaMorp_AddVertexRef_434370\n");

  if (!pMorp || !pVert) return;

  
  for ( loopVar = 0; loopVar < pMorp->nCountVertT3EA_138; ++loopVar )
  {
    if ( !strcmp(pMorp->refsVertT3EA_13C[loopVar]->szName_8, pVert->szName_8) )
      return;
  }
  ++pMorp->nCountVertT3EA_138;
  if ( pMorp->refsVertT3EA_13C )
  {
    ppOldVertexRefs = pMorp->refsVertT3EA_13C;
    pMorp->refsVertT3EA_13C = (CHanaVertT3EA_S1FC **)MemAlloc_40DEF7(4 * pMorp->nCountVertT3EA_138 + 128);
    memcpy(pMorp->refsVertT3EA_13C, ppOldVertexRefs, 4 * pMorp->nCountVertT3EA_138 - 4);
    if ( ppOldVertexRefs )
      MemFree_40DF64(ppOldVertexRefs);
  }
  else
  {
    pMorp->refsVertT3EA_13C = (CHanaVertT3EA_S1FC **)MemAlloc_40DEF7(0x84u);
  }
  pMorp->refsVertT3EA_13C[pMorp->nCountVertT3EA_138 - 1] = pVert;
  CHanaObj__AddRef_40BFC3(pVert);
}

BOOL __cdecl hook_CHanaObj__NewMorpT457(CHanaMorpT457_S40C **ppOut, CHanaMeshT3F5_S138 *pMesh)
{
  CHanaMorpT457_S40C *pMorp; // [esp+4h] [ebp-4h]

  if ( !pMesh )
    return 0;

  ONCELOG("[HNH:SUB] CHanaObj__NewMorpT457_433DA7\n");

  pMorp = (CHanaMorpT457_S40C *)MemAlloc_40DEF7(0x40Cu);
  memset(pMorp, 0, 0x138u);
  pMorp->dwType = 1111;
  CHanaObj__AddRef_40BFC3(pMorp);
  pMorp->pMeshT3F5_138 = pMesh;
  CHanaObj__AddRef_40BFC3(pMesh);
  pMorp->pArrObjs_140 = 0;
  pMorp->arrMKeyT3EA_34C = (CHanaMKey_S8 **)MemAlloc_40DEF7(4 * pMorp->pMeshT3F5_138->nCountVertT3EA_130 + 128);
  pMorp->arrMKeyT3EASize_348 = (int *)MemAlloc_40DEF7(4 * pMorp->pMeshT3F5_138->nCountVertT3EA_130 + 128);
  pMorp->fEaseRate_388 = 2.0;
  *ppOut = pMorp;
  return 1;
}

void __cdecl hook_ParseVERT_440288(
  LPBYTE *ppBuf,
  CHanaVertT3EA_S1FC **ppVert,
  CHanaAnimSetT3F1_S150 *pAnimSetT3F1,
  CHanaFrameT3EC_S354 *pRootFrameT3EC)
{
  int name_len_v4; // eax
  int nCurIndex_v6; // [esp-4h] [ebp-230h]
  char *pBuf_v7; // [esp+4h] [ebp-228h]
  DWORD dwNumVertices_v8; // [esp+8h] [ebp-224h]
  CHanaVertT3EA_S1FC *pVertT3EA; // [esp+Ch] [ebp-220h] BYREF
  CHAR szVertName2[256]; // [esp+10h] [ebp-21Ch] BYREF
  CHAR szVertName[256]; // [esp+110h] [ebp-11Ch] BYREF
  WORD *pBuf_v12; // [esp+210h] [ebp-1Ch]
  int nIndex_v13; // [esp+214h] [ebp-18h]
  int bFound; // [esp+218h] [ebp-14h]
  char *pBuf; // [esp+21Ch] [ebp-10h] BYREF
  unsigned int dwNumIndices_v16; // [esp+220h] [ebp-Ch]
  unsigned int name_len_v17; // [esp+224h] [ebp-8h]
  int v17; // [esp+228h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseVERT_440288\n");

  pBuf = (char *)*ppBuf;
  v17 = 0;
  pVertT3EA = *ppVert;
  memset(szVertName, 0, 0xFFu);
  name_len_v17 = *(_DWORD *)pBuf;
  pBuf += 4;
  wsprintfA(szVertName, "%s", pBuf);
  name_len_v4 = lstrlenA(pBuf);
  pBuf += name_len_v4 + 1;
  dwNumVertices_v8 = *(_DWORD *)pBuf;
  pBuf += 4;
  pBuf_v7 = pBuf;
  pBuf += 32 * dwNumVertices_v8;
  dwNumIndices_v16 = *(_DWORD *)pBuf;
  pBuf += 4;
  pBuf_v12 = (WORD *)pBuf;
  pBuf += 2 * dwNumIndices_v16;
  nIndex_v13 = 1;
  bFound = 0;
  wsprintfA(szVertName2, "%s", szVertName);
  while ( 1 )
  {
    pVertT3EA = GetVertByName_4400C3(szVertName, pRootFrameT3EC);
    if ( !pVertT3EA )
      break;
    if ( pVertT3EA->dwNumVertices_134 == dwNumVertices_v8 && pVertT3EA->dwNumIndices_140 == dwNumIndices_v16 )
    {
      bFound = 1;
      break;
    }
    CHanaObj__Free_40C1E4(pVertT3EA);
    pVertT3EA = 0;
    nCurIndex_v6 = nIndex_v13++;
    wsprintfA(szVertName, "%s(%d)", szVertName2, nCurIndex_v6);// if duplicate name, check by vertices numbers
  }
  if ( bFound != 1 )
  {
    if ( pVertT3EA )
    {
      CHanaObj__Free_40C1E4(pVertT3EA);
      pVertT3EA = 0;
    }
    CHanaObj__NewVertT3EA(&pVertT3EA, D3DFVF_VERTEX, dwNumVertices_v8);// vertices format hardcoded to 0x112
    CHanaObj__FillVertexBufferT3EA(pVertT3EA, pBuf_v7, dwNumVertices_v8, pBuf_v12, dwNumIndices_v16);
    AttachHanaObj_40BAB8(pVertT3EA, szVertName);
  }
  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        while ( 1 )
        {
          while ( !memcmp(pBuf, "MATE", 4u) )
          {
            pBuf += 4;
            ParseOdfMeshMATE_43F8FE(&pBuf, pVertT3EA);
          }
          if ( memcmp(pBuf, "MANM", 4u) )
            break;
          pBuf += 4;
          ParseOdfMeshMANM_43FBD1(&pBuf, pVertT3EA, pAnimSetT3F1);
        }
        if ( memcmp(pBuf, "TEXT", 4u) )
          break;
        pBuf += 4;
        ParseOdfMeshTEXT_43F460(&pBuf, pVertT3EA);
      }
      if ( memcmp(pBuf, "VFLG", 4u) )
        break;
      pBuf += 4;
      ParseOdfMeshVFLG_43FDEC(&pBuf, pVertT3EA);
    }
    if ( !memcmp(pBuf, "Mkey", 4u)
      || !memcmp(pBuf, "MPPL", 4u)
      || !memcmp(pBuf, "ANIM", 4u)
      || !memcmp(pBuf, "VERT", 4u)
      || !memcmp(pBuf, "FRAM", 4u)
      || !memcmp(pBuf, "MESH", 4u)
      || !memcmp(pBuf, "MORP", 4u)
      || !memcmp(pBuf, "ファイルは終了", 14u) )
    {
      break;
    }
    ++pBuf;
  }
  *ppBuf = (LPBYTE)pBuf;
  *ppVert = pVertT3EA;
}

HRESULT __cdecl hook_CHanaObj__FillVertexBufferT3EA(
  CHanaVertT3EA_S1FC *pHanaT3EA,
  const void *pBufVertices,
  int dwNumVertices,
  WORD *pVtxIndices,
  unsigned int dwNumIndices)
{
  unsigned int i; // [esp+10h] [ebp-Ch]
  HRESULT hr; // [esp+14h] [ebp-8h]
  void *pData; // [esp+18h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] CHanaObj__FillVertexBufferT3EA_416E65\n")

  hr = pHanaT3EA->pD3DVtxBuf_130->Lock(0x801, &pData, 0);
  if (FAILED(hr))
    return hr;
  qmemcpy(pData, pBufVertices, pHanaT3EA->dwVertexSize * dwNumVertices);
  pHanaT3EA->pD3DVtxBuf_130->Unlock();
  pHanaT3EA->dwNumVertices_134 = dwNumVertices;
  pHanaT3EA->dwNumIndices_140 = dwNumIndices;
  pHanaT3EA->pVtxIndices = (WORD *)MemAlloc_40DEF7(2 * dwNumIndices + 128);
  for ( i = 0; i < dwNumIndices; ++i )
    pHanaT3EA->pVtxIndices[i] = pVtxIndices[i];
  return 0;
}

HRESULT __cdecl hook_CHanaObj__NewVertT3EA(CHanaVertT3EA_S1FC **ppOut, DWORD fvf, DWORD dwNumVertices)
{
  DWORD dwCaps; // eax
  CHanaVertT3EA_S1FC *pVert; // [esp+1Ch] [ebp-100h]
  D3DDEVICEDESC7 d3dDeviceDesc; // [esp+20h] [ebp-FCh] BYREF
  D3DVERTEXBUFFERDESC d3dVtxBufDesc; // [esp+10Ch] [ebp-10h] BYREF
  ONCELOG("[HNH:SUB] CHanaObj__NewVertT3EA_416CE0\n");

  pVert = (CHanaVertT3EA_S1FC *)MemAlloc_40DEF7(0x1FCu);
  memset(pVert, 0, sizeof(CHanaVertT3EA_S1FC));
  pVert->dwType = 0x3EA;
  if ( g_pD3DDevice_4D6C78->GetCaps(&d3dDeviceDesc) < 0 )
    return E_FAIL;
  d3dVtxBufDesc.dwSize = 16;
  d3dVtxBufDesc.dwCaps = 0;
  d3dVtxBufDesc.dwFVF = fvf;
  d3dVtxBufDesc.dwNumVertices = dwNumVertices;
  pVert->dwFVF = fvf;
  pVert->dwNumVertices_134 = dwNumVertices;
  dwCaps = d3dVtxBufDesc.dwCaps;
  BYTE1(dwCaps) = BYTE1(d3dVtxBufDesc.dwCaps) | 8;
  d3dVtxBufDesc.dwCaps = dwCaps;
  if ( g_pD3D_4D6C84->CreateVertexBuffer(&d3dVtxBufDesc, &pVert->pD3DVtxBuf_130, 0) < 0 )
    return E_FAIL;
  d3dVtxBufDesc.dwNumVertices = 8;
  pVert->dwVertexSize = CalculateVertexSize(fvf);
  pVert->pTexT3EB = 0;
  pVert->pMateT3EE = 0;
  CHanaObj__AddRef_40BFC3(pVert);
  *ppOut = pVert;
  return 0;
}

CHanaVertT3EA_S1FC *__cdecl hook_GetVertByName_4400C3(LPCSTR lpName, CHanaFrameT3EC_S354 *pMeshOrMorp)
{
  int len_v2; // eax
  int len_v4; // eax
  CHanaMorpT457_S40C *pMorpT457_20C; // [esp+0h] [ebp-20h]
  CHanaMeshT3F5_S138 *pMeshT3F5; // [esp+4h] [ebp-1Ch]
  CHanaVertT3EA_S1FC *pVert; // [esp+8h] [ebp-18h]
  CHanaVertT3EA_S1FC *pVert2; // [esp+8h] [ebp-18h]
  CHanaVertT3EA_S1FC *pVert1; // [esp+8h] [ebp-18h]
  unsigned int i; // [esp+Ch] [ebp-14h]
  unsigned int j; // [esp+Ch] [ebp-14h]
  unsigned int k; // [esp+10h] [ebp-10h]
  CHanaNode *pFirstChildNode; // [esp+1Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] GetVertByName_4400C3\n");

  pVert = 0;
  if ( pMeshOrMorp->pAttachedObj_20C )
  {
    if ( pMeshOrMorp->pAttachedObj_20C->dwType == 0x3F5 )
    {
      pMeshT3F5 = (CHanaMeshT3F5_S138 *)pMeshOrMorp->pAttachedObj_20C;
      for ( i = 0; i < pMeshT3F5->nCountVertT3EA_130; ++i )
      {
        len_v2 = lstrlenA(lpName);
        if ( !memcmp(pMeshT3F5->refsVertT3EA[i]->szName_8, lpName, len_v2 + 1) )
        {
          pVert2 = pMeshT3F5->refsVertT3EA[i];
          CHanaObj__AddRef_40BFC3(pVert2);
          return pVert2;
        }
      }
    }
    if ( pMeshOrMorp->pAttachedObj_20C->dwType == 0x457 )
    {
      pMorpT457_20C = pMeshOrMorp->pAttachedObj_20C;
      for ( j = 0; j < pMorpT457_20C->nCountVertT3EA_138; ++j )
      {
        len_v4 = lstrlenA(lpName);
        if ( !memcmp(pMorpT457_20C->refsVertT3EA_13C[j]->szName_8, lpName, len_v4 + 1) )
        {
          pVert1 = pMorpT457_20C->refsVertT3EA_13C[j];
          CHanaObj__AddRef_40BFC3(pVert1);
          return pVert1;
        }
      }
    }
  }
  pFirstChildNode = pMeshOrMorp->pFirstChildNode;
  for ( k = 0; k < pMeshOrMorp->nCountChild; ++k )
  {
    if ( pFirstChildNode->pData )
      pVert = GetVertByName_4400C3(lpName, pFirstChildNode->pData);
    if ( pVert )
      break;
    pFirstChildNode = pFirstChildNode->pNext;
  }
  return pVert;
}

void __cdecl hook_ParseOdfMeshMATE_43F8FE(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA)
{
  int name_len_v2; // eax
  CHAR String[256]; // [esp+0h] [ebp-110h] BYREF
  float *pBuf2; // [esp+100h] [ebp-10h]
  CHanaMateT3EE_S184 *pMateT3EE; // [esp+104h] [ebp-Ch] BYREF
  LPDWORD pBuf; // [esp+108h] [ebp-8h]
  DWORD v7; // [esp+10Ch] [ebp-4h]
  ONCELOG("[HNH:SUB] ParseOdfMeshMATE_43F8FE\n")
  pMateT3EE = 0;
  v7 = 0;
  pBuf = (LPDWORD)*ppBuf;
  v7 = *pBuf++;
  CHanaObj__NewMateT3EE_S184(&pMateT3EE);
  wsprintfA(String, "%s", (const char *)pBuf);
  name_len_v2 = lstrlenA((LPCSTR)pBuf);
  pBuf = (LPDWORD)((char *)pBuf + name_len_v2 + 1);
  AttachHanaObj_40BAB8(pMateT3EE, String);
  pBuf2 = (float *)pBuf;
  LODWORD(pMateT3EE->d3dMaterial_130.ambient.a) = *pBuf;
  ++pBuf2;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.ambient.r = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.ambient.g = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.ambient.b = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.diffuse.a = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.diffuse.r = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.diffuse.g = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.diffuse.b = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.emissive.a = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.emissive.r = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.emissive.g = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.emissive.b = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.specular.a = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.specular.r = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.specular.g = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.specular.b = *pBuf2++;
  ++pBuf;
  pMateT3EE->d3dMaterial_130.power = *pBuf2;
  ++pBuf;
  HanaVert_SetMaterial(pVertT3EA, pMateT3EE);
  if ( pMateT3EE )
  {
    CHanaObj__Free_40C1E4(pMateT3EE);
    pMateT3EE = 0;
  }
  *ppBuf = (char *)pBuf;
}

void __cdecl hook_HanaVert_SetMaterial(CHanaVertT3EA_S1FC *pVertT3EA, CHanaMateT3EE_S184 *pMateT3EE)
{
  ONCELOG("[HNH:SUB] HanaVert_SetMaterial_418221\n")

  if ( pVertT3EA->pMateT3EE )
    FreeMate_41828C(pVertT3EA);
  pVertT3EA->pMateT3EE = pMateT3EE;
  if ( pMateT3EE )
    CHanaObj__AddRef_40BFC3(pMateT3EE);
}

void __cdecl hook_FreeMate_41828C(CHanaVertT3EA_S1FC *pHanaT3EA)
{
  ONCELOG("[HNH:SUB] FreeMate_41828C\n")

  if ( pHanaT3EA->pMateT3EE )
  {
    CHanaObj__Free_40C1E4(pHanaT3EA->pMateT3EE);
    pHanaT3EA->pMateT3EE = 0;
  }
}

void __cdecl hook_HanaMateAnim_InsertKeyframe(CHanaMateAnimT3F7_S148 *pHanaT3F7, float fNum, D3DMATERIAL7 *pD3DMaterial)
{
  CHanaMateAnimKeyT3F7_S50 *pRight; // [esp+28h] [ebp-Ch] BYREF
  CHanaMateAnimKeyT3F7_S50 *pNewNode; // [esp+2Ch] [ebp-8h]
  CHanaMateAnimKeyT3F7_S50 *pLeft; // [esp+30h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] HanaMateAnim_InsertKeyframe_40E7DA\n");

  if ( pHanaT3F7->fLoopEnd_134 < (double)fNum )
    pHanaT3F7->fLoopEnd_134 = fNum;
  if ( FindMateAnimKeyframeBounds(pHanaT3F7, fNum, &pLeft, &pRight) )
  {
    qmemcpy(&pLeft->d3dMat7_4, pD3DMaterial, sizeof(pLeft->d3dMat7_4));
  }
  else if ( pRight )
  {
    ++pHanaT3F7->nKeyCount_138;
    pNewNode = (CHanaMateAnimKeyT3F7_S50 *)MemAlloc_40DEF7(0x50u);
    memset(pNewNode, 0, sizeof(CHanaMateAnimKeyT3F7_S50));
    pRight->pPrevKey = pNewNode;
    pLeft->pNextKey = pNewNode;
    pNewNode->fTime = fNum;
    qmemcpy(&pNewNode->d3dMat7_4, pD3DMaterial, sizeof(pNewNode->d3dMat7_4));
    pNewNode->pPrevKey = pLeft;
    pNewNode->pNextKey = pRight;
  }
  else
  {
    ++pHanaT3F7->nKeyCount_138;
    pNewNode = (CHanaMateAnimKeyT3F7_S50 *)MemAlloc_40DEF7(0x50u);
    memset(pNewNode, 0, sizeof(CHanaMateAnimKeyT3F7_S50));
    pLeft->pNextKey = pNewNode;
    pNewNode->fTime = fNum;
    qmemcpy(&pNewNode->d3dMat7_4, pD3DMaterial, sizeof(pNewNode->d3dMat7_4));
    pNewNode->pPrevKey = pLeft;
    pNewNode->pNextKey = 0;
    pHanaT3F7->pKeyTail_140 = pNewNode;
  }
}

void __cdecl hook_ParseOdfMeshMANM_43FBD1(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA, CHanaAnimSetT3F1_S150 *pAnimSetT3F1)
{
  D3DMATERIAL7 d3dMaterial7; // [esp+0h] [ebp-5Ch] BYREF
  float fNum; // [esp+44h] [ebp-18h]
  unsigned int i; // [esp+48h] [ebp-14h]
  CHanaMateAnimT3F7_S148 *pMateAnimT3F7; // [esp+4Ch] [ebp-10h] BYREF
  int v7; // [esp+50h] [ebp-Ch]
  BYTE *pBuf; // [esp+54h] [ebp-8h]
  int nCount_v10; // [esp+58h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseOdfMeshMANM_43FBD1\n");

  v7 = 0;
  nCount_v10 = 0;
  pBuf = (BYTE *)*ppBuf;
  nCount_v10 = *(_DWORD *)pBuf;
  pBuf += 4;
  if ( pVertT3EA->pMateT3EE )
  {
    pMateAnimT3F7 = 0;
    CHanaObj__NewMateAnimT3F7(&pMateAnimT3F7);
    pVertT3EA->pMateT3EE->pManmT3F7 = pMateAnimT3F7;
    HanaMateAnim_SetMaterial(pMateAnimT3F7, pVertT3EA->pMateT3EE);
    HanaAnimSet_AddMaterialAnimRef(pAnimSetT3F1, pMateAnimT3F7);
    for ( i = 0; i < nCount_v10; ++i )
    {
      fNum = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.ambient.a = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.ambient.r = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.ambient.g = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.ambient.b = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.diffuse.a = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.diffuse.r = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.diffuse.g = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.diffuse.b = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.emissive.a = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.emissive.r = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.emissive.g = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.emissive.b = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.specular.a = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.specular.r = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.specular.g = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.specular.b = *(float *)pBuf;
      pBuf += 4;
      d3dMaterial7.power = *(D3DVALUE *)pBuf;
      pBuf += 4;
      HanaMateAnim_InsertKeyframe(pMateAnimT3F7, fNum, &d3dMaterial7);
    }
    if ( pMateAnimT3F7 )
    {
      CHanaObj__Free_40C1E4(pMateAnimT3F7);
      pMateAnimT3F7 = 0;
    }
  }
  *ppBuf = (char *)pBuf;
}

void __cdecl hook_HanaMateAnim_SetMaterial(CHanaMateAnimT3F7_S148 *pMatAnim, CHanaMateT3EE_S184 *pMate)
{
  ONCELOG("[HNH:SUB] HanaMateAnim_SetMaterial_40E782\n")

  pMatAnim->pMateT3EE = pMate;
  CHanaObj__AddRef_40BFC3(pMate);
  qmemcpy(&pMatAnim->pKeyHead_13C->d3dMat7_4, &pMate->d3dMaterial_130, sizeof(pMatAnim->pKeyHead_13C->d3dMat7_4));
}

void __cdecl hook_CHanaObj__NewMateAnimT3F7(CHanaMateAnimT3F7_S148 **ppOut)
{
  CHanaMateAnimT3F7_S148 *pOut; // edi
  D3DCOLORVALUE *p_diffuse; // eax
  D3DCOLORVALUE *p_ambient; // ecx
  D3DCOLORVALUE *p_specular; // edx
  D3DCOLORVALUE *p_emissive; // eax

  ONCELOG("[HNH:SUB] CHanaObj__NewMateAnimT3F7_40E500\n");

  pOut = (CHanaMateAnimT3F7_S148 *)MemAlloc_40DEF7(0x148u);
  memset(pOut, 0, sizeof(CHanaMateAnimT3F7_S148));
  pOut->dwType = 0x3F7;
  pOut->nKeyCount_138 = 1;
  pOut->pKeyHead_13C = (CHanaMateAnimKeyT3F7_S50 *)MemAlloc_40DEF7(0x50u);
  memset(pOut->pKeyHead_13C, 0, sizeof(CHanaMateAnimKeyT3F7_S50));
  pOut->pKeyTail_140 = pOut->pKeyHead_13C;
  p_diffuse = &pOut->pKeyHead_13C->d3dMat7_4.diffuse;
  p_diffuse->r = 1.0;
  p_diffuse->g = 1.0;
  p_diffuse->b = 1.0;
  p_diffuse->a = 1.0;
  p_ambient = &pOut->pKeyHead_13C->d3dMat7_4.ambient;
  p_ambient->r = 0.0;
  p_ambient->g = 0.0;
  p_ambient->b = 0.0;
  p_ambient->a = 0.0;
  p_specular = &pOut->pKeyHead_13C->d3dMat7_4.specular;
  p_specular->r = 0.0;
  p_specular->g = 0.0;
  p_specular->b = 0.0;
  p_specular->a = 0.0;
  p_emissive = &pOut->pKeyHead_13C->d3dMat7_4.emissive;
  p_emissive->r = 0.0;
  p_emissive->g = 0.0;
  p_emissive->b = 0.0;
  p_emissive->a = 0.0;
  pOut->pKeyHead_13C->d3dMat7_4.power = 0.0;
  pOut->fLoopEnd_134 = 0.0;
  CHanaObj__AddRef_40BFC3(pOut);
  *ppOut = pOut;
}

void __cdecl hook_HanaAnimSet_AddMaterialAnimRef(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaMateAnimT3F7_S148 *pMatAnim)
{
  CHanaMateAnimT3F7_S148 **ppOldManmRefs; // [esp+14h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaAnimSet_AddMaterialAnimRef_4044D9\n");

  ++pAnimSet->nManmCount_140;
  if ( pAnimSet->pRefsManmT3F7 )
  {
    ppOldManmRefs = pAnimSet->pRefsManmT3F7;
    pAnimSet->pRefsManmT3F7 = (CHanaMateAnimT3F7_S148 **)MemAlloc_40DEF7(4 * pAnimSet->nManmCount_140);
    qmemcpy(pAnimSet->pRefsManmT3F7, ppOldManmRefs, 4 * ((4 * pAnimSet->nManmCount_140 - 4) >> 2));
    MemFree_40DF64(ppOldManmRefs);
  }
  else
  {
    pAnimSet->pRefsManmT3F7 = (CHanaMateAnimT3F7_S148 **)MemAlloc_40DEF7(4u);
  }
  pAnimSet->pRefsManmT3F7[pAnimSet->nManmCount_140 - 1] = pMatAnim;
  CHanaObj__AddRef_40BFC3(pMatAnim);
}

void __cdecl hook_ParseOdfMeshTEXT_43F460(char **ppBuf, CHanaVertT3EA_S1FC *pVertT3EA)
{
  int len; // eax
  int i; // [esp+0h] [ebp-468h]
  CHAR imageBmp[512]; // [esp+4h] [ebp-464h] BYREF
  CHanaTextureT3EB_S2CC *pHanaTexture_v2; // [esp+204h] [ebp-264h]
  int bSelfBlitMask; // [esp+208h] [ebp-260h]
  LPDIRECTDRAWSURFACE7 pDDMaskSurf; // [esp+20Ch] [ebp-25Ch]
  struct IDirectDrawSurface7 *pDDFinalSurf; // [esp+210h] [ebp-258h]
  int bSelfBlitImage; // [esp+214h] [ebp-254h]
  LPDIRECTDRAWSURFACE7 pDDImageSurf; // [esp+218h] [ebp-250h]
  CHanaTextureT3EB_S2CC *pHanaTexture_v1; // [esp+21Ch] [ebp-24Ch]
  LPBYTE pBuf; // [esp+220h] [ebp-248h]
  CHAR maskBmp[512]; // [esp+224h] [ebp-244h] BYREF
  CHAR origName[64]; // [esp+424h] [ebp-44h] BYREF
  int unk_v16; // [esp+464h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseOdfMeshTEXT_43F460\n");

  pBuf = (LPBYTE)*ppBuf;
  unk_v16 = 0;
  bSelfBlitImage = 0;
  bSelfBlitMask = 0;
  pDDImageSurf = 0;
  pDDMaskSurf = 0;
  pDDFinalSurf = 0;
  memset(imageBmp, 0, 0x100u);
  memset(maskBmp, 0, 0x100u);
  unk_v16 = *(_DWORD *)pBuf;
  pBuf += 4;
  wsprintfA(imageBmp, "%s", (const char *)pBuf);
  wsprintfA(origName, "%s", (const char *)pBuf);
  pHanaTexture_v1 = (CHanaTextureT3EB_S2CC *)FindObjByType_40BC13(imageBmp, 0x3EB);
  if ( pHanaTexture_v1 )
  {
    pHanaTexture_v2 = pHanaTexture_v1;
    HanaVert_SetTexture(pVertT3EA, pHanaTexture_v1);
    CHanaObj__Free_40C1E4(pHanaTexture_v1);
  }
  else
  {
    for ( i = lstrlenA(imageBmp) - 1; i; --i )
    {
      if ( !memcmp(&imageBmp[i], " & ", 3u) )
      {
        imageBmp[i] = 0;
        imageBmp[i + 1] = 0;
        imageBmp[i + 2] = 0;
        wsprintfA(maskBmp, "%s", &imageBmp[i + 3]);
        break;
      }
    }
    for ( i = lstrlenA(imageBmp); i; --i )
    {
      if ( !_strnicmp(&imageBmp[i], ".bmp", 4u) )
      {
        if ( imageBmp[i + 4] == 'N' )
          bSelfBlitImage = 1;
        imageBmp[i + 4] = 0;
        break;
      }
    }
    for ( i = lstrlenA(maskBmp); i; --i )
    {
      if ( !_strnicmp(&maskBmp[i], ".bmp", 4u) )
      {
        if ( maskBmp[i + 4] == 'N' )
          bSelfBlitMask = 1;
        maskBmp[i + 4] = 0;
        break;
      }
    }
    pDDImageSurf = GetSurfaceFromBmp_419DE5(imageBmp);
    if ( maskBmp[0] )
      pDDMaskSurf = GetSurfaceFromBmp_419DE5(maskBmp);
    if ( pDDImageSurf )
    {
      if ( bSelfBlitImage == 1 )
        SelfBltDC(pDDImageSurf);
      if ( bSelfBlitMask == 1 )
        SelfBltDC(pDDMaskSurf);
      pDDFinalSurf = CreateTextureFromSurfaceWithAlphaMask(pDDImageSurf, pDDMaskSurf);
      pHanaTexture_v2 = (CHanaTextureT3EB_S2CC *)MemAlloc_40DEF7(0x2CCu);
      memset(pHanaTexture_v2, 0, sizeof(CHanaTextureT3EB_S2CC));
      pHanaTexture_v2->dwType = 0x3EB;
      pHanaTexture_v2->ddSurfDesc.dwSize = 124;
      pHanaTexture_v2->ddSurfDesc.dwFlags = 4102;
      CHanaObj__AddRef_40BFC3(pHanaTexture_v2);
      AttachHanaObj_40BAB8(pHanaTexture_v2, origName);
      pHanaTexture_v2->pDDSurf = pDDFinalSurf;
      if ( pDDMaskSurf )
        pHanaTexture_v2->bHasMask = 1;
      HanaVert_SetTexture(pVertT3EA, pHanaTexture_v2);
      if ( pHanaTexture_v2 )
      {
        CHanaObj__Free_40C1E4(pHanaTexture_v2);
        pHanaTexture_v2 = 0;
      }
    }
    if ( pDDImageSurf )
      pDDImageSurf->Release();
    if ( pDDMaskSurf )
      pDDMaskSurf->Release();
    pDDImageSurf = 0;
    pDDMaskSurf = 0;
  }
  len = lstrlenA((LPCSTR)pBuf);
  pBuf += len + 1;
  *ppBuf = (char *)pBuf;
}

void __cdecl hook_HanaVert_SetTexture(CHanaVertT3EA_S1FC *pVert, CHanaTextureT3EB_S2CC *pTex)
{
  ONCELOG("[HNH:SUB] HanaVert_SetTexture_418103\n");

  if ( pVert->pTexT3EB )
    HanaVert_ReleaseTexture(pVert);
  pVert->pTexT3EB = pTex;
  if ( pTex )
    CHanaObj__AddRef_40BFC3(pTex);
}

void __cdecl hook_HanaVert_ReleaseTexture(CHanaVertT3EA_S1FC *pHanaT3EA)
{
  ONCELOG("[HNH:SUB] HanaVert_ReleaseTexture_4181B5\n");

  if ( pHanaT3EA->pTexT3EB )
  {
    CHanaObj__Free_40C1E4(pHanaT3EA->pTexT3EB);
    pHanaT3EA->pTexT3EB = 0;
  }
}

CHanaBase_S130 *__cdecl hook_FindObjByType_40BC13(char *szName, int dwType)
{
  int i; // [esp+0h] [ebp-8h]
  CObjItemRef_S208 *pObjItem; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindObjByType_40BC13\n");

  if ( !*szName )
    return 0;
  pObjItem = g_pObjectTable;
  for ( i = 0; i < g_nObjectTableCount; ++i )
  {
    if ( dwType == pObjItem->dwObjType_204 && pObjItem->szName_0[0] > 0 && !strcmp(szName, pObjItem->szName_0) )
    {
      CHanaObj__AddRef_40BFC3(pObjItem->pObj_200);
      return pObjItem->pObj_200;
    }
    ++pObjItem;
  }
  return 0;
}

void __cdecl hook_ParseOdfMeshVFLG_43FDEC(char **ppBuf, CHanaVertT3EA_S1FC *pHanaT3EA)
{
  WORD dwFlags_v3; // [esp+0h] [ebp-Ch]
  LPBYTE pBuf; // [esp+4h] [ebp-8h]
  LPBYTE pBuf2; // [esp+4h] [ebp-8h]
  LPBYTE Buf2b; // [esp+4h] [ebp-8h]
  LPBYTE Buf2c; // [esp+4h] [ebp-8h]
  int dwSize; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseOdfMeshVFLG_43FDEC\n");

  dwSize = *(DWORD *)*ppBuf;
  pBuf = (LPBYTE)(*ppBuf + 4);
  dwFlags_v3 = *(WORD *)pBuf;
  pHanaT3EA->flg_1D4 = (*(WORD *)pBuf & 2) != 0;
  pHanaT3EA->bFlatShade_1D8 = (dwFlags_v3 & 4) != 0;
  pHanaT3EA->flg_1DC = (dwFlags_v3 & 0x80) != 0;
  pHanaT3EA->bForceAlphaPass_1CC = (dwFlags_v3 & 0x100) != 0;
  if ( (dwFlags_v3 & 0x200) != 0 )
  {
    pHanaT3EA->nBlendMode_1D0 = 1;
  }
  else if ( (dwFlags_v3 & 0x400) != 0 )
  {
    pHanaT3EA->nBlendMode_1D0 = -1;
  }
  else
  {
    pHanaT3EA->nBlendMode_1D0 = 0;
  }
  pHanaT3EA->bShadowOnly_1E0 = (dwFlags_v3 & 0x2000) != 0;
  pHanaT3EA->bUnlit_1E4 = (dwFlags_v3 & 0x4000) != 0;
  pHanaT3EA->bAlphaPass2_1E8 = (dwFlags_v3 & 0x8000) != 0;
  pBuf2 = &pBuf[dwSize];
  if ( !memcmp("BIAS", pBuf2, 4u) )
  {
    Buf2b = pBuf2 + 4;
    pHanaT3EA->nZBias_1EC = *(unsigned __int16 *)Buf2b;
    pBuf2 = Buf2b + 2;
  }
  else
  {
    pHanaT3EA->nZBias_1EC = 0;
  }
  if ( !memcmp("ADRS", pBuf2, 4u) )
  {
    Buf2c = pBuf2 + 4;
    pHanaT3EA->nTexAddrMode_1F8 = (char)*Buf2c++;
    pHanaT3EA->bNoTexPersp_1F4 = (char)*Buf2c;
    pBuf2 = Buf2c + 1;
  }
  else
  {
    pHanaT3EA->nTexAddrMode_1F8 = 0;
    pHanaT3EA->bNoTexPersp_1F4 = 0;
  }
  if ( !memcmp("ALPR", pBuf2, 4u) )
  {
    pHanaT3EA->nAlphaPriority_1F8 = *((DWORD *)pBuf2 + 1);
    pBuf2 += 8;
  }
  else
  {
    pHanaT3EA->nAlphaPriority_1F8 = 0;
  }
  *ppBuf = (char *)pBuf2;
}

void register_ParseODFContent_441A5D() {
  ParseODFContent_441A5D = hook_ParseODFContent_441A5D;
  ParseOdfFRAM_44106A = hook_ParseOdfFRAM_44106A;
  ParseOdfMESH_44068C = hook_ParseOdfMESH_44068C;
  ParseOdfMORP_440896 = hook_ParseOdfMORP_440896;
  ParseOdfANIM_4414EB = hook_ParseOdfANIM_4414EB;
  ParseOdf3DON_44189C = hook_ParseOdf3DON_44189C;
  AssignLigthtToFrame_40539D = hook_AssignLigthtToFrame_40539D;
  CHanaObj__NewAnimT3F0 = hook_CHanaObj__NewAnimT3F0;
  HanaAnim_GetDuration = hook_HanaAnim_GetDuration;
  CHanaAnimT3F0__BakeKeyframesT3F0_402D0E = hook_CHanaAnimT3F0__BakeKeyframesT3F0_402D0E;
  HanaFrame_GetFrameName = hook_HanaFrame_GetFrameName;
  HanaAnim_LinkFrame = hook_HanaAnim_LinkFrame;
  InterpolateHanaAnimFrame = hook_InterpolateHanaAnimFrame;
  CHanaAnimSetT3F1__AddAnimT3F0_403C15 = hook_CHanaAnimSetT3F1__AddAnimT3F0_403C15;
  CHanaAnimSetT3F1__SetTime_403D61 = hook_CHanaAnimSetT3F1__SetTime_403D61;

  ParseVERT_440288 = hook_ParseVERT_440288;

  CHanaObj__NewMorpT457 = hook_CHanaObj__NewMorpT457;
  HanaMorp_AddVertexRef = hook_HanaMorp_AddVertexRef;
  HanaMorp_InitSubObject = hook_HanaMorp_InitSubObject;
  CalcFrameBound_404B80 = hook_CalcFrameBound_404B80;

  SetFramePosRel_4069DD = hook_SetFramePosRel_4069DD;
  ApplyQTS_4035B1 = hook_ApplyQTS_4035B1;
  ConfigureLight = hook_ConfigureLight;
  HanaFrame_GetCenterPosition_408C3E = hook_HanaFrame_GetCenterPosition_408C3E;
  GetSoundBufferAverageLevel = hook_GetSoundBufferAverageLevel;
  HanaAnim_FindInterpolationFrame = hook_HanaAnim_FindInterpolationFrame;
  FindMateAnimKeyframeBounds = hook_FindMateAnimKeyframeBounds;

  ApplyMateAnimMaterial = hook_ApplyMateAnimMaterial;

  CHanaObj__FillVertexBufferT3EA = hook_CHanaObj__FillVertexBufferT3EA;
  CHanaObj__NewVertT3EA = hook_CHanaObj__NewVertT3EA;
  GetVertByName_4400C3 = hook_GetVertByName_4400C3;
  ParseOdfMeshMATE_43F8FE = hook_ParseOdfMeshMATE_43F8FE;
  HanaVert_SetMaterial = hook_HanaVert_SetMaterial;
  FreeMate_41828C = hook_FreeMate_41828C;
  HanaMateAnim_InsertKeyframe = hook_HanaMateAnim_InsertKeyframe;
  ParseOdfMeshMANM_43FBD1 = hook_ParseOdfMeshMANM_43FBD1;
  HanaMateAnim_SetMaterial = hook_HanaMateAnim_SetMaterial;
  CHanaObj__NewMateAnimT3F7 = hook_CHanaObj__NewMateAnimT3F7;
  HanaAnimSet_AddMaterialAnimRef = hook_HanaAnimSet_AddMaterialAnimRef;
  ParseOdfMeshTEXT_43F460 = hook_ParseOdfMeshTEXT_43F460;
  HanaVert_SetTexture = hook_HanaVert_SetTexture;
  HanaVert_ReleaseTexture = hook_HanaVert_ReleaseTexture;
  FindObjByType_40BC13 = hook_FindObjByType_40BC13;
  ParseOdfMeshVFLG_43FDEC = hook_ParseOdfMeshVFLG_43FDEC;

  hnh_HookOff();
    hnh_Hook(0x4069DD, hook_SetFramePosRel_4069DD, "SetFramePosRel_4069DD"); // [OK]


    hnh_Hook(0x44106A, hook_ParseOdfFRAM_44106A, "ParseOdfFRAM_44106A"); // [OK]
    hnh_Hook(0x4414EB, hook_ParseOdfANIM_4414EB, "ParseOdfANIM_4414EB"); // [OK]
    hnh_Hook(0x44068C, hook_ParseOdfMESH_44068C, "ParseOdfMESH_44068C"); // [OK]
    hnh_Hook(0x440896, hook_ParseOdfMORP_440896, "ParseOdfMORP_440896"); // [OK]
    hnh_Hook(0x44189C, hook_ParseOdf3DON_44189C, "ParseOdf3DON_44189C"); // [OK]
    hnh_Hook(0x440288, hook_ParseVERT_440288, "ParseVERT_440288"); // [OK]
    hnh_Hook(0x43F8FE, hook_ParseOdfMeshMATE_43F8FE, "ParseOdfMeshMATE_43F8FE"); // [OK]
    hnh_Hook(0x43FDEC, hook_ParseOdfMeshVFLG_43FDEC, "ParseOdfMeshVFLG_43FDEC"); // [OK]
    hnh_Hook(0x43FBD1, hook_ParseOdfMeshMANM_43FBD1, "ParseOdfMeshMANM_43FBD1"); // [OK]
    hnh_Hook(0x43F460, hook_ParseOdfMeshTEXT_43F460, "ParseOdfMeshTEXT_43F460"); // [OK]

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
    hnh_Hook(0x403D61, hook_CHanaAnimSetT3F1__SetTime_403D61, "CHanaAnimSetT3F1__SetTime_403D61"); // [OK]
    hnh_Hook(0x40EF23, hook_FindMateAnimKeyframeBounds, "FindMateAnimKeyframeBounds_40EF23"); // [OK]
    hnh_Hook(0x40E969, hook_ApplyMateAnimMaterial, "ApplyMateAnimMaterial_40E969"); // [OK]
    hnh_Hook(0x40283C, hook_InterpolateHanaAnimFrame, "InterpolateHanaAnimFrame_40283C"); // [OK]
    hnh_Hook(0x40344A, hook_HanaAnim_FindInterpolationFrame, "HanaAnim_FindInterpolationFrame_40344A"); // [OK]
    hnh_Hook(0x439982, hook_HanaMorp_InitSubObject, "HanaMorp_InitSubObject_439982"); // [OK]
    hnh_Hook(0x434370, hook_HanaMorp_AddVertexRef, "HanaMorp_AddVertexRef_434370"); // [OK]
    hnh_Hook(0x433DA7, hook_CHanaObj__NewMorpT457, "CHanaObj__NewMorpT457_433DA7"); // [OK]
    hnh_Hook(0x4400C3, hook_GetVertByName_4400C3, "GetVertByName_4400C3"); // [OK]
    hnh_Hook(0x416CE0, hook_CHanaObj__NewVertT3EA, "CHanaObj__NewVertT3EA_416CE0"); // [OK]
    hnh_Hook(0x416E65, hook_CHanaObj__FillVertexBufferT3EA, "CHanaObj__FillVertexBufferT3EA_416E65"); // [OK]
    hnh_Hook(0x418221, hook_HanaVert_SetMaterial, "HanaVert_SetMaterial_418221"); // [OK]
    hnh_Hook(0x41828C, hook_FreeMate_41828C, "FreeMate_41828C"); // [OK]
    hnh_Hook(0x40E7DA, hook_HanaMateAnim_InsertKeyframe, "HanaMateAnim_InsertKeyframe_40E7DA"); // [OK]
    hnh_Hook(0x40E500, hook_CHanaObj__NewMateAnimT3F7, "CHanaObj__NewMateAnimT3F7_40E500"); // [OK]
    hnh_Hook(0x40E782, hook_HanaMateAnim_SetMaterial, "HanaMateAnim_SetMaterial_40E782"); // [OK]
    hnh_Hook(0x4044D9, hook_HanaAnimSet_AddMaterialAnimRef, "HanaAnimSet_AddMaterialAnimRef_4044D9"); // [OK]
    hnh_Hook(0x418103, hook_HanaVert_SetTexture, "HanaVert_SetTexture_418103"); // [OK]
    hnh_Hook(0x4181B5, hook_HanaVert_ReleaseTexture, "HanaVert_ReleaseTexture_4181B5"); // [OK]
    hnh_Hook(0x40BC13, hook_FindObjByType_40BC13, "FindObjByType_40BC13"); // [OK]
  hnh_HookOn();
}
