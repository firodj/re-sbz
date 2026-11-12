#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === EXPERIMENT 1: render the otoko hero body ===
//
// Minimal custom scene loop, dispatched from WinMain_485CE0 like GameMainLoop.
// Loads the hero pair (otoko\otoko.sek + .odf, same as PlayExploring/PlayHunting),
// parks it in front of a manually aimed camera, and plays the idle timeline
// (SEK item 1) until ESC. Render skeleton copied from the scene players:
// UpdateAnim -> MtxStackStartMultLocal -> Clear(3) -> BeginScene ->
// HanaFrame_Draw + HanaVert_DrawBatch -> PresentFrame.

static CSekBody *s_pHeroBody = NULL;

// placement constants (world units; hero is ~16 units tall, cf. g_fHitY_4C6F48)
#define CUSTOM_HERO_Z   25.0f
#define CUSTOM_CAM_Y     8.0f
#define CUSTOM_CAM_Z    10.0f

static CHanaFrameT3EC_S354 *HeroFrame(void)
{
  return s_pHeroBody ? s_pHeroBody->pSekHeader_220->pFrameT3EC : NULL;
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

  // park the hero in front of the origin, facing the camera position below
  // (SetupHeroEntry uses this same SetFramePosRel + UpdateBoneLookAt pair)
  SetFramePosRel_4069DD(HeroFrame(), (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                        0.0f, 0.0f, CUSTOM_HERO_Z);
  UpdateBoneLookAtTransform(HeroFrame(), (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

  // camera above and behind the origin, aimed at the hero. The view matrix is
  // rebuilt from this frame's world pose on every HanaFrame_Draw, so parking it
  // once is enough. Camera(0, 8, 10) -> hero(0, 0, 25) is the 8-15-17 right
  // triangle, so the look vector normalizes to exactly (0, -8/17, 15/17).
  SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                        0.0f, CUSTOM_CAM_Y, CUSTOM_CAM_Z);
  UpdateBoneLookAtTransform(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            0.0f, -8.0f / 17.0f, 15.0f / 17.0f,
                            0.0f, 1.0f, 0.0f);

  // idle timeline (the duel round-start pose) + sane time base + menu cursor
  CSekBody__PlayAnim_40179D(s_pHeroBody, 1);
  FrameTimer_BeginFrame();
  RestartInputThread(2);
  DebugLog("[HNH][Custom] hero loaded, entering loop\n");
  return TRUE;
}

void GameCustomLoop(void)
{
  if ( !s_pHeroBody && !GameCustomSetup() )
    return;

  while ( 1 )
  {
    DoPeekMessages();

    float fDeltaTime = (float)GetFrameTime();
    if ( fDeltaTime > 5.0f )
      fDeltaTime = 5.0f;
    FrameTimer_EndFrame();

    if ( IsMenuKeyPressed_481170() )                  // ESC exits the experiment
      break;

    // advance the hero timeline (writes matLocal_138), then rebuild world mats
    CSekBody__UpdateAnim_4019E1(s_pHeroBody, fDeltaTime);
    MtxStackStartMultLocal_4074EF();

    // 3D pass - the canonical scene render skeleton
    g_pRenderCtx->Clear(3);
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
      HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      HanaVert_DrawBatch(1);
      g_pD3DDevice_4D6C78->EndScene();
    }
    PresentFrame();
    HanaVert_DrawBatch(0);
  }

  RestartInputThread(0);
  DebugLog("[HNH][Custom] loop end\n");
}
