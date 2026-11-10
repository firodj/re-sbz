// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`

#include "dx7.hpp"

#include <cmath>

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===

int (__cdecl*PlayCutInScene_46A76F)(CHanaSceneT458_S504 *pScene,
  CHanaSpriteT3F6_S1FC **arrMouseSprites, CHanaSpriteT3F6_S1FC **arrGoSprites,
  CHanaSpriteT3F6_S1FC **arrPostureSprites, CSekBody *pLockOnBody,
  CSekBody *pModeBody, int iGroup, CSoundStruct_8 **arrSoundAHs,
  CSoundStruct_8 **arrSoundSEs, CSekBody **arrTearsBodies, int iMenuTab) =
  reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *,
    CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **,
    CSekBody *, CSekBody *, int, CSoundStruct_8 **, CSoundStruct_8 **,
    CSekBody **, int)>(0x46A76F);

double (__cdecl*GetSekBodyCurrentAnimTime)(CSekBody *pSekBody) =
    reinterpret_cast<double(__cdecl*)(CSekBody *)>(0x401C67);

void (__cdecl*SetAnimSetTime)(CHanaAnimSetT3F1_S150 *pAnimSet, float fTime) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 *, float)>(0x404277);

void (__cdecl*DrawFadeTransition_Scene)(int bFadeOut, CHanaSceneT458_S504 *pScene,
    CHanaSpriteT3F6_S1FC **arrMouseSprites, CHanaSpriteT3F6_S1FC **arrGoSprites) =
    reinterpret_cast<void(__cdecl*)(int, CHanaSceneT458_S504 *,
      CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **)>(0x468DA8);

void (__cdecl*AnimSet_ResetAndTransferAnimations)(CHanaAnimSetT3F1_S150 *pAnimSetFrom,
    CHanaAnimSetT3F1_S150 *pAnimSetTo, CHanaFrameT3EC_S354 *pFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 *,
      CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x467ABE);

CHanaFrameT3EC_S354 *(__cdecl*FindFrameByNameSubstringRecursive)(
    CHanaFrameT3EC_S354 *pRootFrame, LPCSTR pszSubstr) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(*)(CHanaFrameT3EC_S354 *,
      LPCSTR)>(0x47EA86);

void (__cdecl*IsObjectValidRecursive_467DFD)(CHanaFrameT3EC_S354 *pFrame,
    CHanaFrameT3EC_S354 *pRefFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *)>(0x467DFD);

void (__cdecl*CameraFrame_ProjectFramePairOntoForwardRay)(
    CHanaFrameT3EC_S354 *pTargetFrame, CHanaFrameT3EC_S354 *pFrameA,
    CHanaFrameT3EC_S354 *pFrameB) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *)>(0x46A55D);

void (__cdecl*CameraController_UpdateCutscene)(CHanaFrameT3EC_S354 *pTargetFrame,
    CHanaFrameT3EC_S354 *pMapFrame, int nDX, int nDY, short iMode) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, int, int, short)>(0x466BDA);

BOOL (__cdecl*Scene_ProcessPracticeFrames)(CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<BOOL (__cdecl*)(CHanaSceneT458_S504 *)>(0x4668AC);

void (__cdecl*Cursor_ClampToCenterOrSprite)(int bClamp, int *pX, int *pY,
    int *pClampX, int *pClampY, CHanaSpriteT3F6_S1FC *pSprite) =
    reinterpret_cast<void(__cdecl*)(int, int *, int *, int *, int *,
      CHanaSpriteT3F6_S1FC *)>(0x468A4A);

BOOL (__cdecl*Frame_CheckCameraDistanceAndRecurse)(CHanaFrameT3EC_S354 *pFrame) =
    reinterpret_cast<BOOL(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x4665F0);

void (__cdecl*UpdateSekBodyAnimationMatrix)(CSekBody *pSekBody, float fPhase) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, float)>(0x46A503);

void (__cdecl*UpdateCutInSceneBoneTransforms)(CSekBody *pModeBody,
    CHanaAnimSetT3F1_S150 **arrAnimSetsL, CHanaFrameT3EC_S354 **arrFramesL,
    CHanaFrameT3EC_S354 **arrNakaFrames, CHanaAnimSetT3F1_S150 **arrAnimSetsR,
    CHanaFrameT3EC_S354 **arrKutiRFrames, unsigned int k, float fFade) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, CHanaAnimSetT3F1_S150 **,
      CHanaFrameT3EC_S354 **, CHanaFrameT3EC_S354 **, CHanaAnimSetT3F1_S150 **,
      CHanaFrameT3EC_S354 **, unsigned int, float)>(0x4680B4);

int (__cdecl*MoveCutInSceneFramesWithCameraConstraint)(
    CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pKutiRFrame,
    int nDX, int nDY, float fSpeed) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, int, int, float)>(0x4682D6);

void (__cdecl*UpdateCutInSceneCameraFollow)(CSekBody *pModeBody,
    CHanaAnimSetT3F1_S150 *pAnimSetL, CHanaFrameT3EC_S354 *pFrame,
    CHanaAnimSetT3F1_S150 *pAnimSetR, CHanaFrameT3EC_S354 *pKutiRFrame,
    float fSpeed, float fFade, float fFrameTime, float *pfVelocity,
    float *pfAccumulator) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, CHanaAnimSetT3F1_S150 *,
      CHanaFrameT3EC_S354 *, CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *,
      float, float, float, float *, float *)>(0x4686F8);

void (__cdecl*CutInScene_CyclePosAndApplyCameraOffset)(
    CHanaSceneT458_S504 *pScene, DWORD *arrCyclePos,
    CHanaFrameT3EC_S354 *pTargetFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, DWORD *,
      CHanaFrameT3EC_S354 *)>(0x467160);

void (__cdecl*CameraFrame_TrackBetweenObjectsAndLookAt)(
    CHanaFrameT3EC_S354 *pTargetFrame, CHanaFrameT3EC_S354 *pFrameA,
    CHanaFrameT3EC_S354 *pFrameB) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *)>(0x466978);

void (__cdecl*SoundAH_ManageEffectsAndAnimate)(CSoundStruct_8 **arrSoundAHs,
    CHanaFrameT3EC_S354 *pKutiFuFrame, float *pfEffectIntensity,
    float fMousePressed, float fFrameTime) =
    reinterpret_cast<void(__cdecl*)(CSoundStruct_8 **, CHanaFrameT3EC_S354 *,
      float *, float, float)>(0x4690AB);

void (__cdecl*SoundSE_ManageEffectsAndPlay)(CSoundStruct_8 **arrSoundSEs,
    int nNakaFrame, float fIntensityMinus1, float fFrameTime,
    int nMousePressed, int nEffectIdx) =
    reinterpret_cast<void(__cdecl*)(CSoundStruct_8 **, int, float, float,
      int, int)>(0x4698E8);

void (__cdecl*Character_UpdateAnimationState)(CSekBody *pSekBody, float fParam,
    float *pfVec) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, float, float *)>(0x469EB6);

void (__cdecl*AnimSet_MatchAndSortKutiRTibusaLFrames)(
    CHanaAnimSetT3F1_S150 **arrAnimSetsL, CHanaFrameT3EC_S354 **arrFrames,
    CHanaFrameT3EC_S354 **arrNakaFrames, CHanaAnimSetT3F1_S150 **arrAnimSetsR,
    CHanaFrameT3EC_S354 **arrKutiRFrames, CHanaFrameT3EC_S354 **arrTibusaLFrames,
    CSekBody *pModeBody, float *pfSpeeds, CHanaSpriteT3F6_S1FC **arrSprites,
    float fAnimTime) =
    reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 **,
      CHanaFrameT3EC_S354 **, CHanaFrameT3EC_S354 **, CHanaAnimSetT3F1_S150 **,
      CHanaFrameT3EC_S354 **, CHanaFrameT3EC_S354 **, CSekBody *, float *,
      CHanaSpriteT3F6_S1FC **, float)>(0x467E72);


bool (__cdecl *SyncObjectAnimationByName_467BBD)(CHanaFrameT3EC_S354 *pSourceFrame, CHanaFrameT3EC_S354 *pTargetFrame)
= reinterpret_cast<bool (__cdecl*)(CHanaFrameT3EC_S354 *pSourceFrame, CHanaFrameT3EC_S354 *pTargetFrame)>(0x467bbd);
bool (__cdecl *IsParentValid_467D62)(CHanaFrameT3EC_S354 *pParent, CHanaFrameT3EC_S354 *pCurrent)
= reinterpret_cast<bool (__cdecl*)(CHanaFrameT3EC_S354 *pParent, CHanaFrameT3EC_S354 *pCurrent)>(0x467d62);
BOOL (__cdecl *HanaObj_CheckFrameCondition)(CHanaFrameT3EC_S354 *pFrame, bool bCheck)
= reinterpret_cast<BOOL (__cdecl*)(CHanaFrameT3EC_S354 *pFrame, bool bCheck)>(0x48713f);
void (__cdecl *HanaMorp_AnimateAndPropagate)(CHanaFrameT3EC_S354 *pMorp, float fTime, float fWeight, int nFlag)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pMorp, float fTime, float fWeight, int nFlag)>(0x481be0);
void (__cdecl *HanaMatrix_ApplyTransform)(CHanaFrameT3EC_S354 *pFrame, const D3DXMATRIX *pMat)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame, const D3DXMATRIX *pMat)>(0x40799f);
void (__cdecl *HanaFrame_GetMatAfter)(CHanaFrameT3EC_S354 *a1, D3DXMATRIX *pM)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *a1, D3DXMATRIX *pM)>(0x407945);
void (__cdecl *UpdateFrameAnimationMatrix)(CHanaFrameT3EC_S354 *pFrame, float targetTime, float currentTime)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame, float targetTime, float currentTime)>(0x46a3e9);
void (__cdecl *TransferAnimSetFrameAnimations_4679B2)(CHanaAnimSetT3F1_S150 *pSrcAnimSet, CHanaAnimSetT3F1_S150 *pDstAnimSet, CHanaFrameT3EC_S354 *pFrame)
= reinterpret_cast<void (__cdecl*)(CHanaAnimSetT3F1_S150 *pSrcAnimSet, CHanaAnimSetT3F1_S150 *pDstAnimSet, CHanaFrameT3EC_S354 *pFrame)>(0x4679b2);
  void (__cdecl *Scene_ApplyFadeToPracticeFrames)(float fadeAlpha)
    = reinterpret_cast<void (__cdecl*)(float fadeAlpha)>(0x468b99);


// === PINNED GLOBALS ===
// (none new - every global used here is already extern in shared.hpp)

// === FUNCTIONS ===


// PlayCutInScene_46A76F(pScene, ...) -- the interactive "cut-in" touch scene
// of the practice mode, entered from PlayAssault_4703AE by left-clicking the
// top-right GO prompt strip (x > 576, rows of 48 px) while a sub-scene plays.
// It freezes the sub-scene's script/audio, takes over the mode body
// (h\hXX\hhXX_0N) and lets the player grab and drag its "effect" frames with
// the mouse while a camera cutscene plays around the lock-on target.
//
// pScene: the currently playing sub-scene (map frame + hero body source).
// arrMouseSprites: cursor / face cursor sprites (slot picked by iCursorSlot).
// arrGOTypeSprites: 2 GO-prompt strip sprites (drawn at 608, 16/64).
// arrPostureSprites: 30 per-effect posture sprites, copied into arrSprites
//   and drawn under the cursor while their effect is hovered/selected.
// pLockOnBody: lockon\lock body; its root frame is the camera target frame
//   (pTargetFrame) the cutscene cameras revolve around.
// pModeBody: h\hXX\hhXX_0N body whose frames are manipulated.
// iGroup: practice group 0..4, selecting the arrEffectSpeeds row (drag speed
//   per effect slot; rows 5..9 of the table are all zero).
// arrSoundAHs / arrSoundSEs: PlayAssault's voice / SE sound structs, managed
//   here through SoundAH_ManageEffectsAndAnimate / SoundSE_ManageEffectsAndPlay
//   and stopped on exit.
// arrTearsBodies: h\Tears bodies [0..2]; un-hidden on entry, look-at their
//   anchor frames (found via "_eye_fu1", "_l_momo", "_r_momo"), re-hidden on
//   exit. (Their anim item even/odd state picks UpdateTo 0..3.)
// iMenuTab: system-menu tab while in-scene (0 -> tab 0, nonzero -> tab 2).
//
// Setup: pause scene audio, fade in; tears bodies bAlphaDeferred off; for
// k = 0..5 find in the mode body the frame triplets (main / kuti-R /
// tibusa-L names below) plus the substring-searched NAKA "finger" frame;
// create two uniquely-named scratch anim sets per k ("CutAnimations_1887/
// 6997_<k>_<rand>", attached to the object table) and transfer each found
// frame's animations from the mode anim set into them, so the rest of the
// scene stops driving those frames; apply the effect-level alpha. Reveal the
// scene hero (SetFrameVisibilityAndGetHIndex(1)), cut the mode body's anim
// to the hero's, match/sort all frame triplets into place
// (AnimSet_MatchAndSortKutiRTibusaLFrames with the group's speed row), pose
// the mode body onto the hero, then pre-roll 3 camera update passes
// (modes 1/2/3 + Scene_ProcessPracticeFrames each). animTime comes from
// GetSekBodyCurrentAnimTime while pHeroBody is still NULL (sic - the callee
// tolerates it) and is re-applied to the mode anim set throughout.
//
// Frame loop: DoPeekMessages + GetFrameTime (capped 5.0). Menu key pauses
// audio, opens SystemMenu on the BGM (tab 0/2 by iMenuTab) over a copy of the
// scene state, re-applies the effect fades, re-finds the map frame (>= 1
// exits). Mouse: hover-picks the nearest kuti-R frame (projected to screen,
// pick radius 320/z scaled); left-click (or btn 3) grabs it (mousePressed),
// MoveCutInSceneFramesWithCameraConstraint drags the frame pair with the
// cursor warped to a speed*500 px circle, and drag distance/velocity feeds
// effectIntensity (which drives the mode body anim rate and the SE volume);
// btn 1/2/3 held runs camera cutscene modes 1/2/3 with the drag delta
// (iCursorSlot = 1/2/3, target frame shown); GO strip clicks set breakCode:
// 1 breaks (returns), 2 cycles the camera positions
// (CutInScene_CyclePosAndApplyCameraOffset + re-track). Each arrFrames[k]
// fades in while grabbed (to the effect-level alpha), to half while hovered
// and steady, back out (bAlphaDeferred) otherwise. Tears bodies track their
// anchors; when body[0] reaches anim end and its morp nTimeState_36C == 2
// its facing (dirUpZ) picks the next anim item 0..3. Draw: Clear(3) + 3D
// pass (alpha off for HanaFrame_Draw of g_pSceneFrame, on for
// HanaVert_DrawBatch(1)), then a second alpha pass (SRCBLEND 5 /
// DESTBLEND 6) with the GO strip (fading around x = 576 at 608,16/64) and
// either the hovered effect's posture sprite (clamped to 64..576 x,
// 32..448 y) or the cursor sprite; Present + HanaVert_DrawBatch(0).
//
// Exit: hide the hero, transfer every k's animations back into the mode anim
// set, free the 20 scratch anim sets, restore the hero-mode pose, stop all
// AH/SE sounds, fade out (skipped on a menu exit, menuResult >= 1), unpause
// the scene audio, hide the tears bodies again. Returns menuResult: -1 via
// the GO "1" button (its init value), or the SystemMenu result (>= 1 tells
// PlayAssault to leave the scene).
//
// Notes vs the raw decompilation:
// - The interleaved strcpy/memset piles at the top build three contiguous
//   20-byte-row name tables (6x2 frame-name pairs, 6 tibusa names, 6 NAKA
//   substring names); they collapse to memset + strcpy per row. Dead zero
//   padding behind the tables (60 + 180 bytes) was dropped.
// - arrFrameIdxes really holds frame pointers stored as ints (sic); typed
//   CHanaFrameT3EC_S354* here. SoundSE_ManageEffectsAndPlay receives
//   (int)arrNakaFrames[nCurrentEffectIdx] which reads index -1 while nothing
//   is grabbed - kept faithful (the callee evidently tolerates it).
// - dirForwardVec is the float pair {-1.0f, 5.0f} (bit-pattern stores in the
//   original); arrVelocities/arrAccumulators are float pairs per effect.
// - Dead stores dropped: v69 = 1, v75 = 0, v100 = 5, dirForwardX[1]
//   (var_8A4) = 5, the dx/dy reloads in the drag-clamp block, and two empty
//   pointer-walk "loops" that only re-walk the memset-zeroed arrays.
// - bExitRequested is only ever initialized to 0; its == 1 break never fires
//   (kept faithful).

