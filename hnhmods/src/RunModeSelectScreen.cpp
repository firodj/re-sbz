#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*RunModeSelectScreen)(CHanaSceneT458_S504 *pScene,
    CHanaSpriteT3F6_S1FC **arrModeSprites, CHanaSpriteT3F6_S1FC **arrMouseSprites,
    int *piSelected, int iModeId, unsigned __int64 *pSelMask, int iMenuTab) =
    reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, CHanaSpriteT3F6_S1FC **,
      CHanaSpriteT3F6_S1FC **, int *, int, unsigned __int64 *, int)>(0x46F37E);

int (__cdecl*SetFrameVisibilityAndGetHIndex)(CHanaSceneT458_S504 *pScene, int iVisState) =
    reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, int)>(0x467AF3);

CHanaFrameT3EC_S354 *(__cdecl*ApplyFrameEffectOption)(CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(CHanaSceneT458_S504 *)>(0x467420);
void (__cdecl*DrawSpritePulse_46D7A7)(CHanaSpriteT3F6_S1FC *pSprite, float fPhase) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *, float)>(0x46D7A7);

int (__cdecl*RunSceneSelectScreen)(CHanaSceneT458_S504 *pScene,
  CHanaSpriteT3F6_S1FC **arrModeSprites, CHanaSpriteT3F6_S1FC **arrMouseSprites,
  int *piSelected, int iSceneGroup, unsigned __int64 *pSelMask, int iMenuTab) =
  reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, CHanaSpriteT3F6_S1FC **,
    CHanaSpriteT3F6_S1FC **, int *, int, unsigned __int64 *, int)>(0x46D9AF);

// === PINNED GLOBALS ===
LPDIRECTSOUNDBUFFER &g_pDSBuf_52EFD0 = *(LPDIRECTSOUNDBUFFER *)0x52EFD0;  // WRITABLE

// === FUNCTIONS ===


// RunModeSelectScreen_46F37E -- the 5-button "mode" choice screen (sibling of
// RunSceneSelectScreen; sole caller PlayAssault_4703AE: odd mode id -> here,
// even -> scene select).
//
// arrModeSprites[0..6]: [0] pulse indicator (parked at (600,450)), [1..5]
// the five choice buttons at (215,205)/(425,205)/(215,275)/(425,275)/
// (320,240) -- identical on every page, so the original's 126-float x5-page
// table collapses to one arrBtnGeom[6] = {x, y, alphaTarget} -- and [6] the
// panel background (DrawSpriteLTRB 64,112,576,368). All sprites start
// hidden (alpha 0, parked -1111,-9999, scale 1); alpha ramps fFrameTime/10
// toward target = 1.0 normally, 0.5 for already-chosen buttons (bit 1..5 of
// *pSelMask = &g_sceneState.qwSelMask_C8) and for the pulse button while
// choices remain (v42 = bAllSelected = bits 1..5 all set, or iMenuTab != 0
// forces it at start... computed only when !iMenuTab).
//
// piSelected in/out: in = page 0..4 (geometry row; all rows identical), out
// = chosen button 0..4 (mask bit 1<<(btn+1) OR-ed into *pSelMask), or -1 =
// all five chosen and the pulse button clicked.
//
// Loop: menu key -> PauseSceneAudio + SystemMenu on the global BGM
// g_pDSBuf_52EFD0 (tab = iMenuTab ? 2 : 0) over a CSceneState snapshot with
// the protagonist HP pair; >= 1 returns immediately (after
// ApplyFrameEffectOption). Scene time advances (a2 starts 9999999); the 3D
// camera looks at the "_mune_" (chest) frame: SetFrameVisibilityAndGetHIndex
// (vis 1000) + FindFrameByName_47E999 + CameraLookAtFrame mode -1.
// Buttons: hit-test destWHZ_138 +-64 x / +-32 y while alpha != 0, a 30.0
// click-wait timer arms each button once; hover scales 1.2 (pulse button
// only when bAllSelected); any new press while hovered selects (click
// sound? none here). Click pulse button with bAllSelected -> *piSelected
// = -1 return; click button i -> choice.
//
// Render: pSurface_468 blit (antialias off) or Clear(3) + HanaFrame_Draw
// pass; panel + buttons + cursor (mouse + 16, alpha ramping); pulse via
// DrawSpritePulse_46D7A7(phase = accumulated time) when sprite [0] alpha is
// 1.0; PresentFrame, then D3DX Clear(3) again (next frame's 3D pass).
// Returns 0 on choice/completion, else the SystemMenu value.

