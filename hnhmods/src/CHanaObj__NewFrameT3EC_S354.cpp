// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"

// === GLOBAL FUNCTIONS ===

void(__cdecl *CHanaObj__NewFrameT3EC_S354)(CHanaFrameT3EC_S354 *pParent, CHanaFrameT3EC_S354 **ppOutFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354*, CHanaFrameT3EC_S354**)>(0x404720);
void(__cdecl* AttachChild_4055FD)(CHanaFrameRootT3EC_S2D4* pParent, CHanaFrameT3EC_S354* pChild) = reinterpret_cast<void(__cdecl *)(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild)>(0x4055FD);
void(__cdecl* MtxStackStartMultLocal_4074EF)(void) = reinterpret_cast<void(__cdecl*)(void)>(0x4074EF);
void(__cdecl* MtxStackRecursiveMulLocal_4075B0)(CHanaFrameT3EC_S354* pFrame) = reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x4075B0);
void(__cdecl* CHanaObj__NewFrameRootT3EC_S2D4)(CHanaFrameRootT3EC_S2D4 **ppOutFrameRoot) = reinterpret_cast<void(__cdecl*)(CHanaFrameRootT3EC_S2D4**)>(0x404908);
void(__cdecl* CHanaObj__NewMateT3EE_S184)(CHanaMateT3EE_S184** ppOutMate) = reinterpret_cast<void(__cdecl*)(CHanaMateT3EE_S184**)>(0x40F720);
void (__cdecl*CHanaObj__NewSprite3F6byWH)(CHanaSpriteT3F6_S1FC** ppOut, DWORD w, DWORD h) = reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC **ppOut, DWORD w, DWORD h)>(0x4133D0);

// === FUNCITON HOOKS ===
// 
// CHanaObj__NewFrameT3EC_S354 at 0x404720
// Creates and initializes a new CHanaFrameT3EC_S354 object (size 0x354 bytes)
// Attaches it to parent frame or scene root if no parent

void __cdecl hook_CHanaObj__NewFrameT3EC_S354(CHanaFrameT3EC_S354 *pParent, CHanaFrameT3EC_S354 **ppOutFrame)
{
    CHanaFrameT3EC_S354 *pNewFrame;

    ONCELOG("[HNH:SUB] CHanaObj__NewFrameT3EC_S354_404720\n");

    // Allocate memory (0x354 bytes = 852 decimal)
    // Note: memset only clears 0x2D4 bytes (same as FrameRoot size), leaving extra 0x80 bytes uninitialized
    pNewFrame = (CHanaFrameT3EC_S354*)MemAlloc_40DEF7(0x354u);
    memset(pNewFrame, 0, 0x2D4u);

    // Set type identifier (same as FrameRoot: 0x3EC = 1004)
    pNewFrame->dwType = 0x3EC;

    // Initialize two 4x4 matrices to identity
    // matLocal_138 (frame matrix)
    pNewFrame->matLocal_138._11 = 1.0f;
    pNewFrame->matLocal_138._22 = 1.0f;
    pNewFrame->matLocal_138._33 = 1.0f;
    pNewFrame->matLocal_138._44 = 1.0f;
    // Off-diagonal elements zeroed by memset, explicit zero stores for specific elements

    // matWorld_178
    pNewFrame->matWorld_178._11 = 1.0f;
    pNewFrame->matWorld_178._22 = 1.0f;
    pNewFrame->matWorld_178._33 = 1.0f;
    pNewFrame->matWorld_178._44 = 1.0f;

    // Initialize child count
    pNewFrame->nCountChild = 0;

    // Increment reference count
    CHanaObj__AddRef_40BFC3(pNewFrame);

    // Attach to parent frame or scene root
    if (pParent)
        AttachChild_4055FD(pParent, pNewFrame);
    else
        AttachChild_4055FD(g_pSceneFrame, pNewFrame);  // Attach to global scene frame

    // Clear animation pointer (offset 0x2C0)
    pNewFrame->pHanaAnim_2C0 = 0;

    // Return the new frame
    *ppOutFrame = pNewFrame;
}


// MtxStackRecursiveMulLocal_4075B0 at 0x4075B0
// Recursively computes world matrices for a frame and its children
// Uses the global D3DX matrix stack (g_pMatrixStack_4CD77C)

