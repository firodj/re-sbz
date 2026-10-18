#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"
#include "struct.hpp"

int (__cdecl *CHanaObj__Free_40C1E4)(CHanaBase_S130 *pObj) = reinterpret_cast<int (__cdecl*)(CHanaBase_S130*)>(0x40C1E4);

void __cdecl CHanaObj__RemoveFromGlobalRef(CHanaBase_S130 *pObj)
{
    DebugLog("[HNH:SUB] CHanaObj__RemoveFromGlobalRef_40BD4D\n");

    if ( pObj->nIndex_108 > 0 )
    {
        g_pObjectTable[pObj->nIndex_108].szName_0[0] = 0;
        g_pObjectTable[pObj->nIndex_108].pObj_200 = 0;
        g_pObjectTable[pObj->nIndex_108].dwObjType_204 = 0;
    }
}

void __cdecl CHanaObj__FreeVertT3EA(CHanaVertT3EA_S1FC *pVert)
{
    int v1; // [esp+0h] [ebp-4h]

    DebugLog("[HNH:SUB] CHanaObj__FreeVertT3EA_417A0C\n");

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
            GlobalFree_40DF64(pVert->pVtxIndices);
            pVert->pVtxIndices = 0;
        }
        v1 = pVert->pD3DVtxBuf_130->Release();
        if ( v1 < 0 )
            PrintDebugError(v1);
        if ( pVert->pUnk_164 )
            pVert->pUnk_164->Release();
        CHanaObj__RemoveFromGlobalRef(pVert);
        GlobalFree_40DF64(pVert);
    }
    else
    {
        --pVert->nCount_12C;
    }
}

CHanaTextureT3EB_S2CC* &g_pCurTexObj = *(CHanaTextureT3EB_S2CC**)0x4D6CA0;

void __cdecl CHanaObj__FreeTextureT3EB(CHanaTextureT3EB_S2CC *pTexture)
{
    int v1; // [esp+0h] [ebp-4h]

    DebugLog("[HNH:SUB] CHanaObj__FreeTextureT3EB_416256\n");

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
        GlobalFree_40DF64(pTexture);
    }
    else
    {
        --pTexture->nCount_12C;
    }
}

void __cdecl Frame_FreeAttachedObject(CHanaFrameT3EC_S354 *pFrame)
{
    DebugLog("[HNH:SUB] Frame_FreeAttachedObject_40536D\n");
 
    if ( pFrame->pAttachedObj_20C )
    {
        CHanaObj__Free_40C1E4(pFrame->pAttachedObj_20C);
        pFrame->pAttachedObj_20C = 0;
    }
}

void __cdecl Frame_FreeAttachedLight(CHanaFrameT3EC_S354 *pFrame)
{
    DebugLog("[HNH:SUB] Frame_FreeAttachedLight_405421\n");

    if ( pFrame->pHanaLight_210 )
    {
        pFrame->pHanaLight_210->pHanaT3EC_19C = 0;
        CHanaObj__Free_40C1E4(pFrame->pHanaLight_210);
        pFrame->pHanaLight_210 = 0;
        pFrame->bHasLight_214 = 0;
    }
}

void __cdecl RemoveChild_4058C3(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild)
{
    CHanaNode *pPrev; // [esp+0h] [ebp-Ch]
    CHanaNode *pNext; // [esp+0h] [ebp-Ch]
    unsigned int i; // [esp+4h] [ebp-8h]
    CHanaNode *pNode; // [esp+8h] [ebp-4h]

    DebugLog("[HNH:SUB] RemoveChild_4058C3\n");

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
                GlobalFree_40DF64(pNode);
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

void __cdecl CHanaObj__FreeFrameT3EC(CHanaFrameT3EC_S354 *pFrame)
{
    CHanaNode *pNext; // [esp+0h] [ebp-14h]
    unsigned int i; // [esp+4h] [ebp-10h]
    CHanaNode *pFirstChildNode; // [esp+8h] [ebp-Ch]
    unsigned int nCountChild; // [esp+Ch] [ebp-8h]
    CHanaAnimT3F0_S240 *pHanaAnim_2C0; // [esp+10h] [ebp-4h]

    DebugLog("[HNH:SUB] CHanaObj__FreeFrameT3EC_4054B3\n");

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
        GlobalFree_40DF64(pFrame);
    }
    else
    {
        --pFrame->nCount_12C;
    }
}