int __cdecl hook_RunModeSelectScreen(CHanaSceneT458_S504 *pScene,
    CHanaSpriteT3F6_S1FC **arrModeSprites, CHanaSpriteT3F6_S1FC **arrMouseSprites,
    int *piSelected, int iModeId, unsigned __int64 *pSelMask, int iMenuTab)
{
    CSceneState sceneSnapshot;              // [ebp-434h] SystemMenu arg copy
    CHanaFrameT3EC_S354 *pTargetFrame;      // [ebp-24h] "mune" frame
    DWORD dwAaCur;
    float fSceneTime, fFrameTime, fClock, fClickWait, fAlphaTarget;
    int nResult, nPage, nHoverBtn, i;
    int xMouse, yMouse;
    WORD nMouseBtn = 0, nPrevBtn;

    // (the original's 126-float x5-page table -- all pages identical)
    static const float arrBtnGeom[6][3] = {
        { 600.0f, 450.0f, 1.0f },           // [0] pulse indicator
        { 215.0f, 205.0f, 1.0f },           // [1..5] choice buttons
        { 425.0f, 205.0f, 1.0f },
        { 215.0f, 275.0f, 1.0f },
        { 425.0f, 275.0f, 1.0f },
        { 320.0f, 240.0f, 1.0f },
    };

    DebugLog("[HNH:SUB] RunModeSelectScreen_46F37E\n");

    pTargetFrame = NULL;
    nResult = 0;
    fClickWait = 0.0f;
    fSceneTime = 9999999.0f;
    fClock = 0.0f;
    nPrevBtn = 1;
    nHoverBtn = -100;
    nPage = *piSelected;                    // geometry row 0..4

    // camera target: the "_mune_" frame (aMune_0 @0x4C5C2C, ASCII)
    if (pScene)
    {
        int HIndex = SetFrameVisibilityAndGetHIndex(pScene, 1000);
        pTargetFrame = FindFrameByName_47E999(pScene->refsFrames_134[HIndex], "_mune_");
    }

    // hide everything to start
    for (i = 0; i < 7; ++i)
    {
        if (arrModeSprites[i])
        {
            CHanaObj__SetAlpha3F6(arrModeSprites[i], 0.0f);
            CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i], -1111.0f, -9999.0f);
            CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
                    (float)(int)ConvertFrom640AsFloat(1.0f));
        }
    }

    // all-selected? (only tracked when the menu tab is 0)
    int bAllSelected = 1;
    if (!iMenuTab)
    {
        for (i = 1; i < 6; ++i)
        {
            if (!((1 << i) & (DWORD)*pSelMask))
                bAllSelected = 0;
        }
    }
    FrameTimer_EndFrame();

    for (;;)
    {
        DoPeekMessages();
        fFrameTime = (float)GetFrameTime();
        if (fFrameTime > 5.0f)
            fFrameTime = 5.0f;
        fClock += fFrameTime;
        FrameTimer_EndFrame();

        // menu key -> system menu on the global BGM, tab per iMenuTab
        if (IsMenuKeyPressed_481170())
        {
            PauseSceneAudio_46A2A0(pScene, 1);
            StopAndRestartThread(0);
            memcpy(&sceneSnapshot, &g_sceneState_52F7E0, sizeof(sceneSnapshot));
            nResult = SystemMenu_4655AB(g_pDSBuf_52EFD0, -1000, iMenuTab ? 2 : 0,
                                        g_sceneState_52F7E0.protCurrentHP_C0,
                                        g_sceneState_52F7E0.protMaximumHP_C4,
                                        sceneSnapshot);
            StopAndRestartThread(2);
            ApplyFrameEffectOption(pScene);
            PauseSceneAudio_46A2A0(pScene, 0);
            if (nResult >= 1)
                return nResult;
        }

        nPrevBtn = nMouseBtn;
        GetMouseBtn_45A41B(&nMouseBtn);
        GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

        // scene + camera advance
        if (pScene)
        {
            UpdateSceneTimer_4827FE(pScene, &fSceneTime, fFrameTime);
            UpdateSceneAtTime_4825D5(pScene, &fSceneTime, fFrameTime);
        }
        if (pTargetFrame)
            CameraLookAtFrame(g_pCameraFrame, pTargetFrame,
                              (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);

        // position / fade / hit-test the buttons
        for (i = 0; i < 6; ++i)
        {
            if (!arrModeSprites[i])
                continue;
            CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i],
                    (float)(int)ConvertFrom640AsFloat(arrBtnGeom[i][0]),
                    (float)(int)ConvertFrom640AsFloat(arrBtnGeom[i][1]));

            fAlphaTarget = 1.0f;
            if (i)
            {
                if ((1 << i) & (DWORD)*pSelMask)
                    fAlphaTarget = 0.5f;    // already chosen
            }
            else if (!bAllSelected)
            {
                fAlphaTarget = 0.5f;        // pulse dim while choices remain
            }
            if (arrModeSprites[i]->fAlpha_144 <= arrBtnGeom[i][2])
            {
                if (fAlphaTarget * arrBtnGeom[i][2] > arrModeSprites[i]->fAlpha_144)
                {
                    arrModeSprites[i]->fAlpha_144 += fFrameTime / 10.0f;
                    if (fAlphaTarget * arrBtnGeom[i][2] < arrModeSprites[i]->fAlpha_144)
                        arrModeSprites[i]->fAlpha_144 = fAlphaTarget * arrBtnGeom[i][2];
                }
            }
            else
            {
                arrModeSprites[i]->fAlpha_144 -= fFrameTime / 10.0f;
                if (fAlphaTarget * arrBtnGeom[i][2] > arrModeSprites[i]->fAlpha_144)
                    arrModeSprites[i]->fAlpha_144 = fAlphaTarget * arrBtnGeom[i][2];
            }
            CHanaObj__SetAlpha3F6(arrModeSprites[i], arrModeSprites[i]->fAlpha_144);
            CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
                    (float)(int)ConvertFrom640AsFloat(1.0f));

            nHoverBtn = -100;
            if (arrModeSprites[i]->fAlpha_144 != 0.0f)
            {
                if ((double)xMouse > arrModeSprites[i]->destWHZ_138.x - ConvertFrom640AsFloat(64.0f)
                    && ConvertFrom640AsFloat(64.0f) + arrModeSprites[i]->destWHZ_138.x > (double)xMouse
                    && (double)yMouse > arrModeSprites[i]->destWHZ_138.y - ConvertFrom640AsFloat(32.0f)
                    && ConvertFrom640AsFloat(32.0f) + arrModeSprites[i]->destWHZ_138.y > (double)yMouse
                    && fClickWait > 30.0f)
                {
                    fClickWait = 30.0f;     // arm
                    if ((!i && bAllSelected) || i)
                    {
                        CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
                                (float)(int)ConvertFrom640AsFloat(1.2f));  // hover
                    }
                    if (nMouseBtn && nPrevBtn != nMouseBtn)
                    {
                        nHoverBtn = i - 1;   // -1 = pulse button
                        break;
                    }
                }
            }
        }
        fClickWait += fFrameTime;

        if (nHoverBtn == -1 && bAllSelected == 1)
        {
            *piSelected = -1;                // all choices done
            return nResult;
        }
        if (nHoverBtn > -1)
        {
            nResult = 0;
            *piSelected = nHoverBtn;
            *pSelMask |= (unsigned __int64)1 << (nHoverBtn + 1);
            return nResult;
        }

        // background: scene image, or the 3D scene
        if (pScene && pScene->pSurface_468)
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAaCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, pScene->pSurface_468, NULL, DDBLT_WAIT, NULL);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAaCur);
        }
        else
        {
            MtxStackStartMultLocal_4074EF();
            g_pRenderCtx->Clear(3);
            if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
            {
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
                HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                HanaVert_DrawBatch(1);
                g_pD3DDevice_4D6C78->EndScene();
            }
        }

        // panel + buttons + cursor + pulse
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
            if (arrModeSprites[6])
            {
                CHanaObj__SetAlpha3F6(arrModeSprites[6],
                        fFrameTime / 10.0f + arrModeSprites[6]->fAlpha_144);
                DrawSpriteLTRB(arrModeSprites[6],
                        ConvertFrom640AsInt(64), ConvertFrom640AsInt(112),
                        ConvertFrom640AsInt(576), ConvertFrom640AsInt(368));
            }
            for (i = 0; i < 6; ++i)
            {
                if (arrModeSprites[i])
                {
                    CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i],
                            (float)(int)ConvertFrom640AsFloat(arrBtnGeom[i][0]),
                            (float)(int)ConvertFrom640AsFloat(arrBtnGeom[i][1]));
                    DrawSprite2D(arrModeSprites[i]);
                }
            }
            if (*arrMouseSprites)
            {
                CHanaObj__SetAlpha3F6(*arrMouseSprites,
                        fFrameTime / 10.0f + (*arrMouseSprites)->fAlpha_144);
                CHanaSpriteT3F6__SetPos_414A5D(*arrMouseSprites,
                        (float)(xMouse + 16), (float)(yMouse + 16));
                DrawSprite2D(*arrMouseSprites);
                if ((*arrModeSprites)->fAlpha_144 == 1.0f)
                    DrawSpritePulse_46D7A7(*arrModeSprites, fClock);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }

        PresentFrame();
        HanaVert_DrawBatch(0);
        g_pRenderCtx->Clear(3);             // next frame's 3D pass
    }
}


