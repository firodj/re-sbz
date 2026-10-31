#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"
#include "struct.hpp"

// === GLOBAL VARIABLES ===

uint32_t &g_nRefLightAnimSetT3FA = *(uint32_t *)0x4D6D4C; // WRITABLE
uint32_t &g_nRefLightAnimT3F9 = *(uint32_t *)0x4D6D48; // WRITABLE
uint32_t &g_nRefMateAnimSetT3F8 = *(uint32_t *)0x4D6D44; // WRITABLE
uint32_t &g_nRefMateAnimT3F7 = *(uint32_t *)0x4D6D40; // WRITABLE
uint32_t &g_nRefSpriteT3F6 = *(uint32_t *)0x4D6D3C; // WRITABLE
uint32_t &g_nRefMeshT3F5 = *(uint32_t *)0x4D6D14; // WRITABLE
uint32_t &g_nRefTexCoordT3F2 = *(uint32_t *)0x4D6D38; // WRITABLE
uint32_t &g_nRefTextureT3EB = *(uint32_t *)0x4D6D30; // WRITABLE
uint32_t &g_nRefVertT3EA = *(uint32_t *)0x4D6D2C; // WRITABLE
uint32_t &g_nRefFrameT3EC = *(uint32_t *)0x4D6D24; // WRITABLE
uint32_t &g_nRefFrameArrayT3ED = *(uint32_t *)0x4D6D34; // WRITABLE
uint32_t &g_nRefMateT3EE = *(uint32_t *)0x4D6D28; // WRITABLE
uint32_t &g_nRefLightT3EF = *(uint32_t *)0x4D6D18; // WRITABLE
uint32_t &g_nRefAnimT3F0 = *(uint32_t *)0x4D6D1C; // WRITABLE
uint32_t &g_nRefAnimSetT3F1 = *(uint32_t *)0x4D6D20; // WRITABLE
uint32_t &g_nRefMorpT457 = *(uint32_t *)0x4D6D50; // WRITABLE

uint32_t &g_nLightningEnabled = *(uint32_t *)0x4B00AC; // WRITABLE
uint32_t &g_bForceDirectionalLights = *(uint32_t *)0x4D6D04; // WRITABLE

CHanaTextureT3EB_S2CC* &g_pCurTexObj = *(CHanaTextureT3EB_S2CC**)0x4D6CA0;

__int64 &g_bitsLightIndexes_4D6CB8 = *(__int64*)0x4D6CB8;

// === GLOBAL FUNCTIONS ===

void (__cdecl *CHanaObj__FreeT3FB)(CHanaBase_S130 *pObj) = reinterpret_cast<void (__cdecl *)(CHanaBase_S130 *)>(0x4166B5);
void (__cdecl *CHanaObj__FreeT3FC)(CHanaBase_S130 *pObj) = reinterpret_cast<void (__cdecl *)(CHanaBase_S130 *)>(0x410501);
void (__cdecl *CHanaObj__FreeT3FD)(CHanaBase_S130 *pObj) = reinterpret_cast<void (__cdecl *)(CHanaBase_S130 *)>(0x4114DF);
void (__cdecl *CHanaObj__FreeT3FE)(CHanaBase_S130 *pObj) = reinterpret_cast<void (__cdecl *)(CHanaBase_S130 *)>(0x413075);
void (__cdecl * CHanaObj__RemoveFromGlobalRef)(CHanaBase_S130 *pObj) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130 *pObj)>(0x40BD4D);
void (__cdecl * CHanaObj__FreeVertT3EA)(CHanaVertT3EA_S1FC *pVert) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVert)>(0x417A0C);
void (__cdecl * CHanaObj__FreeTextureT3EB)(CHanaTextureT3EB_S2CC *pTexture) = reinterpret_cast<void(__cdecl*)(CHanaTextureT3EB_S2CC *pTexture)>(0x416256);
void (__cdecl * Frame_FreeAttachedObject)(CHanaFrameT3EC_S354 *pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x40536D);
void (__cdecl * Frame_FreeAttachedLight)(CHanaFrameT3EC_S354 *pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x405421);
void (__cdecl * RemoveChild_4058C3)(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild) = reinterpret_cast<void(__cdecl*)(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild)>(0x4058C3);
void (__cdecl * CHanaObj__FreeFrameT3EC)(CHanaFrameT3EC_S354 *pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x4054B3);
void (__cdecl * CHanaObj__FreeMateT3EE)(CHanaMateT3EE_S184 *pMate) = reinterpret_cast<void(__cdecl*)(CHanaMateT3EE_S184 *pMate)>(0x40FC5E);
void (__cdecl * CHanaObj__FreeLightT3EF)(CHanaLightT3EF_S1A0 *pLight) = reinterpret_cast<void(__cdecl*)(CHanaLightT3EF_S1A0 *pLight)>(0x409437);
void (__cdecl * CHanaObj__FreeFrameArrayT3ED)(CHanaFrameArrayT3ED_S138 *pFrameArrayT3ED) = reinterpret_cast<void(__cdecl*)(CHanaFrameArrayT3ED_S138 *pFrameArrayT3ED)>(0x408E8D);
void (__cdecl * UnlinkHanaAnimFromFrame)(CHanaAnimT3F0_S240 *pAnim) = reinterpret_cast<void(__cdecl*)(CHanaAnimT3F0_S240 *pAnim)>(0x401EDF);
void (__cdecl * CHanaObj__FreeAnimT3F0)(CHanaAnimT3F0_S240 *pAnim) = reinterpret_cast<void(__cdecl*)(CHanaAnimT3F0_S240 *pAnim)>(0x401F53);
void (__cdecl * FreeSoundStruct_422C72)(CSoundStruct_8 *pSound) = reinterpret_cast<void(__cdecl*)(CSoundStruct_8 *pSound)>(0x422C72);
void (__cdecl * CHanaObj__FreeAnimSetT3F1)(CHanaAnimSetT3F1_S150 *pAnimSet) = reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet)>(0x403A55);
void (__cdecl * CHanaObj__FreeTexCoordT3F2)(CHanaTextureCoordsT3F2_S140 *pTexCoord) = reinterpret_cast<void(__cdecl*)(CHanaTextureCoordsT3F2_S140 *pTexCoord)>(0x415E05);
void (__cdecl * CHanaObj__FreeTexAnimT3F3)(CHanaTextureAnimT3F3_S16C *pTexAnim) = reinterpret_cast<void(__cdecl*)(CHanaTextureAnimT3F3_S16C *pTexAnim)>(0x414F62);
void (__cdecl * CHanaObj__FreeTexAnimSetT3F4)(CHanaTextureAnimSetT3F4_S138 *pTexAnimSet) = reinterpret_cast<void(__cdecl*)(CHanaTextureAnimSetT3F4_S138 *pTexAnimSet)>(0x41595B);
void (__cdecl * CHanaObj__FreeMeshT3F5)(CHanaMeshT3F5_S138 *pMesh) = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 *pMesh)>(0x40FDCB);
void (__cdecl * CHanaObj__FreeSpriteT3F6)(CHanaSpriteT3F6_S1FC *pSprite) = reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite)>(0x413F07);
void (__cdecl * ApplyMaterial)(CHanaMateT3EE_S184 *pMate, D3DMATERIAL7 *pD3Dmaterial) = reinterpret_cast<void(__cdecl*)(CHanaMateT3EE_S184 *pMate, D3DMATERIAL7 *pD3Dmaterial)>(0x40F891);
void (__cdecl * CHanaObj__FreeMateAnimT3F7)(CHanaMateAnimT3F7_S148 *pMateAnim) = reinterpret_cast<void(__cdecl*)(CHanaMateAnimT3F7_S148 *pMateAnim)>(0x40E6B6);
void (__cdecl * CHanaObj__FreeMateAnimSetT3F8)(CHanaMateAnimSetT3F8_S140 *pMateAnimSet) = reinterpret_cast<void(__cdecl*)(CHanaMateAnimSetT3F8_S140 *pMateAnimSet)>(0x40F3E5);
void (__cdecl * ApplyLightSettings)(CHanaLightT3EF_S1A0 *pLight) = reinterpret_cast<void(__cdecl*)(CHanaLightT3EF_S1A0 *pLight)>(0x40929C);
void (__cdecl * SetupLightFromD3D)(CHanaLightT3EF_S1A0 *pHanaLight, D3DLIGHT7 *pD3DLight) = reinterpret_cast<void(__cdecl*)(CHanaLightT3EF_S1A0 *pHanaLight, D3DLIGHT7 *pD3DLight)>(0x40922E);
void (__cdecl * CHanaObj__FreeLightAnimT3F9)(CHanaLightAnimT3F9_S144 *pLightAnim) = reinterpret_cast<void(__cdecl*)(CHanaLightAnimT3F9_S144 *pLightAnim)>(0x409820);
void (__cdecl * CHanaObj__FreeLightAnimSetT3FA)(CHanaLightAnimSetT3FA_S13C *pLightAnimSet) = reinterpret_cast<void(__cdecl*)(CHanaLightAnimSetT3FA_S13C *pLightAnimSet)>(0x40A3BB);
void (__cdecl * CHanaObj__FreeMorpT457)(CHanaMorpT457_S40C *pMorp) = reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *pMorp)>(0x433E8D);
void (__cdecl * FreeSekHeader)(CSekHeader *pSekHeader) = reinterpret_cast<void(__cdecl*)(CSekHeader *pSekHeader)>(0x40D8F6);
void (__cdecl * CSekBody__Release_4012FE)(CSekBody *pSekBody) = reinterpret_cast<void(__cdecl*)(CSekBody *pSekBody)>(0x4012FE);
void (__cdecl * CHanaObj__FreeSceneT458)(CHanaSceneT458_S504 *pScene) = reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *pScene)>(0x4821FA);
void (__cdecl * CHanaObj__Free_40C1E4)(CHanaBase_S130 *pObj) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130 *pObj)>(0x40C1E4);
void (__cdecl * CHanaObj__GetRealPath_486D9A)(LPSTR szPathOut, int nPathCode) = reinterpret_cast<void(__cdecl*)(LPSTR szPathOut, int nPathCode)>(0x486D9A);
void (__cdecl * CHanaObj__AddRef_40BFC3)(CHanaBase_S130 *pObj) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130 *pObj)>(0x40BFC3);
 
