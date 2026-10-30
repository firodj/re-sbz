#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// PlayScreenFade_4874DE -- the universal screen-fade transition (24 xrefs:
// every scene function, SystemMenu, menus, ParseExecStr, HanaSurface helper).
//
//   bFadeOut:  0 = fade in  (alpha 0 -> 1)
//              1 = fade out (alpha 1 -> 0)
//              >1 = setup only, returns before animating -- and LEAKS the two
//                   sprites and the captured frame (no caller passes > 1)
//   colorref:  solid fade color
//   pDdSurface: optional pre-fade background; NULL = re-render the 3D scene
//               (RenderClear + HanaFrame_Draw over the scene frame)
//   fTimeScale: timer scale for the fade duration (SetTimerScaleFactor)
//   bFlush:    RenderFlush_4874A5() the captured pre-fade frame first
//
// Setup: a 128x128 solid-color tile -> color sprite, plus a second sprite
// textured by CreateTextureFromSurfaceWithAlphaMask(tile, Darken.BMP) for the
// darken overlay. The pre-fade frame is captured into an offscreen surface.
// Each frame: blit the captured frame back, then draw color rect + darken
// overlay twice -- pass 1 DESTBLEND=INVSRCALPHA, pass 2 DESTBLEND=D3DBLEND_ONE
// (SRCALPHA both times). Alpha steps 0.025 * frameTime*2.1 (min 0.01) per
// frame; the darken sprite runs at alpha * 1.25. Teardown frees everything,
// restores Z states, frame timer and timer scale, and ANTIALIAS.

// === PINNED GAME FUNCTIONS ===
LPDIRECTDRAWSURFACE7(__cdecl* PlayScreenFade)(int bisGo, COLORREF colorref,
  LPDIRECTDRAWSURFACE7 pDdSurface, float fFade, char bFlags) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)(int, COLORREF,
      LPDIRECTDRAWSURFACE7, float, char)>(0x4874DE);

DWORD(__cdecl* RGBToSurfaceFormat)(LPDIRECTDRAWSURFACE7 surface, COLORREF color) =
    reinterpret_cast<DWORD(__cdecl*)(LPDIRECTDRAWSURFACE7, COLORREF)>(0x41973F);
int(__cdecl* CHanaObj__NewSpriteT3F6bySurf)(CHanaSpriteT3F6_S1FC **pOut, LPDIRECTDRAWSURFACE7 pSurfaceSrc) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC **, LPDIRECTDRAWSURFACE7)>(0x4137D1);
int(__cdecl* CHanaObj__ReplaceTexture3F6)(CHanaSpriteT3F6_S1FC *pSprite, LPDIRECTDRAWSURFACE7 *pTexture) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, LPDIRECTDRAWSURFACE7 *)>(0x413D78);
void(__cdecl* CHanaObj__SetAlpha3F6)(CHanaSpriteT3F6_S1FC *pSprite, float alpha) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *, float)>(0x414ABA);
int(__cdecl* DrawSpriteLTRB)(CHanaSpriteT3F6_S1FC *pSprite, int left, int top, int right, int bottom) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *, int, int, int, int)>(0x414589);
int(__cdecl* BltFastWH)(LPDIRECTDRAWSURFACE7 pSurf, int xDest, int yDest, LPDIRECTDRAWSURFACE7 pSrc,
    int left, int top, int width, int height, int ddbltfastFlags) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7, int, int, LPDIRECTDRAWSURFACE7,
      int, int, int, int, int)>(0x419D16);
void(__cdecl* RenderFlush_4874A5)(void) = reinterpret_cast<void(__cdecl*)()>(0x4874A5);
HRESULT(__cdecl* RenderClear_40BF4F)(DWORD ClearFlags) =
    reinterpret_cast<HRESULT(__cdecl*)(DWORD)>(0x40BF4F);
HRESULT(__cdecl* HanaFrame_Draw)(CHanaFrameT3EC_S354 *pFrame) =
    reinterpret_cast<HRESULT(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x405C06);
double(__cdecl* GetTimerScaleFactor)(void) = reinterpret_cast<double(__cdecl*)()>(0x47E78F);
void(__cdecl* SetTimerScaleFactor)(float fTimeScale) =
    reinterpret_cast<void(__cdecl*)(float)>(0x47E782);
int(__cdecl* FrameTimer_BeginFrame)(void) = reinterpret_cast<int(__cdecl*)()>(0x47E768);
DWORD(__cdecl* FrameTimer_EndFrame)(void) = reinterpret_cast<DWORD(__cdecl*)()>(0x47E7D5);
double(__cdecl* GetFrameTime)(void) = reinterpret_cast<double(__cdecl*)()>(0x47E79A);
void (__cdecl * HanaFrame_GetRelPosition)( CHanaFrameT3EC_S354 *pFrame, CHanaFrameRootT3EC_S2D4 *pRefFrame, float *pOutX, float *pOutY, float *pOutZ) = 
  reinterpret_cast<void(__cdecl*)( CHanaFrameT3EC_S354 *pFrame, CHanaFrameRootT3EC_S2D4 *pRefFrame, float *pOutX, float *pOutY, float *pOutZ)>(0x406AB1);

void (__cdecl*SelfBltDC)(LPDIRECTDRAWSURFACE7 pDDSurf) = reinterpret_cast<void (__cdecl*)(LPDIRECTDRAWSURFACE7 pDDSurf)>(0x43F3F7);
void (* HanaFrame_BuildMatrixStack)() = reinterpret_cast<void(*)()>(0x40773F);
void (__cdecl * CHanaObj__LockVtxBufT3EA_418544)(CHanaVertT3EA_S1FC *pVert, DWORD *pNumVertices, LPVOID *ppData) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVert, DWORD *pNumVertices, LPVOID *ppData)>(0x418544);
void (__cdecl * CHanaObj__UnlockVrxBufT3EA_41858E)(CHanaVertT3EA_S1FC *pVert) = reinterpret_cast<void(__cdecl*)(CHanaVertT3EA_S1FC *pVert)>(0x41858E);
unsigned int (* CalculateVertexSize)(DWORD fvf) = reinterpret_cast<unsigned int(*)(DWORD fvf)>(0x417F88); 
HRESULT(__cdecl* HanaFrame_PushNode)(CHanaFrameT3EC_S354* pFrame) =
  reinterpret_cast<HRESULT(__cdecl*)(CHanaFrameT3EC_S354* pFrame)>(0x407800);
