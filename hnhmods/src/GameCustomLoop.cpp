#include "dx7.hpp"

#include <math.h>

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === EXPERIMENT 1: render the otoko hero body ===
//
// Minimal custom scene loop, dispatched from WinMain_485CE0 like GameMainLoop.
// Loads the hero pair (otoko\otoko.sek + .odf, same as PlayExploring/PlayHunting),
// parks it in front of an aimed camera, and plays the idle timeline (SEK item 1)
// until ESC. Render skeleton copied from the scene players:
// UpdateAnim -> MtxStackStartMultLocal -> Clear(3) -> BeginScene ->
// HanaFrame_Draw + HanaVert_DrawBatch -> PresentFrame.
//
// === EXPERIMENT 2: orbit camera ===
//
// Hold LMB and drag to orbit around the hero's mid-body; arrow Up/Down zooms.
// NOTE: the otoko rig hangs DOWNWARD from its frame origin (the origin sits at
// the top of the ~16-unit body), so the orbit target is BELOW y=0 - aiming at
// y=+8 renders empty sky. Initial state: dist 25, pitch +0.37 -> eye near
// (0, 1, 1.8) aiming down-forward, i.e. the proven visible framing.
//
// === EXPERIMENT 3: live timeline switching ===
//
// Number keys 0..9 hard-cut the hero SEK timeline item (SekBody__UpdateTo).
// Caution: item slots that are empty in otoko.sek zero the step count, so the
// play-rate recompute divides by zero - the clock NaNs and the pose freezes.

/**
 THE ISSUE ON RELATED VERTEX BUFFER ERROR AND DXWRAPPER

 In a DX7-to-DX9 wrapper using D3DXContext (from the legacy DirectX 7 utility library d3dx.h), a vertex buffer error occurs if the primary buffer is never blitted before UpdateFrame
 because the wrapper relies on the primary surface blit or flip call to lazily initialize, bind, or synchronize the underlying Direct3D 9 device state and swap chain.
 Without that initial blit, UpdateFrame attempts to lock, draw, or flush vertex buffers against an uninitialized or out-of-sync rendering pipeline.

 Root Causes in DX7-to-DX9 Wrappers

 * Lazy Device and Swap Chain Initialization. Many DX7 wrappers (such as dgVoodoo2 or dxwrapper) defer creating or resetting the underlying IDirect3DDevice9 until the application attempts to present a frame. 
   When IDirectDrawSurface7::Blt or Flip is skipped on the primary surface, the D3D9 device remains unready when ID3DXContext::UpdateFrame executes.
 * Implicit BeginScene and EndScene Tracking. In DirectX 7, D3DXContext manages the frame lifecycle internally. Wrappers often hook primary surface blits to trigger IDirect3DDevice9::EndScene() and Present(),
   then reset state for the next BeginScene(). Skipping the blit breaks this state machine, causing subsequent vertex buffer operations (Lock or DrawPrimitive) to fail with invalid call errors (D3DERR_INVALIDCALL).
 * Surface-to-Texture Synchronization. DX7 games frequently render 2D elements via DirectDraw blits to the primary or back buffer before 3D geometry is drawn.
   Wrappers intercept the primary blit to upload system-memory surface changes into D3D9 textures and set up stream sources. If the blit never happens, the wrapper's internal vertex buffer used for screen-space quads
   or transformed vertices may remain unallocated or locked.
 * Viewport and Render Target Binding. ID3DXContext::UpdateFrame expects a valid render target matching the primary surface dimensions.
   Wrappers typically bind the D3D9 back buffer as the active render target during the first primary surface operation.

 How to Fix or Work Around It

 * Trigger a Dummy Primary Blit. Perform a 1x1 transparent or color-keyed Blt (or BltFast) to the primary surface during initialization before entering your UpdateFrame loop.
   This forces the wrapper to complete its D3D9 device setup.
 * Call Clear Before UpdateFrame. Explicitly call ID3DXContext::Clear before your first UpdateFrame. This often forces the wrapper to validate the render target and initialize internal state buffers.
 * Verify Wrapper Configuration. If you are using an existing wrapper rather than writing your own, check its configuration file (such as dxwrapper.ini or dgVoodoo.conf) for options like ForceDirect3D9On12, 
   DdrawUseNativeResolution, or forced flip/blit emulation modes that bypass lazy initialization.

*/
static CSekBody *s_pHeroBody = NULL;

