#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// ProcessFrame_40600C -- the recursive scene-frame renderer (entry from
// HanaFrame_Draw; 3 of its 5 xrefs are its own recursion). Per frame:
//
//   1. every CHILD carrying a real light (dltType != -1) is configured into
//      the device from its world matrix -- position = row 4 (_41.._43),
//      direction = Z column (_13,_23,_33) -- and its index bit is OR-ed into
//      the passed 64-bit light mask and a local "lights I enabled" mask
//   2. bUpdateBone_218 frames get a bone-transform update against camera + scene
//   3. world transform set, bIsRoot_1F8 = 1; g_fCurFrameIsScene latches to 1
//      once the traversal reaches g_pCurFrame
//   4. frames passing the render gate (!bAlphaDeferred_130 && cur-frame reached
//      && !g_fRendering && AlwaysTrue()) get:
//        - transparent verts (masked texture / blend mode / alpha != 1)
//          DEFERRED to the render pool (AddObjectToPool). Material diffuse
//          alpha > 1 switches additive (mode +1, alpha -= 1), < 0
//          subtractive (mode -1, alpha = |alpha|). dwType picks the object
//          kind: 0x3EA single vert (VX-N), 0x3F5 mesh (MH-N), 0x457 morp
//          through its inner mesh (MP-N) -- the same codes the ParseOdfFRAM
//          tag table produces. A pooled single vert (0x3EA) skips direct
//          drawing; mesh/morp verts are marked nPrimType_154[0] = 1 so the
//          direct draw skips them
//        - the attached object drawn directly (HanaVert_DrawByType), with fog
//          disabled around it while bDisableFog_134 == 1
//        - children recursed; a child whose light has dltType == -1 is an
//          AMBIENT light: ApplyLightSettings, recurse under the device's
//          current AMBIENT render state, restore AMBIENT afterwards
//   5. lights enabled in step 1 are LightEnable(idx, 0)'d again; the
//      original then clears "their bit" with mask &= (1<<j == 0) -- which
//      ZEROES the entire 64-bit mask on the first hit. Presumably meant
//      &= ~(1<<j); kept faithful, bug and all.

// === PINNED GAME FUNCTIONS ===
void(__cdecl*ProcessFrame)(CHanaFrameT3EC_S354* pChildFrame, int nAmbient, DWORD* qwBitLightEnables) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 * pChildFrame, int nAmbient, DWORD * qwBitLightEnables)>(0x40600C);

void (__cdecl*UpdateBoneTransform)(CHanaFrameT3EC_S354 *boneStruct,
    CHanaFrameT3EC_S354 *parentStruct, CHanaFrameT3EC_S354 *pSceneFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *)>(0x408283);
//int  (__cdecl*AlwaysTrue)(void) = reinterpret_cast<int(__cdecl*)()>(0x4084C2);
int  (__cdecl*AddObjectToPool)(D3DXMATRIX *mat4x4After, CHanaFrameT3EC_S354 *pChildFrame,
    CHanaVertT3EA_S1FC *pVert, int nAmbient, QWORD qwBitLightEnables) =
    reinterpret_cast<int(__cdecl*)(D3DXMATRIX *, CHanaFrameT3EC_S354 *, CHanaVertT3EA_S1FC *, int, QWORD)>(0x40CD32);
DWORD (__cdecl*CHanaObj__GetChildCountT3F5_40FF0F)(CHanaMeshT3F5_S138 *pHanaT3F5) =
    reinterpret_cast<DWORD(__cdecl*)(CHanaMeshT3F5_S138 *)>(0x40FF0F);
void (__cdecl*HanaVert_DrawByType)(CHanaVertT3EA_S1FC *pVert) =
    reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *)>(0x40C54C);

void(__cdecl*SortObjectsByDepth)() = reinterpret_cast<void(__cdecl*)()>(0x40D9C7);
void(__cdecl*SetLightEnableMask)(QWORD bitLightEnables) = reinterpret_cast<void(__cdecl*)(QWORD)>(0x40CE4E);
void(__cdecl*UpdateVertTextureFromFrame)(CHanaVertT3EA_S1FC* pVertArg, CHanaFrameT3EC_S354* pFrameArg) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC * pVertArg, CHanaFrameT3EC_S354 * pFrameArg)>(0x439D48);
void (__cdecl * ProjectToScreen_40E2A5)(D3DXMATRIX *pM, POINT *pOut) = reinterpret_cast<void(__cdecl*)(D3DXMATRIX *pM, POINT *pOut)>(0x40E2A5);
void (__cdecl * BuildViewportMatrix)() = reinterpret_cast<void(__cdecl*)()>(0x40E167);
double (* GetProjectionFov_40DCF6)() = reinterpret_cast<double(*)()>(0x40DCF6);
void (__cdecl * HanaVertT3EA_Draw)(CHanaVertT3EA_S1FC *pVert) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVert)>(0x417B27);
void (__cdecl * HanaMate_SetMaterial)(CHanaMateT3EE_S184 *pMate) = reinterpret_cast<void(__cdecl*)(CHanaMateT3EE_S184 *pMate)>(0x40FB30);
void (__cdecl * UpdateBoneLookAtTransform)( CHanaFrameT3EC_S354 *pBoneFrame, CHanaFrameT3EC_S354 *pParentFrame, float dirForwardX, float dirForwardY, float dirForwardZ, float dirUpX, float dirUpY, float dirUpZ) = reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pBoneFrame, CHanaFrameT3EC_S354 *pParentFrame, float dirForwardX, float dirForwardY, float dirForwardZ, float dirUpX, float dirUpY, float dirUpZ)>(0x406CA4);
void (__cdecl * CameraLookAtFrame)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pTargetFrame, CHanaFrameT3EC_S354 *pSceneFrame, __int16 iMode) = reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pTargetFrame, CHanaFrameT3EC_S354 *pSceneFrame, __int16 iMode)>(0x40873E);
void (__cdecl * HanaMeshT3F5_Draw)(CHanaMeshT3F5_S138 *pMesh) = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 *pMesh)>(0x40FE8E);
void (__cdecl * HanaMorpT457_Draw)(CHanaMorpT457_S40C *pMorp) = reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *pMorp)>(0x434FC9);

// === PINNED GLOBALS ===
uint32_t &g_fRendering    = *(uint32_t *)0x4CD778;  // WRITABLE
uint32_t &g_fObjBatchActive = *(uint32_t *)0x4D6CD8; // WRITABLE
float (&g_floats_4CE958)[6000] = *(float (*)[6000])0x4CE958; // WRITABLE

// === FUNCTIONS ===
// alpha/blend-vert test shared by all three object kinds
static BOOL VertNeedsAlphaPass(CHanaVertT3EA_S1FC *pVert)
{
    return (pVert->pTexT3EB && pVert->pTexT3EB->bHasMask)
        || pVert->nBlendMode_1D0
        || pVert->bAlphaPass2_1E8
        || pVert->bForceAlphaPass_1CC == 1
        || (pVert->pMateT3EE && pVert->pMateT3EE->d3dMaterial_130.diffuse.a != 1.0f);
}