void (__cdecl * HanaMorpT457_Animate)(CHanaFrameT3EC_S354 *pFrame, float fValue, float fEase, int nTimeAnim) =
  reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame, float fValue, float fEase, int nTimeAnim)>(0x433569);
int(__cdecl*CHanaObj__GetType_40BBF4)(CHanaBase_S130* pObj) = reinterpret_cast<int(__cdecl*)(CHanaBase_S130*)>(0x40BBF4);
void(__cdecl*SetInversTransf_405BB4)(D3DXMATRIX* pM) = reinterpret_cast<void(__cdecl*)(D3DXMATRIX * pM)>(0x405BB4);
void(__cdecl*ProcessFrame)(CHanaFrameT3EC_S354* pChildFrame, int nAmbient, DWORD* qwBitLightEnables) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 * pChildFrame, int nAmbient, DWORD * qwBitLightEnables)>(0x40600C);
void(__cdecl*HanaFrame_PreDraw)(CHanaFrameT3EC_S354* pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354*)>(0x4339B2);

// === GLOBAL VARIABLES ===

float &TIMER_SCALE_FACTOR = *(float*)0x4C6F74; // 20.0f
DWORD& g_timerStart = *(DWORD*)0x52F1A0;
DWORD& g_timerLast = *(DWORD*)0x52F1A4;

LPD3DXMATRIXSTACK& g_pMatrixStack_4CD77C = *(LPD3DXMATRIXSTACK*)0x4CD77C;
int (&g_texCoordSizeLUT_4A8368)[4] = *(int(*)[4])0x4A8368; // READ-ONLY

uint32_t &g_fCurFrameIsScene = *(uint32_t *)0x4CD770; // WRITABLE
CHanaFrameT3EC_S354 * &g_pCurFrame = *(CHanaFrameT3EC_S354 * *)0x4CD774; // WRITABLE


// === FUNCTIONS ===

// GetTimerScaleFactor - returns the global animation time-scale (double) TIMER_SCALE_FACTOR (0x4C6F74).
// Siblings: SetTimerScaleFactor (writes it), GetFrameTime. Used as the multiplier on elapsed-millisecond deltas
// (HanaMorpT457_Animate time tweens, PlayScreenFade, etc.), i.e. the game/animation speed factor.
double hook_GetTimerScaleFactor()
{
  ONCELOG("[HNH:SUB] GetTimerScaleFactor_47E78F\n");

  return TIMER_SCALE_FACTOR;
}

void __cdecl hook_SetTimerScaleFactor(float fTimeScale)
{
  ONCELOG("[HNH:SUB] SetTimerScaleFactor_47E782\n");
  
  TIMER_SCALE_FACTOR = fTimeScale;
}

double hook_GetFrameTime()
{
  ONCELOG("[HNH:SUB] GetFrameTime_47E79A\n");

  return (double)(timeGetTime() - g_timerStart) * TIMER_SCALE_FACTOR / 1000.0;
}

DWORD __cdecl hook_RGBToSurfaceFormat(LPDIRECTDRAWSURFACE7 pSurface, COLORREF rgbColor)
{
  DDPIXELFORMAT pixelFormat; // [esp+0h] [ebp-48h] BYREF
  signed int gMask; // [esp+20h] [ebp-28h]
  signed int bMask; // [esp+24h] [ebp-24h]
  DWORD combinedMask; // [esp+2Ch] [ebp-1Ch]
  signed int rMask; // [esp+30h] [ebp-18h]
  int rShift; // [esp+34h] [ebp-14h]
  int gShift; // [esp+38h] [ebp-10h]
  int i; // [esp+44h] [ebp-4h]

  ONCELOG("[HNH:SUB] RGBToSurfaceFormat_41973F\n");

  pixelFormat.dwSize = 32;
  pSurface->GetPixelFormat(&pixelFormat);
  rMask = pixelFormat.dwRBitMask;
  gMask = pixelFormat.dwGBitMask;
  bMask = pixelFormat.dwBBitMask;
  combinedMask = g_dwGBitMask | g_dwRBitMask | pixelFormat.dwRBitMask;
  for ( rShift = 0; rShift < 32 && ((1 << rShift) & rMask) == 0; ++rShift )
    ;
  for ( i = 0; i < 32 && ((1 << i) & gMask) == 0; ++i )
    ;
  for ( gShift = 0; gShift < 32 && ((1 << gShift) & bMask) == 0; ++gShift )
    ;
  return ((BYTE2(rgbColor) * (bMask >> gShift) / 255) << gShift)
       | ((BYTE1(rgbColor) * (gMask >> i) / 255) << i)
       | (((unsigned __int8)rgbColor * (rMask >> rShift) / 255) << rShift);
}

void __cdecl hook_CHanaObj__NewSpriteT3F6bySurf(CHanaSpriteT3F6_S1FC **ppOut, LPDIRECTDRAWSURFACE7 pSurfaceSrc)
{
  DWORD flags; // [esp+8h] [ebp-8Ch] BYREF
  CHanaSpriteT3F6_S1FC *pSprite; // [esp+Ch] [ebp-88h]
  HRESULT hr; // [esp+10h] [ebp-84h]
  DDSURFACEDESC2 ddSurfDesc; // [esp+14h] [ebp-80h] BYREF
  _D3DX_SURFACEFORMAT pixelFormat; // [esp+90h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] CHanaObj__NewSpriteT3F6bySurf_4137D1\n");

  pixelFormat = D3DX_SF_R8G8B8;
  flags = 256;
  pSprite = (CHanaSpriteT3F6_S1FC *)MemAlloc_40DEF7(0x1FCu);
  memset(pSprite, 0, sizeof(CHanaSpriteT3F6_S1FC));
  pSprite->base_0.dwType = 1014;
  ddSurfDesc.dwSize = 124;
  ddSurfDesc.dwFlags = 4102;
  pSurfaceSrc->GetSurfaceDesc(&ddSurfDesc);
  hr = D3DXCreateTexture(
         g_pD3DDevice_4D6C78,
         &flags,
         &ddSurfDesc.dwWidth,
         &ddSurfDesc.dwHeight,
         &pixelFormat,
         0,
         &pSprite->pTexture_134,
         0);
  if ( hr < 0 )
    PrintDebugError(hr);
  D3DXLoadTextureFromSurface(g_pD3DDevice_4D6C78, pSprite->pTexture_134, 0xFFFFFFFF, pSurfaceSrc, 0, 0, D3DX_FT_POINT);
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
  CHanaObj__AddRef_40BFC3(&pSprite->base_0);
  *ppOut = pSprite;
}