// placement constants (world units; the ~16-unit body hangs BELOW the frame
// origin, cf. g_fHitY_4C6F48, so the orbit target is at negative Y)
#define CUSTOM_HERO_Z   25.0f
#define CUSTOM_TARGET_Y 0.0f           // hero mid-body (origin = top of rig)

// orbit state (spherical around the target at (0, CUSTOM_TARGET_Y, CUSTOM_HERO_Z))
static float s_fCamYaw = 0.0f;          // radians, 0 = eye on the -Z side
static float s_fCamPitch = 0.37f;       // eye above the target, aiming down
static float s_fCamDist = 25.0f;        // eye ~ (0, 1.1, 1.8)

// the device our actor assets were created on; D3DInitBuffers re-assigns
// g_pD3DDevice_4D6C78 on every D3DInit (RecoverLostDevice's WRONGMODE path),
// which invalidates ALL device objects (hero vertex buffers, sprite textures)
// and builds a brand-new g_pSceneFrame - orphaning anything attached to the
// old tree. The stock game survives this by re-entering scenes (full asset
// reload); a persistent custom scene must watch and reload by itself.
static LPDIRECT3DDEVICE7 s_pDevAtLoad = NULL;

static CHanaFrameT3EC_S354 *HeroFrame(void)
{
  return s_pHeroBody ? s_pHeroBody->pSekHeader_220->pFrameT3EC : NULL;
}

// re-place the camera from the current spherical orbit state
static void UpdateOrbitCamera(void)
{
  const float fTx = 0.0f;
  const float fTy = CUSTOM_TARGET_Y;
  const float fTz = CUSTOM_HERO_Z;

  float fCosPitch = cosf(s_fCamPitch);
  float fX = fTx + s_fCamDist * sinf(s_fCamYaw) * fCosPitch;
  float fY = fTy + s_fCamDist * sinf(s_fCamPitch);
  float fZ = fTz - s_fCamDist * cosf(s_fCamYaw) * fCosPitch;

  // look direction = normalized (target - eye)
  float fDx = fTx - fX, fDy = fTy - fY, fDz = fTz - fZ;
  float fLen = sqrtf(fDx * fDx + fDy * fDy + fDz * fDz);
  if ( fLen < 0.0001f )
    return;

  SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                        fX, fY, fZ);
  UpdateBoneLookAtTransform(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            fDx / fLen, fDy / fLen, fDz / fLen,
                            0.0f, 1.0f, 0.0f);
}