void __cdecl hook_MtxStackRecursiveMulLocal_4075B0(CHanaFrameT3EC_S354 *pFrame)
{
  unsigned int i;
  CHanaNode *pChildNode;

  ONCELOG("[HNH:SUB] MtxStackRecursiveMulLocal_4075B0\n");

  // Copy current stack top (parent's world matrix) to frame's matParentWorld_1B8
  qmemcpy(
    &pFrame->matParentWorld_1B8,
    g_pMatrixStack_4CD77C->GetTop(),
    sizeof(pFrame->matParentWorld_1B8));

  // Push new matrix onto stack
  g_pMatrixStack_4CD77C->Push();

  // Multiply by this frame's local matrix (matLocal_138)
  g_pMatrixStack_4CD77C->MultMatrixLocal(&pFrame->matLocal_138);

  // Copy result (this frame's world matrix) to matWorld_178
  qmemcpy(
    &pFrame->matWorld_178,
    g_pMatrixStack_4CD77C->GetTop(),
    sizeof(pFrame->matWorld_178));

  // Mark as processed/root (though this seems odd for non-root frames)
  pFrame->bIsRoot_1F8 = 1;

  // Recursively process all children
  if (pFrame->nCountChild > 0)
  {
    pChildNode = pFrame->pFirstChildNode;
    for (i = 0; i < pFrame->nCountChild; ++i)
    {
      MtxStackRecursiveMulLocal_4075B0(pChildNode->pData);
      pChildNode = pChildNode->pNext;
    }
  }

  // Pop this frame's matrix from stack (restore parent's matrix)
  g_pMatrixStack_4CD77C->Pop();
}

// CHanaObj__NewFrameRootT3EC_S2D4 at 0x404908
// Creates and initializes a new CHanaFrameRootT3EC_S2D4 object (size 0x2D4 bytes)

void __cdecl hook_CHanaObj__NewFrameRootT3EC_S2D4(CHanaFrameRootT3EC_S2D4 **ppOutFrameRoot)
{
  CHanaFrameRootT3EC_S2D4 *pFrameRoot;

  ONCELOG("[HNH:SUB] CHanaObj__NewFrameRootT3EC_S2D4_404908\n");

  // Allocate memory (0x2D4 bytes = 724 decimal)
  pFrameRoot = (CHanaFrameRootT3EC_S2D4*) MemAlloc_40DEF7(0x2D4u);
  memset(pFrameRoot, 0, sizeof(CHanaFrameRootT3EC_S2D4));

  // Set type identifier
  pFrameRoot->dwType = 0x3EC;  // 1004 decimal

  // Initialize three 4x4 matrices to identity
  // matLocal_138 (frame matrix?)
  pFrameRoot->matLocal_138._11 = 1.0f;
  pFrameRoot->matLocal_138._22 = 1.0f;
  pFrameRoot->matLocal_138._33 = 1.0f;
  pFrameRoot->matLocal_138._44 = 1.0f;
  // Off-diagonal elements zeroed by memset, explicit zero stores:
  // m[3][1], m[2][3], m[2][0], m[1][2], m[0][3], m[0][1] = 0

  // matWorld_178
  pFrameRoot->matWorld_178._11 = 1.0f;
  pFrameRoot->matWorld_178._22 = 1.0f;
  pFrameRoot->matWorld_178._33 = 1.0f;
  pFrameRoot->matWorld_178._44 = 1.0f;

  // matParentWorld_1B8
  pFrameRoot->matParentWorld_1B8._11 = 1.0f;
  pFrameRoot->matParentWorld_1B8._22 = 1.0f;
  pFrameRoot->matParentWorld_1B8._33 = 1.0f;
  pFrameRoot->matParentWorld_1B8._44 = 1.0f;

  // Initialize root frame flags
  pFrameRoot->bIsRoot_1F8 = 1;      // This is a root frame
  pFrameRoot->nCountChild = 0;      // No children initially
  pFrameRoot->nCount_12C++;         // Increment reference/instance count

  // Return the new object
  *ppOutFrameRoot = pFrameRoot;
}


// MtxStackStartMultLocal_4074EF at 0x4074EF
// Creates a D3DX matrix stack, loads the scene root frame matrix,
// recursively multiplies local matrices for all child frames,
// then releases the stack

void __cdecl hook_MtxStackStartMultLocal_4074EF()
{
  unsigned int i;
  CHanaNode *pChildNode;

  ONCELOG("[HNH:SUB] MtxStackStartMultLocal_4074EF\n");

  // Create matrix stack
  D3DXCreateMatrixStack(0, &g_pMatrixStack_4CD77C);

  // Push initial matrix and load scene root frame matrix
  g_pMatrixStack_4CD77C->Push();
  g_pMatrixStack_4CD77C->LoadMatrix(&g_pSceneFrame->matLocal_138);

  // Recursively multiply local matrices for all children of scene root
  if (g_pSceneFrame->nCountChild > 0)
  {
    pChildNode = g_pSceneFrame->pFirstChildNode;
    for (i = 0; i < g_pSceneFrame->nCountChild; ++i)
    {
      MtxStackRecursiveMulLocal_4075B0(pChildNode->pData);
      pChildNode = pChildNode->pNext;
    }
  }

  // Pop and release stack
  g_pMatrixStack_4CD77C->Pop();
  g_pMatrixStack_4CD77C->Release();
}