void __cdecl hook_CHanaObj__ReplaceTexture3F6(CHanaSpriteT3F6_S1FC *pSprite, LPDIRECTDRAWSURFACE7 *pTexture)
{
  if ( !pSprite ) return;
  ONCELOG("[HNH:SUB] CHanaObj__ReplaceTexture3F6_413D78\n");
  
  if ( pSprite->pTexture_134 )
    pSprite->pTexture_134->Release();
  pSprite->pTexture_134 = *pTexture;
  *pTexture = 0;
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
}

void __cdecl hook_DrawSpriteLTRB(CHanaSpriteT3F6_S1FC *pSprite, int left, int top, int right, int bottom)
{
  int quadCount; // [esp+0h] [ebp-54h]
  D3DXVECTOR4 *i; // [esp+8h] [ebp-4Ch]
  DWORD dwSavedFog; // [esp+Ch] [ebp-48h] BYREF
  HRESULT hr; // [esp+10h] [ebp-44h]
  D3DXVECTOR4 quad[4]; // [esp+14h] [ebp-40h] BYREF
  ONCELOG("[HNH:SUB] DrawSpriteLTRB_414589\n");
  quadCount = 4;
  for ( i = quad; --quadCount >= 0; ++i )
    ;
  quad[0].x = (float)left;
  quad[0].y = (float)top;
  quad[0].z = 0.0;
  quad[0].w = 0.0;
  quad[1].x = (float)right;
  quad[1].y = quad[0].y;
  quad[1].z = 0.0;
  quad[1].w = 0.0;
  quad[2].x = quad[1].x;
  quad[2].y = (float)bottom;
  quad[2].z = 0.0;
  quad[2].w = 0.0;
  quad[3].x = quad[0].x;
  quad[3].y = quad[2].y;
  quad[3].z = 0.0;
  quad[3].w = 0.0;
  g_pD3DDevice_4D6C78->SetTexture(0, pSprite->pTexture_134);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAOP, 4);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG1, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG2, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLOROP, 2);
  g_pD3DDevice_4D6C78->SetTexture(1, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(1, D3DTSS_ALPHAOP, 1);
  g_pD3DDevice_4D6C78->SetTextureStageState(1, D3DTSS_COLOROP, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 0);
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwSavedFog);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
  hr = D3DXDrawSprite3D(pSprite->pTexture_134, g_pD3DDevice_4D6C78, quad, pSprite->fAlpha_144, pSprite->pRect_16C);
  g_pCurMatObj = 0;
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwSavedFog);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
}

void __cdecl hook_CHanaObj__SetAlpha3F6(CHanaSpriteT3F6_S1FC *pSprite, float alpha)
{
  ONCELOG("[HNH:SUB] CHanaObj__SetAlpha3F6_414ABA\n");
  if ( alpha <= 1.0 )
  {
    if ( alpha < 0.0 )
      alpha = 0.0;
  }
  else
  {
    alpha = 1.0;
  }
  pSprite->fAlpha_144 = alpha;
}

void __cdecl hook_BltFastWH(
        LPDIRECTDRAWSURFACE7 pSurf,
        int xDest,
        int yDest,
        LPDIRECTDRAWSURFACE7 pSrc,
        int left,
        int top,
        int width,
        int height,
        int ddbltfastFlags)
{
  HRESULT hr; // [esp+0h] [ebp-14h]
  RECT srcRect; // [esp+4h] [ebp-10h] BYREF
  ONCELOG("[HNH:SUB] BltFastWH_419D16\n");
  if ( pSurf && pSrc )
  {
    srcRect.left = left;
    srcRect.right = width + left;
    srcRect.top = top;
    srcRect.bottom = height + top;
    hr = pSurf->BltFast(xDest, yDest, pSrc, &srcRect, ddbltfastFlags | 0x10);
    if ( hr )
      g_szLastDDError_51836C = DDErrToString_41898E(hr);
  }
}

void hook_RenderFlush_4874A5()
{
  LPDIRECTDRAWSURFACE7 pSurf; // [esp+4h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] RenderFlush_4874A5\n");
  if ( !g_pD3DDevice_4D6C78->GetRenderTarget(&pSurf) )
  {
    FlushD3DAndTouchSurface(pSurf);
    SelfBltDC(pSurf);
  }
}

void __cdecl hook_RenderClear_40BF4F(DWORD ClearFlags)
{
  ONCELOG("[HNH:SUB] RenderClear_40BF4F\n");
  g_pRenderCtx->Clear(ClearFlags);
}

void hook_FrameTimer_BeginFrame()
{
  ONCELOG("[HNH:SUB] FrameTimer_BeginFrame_47E768\n");
  g_timerStart = timeGetTime();
  g_timerLast = g_timerStart;
}

void hook_FrameTimer_EndFrame()
{
  ONCELOG("[HNH:SUB] FrameTimer_EndFrame_47E7D5\n");
  g_timerStart = g_timerLast;
  g_timerLast = timeGetTime() - 1;
}