int __cdecl hook_PlayCutInScene_46A76F(CHanaSceneT458_S504 *pScene,
    CHanaSpriteT3F6_S1FC **arrMouseSprites, CHanaSpriteT3F6_S1FC **arrGOTypeSprites,
    CHanaSpriteT3F6_S1FC **arrPostureSprites, CSekBody *pLockOnBody,
    CSekBody *pModeBody, int iGroup, CSoundStruct_8 **arrSoundAHs,
    CSoundStruct_8 **arrSoundSEs, CSekBody **arrTearsBodies, int iMenuTab)
{
    CSceneState sceneScratch;           // scratch + SystemMenu by-value copy
    CHanaFrameT3EC_S354 *arrFrames[10];       // main effect frames (found)
    CHanaFrameT3EC_S354 *arrKutiRFrames[10];  // kuti-R effect frames
    CHanaFrameT3EC_S354 *arrTibusaLFrames[10];// tibusa-L effect frames
    CHanaFrameT3EC_S354 *arrNakaFrames[10];   // NAKA "finger" frames (sic ints)
    CHanaAnimSetT3F1_S150 *arrAnimSets1[10];  // scratch anim sets (main)
    CHanaAnimSetT3F1_S150 *arrAnimSets2[10];  // scratch anim sets (kuti-R)
    CHanaSpriteT3F6_S1FC *arrSprites[30];     // copy of arrPostureSprites
    CHanaFrameT3EC_S354 *arrTearsFrames[3];   // _eye_fu1 / _l_momo / _r_momo
    CHanaFrameT3EC_S354 *pTargetFrame;  // lock-on body root frame (camera)
    CHanaFrameT3EC_S354 *pMapFrame;     // scene map frame
    CHanaFrameT3EC_S354 *pCameraFrame;  // revealed hero's camera frame
    CSekBody *pHeroBody;                // revealed hero's sek body
    CHanaFrameT3EC_S354 *pFrameKutiFu;  // "_kuti_fu1" frame (AH sounds)
    CHanaFrameT3EC_S354 *pFrameAtariUe; // "atari_ue" hit frame
    CHanaFrameT3EC_S354 *pFrameAtariSita; // "atari_sita" hit frame
    CHanaMorpT457_S40C *pMorpT457;      // tears[0] eye morp state
    String20 arrCutFrameNames[6][2];    // (main, kuti-R) frame names
    String20 arrTibusaNames[6];         // tibusa-L frame names
    String20 arrNakaSearchNames[6];     // NAKA substring names
    float arrEffectSpeeds[10][10];      // per group x effect drag speeds
    float arrVelocities[10][2];         // per effect (vx, vy) drag velocity
    float arrAccumulators[10][2];       // per effect accumulator pair
    float arrFadeProg[10];              // per effect fade progress
    BYTE nFrameSettled[10];             // per effect "velocity settled" flag
                                        // (uninit in the original; only != 1
                                        // is read before any write)
    float effectAlphaMuls[5];           // effect-level alpha multipliers
    float dirFwdVec[2];                 // {-1, 5} handed to Character_Update
    float dirForwardX[2];               // CalculateLocalRotationMatrix outs
    float dirForwardY, dirForwardZ;
    float dirUpX, dirUpY[2], dirUpZ;
    float fRelX, fRelY, fRelZ;          // HanaFrame_GetRelPosition outs
    float fAnimTime;                    // mode body anim time
    float fFrameTimeScaled;             // frameTime (x0.01 on first hover)
    float fBonePhase;                   // 0..54 anim-matrix phase
    float fadeAlpha;                    // camera-cutscene alpha 1..15
    float effectIntensity;              // grab intensity (anim rate/SE vol)
    float closestEffectDist;
    POINT ptProj;
    DWORD arrCyclePos[2];
    CHAR szObjName[256];
    WORD btnMouse = 0, btnMousePrev;
    int xMouse, yMouse;                 // cursor position
    int xDragStart, yDragStart;         // camera-drag origin
    int xGrab = 0, yGrab = 0;                   // grab anchor of the active effect
    int iCursorSlot;                    // arrMouseSprites index (0..3)
    int iHeroIdx;
    int nCurrentEffectIdx, nLastEffectIdx, nSelectedEffectIdx;
    int nMenuResult, nBreakCode;
    int bExitRequested, mousePressed;
    int nDistSq, posY;
    float fRadius;
    float frameTime;                    // loop frame delta (capped 5.0)
    double dPickRadius, dDist;
    int k;

    ONCELOG("[HNH:SUB] PlayCutInScene_46A76F\n");

    // --- state init
    nMenuResult = -1;
    arrCyclePos[0] = 0;
    arrCyclePos[1] = 0;
    nBreakCode = 0;
    pFrameKutiFu = 0;
    effectIntensity = 0.0f;
    dirFwdVec[0] = -1.0f;
    fBonePhase = 0.0f;
    dirFwdVec[1] = 5.0f;
    pFrameAtariUe = 0;
    pFrameAtariSita = 0;
    iCursorSlot = 0;
    pMapFrame = 0;
    pCameraFrame = 0;
    pHeroBody = 0;
    nCurrentEffectIdx = -1;
    nLastEffectIdx = -1;
    nSelectedEffectIdx = -1;
    closestEffectDist = 9999999.0f;
    memset(arrFadeProg, 0, sizeof(arrFadeProg));
    fadeAlpha = 1.0f;
    iHeroIdx = 0;
    bExitRequested = 0;
    mousePressed = 0;
    effectAlphaMuls[0] = 1.0f;
    effectAlphaMuls[1] = 0.8f;
    effectAlphaMuls[2] = 0.5f;
    effectAlphaMuls[3] = 0.2f;
    effectAlphaMuls[4] = 0.0f;

    // --- frame name tables (20-byte rows in the original)
    memset(arrCutFrameNames, 0, sizeof(arrCutFrameNames));
    strcpy(arrCutFrameNames[0][0].str, "NULL_L_UDE");     strcpy(arrCutFrameNames[0][1].str, "NULL_L_TIBUSA");
    strcpy(arrCutFrameNames[1][0].str, "NULL_R_UDE");     strcpy(arrCutFrameNames[1][1].str, "NULL_R_TIBUSA");
    strcpy(arrCutFrameNames[2][0].str, "NULL_K_L_UDE");   strcpy(arrCutFrameNames[2][1].str, "NULL_L_KETU");
    strcpy(arrCutFrameNames[3][0].str, "NULL_K_R_UDE");   strcpy(arrCutFrameNames[3][1].str, "NULL_R_KETU");
    strcpy(arrCutFrameNames[4][0].str, "NULL_NH_L_UDE");  strcpy(arrCutFrameNames[4][1].str, "NULL_NA00_A_TARGET");
    strcpy(arrCutFrameNames[5][0].str, "NULL_NA_R_UDE");  strcpy(arrCutFrameNames[5][1].str, "NULL_R_FUTO");

    memset(arrTibusaNames, 0, sizeof(arrTibusaNames));
    strcpy(arrTibusaNames[0].str, "NULL_L_TIBUSA");
    strcpy(arrTibusaNames[1].str, "NULL_R_TIBUSA");
    strcpy(arrTibusaNames[2].str, "NULL_L_KETU");
    strcpy(arrTibusaNames[3].str, "NULL_R_KETU");
    strcpy(arrTibusaNames[4].str, "_r_hiza_L");
    strcpy(arrTibusaNames[5].str, "NULL_NA00_A_TARGET");

    memset(arrNakaSearchNames, 0, sizeof(arrNakaSearchNames));
    strcpy(arrNakaSearchNames[0].str, "N00_L_NAKA01");
    strcpy(arrNakaSearchNames[1].str, "N00_R_NAKA01");
    strcpy(arrNakaSearchNames[2].str, "NK00_L_NAKA01");
    strcpy(arrNakaSearchNames[3].str, "NK00_R_NAKA01");
    strcpy(arrNakaSearchNames[4].str, "NH00_L_NAKA01");
    strcpy(arrNakaSearchNames[5].str, "NA00_R_NAKA01");

    // --- per group x effect drag speeds (rows 5..9 stay zero)
    memset(arrEffectSpeeds, 0, sizeof(arrEffectSpeeds));
    arrEffectSpeeds[0][0] = 0.1f;    arrEffectSpeeds[0][1] = 0.1f;
    arrEffectSpeeds[0][2] = 0.05f;   arrEffectSpeeds[0][3] = 0.05f;
    arrEffectSpeeds[0][4] = 0.05f;   arrEffectSpeeds[0][5] = 0.05f;
    arrEffectSpeeds[1][0] = 0.13f;   arrEffectSpeeds[1][1] = 0.13f;
    arrEffectSpeeds[1][2] = 0.05f;   arrEffectSpeeds[1][3] = 0.05f;
    arrEffectSpeeds[1][4] = 0.05f;   arrEffectSpeeds[1][5] = 0.05f;
    arrEffectSpeeds[2][0] = 0.13f;   arrEffectSpeeds[2][1] = 0.13f;
    arrEffectSpeeds[2][2] = 0.05f;   arrEffectSpeeds[2][3] = 0.05f;
    arrEffectSpeeds[2][4] = 0.05f;   arrEffectSpeeds[2][5] = 0.05f;
    arrEffectSpeeds[3][0] = 0.15f;   arrEffectSpeeds[3][1] = 0.15f;
    arrEffectSpeeds[3][2] = 0.05f;   arrEffectSpeeds[3][3] = 0.05f;
    arrEffectSpeeds[3][4] = 0.05f;   arrEffectSpeeds[3][5] = 0.05f;
    arrEffectSpeeds[4][0] = 0.12f;   arrEffectSpeeds[4][1] = 0.12f;
    arrEffectSpeeds[4][2] = 0.05f;   arrEffectSpeeds[4][3] = 0.05f;
    arrEffectSpeeds[4][4] = 0.05f;   arrEffectSpeeds[4][5] = 0.05f;

    // --- setup
    fAnimTime = GetSekBodyCurrentAnimTime(pHeroBody);  // pHeroBody is still
    pTargetFrame = pLockOnBody->pSekHeader_220->pFrameT3EC;  // NULL here (sic)
    dirUpY[1] = 0.0f;
    FrameTimer_BeginFrame();
    memset(arrAccumulators, 0, sizeof(arrAccumulators));
    memset(arrVelocities, 0, sizeof(arrVelocities));
    PauseSceneAudio_46A2A0(pScene, 1);
    DrawFadeTransition_Scene(0, pScene, arrMouseSprites, arrGOTypeSprites);
    arrTearsBodies[0]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
    arrTearsBodies[1]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
    arrTearsBodies[2]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
    memset(arrFrames, 0, sizeof(arrFrames));
    memset(arrKutiRFrames, 0, sizeof(arrKutiRFrames));
    memset(arrAnimSets1, 0, sizeof(arrAnimSets1));
    memset(arrAnimSets2, 0, sizeof(arrAnimSets2));
    GetMouseXY_45A4D8((DWORD *)&xDragStart, (DWORD *)&yDragStart);
    xMouse = xDragStart;
    yMouse = yDragStart;
    SetAnimSetTime(pModeBody->pSekHeader_220->pAnimSetT3F1, fAnimTime);
    if ( g_pds3DListener )
        pFrameKutiFu = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC, "_kuti_fu1");
    for ( k = 0; k < 6; ++k )
    {
        CHanaObj__NewAnimSetT3F1_S150(&arrAnimSets1[k]);
        CHanaObj__NewAnimSetT3F1_S150(&arrAnimSets2[k]);
        wsprintfA(szObjName, "CutAnimations_1887_%d_%d", k, rand());
        AttachHanaObj_40BAB8(arrAnimSets1[k], szObjName);
        wsprintfA(szObjName, "CutAnimations_6997_%d_%d", k, rand());
        AttachHanaObj_40BAB8(arrAnimSets2[k], szObjName);

        arrFrames[k] = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC,
                                              arrCutFrameNames[k][0].str);
        if ( arrFrames[k] )
        {
            arrFrames[k]->bAlphaDeferred_130 = 1;
            AnimSet_ResetAndTransferAnimations(pModeBody->pSekHeader_220->pAnimSetT3F1,
                                               arrAnimSets1[k], arrFrames[k]);
            Frame_ApplyFadeRecursive(arrFrames[k], effectAlphaMuls[g_nHSceneMale_530265]);
            arrNakaFrames[k] = FindFrameByNameSubstringRecursive(arrFrames[k],
                                                                 arrNakaSearchNames[k].str);
        }
        arrKutiRFrames[k] = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC,
                                                   arrCutFrameNames[k][1].str);
        arrTibusaLFrames[k] = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC,
                                                     arrTibusaNames[k].str);
        if ( arrKutiRFrames[k] )
            AnimSet_ResetAndTransferAnimations(pModeBody->pSekHeader_220->pAnimSetT3F1,
                                               arrAnimSets2[k], arrKutiRFrames[k]);
    }
    pMapFrame = Scene_FindMapFrame(pScene);
    iHeroIdx = SetFrameVisibilityAndGetHIndex(pScene, 1);
    if ( iHeroIdx >= 0 )
    {
        pCameraFrame = pScene->refsFrames_134[iHeroIdx];
        pHeroBody = pScene->refsSekBody_13C[iHeroIdx];
        CSekBody__CutToAnim_4018E1(pModeBody, pHeroBody->field_200);
        pModeBody->float_21C = 0.0f;
        pModeBody->float_224 = 0.0f;
        CSekBody__UpdateAnim_4019E1(pModeBody, 1.0f);
        pHeroBody->float_21C = 0.0f;
        pHeroBody->float_224 = 0.0f;
        SetAnimSetTime(pModeBody->pSekHeader_220->pAnimSetT3F1, fAnimTime);
        MtxStackStartMultLocal_4074EF();
    }
    for ( k = 0; k < 30; ++k )
        arrSprites[k] = arrPostureSprites[k];
    AnimSet_MatchAndSortKutiRTibusaLFrames(arrAnimSets1, arrFrames, arrNakaFrames,
        arrAnimSets2, arrKutiRFrames, arrTibusaLFrames, pModeBody,
        arrEffectSpeeds[iGroup], arrSprites, fAnimTime);
    if ( pCameraFrame )
    {
        if ( pHeroBody )
        {
            CSekBody__CutToAnim_4018E1(pModeBody, pHeroBody->field_200);
            CSekBody__UpdateAnim_4019E1(pModeBody, 1.0f);
            MtxStackStartMultLocal_4074EF();
        }
        HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fRelX, &fRelY, &fRelZ);
        CalculateLocalRotationMatrix(pCameraFrame, (CHanaFrameT3EC_S354*)g_pSceneFrame, dirForwardX,
            &dirForwardY, &dirForwardZ, &dirUpX, dirUpY, &dirUpZ);
        SetFramePosRel_4069DD(pModeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
                       fRelX, fRelY, fRelZ);
        UpdateBoneLookAtTransform(pModeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
            dirForwardX[0], dirForwardY, dirForwardZ, dirUpX, dirUpY[0], dirUpZ);
        IsObjectValidRecursive_467DFD(pCameraFrame, pModeBody->pSekHeader_220->pFrameT3EC);
        pModeBody->float_21C = 1.0f;
        pModeBody->float_224 = 1.0f;
    }
    pModeBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
    pFrameAtariSita = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC, "atari_sita");
    pFrameAtariUe = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC, "atari_ue");
    CameraFrame_ProjectFramePairOntoForwardRay(pTargetFrame, pFrameAtariUe, pTargetFrame);
    for ( k = 0; k < 3; ++k )
    {
        MtxStackStartMultLocal_4074EF();
        CameraController_UpdateCutscene(pTargetFrame, pMapFrame, 0, 0, 1);
        CameraController_UpdateCutscene(pTargetFrame, pMapFrame, 0, 0, 2);
        CameraController_UpdateCutscene(pTargetFrame, pMapFrame, 0, 0, 3);
        Scene_ProcessPracticeFrames(pScene);
    }
    arrTearsFrames[0] = FindFrameByName_47E999(pModeBody->pSekHeader_220->pFrameT3EC, "_eye_fu1");
    arrTearsFrames[1] = FindFrameByNameSubstringRecursive(pModeBody->pSekHeader_220->pFrameT3EC, "_l_momo");
    arrTearsFrames[2] = FindFrameByNameSubstringRecursive(pModeBody->pSekHeader_220->pFrameT3EC, "_r_momo");
    FrameTimer_BeginFrame();
    while ( 1 )
    {
        DoPeekMessages();
        frameTime = (float)GetFrameTime();
        if ( frameTime > 5.0f )
            frameTime = 5.0f;
        FrameTimer_EndFrame();

        // --- system menu
        if ( IsMenuKeyPressed_481170() )
        {
            PauseSceneAudio_46A2A0(pScene, 1);
            RestartInputThread(0);
            sceneScratch = g_sceneState_52F7E0;
            nMenuResult = iMenuTab
                ? SystemMenu_4655AB(g_pDSBuf_52EFD0, -1000, 2,
                      g_sceneState_52F7E0.protCurrentHP_C0,
                      g_sceneState_52F7E0.protMaximumHP_C4, sceneScratch)
                : SystemMenu_4655AB(g_pDSBuf_52EFD0, -1000, 0,
                      g_sceneState_52F7E0.protCurrentHP_C0,
                      g_sceneState_52F7E0.protMaximumHP_C4, sceneScratch);
            RestartInputThread(2);
            for ( k = 0; k < 10; ++k )
            {
                if ( arrFrames[k] )
                    Frame_ApplyFadeRecursive(arrFrames[k], effectAlphaMuls[g_nHSceneMale_530265]);
            }
            pMapFrame = Scene_FindMapFrame(pScene);
            PauseSceneAudio_46A2A0(pScene, 0);
            if ( nMenuResult >= 1 )
                break;
        }

        // --- mouse state
        btnMousePrev = btnMouse;
        GetMouseBtn_45A41B(&btnMouse);
        GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);
        if ( !mousePressed && btnMouse == 1 && btnMousePrev != btnMouse )
        {
            for ( k = 0; k < 2; ++k )
            {
                if ( ConvertFrom640AsInt(576) < xMouse
                  && ConvertFrom640AsInt(48 * k) < yMouse
                  && yMouse < ConvertFrom640AsInt(48 * (k + 1) - 16) )
                {
                    nBreakCode = k + 1;
                }
            }
        }
        if ( btnMouse && btnMousePrev != btnMouse )
        {
            xDragStart = xMouse;
            yDragStart = yMouse;
        }
        if ( !btnMouse && btnMousePrev )
        {
            mousePressed = 0;
            nLastEffectIdx = nCurrentEffectIdx;
            nCurrentEffectIdx = -1;
        }
        if ( !btnMouse && btnMousePrev && !mousePressed )
            Cursor_ClampToCenterOrSprite(1, &xMouse, &yMouse, &xGrab, &yGrab,
                                         arrMouseSprites[iCursorSlot]);
        if ( arrMouseSprites[iCursorSlot] )
        {
            if ( btnMouse || mousePressed )
            {
                int tmpX = xGrab;
                int tmpY = yGrab;
                if ( btnMouse == btnMousePrev )
                {
                    Cursor_ClampToCenterOrSprite(0, &xMouse, &yMouse, &xGrab, &yGrab,
                                                 arrMouseSprites[iCursorSlot]);
                    xDragStart -= tmpX - xGrab;
                    yDragStart -= tmpY - yGrab;
                }
            }
            else
            {
                for ( k = 0; k < 10; ++k )
                {
                    if ( arrMouseSprites[k] )
                    {
                        sceneScratch.fPosY_18C = (float)yMouse;
                        sceneScratch.fPosX_188 = (float)xMouse;
                        CHanaSpriteT3F6__SetPos_414A5D(arrMouseSprites[k],
                            sceneScratch.fPosX_188, sceneScratch.fPosY_18C);
                    }
                }
            }
        }

        // --- camera-cutscene alpha / intensity ramp
        if ( btnMouse == 3 || btnMouse == 2 )
        {
            if ( mousePressed )
            {
                fadeAlpha = fadeAlpha + frameTime;
                if ( fadeAlpha < 15.0f )
                {
                    if ( nCurrentEffectIdx >= 0 )
                        effectIntensity = frameTime / 15.0f + effectIntensity;
                }
                else
                {
                    fadeAlpha = 15.0f;
                }
            }
        }
        else
        {
            fadeAlpha = fadeAlpha - frameTime;
            if ( fadeAlpha > 1.0f )
            {
                if ( nCurrentEffectIdx >= 0 && mousePressed )
                    effectIntensity = frameTime / 15.0f + effectIntensity;
            }
            else
            {
                fadeAlpha = 1.0f;
            }
        }

        // --- hover picking of the kuti-R frames
        Frame_CheckCameraDistanceAndRecurse(pModeBody->pSekHeader_220->pFrameT3EC);
        HanaFrame_GetRelPosition(g_pCameraFrame, pMapFrame, &fRelX, &fRelY, &fRelZ);
        nSelectedEffectIdx = -1;
        closestEffectDist = 9999999.0f;
        fFrameTimeScaled = frameTime;
        if ( !mousePressed && xMouse > 10 && ConvertFrom640AsInt(576) > xMouse
          && yMouse > 10 && ConvertFrom640AsInt(470) > yMouse )
        {
            nCurrentEffectIdx = -1;
            MtxStackStartMultLocal_4074EF();
            for ( k = 0; k < 10; ++k )
            {
                if ( arrKutiRFrames[k] )
                {
                    ProjectToScreen_40E2A5(&arrKutiRFrames[k]->matWorld_178, &ptProj);
                    HanaFrame_GetCenterPosition_408C3E(arrKutiRFrames[k], g_pCameraFrame,
                                                       &fRelX, &fRelY, &fRelZ);
                    if ( fRelZ > 1.0f && !mousePressed )
                    {
                        nDistSq = (ptProj.y - yMouse) * (ptProj.y - yMouse)
                                + (ptProj.x - xMouse) * (ptProj.x - xMouse);
                        fRadius = (float)sqrt((double)nDistSq);
                        sceneScratch.fPosY_18C = 320.0f / fRelZ;
                        dPickRadius = ConvertFrom640AsFloat(sceneScratch.fPosY_18C);
                        if ( dPickRadius > fRadius )
                        {
                            if ( closestEffectDist == 9999999.0f )
                                fFrameTimeScaled = fFrameTimeScaled * 0.0099999998f;
                            if ( closestEffectDist >= (double)fRelZ )
                            {
                                nSelectedEffectIdx = k;
                                closestEffectDist = fRelZ;
                            }
                        }
                    }
                }
            }
            if ( nSelectedEffectIdx != -1 && (btnMouse == 1 || btnMouse == 3)
              && !btnMousePrev )
            {
                mousePressed = 1;
                nCurrentEffectIdx = nSelectedEffectIdx;
                nLastEffectIdx = -1;
                xGrab = xMouse;
                yGrab = yMouse;
            }
        }
        if ( bExitRequested == 1 )
            break;

        // --- mode body animation
        sceneScratch.fPosY_18C = (effectIntensity + 1.0f) / 2.0f * fFrameTimeScaled;
        CSekBody__UpdateAnim_4019E1(pModeBody, sceneScratch.fPosY_18C);
        fBonePhase = fFrameTimeScaled * effectIntensity / 10.0f + fBonePhase;
        if ( fBonePhase > 54.0f )
            fBonePhase = 0.0f;
        UpdateSekBodyAnimationMatrix(pModeBody, fBonePhase);
        for ( k = 0; k < 10; ++k )
        {
            if ( arrAnimSets1[k] )
            {
                sceneScratch.fPosY_18C = GetSekBodyCurrentAnimTime(pModeBody);
                SetAnimSetTime(arrAnimSets1[k], sceneScratch.fPosY_18C);
            }
            if ( arrAnimSets2[k] )
            {
                sceneScratch.fPosY_18C = GetSekBodyCurrentAnimTime(pModeBody);
                SetAnimSetTime(arrAnimSets2[k], sceneScratch.fPosY_18C);
            }
        }

        // --- per-effect frame movement
        MtxStackStartMultLocal_4074EF();
        for ( k = 0; k < 10; ++k )
        {
            double dVelX, dVelY;

            if ( nCurrentEffectIdx == k )
            {
                UpdateCutInSceneBoneTransforms(pModeBody, arrAnimSets1, arrFrames,
                    arrNakaFrames, arrAnimSets2, arrKutiRFrames, k, fadeAlpha);
                if ( MoveCutInSceneFramesWithCameraConstraint(arrFrames[k],
                        arrKutiRFrames[k], xMouse - xGrab, yMouse - yGrab,
                        arrEffectSpeeds[iGroup][k]) == 1 )
                {
                    nDistSq = (yMouse - yGrab) * (yMouse - yGrab)
                            + (xMouse - xGrab) * (xMouse - xGrab);
                    dDist = sqrt((double)nDistSq);
                    xMouse = xGrab + (int)((double)(xMouse - xGrab) / dDist
                                * arrEffectSpeeds[iGroup][k] * 500.0);
                    yMouse = yGrab + (int)((double)(yMouse - yGrab) / dDist
                                * arrEffectSpeeds[iGroup][k] * 500.0);
                    SetCursorPos(xMouse, yMouse);
                }
                if ( arrEffectSpeeds[iGroup][k] > 0.0f )
                {
                    dVelX = (double)(xMouse - xGrab) - arrVelocities[k][0];
                    dVelY = (double)(yMouse - yGrab) - arrVelocities[k][1];
                    dDist = sqrt(dVelX * dVelX + dVelY * dVelY);
                    effectIntensity = dDist / arrEffectSpeeds[iGroup][k]
                                    * frameTime / 5000.0 + effectIntensity;
                }
                arrVelocities[k][0] = (float)(xMouse - xGrab);
                arrVelocities[k][1] = (float)(yMouse - yGrab);
                arrAccumulators[k][0] = 0.0f;
                arrAccumulators[k][1] = 0.0f;
                nFrameSettled[k] = 0;
            }
            else if ( arrFrames[k] && arrAnimSets2[k] && arrKutiRFrames[k] )
            {
                if ( nLastEffectIdx == k )
                    UpdateCutInSceneBoneTransforms(pModeBody, arrAnimSets1, arrFrames,
                        arrNakaFrames, arrAnimSets2, arrKutiRFrames, k, fadeAlpha);
                else
                    UpdateCutInSceneBoneTransforms(pModeBody, arrAnimSets1, arrFrames,
                        arrNakaFrames, arrAnimSets2, arrKutiRFrames, k, 1.0f);
                UpdateCutInSceneCameraFollow(pModeBody, arrAnimSets1[k], arrFrames[k],
                    arrAnimSets2[k], arrKutiRFrames[k], arrEffectSpeeds[iGroup][k],
                    fadeAlpha, frameTime, &arrVelocities[k][0], &arrAccumulators[k][0]);
                if ( sqrt(arrVelocities[k][0] * arrVelocities[k][0]
                        + arrVelocities[k][1] * arrVelocities[k][1]) < 0.1000000014901161 )
                    nFrameSettled[k] = 1;
            }
        }

        // --- per-effect fades
        for ( k = 0; k < 10; ++k )
        {
            if ( arrFrames[k] )
            {
                if ( nCurrentEffectIdx == k )
                {
                    if ( arrFadeProg[k] < 1.0f )
                    {
                        arrFadeProg[k] = frameTime / 10.0f + arrFadeProg[k];
                        if ( arrFadeProg[k] > 1.0f )
                            arrFadeProg[k] = 1.0f;
                        if ( arrFadeProg[k] < 0.0f )
                            arrFadeProg[k] = 0.0f;
                        sceneScratch.fPosY_18C = effectAlphaMuls[g_nHSceneMale_530265]
                                               * arrFadeProg[k];
                        Frame_ApplyFadeRecursive(arrFrames[k], sceneScratch.fPosY_18C);
                        arrFrames[k]->bAlphaDeferred_130 = 0;
                    }
                }
                else if ( nSelectedEffectIdx != k || mousePressed || btnMouse
                       || nFrameSettled[k] != 1 )
                {
                    if ( arrFadeProg[k] < 0.0f )
                        arrFadeProg[k] = 0.0f;
                    sceneScratch.fPosY_18C = effectAlphaMuls[g_nHSceneMale_530265]
                                           * arrFadeProg[k];
                    Frame_ApplyFadeRecursive(arrFrames[k], sceneScratch.fPosY_18C);
                    if ( arrFadeProg[k] <= 0.0f )
                        arrFrames[k]->bAlphaDeferred_130 = 1;
                    else
                        arrFadeProg[k] = arrFadeProg[k] - frameTime / 10.0f;
                }
                else if ( arrFadeProg[k] != 0.5f )
                {
                    arrFadeProg[k] = frameTime / 20.0f + arrFadeProg[k];
                    if ( arrFadeProg[k] > 0.5f )
                        arrFadeProg[k] = 0.5f;
                    if ( arrFadeProg[k] < 0.0f )
                        arrFadeProg[k] = 0.0f;
                    Frame_ApplyFadeRecursive(arrFrames[k], arrFadeProg[k]);
                    arrFrames[k]->bAlphaDeferred_130 = 0;
                }
            }
        }

        // --- camera cutscene / GO-strip exits
        if ( !mousePressed )
        {
            if ( nBreakCode )
            {
                if ( nBreakCode == 1 )
                    break;
                if ( nBreakCode == 2 )
                {
                    iCursorSlot = 0;
                    nBreakCode = 0;
                    CutInScene_CyclePosAndApplyCameraOffset(pScene, arrCyclePos, pTargetFrame);
                    Scene_ProcessPracticeFrames(pScene);
                    CameraFrame_ProjectFramePairOntoForwardRay(pTargetFrame,
                                                               pFrameAtariUe, pFrameAtariSita);
                    CameraFrame_TrackBetweenObjectsAndLookAt(pTargetFrame,
                                                             pFrameAtariUe, pFrameAtariSita);
                    sceneScratch.fPosY_18C = 0.0f;
                    CameraController_UpdateCutscene(pTargetFrame, pMapFrame, 0, 0, 2);
                }
            }
            else
            {
                pTargetFrame->bAlphaDeferred_130 = 1;
                if ( btnMouse == btnMousePrev )
                {
                    if ( btnMouse == 1 )
                    {
                        sceneScratch.fPosY_18C = frameTime;
                        CameraController_UpdateCutscene(pTargetFrame, pMapFrame,
                            xMouse - xDragStart, yMouse - yDragStart, 1);
                        iCursorSlot = 1;
                        pTargetFrame->bAlphaDeferred_130 = 0;
                    }
                    if ( btnMouse == 2 )
                    {
                        sceneScratch.fPosY_18C = frameTime;
                        CameraController_UpdateCutscene(pTargetFrame, pMapFrame,
                            xMouse - xDragStart, yMouse - yDragStart, 2);
                        iCursorSlot = 2;
                        pTargetFrame->bAlphaDeferred_130 = 0;
                    }
                    if ( btnMouse == 3 )
                    {
                        sceneScratch.fPosY_18C = frameTime;
                        CameraController_UpdateCutscene(pTargetFrame, pMapFrame,
                            xMouse - xDragStart, yMouse - yDragStart, 3);
                        iCursorSlot = 3;
                        pTargetFrame->bAlphaDeferred_130 = 0;
                    }
                    Scene_ProcessPracticeFrames(pScene);
                    CameraFrame_TrackBetweenObjectsAndLookAt(pTargetFrame,
                                                             pFrameAtariUe, pFrameAtariSita);
                }
            }
        }

        // --- sounds + tears bodies
        MtxStackStartMultLocal_4074EF();
        sceneScratch.fPosX_188 = (float)mousePressed;
        SoundAH_ManageEffectsAndAnimate(arrSoundAHs, pFrameKutiFu,
            &effectIntensity, sceneScratch.fPosX_188, frameTime);
        *(int *)&sceneScratch.fPosY_18C = nCurrentEffectIdx;
        *(int *)&sceneScratch.fPosX_188 = mousePressed;
        *(float *)&sceneScratch.lpDDSurface_184 = frameTime;
        sceneScratch.frameTimeMultiplied_180 = effectIntensity - 1.0f;
        SoundSE_ManageEffectsAndPlay(arrSoundSEs,
            (int)arrNakaFrames[nCurrentEffectIdx],  // index -1 while idle (sic)
            sceneScratch.frameTimeMultiplied_180, frameTime, mousePressed,
            nCurrentEffectIdx);
        sceneScratch.fPosX_188 = (float)mousePressed + effectIntensity;
        Character_UpdateAnimationState(pModeBody, sceneScratch.fPosX_188, dirFwdVec);

        if ( CSekBody__UpdateAnim_4019E1(arrTearsBodies[0], frameTime) == 1
          && arrTearsFrames[0]->pAttachedObj_20C->dwType == 1111 )
        {
            pMorpT457 = arrTearsFrames[0]->pAttachedObj_20C;
            if ( pMorpT457->nTimeState_36C == 2 )
            {
                CalculateLocalRotationMatrix(arrTearsFrames[0], (CHanaFrameT3EC_S354*)g_pSceneFrame,
                    dirForwardX, &dirForwardY, &dirForwardZ, &dirUpX, dirUpY, &dirUpZ);
                if ( dirUpZ >= 0.0f )
                {
                    if ( arrTearsBodies[0]->field_200 % 2 )
                        SekBody__UpdateTo_4014E4(arrTearsBodies[0], 2);
                    else
                        SekBody__UpdateTo_4014E4(arrTearsBodies[0], 3);
                }
                else if ( arrTearsBodies[0]->field_200 % 2 )
                {
                    SekBody__UpdateTo_4014E4(arrTearsBodies[0], 0);
                }
                else
                {
                    SekBody__UpdateTo_4014E4(arrTearsBodies[0], 1);
                }
            }
        }
        if ( arrTearsBodies[0]->field_200 == 3 )
        {
            UpdateBoneLookAtTransform(arrTearsBodies[0]->pSekHeader_220->pFrameT3EC,
                arrTearsFrames[0], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            HanaFrame_GetRelPosition(arrTearsBodies[0]->pSekHeader_220->pFrameT3EC,
                g_pSceneFrame, dirForwardX, &dirForwardY, &dirForwardZ);
            sceneScratch.fPosY_18C = dirForwardZ;
            sceneScratch.fPosX_188 = dirForwardY - frameTime / 5.0f;
            SetFramePosRel_4069DD(arrTearsBodies[0]->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
                dirForwardX[0], sceneScratch.fPosX_188, dirForwardZ);
        }
        else
        {
            UpdateBoneLookAtTransform(arrTearsBodies[0]->pSekHeader_220->pFrameT3EC,
                arrTearsFrames[0], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            SetFramePosRel_4069DD(arrTearsBodies[0]->pSekHeader_220->pFrameT3EC,
                arrTearsFrames[0], 0.0f, 0.0f, 0.0f);
        }
        for ( k = 1; k <= 2; ++k )
        {
            if ( arrTearsFrames[k] )
            {
                UpdateBoneLookAtTransform(arrTearsBodies[k]->pSekHeader_220->pFrameT3EC,
                    arrTearsFrames[k], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
                SetFramePosRel_4069DD(arrTearsBodies[k]->pSekHeader_220->pFrameT3EC,
                    arrTearsFrames[k], 0.0f, 0.0f, 0.0f);
            }
            CalculateLocalRotationMatrix(arrTearsFrames[k], (CHanaFrameT3EC_S354*)g_pSceneFrame,
                dirForwardX, &dirForwardY, &dirForwardZ, &dirUpX, dirUpY, &dirUpZ);
            CSekBody__UpdateAnim_4019E1(arrTearsBodies[k], frameTime);
            if ( dirUpZ > 0.0f && nCurrentEffectIdx == 5 && effectIntensity > 1.0f )
                SekBody__UpdateTo_4014E4(arrTearsBodies[k], 0);
        }

        // --- draw
        MtxStackStartMultLocal_4074EF();
        g_pRenderCtx->Clear(3);
        if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
            HanaFrame_Draw((CHanaFrameT3EC_S354*)g_pSceneFrame);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            HanaVert_DrawBatch(1);
            g_pD3DDevice_4D6C78->EndScene();
        }
        if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
            if ( !mousePressed && pTargetFrame->bAlphaDeferred_130 == 1 )
            {
                for ( k = 0; k < 2; ++k )
                {
                    if ( arrGOTypeSprites[k] )
                    {
                        sceneScratch.fPosY_18C = ConvertFrom640AsInt(576) >= xMouse
                            ? arrGOTypeSprites[k]->fAlpha_144 - frameTime / 10.0f
                            : frameTime / 10.0f + arrGOTypeSprites[k]->fAlpha_144;
                        CHanaObj__SetAlpha3F6(arrGOTypeSprites[k], sceneScratch.fPosY_18C);
                        if ( arrGOTypeSprites[k]->fAlpha_144 > 0.0f )
                        {
                            posY = 48 * (k + 1) - 32;
                            sceneScratch.fPosY_18C = (float)posY;
                            sceneScratch.fPosY_18C = (float)ConvertFrom640AsFloat(sceneScratch.fPosY_18C);
                            sceneScratch.fPosX_188 = (float)ConvertFrom640AsFloat(608.0f);
                            CHanaSpriteT3F6__SetPos_414A5D(arrGOTypeSprites[k],
                                sceneScratch.fPosX_188, sceneScratch.fPosY_18C);
                            DrawSprite2D(arrGOTypeSprites[k]);
                        }
                    }
                }
                if ( nSelectedEffectIdx >= 0 && arrSprites[nSelectedEffectIdx] )
                {
                    sceneScratch.fPosY_18C = (float)yMouse;
                    sceneScratch.fPosX_188 = (float)xMouse;
                    CHanaSpriteT3F6__SetPos_414A5D(arrSprites[nSelectedEffectIdx],
                        sceneScratch.fPosX_188, sceneScratch.fPosY_18C);
                    if ( ConvertFrom640AsFloat(576.0f) < arrSprites[nSelectedEffectIdx]->destWHZ_138.x )
                        arrSprites[nSelectedEffectIdx]->destWHZ_138.x = ConvertFrom640AsFloat(576.0f);
                    if ( ConvertFrom640AsFloat(64.0f) > arrSprites[nSelectedEffectIdx]->destWHZ_138.x )
                        arrSprites[nSelectedEffectIdx]->destWHZ_138.x = ConvertFrom640AsFloat(64.0f);
                    if ( ConvertFrom640AsFloat(448.0f) < arrSprites[nSelectedEffectIdx]->destWHZ_138.y )
                        arrSprites[nSelectedEffectIdx]->destWHZ_138.y = ConvertFrom640AsFloat(448.0f);
                    if ( ConvertFrom640AsFloat(32.0f) > arrSprites[nSelectedEffectIdx]->destWHZ_138.y )
                        arrSprites[nSelectedEffectIdx]->destWHZ_138.y = ConvertFrom640AsFloat(32.0f);
                    DrawSprite2D(arrSprites[nSelectedEffectIdx]);
                }
                else if ( arrMouseSprites[iCursorSlot] )
                {
                    DrawSprite2D(arrMouseSprites[iCursorSlot]);
                }
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        // nop_4863C3();                                   // disabled, as elsewhere
        PresentFrame();
        HanaVert_DrawBatch(0);
        xDragStart = xMouse;
        yDragStart = yMouse;
    }

    // --- exit / cleanup
    SetFrameVisibilityAndGetHIndex(pScene, 0);
    pModeBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    for ( k = 0; k < 6; ++k )
    {
        AnimSet_ResetAndTransferAnimations(arrAnimSets1[k],
            pModeBody->pSekHeader_220->pAnimSetT3F1, arrFrames[k]);
        AnimSet_ResetAndTransferAnimations(arrAnimSets2[k],
            pModeBody->pSekHeader_220->pAnimSetT3F1, arrKutiRFrames[k]);
    }
    for ( k = 0; k < 10; ++k )
    {
        if ( arrAnimSets1[k] )
        {
            CHanaObj__Free_40C1E4(arrAnimSets1[k]);
            arrAnimSets1[k] = 0;
        }
        if ( arrAnimSets2[k] )
        {
            CHanaObj__Free_40C1E4(arrAnimSets2[k]);
            arrAnimSets2[k] = 0;
        }
    }
    if ( pHeroBody )
    {
        CSekBody__UpdateAnim_4019E1(pModeBody, frameTime);
        HanaFrame_GetRelPosition(pModeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame,
                                 &fRelX, &fRelY, &fRelZ);
        CalculateLocalRotationMatrix(pModeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
            dirForwardX, &dirForwardY, &dirForwardZ, &dirUpX, dirUpY, &dirUpZ);
        SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
                       fRelX, fRelY, fRelZ);
        UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354*)g_pSceneFrame,
            dirForwardX[0], dirForwardY, dirForwardZ, dirUpX, dirUpY[0], dirUpZ);
        IsObjectValidRecursive_467DFD(pModeBody->pSekHeader_220->pFrameT3EC,
                                      pHeroBody->pSekHeader_220->pFrameT3EC);
        pHeroBody->float_21C = 1.0f;
        pHeroBody->float_224 = 1.0f;
        CSekBody__UpdateAnim_4019E1(pHeroBody, frameTime);
    }
    for ( k = 0; k < 30; ++k )
    {
        if ( arrSoundAHs[k] )
            arrSoundAHs[k]->pDSBuffer_0->Stop();
        if ( arrSoundSEs[k] )
            arrSoundSEs[k]->pDSBuffer_0->Stop();
    }
    if ( nMenuResult < 1 )
        DrawFadeTransition_Scene(1, pScene, arrMouseSprites, arrGOTypeSprites);
    PauseSceneAudio_46A2A0(pScene, 0);
    arrTearsBodies[0]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    arrTearsBodies[1]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    arrTearsBodies[2]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    return nMenuResult;
}