// === FUNCTION IMPLEMENTATIONS ===
void __cdecl hook_CHanaObj__RemoveFromGlobalRef(CHanaBase_S130 *pObj)
{
  ONCELOG("[HNH:SUB] CHanaObj__RemoveFromGlobalRef_40BD4D\n");

  if ( pObj->nIndex_108 > 0 )
  {
    g_pObjectTable[pObj->nIndex_108].szName_0[0] = 0;
    g_pObjectTable[pObj->nIndex_108].pObj_200 = 0;
    g_pObjectTable[pObj->nIndex_108].dwObjType_204 = 0;
  }
}

void __cdecl hook_CHanaObj__FreeVertT3EA(CHanaVertT3EA_S1FC *pVert)
{
  int v1; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeVertT3EA_417A0C\n");

  if ( pVert->nCount_12C <= 1 )
  {
    if ( pVert->pMateT3EE )
    {
      CHanaObj__Free_40C1E4(pVert->pMateT3EE);
      pVert->pMateT3EE = 0;
    }
    if ( pVert->pTexT3EB )
    {
      CHanaObj__Free_40C1E4(pVert->pTexT3EB);
      pVert->pTexT3EB = 0;
    }
    if ( pVert->pVtxIndices )
    {
      MemFree_40DF64(pVert->pVtxIndices);
      pVert->pVtxIndices = 0;
    }
    v1 = pVert->pD3DVtxBuf_130->Release();
    if ( v1 < 0 )
      PrintDebugError(v1);
    if ( pVert->pUnk_164 )
      pVert->pUnk_164->Release();
    CHanaObj__RemoveFromGlobalRef(pVert);
    MemFree_40DF64(pVert);
  }
  else
  {
    --pVert->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeTextureT3EB(CHanaTextureT3EB_S2CC *pTexture)
{
  int v1; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeTextureT3EB_416256\n");

  if ( pTexture->nCount_12C <= 1 )
  {
    if ( g_pD3DDevice_4D6C78 )
      g_pD3DDevice_4D6C78->SetTexture( 0, 0);
    g_pCurTexObj = 0;
    if ( pTexture->pDDSurf )
    {
      v1 = pTexture->pDDSurf->Release();
      if ( v1 < 0 )
        PrintDebugError(v1);
    }
    CHanaObj__RemoveFromGlobalRef(pTexture);
    MemFree_40DF64(pTexture);
  }
  else
  {
    --pTexture->nCount_12C;
  }
}

void __cdecl hook_Frame_FreeAttachedObject(CHanaFrameT3EC_S354 *pFrame)
{
  ONCELOG("[HNH:SUB] Frame_FreeAttachedObject_40536D\n");
 
  if ( pFrame->pAttachedObj_20C )
  {
    CHanaObj__Free_40C1E4(pFrame->pAttachedObj_20C);
    pFrame->pAttachedObj_20C = 0;
  }
}

void __cdecl hook_Frame_FreeAttachedLight(CHanaFrameT3EC_S354 *pFrame)
{
  ONCELOG("[HNH:SUB] Frame_FreeAttachedLight_405421\n");

  if ( pFrame->pHanaLight_210 )
  {
    pFrame->pHanaLight_210->pHanaT3EC_19C = 0;
    CHanaObj__Free_40C1E4(pFrame->pHanaLight_210);
    pFrame->pHanaLight_210 = 0;
    pFrame->bHasLight_214 = 0;
  }
}

void __cdecl hook_RemoveChild_4058C3(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild)
{
  CHanaNode *pPrev; // [esp+0h] [ebp-Ch]
  CHanaNode *pNext; // [esp+0h] [ebp-Ch]
  unsigned int i; // [esp+4h] [ebp-8h]
  CHanaNode *pNode; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] RemoveChild_4058C3\n");

  if ( pParent->nCountChild )
  {
    pNode = pParent->pFirstChildNode;
    for ( i = 0; i < pParent->nCountChild; ++i )
    {
      if ( pNode->pData == pChild )
      {
        if ( pNode->pPrev )
        {
          pPrev = pNode->pPrev;
          pPrev->pNext = pNode->pNext;
          if ( !pPrev->pNext )
            pParent->pLastChildNode = pPrev;
        }
        if ( pNode->pNext )
        {
          pNext = pNode->pNext;
          pNext->pPrev = pNode->pPrev;
          if ( !pNext->pPrev )
            pParent->pFirstChildNode = pNext;
        }
        if ( pNode == pParent->pLastChildNode )
          pParent->pLastChildNode = pNode->pPrev;
        MemFree_40DF64(pNode);
        if ( !--pParent->nCountChild )
        {
          pParent->pLastChildNode = 0;
          pParent->pFirstChildNode = 0;
        }
        pChild->pParent = 0;
        return;
      }
      pNode = pNode->pNext;
    }
  }
}

void __cdecl hook_CHanaObj__FreeFrameT3EC(CHanaFrameT3EC_S354 *pFrame)
{
  CHanaNode *pNext; // [esp+0h] [ebp-14h]
  unsigned int i; // [esp+4h] [ebp-10h]
  CHanaNode *pFirstChildNode; // [esp+8h] [ebp-Ch]
  unsigned int nCountChild; // [esp+Ch] [ebp-8h]
  CHanaAnimT3F0_S240 *pHanaAnim_2C0; // [esp+10h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeFrameT3EC_4054B3\n");

  if ( pFrame->nCount_12C <= 1 )
  {
    pHanaAnim_2C0 = pFrame->pHanaAnim_2C0;
    if ( pHanaAnim_2C0 )
      pHanaAnim_2C0->pHanaFram_130 = 0;
    pFrame->pHanaAnim_2C0 = 0;
    if ( pFrame->pAttachedObj_20C )
      Frame_FreeAttachedObject(pFrame);
    if ( pFrame->pHanaLight_210 )
      Frame_FreeAttachedLight(pFrame);
    if ( pFrame->nCountChild )
    {
      pFirstChildNode = pFrame->pFirstChildNode;
      nCountChild = pFrame->nCountChild;
      for ( i = 0; i < nCountChild; ++i )
      {
        pNext = pFirstChildNode->pNext;
        CHanaObj__Free_40C1E4(pFirstChildNode->pData);
        pFirstChildNode = pNext;
      }
      pFrame->pLastChildNode = 0;
      pFrame->pFirstChildNode = 0;
      pFrame->nCountChild = 0;
  }
    if ( pFrame->pParent )
      RemoveChild_4058C3(pFrame->pParent, pFrame);
    CHanaObj__RemoveFromGlobalRef(pFrame);
    MemFree_40DF64(pFrame);
  }
  else
  {
    --pFrame->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeMateT3EE(CHanaMateT3EE_S184 *pMate)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeMateT3EE_40FC5E\n");

  if ( pMate->nCount_12C <= 1 )
  {
    CHanaObj__RemoveFromGlobalRef(pMate);
    MemFree_40DF64(pMate);
  }
  else
  {
    --pMate->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeLightT3EF(CHanaLightT3EF_S1A0 *pLight)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeLightT3EF_409437\n");

  if ( pLight->nCount_12C <= 1 )
  {
    g_bitsLightIndexes_4D6CB8 = (1LL << pLight->dwLightIndex_130 == 0) & g_bitsLightIndexes_4D6CB8;
    g_pD3DDevice_4D6C78->SetLight(pLight->dwLightIndex_130, 0);
    g_pD3DDevice_4D6C78->LightEnable(pLight->dwLightIndex_130, 0);
    CHanaObj__RemoveFromGlobalRef(pLight);
    MemFree_40DF64(pLight);
  }
  else
  {
    --pLight->nCount_12C;
  }
}

// Idiomatic with IUnknown::Release() - decrements nCount_12C; at <= 1 frees arrFrames, unlinks global ref, frees object
void __cdecl hook_CHanaObj__FreeFrameArrayT3ED(CHanaFrameArrayT3ED_S138 *pFrameArrayT3ED)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeFrameArrayT3ED_408E8D\n");

  if ( pFrameArrayT3ED )
  {
    if ( pFrameArrayT3ED->nCount_12C <= 1 )
    {
      if ( pFrameArrayT3ED->arrFrames )
      {
        MemFree_40DF64(pFrameArrayT3ED->arrFrames);
        pFrameArrayT3ED->arrFrames = 0;
      }
      CHanaObj__RemoveFromGlobalRef(pFrameArrayT3ED);
      MemFree_40DF64(pFrameArrayT3ED);
    }
    else
    {
      --pFrameArrayT3ED->nCount_12C;
    }
  }
}

void __cdecl hook_UnlinkHanaAnimFromFrame(CHanaAnimT3F0_S240 *pAnim)
{
  CHanaFrameT3EC_S354 *pFrame; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  CObjItemRef_S208 *pObjIt; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] UnlinkHanaAnimFromFrame_401EDF\n");

  pObjIt = g_pObjectTable;
  for ( i = 0; i < g_nObjectTableCount; ++i )
  {
    if ( pObjIt->dwObjType_204 == 0x3EC )
    {
      pFrame = (CHanaFrameT3EC_S354 *)pObjIt->pObj_200;
      if ( pAnim == pFrame->pHanaAnim_2C0 )
      {
        pFrame->pHanaAnim_2C0 = 0;
        return;
      }
    }
    ++pObjIt;
  }
}

void __cdecl hook_CHanaObj__FreeAnimT3F0(CHanaAnimT3F0_S240 *pAnim)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeAnimT3F0_401F53\n");

  if ( pAnim->nCount_12C <= 1 )
  {
    UnlinkHanaAnimFromFrame(pAnim);
    if ( pAnim->arrKeys_13C )
      MemFree_40DF64(pAnim->arrKeys_13C);
    CHanaObj__RemoveFromGlobalRef(pAnim);
    MemFree_40DF64(pAnim);
  }
  else
  {
    --pAnim->nCount_12C;
  }
}