void __cdecl CHanaObj__FreeMateT3EE(CHanaMateT3EE_S184 *pMate)
{
    DebugLog("[HNH:SUB] CHanaObj__FreeMateT3EE_40FC5E\n");

    if ( pMate->nCount_12C <= 1 )
    {
        CHanaObj__RemoveFromGlobalRef(pMate);
        GlobalFree_40DF64(pMate);
    }
    else
    {
        --pMate->nCount_12C;
    }
}

__int64 &g_bitsLightIndexes_4D6CB8 = *(__int64*)0x4D6CB8;

void __cdecl CHanaObj__FreeLightT3EF(CHanaLightT3EF_S1A0 *pLight)
{
    DebugLog("[HNH:SUB] CHanaObj__FreeLightT3EF_409437\n");

    if ( pLight->nCount_12C <= 1 )
    {
        g_bitsLightIndexes_4D6CB8 = (1LL << pLight->dwLightIndex_130 == 0) & g_bitsLightIndexes_4D6CB8;
        g_pD3DDevice_4D6C78->SetLight(pLight->dwLightIndex_130, 0);
        g_pD3DDevice_4D6C78->LightEnable(pLight->dwLightIndex_130, 0);
        CHanaObj__RemoveFromGlobalRef(pLight);
        GlobalFree_40DF64(pLight);
    }
    else
    {
        --pLight->nCount_12C;
    }
}

// Idiomatic with IUnknown::Release() - decrements nCount_12C; at <= 1 frees arrFrames, unlinks global ref, frees object
void __cdecl CHanaObj__FreeFrameArrayT3ED(CHanaFrameArrayT3ED_S138 *pFrameArrayT3ED)
{
  DebugLog("[HNH:SUB] CHanaObj__FreeFrameArrayT3ED_408E8D\n");

  if ( pFrameArrayT3ED )
  {
    if ( pFrameArrayT3ED->nCount_12C <= 1 )
    {
      if ( pFrameArrayT3ED->arrFrames )
      {
        GlobalFree_40DF64(pFrameArrayT3ED->arrFrames);
        pFrameArrayT3ED->arrFrames = 0;
      }
      CHanaObj__RemoveFromGlobalRef(pFrameArrayT3ED);
      GlobalFree_40DF64(pFrameArrayT3ED);
    }
    else
    {
      --pFrameArrayT3ED->nCount_12C;
    }
  }
}

void __cdecl UnlinkHanaAnimFromFrame(CHanaAnimT3F0_S240 *pAnim)
{
  CHanaFrameT3EC_S354 *pFrame; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  CObjItemRef_S208 *pObjIt; // [esp+8h] [ebp-4h]

  DebugLog("[HNH:SUB] UnlinkHanaAnimFromFrame_401EDF\n");

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

void __cdecl CHanaObj__FreeAnimT3F0(CHanaAnimT3F0_S240 *pAnim)
{
  DebugLog("[HNH:SUB] CHanaObj__FreeAnimT3F0_401F53\n");

  if ( pAnim->nCount_12C <= 1 )
  {
    UnlinkHanaAnimFromFrame(pAnim);
    if ( pAnim->arroS78_13C )
      GlobalFree_40DF64(pAnim->arroS78_13C);
    CHanaObj__RemoveFromGlobalRef(pAnim);
    GlobalFree_40DF64(pAnim);
  }
  else
  {
    --pAnim->nCount_12C;
  }
}

void __cdecl FreeSoundStruct_422C72(CSoundStruct_8 *pSound)
{
  DebugLog("[HNH:SUB] FreeSoundStruct_422C72\n");

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

void __cdecl CHanaObj__FreeAnimSetT3F1(CHanaAnimSetT3F1_S150 *pAnimSet)
{
  unsigned int k; // [esp+0h] [ebp-Ch]
  int j; // [esp+4h] [ebp-8h]
  unsigned int i; // [esp+8h] [ebp-4h]

  DebugLog("[HNH:SUB] CHanaObj__FreeAnimSetT3F1_403A55\n");

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
      GlobalFree_40DF64(pAnimSet->arroS118_14C);
    }
    if ( pAnimSet->refsAnimT3F0_138 )
    {
      for ( j = 0; j < pAnimSet->nAnimCount_134; ++j )
        CHanaObj__Free_40C1E4(pAnimSet->refsAnimT3F0_138[j]);
      GlobalFree_40DF64(pAnimSet->refsAnimT3F0_138);
      pAnimSet->refsAnimT3F0_138 = 0;
    }
    if ( pAnimSet->pRefsManmT3F7 )
    {
      for ( k = 0; k < pAnimSet->nManmCount_140; ++k )
        CHanaObj__Free_40C1E4(pAnimSet->pRefsManmT3F7[k]);
      GlobalFree_40DF64(pAnimSet->pRefsManmT3F7);
    }
    CHanaObj__RemoveFromGlobalRef(pAnimSet);
    GlobalFree_40DF64(pAnimSet);
  }
  else
  {
    --pAnimSet->nCount_12C;
  }
}