bool __cdecl hook_SyncObjectAnimationByName_467BBD(CHanaFrameT3EC_S354 *pSourceFrame, CHanaFrameT3EC_S354 *pTargetFrame)
{
  CHanaAnimT3F0_S240 *pSourceAnim; // [esp+0h] [ebp-1Ch]
  CHanaAnimT3F0_S240 *pTargetAnim; // [esp+4h] [ebp-18h]
  int targetUnderscorePos; // [esp+8h] [ebp-14h]
  int sourceUnderscorePos; // [esp+Ch] [ebp-10h]
  int i; // [esp+10h] [ebp-Ch]
  int j; // [esp+10h] [ebp-Ch]
  int targetNameLen; // [esp+14h] [ebp-8h]
  int sourceNameLen; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] SyncObjectAnimationByName_467BBD\n");


  sourceUnderscorePos = 0;
  targetUnderscorePos = 0;
  sourceNameLen = lstrlenA(pSourceFrame->szName_8);
  targetNameLen = lstrlenA(pTargetFrame->szName_8);
  for ( i = 0; i < sourceNameLen; ++i )
  {
    if ( pSourceFrame->szName_8[i] == '_' )
    {
      sourceUnderscorePos = i;
      break;
    }
  }
  for ( j = 0; j < targetNameLen; ++j )
  {
    if ( pTargetFrame->szName_8[j] == '_' )
    {
      targetUnderscorePos = j;
      break;
    }
  }
  if ( sourceNameLen - sourceUnderscorePos != targetNameLen - targetUnderscorePos
    || _memicmp(
      &pSourceFrame->szName_8[sourceUnderscorePos],
      &pTargetFrame->szName_8[targetUnderscorePos],
      sourceNameLen - sourceUnderscorePos) )
  {
    return 0;
  }
  pSourceAnim = pSourceFrame->pHanaAnim_2C0;
  pTargetAnim = pTargetFrame->pHanaAnim_2C0;
  if ( pSourceAnim )
  {
    if ( pTargetAnim )
    {
      pTargetAnim->keyCur_14C.quatRot.x = pSourceAnim->keyCur_14C.quatRot.x;
      pTargetAnim->keyCur_14C.quatRot.y = pSourceAnim->keyCur_14C.quatRot.y;
      pTargetAnim->keyCur_14C.quatRot.z = pSourceAnim->keyCur_14C.quatRot.z;
      pTargetAnim->keyCur_14C.quatRot.w = pSourceAnim->keyCur_14C.quatRot.w;
      pTargetAnim->keyCur_14C.fPosX = pSourceAnim->keyCur_14C.fPosX;
      pTargetAnim->keyCur_14C.fPosY = pSourceAnim->keyCur_14C.fPosY;
      pTargetAnim->keyCur_14C.fPosZ = pSourceAnim->keyCur_14C.fPosZ;
      pTargetAnim->keyCur_14C.fScaleX = pSourceAnim->keyCur_14C.fScaleX;
      pTargetAnim->keyCur_14C.fScaleY = pSourceAnim->keyCur_14C.fScaleY;
      pTargetAnim->keyCur_14C.fScaleZ = pSourceAnim->keyCur_14C.fScaleZ;
    }
  }
  return 1;
}