// RunSceneSelectScreen_46D9AF -- the scene-grid choice screen (sibling of
// RunModeSelectScreen; PlayAssault_4703AE dispatches even mode ids here).
// 5 pages (nPage = *piSelected / 4 on entry); each page shows 8 buttons
// (plus the shared pulse indicator [0]):
//   buttons 1..5  the five MODE choices -- selmask bits 1..5 (shared with
//                 RunModeSelectScreen), output scenes 0/4/8/12/16 (group
//                 starts)
//   buttons 6..8  per-group EXTRA scenes -- bits 9..20 (three per group),
//                 outputs +1..+4 offsets; page 3 (group 3) has only one
//                 extra (buttons 7,8 output -1 and are parked offscreen)
// The original's ~600 lines of table noise (v47/v48/v49 float stores +
// v31/v32/v33 bit stores + v37 output stores) collapses to three
// [5][...]-page tables below. arrModeSprites is 26 sprites: [0] pulse,
// [1..8] buttons, [9..13] page labels, [14..15] panels, [16] page-4 panel,
// [17..25] page-4 button mirrors.
//
// Unlock gating (sceneFlags_98[iSceneGroup]):
//   flags & 0x3FF == 0 (nothing done in the group) -> the pulse indicator
//     is hidden on pages 0..3 (page 4 keeps it);
//   each of bits 0..2 clear -> page-4 extra button 6+bit parked at x=-999.
// bAllSelected = selmask bits 1..4 set (the original re-checks the same
// four bits five times via its per-page table).
//
// Loop identical in shape to RunModeSelectScreen: menu key -> SystemMenu on
// g_pDSBuf_52EFD0 (tab = iMenuTab ? 2 : 0, protagonist HP pair, snapshot)
// -> >= 1 returns; scene timer advances (starts 9999999); camera looks at
// the "_mune_" frame; buttons fade fFrameTime/10 toward 1.0 (0.5 for
// already-chosen bits / pulse while choices remain), hit-test
// destWHZ_138 +-64x/+-32y with the 30.0 click-wait, hover scale 1.2.
// Pulse clicked with all-selected -> *piSelected = -1; button clicked ->
// *piSelected = choiceOut[page][btn-1], selmask |= 1<<bit, return 0.
//
// Render: pSurface_468 blit or Clear(3)+3D pass; panel [14] (88,28,560,
// 228) or [16] on page 4, panel [15] (88,248,560,445) always; buttons
// (page 4 draws its mirrors [i+11] with alpha/scale copied from [i] and
// skips [6..8] themselves); page label [9+page] at (320,108) fading in;
// cursor; pulse with phase = -clock. PresentFrame, Clear(3).