// HanaFrame_GetRelPosition - get the position of pFrame expressed in pRefFrame's coordinate system.
// If pRefFrame is the scene root (g_pSceneFrame) it returns pFrame's own world-space translation (row-4 of mat4x4_after);
// otherwise it returns the translation row of (pFrame->mat4x4_after * inverse(pRefFrame->mat4x4_after)).
// Writes the x/y/z result into *pOutX, *pOutY, *pOutZ. Heavily used across scene callbacks and
// HanaFrame_Draw to query relative positions.
void __cdecl hook_HanaFrame_GetRelPosition(
        CHanaFrameT3EC_S354 *pFrame,
        CHanaFrameRootT3EC_S2D4 *pRefFrame,
        float *pOutX,
        float *pOutY,
        float *pOutZ)
{
  D3DXMATRIX pRelMat; // [esp+0h] [ebp-40h] BYREF
  ONCELOG("[HNH:SUB] HanaFrame_GetRelPosition_406AB1\n");

  if ( pRefFrame == g_pSceneFrame )
  {
    *pOutX = pFrame->mat4x4_after._41;
    *pOutY = pFrame->mat4x4_after._42;
    *pOutZ = pFrame->mat4x4_after._43;
  }
  else
  {
    D3DXMatrixInverse(&pRelMat, 0, &pRefFrame->mat4x4_after);
    D3DXMatrixMultiply(&pRelMat, &pFrame->mat4x4_after, &pRelMat);
    *pOutX = pRelMat._41;
    *pOutY = pRelMat._42;
    *pOutZ = pRelMat._43;
  }
}