bool __cdecl hook_IsParentValid_467D62(CHanaFrameT3EC_S354 *pParent, CHanaFrameT3EC_S354 *pCurrent)
{
  CHanaNode *pFirstChildNode; // [esp+4h] [ebp-Ch]
  unsigned int i; // [esp+8h] [ebp-8h]
  bool bNotValid; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] IsParentValid_467D62\n");

  bNotValid = 0;
  if ( SyncObjectAnimationByName_467BBD(pParent, pCurrent) )
    return 1;
  if ( pParent->nCountChild )
  {
    pFirstChildNode = pParent->pFirstChildNode;
    for ( i = 0; i < pParent->nCountChild; ++i )
    {
      if ( IsParentValid_467D62(pFirstChildNode->pData, pCurrent) )
        return 1;
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
  return bNotValid;
}

void __cdecl hook_IsObjectValidRecursive_467DFD(CHanaFrameT3EC_S354 *parentObj, CHanaFrameT3EC_S354 *currentObj)
{
  CHanaNode *pFirstChildNode; // [esp+4h] [ebp-8h]
  unsigned int i; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] IsObjectValidRecursive_467DFD\n");

  IsParentValid_467D62(parentObj, currentObj);
  if ( currentObj->nCountChild )
  {
    pFirstChildNode = currentObj->pFirstChildNode;
    for ( i = 0; i < currentObj->nCountChild; ++i )
    {
      IsObjectValidRecursive_467DFD(parentObj, pFirstChildNode->pData);
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
}

void __cdecl hook_SoundSE_ManageEffectsAndPlay(
  CSoundStruct_8 *ppSoundSEs[],
  int pFrame,
  float effectType,
  float fFrameTime,
  int bMousePressed,
  int nCurrentEffectIdx)
{
  D3DVALUE posX; // [esp+8h] [ebp-44h] BYREF
  int nLastPlayedIdx; // [esp+Ch] [ebp-40h]
  int i; // [esp+10h] [ebp-3Ch]
  D3DVALUE posZ; // [esp+14h] [ebp-38h] BYREF
  LONG nVolume; // [esp+18h] [ebp-34h]
  int nActiveSoundIdx; // [esp+1Ch] [ebp-30h]
  D3DVALUE posY; // [esp+20h] [ebp-2Ch] BYREF
  float fEffectTypeToSoundIdx[10]; // [esp+24h] [ebp-28h]
  float fSoundIdx; // [esp+5Ch] [ebp+10h]

  ONCELOG("[HNH:SUB] SoundSE_ManageEffectsAndPlay_4698E8\n");
  nActiveSoundIdx = -1;
  fEffectTypeToSoundIdx[0] = 0.0;
  fEffectTypeToSoundIdx[1] = 0.0;
  fEffectTypeToSoundIdx[2] = 1.0;
  fEffectTypeToSoundIdx[3] = 1.0;
  fEffectTypeToSoundIdx[4] = 2.0;
  fEffectTypeToSoundIdx[5] = 2.0;
  fEffectTypeToSoundIdx[6] = 2.0;
  fEffectTypeToSoundIdx[7] = 2.0;
  fEffectTypeToSoundIdx[8] = 2.0;
  fEffectTypeToSoundIdx[9] = 0.0;
  if ( effectType > 3.0 )
    effectType = 3.0;
  if ( bMousePressed && nCurrentEffectIdx == 5 && effectType >= 0.0 )
  {
    fSoundIdx = fEffectTypeToSoundIdx[(__int64)effectType];
    for ( i = 0; i < 9; ++i )
    {
      if ( ppSoundSEs[i] )
      {
        HanaFrame_GetCenterPosition_408C3E((CHanaFrameRootT3EC_S2D4 *)pFrame, g_pSceneFrame, &posX, &posY, &posZ);
        if ( ppSoundSEs[i]->pDSBuffer_0
          && !ppSoundSEs[i]->pDSBuffer_0->GetStatus(&g_dwDSStatus_5185C8)
          && (g_dwDSStatus_5185C8 & 1) != 0 )
        {
          ppSoundSEs[i]->pDS3DBuffer_4->SetPosition(posX, posY, posZ, 1u);
          nActiveSoundIdx = i;
        }
      }
    }
    if ( nActiveSoundIdx == -1 )
    {
      if ( fSoundIdx >= 0.0 )
      {
        nLastPlayedIdx = 3 * (__int64)fSoundIdx + rand() % 3;
        if ( ppSoundSEs[nLastPlayedIdx] )
        {
          ppSoundSEs[nLastPlayedIdx]->pDSBuffer_0->SetCurrentPosition(
            0);
          ppSoundSEs[nLastPlayedIdx]->pDSBuffer_0->Play(0, 0, 0);
        }
      }
    }
    else
    {
      if ( ppSoundSEs[nActiveSoundIdx] )
      {
        if ( (__int64)fSoundIdx == nActiveSoundIdx / 3 )
        {
          nVolume = (__int64)(fFrameTime * 300.0) + DSBufferGetVolume_423C65(ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0);
          if ( nVolume <= 0 )
          {
            if ( ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0 )
            {
              if ( nVolume >= -10000 )
                ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(
                  nVolume);
              else
                ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(
                  -10000);
            }
          }
          else if ( ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0 )
          {
            ppSoundSEs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(0);
          }
        }
        else
        {
          nActiveSoundIdx = -1;
        }
      }
      for ( i = 0; i < 30; ++i )
      {
        if ( ppSoundSEs[i] && nActiveSoundIdx != i )
        {
          nVolume = DSBufferGetVolume_423C65(ppSoundSEs[i]->pDSBuffer_0);
          if ( nVolume >= -1000 )
          {
            if ( ppSoundSEs[i]->pDSBuffer_0 )
            {
              if ( (int)(nVolume - (__int64)(fFrameTime * 300.0)) >= -10000 )
              {
                if ( (int)(nVolume - (__int64)(fFrameTime * 300.0)) <= 0 )
                  ppSoundSEs[i]->pDSBuffer_0->SetVolume(
                    nVolume - (__int64)(fFrameTime * 300.0));
                else
                  ppSoundSEs[i]->pDSBuffer_0->SetVolume(0);
              }
              else
              {
                ppSoundSEs[i]->pDSBuffer_0->SetVolume(-10000);
              }
            }
          }
          else
          {
            ppSoundSEs[i]->pDSBuffer_0->Stop();
          }
        }
      }
    }
  }
  else
  {
    for ( i = 0; i < 30; ++i )
    {
      if ( ppSoundSEs[i] )
      {
        nVolume = DSBufferGetVolume_423C65(ppSoundSEs[i]->pDSBuffer_0);
        if ( nVolume >= -1000 )
        {
          if ( ppSoundSEs[i]->pDSBuffer_0 )
          {
            if ( (int)(nVolume - (__int64)(fFrameTime * 300.0)) >= -10000 )
            {
              if ( (int)(nVolume - (__int64)(fFrameTime * 300.0)) <= 0 )
                ppSoundSEs[i]->pDSBuffer_0->SetVolume(
                  nVolume - (__int64)(fFrameTime * 300.0));
              else
                ppSoundSEs[i]->pDSBuffer_0->SetVolume(0);
            }
            else
            {
              ppSoundSEs[i]->pDSBuffer_0->SetVolume(-10000);
            }
          }
        }
        else
        {
          ppSoundSEs[i]->pDSBuffer_0->Stop();
        }
      }
    }
  }
}

// Matches "KutiR" frames to their closest "TibusaL" counterpart frames by
// comparing 3D center positions, then swaps array entries to align matching pairs. Used in cutscene setup to pair
// related animation frames.

void __cdecl hook_AnimSet_MatchAndSortKutiRTibusaLFrames(
  CHanaAnimSetT3F1_S150 **arrSrcAnimSets,
  CHanaFrameT3EC_S354 **arrTargetFrames,
  CHanaFrameT3EC_S354 **arrTargetFrameIdxes,
  CHanaAnimSetT3F1_S150 **arrDstAnimSets,
  CHanaFrameT3EC_S354 **arrKutiRFrames,
  CHanaFrameT3EC_S354 **arrTibusaLFrames,
  CSekBody *pModeBody,
  float *arrFloatParams,
  CHanaSpriteT3F6_S1FC **arrSprites,
  float animTime)
{
  int bestMatchIdx; // [esp+4h] [ebp-3Ch]
  float tibusaLPosY[2]; // [esp+8h] [ebp-38h] BYREF
  float pz; // [esp+10h] [ebp-30h] BYREF
  CHanaFrameT3EC_S354 *tempFrame; // [esp+14h] [ebp-2Ch]
  int pairIdx; // [esp+18h] [ebp-28h]
  float targetPosY; // [esp+1Ch] [ebp-24h] BYREF
  float distSq; // [esp+20h] [ebp-20h]
  CHanaAnimSetT3F1_S150 *tempAnimSet; // [esp+24h] [ebp-1Ch]
  int i; // [esp+28h] [ebp-18h]
  float py; // [esp+2Ch] [ebp-14h] BYREF
  float minDistSq; // [esp+30h] [ebp-10h]
  float tempFloat; // [esp+34h] [ebp-Ch]
  float targetPosX2; // [esp+38h] [ebp-8h] BYREF
  float px; // [esp+3Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] AnimSet_MatchAndSortKutiRTibusaLFrames_467E72\n");

  tempFrame = 0;
  tempAnimSet = 0;
  bestMatchIdx = -1;
  minDistSq = 9999999.0;
  tibusaLPosY[1] = 0.0;
  MtxStackStartMultLocal_4074EF();              // Initialize matrix stack
  for ( i = 0; i < 10; i += 2 )                 // Process 5 pairs (i = 0, 2, 4, 6, 8)
  {
    if ( arrDstAnimSets[i] && arrKutiRFrames[i] )// Skip if no KutiR anim set or frame at this even index
    {
      minDistSq = 9999999.0;                    // Odd index for target data
      pairIdx = i + 1;
      SetAnimSetTime(arrDstAnimSets[i], animTime);// Set animation time on KutiR's anim set
      if ( arrTargetFrames[pairIdx] )
      {
        if ( arrTargetFrameIdxes[pairIdx] )
        {
          if ( arrSrcAnimSets[pairIdx] )        // Check if target frame data exists at pairIdx
          {
            SetAnimSetTime(arrSrcAnimSets[pairIdx], animTime);// Set animation time on source anim set
            MtxStackStartMultLocal_4074EF();    // Push new matrix stack frame for position calculation
            HanaFrame_GetCenterPosition_408C3E( //  Get TibusaL frame center position (in scene space)
              arrTibusaLFrames[i],
              g_pSceneFrame,
              &px,
              &py,
              &pz);
            HanaFrame_GetCenterPosition_408C3E( // Get Target frame center position (in scene space)
              arrTargetFrameIdxes[pairIdx],
              g_pSceneFrame,
              &targetPosX2,
              &targetPosY,
              tibusaLPosY);
            distSq = (px - targetPosX2) * (px - targetPosX2)// Compute squared 3D distance between centers
              + (py - targetPosY) * (py - targetPosY)
              + (pz - tibusaLPosY[0]) * (pz - tibusaLPosY[0]);
            if ( distSq < (double)minDistSq )   // Track closest target frame
            {
              minDistSq = distSq;
              bestMatchIdx = pairIdx;
            }
          }
        }
      }
      if ( minDistSq != 9999999.0 )             // If a match was found AND it's not already at the correct position
      {
        minDistSq = 9999999.0;
        if ( i != bestMatchIdx )
        {
          tempFrame = arrKutiRFrames[i];        // Swap KutiR frames
          arrKutiRFrames[i] = arrKutiRFrames[bestMatchIdx];
          arrKutiRFrames[bestMatchIdx] = tempFrame;
          tempFrame = arrTibusaLFrames[i];      // Swap TibusaL frames
          arrTibusaLFrames[i] = arrTibusaLFrames[bestMatchIdx];
          arrTibusaLFrames[bestMatchIdx] = tempFrame;
          tempAnimSet = arrDstAnimSets[i];      // Swap destination anim sets
          arrDstAnimSets[i] = arrDstAnimSets[bestMatchIdx];
          arrDstAnimSets[bestMatchIdx] = tempAnimSet;
          tempFloat = arrFloatParams[i];        // Swap float params
          arrFloatParams[i] = arrFloatParams[bestMatchIdx];
          arrFloatParams[bestMatchIdx] = tempFloat;
        }
      }
    }
  }
}

double __cdecl hook_GetSekBodyCurrentAnimTime(CSekBody *pSekBody)
{
  ONCELOG("[HNH:SUB] GetSekBodyCurrentAnimTime_401C67\n");

  if ( !pSekBody || pSekBody->field_24C || !pSekBody->pSekHeader_220 || !pSekBody->pSekHeader_220->pAnimSetT3F1 )
    return 0.0;
  g_pSekItem_4CD760 = &pSekBody->items_250[pSekBody->field_200];
  return g_pSekItem_4CD760->float_60;
}

void __cdecl hook_SetAnimSetTime(CHanaAnimSetT3F1_S150 *pAnimSet, float fTimeScale)
{
  int animIdx; // [esp+0h] [ebp-4h]
  signed int i; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] SetAnimSetTime_404277\n");

  pAnimSet->field_130 = fTimeScale;             // Store time scale in anim set
  for ( animIdx = 0; animIdx < pAnimSet->nAnimCount_134; ++animIdx )// Apply to all regular animations (CHanaAnimT3F0_S240)
    InterpolateHanaAnimFrame(pAnimSet->refsAnimT3F0_138[animIdx], fTimeScale);
  for ( i = 0; i < (signed int)pAnimSet->nManmCount_140; ++i )
    ApplyMateAnimMaterial(pAnimSet->pRefsManmT3F7[i], fTimeScale);// Apply to all material animations (CHanaMateAnimT3F7_S184)
}
  
void __cdecl hook_UpdateCutInSceneCameraFollow(
  CSekBody *pModeBody,
  CHanaAnimSetT3F1_S150 *pAnimSetL,
  CHanaFrameT3EC_S354 *pFrame,
  CHanaAnimSetT3F1_S150 *pAnimSetR,
  CHanaFrameT3EC_S354 *pKutiRFrame,
  float fSpeed,
  float fFade,
  float fFrameTime,
  float *pfVelocity,
  float *pfAccumulator)
{
  float adjustedX; // [esp+0h] [ebp-74h]
  float X; // [esp+4h] [ebp-70h]
  float targetForwardX; // [esp+Ch] [ebp-68h] BYREF
  float targetRelToCamZ; // [esp+10h] [ebp-64h] BYREF
  float camForwardX; // [esp+14h] [ebp-60h] BYREF
  float targetRelPosZ; // [esp+18h] [ebp-5Ch] BYREF
  float pOutZ; // [esp+1Ch] [ebp-58h] BYREF
  float targetZXrot; // [esp+20h] [ebp-54h] BYREF
  float dirUpZ; // [esp+24h] [ebp-50h] BYREF
  float pOutY; // [esp+28h] [ebp-4Ch] BYREF
  float velMagnitudeRaw; // [esp+2Ch] [ebp-48h]
  float targetRelPosY; // [esp+30h] [ebp-44h] BYREF
  float velMagnitude; // [esp+34h] [ebp-40h]
  float camPosY; // [esp+38h] [ebp-3Ch] BYREF
  float targetYXrot; // [esp+3Ch] [ebp-38h] BYREF
  float normVelY; // [esp+40h] [ebp-34h]
  float dirUpY; // [esp+44h] [ebp-30h] BYREF
  float normVelX; // [esp+48h] [ebp-2Ch]
  float targetForwardZ; // [esp+4Ch] [ebp-28h] BYREF
  float camForwardZ; // [esp+50h] [ebp-24h] BYREF
  float targetRelToCamX; // [esp+54h] [ebp-20h] BYREF
  float targetRelPosX; // [esp+58h] [ebp-1Ch] BYREF
  float targetRotAngles[2]; // [esp+5Ch] [ebp-18h] BYREF
  float pOutX; // [esp+64h] [ebp-10h] BYREF
  float camUpX; // [esp+68h] [ebp-Ch] BYREF
  float targetForwardY; // [esp+6Ch] [ebp-8h] BYREF
  float camForwardY; // [esp+70h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] UpdateCutInSceneCameraFollow_4686F8\n");
  targetRotAngles[1] = GetSekBodyCurrentAnimTime(pModeBody);
  CalculateLocalRotationMatrix(
    pKutiRFrame,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &targetForwardX,
    &targetForwardY,
    &targetForwardZ,
    targetRotAngles,
    &targetYXrot,
    &targetZXrot);
  HanaFrame_GetRelPosition(pKutiRFrame, g_pSceneFrame, &targetRelPosX, &targetRelPosY, &targetRelPosZ);
  if ( fSpeed != 0.0 )
  {
    normVelX = *pfVelocity / 500.0;
    normVelY = pfVelocity[1] / 500.0;
    velMagnitude = sqrt(normVelX * normVelX + normVelY * normVelY);
    if ( velMagnitude != 0.0 )
    {
      if ( velMagnitude > (double)fSpeed )
      {
        normVelX = normVelX * fSpeed / velMagnitude;
        normVelY = normVelY * fSpeed / velMagnitude;
      }
      velMagnitudeRaw = sqrt(*pfVelocity * *pfVelocity + pfVelocity[1] * pfVelocity[1]);
      if ( fSpeed * 500.0 < velMagnitudeRaw )
      {
        *pfVelocity = fSpeed * *pfVelocity * 500.0 / velMagnitudeRaw;
        pfVelocity[1] = fSpeed * pfVelocity[1] * 500.0 / velMagnitudeRaw;
      }
      CalculateLocalRotationMatrix(
        g_pCameraFrame,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        &camForwardX,
        &camForwardY,
        &camForwardZ,
        &camUpX,
        &dirUpY,
        &dirUpZ);
      HanaFrame_GetRelPosition(g_pCameraFrame, g_pSceneFrame, &pOutX, &camPosY, &pOutZ);
      MtxStackStartMultLocal_4074EF();
      HanaFrame_GetRelPosition(g_pCameraFrame, pKutiRFrame, &targetRelPosX, &targetRelPosY, &targetRelPosZ);
      if ( targetRelPosZ <= 0.0 )
        SetFramePosRel_4069DD(g_pCameraFrame, pKutiRFrame, 0.0, 0.0, -15.0);
      else
        SetFramePosRel_4069DD(g_pCameraFrame, pKutiRFrame, 0.0, 0.0, 15.0);
      CameraLookAtFrame(g_pCameraFrame, pKutiRFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);
      MtxStackStartMultLocal_4074EF();
      HanaFrame_GetRelPosition(pKutiRFrame, g_pCameraFrame, &targetRelToCamX, &pOutY, &targetRelToCamZ);
      X = pOutY - normVelY;
      adjustedX = targetRelToCamX + normVelX;
      SetFramePosRel_4069DD(pKutiRFrame, g_pCameraFrame, adjustedX, X, targetRelToCamZ);
      UpdateBoneLookAtTransform(
        g_pCameraFrame,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        camForwardX,
        camForwardY,
        camForwardZ,
        camUpX,
        dirUpY,
        dirUpZ);
      SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, pOutX, camPosY, pOutZ);
      *pfAccumulator = *pfAccumulator - fFrameTime * *pfVelocity * 5.0 / 6.0 * fSpeed;
      pfAccumulator[1] = pfAccumulator[1] - fFrameTime * pfVelocity[1] * 5.0 / 6.0 * fSpeed;
      *pfVelocity = *pfVelocity + *pfAccumulator;
      pfVelocity[1] = pfVelocity[1] + pfAccumulator[1];
      *pfVelocity = *pfVelocity * 0.89999998;
      pfVelocity[1] = pfVelocity[1] * 0.89999998;
    }
  }
}