// diffuse-alpha sign magic -> blend mode, then defer to the render pool
static void FixAlphaAndAddToPool(D3DXMATRIX *pMat, CHanaFrameT3EC_S354 *pFrame,
                                 CHanaVertT3EA_S1FC *pVert, int nAmbient,
                                 unsigned __int64 qwLightEnables)
{
    if (pVert->pMateT3EE && pVert->pMateT3EE->d3dMaterial_130.diffuse.a > 1.0f)
    {
        pVert->nBlendMode_1D0 = 1;                             // additive
        pVert->pMateT3EE->d3dMaterial_130.diffuse.a =
            fabsf(pVert->pMateT3EE->d3dMaterial_130.diffuse.a - 1.0f);
    }
    if (pVert->pMateT3EE && pVert->pMateT3EE->d3dMaterial_130.diffuse.a < 0.0f)
    {
        pVert->nBlendMode_1D0 = -1;                            // subtractive
        pVert->pMateT3EE->d3dMaterial_130.diffuse.a =
            fabsf(pVert->pMateT3EE->d3dMaterial_130.diffuse.a);
    }
    AddObjectToPool(pMat, pFrame, pVert, nAmbient, qwLightEnables);
}

BOOL AlwaysTrue()
{
  return 1;
}

void __cdecl hook_ProcessFrame(CHanaFrameT3EC_S354 *pChildFrame, int nAmbient, DWORD *qwBitLightEnables)
{
    D3DXMATRIX matLight;                     // [esp+64h] [ebp-64h] light frame world matrix
    CHanaFrameT3EC_S354 *pLightChild;        // [esp+A4h] [ebp-24h]
    CHanaNode *pNode;                        // [esp+C4h] [ebp-4h]
    unsigned __int64 qwAccumulatedLights;    // [esp+A8h] [ebp-20h]
    CHanaMeshT3F5_S138 *pMesh;               // [esp+B0h] [ebp-18h]
    unsigned int iLight, nChildCount, iVert, nVertCount;
    CHanaVertT3EA_S1FC *pVert;               // [esp+B8h] [ebp-10h]
    DWORD dwAmbientCur;
    unsigned int j;

    ONCELOG("[HNH:SUB] ProcessFrame_40600C\n");

    qwAccumulatedLights = 0;

    // 1) enable lights carried by direct children (dltType != -1)
    if (pChildFrame->nCountChild)
    {
        pNode = pChildFrame->pFirstChildNode;
        for (iLight = 0; iLight < pChildFrame->nCountChild; ++iLight)
        {
            pLightChild = pNode->pData;
            if (pLightChild && pLightChild->pHanaLight_210 && g_nLightningEnabled
                && pLightChild->pHanaLight_210->d3dLight_134.dltType != (D3DLIGHTTYPE)-1)
            {
                matLight = pLightChild->mat4x4_after;
                ConfigureLight(pLightChild->pHanaLight_210,
                               matLight._41, matLight._42, matLight._43,    // position
                               matLight._13, matLight._23, matLight._33);   // direction (Z col)
                *(_QWORD *)qwBitLightEnables |= (unsigned __int64)1
                    << pLightChild->pHanaLight_210->dwLightIndex_130;
                qwAccumulatedLights |= (unsigned __int64)1
                    << pLightChild->pHanaLight_210->dwLightIndex_130;
            }
            pNode = pNode->pNext;
        }
    }

    if (pChildFrame->bUpdateBone_218)
        UpdateBoneTransform(pChildFrame, g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame);
    g_pD3DDevice_4D6C78->SetTransform(D3DTRANSFORMSTATE_WORLD, (LPD3DMATRIX) & pChildFrame->mat4x4_after);
    pChildFrame->bIsRoot_1F8 = 1;
    if (pChildFrame == g_pCurFrame)
        g_fCurFrameIsScene = 1;

    if (!pChildFrame->bAlphaDeferred_130 && g_fCurFrameIsScene && !g_fRendering && AlwaysTrue())
    {
        BOOL bPooledOnly = FALSE;

        // 4a) defer transparent verts to the render pool
        if (!g_fObjBatchActive)
        {
            pMesh = (CHanaMeshT3F5_S138 *)pChildFrame->pAttachedObj_20C;
            if (pMesh)
            {
                switch (pMesh->dwType)
                {
                case 0x3EA:                       // single vert (VX-N)
                    pVert = (CHanaVertT3EA_S1FC *)pMesh;
                    // (the decompiler reads pVert->pTexT3EB through an
                    // out-of-struct cast on the mesh type)
                    if (VertNeedsAlphaPass(pVert))
                    {
                        FixAlphaAndAddToPool(&pChildFrame->mat4x4_after, pChildFrame, pVert,
                                             nAmbient, *(_QWORD *)qwBitLightEnables);
                        bPooledOnly = TRUE;       // skip the direct draw below
                    }
                    break;
                case 0x3F5:                       // mesh (MH-N)
                    nVertCount = CHanaObj__GetChildCountT3F5_40FF0F(pMesh);
                    for (iVert = 0; iVert < nVertCount; ++iVert)
                    {
                        pVert = pMesh->refsVertT3EA[iVert];
                        if (VertNeedsAlphaPass(pVert))
                        {
                            pVert->nPrimType_154[0] = 1;  // pooled: direct draw skips it
                            FixAlphaAndAddToPool(&pChildFrame->mat4x4_after, pChildFrame, pVert,
                                                 nAmbient, *(_QWORD *)qwBitLightEnables);
                        }
                    }
                    break;
                case 0x457:                       // morp (MP-N): verts of the inner mesh
                    {
                        CHanaMeshT3F5_S138 *pInnerMesh =
                            ((CHanaMorpT457_S40C *)pMesh)->pMeshT3F5_138;
                        nVertCount = CHanaObj__GetChildCountT3F5_40FF0F(pInnerMesh);
                        for (iVert = 0; iVert < nVertCount; ++iVert)
                        {
                            pVert = pInnerMesh->refsVertT3EA[iVert];
                            if (VertNeedsAlphaPass(pVert))
                            {
                                pVert->nPrimType_154[0] = 1;
                                FixAlphaAndAddToPool(&pChildFrame->mat4x4_after, pChildFrame, pVert,
                                                     nAmbient, *(_QWORD *)qwBitLightEnables);
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }

        // 4b) direct draw of the attached object
        if (!bPooledOnly && pChildFrame->pAttachedObj_20C)
        {
            if (pChildFrame->bDisableFog_134 == 1 && g_dwFogEnable_4D6CF4 == 1)
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
            HanaVert_DrawByType((CHanaVertT3EA_S1FC *)pChildFrame->pAttachedObj_20C);
            if (pChildFrame->bDisableFog_134 == 1 && g_dwFogEnable_4D6CF4 == 1)
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 1);
        }

        // 4c) recurse into children
        if (pChildFrame->nCountChild)
        {
            nChildCount = pChildFrame->nCountChild;
            pNode = pChildFrame->pFirstChildNode;
            for (iLight = 0; iLight < nChildCount; ++iLight)
            {
                CHanaFrameT3EC_S354 *pChild = pNode->pData;
                if (pChild->pHanaLight_210 && g_nLightningEnabled
                    && pChild->pHanaLight_210->d3dLight_134.dltType == (D3DLIGHTTYPE)-1)
                {
                    // ambient (-1) light child: recurse under its ambient
                    ApplyLightSettings(pChild->pHanaLight_210);
                    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbientCur);
                    hook_ProcessFrame(pChild, (int)dwAmbientCur, qwBitLightEnables);
                    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, (DWORD)nAmbient);
                }
                else
                {
                    hook_ProcessFrame(pChild, nAmbient, qwBitLightEnables);
                }
                pNode = pNode->pNext;
            }
        }
    }

    // 5) disable the lights enabled in step 1
    for (j = 0; j < 64; ++j)
    {
        if (((unsigned __int64)1 << j) & qwAccumulatedLights)
        {
            g_pD3DDevice_4D6C78->LightEnable(j, 0);
            // ORIGINAL BUG: clears the WHOLE 64-bit mask (not just bit j) --
            // (1<<j)==0 is always false for j < 64. Kept faithful.
            *(_QWORD *)qwBitLightEnables &= (DWORD)(((unsigned __int64)1 << j) == 0);
        }
    }
}

void __cdecl hook_SortObjectsByDepth()
{
  QWORD tempBitLightEnables; // [esp+10h] [ebp-70h]
  BYTE swapBuffer[64]; // [esp+18h] [ebp-68h] BYREF
  int nAmbient; // [esp+58h] [ebp-28h]
  int j; // [esp+5Ch] [ebp-24h]
  float tempFloat; // [esp+60h] [ebp-20h]
  int i; // [esp+64h] [ebp-1Ch]
  float centerX; // [esp+68h] [ebp-18h] BYREF
  float centerY; // [esp+6Ch] [ebp-14h] BYREF
  float centerZ; // [esp+70h] [ebp-10h] BYREF
  CHanaFrameT3EC_S354 *tempFrame; // [esp+74h] [ebp-Ch]
  int unused_11; // [esp+78h] [ebp-8h]
  CHanaVertT3EA_S1FC *tempVert; // [esp+7Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] SortObjectsByDepth_40D9C7\n");

  if ( g_nObjCount >= 1 )
  {
    if ( g_nObjCount >= 6000 )
      g_nObjCount = 6000;
    unused_11 = 0;
    for ( i = 0; i < g_nObjCount; ++i )
    {
      HanaFrame_GetCenterPosition_408C3E(g_pObjPool[i].pFrame, g_pCameraFrame, &centerX, &centerY, &centerZ);
      g_floats_4CE958[i] = centerZ - (double)(1000 * g_pObjPool[i].pVert->alpr_1F8);
    }
    for ( i = 0; i < g_nObjCount - 1; ++i )
    {
      for ( j = i + 1; j < g_nObjCount; ++j )
      {
        if ( g_floats_4CE958[i] < (double)g_floats_4CE958[j] )
        {
          tempFloat = g_floats_4CE958[i];
          g_floats_4CE958[i] = g_floats_4CE958[j];
          g_floats_4CE958[j] = tempFloat;
          tempFrame = g_pObjPool[i].pFrame;
          g_pObjPool[i].pFrame = g_pObjPool[j].pFrame;
          g_pObjPool[j].pFrame = tempFrame;
          tempVert = g_pObjPool[i].pVert;
          g_pObjPool[i].pVert = g_pObjPool[j].pVert;
          g_pObjPool[j].pVert = tempVert;
          qmemcpy(swapBuffer, &g_pObjPool[i], sizeof(swapBuffer));
          qmemcpy(&g_pObjPool[i], &g_pObjPool[j], 0x40u);
          qmemcpy(&g_pObjPool[j], swapBuffer, 0x40u);
          nAmbient = g_pObjPool[i].nAmbient;
          g_pObjPool[i].nAmbient = g_pObjPool[j].nAmbient;
          g_pObjPool[j].nAmbient = nAmbient;
          tempBitLightEnables = g_pObjPool[i].bitLightEnables;
          g_pObjPool[i].bitLightEnables = g_pObjPool[j].bitLightEnables;
          g_pObjPool[j].bitLightEnables = tempBitLightEnables;
        }
      }
    }
  }
}

// SetLightEnableMask(bitLightEnables): apply a 64-bit light-enable mask to the D3D
//   device - for each of 64 lights, LightEnable(i, (mask>>i)&1).
//   (D3D7 honors only D3D_MAX_LIGHTS=16; bits 16..63 are no-ops.)
//   Called per-object from HanaVert_DrawBatch:
//     - opaque/translucent passes: SetLightEnableMask(obj.bitLightEnables)
//     - glow pass: SetLightEnableMask(0) -> all lights off
void __cdecl hook_SetLightEnableMask(QWORD bitLightEnables)
{
  QWORD i; // [esp+8h] [ebp-8h]

  ONCELOG("[HNH:SUB] SetLightEnableMask_0040CE4E\n");

  for ( i = 0; i < 64; ++i )
  {
    if ( ((1LL << i) & bitLightEnables) != 0 )
      g_pD3DDevice_4D6C78->LightEnable(i, 1);
    else
      g_pD3DDevice_4D6C78->LightEnable(i, 0);
  }
}

void __cdecl hook_UpdateVertTextureFromFrame(CHanaVertT3EA_S1FC *pVertArg, CHanaFrameT3EC_S354 *pFrameArg)
{
  unsigned int textureWidth; // [esp+Ch] [ebp-E0h]
  struct IDirectDrawSurface7 *pSrcSurface; // [esp+10h] [ebp-DCh]
  struct tagRECT srcRect; // [esp+18h] [ebp-D4h] BYREF
  int textureHeight; // [esp+28h] [ebp-C4h]
  LPDIRECTDRAWSURFACE7 pDestSurface; // [esp+2Ch] [ebp-C0h]
  D3DXMATRIX translationMatrix; // [esp+30h] [ebp-BCh] BYREF
  int flipFlag; // [esp+70h] [ebp-7Ch]
  CHanaVertT3EA_S1FC *pVert; // [esp+74h] [ebp-78h]
  DWORD ambientColor; // [esp+78h] [ebp-74h] BYREF
  POINT min; // [esp+7Ch] [ebp-70h] BYREF
  DWORD oldAntialias; // [esp+84h] [ebp-68h] BYREF
  int xDest; // [esp+88h] [ebp-64h]
  HRESULT hr; // [esp+8Ch] [ebp-60h]
  RECT destRect; // [esp+90h] [ebp-5Ch] BYREF
  D3DXMATRIX worldViewProj; // [esp+A0h] [ebp-4Ch] BYREF
  DWORD fogEnabled; // [esp+E0h] [ebp-Ch] BYREF
  POINT max; // [esp+E4h] [ebp-8h] BYREF
  
  ONCELOG("[HNH:SUB] UpdateVertTextureFromFrame_439D48\n");

  pVert = 0;
  flipFlag = 0;
  CHanaObj__AddRef_40BFC3(pVertArg);
  CalcFrameBound_404B80(pFrameArg, (CHanaMorpT457_S40C *)pVertArg);
  CHanaObj__Free_40C1E4(pVertArg);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 3);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
  if ( pVertArg->dwType == 1002 )               // 0x3EA
  {
    pVert = pVertArg;
    if ( pVertArg->pTexT3EB )
    {
      hr = g_pD3DDevice_4D6C78->EndScene();
      if ( hr )
        g_pRenderCtx->Clear(3);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &oldAntialias);
      Sleep(0);
      qmemcpy(&worldViewProj, &pFrameArg->mat4x4_after, sizeof(worldViewProj));
      D3DXMatrixTranslation(
        &translationMatrix,
        pFrameArg->vertBoundMax_220.x,
        pFrameArg->vertBoundMax_220.y,
        pFrameArg->vertBoundMax_220.z);
      D3DXMatrixMultiply(&worldViewProj, &translationMatrix, &worldViewProj);
      ProjectToScreen_40E2A5(&worldViewProj, &max);
      qmemcpy(&worldViewProj, &pFrameArg->mat4x4_after, sizeof(worldViewProj));
      D3DXMatrixTranslation(
        &translationMatrix,
        pFrameArg->vertBoundMin_22C.x,
        pFrameArg->vertBoundMin_22C.y,
        pFrameArg->vertBoundMin_22C.z);
      D3DXMatrixMultiply(&worldViewProj, &translationMatrix, &worldViewProj);
      ProjectToScreen_40E2A5(&worldViewProj, &min);
      if ( max.x >= min.x )
      {
        destRect.right = max.x;
        destRect.left = min.x;
        flipFlag = 1;
      }
      else
      {
        destRect.left = max.x;
        destRect.right = min.x;
        flipFlag = 0;
      }
      if ( max.y >= min.y )
      {
        destRect.bottom = max.y;
        destRect.top = min.y;
      }
      else
      {
        destRect.top = max.y;
        destRect.bottom = min.y;
      }
      if ( destRect.left < 0 )
        destRect.left = 0;
      if ( destRect.right > ConvertFrom640AsInt(640) )
        destRect.right = ConvertFrom640AsInt(640);
      if ( destRect.top < 0 )
        destRect.top = 0;
      if ( destRect.bottom > ConvertFrom640AsInt(480) )
        destRect.bottom = ConvertFrom640AsInt(480);
      textureWidth = (destRect.right - destRect.left) / 14;
      textureHeight = (destRect.bottom - destRect.top) / 14;
      if ( textureWidth )
      {
        if ( textureWidth > 8 )
          textureWidth = 8;
      }
      else
      {
        textureWidth = 1;
      }
      if ( textureHeight )
      {
        if ( (unsigned int)textureHeight > 8 )
          textureHeight = 8;
      }
      else
      {
        textureHeight = 1;
      }
      pSrcSurface = CreateSurfaceWH(textureWidth, textureHeight);
      pDestSurface = CreateSurfaceWH(textureWidth, textureHeight);
      srcRect.left = 0;
      srcRect.top = 0;
      srcRect.right = textureWidth;
      srcRect.bottom = textureHeight;
      if ( !pSrcSurface->Blt(&srcRect, g_pBackBuffer_4D6C90, &destRect, 0x1000000, 0) )
      {
        if ( flipFlag )
        {
          for ( xDest = 0; xDest < textureWidth; ++xDest )
            BltFastWH(pDestSurface, xDest, 0, pSrcSurface, textureWidth - xDest - 1, 0, 1, textureHeight, 0);
          D3DXLoadTextureFromSurface(
            g_pD3DDevice_4D6C78,
            pVert->pTexT3EB->pDDSurf,
            0xFFFFFFFF,
            pDestSurface,
            &srcRect,
            0,
            D3DX_FT_POINT);
        }
        else
        {
          D3DXLoadTextureFromSurface(
            g_pD3DDevice_4D6C78,
            pVert->pTexT3EB->pDDSurf,
            0xFFFFFFFF,
            pSrcSurface,
            &srcRect,
            0,
            D3DX_FT_POINT);
        }
      }
      pSrcSurface->Release();
      pDestSurface->Release();
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, oldAntialias);
      Sleep(0);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      hr = g_pD3DDevice_4D6C78->BeginScene();
      if ( hr )
        g_pRenderCtx->Clear(3);
    }
  }
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 1);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 1);
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_AMBIENT, &ambientColor);
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_FOGENABLE, &fogEnabled);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, -1);
  HanaVertT3EA_Draw(pVert);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, fogEnabled);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, ambientColor);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER, 2);
}