int __cdecl hook_RunSceneSelectScreen(CHanaSceneT458_S504 *pScene,
  CHanaSpriteT3F6_S1FC **arrModeSprites, CHanaSpriteT3F6_S1FC **arrMouseSprites,
  int *piSelected, int iSceneGroup, unsigned __int64 *pSelMask, int iMenuTab)
{
  CSceneState sceneSnapshot;              // [ebp-C88h] SystemMenu arg copy
  CHanaFrameT3EC_S354 *pTargetFrame;      // [ebp-24h] "mune" frame
  DWORD dwAaCur;
  float fSceneTime, fFrameTime, fClock, fClickWait, fAlphaTarget;
  int nResult, nPage, nHoverBtn, i, j;
  int xMouse, yMouse;
  WORD nMouseBtn = 0, nPrevBtn;
  unsigned __int64 qwSelMask;

  // page geometry {x, y, alphaTarget} for sprites 0..8 (pages 0..2 and 4
  // identical; page 3 parks buttons 7/8 offscreen -- they output -1)
  static const float arrGeom[5][9][3] = {
    { {600,450,1},{215,315,1},{425,315,1},{215,385,1},{425,385,1},
    {320,350,1},{320,184,1},{195,108,1},{445,108,1} },
    { {600,450,1},{215,315,1},{425,315,1},{215,385,1},{425,385,1},
    {320,350,1},{320,184,1},{195,108,1},{445,108,1} },
    { {600,450,1},{215,315,1},{425,315,1},{215,385,1},{425,385,1},
    {320,350,1},{320,184,1},{195,108,1},{445,108,1} },
    { {600,450,1},{215,315,1},{425,315,1},{215,385,1},{425,385,1},
    {320,350,1},{320,184,1},{-999,0,0},{0,-999,0} },
    { {600,450,1},{215,315,1},{425,315,1},{215,385,1},{425,385,1},
    {320,350,1},{320,184,1},{195,108,1},{445,108,1} },
  };
  // selmask bit per page/button (index 0 = pulse; original stores a dead
  // bit there too) -- buttons 1..5 always bits 1..5, extras 9..20
  static const int arrBit[5][9] = {
    { 0, 1, 2, 3, 4, 5,  6,  7,  8 },
    { 0, 1, 2, 3, 4, 5,  9, 10, 11 },
    { 0, 1, 2, 3, 4, 5, 12, 13, 14 },
    { 0, 1, 2, 3, 4, 5, 15, 16, 17 },
    { 0, 1, 2, 3, 4, 5, 18, 19, 20 },
  };
  // *piSelected output per page / button 1..8 (page 3 extras 7,8 = -1)
  static const int arrChoiceOut[5][8] = {
    { 0, 4,  8, 12, 16,  1,  2,  3 },
    { 0, 4,  8, 12, 16,  5,  6,  7 },
    { 0, 4,  8, 12, 16,  9, 10, 11 },
    { 0, 4,  8, 12, 16, 13, -1, -1 },
    { 0, 4,  8, 12, 16, 17, 18, 19 },
  };
  // runtime-tweaked copy (unlock gating below parks sprites)
  float arrPos[5][9][3];
  memcpy(arrPos, arrGeom, sizeof(arrPos));

  ONCELOG("[HNH:SUB] RunSceneSelectScreen_46D9AF\n");

  pTargetFrame = NULL;
  nResult = 0;
  fClickWait = 0.0f;
  fSceneTime = 9999999.0f;
  fClock = 0.0f;
  nPrevBtn = 1;
  nHoverBtn = -1;
  nPage = *piSelected / 4;                // scene index -> page 0..4
  qwSelMask = *pSelMask;

  // camera target: the "_mune_" frame (aMune @0x4C5C20, ASCII)
  if (pScene)
  {
    int HIndex = SetFrameVisibilityAndGetHIndex(pScene, 1000);
    pTargetFrame = FindFrameByName_47E999(pScene->refsFrames_134[HIndex], "_mune_");
  }

  // hide everything to start
  for (i = 0; i < 30; ++i)
  {
    if (arrModeSprites[i])
    {
      CHanaObj__SetAlpha3F6(arrModeSprites[i], 0.0f);
      CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i], -1111.0f, -9999.0f);
      CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
        (float)(int)ConvertFrom640AsFloat(1.0f));
    }
  }

  // unlock gating from the group's scene flags
  if ((g_sceneState_52F7E0.sceneFlags_98[iSceneGroup] & 0x3FF) == 0)
  {
    for (i = 0; i < 4; ++i)
      arrPos[i][0][0] = -999.0f;      // hide pulse on pages 0..3
  }
  for (i = 0; i < 3; ++i)
  {
    if (!(g_sceneState_52F7E0.sceneFlags_98[iSceneGroup] & ((QWORD)1 << i)))
      arrPos[4][6 + i][0] = -999.0f;   // park page-4 extras
  }

  // all-selected = selmask bits 1..4 (the original re-checks the same
  // bits once per page through its table)
  int bAllSelected = 1;
  if (!iMenuTab)
  {
    for (j = 0; j < 5; ++j)
    {
      for (i = 1; i <= 4; ++i)
      {
        if (!((unsigned __int64)1 << arrBit[j][i] & qwSelMask))
          bAllSelected = 0;
      }
    }
  }
  FrameTimer_EndFrame();

  for (;;)
  {
    DoPeekMessages();
    fFrameTime = (float)GetFrameTime();
    if (fFrameTime > 5.0f)
      fFrameTime = 5.0f;
    fClock += fFrameTime;
    FrameTimer_EndFrame();

    // menu key -> system menu on the global BGM, tab per iMenuTab
    if (IsMenuKeyPressed_481170())
    {
      PauseSceneAudio_46A2A0(pScene, 1);
      StopAndRestartThread(0);
      memcpy(&sceneSnapshot, &g_sceneState_52F7E0, sizeof(sceneSnapshot));
      nResult = SystemMenu_4655AB(g_pDSBuf_52EFD0, -1000, iMenuTab ? 2 : 0,
        g_sceneState_52F7E0.protCurrentHP_C0,
        g_sceneState_52F7E0.protMaximumHP_C4,
        sceneSnapshot);
      StopAndRestartThread(2);
      ApplyFrameEffectOption(pScene);
      PauseSceneAudio_46A2A0(pScene, 0);
      if (nResult >= 1)
        return nResult;
    }

    nPrevBtn = nMouseBtn;
    GetMouseBtn_45A41B(&nMouseBtn);
    GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

    // scene + camera advance
    if (pScene)
    {
      UpdateSceneTimer_4827FE(pScene, &fSceneTime, fFrameTime);
      UpdateSceneAtTime_4825D5(pScene, &fSceneTime, fFrameTime);
    }
    if (pTargetFrame)
      CameraLookAtFrame(g_pCameraFrame, pTargetFrame,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);

    // position / fade / hit-test the buttons
    for (i = 0; i < 9; ++i)
    {
      if (!arrModeSprites[i])
        continue;
      CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i],
        (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][0]),
        (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][1]));

      fAlphaTarget = 1.0f;
      if (i)
      {
        if (((unsigned __int64)1 << arrBit[nPage][i]) & qwSelMask)
          fAlphaTarget = 0.5f;    // already chosen
      }
      else if (!bAllSelected)
      {
        fAlphaTarget = 0.5f;        // pulse dim while choices remain
      }
      if (arrModeSprites[i]->fAlpha_144 <= (double)arrPos[nPage][i][2])
      {
        if (arrModeSprites[i]->fAlpha_144 < (double)arrPos[nPage][i][2])
        {
          arrModeSprites[i]->fAlpha_144 += fFrameTime / 10.0f;
          if (fAlphaTarget * arrPos[nPage][i][2] < arrModeSprites[i]->fAlpha_144)
            arrModeSprites[i]->fAlpha_144 = fAlphaTarget * arrPos[nPage][i][2];
        }
      }
      else
      {
        arrModeSprites[i]->fAlpha_144 -= fFrameTime / 10.0f;
        if (fAlphaTarget * arrPos[nPage][i][2] > arrModeSprites[i]->fAlpha_144)
          arrModeSprites[i]->fAlpha_144 = fAlphaTarget * arrPos[nPage][i][2];
      }
      CHanaObj__SetAlpha3F6(arrModeSprites[i], arrModeSprites[i]->fAlpha_144);
      CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
        (float)(int)ConvertFrom640AsFloat(1.0f));

      nHoverBtn = -100;
      if (arrModeSprites[i]->fAlpha_144 != 0.0f)
      {
        if ((double)xMouse > arrModeSprites[i]->destWHZ_138.x - ConvertFrom640AsFloat(64.0f)
          && ConvertFrom640AsFloat(64.0f) + arrModeSprites[i]->destWHZ_138.x > (double)xMouse
          && (double)yMouse > arrModeSprites[i]->destWHZ_138.y - ConvertFrom640AsFloat(32.0f)
          && ConvertFrom640AsFloat(32.0f) + arrModeSprites[i]->destWHZ_138.y > (double)yMouse
          && fClickWait > 30.0f)
        {
          fClickWait = 30.0f;     // arm
          if ((!i && bAllSelected) || i)
          {
            CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i],
              (float)(int)ConvertFrom640AsFloat(1.2f));  // hover
          }
          if (nMouseBtn && nPrevBtn != nMouseBtn)
          {
            nHoverBtn = i - 1;   // -1 = pulse button
            break;
          }
        }
      }
    }
    fClickWait += fFrameTime;

    if (nHoverBtn == -1 && bAllSelected == 1)
    {
      *piSelected = -1;                // all choices done
      return nResult;
    }
    if (nHoverBtn > -1)
    {
      nResult = 0;
      *piSelected = arrChoiceOut[nPage][nHoverBtn];
      *pSelMask |= (unsigned __int64)1 << arrBit[nPage][nHoverBtn + 1];
      return nResult;
    }

    // background: scene image, or the 3D scene
    if (pScene && pScene->pSurface_468)
    {
      g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAaCur);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pBackBuffer_4D6C90->Blt(NULL, pScene->pSurface_468, NULL, DDBLT_WAIT, NULL);
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAaCur);
    }
    else
    {
      MtxStackStartMultLocal_4074EF();
      g_pRenderCtx->Clear(3);
      if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
      {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
        HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        HanaVert_DrawBatch(1);
        g_pD3DDevice_4D6C78->EndScene();
      }
    }

    // panels + buttons (page-4 mirrors) + page label + cursor + pulse
    if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
      if (nPage == 4)
      {
        if (arrModeSprites[16])
        {
          CHanaObj__SetAlpha3F6(arrModeSprites[16],
            fFrameTime / 10.0f + arrModeSprites[16]->fAlpha_144);
          DrawSpriteLTRB(arrModeSprites[16],
            ConvertFrom640AsInt(88), ConvertFrom640AsInt(28),
            ConvertFrom640AsInt(560), ConvertFrom640AsInt(228));
        }
      }
      else if (arrModeSprites[14])
      {
        CHanaObj__SetAlpha3F6(arrModeSprites[14],
          fFrameTime / 10.0f + arrModeSprites[14]->fAlpha_144);
        DrawSpriteLTRB(arrModeSprites[14],
          ConvertFrom640AsInt(88), ConvertFrom640AsInt(28),
          ConvertFrom640AsInt(560), ConvertFrom640AsInt(228));
      }
      if (arrModeSprites[15])
      {
        CHanaObj__SetAlpha3F6(arrModeSprites[15],
          fFrameTime / 10.0f + arrModeSprites[15]->fAlpha_144);
        DrawSpriteLTRB(arrModeSprites[15],
          ConvertFrom640AsInt(88), ConvertFrom640AsInt(248),
          ConvertFrom640AsInt(560), ConvertFrom640AsInt(445));
      }
      for (i = 0; i < 9; ++i)
      {
        if (nPage == 4 && i >= 6)
        {
          // page 4: extras drawn via their mirrors [i+11]
          if (arrModeSprites[i + 11])
          {
            if (arrModeSprites[i])
            {
              CHanaObj__SetAlpha3F6(arrModeSprites[i + 11],
                arrModeSprites[i]->fAlpha_144);
              CHanaSpriteT3F6__SetScale_414B09(arrModeSprites[i + 11],
                arrModeSprites[i]->fScale_148);
            }
            CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i + 11],
              (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][0]),
              (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][1]));
            DrawSprite2D(arrModeSprites[i + 11]);
          }
        }
        else if (arrModeSprites[i])
        {
          CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[i],
            (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][0]),
            (float)(int)ConvertFrom640AsFloat(arrPos[nPage][i][1]));
          DrawSprite2D(arrModeSprites[i]);
        }
      }
      if (arrModeSprites[nPage + 9])   // page label
      {
        CHanaSpriteT3F6__SetPos_414A5D(arrModeSprites[nPage + 9],
          (float)(int)ConvertFrom640AsFloat(320.0f),
          (float)(int)ConvertFrom640AsFloat(108.0f));
        CHanaObj__SetAlpha3F6(arrModeSprites[nPage + 9],
          fFrameTime / 10.0f + arrModeSprites[nPage + 9]->fAlpha_144);
        DrawSprite2D(arrModeSprites[nPage + 9]);
      }
      if (*arrMouseSprites)
      {
        CHanaObj__SetAlpha3F6(*arrMouseSprites,
          fFrameTime / 10.0f + (*arrMouseSprites)->fAlpha_144);
        CHanaSpriteT3F6__SetPos_414A5D(*arrMouseSprites,
          (float)(xMouse + 16), (float)(yMouse + 16));
        DrawSprite2D(*arrMouseSprites);
        if ((*arrModeSprites)->fAlpha_144 == 1.0f)
          DrawSpritePulse_46D7A7(*arrModeSprites, -fClock);  // negated
      }
      g_pD3DDevice_4D6C78->EndScene();
    }

    PresentFrame();
    HanaVert_DrawBatch(0);
    g_pRenderCtx->Clear(3);             // next frame's 3D pass
  }
}

void register_RunModeSelectScreen() {
  hnh_Hook(0x46D9AF, hook_RunSceneSelectScreen, "RunSceneSelectScreen_46D9AF");
  hnh_Hook(0x46F37E, hook_RunModeSelectScreen, "RunModeSelectScreen_46F37E");
}