void __cdecl hook_Character_UpdateAnimationState(CSekBody *pModeBody, float nTargetState, float *pCurrentState)
{
  _DWORD stateToAnimGroup[10]; // [esp+8h] [ebp-FCh]
  float fCurrentState; // [esp+30h] [ebp-D4h]
  int easeInFrameStart; // [esp+34h] [ebp-D0h]
  int easeInFrameEnd; // [esp+38h] [ebp-CCh]
  int easeInDuration; // [esp+3Ch] [ebp-C8h]
  _DWORD easeInKeyframes[17]; // [esp+40h] [ebp-C4h] BYREF
  int timeFrameStart; // [esp+84h] [ebp-80h]
  int timeFrameEnd; // [esp+88h] [ebp-7Ch]
  int timeDuration; // [esp+8Ch] [ebp-78h]
  _DWORD timeKeyframes[17]; // [esp+90h] [ebp-74h] BYREF
  CHanaFrameT3EC_S354 *pFrame; // [esp+D4h] [ebp-30h]
  int bAnimationPlaying; // [esp+D8h] [ebp-2Ch]
  int easeOutFrameStart; // [esp+DCh] [ebp-28h]
  _DWORD easeOutKeyframes[9]; // [esp+E0h] [ebp-24h]
  ONCELOG("[HNH:SUB] Character_UpdateAnimationState_469EB6\n");
 
  stateToAnimGroup[0] = 0;
  stateToAnimGroup[1] = 1;
  stateToAnimGroup[2] = 2;
  stateToAnimGroup[3] = 2;
  stateToAnimGroup[4] = 3;
  stateToAnimGroup[5] = 3;
  stateToAnimGroup[6] = 3;
  stateToAnimGroup[7] = 3;
  stateToAnimGroup[8] = 0;
  stateToAnimGroup[9] = 0;
  pFrame = pModeBody->pSekHeader_220->pFrameT3EC;
  bAnimationPlaying = 0;
  fCurrentState = *pCurrentState;
  easeInFrameStart = 720;
  easeInFrameEnd = 739;
  easeInDuration = 43;
  easeInKeyframes[0] = 58;
  easeInKeyframes[1] = 340;
  easeInKeyframes[2] = 359;
  easeInKeyframes[3] = 101;
  easeInKeyframes[4] = 120;
  easeInKeyframes[5] = 1140;
  easeInKeyframes[6] = 1159;
  easeInKeyframes[7] = 811;
  easeInKeyframes[8] = 830;
  easeInKeyframes[9] = 811;
  easeInKeyframes[10] = 819;
  easeInKeyframes[11] = 819;
  easeInKeyframes[12] = 821;
  memset(&easeInKeyframes[13], 0, 16);
  timeFrameStart = 520;
  timeFrameEnd = 539;
  timeDuration = 41;
  timeKeyframes[0] = 60;
  timeKeyframes[1] = 940;
  timeKeyframes[2] = 959;
  timeKeyframes[3] = 101;
  timeKeyframes[4] = 120;
  timeKeyframes[5] = 821;
  timeKeyframes[6] = 830;
  timeKeyframes[7] = 811;
  timeKeyframes[8] = 830;
  timeKeyframes[9] = 0;
  timeKeyframes[10] = 1;
  timeKeyframes[11] = 0;
  timeKeyframes[12] = 1;
  memset(&timeKeyframes[13], 0, 16);
  easeOutFrameStart = 45;
  easeOutKeyframes[0] = 55;
  easeOutKeyframes[1] = 161;
  easeOutKeyframes[2] = 179;
  easeOutKeyframes[3] = 71;
  easeOutKeyframes[4] = 89;
  easeOutKeyframes[5] = 101;
  easeOutKeyframes[6] = 119;
  easeOutKeyframes[7] = 0;
  easeOutKeyframes[8] = 0;
  if ( nTargetState > 4.0 )
    nTargetState = 4.0;
  if ( stateToAnimGroup[(__int64)fCurrentState] != stateToAnimGroup[(__int64)nTargetState] )
  {
    bAnimationPlaying = HanaObj_CheckFrameCondition(pFrame, 0);
    if ( !bAnimationPlaying )
    {
      if ( stateToAnimGroup[(__int64)fCurrentState] < stateToAnimGroup[(__int64)nTargetState] )
        Character_ApplyAnimationParamsToChildren(
          pFrame,
          0,
          *(&easeInFrameStart + 4 * stateToAnimGroup[(__int64)nTargetState]),
          *(&easeInFrameEnd + 4 * stateToAnimGroup[(__int64)nTargetState]),
          easeInKeyframes[4 * stateToAnimGroup[(__int64)nTargetState] - 1],
          easeInKeyframes[4 * stateToAnimGroup[(__int64)nTargetState]]);
      if ( stateToAnimGroup[(__int64)fCurrentState] > stateToAnimGroup[(__int64)nTargetState] )
        Character_ApplyAnimationParamsToChildren(
          pFrame,
          0,
          *(&timeFrameStart + 4 * stateToAnimGroup[(__int64)nTargetState]),
          *(&timeFrameEnd + 4 * stateToAnimGroup[(__int64)nTargetState]),
          timeKeyframes[4 * stateToAnimGroup[(__int64)nTargetState] - 1],
          timeKeyframes[4 * stateToAnimGroup[(__int64)nTargetState]]);
      Character_ApplyAnimationParamsToChildren(
        pFrame,
        1,
        0,
        0,
        easeOutKeyframes[2 * stateToAnimGroup[(__int64)nTargetState] - 1],
        easeOutKeyframes[2 * stateToAnimGroup[(__int64)nTargetState]]);
      fCurrentState = (float)(__int64)nTargetState;
    }
  }
  *pCurrentState = fCurrentState;
}

BOOL __cdecl hook_HanaObj_CheckFrameCondition(CHanaFrameT3EC_S354 *pFrame, bool bCheck)
{
  BOOL bRet; // eax
  CHanaMorpT457_S40C *pMorp; // [esp+0h] [ebp-18h]
  CHanaFrameArrayT3ED_S138 *pFrameList; // [esp+4h] [ebp-14h] BYREF
  unsigned int nIndex; // [esp+8h] [ebp-10h]
  CHanaFrameT3EC_S354 *pSubFrame; // [esp+Ch] [ebp-Ch] BYREF
  int result; // [esp+10h] [ebp-8h]
  unsigned int nCount; // [esp+14h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaObj_CheckFrameCondition_48713F\n");

  result = 0;
  CHanaObj__NewFrameArrayT3EDFromChildT3EC(pFrame, &pFrameList);
  nCount = CHanaObj__GetFramesCountT3ED_408E75(pFrameList);
  for ( nIndex = 0; nIndex < nCount; ++nIndex )
  {
    CHanaObj__GetFrameAtIndexT3ED(pFrameList, nIndex, &pSubFrame);
    if ( pSubFrame->pAttachedObj_20C && pSubFrame->pAttachedObj_20C->dwType == 0x457 )
    {
      pMorp = pSubFrame->pAttachedObj_20C;
      if ( bCheck && pMorp->dwEaseEnabled_358 == 2 )
      {
        result = 1;
      }
      else if ( !bCheck && pMorp->nTimeState_36C == 2 )
      {
        result = 1;
      }
    }
    bRet = HanaObj_CheckFrameCondition(pSubFrame, bCheck);
    result |= bRet;
    if ( pSubFrame )
    {
      CHanaObj__Free_40C1E4(pSubFrame);
      pSubFrame = 0;
    }
    if ( result == 1 )
      break;
  }
  if ( pFrameList )
  {
    CHanaObj__Free_40C1E4(pFrameList);
    pFrameList = 0;
  }
  return result;
}

void __cdecl hook_SoundAH_ManageEffectsAndAnimate(
  CSoundStruct_8 **ppSoundAHs,
  CHanaFrameT3EC_S354 *pFrame,
  float *pEffectParam,
  float fEffectIntensity,
  float fFrameTime)
{
  int nIdx; // esi
  D3DVALUE posX; // [esp+10h] [ebp-94h] BYREF
  int nRandomSoundIdx; // [esp+14h] [ebp-90h]
  float fEffectIntensityToGroup[10]; // [esp+18h] [ebp-8Ch]
  float fPlaybackAmplitude; // [esp+40h] [ebp-64h] BYREF
  int i; // [esp+44h] [ebp-60h]
  D3DVALUE posZ; // [esp+48h] [ebp-5Ch] BYREF
  LONG nVol; // [esp+4Ch] [ebp-58h]
  int soundGroupRanges[10]; // [esp+50h] [ebp-54h]
  int nActiveSoundIdx; // [esp+78h] [ebp-2Ch]
  _BYTE bSoundPlaying[32]; // [esp+7Ch] [ebp-28h] BYREF
  D3DVALUE posY; // [esp+9Ch] [ebp-8h] BYREF
  float nSoundGroupIdx; // [esp+A0h] [ebp-4h]
  ONCELOG("[HNH:SUB] SoundAH_ManageEffectsAndAnimate_4690AB\n");
  
  nActiveSoundIdx = -1;
  memset(bSoundPlaying, 0, sizeof(bSoundPlaying));
  i = 0;
  nVol = 0;
  soundGroupRanges[0] = 21;
  soundGroupRanges[1] = 21;
  soundGroupRanges[2] = 0;
  soundGroupRanges[3] = 6;
  soundGroupRanges[4] = 7;
  soundGroupRanges[5] = 13;
  soundGroupRanges[6] = 14;
  soundGroupRanges[7] = 20;
  soundGroupRanges[8] = 0;
  soundGroupRanges[9] = 0;
  nRandomSoundIdx = 0;
  fEffectIntensityToGroup[0] = 0.0;
  fEffectIntensityToGroup[1] = 1.0;
  fEffectIntensityToGroup[2] = 2.0;
  fEffectIntensityToGroup[3] = 2.0;
  fEffectIntensityToGroup[4] = 3.0;
  fEffectIntensityToGroup[5] = 3.0;
  fEffectIntensityToGroup[6] = 3.0;
  fEffectIntensityToGroup[7] = 3.0;
  fEffectIntensityToGroup[8] = 3.0;
  fEffectIntensityToGroup[9] = 0.0;
  if ( *pEffectParam > 5.0 )
    *pEffectParam = 5.0;
  if ( *pEffectParam < 0.0 )
    *pEffectParam = 0.0;
  nSoundGroupIdx = fEffectIntensityToGroup[(unsigned int)(__int64)(fEffectIntensity + *pEffectParam)];
  if ( pFrame )
  {
    if ( nSoundGroupIdx > 3.0 )
      nSoundGroupIdx = 3.0;
    if ( nSoundGroupIdx < 0.0 )
      nSoundGroupIdx = 0.0;
    for ( i = 0; i < 30; ++i )
    {
      if ( ppSoundAHs[i] )
      {
        HanaFrame_GetCenterPosition_408C3E(pFrame, g_pSceneFrame, &posX, &posY, &posZ);
        if ( ppSoundAHs[i]->pDSBuffer_0
          && !ppSoundAHs[i]->pDSBuffer_0->GetStatus(&g_dwDSStatus_5185C8)
          && (g_dwDSStatus_5185C8 & 1) != 0 )
        {
          ppSoundAHs[i]->pDS3DBuffer_4->SetPosition(posX, posY, posZ, 1u);
          nActiveSoundIdx = i;
          bSoundPlaying[i] = 1;
        }
      }
    }
    if ( nActiveSoundIdx == -1 )
    {
      nIdx = (__int64)nSoundGroupIdx;
      nRandomSoundIdx = rand() % (soundGroupRanges[2 * nIdx + 1] - soundGroupRanges[2 * nIdx] + 1)
        + soundGroupRanges[2 * nIdx];
      if ( ppSoundAHs[nRandomSoundIdx] )
      {
        if ( ppSoundAHs[nRandomSoundIdx]->pDSBuffer_0 )
          ppSoundAHs[nRandomSoundIdx]->pDSBuffer_0->SetVolume(0);
        ppSoundAHs[nRandomSoundIdx]->pDSBuffer_0->SetCurrentPosition(
          0);
        ppSoundAHs[nRandomSoundIdx]->pDSBuffer_0->Play(0, 0, 0);
      }
    }
    else if ( nActiveSoundIdx >= soundGroupRanges[2 * (__int64)nSoundGroupIdx]
      && soundGroupRanges[2 * (__int64)nSoundGroupIdx + 1] >= nActiveSoundIdx )
    {
      if ( ppSoundAHs[nActiveSoundIdx] )
      {
        nVol = (__int64)(fFrameTime * 300.0) + DSBufferGetVolume_423C65(ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0);
        if ( nVol <= 0 )
        {
          if ( ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0 )
          {
            if ( nVol >= -10000 )
            {
              if ( nVol <= 0 )
                ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(
                  nVol);
              else
                ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(0);
            }
            else
            {
              ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(
                -10000);
            }
          }
        }
        else if ( ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0 )
        {
          ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0->SetVolume(0);
        }
        GetSoundBufferAverageLevel(ppSoundAHs[nActiveSoundIdx]->pDSBuffer_0, &fPlaybackAmplitude);
        HanaMorp_AnimateAndPropagate(pFrame, fFrameTime, fPlaybackAmplitude, 0);
        for ( i = 0; i < 30; ++i )
        {
          if ( bSoundPlaying[i] == 1 && nActiveSoundIdx != i )
          {
            nVol = DSBufferGetVolume_423C65(ppSoundAHs[i]->pDSBuffer_0);
            if ( nVol >= -1000 )
            {
              if ( ppSoundAHs[i]->pDSBuffer_0 )
              {
                if ( (int)(nVol - (__int64)(fFrameTime * 300.0)) >= -10000 )
                {
                  if ( (int)(nVol - (__int64)(fFrameTime * 300.0)) <= 0 )
                    ppSoundAHs[i]->pDSBuffer_0->SetVolume(
                      nVol - (__int64)(fFrameTime * 300.0));
                  else
                    ppSoundAHs[i]->pDSBuffer_0->SetVolume(0);
                }
                else
                {
                  ppSoundAHs[i]->pDSBuffer_0->SetVolume(-10000);
                }
              }
            }
            else
            {
              ppSoundAHs[i]->pDSBuffer_0->Stop();
            }
          }
        }
      }
    }
    else
    {
      for ( i = 0; i < 30; ++i )
      {
        if ( bSoundPlaying[i] == 1 )
        {
          nVol = DSBufferGetVolume_423C65(ppSoundAHs[i]->pDSBuffer_0);
          if ( nVol >= -1000 )
          {
            if ( ppSoundAHs[i]->pDSBuffer_0 )
            {
              if ( (int)(nVol - (__int64)(fFrameTime * 300.0)) >= -10000 )
              {
                if ( (int)(nVol - (__int64)(fFrameTime * 300.0)) <= 0 )
                  ppSoundAHs[i]->pDSBuffer_0->SetVolume(
                    nVol - (__int64)(fFrameTime * 300.0));
                else
                  ppSoundAHs[i]->pDSBuffer_0->SetVolume(0);
              }
              else
              {
                ppSoundAHs[i]->pDSBuffer_0->SetVolume(-10000);
              }
            }
          }
          else
          {
            ppSoundAHs[i]->pDSBuffer_0->Stop();
          }
        }
      }
    }
  }
  *pEffectParam = *pEffectParam - fFrameTime / 15.0;
  if ( *pEffectParam > 4.1999998 )
    *pEffectParam = 4.1999998;
  if ( *pEffectParam < 0.0 )
    *pEffectParam = 0.0;
}

void __cdecl hook_HanaMorp_AnimateAndPropagate(CHanaFrameT3EC_S354 *pMorp, float fTime, float fWeight, int nFlag)
{
  unsigned int loopIdx; // [esp+0h] [ebp-8h]
  CHanaNode *pArray; // [esp+4h] [ebp-4h]
  ONCELOG("[HNH:SUB] HanaMorp_AnimateAndPropagate_481BE0\n");
 
  HanaMorpT457_Animate(pMorp, fTime, fWeight, nFlag);
  if ( pMorp->nCountChild )
  {
    pArray = pMorp->pFirstChildNode;
    for ( loopIdx = 0; loopIdx < pMorp->nCountChild; ++loopIdx )
    {
      HanaMorp_AnimateAndPropagate(pArray->pData, fTime, fWeight, nFlag);
      pArray = pArray->pNext;
    }
  }
}