// HanaFrame_BuildMatrixStack - build and traverse the global D3D matrix stack for the frame tree.
// Creates pMatrixStack_4CD77C, pushes it, loads the scene frame's base matrix (g_pSceneFrame->mat4x4_fmrx),
// recursively pushes every descendant node via HanaFrame_PushNode (which also updates each node's light from
// the stack top and special-cases the camera frame), then pops and releases the stack. Calledfrom
//  HanaFrame_Draw before per-object rendering.
void hook_HanaFrame_BuildMatrixStack()
{
  unsigned int i; // [esp+0h] [ebp-8h]
  CHanaNode *pFirstChildNode; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaFrame_BuildMatrixStack_40773F\n");

  D3DXCreateMatrixStack(0, &g_pMatrixStack_4CD77C);
  g_pMatrixStack_4CD77C->Push();
  g_pMatrixStack_4CD77C->LoadMatrix(&g_pSceneFrame->mat4x4_fmrx);
  if ( g_pSceneFrame->nCountChild )
  {
    pFirstChildNode = g_pSceneFrame->pFirstChildNode;
    for ( i = 0; i < g_pSceneFrame->nCountChild; ++i )
    {
      HanaFrame_PushNode(pFirstChildNode->pData);
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
  g_pMatrixStack_4CD77C->Pop();
  g_pMatrixStack_4CD77C->Release();
}

int __cdecl hook_CHanaObj__GetType_40BBF4(CHanaBase_S130 *pObj)
{
  ONCELOG("[HNH:SUB] CHanaObj__GetType_40BBF4\n");
  if ( pObj )
    return pObj->dwType;
  else
    return -1;
}

// HanaMorpT457_Animate - computes the current morph parameter (fMorpParam_130) of the T457 object on a frame from up to three sources. Only acts if pFrame->pMorpT457_20C is a T457 (GetType==0x457==1111). fTimeScale=sub_47E78F() (global animation time scale).
//  (a) IMMEDIATE: if no eased (dwEaseEnabled_358) and no time (nTimeState_36C) anim and fValue>=0 -> fMorpParam_130 = fValue.
//  (b) EASED: if fEase>=0 and dwEaseEnabled_358: fEase==0 -> snap to fEaseProgress_384 and decay it (/2); else fProgress=clamp(fEase*fEaseRate_388,<=1) and lerp fMorpParam_130 between fEaseFrom_35C..fEaseTo_360 by (fProgress+fEaseProgress_384)/2, storing fProgress back.
//  (c) TIME: if nTimeAnim>=0: a wall-clock tween from fTimeFrom_370 to fTimeTo_374 driven by timeGetTime()-dwClockStart_380 (times fTimeScale/1000). nTimeState_36C is the lifecycle: 1&nTimeAnim==1 -> arm (dwClockStart=timeGetTime); running (==2 or arm) -> advance and set state 3 (done, param=fTimeTo) or 2 (running); otherwise (queued) -> prime state 1, set param=fTimeTo, and load next keyframe from fNextFrom_378/fNextTo_37C.
// Called from HanaMorp_ApplyTree (per-frame) and via the tree-propagator sub_481BE0.
void __cdecl hook_HanaMorpT457_Animate(CHanaFrameT3EC_S354 *pFrame, float fValue, float fEase, int nTimeAnim)
{
  double dElapsed; // st7
  CHanaMorpT457_S40C *pMorp; // [esp+1Ch] [ebp-14h]
  float fElapsed; // [esp+20h] [ebp-10h]
  unsigned int fParamDelta; // [esp+24h] [ebp-Ch]
  float fTimeScale; // [esp+2Ch] [ebp-4h]
  float fProgress; // [esp+40h] [ebp+10h]
  
  ONCELOG("[HNH:SUB] HanaMorpT457_Animate_433569");

  if ( pFrame && CHanaObj__GetType_40BBF4(pFrame->pAttachedObj_20C) == 1111 )
  {
    pMorp = pFrame->pAttachedObj_20C;
    fTimeScale = GetTimerScaleFactor();
    if ( !pMorp->dwEaseEnabled_358 && !pMorp->nTimeState_36C && fValue >= 0.0 )
      pMorp->fMorpParam_130 = fValue;
    if ( fEase >= 0.0 && pMorp->dwEaseEnabled_358 )
    {
      if ( fEase == 0.0 )
      {
        pMorp->fMorpParam_130 = pMorp->fEaseProgress_384;
        pMorp->fEaseProgress_384 = pMorp->fEaseProgress_384 / 2.0;
      }
      else
      {
        fProgress = fEase * pMorp->fEaseRate_388;
        if ( fProgress > 1.0 )
          fProgress = 1.0;
        pMorp->fMorpParam_130 = (double)(unsigned int)(pMorp->nEaseTo_360 - pMorp->nEaseFrom_35C)
                              * (fProgress + pMorp->fEaseProgress_384)
                              / 2.0
                              + (double)pMorp->nEaseFrom_35C;
        pMorp->fEaseProgress_384 = fProgress;
      }
    }
    if ( nTimeAnim >= 0 )
    {
      if ( pMorp->nTimeState_36C == 2 || pMorp->nTimeState_36C == 1 && nTimeAnim == 1 )
      {
        if ( pMorp->nTimeState_36C == 1 && nTimeAnim == 1 )
          pMorp->dwClockStart_380 = timeGetTime();
        fParamDelta = pMorp->nTimeTo_374 - pMorp->nTimeFrom_370;
        if ( fParamDelta )
        {
          dElapsed = (double)(timeGetTime() - pMorp->dwClockStart_380) * fTimeScale / 1000.0;
          fElapsed = dElapsed;
          pMorp->fMorpParam_130 = fElapsed
                                - (double)(pMorp->nTimeTo_374 - pMorp->nTimeFrom_370)
                                * (double)(int)((unsigned int)(__int64)dElapsed / fParamDelta)
                                + (double)pMorp->nTimeFrom_370;
          if ( (unsigned int)(__int64)dElapsed / fParamDelta )
          {
            pMorp->nTimeState_36C = 3;
            pMorp->fMorpParam_130 = (float)pMorp->nTimeTo_374;
          }
          else
          {
            pMorp->nTimeState_36C = 2;
          }
        }
      }
      else if ( pMorp->nTimeState_36C )
      {
        pMorp->nTimeState_36C = 1;
        pMorp->fMorpParam_130 = (float)pMorp->nTimeTo_374;
        pMorp->nTimeTo_374 = pMorp->nNextTo_37C;
        pMorp->nTimeFrom_370 = pMorp->nNextFrom_378;
      }
    }
  }
}

void __cdecl hook_CHanaObj__LockVtxBufT3EA_418544(CHanaVertT3EA_S1FC *pVert, DWORD *pNumVertices, LPVOID *ppData)
{
  ONCELOG("[HNH:SUB] CHanaObj__LockVtxBufT3EA_418544");

  if ( pVert->pD3DVtxBuf_130->Lock(DDLOCK_NOSYSLOCK | DDLOCK_WAIT, ppData, 0) >= 0 )
    *pNumVertices = pVert->dwNumVertices_134;
}

void __cdecl hook_CHanaObj__UnlockVrxBufT3EA_41858E(CHanaVertT3EA_S1FC *pVert)
{
  ONCELOG("[HNH:SUB] CHanaObj__UnlockVrxBufT3EA_41858E");

  pVert->pD3DVtxBuf_130->Unlock();
}

unsigned int hook_CalculateVertexSize(DWORD fvf)
{
  unsigned int i; // [esp+4h] [ebp-Ch]
  unsigned int vertexSize; // [esp+8h] [ebp-8h]
  unsigned int texCount; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] CalculateVertexSize_417F88");

  vertexSize = 0;
  if ( (fvf & D3DFVF_RESERVED0) != 0 )
    vertexSize = 4;
  if ( (fvf & D3DFVF_RESERVED1) != 0)
    vertexSize += 4;
  if ( (fvf & D3DFVF_DIFFUSE) != 0 )
    vertexSize += 4;
  if ( (fvf & D3DFVF_NORMAL) != 0 )
    vertexSize += 12;
  if ( ((unsigned __int8)fvf & (unsigned __int8)D3DFVF_SPECULAR) != 0 )
    vertexSize += 4;
  switch ( fvf & 0xE )
  {
    case D3DFVF_XYZ:
      vertexSize += 12;
      break;
    case D3DFVF_XYZRHW:
      vertexSize += 16;
      break;
    case D3DFVF_XYZB1:
      vertexSize += 16;
      break;
    case D3DFVF_XYZB2:
      vertexSize += 20;
      break;
    case D3DFVF_XYZB3:
      vertexSize += 24;
      break;
    case D3DFVF_XYZB4:
      vertexSize += 28;
      break;
    case D3DFVF_XYZB5:
      vertexSize += 32;
      break;
    default:
      break;
  }
  texCount = (unsigned __int16)(fvf & 0xF00) >> 8;
  if ( texCount )
  {
    for ( i = 0; i < texCount; ++i )
      vertexSize += 4 * g_texCoordSizeLUT_4A8368[fvf >> (2 * i + 16)];
  }
  return vertexSize;
}

void __cdecl hook_SelfBltDC(LPDIRECTDRAWSURFACE7 pDDSurf)
{
  HDC hdc; // [esp+0h] [ebp-Ch] BYREF
  DWORD w; // [esp+4h] [ebp-8h] BYREF
  DWORD h; // [esp+8h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] SelfBltDC_43F3F7");
  
  if ( pDDSurf )
  {
    GetSurfaceWH(pDDSurf, &w, &h);
    pDDSurf->GetDC(&hdc);
    BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, 0x330008u);
    pDDSurf->ReleaseDC(hdc);
  }
}

void __cdecl hook_SetInversTransf_405BB4(D3DXMATRIX *pM)
{
  ONCELOG("[HNH:SUB] SetInversTransf_405BB4");

  D3DXMatrixInverse(&g_mat4x4View_4D4780, 0, pM);
  g_pD3DDevice_4D6C78->SetTransform(D3DTRANSFORMSTATE_VIEW, (LPD3DMATRIX) & g_mat4x4View_4D4780);
}