// HanaVertT3EA_Draw(pVert): draw a CHanaVert geometry with full D3D7 state setup.
// - early-out if pVert->fShadowOnly_1E0 && shadows off (g_dwShadowOff)
// - texstage0 = modulate; material = pVert->pMateT3EE (or g_pMaterial),
//   skip if material diffuse.a <= 0.0
// - bind pVert->pTexT3EB (ALPHAOP per nAlphaOpSel_1F0, TEXPERSP per fNoTexPersp_1F4)
//   or no-texture modulate
// - COLOROP = select texture when fUnlit_1E4 or lighting disabled
// - alpha blend per nBlendMode_1D0: 1=SRCALPHA/INVSRCALPHA, -1=additive
// - shade: fFlatShade_1D8 ? FLAT : GOURAUD; fog density = nFogDensity_1EC
// - primitive per nPrimType_154: 0=Indexed TRIANGLELIST, 1=TRIANGLESTRIP, else=TRIANGLEFAN
// - restore LIGHTING=1, SHADEMODE=GOURAUD, ALPHABLENDENABLE=0
void __cdecl hook_HanaVertT3EA_Draw(CHanaVertT3EA_S1FC *pVert)
{
  ONCELOG("[HNH:SUB] HanaVertT3EA_Draw_417B27\n");

  if ( pVert->bShadowOnly_1E0 != 1 || g_dwShadowOff != 1 )
  {
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAOP, 4);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG1, 2);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG2, 0);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLOROP, 4);
    if ( pVert->pMateT3EE )
    {
      if ( pVert->pMateT3EE->d3dMaterial_130.diffuse.a <= 0.0 )
        return;
      HanaMate_SetMaterial(pVert->pMateT3EE);
    }
    else
    {
      HanaMate_SetMaterial(g_pMaterial);
    }
    if ( pVert->pTexT3EB )
    {
      g_pD3DDevice_4D6C78->SetTexture(0, pVert->pTexT3EB->pDDSurf);
      if ( pVert->nAlphaOpSel_1F0 )
      {
        if ( pVert->nAlphaOpSel_1F0 == 1 )
          g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 2);
        else
          g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 3);
      }
      else
      {
        g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ADDRESS, 1);
      }
      if ( pVert->fNoTexPersp_1F4 )
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0);
      else
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);
    }
    else
    {
      g_pD3DDevice_4D6C78->SetTexture(0, 0);
      g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAOP, 3);
      g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLOROP, 3);
    }
    if ( pVert->bUnlit_1E4 == 1 || !g_nLightningEnabled )
      g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLOROP, 2);
    if ( pVert->nBlendMode_1D0 == 1 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
    }
    else if ( pVert->nBlendMode_1D0 == -1 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 4);
    }
    if ( pVert->fFlatShade_1D8 == 1 )
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SHADEMODE, 1);
    else
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SHADEMODE, 2);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZBIAS, pVert->nFogDensity_1EC);
    if ( pVert->nPrimType_154[1] )
    {
      if ( pVert->nPrimType_154[1] == 1 )
        g_pD3DDevice_4D6C78->DrawPrimitiveVB(
          D3DPT_TRIANGLESTRIP,
          pVert->pD3DVtxBuf_130,
          0,
          pVert->dwNumVertices_134,
          0);
      else
        g_pD3DDevice_4D6C78->DrawPrimitiveVB(
          D3DPT_TRIANGLEFAN,
          pVert->pD3DVtxBuf_130,
          0,
          pVert->dwNumVertices_134,
          0);
    }
    else
    {
      g_pD3DDevice_4D6C78->DrawIndexedPrimitiveVB(
        D3DPT_TRIANGLELIST,
        pVert->pD3DVtxBuf_130,
        0,
        pVert->dwNumVertices_134,
        pVert->pVtxIndices,
        pVert->dwNumIndices_140,
        0);
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SHADEMODE, 2);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
  }
}