void __cdecl hook_CameraFrame_TrackBetweenObjectsAndLookAt(
  CHanaFrameT3EC_S354 *pFrame,
  CHanaFrameT3EC_S354 *pObjA,
  CHanaFrameT3EC_S354 *parentStruct)
{
  float clampedX; // [esp+0h] [ebp-50h]
  float clampedY; // [esp+4h] [ebp-4Ch]
  float clampedZ; // [esp+8h] [ebp-48h]
  float objB_x; // [esp+Ch] [ebp-44h] BYREF
  float closestY; // [esp+10h] [ebp-40h]
  float v8; // [esp+14h] [ebp-3Ch]
  float v9; // [esp+18h] [ebp-38h] BYREF
  float v10; // [esp+1Ch] [ebp-34h] BYREF
  float frame_z; // [esp+20h] [ebp-30h] BYREF
  float squaredDistAB; // [esp+24h] [ebp-2Ch]
  float objA_x; // [esp+28h] [ebp-28h] BYREF
  float objA_y; // [esp+2Ch] [ebp-24h] BYREF
  float objA_z; // [esp+30h] [ebp-20h] BYREF
  float t; // [esp+34h] [ebp-1Ch] BYREF
  float objB_y; // [esp+38h] [ebp-18h] BYREF
  float objB_z; // [esp+3Ch] [ebp-14h]
  float clampedPosZ; // [esp+40h] [ebp-10h] BYREF
  float clampedPosY; // [esp+44h] [ebp-Ch] BYREF
  float v21; // [esp+48h] [ebp-8h] BYREF
  float distToClosest; // [esp+4Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] CameraFrame_TrackBetweenObjectsAndLookAt_466978\n");

  if ( pObjA && parentStruct )
  {
    HanaFrame_GetRelPosition(g_pCameraFrame, pFrame, &objA_z, &objA_y, &objA_x);
    HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &frame_z, &v10, &v9);
    HanaFrame_GetRelPosition(pObjA, g_pSceneFrame, &clampedPosZ, &objB_y, &t);
    HanaFrame_GetRelPosition(parentStruct, g_pSceneFrame, &objB_x, &v21, &clampedPosY);
    squaredDistAB = (clampedPosZ - objB_x) * (clampedPosZ - objB_x)
      + (objB_y - v21) * (objB_y - v21)
      + (t - clampedPosY) * (t - clampedPosY);
    if ( squaredDistAB != 0.0 )
    {
      objB_z = ((frame_z - objB_x) * (clampedPosZ - objB_x)
        + (v10 - v21) * (objB_y - v21)
        + (v9 - clampedPosY) * (t - clampedPosY))
        / squaredDistAB;
      if ( objB_z < 0.0 )
        objB_z = 0.0;
      if ( objB_z > 1.0 )
        objB_z = 1.0;
      v8 = (clampedPosZ - objB_x) * objB_z + objB_x;
      closestY = (objB_y - v21) * objB_z + v21;
      distToClosest = (t - clampedPosY) * objB_z + clampedPosY;
      squaredDistAB = sqrt(
        (frame_z - v8) * (frame_z - v8)
        + (v10 - closestY) * (v10 - closestY)
        + (v9 - distToClosest) * (v9 - distToClosest));
      if ( squaredDistAB != 0.0 && squaredDistAB > 1.0 )
      {
        clampedZ = (v9 - distToClosest) * 1.0 / squaredDistAB + distToClosest;
        clampedY = (v10 - closestY) * 1.0 / squaredDistAB + closestY;
        clampedX = (frame_z - v8) * 1.0 / squaredDistAB + v8;
        SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, clampedX, clampedY, clampedZ);
      }
    }
    SetFramePosRel_4069DD(g_pCameraFrame, pFrame, objA_z, objA_y, objA_x);
    UpdateBoneLookAtTransform(pFrame, parentStruct, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  }
}

void __cdecl hook_HanaMatrix_ApplyTransform(CHanaFrameT3EC_S354 *pFrame, const D3DXMATRIX *pMat)
{
  D3DXMATRIX mInverse; // [esp+8h] [ebp-40h] BYREF
  ONCELOG("[HNH:SUB] HanaMatrix_ApplyTransform_40799F\n");
 
  qmemcpy(&pFrame->matWorld_178, pMat, sizeof(pFrame->matWorld_178));
  D3DXMatrixInverse(&mInverse, 0, &pFrame->matParentWorld_1B8);
  D3DXMatrixMultiply(&pFrame->matLocal_138, &pFrame->matWorld_178, &mInverse);
}

void __cdecl hook_CutInScene_CyclePosAndApplyCameraOffset(
  CHanaSceneT458_S504 *pSceneObj,
  DWORD *pPosIndex,
  CHanaFrameT3EC_S354 *pChild)
{
  int iPosIndex; // [esp+0h] [ebp-4Ch]
  int posFrameCount; // [esp+4h] [ebp-48h]
  int bFoundPosFrame; // [esp+8h] [ebp-44h]
  D3DXMATRIX pM; // [esp+Ch] [ebp-40h] BYREF
  ONCELOG("[HNH:SUB] CutInScene_CyclePosAndApplyCameraOffset_467160\n");
  posFrameCount = 0;
  bFoundPosFrame = 0;
  for ( iPosIndex = 0; iPosIndex < pSceneObj->dwCount_130; ++iPosIndex )
  {
    if ( !_memicmp(pSceneObj->refsFrames_134[iPosIndex]->szName_8, "POS", 3u) )
    {
      if ( *pPosIndex == posFrameCount )
      {
        HanaFrame_GetMatAfter(pSceneObj->refsFrames_134[iPosIndex], &pM);
        HanaMatrix_ApplyTransform(g_pCameraFrame, &pM);
        bFoundPosFrame = 1;
      }
      ++posFrameCount;
    }
  }
  if ( bFoundPosFrame == 1 && posFrameCount )
  {
    ++*pPosIndex;
    *pPosIndex %= posFrameCount;
  }
  SetFramePosRel_4069DD(pChild, g_pCameraFrame, 0.0, 0.0, 15.0);
}

void __cdecl hook_HanaFrame_GetMatAfter(CHanaFrameT3EC_S354 *pFrame, D3DXMATRIX *pOutMat)
{
  ONCELOG("[HNH:SUB] HanaFrame_GetMatAfter_407945\n");

  qmemcpy(pOutMat, &pFrame->matWorld_178, sizeof(D3DXMATRIX));
}

BOOL __cdecl hook_MoveCutInSceneFramesWithCameraConstraint(
  CHanaFrameT3EC_S354 *pFrameA,
  CHanaFrameT3EC_S354 *pChild,
  int deltaX,
  int deltaY,
  float maxDistance)
{
  float adjustedX; // [esp+0h] [ebp-A0h]
  float adjustedX2; // [esp+0h] [ebp-A0h]
  float adjustedY; // [esp+4h] [ebp-9Ch]
  float adjustedY2; // [esp+4h] [ebp-9Ch]
  float v10; // [esp+Ch] [ebp-94h] BYREF
  float frameARelToCamZ; // [esp+10h] [ebp-90h] BYREF
  float frameBRelToCamZ; // [esp+14h] [ebp-8Ch] BYREF
  float camForwardX; // [esp+18h] [ebp-88h] BYREF
  float pOutZ; // [esp+1Ch] [ebp-84h] BYREF
  float camPosZ; // [esp+20h] [ebp-80h] BYREF
  float pOutY; // [esp+24h] [ebp-7Ch] BYREF
  float v17; // [esp+28h] [ebp-78h] BYREF
  float camUpZ; // [esp+2Ch] [ebp-74h] BYREF
  float frameARelToCamY; // [esp+30h] [ebp-70h] BYREF
  float fRelPosY; // [esp+34h] [ebp-6Ch] BYREF
  float frameBRelToCamY; // [esp+38h] [ebp-68h] BYREF
  float frameARelToBY; // [esp+3Ch] [ebp-64h] BYREF
  float distance; // [esp+40h] [ebp-60h]
  BOOL bClamped; // [esp+44h] [ebp-5Ch]
  float camPosY; // [esp+48h] [ebp-58h] BYREF
  float normDeltaY; // [esp+4Ch] [ebp-54h]
  float camYXrot; // [esp+50h] [ebp-50h] BYREF
  float pOutX; // [esp+54h] [ebp-4Ch] BYREF
  float normDeltaX; // [esp+58h] [ebp-48h]
  float frameARelToCamX; // [esp+5Ch] [ebp-44h] BYREF
  float frameBRelPosX; // [esp+60h] [ebp-40h] BYREF
  float camForwardZ; // [esp+64h] [ebp-3Ch] BYREF
  float v33; // [esp+68h] [ebp-38h] BYREF
  float v34[4]; // [esp+6Ch] [ebp-34h] BYREF
  float camPosX; // [esp+7Ch] [ebp-24h] BYREF
  float camXYrot; // [esp+80h] [ebp-20h] BYREF
  float frameBForwardY; // [esp+84h] [ebp-1Ch] BYREF
  float frameBForwardZ; // [esp+88h] [ebp-18h] BYREF
  float frameBUpX; // [esp+8Ch] [ebp-14h] BYREF
  float frameBUpY[2]; // [esp+90h] [ebp-10h] BYREF
  float frameBForwardX; // [esp+98h] [ebp-8h] BYREF
  float camForwardY; // [esp+9Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] MoveCutInSceneFramesWithCameraConstraint_4682D6\n");

  bClamped = 0;
  if ( maxDistance == 0.0 )
    return bClamped;
  normDeltaX = (double)deltaX / 500.0;
  normDeltaY = (double)deltaY / 500.0;
  distance = sqrt(normDeltaX * normDeltaX + normDeltaY * normDeltaY);
  if ( distance > (double)maxDistance && distance != 0.0 )
  {
    normDeltaX = normDeltaX * maxDistance / distance;
    normDeltaY = normDeltaY * maxDistance / distance;
    bClamped = 1;
  }
  CalculateLocalRotationMatrix(
    g_pCameraFrame,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &camForwardX,
    &camForwardY,
    &camForwardZ,
    &camXYrot,
    &camYXrot,
    &camUpZ);
  HanaFrame_GetRelPosition(g_pCameraFrame, g_pSceneFrame, &camPosX, &camPosY, &camPosZ);
  HanaFrame_GetRelPosition(pChild, g_pSceneFrame, &pOutX, &pOutY, &v10);
  MtxStackStartMultLocal_4074EF();
  CalculateLocalRotationMatrix(
    pChild,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &frameBForwardX,
    &frameBForwardY,
    &frameBForwardZ,
    &frameBUpX,
    frameBUpY,
    &frameBUpY[1]);
  HanaFrame_GetRelPosition(pChild, g_pSceneFrame, &frameBRelPosX, &fRelPosY, &v17);
  SetFramePosRel_4069DD(pChild, pFrameA, 0.0, 0.0, 0.0);
  UpdateBoneLookAtTransform(pChild, pFrameA, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  HanaFrame_GetRelPosition(pFrameA, g_pCameraFrame, &frameARelToCamX, &frameARelToCamY, &frameARelToCamZ);
  adjustedY = frameARelToCamY - normDeltaY;
  adjustedX = frameARelToCamX + normDeltaX;
  SetFramePosRel_4069DD(pFrameA, g_pCameraFrame, adjustedX, adjustedY, frameARelToCamZ);
  HanaFrame_GetRelPosition(pFrameA, pChild, v34, &frameARelToBY, &pOutZ);
  SetFramePosRel_4069DD(pFrameA, pChild, v34[0], frameARelToBY, 0.0);
  UpdateBoneLookAtTransform(
    pChild,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    frameBForwardX,
    frameBForwardY,
    frameBForwardZ,
    frameBUpX,
    frameBUpY[0],
    frameBUpY[1]);
  SetFramePosRel_4069DD(pChild, (CHanaFrameT3EC_S354 *)g_pSceneFrame, frameBRelPosX, fRelPosY, v17);
  MtxStackStartMultLocal_4074EF();
  CalculateLocalRotationMatrix(
    pFrameA,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &frameBForwardX,
    &frameBForwardY,
    &frameBForwardZ,
    &frameBUpX,
    frameBUpY,
    &frameBUpY[1]);
  HanaFrame_GetRelPosition(pFrameA, g_pSceneFrame, &frameBRelPosX, &fRelPosY, &v17);
  SetFramePosRel_4069DD(pFrameA, pChild, 0.0, 0.0, 0.0);
  UpdateBoneLookAtTransform(pFrameA, pChild, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  HanaFrame_GetRelPosition(pChild, g_pCameraFrame, &v33, &frameBRelToCamY, &frameBRelToCamZ);
  adjustedY2 = frameBRelToCamY - normDeltaY;
  adjustedX2 = v33 + normDeltaX;
  SetFramePosRel_4069DD(pChild, g_pCameraFrame, adjustedX2, adjustedY2, frameBRelToCamZ);
  HanaFrame_GetRelPosition(pChild, pFrameA, v34, &frameARelToBY, &pOutZ);
  SetFramePosRel_4069DD(pChild, pFrameA, v34[0], frameARelToBY, 0.0);
  UpdateBoneLookAtTransform(
    pFrameA,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    frameBForwardX,
    frameBForwardY,
    frameBForwardZ,
    frameBUpX,
    frameBUpY[0],
    frameBUpY[1]);
  SetFramePosRel_4069DD(pFrameA, (CHanaFrameT3EC_S354 *)g_pSceneFrame, frameBRelPosX, fRelPosY, v17);
  MtxStackStartMultLocal_4074EF();
  UpdateBoneLookAtTransform(
    g_pCameraFrame,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    camForwardX,
    camForwardY,
    camForwardZ,
    camXYrot,
    camYXrot,
    camUpZ);
  SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, camPosX, camPosY, camPosZ);
  MtxStackStartMultLocal_4074EF();
  return bClamped;
}

void __cdecl hook_UpdateCutInSceneBoneTransforms(
  CSekBody *pSekBody,
  CHanaAnimSetT3F1_S150 *pAnimSetsMain[],
  CHanaFrameT3EC_S354 *pFramesMain[],
  CHanaFrameT3EC_S354 *arrKutiRFrames[],
  CHanaAnimSetT3F1_S150 *pAnimSetsKutiR[],
  CHanaFrameT3EC_S354 *pFramesKutiR[],
  unsigned int boneIndex,
  float blendAlpha)
{
  float pXpos; // [esp+0h] [ebp-50h] BYREF
  float mainForwardX; // [esp+4h] [ebp-4Ch] BYREF
  float pZXrot; // [esp+8h] [ebp-48h] BYREF
  float mainUpZ; // [esp+Ch] [ebp-44h] BYREF
  float kutiRRelPosZ; // [esp+10h] [ebp-40h] BYREF
  float mainRelPosZ; // [esp+14h] [ebp-3Ch] BYREF
  float pYXrot; // [esp+18h] [ebp-38h] BYREF
  float mainUpY; // [esp+1Ch] [ebp-34h] BYREF
  float kutiRPosY; // [esp+20h] [ebp-30h] BYREF
  CHanaFrameT3EC_S354 *pParent; // [esp+24h] [ebp-2Ch]
  float mainPosY; // [esp+28h] [ebp-28h] BYREF
  float pZpos; // [esp+2Ch] [ebp-24h] BYREF
  float mainForwardZ; // [esp+30h] [ebp-20h] BYREF
  float kutiRXYrot; // [esp+34h] [ebp-1Ch] BYREF
  float currentAnimTime; // [esp+38h] [ebp-18h]
  float mainUpX; // [esp+3Ch] [ebp-14h] BYREF
  float kutiRYpos; // [esp+40h] [ebp-10h] BYREF
  float kutiRPosX; // [esp+44h] [ebp-Ch] BYREF
  float mainForwardY; // [esp+48h] [ebp-8h] BYREF
  float mainPosX; // [esp+4Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateCutInSceneBoneTransforms_4680B4\n");
  
  currentAnimTime = GetSekBodyCurrentAnimTime(pSekBody);
  pParent = 0;
  if ( boneIndex <= 0xA && pFramesKutiR[boneIndex] && pAnimSetsKutiR[boneIndex] )
  {
    pParent = pFramesMain[boneIndex]->pParent;
    SetAnimSetTime(pAnimSetsMain[boneIndex], currentAnimTime);
    SetAnimSetTime(pAnimSetsKutiR[boneIndex], currentAnimTime);
    MtxStackStartMultLocal_4074EF();
    if ( boneIndex != 5 )
    {
      CalculateLocalRotationMatrix(
        pFramesMain[boneIndex],
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        &mainForwardX,
        &mainForwardY,
        &mainForwardZ,
        &mainUpX,
        &mainUpY,
        &mainUpZ);
      HanaFrame_GetRelPosition(pFramesMain[boneIndex], g_pSceneFrame, &mainPosX, &mainPosY, &mainRelPosZ);
      CalculateLocalRotationMatrix(
        pFramesKutiR[boneIndex],
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        &pXpos,
        &kutiRYpos,
        &pZpos,
        &kutiRXYrot,
        &pYXrot,
        &pZXrot);
      HanaFrame_GetRelPosition(pFramesKutiR[boneIndex], g_pSceneFrame, &kutiRPosX, &kutiRPosY, &kutiRRelPosZ);
      SetAnimSetTime(pAnimSetsMain[boneIndex], blendAlpha);
      SetAnimSetTime(pAnimSetsKutiR[boneIndex], blendAlpha);
      MtxStackStartMultLocal_4074EF();
      UpdateBoneLookAtTransform(
        pFramesMain[boneIndex],
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        mainForwardX,
        mainForwardY,
        mainForwardZ,
        mainUpX,
        mainUpY,
        mainUpZ);
      SetFramePosRel_4069DD(pFramesMain[boneIndex], (CHanaFrameT3EC_S354 *)g_pSceneFrame, mainPosX, mainPosY, mainRelPosZ);
      UpdateBoneLookAtTransform(
        pFramesKutiR[boneIndex],
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        pXpos,
        kutiRYpos,
        pZpos,
        kutiRXYrot,
        pYXrot,
        pZXrot);
      SetFramePosRel_4069DD(pFramesKutiR[boneIndex], (CHanaFrameT3EC_S354 *)g_pSceneFrame, kutiRPosX, kutiRPosY, kutiRRelPosZ);
    }
  }
}

void __cdecl hook_UpdateSekBodyAnimationMatrix(CSekBody *pModeBody, float fTime)
{
  float targetTime; // [esp+0h] [ebp-Ch]
  float currentTime; // [esp+8h] [ebp-4h]
  ONCELOG("[HNH:SUB] UpdateSekBodyAnimationMatrix_46A503\n");
  currentTime = GetSekBodyCurrentAnimTime(pModeBody);
  SetAnimSetTime(pModeBody->pSekHeader_220->pAnimSetT3F1, 31.0);
  targetTime = fTime + 31.0;
  UpdateFrameAnimationMatrix(pModeBody->pSekHeader_220->pFrameT3EC, targetTime, currentTime);
}

void __cdecl hook_UpdateFrameAnimationMatrix(CHanaFrameT3EC_S354 *pFrame, float targetTime, float currentTime)
{
  D3DXQUATERNION quatRotation; // [esp+0h] [ebp-5Ch] BYREF
  unsigned int childIndex; // [esp+10h] [ebp-4Ch]
  CHanaFrameT3EC_S354 *childFrame; // [esp+14h] [ebp-48h]
  CHanaNode *pChildArray; // [esp+18h] [ebp-44h]
  D3DXMATRIX tempMatrix; // [esp+1Ch] [ebp-40h] BYREF

  ONCELOG("[HNH:SUB] UpdateFrameAnimationMatrix_46A3E9\n");

  childFrame = 0;
  if ( pFrame->pHanaAnim_2C0 )
  {
    D3DXMatrixInverse(&tempMatrix, 0, &pFrame->matLocal_138);
    InterpolateHanaAnimFrame(pFrame->pHanaAnim_2C0, targetTime);
    D3DXMatrixMultiply(&tempMatrix, &pFrame->matLocal_138, &tempMatrix);
    memset(tempMatrix.m[3], 0, 12);
    D3DXQuaternionRotationMatrix(&quatRotation, &tempMatrix);
    InterpolateHanaAnimFrame(pFrame->pHanaAnim_2C0, currentTime);
    D3DXMatrixMultiply(&pFrame->matLocal_138, &tempMatrix, &pFrame->matLocal_138);
  }
  if ( pFrame->nCountChild )
  {
    pChildArray = pFrame->pFirstChildNode;
    for ( childIndex = 0; childIndex < pFrame->nCountChild; ++childIndex )
    {
      childFrame = pChildArray->pData;
      if ( childFrame )
        UpdateFrameAnimationMatrix(childFrame, targetTime, currentTime);
      pChildArray = pChildArray->pNext;
    }
  }

}


BOOL __cdecl hook_Frame_CheckCameraDistanceAndRecurse(CHanaFrameT3EC_S354 *pFrame)
{
  BOOL bChildOk; // eax
  float moveX; // [esp+0h] [ebp-30h]
  float moveY; // [esp+4h] [ebp-2Ch]
  float moveZ; // [esp+8h] [ebp-28h]
  CHanaNode *pFirstChildNode; // [esp+10h] [ebp-20h]
  float relPosX; // [esp+14h] [ebp-1Ch] BYREF
  unsigned int i; // [esp+18h] [ebp-18h]
  float fDist; // [esp+1Ch] [ebp-14h]
  BOOL bOk; // [esp+20h] [ebp-10h]
  float relPosZ; // [esp+24h] [ebp-Ch] BYREF
  float relPosY; // [esp+28h] [ebp-8h] BYREF
  int v13; // [esp+2Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] Frame_CheckCameraDistanceAndRecurse_4665F0\n");

  bOk = 0;
  v13 = 0;
  if ( !pFrame->bAlphaDeferred_130 )            // Skip if flag_130 is set (already processed or disabled)
  {
    HanaFrame_GetRelPosition(g_pCameraFrame, pFrame, &relPosX, &relPosY, &relPosZ);// Get camera-relative position of this frame
    fDist = sqrt(relPosX * relPosX + relPosY * relPosY + relPosZ * relPosZ);
    if ( fDist < 3.0 )                          // Within 3.0 units = "in practice range"
    {
      if ( pFrame->field_11C )
      {
        bOk = 1;                                // Practice animations initialized ?øΩ?øΩ signal practice mode active
      }
      else if ( fDist != 0.0 )                  // Not initialized yet ?øΩ?øΩ push camera to exactly 3.0 units away
      {
        moveZ = relPosZ * 3.0 / fDist;
        moveY = relPosY * 3.0 / fDist;
        moveX = relPosX * 3.0 / fDist;
        SetFramePosRel_4069DD(g_pCameraFrame, pFrame, moveX, moveY, moveZ);
      }
    }
  }
  if ( pFrame->nCountChild )                    // Recurse into children via CHanaNode linked list
  {
    pFirstChildNode = pFrame->pFirstChildNode;
    for ( i = 0; i < pFrame->nCountChild; ++i )
    {
      bChildOk = Frame_CheckCameraDistanceAndRecurse(pFirstChildNode->pData);
      bOk |= bChildOk;
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
  return bOk;                                   // 1 if any frame in subtree triggered practice mode
}

void __cdecl hook_Cursor_ClampToCenterOrSprite(
  int nMode,
  int *pCursorX,
  int *pCursorY,
  int *pDeltaX,
  int *pDeltaY,
  CHanaSpriteT3F6_S1FC *pSprite)
{
  int centerX; // eax
  int centerY; // [esp-4h] [ebp-8h]

  ONCELOG("[HNH:SUB] Cursor_ClampToCenterOrSprite_468A4A\n");

  if ( !nMode
    && (*pCursorX <= 10
      || ConvertFrom640AsInt(576) <= *pCursorX
      || *pCursorY <= 10
      || ConvertFrom640AsInt(470) <= *pCursorY) )
  {
    centerY = ConvertFrom640AsInt(240);
    centerX = ConvertFrom640AsInt(320);
    SetCursorPos(centerX, centerY);
    *pDeltaX -= *pCursorX - ConvertFrom640AsInt(320);
    *pCursorX = ConvertFrom640AsInt(320);
    *pDeltaY -= *pCursorY - ConvertFrom640AsInt(240);
    *pCursorY = ConvertFrom640AsInt(240);
  }
  if ( nMode )
  {
    if ( pSprite )
    {
      SetCursorPos((__int64)pSprite->destWHZ_138.x, (__int64)pSprite->destWHZ_138.y);
      *pDeltaX = (__int64)pSprite->destWHZ_138.x;
      *pCursorX = *pDeltaX;
      *pDeltaY = (__int64)pSprite->destWHZ_138.y;
      *pCursorY = *pDeltaY;
    }
  }
}

BOOL __cdecl hook_Scene_ProcessPracticeFrames(CHanaSceneT458_S504 *pScene)
{
  unsigned int i; // [esp+0h] [ebp-8h]
  BOOL bOk; // [esp+4h] [ebp-4h]
  ONCELOG("[HNH:SUB] Scene_ProcessPracticeFrames_4668AC\n");
  bOk = 0;
  for ( i = 0; i < pScene->dwCount_130; ++i )
  {
    if ( !_memicmp(pScene->refsFrames_134[i]->szName_8, "h\\h", 3u) )
    {
      if ( !pScene->refsFrames_134[i]->field_11C )
      {
        pScene->refsFrames_134[i]->field_11C = 1;
        InitializePracticeAnimations_4667A9(pScene->refsFrames_134[i]);
      }
      bOk |= Frame_CheckCameraDistanceAndRecurse(pScene->refsFrames_134[i]);
    }
  }
  return bOk;
}

// Camera behavior selector:
// * 1 = Track & Dolly -- orbit/dolly around target with distance-based speed
// * 2 = Offset LookAt -- position camera at offset from target, then look at target
// * 3 = Simple  Dolly -- move camera forward/back only
// * Other = Default Follow -- just maintain look-at with distance clamping
void __cdecl hook_CameraController_UpdateCutscene(
  CHanaFrameT3EC_S354 *pTargetFrame,  // Frame to track/look at
  CHanaFrameT3EC_S354 *pSceneFrameArg,// Scene root frame (unused, global g_pSceneFrame used instead)
  int panSpeedX,                          // Horizontal pan speed multiplier
  int panSpeedY,                          // Vertical pan speed multiplier
  __int16 cameraMode)                     // Camera behavior mode (1, 2, or 3)
{
  double v5; // st7
  float x; // [esp+0h] [ebp-3Ch]
  float moveX; // [esp+0h] [ebp-3Ch]
  float moveX2; // [esp+0h] [ebp-3Ch]
  float moveX3; // [esp+0h] [ebp-3Ch]
  float panX2; // [esp+0h] [ebp-3Ch]
  float moveX4; // [esp+0h] [ebp-3Ch]
  float moveX5; // [esp+0h] [ebp-3Ch]
  float panY; // [esp+4h] [ebp-38h]
  float X; // [esp+4h] [ebp-38h]
  float moveY; // [esp+4h] [ebp-38h]
  float Xa; // [esp+4h] [ebp-38h]
  float moveY2; // [esp+4h] [ebp-38h]
  float Xb; // [esp+4h] [ebp-38h]
  float moveZ; // [esp+8h] [ebp-34h]
  float X_4; // [esp+8h] [ebp-34h]
  float moveZ2; // [esp+8h] [ebp-34h]
  float X_4a; // [esp+8h] [ebp-34h]
  float panZ; // [esp+8h] [ebp-34h]
  float a3; // [esp+18h] [ebp-24h] BYREF
  float distXZ; // [esp+1Ch] [ebp-20h]
  float distXYZ; // [esp+20h] [ebp-1Ch]
  float z; // [esp+24h] [ebp-18h] BYREF
  float distXYZ_prev; // [esp+28h] [ebp-14h]
  float relPosY2; // [esp+2Ch] [ebp-10h] BYREF
  float relPosX2; // [esp+30h] [ebp-Ch] BYREF
  float a5; // [esp+34h] [ebp-8h] BYREF
  float a4; // [esp+38h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] CameraController_UpdateCutscene_466BDA\n");

  // 1. Initialize camera basis: forward=(0,0,1), up=(0,1,0) relative to target
  UpdateBoneLookAtTransform(
    g_pCameraFrame,
    (CHanaFrameT3EC_S354 *)pTargetFrame,
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0);
  CameraLookAtFrame(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);// 2. Make camera look at target (mode -1 = use camera's local up)
  if ( cameraMode == 1 )                        // Mode 1: Track & Dolly
  {
    HanaFrame_GetRelPosition(g_pCameraFrame, pTargetFrame, &a3, &a4, &a5);// Get initial relative position
    distXZ = sqrt(a3 * a3 + a5 * a5) / 150.0;   // Horizontal distance scaled by 1/150
    panY = (double)-panSpeedY * distXZ;         // Pan camera horizontally based on distance
    x = (double)panSpeedX * distXZ;
    SetFramePosRel_4069DD(g_pCameraFrame, g_pCameraFrame, x, panY, 0.0);
    HanaFrame_GetRelPosition(g_pCameraFrame, pTargetFrame, &relPosX2, &relPosY2, &z);// Get new position after pan
    distXYZ_prev = sqrt(a3 * a3 + a4 * a4 + a5 * a5);
    distXYZ = sqrt(relPosX2 * relPosX2 + relPosY2 * relPosY2 + z * z);
    if ( distXYZ != 0.0 )                       // Dolly correction: move camera toward/away to maintain original distance
    {
      moveZ = z * distXYZ_prev / distXYZ;
      X = relPosY2 * distXYZ_prev / distXYZ;
      moveX = relPosX2 * distXYZ_prev / distXYZ;
      SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, moveX, X, moveZ);
    }
    HanaFrame_GetRelPosition(g_pCameraFrame, pTargetFrame, &relPosX2, &relPosY2, &z);// Anti-jitter: if too close (<2) or moved further away, push back
    distXZ = sqrt(relPosX2 * relPosX2 + z * z);
    if ( distXZ != 0.0 )
    {
      if ( distXZ < 2.0 || (v5 = sqrt((relPosX2 - a3) * (relPosX2 - a3) + (z - a5) * (z - a5)), v5 > distXZ) )
      {
        X_4 = (z + z) / distXZ;
        moveX2 = (relPosX2 + relPosX2) / distXZ;
        SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, moveX2, a4, X_4);
      }
    }
  }
  if ( cameraMode == 3 )                        // Mode 3: Simple Dolly
  {
    moveZ2 = -(double)panSpeedY / 20.0;         // Just move camera forward/back along Z
    SetFramePosRel_4069DD(g_pCameraFrame, g_pCameraFrame, 0.0, 0.0, moveZ2);
  }
  if ( cameraMode == 2 )                        // Mode 2: Offset LookAt
  {
    CameraLookAtFrame(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);// Re-establish look-at
    HanaFrame_GetRelPosition((CHanaFrameT3EC_S354 *)pTargetFrame, g_pCameraFrame, &relPosX2, &relPosY2, &z);//  Get target-relative camera position
    moveY = relPosY2 - (double)panSpeedY / 80.0;// Position camera at offset from target
    moveX3 = (double)panSpeedX / 80.0 + relPosX2;
    SetFramePosRel_4069DD((CHanaFrameT3EC_S354 *)pTargetFrame, g_pCameraFrame, moveX3, moveY, z);
    Xa = -(double)panSpeedY / 80.0;             // Small camera self-pan
    panX2 = (double)panSpeedX / 80.0;
    SetFramePosRel_4069DD(g_pCameraFrame, g_pCameraFrame, panX2, Xa, 0.0);
  }                                             // All Modes: Distance Clamping
  HanaFrame_GetRelPosition(g_pCameraFrame, pTargetFrame, &a3, &a4, &a5);
  distXYZ = sqrt(a3 * a3 + a4 * a4 + a5 * a5);
  if ( distXYZ <= 30.0 || distXYZ == 0.0 )      // Clamp distance to [2.0, 30.0] range
  {
    if ( distXYZ < 2.0 && distXYZ != 0.0 )      // Too close: push away along normalized vector
    {
      panZ = (a5 + a5) / distXYZ;
      Xb = (a4 + a4) / distXYZ;
      moveX5 = (a3 + a3) / distXYZ;
      SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, moveX5, Xb, panZ);
    }
  }
  else
  {                                             // Too far (>30): pull to exactly 30 units
    X_4a = a5 * 30.0 / distXYZ;
    moveY2 = a4 * 30.0 / distXYZ;
    moveX4 = a3 * 30.0 / distXYZ;
    SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, moveX4, moveY2, X_4a);
  }
  UpdateBoneLookAtTransform(
    (CHanaFrameT3EC_S354 *)pTargetFrame,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0);                                       // Update target's bone look-at transform
  CameraLookAtFrame(g_pCameraFrame, (CHanaFrameT3EC_S354 *)pTargetFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, -1);
}