void __cdecl hook_HanaFrame_PushNode(CHanaFrameT3EC_S354 *pFrame)
{
  D3DXMATRIX *pTopMat; // eax
  D3DXMATRIX fTopMat; // [esp+8h] [ebp-48h] BYREF
  unsigned int i; // [esp+48h] [ebp-8h]
  CHanaNode *pFirstChildNode; // [esp+4Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaFrame_PushNode_407800");

  g_pMatrixStack_4CD77C->Push();
  g_pMatrixStack_4CD77C->MultMatrixLocal(& pFrame->mat4x4_fmrx);
  if ( pFrame->pHanaLight_210 )
  {
    qmemcpy(&fTopMat, g_pMatrixStack_4CD77C->GetTop(), sizeof(fTopMat));
    ConfigureLight(pFrame->pHanaLight_210, fTopMat._41, fTopMat._42, fTopMat._43, fTopMat._13, fTopMat._23, fTopMat._33);
  }
  if ( pFrame == g_pCameraFrame )
  {
    pTopMat = g_pMatrixStack_4CD77C->GetTop();
    SetInversTransf_405BB4(pTopMat);
    g_pMatrixStack_4CD77C->Pop();
  }
  else
  {
    if ( pFrame->nCountChild )
    {
      pFirstChildNode = pFrame->pFirstChildNode;
      for ( i = 0; i < pFrame->nCountChild; ++i )
      {
        HanaFrame_PushNode(pFirstChildNode->pData);
        pFirstChildNode = pFirstChildNode->pNext;
      }
    }
    g_pMatrixStack_4CD77C->Pop();
  }
}

void __cdecl hook_PlayScreenFade(int bFadeOut, COLORREF colorref,
                                 LPDIRECTDRAWSURFACE7 pDdSurface, float fTimeScale, char bFlush)
{
    CHanaSpriteT3F6_S1FC *pDarkenSprite;    // [esp+D4h] [ebp-4h]
    CHanaSpriteT3F6_S1FC *pColorSprite;     // [esp+94h] [ebp-44h]
    LPDIRECTDRAWSURFACE7 pCapturedBg;       // [esp+9Ch] [ebp-3Ch]
    LPDIRECTDRAWSURFACE7 pColorSurface;     // [esp+A8h] [ebp-30h]
    LPDIRECTDRAWSURFACE7 pTexture;          // [esp+B8h] [ebp-20h]
    DWORD dwColor, w, h, dwAntiAliasSaved;  // color / [ebp-24h] / [ebp-18h] / [ebp-34h]
    float fAlpha, fAlphaDarken, fTimeAlpha, fTimerScaleSaved;
    int left, top, right, bottom, right2, bottom2;
    BYTE bDone;
    HRESULT hr;

    DebugLog("[HNH:SUB] PlayScreenFade_4874DE dir=%d scale=%f\n", bFadeOut, fTimeScale);

    pDarkenSprite = NULL;
    pColorSprite = NULL;
    pTexture = NULL;
    pCapturedBg = NULL;
    bDone = 0;
    left = -1;
    top = -1;
    right  = ConvertFrom640AsInt(641);            // one past the screen edges
    bottom = ConvertFrom640AsInt(481);
    right2  = ConvertFrom640AsInt(640) + 100;     // darken overlay overscan
    bottom2 = ConvertFrom640AsInt(480) + 100;

    // 128x128 solid-color tile -> color sprite
    pColorSurface = CreateSurfaceWH(0x80, 0x80);
    if (!pColorSurface)
        return;
    dwColor = RGBToSurfaceFormat(pColorSurface, colorref);
    BltSurface(pColorSurface, 0, 0, 128, 128, (int)dwColor);
    CHanaObj__NewSpriteT3F6bySurf(&pColorSprite, pColorSurface);
    if (!pColorSprite)
    {
        pColorSurface->Release();
        return;
    }
    // second sprite: the tile re-textured with darken.bmp as alpha mask
    CHanaObj__NewSprite3F6byWH(&pDarkenSprite, 128, 128);
    if (!pDarkenSprite)
    {
        CHanaObj__Free_40C1E4(&pColorSprite->base_0);
        pColorSprite = NULL;
        pColorSurface->Release();
        return;
    }
    pTexture = CreateTextureFromSurfaceWithAlphaMask(pColorSurface, g_pSurfaceDarkenBmp_52F1A8);
    CHanaObj__ReplaceTexture3F6(pDarkenSprite, &pTexture);
    pTexture = NULL;
    pColorSurface->Release();
    pColorSurface = NULL;

    // capture the pre-fade frame: supplied surface, or a fresh 3D render
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
    if (pDdSurface)
    {
        FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
        GetSurfaceWH(pDdSurface, &w, &h);
        BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pDdSurface, 0, 0, (int)w, (int)h, 0);
        GetSurfaceWH(g_pBackBuffer_4D6C90, &w, &h);
        pCapturedBg = CreateSurfaceWH(w, h);
        if (bFlush)
            RenderFlush_4874A5();
        BltFastWH(pCapturedBg, 0, 0, g_pBackBuffer_4D6C90, 0, 0, (int)w, (int)h, 0);
    }
    else
    {
        RenderClear_40BF4F(3);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
        HanaVert_DrawBatch(0);
        MtxStackStartMultLocal_4074EF();
        hr = g_pD3DDevice_4D6C78->BeginScene();
        if (hr >= 0)
        {
            HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
            HanaVert_DrawBatch(1);
            g_pD3DDevice_4D6C78->EndScene();
        }
        if (bFlush)
            RenderFlush_4874A5();
        GetSurfaceWH(g_pBackBuffer_4D6C90, &w, &h);
        pCapturedBg = CreateSurfaceWH(w, h);
        BltFastWH(pCapturedBg, 0, 0, g_pBackBuffer_4D6C90, 0, 0, (int)w, (int)h, 0);
        BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pCapturedBg, 0, 0, (int)w, (int)h, 0);
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasSaved);
    FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);

    // (the original re-tests pDarkenSprite here; it cannot be NULL anymore)

    // bFadeOut > 1: setup only -- returns WITHOUT freeing the sprites or the
    // captured frame (original leaks them; no caller passes > 1)
    if (bFadeOut != 0 && bFadeOut != 1)
        return;
    fAlpha = (bFadeOut == 1) ? 1.0f : 0.0f;

    fTimerScaleSaved = (float)GetTimerScaleFactor();
    SetTimerScaleFactor(fTimeScale);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();

    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    while (!bDone)
    {
        fTimeAlpha = (float)(GetFrameTime() * 2.1 * 5); // default 2.1
        if (fTimeAlpha < 0.01f)
            fTimeAlpha = 0.01f;
        if (bFadeOut == 0)
            fAlpha += 0.025f * fTimeAlpha;
        else
            fAlpha -= 0.025f * fTimeAlpha;
        FrameTimer_EndFrame();

        // restore the captured pre-fade frame
        if (pCapturedBg)
        {
            FlushD3DAndTouchSurface(g_pPrimaryBuffer_4D6C88);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            GetSurfaceWH(pCapturedBg, &w, &h);
            BltFastWH(g_pBackBuffer_4D6C90, 0, 0, pCapturedBg, 0, 0, (int)w, (int)h, 0);
        }

        // clamp for drawing / detect the end
        float alpha = fAlpha;
        if (fAlpha >= 1.0f)
        {
            alpha = 1.0f;
            bDone = 1;
        }
        if (alpha <= 0.0f)
        {
            alpha = 0.0f;
            bDone = 1;
        }

        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
        fAlphaDarken = alpha * 1.25f;
        CHanaObj__SetAlpha3F6(pDarkenSprite, fAlphaDarken);
        CHanaObj__SetAlpha3F6(pColorSprite, alpha);

        hr = g_pD3DDevice_4D6C78->BeginScene();
        if (hr >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
            DrawSpriteLTRB(pColorSprite, left, top, right, bottom);
            DrawSpriteLTRB(pDarkenSprite, -100, -100, right2, bottom2);

            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 0);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
            DrawSpriteLTRB(pColorSprite, left, top, right, bottom);
            DrawSpriteLTRB(pDarkenSprite, -100, -100, right2, bottom2);
            g_pD3DDevice_4D6C78->EndScene();
        }
        PresentFrame();
        HanaVert_DrawBatch(0);
    }
    Sleep(10);

    // teardown / restore
    if (pDarkenSprite)
    {
        CHanaObj__Free_40C1E4(&pDarkenSprite->base_0);
        pDarkenSprite = NULL;
    }
    if (pColorSprite)
    {
        CHanaObj__Free_40C1E4(&pColorSprite->base_0);
        pColorSprite = NULL;
    }
    if (pCapturedBg)
        pCapturedBg->Release();
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZVISIBLE, 1);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();
    SetTimerScaleFactor(fTimerScaleSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasSaved);
}