void __cdecl hook_FreeSoundStruct_422C72(CSoundStruct_8 *pSound)
{
  ONCELOG("[HNH:SUB] FreeSoundStruct_422C72\n");

  if ( g_bDSInited_5185C4 && pSound )
  {
    if ( pSound->pDS3DBuffer_4 )
      pSound->pDS3DBuffer_4->Release();
    if ( pSound->pDSBuffer_0 )
    {
      pSound->pDSBuffer_0->Stop();
      pSound->pDSBuffer_0->Release();
    }
    GlobalFree(pSound);
  }
}

void __cdecl hook_CHanaObj__FreeAnimSetT3F1(CHanaAnimSetT3F1_S150 *pAnimSet)
{
  unsigned int k; // [esp+0h] [ebp-Ch]
  int j; // [esp+4h] [ebp-8h]
  unsigned int i; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeAnimSetT3F1_403A55\n");

  if ( pAnimSet->nCount_12C <= 1 )
  {
    if ( pAnimSet->arroS118_14C )
    {
      for ( i = 0; i < pAnimSet->nCount_148; ++i )
      {
        if ( pAnimSet->arroS118_14C[i].pFrame_114 )
          CHanaObj__Free_40C1E4(pAnimSet->arroS118_14C[i].pFrame_114);
        if ( pAnimSet->arroS118_14C[i].pSnd_0 )
          FreeSoundStruct_422C72(pAnimSet->arroS118_14C[i].pSnd_0);
      }
      MemFree_40DF64(pAnimSet->arroS118_14C);
    }
    if ( pAnimSet->refsAnimT3F0_138 )
    {
      for ( j = 0; j < pAnimSet->nAnimCount_134; ++j )
        CHanaObj__Free_40C1E4(pAnimSet->refsAnimT3F0_138[j]);
      MemFree_40DF64(pAnimSet->refsAnimT3F0_138);
      pAnimSet->refsAnimT3F0_138 = 0;
    }
    if ( pAnimSet->pRefsManmT3F7 )
    {
      for ( k = 0; k < pAnimSet->nManmCount_140; ++k )
        CHanaObj__Free_40C1E4(pAnimSet->pRefsManmT3F7[k]);
      MemFree_40DF64(pAnimSet->pRefsManmT3F7);
    }
    CHanaObj__RemoveFromGlobalRef(pAnimSet);
    MemFree_40DF64(pAnimSet);
  }
  else
  {
    --pAnimSet->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeTexCoordT3F2(CHanaTextureCoordsT3F2_S140 *pTexCoord)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeTexCoordT3F2_415E05\n");

  if ( pTexCoord->tvs_138 )
    MemFree_40DF64(pTexCoord->tvs_138);
  if ( pTexCoord->tus_134 )
    MemFree_40DF64(pTexCoord->tus_134);
  MemFree_40DF64(pTexCoord);
}

void __cdecl hook_CHanaObj__FreeTexAnimT3F3(CHanaTextureAnimT3F3_S16C *pTexAnim)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeTexAnimT3F3_414F62\n");

  if ( pTexAnim->nCount_12C <= 1 )
  {
    if ( pTexAnim->pTexCoords )
    {
      for ( i = 0; i < pTexAnim->dwCount_140; ++i )
        CHanaObj__Free_40C1E4(pTexAnim->pTexCoords[i]);
      MemFree_40DF64(pTexAnim->pTexCoords);
    }
    CHanaObj__RemoveFromGlobalRef(pTexAnim);
    MemFree_40DF64(pTexAnim);
  }
  else
  {
    --pTexAnim->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeTexAnimSetT3F4(CHanaTextureAnimSetT3F4_S138 *pTexAnimSet)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeTexAnimSetT3F4_41595B\n");

  if ( pTexAnimSet->nCount_12C <= 1 )
  {
    if ( pTexAnimSet->pArrObjs_134 )
    {
      for ( i = 0; i < pTexAnimSet->nCount_130; ++i )
        CHanaObj__Free_40C1E4(pTexAnimSet->pArrObjs_134[i]);
      MemFree_40DF64(pTexAnimSet->pArrObjs_134);
    }
    CHanaObj__RemoveFromGlobalRef(pTexAnimSet);
    MemFree_40DF64(pTexAnimSet);
  }
  else
  {
    --pTexAnimSet->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeMeshT3F5(CHanaMeshT3F5_S138 *pMesh)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeMeshT3F5_40FDCB\n");

  if ( pMesh->nCount_12C <= 1 )
  {
    if ( pMesh->refsVertT3EA )
    {
      for ( i = 0; i < pMesh->nCountVertT3EA_130; ++i )
      {
        if ( pMesh->refsVertT3EA[i] )
        {
          CHanaObj__Free_40C1E4(pMesh->refsVertT3EA[i]);
          pMesh->refsVertT3EA[i] = 0;
        }
      }
      MemFree_40DF64(pMesh->refsVertT3EA);
    }
    CHanaObj__RemoveFromGlobalRef(pMesh);
    MemFree_40DF64(pMesh);
  }
  else
  {
    --pMesh->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeSpriteT3F6(CHanaSpriteT3F6_S1FC *pSprite)
{
  ONCELOG("[HNH:SUB] CHanaObj__FreeSpriteT3F6_413F07\n");

  if ( pSprite->base_0.nCount_12C <= 1 )
  {
    if ( pSprite->pTexture_134 )
      pSprite->pTexture_134->Release();
    CHanaObj__RemoveFromGlobalRef(&pSprite->base_0);
    MemFree_40DF64(pSprite);
  }
  else
  {
    --pSprite->base_0.nCount_12C;
  }
}

void __cdecl hook_ApplyMaterial(CHanaMateT3EE_S184 *pMate, D3DMATERIAL7 *pD3Dmaterial)
{
  ONCELOG("[HNH:SUB] ApplyMaterial_40F891\n");

  qmemcpy(&pMate->d3dMaterial_130, pD3Dmaterial, sizeof(pMate->d3dMaterial_130));
  if ( pMate->d3dMaterial_130.diffuse.a > 1.0 )
    pMate->d3dMaterial_130.diffuse.a = 1.0;
  if ( pMate->d3dMaterial_130.diffuse.a < 0.0 )
    pMate->d3dMaterial_130.diffuse.a = 0.0;
  if ( pMate->d3dMaterial_130.diffuse.r > 1.0 )
    pMate->d3dMaterial_130.diffuse.r = 1.0;
  if ( pMate->d3dMaterial_130.diffuse.r < 0.0 )
    pMate->d3dMaterial_130.diffuse.r = 0.0;
  if ( pMate->d3dMaterial_130.diffuse.g > 1.0 )
    pMate->d3dMaterial_130.diffuse.g = 1.0;
  if ( pMate->d3dMaterial_130.diffuse.g < 0.0 )
    pMate->d3dMaterial_130.diffuse.g = 0.0;
  if ( pMate->d3dMaterial_130.diffuse.b > 1.0 )
    pMate->d3dMaterial_130.diffuse.b = 1.0;
  if ( pMate->d3dMaterial_130.diffuse.b < 0.0 )
    pMate->d3dMaterial_130.diffuse.b = 0.0;
  if ( pMate->d3dMaterial_130.ambient.a > 1.0 )
    pMate->d3dMaterial_130.ambient.a = 1.0;
  if ( pMate->d3dMaterial_130.ambient.a < 0.0 )
    pMate->d3dMaterial_130.ambient.a = 0.0;
  if ( pMate->d3dMaterial_130.ambient.r > 1.0 )
    pMate->d3dMaterial_130.ambient.r = 1.0;
  if ( pMate->d3dMaterial_130.ambient.r < 0.0 )
    pMate->d3dMaterial_130.ambient.r = 0.0;
  if ( pMate->d3dMaterial_130.ambient.g > 1.0 )
    pMate->d3dMaterial_130.ambient.g = 1.0;
  if ( pMate->d3dMaterial_130.ambient.g < 0.0 )
    pMate->d3dMaterial_130.ambient.g = 0.0;
  if ( pMate->d3dMaterial_130.ambient.b > 1.0 )
    pMate->d3dMaterial_130.ambient.b = 1.0;
  if ( pMate->d3dMaterial_130.ambient.b < 0.0 )
    pMate->d3dMaterial_130.ambient.b = 0.0;
  pMate->fMatDirty = 1;
}

void __cdecl hook_CHanaObj__FreeMateAnimT3F7(CHanaMateAnimT3F7_S148 *pMateAnim)
{
  CHanaSubObj_S50 *psoS50_4C; // [esp+0h] [ebp-8h]
  CHanaSubObj_S50 *hMem; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeMateAnimT3F7_40E6B6\n");

  if ( pMateAnim->nCount_12C <= 1 )
  {
    if ( pMateAnim->pObj_S50 )
    {
      hMem = pMateAnim->pObj_S50;
      ApplyMaterial(pMateAnim->pMateT3EE, &hMem->d3dMat7_4);
      CHanaObj__Free_40C1E4(pMateAnim->pMateT3EE);
      pMateAnim->pMateT3EE = 0;
      while ( 1 )
      {
        psoS50_4C = hMem->psoS50_4C;
        MemFree_40DF64(hMem);
        if ( !psoS50_4C )
          break;
        hMem = psoS50_4C;
      }
    }
    CHanaObj__RemoveFromGlobalRef(pMateAnim);
    MemFree_40DF64(pMateAnim);
  }
  else
  {
    --pMateAnim->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeMateAnimSetT3F8(CHanaMateAnimSetT3F8_S140 *pMateAnimSet)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeMateAnimSetT3F8_40F3E5\n");

  if ( pMateAnimSet->nCount_12C <= 1 )
  {
    if ( pMateAnimSet->pArrObjs_138 )
    {
      for ( i = 0; i < pMateAnimSet->dwObjCount_134; ++i )
        CHanaObj__Free_40C1E4(pMateAnimSet->pArrObjs_138[i]);
      MemFree_40DF64(pMateAnimSet->pArrObjs_138);
    }
    CHanaObj__RemoveFromGlobalRef(pMateAnimSet);
    MemFree_40DF64(pMateAnimSet);
  }
  else
  {
    --pMateAnimSet->nCount_12C;
  }
}

void __cdecl hook_ApplyLightSettings(CHanaLightT3EF_S1A0 *pLight)
{
  CHanaFrameT3EC_S354 *pHanaFrame; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] ApplyLightSettings_40929C\n");
  
  if ( g_nLightningEnabled )
  {
    if ( pLight->d3dLight_134.dltType == (D3DLIGHT_FORCE_DWORD|0x80000000) )
    {
      g_pD3DDevice_4D6C78->SetRenderState(
        D3DRENDERSTATE_AMBIENT,
           static_cast<int>(pLight->d3dLight_134.dcvDiffuse.b * 255.0)
        | (static_cast<int>(pLight->d3dLight_134.dcvDiffuse.g * 255.0) << 8)
        | (static_cast<int>(pLight->d3dLight_134.dcvDiffuse.r * 255.0) << 16)
        | 0xFF000000);
    }
    else
    {
      if ( !g_bSpecularEnable )
      {
        pLight->d3dLight_134.dcvSpecular.r = 0.0;
        pLight->d3dLight_134.dcvSpecular.g = 0.0;
        pLight->d3dLight_134.dcvSpecular.b = 0.0;
        pLight->d3dLight_134.dcvSpecular.a = 0.0;
      }
      if ( pLight->pHanaT3EC_19C )
      {
        pHanaFrame = pLight->pHanaT3EC_19C;
        if ( g_bForceDirectionalLights == 1 )
          pLight->d3dLight_134.dltType = D3DLIGHT_DIRECTIONAL;
        g_pD3DDevice_4D6C78->SetLight(pLight->dwLightIndex_130, &pLight->d3dLight_134);
        pHanaFrame->bHasLight_214 = 0;
      }
      g_pD3DDevice_4D6C78->LightEnable(pLight->dwLightIndex_130, 1);
    }
  }
}

void __cdecl hook_SetupLightFromD3D(CHanaLightT3EF_S1A0 *pHanaLight, D3DLIGHT7 *pD3DLight)
{
  ONCELOG("[HNH:SUB] SetupLightFromD3D_40922E\n");

  qmemcpy(&pHanaLight->d3dLight_134, pD3DLight, sizeof(pHanaLight->d3dLight_134));
  ApplyLightSettings(pHanaLight);
}

void __cdecl hook_CHanaObj__FreeLightAnimT3F9(CHanaLightAnimT3F9_S144 *pLightAnim)
{
  CHanaSubObj_S74 *pNext_70; // [esp+0h] [ebp-8h]
  CHanaSubObj_S74 *hMem; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeLightAnimT3F9_409820\n");

  if ( pLightAnim->nCount_12C <= 1 )
  {
    if ( pLightAnim->psoS74_13C )
    {
      hMem = pLightAnim->psoS74_13C;
      SetupLightFromD3D(pLightAnim->pLight_130, &hMem->d3dLight_4);
      while ( 1 )
      {
        pNext_70 = hMem->pNext_70;
        if ( hMem )
          MemFree_40DF64(hMem);
        if ( !pNext_70 )
          break;
        hMem = pNext_70;
      }
    }
    CHanaObj__RemoveFromGlobalRef(pLightAnim);
    MemFree_40DF64(pLightAnim);
  }
  else
  {
    --pLightAnim->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeLightAnimSetT3FA(CHanaLightAnimSetT3FA_S13C *pLightAnimSet)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeLightAnimSetT3FA_40A3BB\n");

  if ( pLightAnimSet->nCount_12C <= 1 )
  {
    if ( pLightAnimSet->pArrObjs_8 )
    {
      for ( i = 0; i < pLightAnimSet->nCount_4; ++i )
        CHanaObj__Free_40C1E4(pLightAnimSet->pArrObjs_8[i]);
      MemFree_40DF64(pLightAnimSet->pArrObjs_8);
    }
    CHanaObj__RemoveFromGlobalRef(pLightAnimSet);
    MemFree_40DF64(pLightAnimSet);
  }
  else
  {
    --pLightAnimSet->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__FreeMorpT457(CHanaMorpT457_S40C *pMorp)
{
  unsigned int m; // [esp+0h] [ebp-8h]
  unsigned int n; // [esp+0h] [ebp-8h]
  unsigned int i; // [esp+4h] [ebp-4h]
  unsigned int j; // [esp+4h] [ebp-4h]
  unsigned int k; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeMorpT457_433E8D\n");

  if ( pMorp )
  {
    if ( pMorp->nCount_12C <= 1 )
    {
      if ( pMorp->pArrObjs_140 )
      {
        for ( i = 0; i < pMorp->pMeshT3F5_138->nCountVertT3EA_130; ++i )
        {
          if ( pMorp->pArrObjs_140[i] )
          {
            MemFree_40DF64(pMorp->pArrObjs_140[i]);
            pMorp->pArrObjs_140[i] = 0;
            pMorp->pArrObjs_140[i] = 0;
          }
        }
        if ( pMorp->pArrObjs_140 )
        {
          MemFree_40DF64(pMorp->pArrObjs_140);
          pMorp->pArrObjs_140 = 0;
        }
        pMorp->pArrObjs_140 = 0;
      }
      if ( pMorp->refsVertT3EA_13C )
      {
        if ( pMorp->arrMKeyT3EASize_348 )
        {
          MemFree_40DF64(pMorp->arrMKeyT3EASize_348);
          pMorp->arrMKeyT3EASize_348 = 0;
        }
        if ( pMorp->arrMKeyT3EA_34C )
        {
          for ( j = 0; j < pMorp->pMeshT3F5_138->nCountVertT3EA_130; ++j )
          {
            if ( pMorp->arrMKeyT3EA_34C[j] )
            {
              MemFree_40DF64(pMorp->arrMKeyT3EA_34C[j]);
              pMorp->arrMKeyT3EA_34C[j] = 0;
              pMorp->arrMKeyT3EA_34C[j] = 0;
            }
          }
          if ( pMorp->arrMKeyT3EA_34C )
          {
            MemFree_40DF64(pMorp->arrMKeyT3EA_34C);
            pMorp->arrMKeyT3EA_34C = 0;
          }
          pMorp->arrMKeyT3EA_34C = 0;
        }
        for ( k = 0; k < pMorp->nCountVertT3EA_138; ++k )
        {
          CHanaObj__RemoveFromGlobalRef(pMorp->refsVertT3EA_13C[k]);
          if ( pMorp->refsVertT3EA_13C[k] )
          {
            CHanaObj__Free_40C1E4(pMorp->refsVertT3EA_13C[k]);
            pMorp->refsVertT3EA_13C[k] = 0;
          }
        }
        if ( pMorp->refsVertT3EA_13C )
        {
          MemFree_40DF64(pMorp->refsVertT3EA_13C);
          pMorp->refsVertT3EA_13C = 0;
        }
      }
      for ( m = 0; m < pMorp->nCount_344; ++m )
      {
        if ( pMorp->arrSubObj_144[m].arrFun_4 )
        {
          MemFree_40DF64(pMorp->arrSubObj_144[m].arrFun_4);
          pMorp->arrSubObj_144[m].arrFun_4 = 0;
          pMorp->arrSubObj_144[m].arrFun_4 = 0;
        }
      }
      for ( n = 0; n < pMorp->nCount_344; ++n )
      {
        if ( pMorp->arrSubObj_144[n].pSubObj_0 )
        {
          if ( pMorp->arrSubObj_144[n].pSubObj_0->parrS2C )
          {
            MemFree_40DF64(pMorp->arrSubObj_144[n].pSubObj_0->parrS2C);
            pMorp->arrSubObj_144[n].pSubObj_0->parrS2C = 0;
            pMorp->arrSubObj_144[n].pSubObj_0->parrS2C = 0;
          }
          if ( pMorp->arrSubObj_144[n].pSubObj_0 )
          {
            MemFree_40DF64(pMorp->arrSubObj_144[n].pSubObj_0);
            pMorp->arrSubObj_144[n].pSubObj_0 = 0;
          }
          pMorp->arrSubObj_144[n].pSubObj_0 = 0;
          if ( pMorp->arrSubObj_144[n].pSubObj_0 )
          {
            MemFree_40DF64(pMorp->arrSubObj_144[n].pSubObj_0);
            pMorp->arrSubObj_144[n].pSubObj_0 = 0;
          }
        }
      }
      if ( pMorp->pMeshT3F5_138 )
      {
        CHanaObj__RemoveFromGlobalRef(pMorp->pMeshT3F5_138);
        CHanaObj__Free_40C1E4(pMorp->pMeshT3F5_138);
      }
      CHanaObj__RemoveFromGlobalRef(pMorp);
      MemFree_40DF64(pMorp);
    }
    else
    {
      --pMorp->nCount_12C;
    }
  }
}

void __cdecl hook_FreeSekHeader(CSekHeader *pSekHeader)
{
  ONCELOG("[HNH:SUB] FreeSekHeader_40D8F6\n");

  if ( pSekHeader )
  {
    if ( pSekHeader->pObj_12C )
      CHanaObj__Free_40C1E4(pSekHeader->pObj_12C);
    if ( pSekHeader->pObj_134 )
      CHanaObj__Free_40C1E4(pSekHeader->pObj_134);
    if ( pSekHeader->pObj_138 )
      CHanaObj__Free_40C1E4(pSekHeader->pObj_138);
    if ( pSekHeader->pObj_130 )
      CHanaObj__Free_40C1E4(pSekHeader->pObj_130);
    if ( pSekHeader->pAnimSetT3F1 )
      CHanaObj__Free_40C1E4(pSekHeader->pAnimSetT3F1);
    if ( pSekHeader->pFrameT3EC )
      CHanaObj__Free_40C1E4(pSekHeader->pFrameT3EC);
    MemFree_40DF64(pSekHeader);
  }
}

// CSekBody::Release - refcounted release of a CSekBody. Decrement count_5050; when it hits 0, free the header (sub_40D8F6 on pSekHeader_220) and GlobalFree the body. Returns the body pointer.
void __cdecl hook_CSekBody__Release_4012FE(CSekBody *pSekBody)
{
  ONCELOG("[HNH:SUB] CSekBody__Release_4012FE\n");

  if ( pSekBody )
  {
    if ( --pSekBody->count_5050 <= 0 )
    {
      FreeSekHeader(pSekBody->pSekHeader_220);
      pSekBody->pSekHeader_220 = 0;
      GlobalFree(pSekBody);
    }
  }
}

void __cdecl hook_CHanaObj__FreeSceneT458(CHanaSceneT458_S504 *pScene)
{
  unsigned int i; // [esp+0h] [ebp-4h]
  unsigned int j; // [esp+0h] [ebp-4h]
  unsigned int k; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__FreeSceneT458_4821FA\n");

  if ( pScene->nCount_12C <= 1 )
  {
    if ( pScene->pUnk_47C )
      pScene->pUnk_47C->Release();
    if ( pScene->pSurface_468 )
      pScene->pSurface_468->Release();
    if ( pScene->pSndBuf_46C )
    {
      pScene->pSndBuf_46C->Stop();
      pScene->pSndBuf_46C->Release();
      pScene->pSndBuf_46C = 0;
    }
    for ( i = 0; i < 5; ++i )
    {
      if ( pScene->pSprWindowBmp[i] )
      {
        CHanaObj__Free_40C1E4(&pScene->pSprWindowBmp[i]->base_0);
        pScene->pSprWindowBmp[i] = 0;
      }
    }
    for ( j = 0; j < 0x78; ++j )
    {
      if ( pScene->textSprites_264[0][j] )
      {
        CHanaObj__Free_40C1E4(&pScene->textSprites_264[0][j]->base_0);
        pScene->textSprites_264[0][j] = 0;
      }
    }
    if ( pScene->pAnimSet_148 )
    {
      CHanaObj__Free_40C1E4(pScene->pAnimSet_148);
      pScene->pAnimSet_148 = 0;
    }
    for ( k = 0; k < pScene->dwCount_130; ++k )
    {
      if ( pScene->arrSekItemProps_140 && pScene->arrSekItemProps_140[k] )
      {
        MemFree_40DF64(pScene->arrSekItemProps_140[k]);
        pScene->arrSekItemProps_140[k] = 0;
      }
      if ( pScene->refsAnimeSets_138 && pScene->refsAnimeSets_138[k] )
      {
        CHanaObj__Free_40C1E4(pScene->refsAnimeSets_138[k]);
        pScene->refsAnimeSets_138[k] = 0;
      }
      if ( pScene->refsFrames_134 && pScene->refsFrames_134[k] )
      {
        CHanaObj__Free_40C1E4(pScene->refsFrames_134[k]);
        pScene->refsFrames_134[k] = 0;
      }
      if ( pScene->refsSekBody_13C )
        CSekBody__Release_4012FE(pScene->refsSekBody_13C[k]);
    }
    if ( pScene->refsFrames_134 )
    {
      MemFree_40DF64(pScene->refsFrames_134);
      pScene->refsFrames_134 = 0;
    }
    if ( pScene->refsAnimeSets_138 )
    {
      MemFree_40DF64(pScene->refsAnimeSets_138);
      pScene->refsAnimeSets_138 = 0;
    }
    if ( pScene->refsSekBody_13C )
    {
      MemFree_40DF64(pScene->refsSekBody_13C);
      pScene->refsSekBody_13C = 0;
    }
    if ( pScene->arrSizes_144 )
    {
      MemFree_40DF64(pScene->arrSizes_144);
      pScene->arrSizes_144 = 0;
    }
    if ( pScene->arrSekItemProps_140 )
    {
      MemFree_40DF64(pScene->arrSekItemProps_140);
      pScene->arrSekItemProps_140 = 0;
    }
    if ( pScene->ptrStrData_260 )
    {
      MemFree_40DF64(pScene->ptrStrData_260);
      pScene->ptrStrData_260 = 0;
    }
    if ( pScene )
      MemFree_40DF64(pScene);
  }
  else
  {
    --pScene->nCount_12C;
  }
}

void __cdecl hook_CHanaObj__Free_40C1E4(CHanaBase_S130 *pObj)
{
  ONCELOG("[HNH:SUB] CHanaObj__Free_40C1E4\n");
  
  switch ( pObj->dwType )
  {
  case 0x3EA:
    CHanaObj__FreeVertT3EA((CHanaVertT3EA_S1FC*)pObj);
    --g_nRefVertT3EA;
    break;
  case 0x3EB:
    CHanaObj__FreeTextureT3EB((CHanaTextureT3EB_S2CC*)pObj);
    --g_nRefTextureT3EB;
    break;
  case 0x3EC:
    CHanaObj__FreeFrameT3EC((CHanaFrameT3EC_S354*)pObj);
    --g_nRefFrameT3EC;
    break;
  case 0x3ED:
    CHanaObj__FreeFrameArrayT3ED((CHanaFrameArrayT3ED_S138*)pObj);
    --g_nRefFrameArrayT3ED;
    break;
  case 0x3EE:
    CHanaObj__FreeMateT3EE((CHanaMateT3EE_S184*)pObj);
    --g_nRefMateT3EE;
    break;
  case 0x3EF:
    CHanaObj__FreeLightT3EF((CHanaLightT3EF_S1A0*)pObj);
    --g_nRefLightT3EF;
    break;
  case 0x3F0:
    CHanaObj__FreeAnimT3F0((CHanaAnimT3F0_S240*)pObj);
    --g_nRefAnimT3F0;
    break;
  case 0x3F1:
    CHanaObj__FreeAnimSetT3F1((CHanaAnimSetT3F1_S150*)pObj);
    --g_nRefAnimSetT3F1;
    break;
  case 0x3F2:
    CHanaObj__FreeTexCoordT3F2((CHanaTextureCoordsT3F2_S140*)pObj);
    --g_nRefTexCoordT3F2;
    break;
  case 0x3F3:
    CHanaObj__FreeTexAnimT3F3((CHanaTextureAnimT3F3_S16C*)pObj);
    break;
  case 0x3F4:
    CHanaObj__FreeTexAnimSetT3F4((CHanaTextureAnimSetT3F4_S138*)pObj);
    break;
  case 0x3F5:
    CHanaObj__FreeMeshT3F5((CHanaMeshT3F5_S138*)pObj);
    --g_nRefMeshT3F5;
    break;
  case 0x3F6:
    CHanaObj__FreeSpriteT3F6((CHanaSpriteT3F6_S1FC*)pObj);
    --g_nRefSpriteT3F6;
    break;
  case 0x3F7:
    CHanaObj__FreeMateAnimT3F7((CHanaMateAnimT3F7_S148*)pObj);
    --g_nRefMateAnimT3F7;
    break;
  case 0x3F8:
    CHanaObj__FreeMateAnimSetT3F8((CHanaMateAnimSetT3F8_S140*)pObj);
    --g_nRefMateAnimSetT3F8;
    break;
  case 0x3F9:
    CHanaObj__FreeLightAnimT3F9((CHanaLightAnimT3F9_S144*)pObj);
    --g_nRefLightAnimT3F9;
    break;
  case 0x3FA:
    CHanaObj__FreeLightAnimSetT3FA((CHanaLightAnimSetT3FA_S13C*)pObj);
    --g_nRefLightAnimSetT3FA;
    break;
  case 0x3FB:
    DebugLog("{%s} %s\n", "WARN", "Free T3FB");
    CHanaObj__FreeT3FB(pObj);
    break;
  case 0x3FC:
    DebugLog("{%s} %s\n", "WARN", "Free T3FC");
    CHanaObj__FreeT3FC(pObj);
    break;
  case 0x3FD:
    DebugLog("{%s} %s\n", "WARN", "Free T3FD");
    CHanaObj__FreeT3FD(pObj);
    break;
  case 0x3FE:
    DebugLog("{%s} %s\n", "WARN", "Free T3FE");
    CHanaObj__FreeT3FE(pObj);
    break;
  case 0x457:
    CHanaObj__FreeMorpT457((CHanaMorpT457_S40C*)pObj);
    --g_nRefMorpT457;
    break;
  case 0x458:
    CHanaObj__FreeSceneT458((CHanaSceneT458_S504*)pObj);
    break;
  default:
    return;
  }
}

void __cdecl hook_CHanaObj__GetRealPath_486D9A(LPSTR szPathOut, int nPathCode)
{
  CHAR szPathTemp[256]; // [esp+4h] [ebp-100h] BYREF

  ONCELOG("[HNH:SUB] %s\n", "CHanaObj__GetRealPath_486D9A");

  switch ( nPathCode )
  {
  case 0x3E8:
    wsprintfA(szPathTemp, "%s\\%s", g_szDriveLetr_52FC70, szPathOut);
    break;
  case 0x3E9:
    wsprintfA(szPathTemp, "%s\\%s", g_szHnhPath_530270, szPathOut);
    break;
  case 0x3EA:
    wsprintfA(szPathTemp, "%s%s", g_szODSPath, szPathOut);
    break;
  case 0x3EB:
    wsprintfA(szPathTemp, "%s%s", g_szSPRPath, szPathOut);
    break;
  case 0x3EC:
    wsprintfA(szPathTemp, "%s%s", g_szStrPath, szPathOut);
    break;
  case 0x3ED:
    wsprintfA(szPathTemp, "%s%s", g_szODFPath, szPathOut);
    break;
  case 0x3EE:
    wsprintfA(szPathTemp, "%s%s", g_szWAVPath, szPathOut);
    break;
  case 0x3EF:
    wsprintfA(szPathTemp, "%s%s", g_szGRPPath, szPathOut);
    break;
  case 0x3F0:
    wsprintfA(szPathTemp, "%s%s", g_szGRP800Path, szPathOut);
    break;
  case 0x3F1:
    wsprintfA(szPathTemp, "%s%s", g_szSEPath, szPathOut);
    break;
  case 0x3F2:
    wsprintfA(szPathTemp, "%s%s", g_szDATAPath, szPathOut);
    break;
  default:
    break;
  }
  wsprintfA(szPathOut, "%s", szPathTemp);
}

void __cdecl hook_CHanaObj__AddRef_40BFC3(CHanaBase_S130 *pObj)
{
  ONCELOG("[HNH:SUB] %s\n", "CHanaObj__AddRef_40BFC3");
  
  switch ( pObj->dwType )
  {
  case 0x3EA:
    ++g_nRefVertT3EA;
    break;
  case 0x3EB:
    ++g_nRefTextureT3EB;
    break;
  case 0x3EC:
    ++g_nRefFrameT3EC;
    break;
  case 0x3ED:
    ++g_nRefFrameArrayT3ED;
    break;
  case 0x3EE:
    ++g_nRefMateT3EE;
    break;
  case 0x3EF:
    ++g_nRefLightT3EF;
    break;
  case 0x3F0:
    ++g_nRefAnimT3F0;
    break;
  case 0x3F1:
    ++g_nRefAnimSetT3F1;
    break;
  case 0x3F2:
    ++g_nRefTexCoordT3F2;
    break;
  case 0x3F5:
    ++g_nRefMeshT3F5;
    break;
  case 0x3F6:
    ++g_nRefSpriteT3F6;
    break;
  case 0x3F7:
    ++g_nRefMateAnimT3F7;
    break;
  case 0x3F8:
    ++g_nRefMateAnimSetT3F8;
    break;
  case 0x3F9:
    ++g_nRefLightAnimT3F9;
    break;
  case 0x3FA:
    ++g_nRefLightAnimSetT3FA;
    break;
  case 0x457:
    ++g_nRefMorpT457;
    break;
  default:
    break;
  }
  ++pObj->nCount_12C;
}

void register_CHanaObj__Free_40C1E4() {
  hnh_Hook(0x40922E, hook_SetupLightFromD3D, "SetupLightFromD3D_40922E");
  hnh_Hook(0x415E05, hook_CHanaObj__FreeTexCoordT3F2, "CHanaObj__FreeTexCoordT3F2_415E05");
  hnh_Hook(0x414F62, hook_CHanaObj__FreeTexAnimT3F3, "CHanaObj__FreeTexAnimT3F3_414F62");
  hnh_Hook(0x41595B, hook_CHanaObj__FreeTexAnimSetT3F4, "CHanaObj__FreeTexAnimSetT3F4_41595B");

  hnh_Hook(0x40F3E5, hook_CHanaObj__FreeMateAnimSetT3F8, "CHanaObj__FreeMateAnimSetT3F8_40F3E5");  
  hnh_Hook(0x409820, hook_CHanaObj__FreeLightAnimT3F9, "CHanaObj__FreeLightAnimT3F9_409820");
  hnh_Hook(0x40A3BB, hook_CHanaObj__FreeLightAnimSetT3FA, "CHanaObj__FreeLightAnimSetT3FA_40A3BB");

  hnh_Hook(0x40BD4D, hook_CHanaObj__RemoveFromGlobalRef, "CHanaObj__RemoveFromGlobalRef__40BD4D");

  hnh_HookOff();
  hnh_Hook(0x40C1E4, hook_CHanaObj__Free_40C1E4, "CHanaObj__Free_40C1E4"); // [OK] 
  hnh_Hook(0x40BD4D, hook_CHanaObj__RemoveFromGlobalRef, "CHanaObj__RemoveFromGlobalRef_40BD4D"); // [OK] 
  hnh_Hook(0x4058C3, hook_RemoveChild_4058C3, "RemoveChild_4058C3"); // [OK] 
  hnh_Hook(0x486D9A, hook_CHanaObj__GetRealPath_486D9A, "CHanaObj__GetRealPath_486D9A"); // [OK] 
  hnh_Hook(0x40BFC3, hook_CHanaObj__AddRef_40BFC3, "CHanaObj__AddRef_40BFC3"); // [OK] 
  hnh_Hook(0x401EDF, hook_UnlinkHanaAnimFromFrame, "UnlinkHanaAnimFromFrame_401EDF"); // [OK] 
  hnh_Hook(0x40F891, hook_ApplyMaterial, "ApplyMaterial_40F891"); // [OK] 
  hnh_Hook(0x40929C, hook_ApplyLightSettings, "ApplyLightSettings_40929C"); // [OK] 
  hnh_Hook(0x417A0C, hook_CHanaObj__FreeVertT3EA, "CHanaObj__FreeVertT3EA_417A0C"); // [OK] 
  hnh_Hook(0x416256, hook_CHanaObj__FreeTextureT3EB, "CHanaObj__FreeTextureT3EB_416256"); // [OK] 
  hnh_Hook(0x40536D, hook_Frame_FreeAttachedObject, "Frame_FreeAttachedObject_40536D"); // [OK] 
  hnh_Hook(0x405421, hook_Frame_FreeAttachedLight, "Frame_FreeAttachedLight_405421"); // [OK] 
  hnh_Hook(0x4054B3, hook_CHanaObj__FreeFrameT3EC, "CHanaObj__FreeFrameT3EC_4054B3"); // [OK] 
  hnh_Hook(0x40FC5E, hook_CHanaObj__FreeMateT3EE, "CHanaObj__FreeMateT3EE_40FC5E"); // [OK] 
  hnh_Hook(0x409437, hook_CHanaObj__FreeLightT3EF, "CHanaObj__FreeLightT3EF_409437"); // [OK] 
  hnh_Hook(0x408E8D, hook_CHanaObj__FreeFrameArrayT3ED, "CHanaObj__FreeFrameArrayT3ED_408E8D"); // [OK] 
  hnh_Hook(0x401F53, hook_CHanaObj__FreeAnimT3F0, "CHanaObj__FreeAnimT3F0_401F53"); // [OK] 
  hnh_Hook(0x422C72, hook_FreeSoundStruct_422C72, "FreeSoundStruct_422C72"); // [OK] 
  hnh_Hook(0x403A55, hook_CHanaObj__FreeAnimSetT3F1, "CHanaObj__FreeAnimSetT3F1_403A55"); // [OK] 
  hnh_Hook(0x40FDCB, hook_CHanaObj__FreeMeshT3F5, "CHanaObj__FreeMeshT3F5_40FDCB"); // [OK] 
  hnh_Hook(0x413F07, hook_CHanaObj__FreeSpriteT3F6, "CHanaObj__FreeSpriteT3F6_413F07"); // [OK] 
  hnh_Hook(0x40E6B6, hook_CHanaObj__FreeMateAnimT3F7, "CHanaObj__FreeMateAnimT3F7_40E6B6"); // [OK] 
  hnh_Hook(0x433E8D, hook_CHanaObj__FreeMorpT457, "CHanaObj__FreeMorpT457_433E8D"); // [OK] 
  hnh_Hook(0x40D8F6, hook_FreeSekHeader, "FreeSekHeader_40D8F6"); // [OK] 
  hnh_Hook(0x4012FE, hook_CSekBody__Release_4012FE, "CSekBody__Release_4012FE"); // [OK] 
  hnh_Hook(0x4821FA, hook_CHanaObj__FreeSceneT458, "CHanaObj__FreeSceneT458_4821FA"); // [OK] 
  hnh_Hook(0x40C1E4, hook_CHanaObj__Free_40C1E4, "CHanaObj__Free_40C1E4"); // [OK] 
  hnh_HookOn();
}