// CameraFrame_ProjectFramePairOntoForwardRay positions a target frame at the point where the line between two frames
// (FrameA, FrameB) intersects the camera's forward view ray in camera space. The X/Z position is the intersection with
// the camera's central ray (X=0, Z from 0 to -10); the Y position is the average of the two frames' Y coordinates. The
// resulting transform is computed via SetFramePosRel_4069DD which builds a translation matrix, multiplies by the camera's world
// matrix, and applies it to the target frame's matWorld_178.
// 
// This is likely used for UI/camera effects where an object (target frame) should appear centered between two 3D objects
// from the camera's perspective.
void __cdecl hook_CameraFrame_ProjectFramePairOntoForwardRay(
  CHanaFrameT3EC_S354 *pTargetFrame,  // [in/out] Target frame root to receive the computed matrix
  CHanaFrameT3EC_S354 *pFrameA,           // [in] First frame (defines line segment with pFrameB)
  CHanaFrameT3EC_S354 *pFrameB)           // [in] Second frame (defines line segment with pFrameA)
{
  float avgX; // [esp+0h] [ebp-44h]
  float frameB_posY; // [esp+4h] [ebp-40h] BYREF
  float intersectX; // [esp+8h] [ebp-3Ch]
  float frameA_posY; // [esp+Ch] [ebp-38h] BYREF
  int camPosZ_1; // [esp+10h] [ebp-34h] BYREF
  float camPosY_1; // [esp+14h] [ebp-30h] BYREF
  float camPosX_1; // [esp+18h] [ebp-2Ch] BYREF
  float frameB_posX; // [esp+1Ch] [ebp-28h] BYREF
  float frameA_posX; // [esp+20h] [ebp-24h] BYREF
  float intersectZ; // [esp+24h] [ebp-20h]
  float avgY; // [esp+28h] [ebp-1Ch]
  int camPosZ_2; // [esp+2Ch] [ebp-18h] BYREF
  float camPosY_2; // [esp+30h] [ebp-14h] BYREF
  float t; // [esp+34h] [ebp-10h]
  float camPosX_2; // [esp+38h] [ebp-Ch] BYREF
  int frameB_posZ; // [esp+3Ch] [ebp-8h] BYREF
  int frameA_posZ; // [esp+40h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] CameraFrame_ProjectFramePairOntoForwardRay_46A55D\n");
  
  HanaFrame_GetRelPosition(g_pCameraFrame, g_pCameraFrame, &camPosX_1, &camPosY_1, (float *)&camPosZ_1);// // ---- Get camera position in its own space (always 0,0,0) ----
  HanaFrame_GetRelPosition(g_pCameraFrame, g_pCameraFrame, &camPosX_2, &camPosY_2, (float *)&camPosZ_2);
  *(float *)&camPosZ_2 = *(float *)&camPosZ_2 - 10.0;// ---- Get camera position again, then offset Z by -10 (forward ray endpoint) ----
  HanaFrame_GetRelPosition(pFrameA, g_pCameraFrame, &frameA_posX, &frameA_posY, (float *)&frameA_posZ);// ---- Get FrameA and FrameB positions in camera space ----
  HanaFrame_GetRelPosition(pFrameB, g_pCameraFrame, &frameB_posX, &frameB_posY, (float *)&frameB_posZ);
  avgX = frameA_posX / 2.0 + frameB_posX / 2.0;
  intersectX = avgX;
  avgY = frameA_posY / 2.0 + frameB_posY / 2.0;
  intersectZ = *(float *)&frameA_posZ / 2.0 + *(float *)&frameB_posZ / 2.0;
  if ( (frameB_posX - frameA_posX) * (*(float *)&camPosZ_2 - *(float *)&camPosZ_1)
    - (*(float *)&frameB_posZ - *(float *)&frameA_posZ) * (camPosX_2 - camPosX_1) != 0.0 )
{                                             
    // Geometric Interpretation
    // 
    // Camera Space (view space):
    //                         Camera at origin (0,0,0)
    //                               |
    //                               |  Forward ray (10 units along -Z)
    //                               v
    //                         (0, 0, -10) <- camPosZ_2 after adjustment
    // 
    //     FrameA  *-------------------*  FrameB
    //              \                 /
    //               \   Line A-B    /
    //                \             /
    //                 \           /
    //                  \         /
    //                   \       /
    //                    \     /
    //                     \   /
    //                      \ /
    //                       *  Intersection (X~~0, Z=depth)
    //                       |
    //                       |  (Y = average of FrameA.Y and FrameB.Y)
    //                       v
    //               Target frame positioned here
    // 
    // - XZ intersection: Finds where the line between FrameA and FrameB crosses the camera's
    //   central view ray (X=0 line). This is the "screen-center" depth.
    // - XY intersection: Degenerate (camera forward ray has no Y extent), so falls back to midpoint Y.
    // - Result: Target frame is placed at the screen-center depth of the FrameA-FrameB segment, at their average height.
    t = ((*(float *)&frameA_posZ - camPosY_1) * (camPosX_2 - camPosX_1)
      - (frameA_posX - camPosX_1) * (*(float *)&camPosZ_2 - *(float *)&camPosZ_1))
      / ((frameB_posX - frameA_posX) * (*(float *)&camPosZ_2 - *(float *)&camPosZ_1)
        - (*(float *)&frameB_posZ - *(float *)&frameA_posZ) * (camPosX_2 - camPosX_1));
    intersectX = (frameB_posX - frameA_posX) * t + frameA_posX;
    intersectZ = (*(float *)&frameB_posZ - *(float *)&frameA_posZ) * t + *(float *)&frameA_posZ;
  }
  if ( (frameB_posX - frameA_posX) * (camPosY_2 - camPosY_1) - (frameB_posY - frameA_posY) * (camPosX_2 - camPosX_1) != 0.0 )
  {
    t = ((frameA_posY - camPosY_1) * (camPosX_2 - camPosX_1) - (frameA_posX - camPosX_1) * (camPosY_2 - camPosY_1))
      / ((frameB_posX - frameA_posX) * (camPosY_2 - camPosY_1) - (frameB_posY - frameA_posY) * (camPosX_2 - camPosX_1));
    avgY = (frameB_posY - frameA_posY) * t + frameA_posY;
  }
  SetFramePosRel_4069DD(pTargetFrame, g_pCameraFrame, intersectX, avgY, intersectZ);
}