static BOOL GameCustomSetup(void)
{
  char szSekPath[256];
  char szOdfPath[256];

  // hero body pair, resolved through the ODF dir (path code 1005)
  wsprintfA(szSekPath, "otoko\\otoko.sek");
  CHanaObj__GetRealPath_486D9A(szSekPath, 1005);
  wsprintfA(szOdfPath, "otoko\\otoko.odf");
  CHanaObj__GetRealPath_486D9A(szOdfPath, 1005);
  if ( !LoadSekAndOdf_4445DD(szSekPath, szOdfPath, &s_pHeroBody) )
  {
    DebugLog("[HNH][Custom] error LoadSekAndOdf otoko\n");
    return FALSE;
  }

  // park the hero in front of the origin, facing back towards the camera
  // (corrected: the rig's face-the-camera direction is +Z here)
  SetFramePosRel_4069DD(HeroFrame(), (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                        0.0f, 0.0f, CUSTOM_HERO_Z);
  UpdateBoneLookAtTransform(HeroFrame(), (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f);

  // the orbit camera owns placement from here on (first call below frames the
  // corrected static view); the view matrix is rebuilt from this frame's world
  // pose on every HanaFrame_Draw
  UpdateOrbitCamera();

  // widen the boot FOV (0.5 rad crops a 16-unit body at this distance);
  // 0.9 rad fits it fully at dist 25 (visible height ~19 units)
  SetProjectionFov_40DCA3(0.9f);

  // idle timeline (the duel round-start pose) + sane time base + menu cursor
  CSekBody__PlayAnim_40179D(s_pHeroBody, 1);
  FrameTimer_BeginFrame();
  RestartInputThread(2);

  // arm the device-reinit watch (checked every frame in the loop)
  s_pDevAtLoad = g_pD3DDevice_4D6C78;
  DebugLog("[HNH][Custom] hero loaded on device 0x%p, entering loop\n", s_pDevAtLoad);
  return TRUE;
}

static void ShowWarning() {
  char szPathBuf[152];

  // --- one-time intro ----------------------------------------------------
  // war.bmp splash stretched over the primary surface (640x480 design
  // resolution, scaled to the current mode).
  wsprintfA(szPathBuf, "war.bmp");                  // aWarBmp @0x4C72F8
  CHanaObj__GetRealPath_486D9A(szPathBuf, 1007);    // 1007 = GRP dir
  LPDIRECTDRAWSURFACE7 pWarSurface = GetSurfaceFromBmp_419DE5(szPathBuf);
  if (pWarSurface)
  {
    RECT rc;
    SetRect(&rc, 0, 0, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    g_pPrimaryBuffer_4D6C88->Blt(&rc, pWarSurface, NULL, DDBLT_WAIT, NULL);
    pWarSurface->Release();
  }
}

// experiment 2 input: LMB-drag orbit + arrow-key zoom
static void HandleOrbitInput(float fDeltaTime)
{
  static DWORD s_dwPrevX = 0;
  static DWORD s_dwPrevY = 0;
  static BOOL s_bHavePrev = FALSE;

  WORD wBtn = 0;
  DWORD dwX = 0, dwY = 0;
  GetMouseBtn_45A41B(&wBtn);
  GetMouseXY_45A4D8(&dwX, &dwY);

  if ( s_bHavePrev && (wBtn & 1) )                     // LMB held: orbit
  {

    s_fCamYaw += ((float)(LONG)dwX - (LONG)s_dwPrevX) * 0.005f;
    s_fCamPitch -= ((float)(LONG)dwY - (LONG)s_dwPrevY) * 0.005f;
    if ( s_fCamPitch > 1.3f )
      s_fCamPitch = 1.3f;                              // near top-down
    if ( s_fCamPitch < -0.1f )
      s_fCamPitch = -0.1f;                             // stay above ground level

  }
  s_dwPrevX = dwX;
  s_dwPrevY = dwY;
  s_bHavePrev = TRUE;

  if ( GetAsyncKeyState(VK_UP) & 0x8000 )              // zoom in/out
    s_fCamDist -= 20.0f * fDeltaTime;
  if ( GetAsyncKeyState(VK_DOWN) & 0x8000 )
    s_fCamDist += 20.0f * fDeltaTime;
  if ( s_fCamDist < 12.0f )
    s_fCamDist = 12.0f;
  if ( s_fCamDist > 80.0f )
    s_fCamDist = 80.0f;
}

// experiment 3 input: number keys 0..9 hard-cut the hero timeline item
static void HandlePoseInput(void)
{
  static BOOL s_abPrevKey[10] = { FALSE };

  for ( int i = 0; i < 10; ++i )
  {
    BOOL bDown = (GetAsyncKeyState('0' + i) & 0x8000) != 0;
    if ( bDown && !s_abPrevKey[i] )
    {
      SekBody__UpdateTo_4014E4(s_pHeroBody, i);
      DebugLog("[HNH][Custom] timeline item %d\n", i);
    }
    s_abPrevKey[i] = bDown;
  }
}

void ResetGameData(void) {
  // reset the UI-effect DirectSound buffers and both footstep-sound tables
  memset(g_arrUIEffectDSBuffer_52F168, 0, sizeof(g_arrUIEffectDSBuffer_52F168));
  memset(g_arrFootstepSound_52F18C, 0, sizeof(g_arrFootstepSound_52F18C));
  memset(g_arrFootstepSound_52F17C, 0, sizeof(g_arrFootstepSound_52F17C));

  // reset the data0.dat save image, default AVI volume (2), byte_530267 := 1
  memset(&g_sysDataSave_530170, 0, sizeof(g_sysDataSave_530170));

  g_sysDataSave_530170.g_nVolumeLevel_530266 = 2;
  g_sysDataSave_530170.g_nTextSpeed_530267 = 1;

  // load the previous save if present: <DATA>\data0.dat
  CHAR szPathBuf[152];               // [esp+4h] [ebp-B4h]

  wsprintfA(szPathBuf, "data0.dat");                // aData0Dat @0x4C7300
  CHanaObj__GetRealPath_486D9A(szPathBuf, 1010);    // 1010 = DATA dir
  if (IsFileExists_47E8EE(szPathBuf))
    ReadFileToBuffer(szPathBuf, &g_sysDataSave_530170, sizeof(g_sysDataSave_530170));

  RestartInputThread(0);
  ReleaseSoundEffects();
}

// mouse cursor sprite - device-dependent (D3DX texture), so it must be
// re-created after any device re-init; NULL it to force a reload
static CHanaSpriteT3F6_S1FC *s_pMouseSprite = NULL;

CHanaSpriteT3F6_S1FC * MouseCursor() {
  if (!s_pMouseSprite) {
    CHAR szFileName[256];                  // [esp+D0h] [ebp-174h]

    wsprintfA(szFileName, "mouse.bmp");                     // aMouseBmp_2 @0x4C6CBC
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);         // 1003 = SPR dir
    LoadBmpWithMask_413A5E(&s_pMouseSprite, szFileName, szFileName);
  }
  return s_pMouseSprite;
}

void GameCustomLoop(void)
{

  ResetGameData();
  ShowWarning();

  if ( !s_pHeroBody && !GameCustomSetup() )
    return;

  while ( 1 )
  {
    int mouseX, mouseY;

    DoPeekMessages();

    float fDeltaTime = (float)GetFrameTime();
    if ( fDeltaTime > 5.0f )
      fDeltaTime = 5.0f;
    FrameTimer_EndFrame();

    if ( IsMenuKeyPressed_481170() )                  // ESC exits the experiment
      break;

    // device re-created underneath us? -> every device object we hold is dead;
    // drop the hero + cursor and rebuild them on the new device / new tree
    if ( g_pD3DDevice_4D6C78 != s_pDevAtLoad )
    {
      DebugLog("[HNH][Custom] device re-created 0x%p -> 0x%p, reloading actors\n",
               s_pDevAtLoad, g_pD3DDevice_4D6C78);
      s_pHeroBody = NULL;
      s_pMouseSprite = NULL;
      if ( !GameCustomSetup() )
        break;
    }

    CHanaSpriteT3F6_S1FC *pMouseCursor = MouseCursor();

    GetMouseXY_45A4D8((DWORD *)&mouseX, (DWORD *)&mouseY);

    BOOL bDown = (GetAsyncKeyState('C') & 0x8000) != 0;

    HandleOrbitInput(fDeltaTime);
    HandlePoseInput();
    UpdateOrbitCamera();

    // advance the hero timeline (writes matLocal_138), then rebuild world mats
    CSekBody__UpdateAnim_4019E1(s_pHeroBody, fDeltaTime);

    MtxStackStartMultLocal_4074EF();
    // 3D pass - the canonical scene render skeleton
    g_pRenderCtx->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);

    if (SUCCEEDED(g_pD3DDevice_4D6C78->BeginScene()))
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
      HanaFrame_Draw((CHanaFrameT3EC_S354*)g_pSceneFrame);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      HanaVert_DrawBatch(1);
      g_pD3DDevice_4D6C78->EndScene();
    }

    if (SUCCEEDED(g_pD3DDevice_4D6C78->BeginScene()))
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
      if (pMouseCursor) {
        CHanaSpriteT3F6__SetPos_414A5D(pMouseCursor, (float)(mouseX + 16), (float)(mouseY + 16));
        DrawSprite2D(pMouseCursor);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }

    PresentFrame();
    HanaVert_DrawBatch(0);
  }

  RestartInputThread(0);
  DebugLog("[HNH][Custom] loop end\n");
}