// AttachChild_4055FD at 0x4055FD
// Attaches a child frame to a parent frame in the scene hierarchy
// Uses a doubly-linked list of CHanaNode structures

void __cdecl hook_AttachChild_4055FD(CHanaFrameRootT3EC_S2D4 *pParent, CHanaFrameT3EC_S354 *pChild)
{
  CHanaNode *pNewNode;

  ONCELOG("[HNH:SUB] AttachChild_4055FD\n");

  // If child is already attached to this parent, do nothing
  if (pChild->pParent == pParent)
    return;

  // Add child to parent's child list
  if (pParent->nCountChild > 0)
  {
    // Parent already has children - append to end of list
    pNewNode = (CHanaNode *)MemAlloc_40DEF7(0xCu);  // Allocate new node (12 bytes)
    pNewNode->pNext = 0;
    pNewNode->pPrev = pParent->pLastChildNode;
    pNewNode->pData = pChild;
    pParent->pLastChildNode->pNext = pNewNode;
    pParent->pLastChildNode = pNewNode;
    ++pParent->nCountChild;
  }
  else
  {
    // First child - create the list
    pParent->pFirstChildNode = (CHanaNode *) MemAlloc_40DEF7(0xCu);
    memset(pParent->pFirstChildNode, 0, sizeof(CHanaNode));
    pParent->pLastChildNode = pParent->pFirstChildNode;
    pParent->nCountChild = 1;
    pParent->pFirstChildNode->pData = pChild;
    pParent->pFirstChildNode->pPrev = 0;
    pParent->pFirstChildNode->pNext = 0;
  }

  // If child was attached to another parent, remove it from there
  if (pChild->pParent)
    RemoveChild_4058C3(pChild->pParent, pChild);

  // Set new parent
  pChild->pParent = (CHanaFrameT3EC_S354*)pParent;

  // Notify matrix stack system
  MtxStackStartMultLocal_4074EF();
}

// CHanaObj__NewMateT3EE_S184 at 0x40F720
// Creates and initializes a new CHanaMateT3EE_S184 object (size 0x184 bytes)

void __cdecl hook_CHanaObj__NewMateT3EE_S184(CHanaMateT3EE_S184 **ppOutMate)
{
  CHanaMateT3EE_S184 *pMate;

  ONCELOG("[HNH:SUB] CHanaObj__NewMateT3EE_S184_40F720\n");

  // Allocate memory (0x184 bytes = 388 decimal)
  pMate = (CHanaMateT3EE_S184 *)MemAlloc_40DEF7(0x184u);
  memset(pMate, 0, sizeof(CHanaMateT3EE_S184));

  // Set type identifier
  pMate->dwType = 0x3EE;  // 1006 decimal

  // Initialize D3D material to white diffuse/ambient, black specular/emissive
  pMate->d3dMaterial_130.diffuse.r = 1.0f;
  pMate->d3dMaterial_130.diffuse.g = 1.0f;
  pMate->d3dMaterial_130.diffuse.b = 1.0f;
  pMate->d3dMaterial_130.diffuse.a = 1.0f;

  pMate->d3dMaterial_130.ambient.r = 1.0f;
  pMate->d3dMaterial_130.ambient.g = 1.0f;
  pMate->d3dMaterial_130.ambient.b = 1.0f;
  pMate->d3dMaterial_130.ambient.a = 1.0f;

  // Specular and emissive set to black (0)
  *(DWORD *)&pMate->d3dMaterial_130.specular.r = 0;  // r,g = 0
  *(DWORD *)&pMate->d3dMaterial_130.specular.b = 0;  // b,a = 0
  *(DWORD *)&pMate->d3dMaterial_130.emissive.r = 0;  // r,g = 0
  *(DWORD *)&pMate->d3dMaterial_130.emissive.b = 0;  // b,a = 0

  pMate->d3dMaterial_130.power = 0.0f;

  // Cache diffuse alpha and mark material as dirty
  pMate->fDiffuseA = pMate->d3dMaterial_130.diffuse.a;
  pMate->fMatDirty = 1;

  // Increment reference count
  CHanaObj__AddRef_40BFC3(pMate);

  // Return the new object
  *ppOutMate = pMate;
}