#if 0

void __cdecl CHanaObj__Free_40C1E4(CHanaBase_S130 *pObj)
{
    switch ( pObj->dwType )
    {
    case 0x3EA:
        CHanaObj__FreeVertT3EA(pObj);
        --g_nRefVertT3EA;
        break;
    case 0x3EB:
        CHanaObj__FreeTextureT3EB(pObj);
        --g_nRefTextureT3EB;
        break;
    case 0x3EC:
        CHanaObj__FreeFrameT3EC(pObj);
        --g_nRefFrameT3EC;
        break;
    case 0x3ED:
      CHanaObj__FreeFrameArrayT3ED(pObj);
        --g_nRefT3ED;
        break;
    case 0x3EE:
        CHanaObj__FreeMateT3EE(pObj);
        --g_nRefMateT3EE;
        break;
    case 0x3EF:
        CHanaObj__FreeLightT3EF(pObj);
        --g_nRefLightT3EF;
        break;
    case 0x3F0:
        CHanaObj__FreeAnimT3F0(pObj);
        --g_nRefAnimT3F0;
        break;
    case 0x3F1:
        CHanaObj__FreeAnimSetT3F1(pObj);
        --g_nRefAnimSetT3F1;
        break;
    case 0x3F2:
        CHanaObj__FreeTexCoordT3F2(pObj);
        --g_nRefTexCoordT3F2;
        break;
    case 0x3F3:
        CHanaObj__FreeTexAnimT3F3(pObj);
        break;
    case 0x3F4:
        CHanaObj__FreeTexAnimSetT3F4(pObj);
        break;
    case 0x3F5:
        CHanaObj__FreeMeshT3F5(pObj);
        --g_nRefMeshT3F5;
        break;
    case 0x3F6:
        CHanaObj__FreeSpriteT3F6(pObj);
        --g_nRefSpriteT3F6;
        break;
    case 0x3F7:
        CHanaObj__FreeMateAnimT3F7(pObj);
        --g_nRefMateAnimT3F7;
        break;
    case 0x3F8:
        CHanaObj__FreeMateAnimSetT3F8(pObj);
        --g_nRefMateAnimSetT3F8;
        break;
    case 0x3F9:
        CHanaObj__FreeLightAnimT3F9(pObj);
        --g_nRefLightAnimT3F9;
        break;
    case 0x3FA:
        CHanaObj__FreeLightAnimSetT3FA(pObj);
        --g_nRefLightAnimSetT3FA;
        break;
    case 0x3FB:
        CHanaObj__FreeT3FB(pObj);
        break;
    case 0x3FC:
        CHanaObj__FreeT3FC(pObj);
        break;
    case 0x3FD:
        CHanaObj__FreeT3FD(pObj);
        break;
    case 0x3FE:
        CHanaObj__FreeT3FE(pObj);
        break;
    case 0x457:
        CHanaObj__FreeMorpT457(pObj);
        --g_nRefMorpT457;
        break;
    case 0x458:
        CHanaObj__FreeSceneT458(pObj);
        break;
    default:
        return;
    }
}

#endif