// Project a world point (matrix pPoint) to screen/NDC coordinates. Multiplies world-transform * projection * view * pPoint and writes _41/_44, _42/_44 (normalized x,y) into pScreenOut.
void __cdecl hook_ProjectToScreen_40E2A5(D3DXMATRIX *pM, POINT *pOut)
{
  D3DXMATRIX pM0; // [esp+0h] [ebp-100h] BYREF
  D3DXMATRIX pM3; // [esp+40h] [ebp-C0h] BYREF
  D3DXMATRIX pM2; // [esp+C0h] [ebp-40h] BYREF

  ONCELOG("[HNH:SUB] ProjectToScreen_40E2A5\n");

  BuildViewportMatrix();
  D3DXMatrixMultiply(&pM3, &g_mat4x4View_4D4780, &g_mat4x4Projection_4CE870);
  D3DXMatrixMultiply(&pM2, &pM3, &g_pM2);
  D3DXMatrixMultiply(&pM0, pM, &pM2);
  pOut->x = (__int64)(pM0._41 / pM0._44);
  pOut->y = (__int64)(pM0._42 / pM0._44);
}

// HanaMate_SetMaterial - push a HanaMate's D3D material to the device.
// Copies pMate->d3dMaterial_130, clamps diffuse alpha to [0,1], clears
// pMate->field_17C, and (when g_dwSpecularEnable==0, i.e. D3DRS_SPECULARENABLE
// is off) zeroes the specular color and power before SetMaterial().
void __cdecl hook_HanaMate_SetMaterial(CHanaMateT3EE_S184 *pMate)
{
  D3DMATERIAL7 mat; // [esp+0h] [ebp-44h] BYREF

  ONCELOG("[HNH:SUB] HanaMate_SetMaterial_40FB30\n");

  mat = pMate->d3dMaterial_130;

  if ( mat.diffuse.a < 0.0 )
    mat.diffuse.a = 0.0;
  if ( mat.diffuse.a > 1.0 )
    mat.diffuse.a = 1.0;
  pMate->fMatDirty = 0;
  if ( !g_bSpecularEnable )
  {
    memset(&mat.specular, 0, sizeof(mat.specular));
    mat.power = 0.0;
  }
  g_pD3DDevice_4D6C78->SetMaterial(&mat);
}