CHanaFrameT3EC_S354 *__cdecl hook_FindFrameByNameSubstringRecursive(CHanaFrameT3EC_S354 *pFrame, LPCSTR pSearchStr)
{
  int searchLen; // [esp+0h] [ebp-1Ch]
  int i; // [esp+4h] [ebp-18h]
  CHanaFrameT3EC_S354 *FrameByNameSubstringRecursive; // [esp+8h] [ebp-14h]
  unsigned int childIdx; // [esp+Ch] [ebp-10h]
  int frameNameLen; // [esp+14h] [ebp-8h]
  CHanaNode *pChildNode; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindFrameByNameSubstringRecursive_47EA86\n");
  
  FrameByNameSubstringRecursive = 0;            // 1. Check CURRENT frame: must have attached object AND name contains substring
  if ( pFrame->pAttachedObj_20C )
  {
    searchLen = lstrlenA(pSearchStr);
    frameNameLen = lstrlenA(pFrame->szName_8);
    for ( i = 0; i <= frameNameLen - searchLen; ++i )// Slide substring search over frame name (case-insensitive)
    {
      if ( !_strnicmp(&pFrame->szName_8[i], pSearchStr, searchLen) )
        return pFrame;
    }
  }
  if ( pFrame->nCountChild )                    // 2. Recurse into children (depth-first, left-to-right)
  {
    pChildNode = pFrame->pFirstChildNode;
    for ( childIdx = 0; childIdx < pFrame->nCountChild; ++childIdx )
    {
      if ( pChildNode->pData )                  // // Child frame exists
      {
        FrameByNameSubstringRecursive = FindFrameByNameSubstringRecursive(pChildNode->pData, pSearchStr);
        if ( FrameByNameSubstringRecursive )    // // Propagate first match up
          break;
      }
      pChildNode = pChildNode->pNext;           // Next sibling
    }
  }
  return FrameByNameSubstringRecursive;
}

void __cdecl hook_AnimSet_ResetAndTransferAnimations(
  CHanaAnimSetT3F1_S150 *pSrcAnimSet,
  CHanaAnimSetT3F1_S150 *pDstAnimSet,
  CHanaFrameT3EC_S354 *pRootFrame)
{
  ONCELOG("[HNH:SUB] AnimSet_ResetAndTransferAnimations_467ABE\n");

  SetAnimSetTime(pDstAnimSet, 0.0);             // 1. Reset destination animation set time to 0.0 (start)
  SetAnimSetTime(pSrcAnimSet, 0.0);             // 2. Reset source animation set time to 0.0 (start)
  TransferAnimSetFrameAnimations_4679B2(pSrcAnimSet, pDstAnimSet, pRootFrame);// 3. Transfer animations from source to destination for the frame hierarchy
  //    Recursively finds animations in pSrcAnimSet that target frames in pRootFrame's tree,
  //    moves them to pDstAnimSet (adds to dst, removes from src)
}

void __cdecl hook_TransferAnimSetFrameAnimations_4679B2(
  CHanaAnimSetT3F1_S150 *pSrcAnimSet,
  CHanaAnimSetT3F1_S150 *pDstAnimSet,
  CHanaFrameT3EC_S354 *pFrame)
{
  CHanaAnimT3F0_S240 *pHanaAnimT3F0; // [esp+4h] [ebp-Ch]
  unsigned int i; // [esp+8h] [ebp-8h]
  unsigned int j; // [esp+8h] [ebp-8h]
  CHanaNode *pFirstChildNode; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] TransferAnimSetFrameAnimations_4679B2\n");

  if ( pFrame )
  {
    for ( i = 0; i < pSrcAnimSet->nAnimCount_134; ++i )// Find animation in source set targeting THIS frame
    {
      if ( pSrcAnimSet->refsAnimT3F0_138[i]->pHanaFram_130 == pFrame )
      {
        pHanaAnimT3F0 = pSrcAnimSet->refsAnimT3F0_138[i];
        CHanaAnimSetT3F1__AddAnimT3F0_403C15(pDstAnimSet, pHanaAnimT3F0);// Add to destination set (increments refcount, bakes keyframes)
        CHanaAnimSetT3F1__RemoveAnimT3F0_404349(pSrcAnimSet, pHanaAnimT3F0);// Remove from source set
        break;
      }
    }
    if ( pFrame->nCountChild )                  // Recurse into children
    {
      pFirstChildNode = pFrame->pFirstChildNode;
      for ( j = 0; j < pFrame->nCountChild; ++j )
      {
        TransferAnimSetFrameAnimations_4679B2(pSrcAnimSet, pDstAnimSet, pFirstChildNode->pData);
        pFirstChildNode = pFirstChildNode->pNext;
      }
    }
  }
}

void __cdecl hook_DrawFadeTransition_Scene(
  int transitionState,
  CHanaSceneT458_S504 *pScene,
  CHanaSpriteT3F6_S1FC **arrMouseSprites,
  CHanaSpriteT3F6_S1FC **arrGOTypeSprites)
{
  double v4; // st7
  float fPosX; // [esp+4Ch] [ebp-20h]
  float fPosY; // [esp+50h] [ebp-1Ch]
  float fPosYa; // [esp+50h] [ebp-1Ch]
  float fPosYb; // [esp+50h] [ebp-1Ch]
  float fPosYc; // [esp+50h] [ebp-1Ch]
  int i; // [esp+58h] [ebp-14h]
  int j; // [esp+58h] [ebp-14h]
  float alpha; // [esp+5Ch] [ebp-10h]
  float FrameTime; // [esp+60h] [ebp-Ch]
  DWORD dwMouseY; // [esp+64h] [ebp-8h] BYREF
  DWORD dwMouseX; // [esp+68h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] DrawFadeTransition_Scene_468DA8\n");
  
  for ( i = 0; i < 2; ++i )
  {
    if ( arrGOTypeSprites[i] )
    {
      fPosY = (double)(i + 1) * 48.0 - 16.0 - 16.0;
      fPosYa = ConvertFrom640AsFloat(fPosY);
      fPosX = ConvertFrom640AsFloat(608.0);
      CHanaSpriteT3F6__SetPos_414A5D(arrGOTypeSprites[i], fPosX, fPosYa);
    }
  }
  if ( transitionState )
    alpha = 1.0;
  else
    alpha = 0.0;
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  while ( 1 )
  {
    FrameTime = GetFrameTime();
    if ( FrameTime > 5.0 )
      FrameTime = 5.0;
    FrameTimer_EndFrame();
    v4 = transitionState ? alpha - FrameTime / 10.0 : FrameTime / 10.0 + alpha;
    alpha = v4;
    GetMouseXY_45A4D8(&dwMouseX, &dwMouseY);
    fPosYb = 1.0 - alpha;
    Scene_ApplyFadeToPracticeFrames(fPosYb);
    if ( alpha < 0.0 || alpha > 1.0 )
      break;
    MtxStackStartMultLocal_4074EF();
    g_pRenderCtx->Clear(3);
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
      HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      HanaVert_DrawBatch(1);
      g_pD3DDevice_4D6C78->EndScene();
    }
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
      for ( j = 0; j < 2; ++j )
      {
        if ( arrGOTypeSprites[j] )
        {
          CHanaObj__SetAlpha3F6(arrGOTypeSprites[j], alpha);
          DrawSprite2D(arrGOTypeSprites[j]);
        }
      }
      while ( j < 8 )
      {
        if ( arrGOTypeSprites[j] )
        {
          fPosYc = 1.0 - alpha;
          CHanaObj__SetAlpha3F6(arrGOTypeSprites[j], fPosYc);
          DrawSprite2D(arrGOTypeSprites[j]);
        }
        ++j;
      }
      if ( arrMouseSprites[5] )
        DrawSprite2D(arrMouseSprites[5]);
      g_pD3DDevice_4D6C78->EndScene();
    }

    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
}

// Applies a fade/alpha effect to specific "practice mode" frames in the scene
// hierarchy. Used during fade transitions to show/hide practice-related visual elements based on the current effect
// level.
//   
// Returns: The "_d00_" frame if found, NULL otherwise
void __cdecl hook_Scene_ApplyFadeToPracticeFrames(float fadeAlpha)
{
  float alpha; // [esp+0h] [ebp-C0h]
  //float effectAlphaMultipliers[5]; // [esp+8h] [ebp-B8h]
  CHanaFrameT3EC_S354 *pFoundFrame; // [esp+1Ch] [ebp-A4h]
  CHanaFrameT3EC_S354 *pD00Frame; // [esp+20h] [ebp-A0h]
  int i; // [esp+24h] [ebp-9Ch]
  //String 50 szArrFrameNames[3]; // [esp+28h] [ebp-98h] BYREF

  ONCELOG("[HNH:SUB] Scene_ApplyFadeToPracticeFrames_468B99\n");

#if 1
  char szArrFrameNames[3][50] = {
    "_h00_",
    "_p00_",
    ""
  };
#else
  strcpy((char *)szArrFrameNames[0].str, "_h00_");// Prepare name templates for _h00_ frames (2 slots: indices 0 and 1)
  // Each slot is 25 chars: "_h00_" + padding
  memset((void *)&szArrFrameNames[0].str[6], 0, 0x2Cu);
  strcpy((char *)szArrFrameNames[1].str, "_p00_");
  memset((void *)&szArrFrameNames[1].str[6], 0, 0x2Cu);// Prepare _p00_ name template (not used in loop but prepared)
  szArrFrameNames[2].str[0] = 0;
  memset((void *)&szArrFrameNames[2].str[1], 0, 0x30u);
  szArrFrameNames[2].str[49] = 0;
#endif

  float effectAlphaMultipliers[5] = {     // Effect level alpha multipliers: [0]=0.0, [1]=0.8, [2]=0.5, [3]=0.2, [4]=0.0
    0.0,
    0.80000001,
    0.5,
    0.2,
    0.0,
  };

  pFoundFrame = 0;
  pD00Frame = 0;

  if ( fadeAlpha < 0.0 )                        // Clamp input alpha to valid range
    fadeAlpha = 0.0;
  if ( fadeAlpha > 1.0 )
    fadeAlpha = 1.0;
  for ( i = 0; i < 2; ++i )                     // Process TWO _h00_ frames: _h00_ (slot 0) and _h25_ (slot 1)
  {
    pFoundFrame = FindFrameByName_47E999((CHanaFrameT3EC_S354 *)g_pSceneFrame, szArrFrameNames[i]);
    if ( pFoundFrame )
    {
      if ( g_nHSceneMale_530265 )              // Effect level 1-4: force fully transparent, mark as "effect active"
      {
        Frame_ApplyFadeRecursive(pFoundFrame, 0.0);
        pFoundFrame->bAlphaDeferred_130 = 1;    // Flag: under effect control
      }
      else                                      // Effect level 0: normal fade with given alpha
      {
        Frame_ApplyFadeRecursive(pFoundFrame, fadeAlpha);
        pFoundFrame->bAlphaDeferred_130 = 0;    //  Flag: normal fade
      }
    }
  }
  pD00Frame = FindFrameByName_47E999((CHanaFrameT3EC_S354 *)g_pSceneFrame, "_d00_");
  if ( pD00Frame )
  {
    pD00Frame->bAlphaDeferred_130 = !g_nHSceneMale_530265 || g_nHSceneMale_530265 == 4;// 1 for effect levels 1-3, 0 for level 0 or 4
    alpha = fadeAlpha * effectAlphaMultipliers[(unsigned __int8)g_nHSceneMale_530265];// Compute final alpha: fadeAlpha * multiplier[effectLevel]
    Frame_ApplyFadeRecursive(pD00Frame, alpha); // Apply fade recursively to _d00_ frame and children
  }
}

void register_PlayCutInScene_46A76F(int cfg) {
  if (cfg == 1) {
    PlayCutInScene_46A76F = hook_PlayCutInScene_46A76F;
    SetAnimSetTime = hook_SetAnimSetTime;
    DrawFadeTransition_Scene = hook_DrawFadeTransition_Scene;
    AnimSet_ResetAndTransferAnimations = hook_AnimSet_ResetAndTransferAnimations;
    FindFrameByNameSubstringRecursive = hook_FindFrameByNameSubstringRecursive;
    IsObjectValidRecursive_467DFD = hook_IsObjectValidRecursive_467DFD;
    Cursor_ClampToCenterOrSprite = hook_Cursor_ClampToCenterOrSprite;
    Frame_CheckCameraDistanceAndRecurse = hook_Frame_CheckCameraDistanceAndRecurse;
    UpdateSekBodyAnimationMatrix = hook_UpdateSekBodyAnimationMatrix;
    MoveCutInSceneFramesWithCameraConstraint = hook_MoveCutInSceneFramesWithCameraConstraint;
    UpdateCutInSceneCameraFollow = hook_UpdateCutInSceneCameraFollow;
    CameraFrame_TrackBetweenObjectsAndLookAt = hook_CameraFrame_TrackBetweenObjectsAndLookAt;
    SoundAH_ManageEffectsAndAnimate = hook_SoundAH_ManageEffectsAndAnimate;
    SoundSE_ManageEffectsAndPlay = hook_SoundSE_ManageEffectsAndPlay;
    Character_UpdateAnimationState = hook_Character_UpdateAnimationState;
    SyncObjectAnimationByName_467BBD = hook_SyncObjectAnimationByName_467BBD;
    IsParentValid_467D62 = hook_IsParentValid_467D62;
    HanaObj_CheckFrameCondition = hook_HanaObj_CheckFrameCondition;
    HanaMorp_AnimateAndPropagate = hook_HanaMorp_AnimateAndPropagate;
    HanaMatrix_ApplyTransform = hook_HanaMatrix_ApplyTransform;
    HanaFrame_GetMatAfter = hook_HanaFrame_GetMatAfter;
    UpdateFrameAnimationMatrix = hook_UpdateFrameAnimationMatrix;
    TransferAnimSetFrameAnimations_4679B2 = hook_TransferAnimSetFrameAnimations_4679B2;
    Scene_ApplyFadeToPracticeFrames = hook_Scene_ApplyFadeToPracticeFrames;
    GetSekBodyCurrentAnimTime = hook_GetSekBodyCurrentAnimTime;
    CameraFrame_ProjectFramePairOntoForwardRay = hook_CameraFrame_ProjectFramePairOntoForwardRay;
    CameraController_UpdateCutscene = hook_CameraController_UpdateCutscene;
    Scene_ProcessPracticeFrames = hook_Scene_ProcessPracticeFrames;
    UpdateCutInSceneBoneTransforms = hook_UpdateCutInSceneBoneTransforms;
    CutInScene_CyclePosAndApplyCameraOffset = hook_CutInScene_CyclePosAndApplyCameraOffset;
    AnimSet_MatchAndSortKutiRTibusaLFrames = hook_AnimSet_MatchAndSortKutiRTibusaLFrames;
  }

  hnh_HookOff();
  hnh_Hook(0x46A76F, hook_PlayCutInScene_46A76F, "PlayCutInScene_46A76F"); // [OK]

  hnh_Hook(0x467bbd, hook_SyncObjectAnimationByName_467BBD, "SyncObjectAnimationByName_467BBD"); // [OK]
  hnh_Hook(0x467d62, hook_IsParentValid_467D62, "IsParentValid_467D62");  // [OK]
  hnh_Hook(0x467dfd, hook_IsObjectValidRecursive_467DFD, "IsObjectValidRecursive_467DFD"); // [OK]
  hnh_Hook(0x4698e8, hook_SoundSE_ManageEffectsAndPlay, "SoundSE_ManageEffectsAndPlay_4698E8"); // [OK]
  hnh_Hook(0x467e72, hook_AnimSet_MatchAndSortKutiRTibusaLFrames, "AnimSet_MatchAndSortKutiRTibusaLFrames_467E72"); // [OK]
  hnh_Hook(0x401c67, hook_GetSekBodyCurrentAnimTime, "GetSekBodyCurrentAnimTime_401C67");  // [OK]
  hnh_Hook(0x404277, hook_SetAnimSetTime, "SetAnimSetTime_404277");  // [OK]
  hnh_Hook(0x4686f8, hook_UpdateCutInSceneCameraFollow, "UpdateCutInSceneCameraFollow_4686F8");  // [OK]
  hnh_Hook(0x469eb6, hook_Character_UpdateAnimationState, "Character_UpdateAnimationState_469EB6");  // [OK]
  hnh_Hook(0x48713f, hook_HanaObj_CheckFrameCondition, "HanaObj_CheckFrameCondition_48713F");  // [OK]
  hnh_Hook(0x4690ab, hook_SoundAH_ManageEffectsAndAnimate, "SoundAH_ManageEffectsAndAnimate_4690AB");  // [OK]
  hnh_Hook(0x481be0, hook_HanaMorp_AnimateAndPropagate, "HanaMorp_AnimateAndPropagate_481BE0");  // [OK]
  hnh_Hook(0x466978, hook_CameraFrame_TrackBetweenObjectsAndLookAt, "CameraFrame_TrackBetweenObjectsAndLookAt_466978");  // [OK]
  hnh_Hook(0x40799f, hook_HanaMatrix_ApplyTransform, "HanaMatrix_ApplyTransform_40799F");  // [OK]
  hnh_Hook(0x467160, hook_CutInScene_CyclePosAndApplyCameraOffset, "CutInScene_CyclePosAndApplyCameraOffset_467160");  // [OK]
  hnh_Hook(0x407945, hook_HanaFrame_GetMatAfter, "HanaFrame_GetMatAfter_407945");  // [OK]
  hnh_Hook(0x4682d6, hook_MoveCutInSceneFramesWithCameraConstraint, "MoveCutInSceneFramesWithCameraConstraint_4682D6");  // [OK]
  hnh_Hook(0x4680b4, hook_UpdateCutInSceneBoneTransforms, "UpdateCutInSceneBoneTransforms_4680B4");  // [OK]
  hnh_Hook(0x46a503, hook_UpdateSekBodyAnimationMatrix, "UpdateSekBodyAnimationMatrix_46A503");  // [OK]
  hnh_Hook(0x46a3e9, hook_UpdateFrameAnimationMatrix, "UpdateFrameAnimationMatrix_46A3E9");  // [OK]
  hnh_Hook(0x4665f0, hook_Frame_CheckCameraDistanceAndRecurse, "Frame_CheckCameraDistanceAndRecurse_4665F0");  // [OK]
  hnh_Hook(0x468a4a, hook_Cursor_ClampToCenterOrSprite, "Cursor_ClampToCenterOrSprite_468A4A");  // [OK]
  hnh_Hook(0x4668ac, hook_Scene_ProcessPracticeFrames, "Scene_ProcessPracticeFrames_4668AC");  // [OK]
  hnh_Hook(0x466bda, hook_CameraController_UpdateCutscene, "CameraController_UpdateCutscene_466BDA");  // [OK]
  hnh_Hook(0x46a55d, hook_CameraFrame_ProjectFramePairOntoForwardRay, "CameraFrame_ProjectFramePairOntoForwardRay_46A55D");  // [OK]
  hnh_Hook(0x47ea86, hook_FindFrameByNameSubstringRecursive, "FindFrameByNameSubstringRecursive_47EA86");  // [OK]
  hnh_Hook(0x467abe, hook_AnimSet_ResetAndTransferAnimations, "AnimSet_ResetAndTransferAnimations_467ABE");  // [OK]
  hnh_Hook(0x4679b2, hook_TransferAnimSetFrameAnimations_4679B2, "TransferAnimSetFrameAnimations_4679B2");  // [OK]
  hnh_Hook(0x468da8, hook_DrawFadeTransition_Scene, "DrawFadeTransition_Scene_468DA8");  // [OK]
  hnh_Hook(0x468b99, hook_Scene_ApplyFadeToPracticeFrames, "Scene_ApplyFadeToPracticeFrames_468B99");  // [OK]
  hnh_HookOn();
}