void hook_CHanaObj__NewSprite3F6byWH(CHanaSpriteT3F6_S1FC **ppOut, DWORD w, DWORD h)
{
  _D3DX_SURFACEFORMAT pixelformat; // [esp+8h] [ebp-14h] BYREF
  DWORD flags; // [esp+Ch] [ebp-10h] BYREF
  LPDIRECTDRAWSURFACE7 pSurf_v1; // [esp+10h] [ebp-Ch]
  CHanaSpriteT3F6_S1FC *pSprite; // [esp+14h] [ebp-8h]
  HRESULT hr; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__NewSprite3F6byWH_4133D0\n");

  pixelformat = D3DX_SF_R8G8B8;
  flags = 256;
  pSprite = (CHanaSpriteT3F6_S1FC *)MemAlloc_40DEF7(0x1FCu);
  memset(pSprite, 0, sizeof(CHanaSpriteT3F6_S1FC));
  pSprite->dwType = 0x3F6;
  hr = D3DXCreateTexture(g_pD3DDevice_4D6C78, &flags, &w, &h, &pixelformat, 0, &pSprite->pTexture_134, 0);
  if ( hr < 0 )
    PrintDebugError(hr);
  pSurf_v1 = CreateSurfaceWH(16u, 0x10u);
  BltSurface(pSurf_v1, 0, 0, 16, 16, 0);
  D3DXLoadTextureFromSurface(g_pD3DDevice_4D6C78, pSprite->pTexture_134, 0xFFFFFFFF, pSurf_v1, 0, 0, D3DX_FT_POINT);
  pSurf_v1->Release();
  pSprite->ddSurfaceDesc_170.dwSize = 124;
  pSprite->ddSurfaceDesc_170.dwFlags = 4102;
  pSprite->pTexture_134->GetSurfaceDesc(&pSprite->ddSurfaceDesc_170);
  pSprite->dwRGBBitCount_1EC = pSprite->ddSurfaceDesc_170.ddpfPixelFormat.dwRGBBitCount;
  pSprite->dwWidth_1F0 = pSprite->ddSurfaceDesc_170.dwWidth;
  pSprite->dwHeight_1F4 = pSprite->ddSurfaceDesc_170.dwHeight;
  pSprite->lPitch_1F8 = pSprite->ddSurfaceDesc_170.lPitch;
  pSprite->fAlpha_144 = 1.0;
  pSprite->fScale_148 = 1.0;
  pSprite->pCenter_158 = 0;
  pSprite->fHalfWidth_150 = (float)(pSprite->dwWidth_1F0 / 2);
  pSprite->fHalfHeight_154 = (float)(pSprite->dwHeight_1F4 / 2);
  pSprite->pRect_16C = 0;
  pSprite->rect_15C.top = 0;
  pSprite->rect_15C.left = 0;
  pSprite->rect_15C.bottom = pSprite->dwHeight_1F4;
  pSprite->rect_15C.right = pSprite->dwWidth_1F0;
  CHanaObj__AddRef_40BFC3((CHanaBase_S130*)pSprite);
  *ppOut = pSprite;
}

// Register hook if needed
void register_CHanaObj__NewFrameT3EC_S354_404720(int cfg) {
  if (cfg == 1) {
    CHanaObj__NewFrameT3EC_S354 = hook_CHanaObj__NewFrameT3EC_S354;
    AttachChild_4055FD = hook_AttachChild_4055FD;
    MtxStackStartMultLocal_4074EF = hook_MtxStackStartMultLocal_4074EF;
    MtxStackRecursiveMulLocal_4075B0 = hook_MtxStackRecursiveMulLocal_4075B0;
    CHanaObj__NewFrameRootT3EC_S2D4 = hook_CHanaObj__NewFrameRootT3EC_S2D4;
    CHanaObj__NewMateT3EE_S184 = hook_CHanaObj__NewMateT3EE_S184;
    CHanaObj__NewSprite3F6byWH = hook_CHanaObj__NewSprite3F6byWH;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x4074EF, hook_MtxStackStartMultLocal_4074EF, "MtxStackStartMultLocal_4074EF"); // [OK] 
  hnh_Hook(0x4075B0, hook_MtxStackRecursiveMulLocal_4075B0, "MtxStackRecursiveMulLocal_4075B0"); // [OK] 

  hnh_Hook(0x404720, hook_CHanaObj__NewFrameT3EC_S354, "CHanaObj__NewFrameT3EC_S354_404720"); // [OK] 
  hnh_Hook(0x4055FD, hook_AttachChild_4055FD, "AttachChild_4055FD"); // [OK] 
  hnh_Hook(0x404908, hook_CHanaObj__NewFrameRootT3EC_S2D4, "CHanaObj__NewFrameRootT3EC_S2D4_404908"); // [OK] 
  hnh_Hook(0x40F720, hook_CHanaObj__NewMateT3EE_S184, "CHanaObj__NewMateT3EE_S184_40F720"); // [OK] 
  hnh_Hook(0x4133D0, hook_CHanaObj__NewSprite3F6byWH, "CHanaObj__NewSprite3F6byWH_4133D0"); // [OK] 
  hnh_HookOn();
}