// HanaFrame_Draw - per-frame render/dispatch entry (thin wrapper sub_405BE1 sets g_fRendering=1 around it).
// 1) DS3D listener: CommitDeferredSettings; orient/position it to the camera relative to the scene (CalculateLocalRotationMatrix->SetOrientation, sub_406AB1->SetPosition).
// 2) g_pCurFrame = pFrame; g_fCurFrameIsScene = (pFrame==g_pSceneFrame). sub_40773F() builds/pushes the world-matrix stack for all scene children.
// 3) If the frame's skip-draw flag (*(pFrame+304)) is 0: prep the frame (sub_4339B2), save D3DRS_AMBIENT, then for each scene child frame:
//    - reset the light mask; when g_dwLightningState, disable all 0x40 lights;
//    - set per-frame fog (FOGENABLE/COLOR/START/END from bHasFog/dwFogColor/fFogStart/fFogEnd) or disable fog;
//    - position & enable the frame's CHanaLight (sub_409510 positions by dltType, sub_40929C does SetLight+LightEnable / ambient), set its bit in bitLightEnables;
//    - run the object updater sub_40600C(child, {bitLightEnables, ambient}) and restore ambient.
// Returns ambient-restore HRESULT. g_pCurFrame/g_fCurFrameIsScene/g_fRendering are read by sub_40600C.
void __cdecl hook_HanaFrame_Draw(CHanaFrameT3EC_S354 *pFrame)
{
  DWORD nAmbient; // [esp+8h] [ebp-7Ch] BYREF
  D3DXMATRIX posX; // [esp+Ch] [ebp-78h] BYREF
  D3DVALUE pZXrot; // [esp+4Ch] [ebp-38h] BYREF
  D3DVALUE pZpos; // [esp+50h] [ebp-34h] BYREF
  D3DVALUE pYXrot; // [esp+54h] [ebp-30h] BYREF
  D3DVALUE pYpos; // [esp+58h] [ebp-2Ch] BYREF
  D3DVALUE pXYrot; // [esp+5Ch] [ebp-28h] BYREF
  D3DVALUE pXpos; // [esp+60h] [ebp-24h] BYREF
  DWORD qwBitLightEnables[2]; // [esp+64h] [ebp-20h] BYREF
  DWORD dwAmbientSaved; // [esp+6Ch] [ebp-18h] BYREF
  DWORD j; // [esp+70h] [ebp-14h]
  unsigned int i; // [esp+74h] [ebp-10h]
  CHanaFrameT3EC_S354 *pChildFrame; // [esp+78h] [ebp-Ch]
  int v14; // [esp+7Ch] [ebp-8h]
  CHanaNode *pFirstChildNode; // [esp+80h] [ebp-4h]

  qwBitLightEnables[0] = 0;
  qwBitLightEnables[1] = 0;
  pChildFrame = 0;
  if ( g_pds3DListener )
  {
    g_pds3DListener->CommitDeferredSettings();
    CalculateLocalRotationMatrix(
      g_pCameraFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      &pXpos,
      &pYpos,
      &pZpos,
      &pXYrot,
      &pYXrot,
      &pZXrot);
    g_pds3DListener->SetOrientation(pXpos, pYpos, pZpos, pXYrot, pYXrot, pZXrot, 1u);
    HanaFrame_GetRelPosition(g_pCameraFrame, g_pSceneFrame, &pXpos, &pYpos, &pZpos);
    g_pds3DListener->SetPosition(pXpos, pYpos, pZpos, 1u);
  }
  g_fCurFrameIsScene = 0;
  g_pCurFrame = pFrame;
  HanaFrame_BuildMatrixStack();
  if ( g_pSceneFrame == g_pCurFrame )
    g_fCurFrameIsScene = 1;
  if ( !g_pCurFrame->bFlagAlpha_130 )
  {
    HanaFrame_PreDraw(pFrame);
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbientSaved);
    if ( g_pSceneFrame->nCountChild )
    {
      pFirstChildNode = g_pSceneFrame->pFirstChildNode;
      for ( i = 0; i < g_pSceneFrame->nCountChild; ++i )
      {
        v14 = 0;
        qwBitLightEnables[0] = 0;
        qwBitLightEnables[1] = 0;
        pChildFrame = pFirstChildNode->pData;
        if ( pChildFrame )
        {
          if ( g_nLightningEnabled )
          {
            for ( j = 0; j < 64; ++j )
              g_pD3DDevice_4D6C78->LightEnable(j, 0);
          }
          if ( pChildFrame->bHasFog == 1 )
          {
            g_pD3DDevice_4D6C78->SetRenderState(
              D3DRENDERSTATE_FOGENABLE,
              g_dwFogEnable_4D6CF4);
            g_pD3DDevice_4D6C78->SetRenderState(
              D3DRENDERSTATE_FOGCOLOR,
              pChildFrame->dwFogColor);
            g_pD3DDevice_4D6C78->SetRenderState(
              D3DRENDERSTATE_FOGTABLESTART,
              LODWORD(pChildFrame->fFogStart));
            g_pD3DDevice_4D6C78->SetRenderState(
              D3DRENDERSTATE_FOGTABLEEND,
              LODWORD(pChildFrame->fFogEnd));
          }
          else
          {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
          }
          if ( pChildFrame->pHanaLight_210
            && g_nLightningEnabled
            && pChildFrame->pHanaLight_210->d3dLight_134.dltType != (D3DLIGHT_FORCE_DWORD|0x80000000) )
          {
            qmemcpy(&posX, &pChildFrame->mat4x4_after, sizeof(posX));
            ConfigureLight(pChildFrame->pHanaLight_210, posX._41, posX._42, posX._43, posX._13, posX._23, posX._33);
            *(_QWORD *)qwBitLightEnables |= 1 << pChildFrame->pHanaLight_210->dwLightIndex_130;
          }
          if ( pChildFrame->pHanaLight_210 && g_nLightningEnabled )
          {
            ApplyLightSettings(pChildFrame->pHanaLight_210);
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_AMBIENT, &nAmbient);
            ProcessFrame(pChildFrame, nAmbient, qwBitLightEnables);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbientSaved);
            if ( pChildFrame->pHanaLight_210->d3dLight_134.dltType != (D3DLIGHT_FORCE_DWORD|0x80000000) )// != -1
              g_pD3DDevice_4D6C78->LightEnable(
                pChildFrame->pHanaLight_210->dwLightIndex_130,
                0);
          }
          else
          {
            ProcessFrame(pChildFrame, dwAmbientSaved, qwBitLightEnables);
          }
        }
        pFirstChildNode = pFirstChildNode->pNext;
      }
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbientSaved);
  }
}