void __cdecl hook_BuildViewportMatrix()
{
  D3DVIEWPORT7 viewport; // [esp+20h] [ebp-24h] BYREF
  float halfHeight; // [esp+38h] [ebp-Ch]
  float halfWidth; // [esp+3Ch] [ebp-8h]
  float ProjectionFov_40DCF6; // [esp+40h] [ebp-4h]

  ONCELOG("[HNH:SUB] BuildViewportMatrix_40E167\n");

  *(_QWORD *)&g_pM2.m[3][1] = 0;
  *(_QWORD *)&g_pM2.m[2][3] = 0;
  *(_QWORD *)&g_pM2.m[2][0] = 0;
  *(_QWORD *)&g_pM2.m[1][2] = 0;
  *(_QWORD *)&g_pM2.m[0][3] = 0;
  *(_QWORD *)&g_pM2.m[0][1] = 0;
  g_pM2._44 = 1.0;
  g_pM2._33 = 1.0;
  g_pM2._22 = 1.0;
  g_pM2._11 = 1.0;
  ProjectionFov_40DCF6 = GetProjectionFov_40DCF6();
  g_pD3DDevice_4D6C78->GetViewport(&viewport);
  halfWidth = (double)viewport.dwWidth * 0.5;
  halfHeight = (double)viewport.dwHeight * 0.5;
  g_pM2._11 = halfWidth;
  g_pM2._22 = -halfHeight;
  g_pM2._41 = (double)viewport.dwX + halfWidth;
  g_pM2._42 = (double)viewport.dwY + halfHeight;
}

double hook_GetProjectionFov_40DCF6()
{
  ONCELOG("[HNH:SUB] GetProjectionFov_40DCF6\n");
  return g_fProjFov;
}

