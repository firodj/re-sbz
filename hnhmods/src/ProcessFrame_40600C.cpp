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
int  (__cdecl*AlwaysTrue)(void) = reinterpret_cast<int(__cdecl*)()>(0x4084C2);
int  (__cdecl*AddObjectToPool)(D3DXMATRIX *mat4x4After, CHanaFrameT3EC_S354 *pChildFrame,
    CHanaVertT3EA_S1FC *pVert, int nAmbient, QWORD qwBitLightEnables) =
    reinterpret_cast<int(__cdecl*)(D3DXMATRIX *, CHanaFrameT3EC_S354 *, CHanaVertT3EA_S1FC *, int, QWORD)>(0x40CD32);
DWORD (__cdecl*CHanaObj__GetChildCountT3F5_40FF0F)(CHanaMeshT3F5_S138 *pHanaT3F5) =
    reinterpret_cast<DWORD(__cdecl*)(CHanaMeshT3F5_S138 *)>(0x40FF0F);
int  (__cdecl*HanaVert_DrawByType)(CHanaVertT3EA_S1FC *pVert) =
    reinterpret_cast<int(__cdecl*)(CHanaVertT3EA_S1FC *)>(0x40C54C);

// === PINNED GLOBALS ===
uint32_t &g_fRendering    = *(uint32_t *)0x4CD778;  // WRITABLE
uint32_t &g_fObjBatchActive = *(uint32_t *)0x4D6CD8; // WRITABLE

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

    DebugLog("[HNH:SUB] ProcessFrame_40600C\n");

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

void register_ProcessFrame_40600C() {
    hnh_Hook(0x40600C, hook_ProcessFrame, "ProcessFrame_40600C");
}