void register_PlayScreenFade_4874DE() {
  hnh_Hook(0x4874DE, hook_PlayScreenFade, "PlayScreenFade_4874DE"); // [OK]

  hnh_Hook(0x47E78F, hook_GetTimerScaleFactor,           "GetTimerScaleFactor_47E78F");
  hnh_Hook(0x47E782, hook_SetTimerScaleFactor,           "SetTimerScaleFactor_47E782");
  hnh_Hook(0x41973F, hook_RGBToSurfaceFormat,            "RGBToSurfaceFormat_41973F");
  hnh_Hook(0x4137D1, hook_CHanaObj__NewSpriteT3F6bySurf, "CHanaObj__NewSpriteT3F6bySurf_4137D1");
  hnh_Hook(0x413D78, hook_CHanaObj__ReplaceTexture3F6,   "CHanaObj__ReplaceTexture3F6_413D78");
  hnh_Hook(0x414589, hook_DrawSpriteLTRB,                "DrawSpriteLTRB_414589");
  hnh_Hook(0x414ABA, hook_CHanaObj__SetAlpha3F6,         "CHanaObj__SetAlpha3F6_414ABA");
  hnh_Hook(0x419D16, hook_BltFastWH,                     "BltFastWH_419D16");
  hnh_Hook(0x4874A5, hook_RenderFlush_4874A5,            "RenderFlush_4874A5");
  hnh_Hook(0x40BF4F, hook_RenderClear_40BF4F,            "RenderClear_40BF4F");
  hnh_Hook(0x47E768, hook_FrameTimer_BeginFrame,         "FrameTimer_BeginFrame_47E768");
  hnh_Hook(0x47E7D5, hook_FrameTimer_EndFrame,           "FrameTimer_EndFrame_47E7D5");
  hnh_Hook(0x406AB1, hook_HanaFrame_GetRelPosition,      "HanaFrame_GetRelPosition_406AB1");
  hnh_Hook(0x40773F, hook_HanaFrame_BuildMatrixStack,    "HanaFrame_BuildMatrixStack_40773F");
  hnh_Hook(0x40BBF4, hook_CHanaObj__GetType_40BBF4,      "CHanaObj__GetType_40BBF4");
  hnh_Hook(0x433569, hook_HanaMorpT457_Animate,          "HanaMorpT457_Animate_433569");
  hnh_Hook(0x418544, hook_CHanaObj__LockVtxBufT3EA_418544,   "CHanaObj__LockVtxBufT3EA_418544");
  hnh_Hook(0x41858E, hook_CHanaObj__UnlockVrxBufT3EA_41858E, "CHanaObj__UnlockVrxBufT3EA_41858E");
  hnh_Hook(0x417F88, hook_CalculateVertexSize, "CalculateVertexSize_417F88");
  hnh_Hook(0x47E79A, hook_GetFrameTime, "GetFrameTime_47E79A");
  hnh_Hook(0x43F3F7, hook_SelfBltDC,    "SelfBltDC_43F3F7");
  hnh_Hook(0x407800, hook_HanaFrame_PushNode,     "HanaFrame_PushNode_407800");
  hnh_Hook(0x405BB4, hook_SetInversTransf_405BB4, "SetInversTransf_405BB4");
}