void __cdecl hook_UpdateBoneTransform(
  CHanaFrameT3EC_S354 *boneStruct,
  CHanaFrameT3EC_S354 *parentStruct,
  CHanaFrameT3EC_S354 *pSceneFrame)
{
  ONCELOG("[HNH:SUB] UpdateBoneTransform_408283\n");

  UpdateBoneLookAtTransform(boneStruct, parentStruct, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  CameraLookAtFrame(boneStruct, parentStruct, pSceneFrame, -1);
}

/**
 *  What it does - Gram-Schmidt basis + matrix reparenting
 *  
 *  1. Build an orthonormal basis (LH convention):
 *  pForward = normalize(dirForward);
 *  pRight   = normalize(crossUpFwd);       // v9 = pUp * pForward   -> right axis
 *  pUp      = normalize(crossFwdRight);    // v8 = pForward * pRight -> re-orthogonalized up
 *  Standard left-handed cross products: right = up * forward, up = forward * right - exactly what D3DXMatrixLookAtLH does
 *  internally, so the up input only needs to be roughly correct.
 *  
 *  2. Build the rotation matrix (columns = basis vectors):
 *  matLookRot = | pRight.x  pUp.x  pForward.x  0 |
 *               | pRight.y  pUp.y  pForward.y  0 |
 *               | pRight.z  pUp.z  pForward.z  0 |
 *               |    0        0        0       . |
 *  (Object-to-parent orientation whose columns are the object's axes in parent space. _44 is never initialized -
 *  harmless, since only the 3*3 part survives to the end.)
 *  
 *  3. Convert into the bone's local matrix:
 *  matLookRot = matLookRot * pParentFrame->mat4x4_after;      // parent space -> world space
 *  matBeforeInv = D3DXMatrixInverse(bone->mat4x4_before);     // bone rest/rest-pose inverse
 *  matLookRot = matLookRot * matBeforeInv;                    // strip bone's rest orientation
 *  
 *  4. Write back rotation-only (translation preserved) and recompose:
 *  pBoneFrame->mat4x4_fmrx._11.._33 = matLookRot._11.._33;    // 3*3 rotation only
 *  pBoneFrame->mat4x4_after = mat4x4_fmrx * mat4x4_before;    // world = local * parent-chain
 *  
 *  Net effect
 *  
 *  The bone's world orientation becomes LookRot * parentWorld - i.e., the frame is made to point its local +Z (and +Y)
 *  along the given forward/up directions expressed in the parent's space, while discarding the bone's own rest-pose
 *  rotation and keeping its position. This is the engine's per-bone aim-at constraint (eyes/head tracking, look-at IK
 *  style chains).
 */
void __cdecl hook_UpdateBoneLookAtTransform(
  CHanaFrameT3EC_S354 *pBoneFrame,
  CHanaFrameT3EC_S354 *pParentFrame,
  float dirForwardX,
  float dirForwardY,
  float dirForwardZ,
  float dirUpX,
  float dirUpY,
  float dirUpZ)
{
  D3DXVECTOR3 crossFwdRight; // [esp+0h] [ebp-BCh]
  D3DXVECTOR3 crossUpFwd; // [esp+Ch] [ebp-B0h]
  D3DXMATRIX matBeforeInv; // [esp+18h] [ebp-A4h] BYREF
  D3DXVECTOR3 pRight; // [esp+58h] [ebp-64h] BYREF
  D3DXMATRIX matLookRot; // [esp+64h] [ebp-58h] BYREF
  D3DXVECTOR3 pForward; // [esp+A4h] [ebp-18h] BYREF
  D3DXVECTOR3 pUp; // [esp+B0h] [ebp-Ch] BYREF

  ONCELOG("[HNH:SUB] UpdateBoneLookAtTransform_406CA4\n");

  pForward.x = dirForwardX;
  pForward.y = dirForwardY;
  pForward.z = dirForwardZ;
  D3DXVec3Normalize(&pForward, &pForward);
  pUp.x = dirUpX;
  pUp.y = dirUpY;
  pUp.z = dirUpZ;
  D3DXVec3Normalize(&pUp, &pUp);
  crossUpFwd.x = pUp.y * pForward.z - pUp.z * pForward.y;
  crossUpFwd.y = pUp.z * pForward.x - pUp.x * pForward.z;
  crossUpFwd.z = pUp.x * pForward.y - pUp.y * pForward.x;
  pRight = crossUpFwd;
  D3DXVec3Normalize(&pRight, &pRight);
  crossFwdRight.x = pForward.y * pRight.z - pForward.z * pRight.y;
  crossFwdRight.y = pForward.z * pRight.x - pForward.x * pRight.z;
  crossFwdRight.z = pForward.x * pRight.y - pForward.y * pRight.x;
  pUp = crossFwdRight;
  D3DXVec3Normalize(&pUp, &pUp);
  *(_QWORD *)&matLookRot._11 = __PAIR64__(LODWORD(pUp.x), LODWORD(pRight.x));
  *(_QWORD *)&matLookRot.m[1][0] = __PAIR64__(LODWORD(pUp.y), LODWORD(pRight.y));
  *(_QWORD *)&matLookRot.m[2][0] = __PAIR64__(LODWORD(pUp.z), LODWORD(pRight.z));
  *(_QWORD *)&matLookRot.m[0][2] = LODWORD(pForward.x);
  *(_QWORD *)&matLookRot.m[1][2] = LODWORD(pForward.y);
  matLookRot._33 = pForward.z;
  memset(&matLookRot.m[2][3], 0, 16);
  D3DXMatrixMultiply(&matLookRot, &matLookRot, &pParentFrame->mat4x4_after);
  D3DXMatrixInverse(&matBeforeInv, 0, &pBoneFrame->mat4x4_before);
  D3DXMatrixMultiply(&matLookRot, &matLookRot, &matBeforeInv);
  pBoneFrame->mat4x4_fmrx._11 = matLookRot._11;
  pBoneFrame->mat4x4_fmrx._21 = matLookRot._21;
  pBoneFrame->mat4x4_fmrx._31 = matLookRot._31;
  pBoneFrame->mat4x4_fmrx._12 = matLookRot._12;
  pBoneFrame->mat4x4_fmrx._22 = matLookRot._22;
  pBoneFrame->mat4x4_fmrx._32 = matLookRot._32;
  pBoneFrame->mat4x4_fmrx._13 = matLookRot._13;
  pBoneFrame->mat4x4_fmrx._23 = matLookRot._23;
  pBoneFrame->mat4x4_fmrx._33 = matLookRot._33;
  D3DXMatrixMultiply(&pBoneFrame->mat4x4_after, &pBoneFrame->mat4x4_fmrx, &pBoneFrame->mat4x4_before);
}

/**
 * CameraLookAtFrame(pCameraFrame, pTargetFrame, pSceneFrame, iMode)
 * Rebuilds the camera frame's orientation basis so it looks at pTargetFrame.
 * pCameraFrame: camera frame (g_pCameraFrame). pTargetFrame: frame to look at.
 * pSceneFrame: scene frame (g_pSceneFrame).
 * iMode: 0 = world up (0,1,0); 1 or 2 = no-op (returns iMode, matrices untouched);
 * otherwise (-1 at every current call site) = up taken from the camera's local
 * rotation relative to the scene (CalculateLocalRotationMatrix, middle column).
 * Steps (modes 0 and -1):
 * 1. pLookDir = normalize(target.pos - camera.pos); positions read from the
 *    mat4x4_after (offset 0x178) translation row m[3][0..2] (fTgtMatrix vs fCamMatrix).
 * 2. pRightVec = normalize(up x pLookDir); pUpVec = normalize(pLookDir x pRightVec);
 *    mode -1 zeroes up components < 0.001 before normalizing.
 * 3. Writes basis into pCameraFrame->mat4x4_after: _1* = right, _2* = up,
 *    _3* = look dir; translation m[3][0..2] (fCamTrans01/fCamTrans2) and m[0][3]
 *    / m[3][3] kept.
 * 4. mat4x4_fmrx (0x138) = mat4x4_after x inv(mat4x4_before (0x1B8)); returns
 *    &pCameraFrame->mat4x4_fmrx (all callers ignore the return).
 * Callers: sub_408283 (mode -1 wrapper), sub_466BDA (camera move-to-target,
 * mode -1 x3), sub_4686F8 (camera follow/physics), sub_46D9AF/sub_46F37E
 * (pause loop: look at the scene's "mune" frame when present).
 */
void __cdecl hook_CameraLookAtFrame(
  CHanaFrameT3EC_S354 *pCameraFrame,
  CHanaFrameT3EC_S354 *pTargetFrame,
  CHanaFrameT3EC_S354 *pSceneFrame,
  __int16 iMode)
{
  D3DXVECTOR3 crossFwdRightM1; // [esp+10h] [ebp-16Ch]
  D3DXVECTOR3 crossUpFwdM1; // [esp+1Ch] [ebp-160h]
  D3DXVECTOR3 crossFwdRight0; // [esp+28h] [ebp-154h]
  D3DXVECTOR3 crossUpFwd0; // [esp+34h] [ebp-148h]
  D3DXMATRIX matCamBeforeInvM1; // [esp+40h] [ebp-13Ch] BYREF
  D3DXMATRIX matCamBeforeInv0; // [esp+80h] [ebp-FCh] BYREF
  D3DXVECTOR3 vecRight; // [esp+C0h] [ebp-BCh] BYREF
  D3DXMATRIX fTgtMatrix; // [esp+CCh] [ebp-B0h] BYREF
  float pOutFwdZ; // [esp+10Ch] [ebp-70h] BYREF
  float pOutFwdY; // [esp+110h] [ebp-6Ch] BYREF
  float pOutFwdX; // [esp+114h] [ebp-68h] BYREF
  D3DXVECTOR3 qCamTransXY; // [esp+118h] [ebp-64h]
  D3DXVECTOR3 vecLookDir; // [esp+124h] [ebp-58h] BYREF
  D3DXVECTOR3 vecUp; // [esp+130h] [ebp-4Ch] BYREF
  D3DXMATRIX fCamMatrix; // [esp+13Ch] [ebp-40h] BYREF

  ONCELOG("[HNH:SUB] CameraLookAtFrame_40873E\n");

  qmemcpy(&fTgtMatrix, &pTargetFrame->mat4x4_after, sizeof(fTgtMatrix));
  qmemcpy(&fCamMatrix, &pCameraFrame->mat4x4_after, sizeof(fCamMatrix));
  qCamTransXY = *(D3DXVECTOR3 *)&fCamMatrix.m[3][0];
  if ( iMode )
  {
    if ( iMode != 1 && iMode != 2 )
    {
      vecLookDir.x = fTgtMatrix._41 - fCamMatrix._41;
      vecLookDir.y = fTgtMatrix._42 - fCamMatrix._42;
      vecLookDir.z = fTgtMatrix._43 - fCamMatrix._43;
      D3DXVec3Normalize(&vecLookDir, &vecLookDir);
      CalculateLocalRotationMatrix(
        pSceneFrame,
        pCameraFrame,
        &pOutFwdX,
        &pOutFwdY,
        &pOutFwdZ,
        &vecUp.x,
        &vecUp.y,
        &vecUp.z);
      if ( fabs(vecUp.x) < 0.001000000047497451 )
        vecUp.x = 0.0;
      if ( fabs(vecUp.y) < 0.001000000047497451 )
        vecUp.y = 0.0;
      if ( fabs(vecUp.z) < 0.001000000047497451 )
        vecUp.z = 0.0;
      D3DXVec3Normalize(&vecUp, &vecUp);
      crossUpFwdM1.x = vecUp.y * vecLookDir.z - vecUp.z * vecLookDir.y;
      crossUpFwdM1.y = vecUp.z * vecLookDir.x - vecUp.x * vecLookDir.z;
      crossUpFwdM1.z = vecUp.x * vecLookDir.y - vecUp.y * vecLookDir.x;
      vecRight = crossUpFwdM1;
      D3DXVec3Normalize(&vecRight, &vecRight);
      crossFwdRightM1.x = vecLookDir.y * vecRight.z - vecLookDir.z * vecRight.y;
      crossFwdRightM1.y = vecLookDir.z * vecRight.x - vecLookDir.x * vecRight.z;
      crossFwdRightM1.z = vecLookDir.x * vecRight.y - vecLookDir.y * vecRight.x;
      vecUp = crossFwdRightM1;
      D3DXVec3Normalize(&vecUp, &vecUp);
      pCameraFrame->mat4x4_after._11 = vecRight.x;
      pCameraFrame->mat4x4_after._21 = vecUp.x;
      pCameraFrame->mat4x4_after._31 = vecLookDir.x;
      pCameraFrame->mat4x4_after._12 = vecRight.y;
      pCameraFrame->mat4x4_after._22 = vecUp.y;
      pCameraFrame->mat4x4_after._32 = vecLookDir.y;
      pCameraFrame->mat4x4_after._13 = vecRight.z;
      pCameraFrame->mat4x4_after._23 = vecUp.z;
      pCameraFrame->mat4x4_after._33 = vecLookDir.z;
      *(D3DXVECTOR3 *)&pCameraFrame->mat4x4_after.m[3][0] = qCamTransXY;
      D3DXMatrixInverse(&matCamBeforeInvM1, 0, &pCameraFrame->mat4x4_before);
      D3DXMatrixMultiply(&pCameraFrame->mat4x4_fmrx, &pCameraFrame->mat4x4_after, &matCamBeforeInvM1);
    }
  }
  else
  {
    vecLookDir.x = fTgtMatrix._41 - fCamMatrix._41;
    vecLookDir.y = fTgtMatrix._42 - fCamMatrix._42;
    vecLookDir.z = fTgtMatrix._43 - fCamMatrix._43;
    D3DXVec3Normalize(&vecLookDir, &vecLookDir);
    vecUp.x = 0.0;
    vecUp.y = 1.0;
    vecUp.z = 0.0;
    D3DXVec3Normalize(&vecUp, &vecUp);
    crossUpFwd0.x = vecUp.y * vecLookDir.z - vecUp.z * vecLookDir.y;
    crossUpFwd0.y = vecUp.z * vecLookDir.x - vecUp.x * vecLookDir.z;
    crossUpFwd0.z = vecUp.x * vecLookDir.y - vecUp.y * vecLookDir.x;
    vecRight = crossUpFwd0;
    D3DXVec3Normalize(&vecRight, &vecRight);
    crossFwdRight0.x = vecLookDir.y * vecRight.z - vecLookDir.z * vecRight.y;
    crossFwdRight0.y = vecLookDir.z * vecRight.x - vecLookDir.x * vecRight.z;
    crossFwdRight0.z = vecLookDir.x * vecRight.y - vecLookDir.y * vecRight.x;
    vecUp = crossFwdRight0;
    D3DXVec3Normalize(&vecUp, &vecUp);
    pCameraFrame->mat4x4_after._11 = vecRight.x;
    pCameraFrame->mat4x4_after._21 = vecUp.x;
    pCameraFrame->mat4x4_after._31 = vecLookDir.x;
    pCameraFrame->mat4x4_after._12 = vecRight.y;
    pCameraFrame->mat4x4_after._22 = vecUp.y;
    pCameraFrame->mat4x4_after._32 = vecLookDir.y;
    pCameraFrame->mat4x4_after._13 = vecRight.z;
    pCameraFrame->mat4x4_after._23 = vecUp.z;
    pCameraFrame->mat4x4_after._33 = vecLookDir.z;
    *(D3DXVECTOR3 *)&pCameraFrame->mat4x4_after.m[3][0] = qCamTransXY;
    D3DXMatrixInverse(&matCamBeforeInv0, 0, &pCameraFrame->mat4x4_before);
    D3DXMatrixMultiply(&pCameraFrame->mat4x4_fmrx, &pCameraFrame->mat4x4_after, &matCamBeforeInv0);
  }
}

void __cdecl hook_AddObjectToPool(
  D3DXMATRIX *mat4x4After,
  CHanaFrameT3EC_S354 *pChildFrame,
  CHanaVertT3EA_S1FC *pVert,
  int nAmbient,
  QWORD qwBitLightEnables)
{
  CObjPoolMgr_S58 *pOldPool; // [esp+10h] [ebp-4h]

  ONCELOG("[HNH:SUB] AddObjectToPool_40CD32\n");

  if ( g_nObjCount >= g_nCapObjects_4CD780 - 1 )
  {
    pOldPool = g_pObjPool;
    g_pObjPool = (CObjPoolMgr_S58 *)MemAlloc_40DEF7(0x58 * (g_nCapObjects_4CD780 + 256));
    qmemcpy(g_pObjPool, pOldPool, 4 * ((unsigned int)(0x58 * g_nCapObjects_4CD780) >> 2));
    MemFree_40DF64(pOldPool);
    g_nCapObjects_4CD780 += 256;
  }
  qmemcpy(&g_pObjPool[g_nObjCount], mat4x4After, 0x40u);
  g_pObjPool[g_nObjCount].pFrame = pChildFrame;
  g_pObjPool[g_nObjCount].pVert = pVert;
  g_pObjPool[g_nObjCount].fNeedsDraw_48 = 1;
  g_pObjPool[g_nObjCount].nAmbient = nAmbient;
  g_pObjPool[g_nObjCount++].bitLightEnables = qwBitLightEnables;
}

void(__cdecl*HanaT3FB_Draw)(CHanaBase_S130* pObj) = reinterpret_cast<void(__cdecl*)(CHanaBase_S130*)>(0x416776);

DWORD __cdecl hook_CHanaObj__GetChildCountT3F5_40FF0F(CHanaMeshT3F5_S138 *pHanaT3F5)
{
  ONCELOG("[HNH:SUB] CHanaObj__GetChildCountT3F5_40FF0F\n");

  return pHanaT3F5->nCountVertT3EA_130;
}

// HanaMeshT3F5_Draw(pMesh): draw a compound CHanaMeshT3F5 = an array of
//   CHanaVertT3EA children (refsVertT3EA[0..nCountVertT3EA_130)).
//   For each child:
//     - if child->nPrimType_154[0] != 0: clear it and skip this child this frame
//     - else: HanaVert_Draw(child)
//   (nPrimType_154[1] is the primitive type used inside HanaVert_Draw; [0]
//    doubles as a per-child skip flag here.)
//   Polymorphic target of HanaVert_DrawByType (dwType 0x3F5); also called from
//   sub_434FC9 (the 0x457 morph draw, which shares this child-vert layout).
void __cdecl hook_HanaMeshT3F5_Draw(CHanaMeshT3F5_S138 *pMesh)
{
  unsigned int i; // [esp+0h] [ebp-8h]

  ONCELOG("[HNH:SUB] HanaMeshT3F5_Draw_40FE8E\n");

  for ( i = 0; i < pMesh->nCountVertT3EA_130; ++i )
  {
    if ( pMesh->refsVertT3EA[i]->nPrimType_154[0] )
      pMesh->refsVertT3EA[i]->nPrimType_154[0] = 0;
    else
      HanaVertT3EA_Draw(pMesh->refsVertT3EA[i]);
  }
}

// HanaMorpT457_Draw(pMorp): dwType 0x457 geometry draw. A CHanaMorpT457 wraps a
//   CHanaMeshT3F5 (pMorp->pMeshT3F5_138); drawing it is a pure delegation to
//   HanaMeshT3F5_Draw (which draws the child-vert array).
//   Confirms the morph geometry shares the 0x3F5 mesh child-vert layout.
//   Polymorphic target of HanaVert_DrawByType (dwType 0x457).
void __cdecl hook_HanaMorpT457_Draw(CHanaMorpT457_S40C *pMorp)
{
  ONCELOG("[HNH:SUB] HanaMorpT457_Draw_434FC9\n");

  HanaMeshT3F5_Draw(pMorp->pMeshT3F5_138);
}

// HanaVert_DrawByType(pVert): dispatch a CHana geometry object to its
//   type-specific D3D draw routine, selected by pVert->dwType:
//     0x3EA  HanaVert_Draw   (CHanaVertT3EA_S1FC)
//     0x3F5  sub_40FE8E      (CHanaMeshT3F5_S138)   mesh
//     0x3FB  HanaMesh_Draw   (CHana3FB_S160)
//     0x457  sub_434FC9      (CHanaMorpT457_S40C)   morph
//     else   no-op (return 0)
//   The 'T####' suffix in each CHana* struct name == this dwType tag.
//   Called from HanaVert_DrawBatch (opaque + translucent passes) and sub_40600C.
void __cdecl hook_HanaVert_DrawByType(CHanaVertT3EA_S1FC *pObj)
{
  ONCELOG("[HNH:SUB] HanaVert_DrawByType_40C54C\n");

  switch ( pObj->dwType )
  {
  case 0x3EA:
    HanaVertT3EA_Draw((CHanaVertT3EA_S1FC*)pObj);
    break;
  case 0x3F5:
    HanaMeshT3F5_Draw((CHanaMeshT3F5_S138 *)pObj);
    break;
  case 0x3FB:
    HanaT3FB_Draw((CHanaBase_S130 *)pObj);
    break;
  case 0x457:
    HanaMorpT457_Draw((CHanaMorpT457_S40C *)pObj);
    break;
  default:
    return;
  }
}

void register_ProcessFrame_40600C() {
    hnh_Hook(0x40600C, hook_ProcessFrame, "ProcessFrame_40600C"); // [OK]
    
    hnh_Hook(0x439D48, hook_UpdateVertTextureFromFrame, "UpdateVertTextureFromFrame_439D48");
    hnh_Hook(0x40E2A5, hook_ProjectToScreen_40E2A5, "ProjectToScreen_40E2A5");

    hnh_HookOff();
    hnh_Hook(0x40CD32, hook_AddObjectToPool, "AddObjectToPool_40CD32"); // [OK]
    hnh_Hook(0x40D9C7, hook_SortObjectsByDepth, "SortObjectsByDepth_40D9C7"); // [OK]
    hnh_Hook(0x40873E, hook_CameraLookAtFrame, "CameraLookAtFrame_40873E"); // [OK]
    hnh_Hook(0x40DCF6, hook_GetProjectionFov_40DCF6, "GetProjectionFov_40DCF6"); // [OK]
    hnh_Hook(0x40CE4E, hook_SetLightEnableMask, "SetLightEnableMask_0040CE4E"); // [OK]
    hnh_Hook(0x40E167, hook_BuildViewportMatrix, "BuildViewportMatrix_40E167"); // [OK]
    hnh_Hook(0x417B27, hook_HanaVertT3EA_Draw, "HanaVertT3EA_Draw_417B27"); // [OK]
    hnh_Hook(0x40FB30, hook_HanaMate_SetMaterial, "HanaMate_SetMaterial_40FB30"); // [OK]
    hnh_Hook(0x408283, hook_UpdateBoneTransform, "UpdateBoneTransform_408283"); // [OK]
    hnh_Hook(0x406CA4, hook_UpdateBoneLookAtTransform, "UpdateBoneLookAtTransform_406CA4"); // [OK]
    hnh_Hook(0x40FF0F, hook_CHanaObj__GetChildCountT3F5_40FF0F, "CHanaObj__GetChildCountT3F5_40FF0F"); // [OK]
    hnh_Hook(0x40C54C, hook_HanaVert_DrawByType, "HanaVert_DrawByType_40C54C"); // [OK]
    hnh_Hook(0x40FE8E, hook_HanaMeshT3F5_Draw, "HanaMeshT3F5_Draw_40FE8E"); // [OK]
    hnh_Hook(0x434FC9, hook_HanaMorpT457_Draw, "HanaMorpT457_Draw_434FC9"); // [OK]

    hnh_HookOn();

}
