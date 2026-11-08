// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===

void (__cdecl*PlayTugOfWar_45460A)(CHanaSceneT458_S504 *pOdsScene, 
  CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame, 
  CHanaSpriteT3F6_S1FC **pArrHudMeters, CHanaSpriteT3F6_S1FC **pArrSprites, 
  float *pfHeroHpCur, float fHeroHpMax, float *pfFoeHpCur, float fFoeHpMax, 
  int *pnClockBase, unsigned int *pdwElapsedMs, int dwClockBase2, 
  CSoundStruct_8 **pArrVoiceBufs, CHanaSpriteT3F6_S1FC *pMaskBmp) =
  reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
    CSekBody *, CHanaFrameT3EC_S354 *, CHanaSpriteT3F6_S1FC **,
    CHanaSpriteT3F6_S1FC **, float *, float, float *, float, int *, unsigned int *,
    int, CSoundStruct_8 **, CHanaSpriteT3F6_S1FC *)>(0x45460A);

void (__cdecl *DrawCountdownFlash_4532D0)(CHanaSpriteT3F6_S1FC *, float)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC *, float)>(0x4532d0);
double (__cdecl *UpdateBodiesInput_4542E0)(CSekBody *pBodyA, CSekBody *pBodyB, float fBaseDelta, int bInputDrive, __int16 *pnLastX, __int16 *pnLastY)
= reinterpret_cast<double (__cdecl*)(CSekBody *pBodyA, CSekBody *pBodyB, float fBaseDelta, int bInputDrive, __int16 *pnLastX, __int16 *pnLastY)>(0x4542e0);
void (__cdecl*DrawClockHud_44AF5B)(CHanaSpriteT3F6_S1FC **arrSprNumbers, int nX, int nY,
  int nTimeLeft) =
  reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, int, int)>(0x44AF5B);
void (__cdecl *DrawTransitionFlash_453FAC)(CHanaSpriteT3F6_S1FC *pSprites[], unsigned int dwElapsed, float fProgress)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC *pSprites[], unsigned int dwElapsed, float fProgress)>(0x453fac);

void (__cdecl *DrawHudSpriteNumber_44B97B)(CHanaSpriteT3F6_S1FC **pDigitSprites, int xScreen, int yScreen, int nValue)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC **pDigitSprites, int xScreen, int yScreen, int nValue)>(0x44b97b);

// PlayTugOfWar(pCtrl, pBodyA, pBodyB, pCamFrame, a5..a14, pHudSprite)
// Timed cinematic/cutscene player (~3.75s budget, v43=3750ms). Renders two CSekBody models
// under an animated camera (g_pCameraFrame + cube-map face matrices) with a countdown HUD.
// Per frame: bone/anim update (sub_458B3A), body render (sub_4542E0), camera follow,
// sound pos (sub_44B23B), HUD text (sub_44AF5B), progress bar (sub_4532D0), and the
// transition flash (DrawTransitionFlash_453FAC). Loops until elapsed>3750ms, the pCtrl
// timeline (a11/derived v48) reaches 0, or a pause-timeout; returns 1.
/*
void (__cdecl*PlayTugOfWar_45460A)(CHanaSceneT458_S504 *pOdsScene, 
CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame, 
CHanaSpriteT3F6_S1FC **pArrHudMeters, CHanaSpriteT3F6_S1FC **pArrSprites, 
float *pfHeroHpCur, float fHeroHpMax, float *pfFoeHpCur, float fFoeHpMax, 
int *pnClockBase, unsigned int *pdwElapsedMs, int dwClockBase2, 
CSoundStruct_8 **pArrVoiceBufs, CHanaSpriteT3F6_S1FC *pMaskBmp)
*/
void __cdecl hook_PlayTugOfWar_45460A(
  CHanaSceneT458_S504 *pOdsScene,
  CSekBody *pHeroBody,
  CSekBody *pFoeBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  CHanaSpriteT3F6_S1FC **pArrHudMeters,
  CHanaSpriteT3F6_S1FC **pArrSprites,
  float *pfHeroHpCur,
  float fHeroHpMax,
  float *pfFoeHpCur,
  float fFoeHpMax,
  int *nClockBase,
  DWORD *dwElapsedMs,
  int dwClockBase2,
  CSoundStruct_8 **pArrVoiceBufs,
  CHanaSpriteT3F6_S1FC *pHudSprite)
{
  double updated; // st7
  float fFoeYOff; // [esp+4Ch] [ebp-98h]
  float fPivotYLow; // [esp+4Ch] [ebp-98h]
  float fPivotYHigh; // [esp+4Ch] [ebp-98h]
  float fFlashX1; // [esp+4Ch] [ebp-98h]
  float fFlashX2; // [esp+4Ch] [ebp-98h]
  float fFoeZOff; // [esp+50h] [ebp-94h]
  float fFovZoom; // [esp+50h] [ebp-94h]
  float fRotPitch; // [esp+50h] [ebp-94h]
  float fRotYaw; // [esp+50h] [ebp-94h]
  float fFlashY1; // [esp+50h] [ebp-94h]
  float fFlashY2; // [esp+50h] [ebp-94h]
  float fFlashScale; // [esp+50h] [ebp-94h]
  float fFlashAlpha; // [esp+50h] [ebp-94h]
  float fProgress; // [esp+50h] [ebp-94h]
  float fElapsedF; // [esp+50h] [ebp-94h]
  float fFlashPulse; // [esp+68h] [ebp-7Ch]
  CHanaFrameT3EC_S354 *pFoeFaceFrame; // [esp+74h] [ebp-70h]
  float fHeroHpSave; // [esp+78h] [ebp-6Ch]
  int nClockBaseSave; // [esp+7Ch] [ebp-68h]
  float pfExecTime; // [esp+80h] [ebp-64h] BYREF
  float z; // [esp+84h] [ebp-60h] BYREF
  float y; // [esp+88h] [ebp-5Ch] BYREF
  CHanaFrameT3EC_S354 *pCamPivotFrame; // [esp+8Ch] [ebp-58h] BYREF
  float fPivotX; // [esp+90h] [ebp-54h] BYREF
  unsigned int nSeqBudgetMs; // [esp+94h] [ebp-50h]
  float fHeroZ; // [esp+98h] [ebp-4Ch] BYREF
  float fHeroY; // [esp+9Ch] [ebp-48h] BYREF
  float fFoeZ; // [esp+A0h] [ebp-44h] BYREF
  int bRotating; // [esp+A4h] [ebp-40h]
  float fFoeHpSave; // [esp+A8h] [ebp-3Ch]
  int lastY; // [esp+ACh] [ebp-38h] BYREF
  float fHeroX; // [esp+B0h] [ebp-34h] BYREF
  float fFoeY; // [esp+B4h] [ebp-30h] BYREF
  float fFoeX; // [esp+B8h] [ebp-2Ch] BYREF
  float fFovSaved; // [esp+BCh] [ebp-28h]
  CHanaFrameT3EC_S354 *pHeroHeadFrame; // [esp+C0h] [ebp-24h]
  int lastX; // [esp+C4h] [ebp-20h] BYREF
  DWORD Time; // [esp+C8h] [ebp-1Ch]
  unsigned int dwSeqElapsed; // [esp+CCh] [ebp-18h]
  float fDelta; // [esp+D0h] [ebp-14h]
  D3DXVECTOR3 pV; // [esp+D4h] [ebp-10h] BYREF
  DWORD dwElapsedNow; // [esp+E0h] [ebp-4h]

  ONCELOG("[HNH:SUB] PlayTugOfWar_45460A\n");

  fHeroHpSave = *pfHeroHpCur;
  fFoeHpSave = *pfFoeHpCur;
  nClockBaseSave = *nClockBase;
  dwElapsedNow = *dwElapsedMs;
  pfExecTime = 0.0;
  LOWORD_(lastX) = ConvertFrom640AsInt(320);
  LOWORD_(lastY) = ConvertFrom640AsInt(240);
  nSeqBudgetMs = 3750;
  pCamPivotFrame = 0;
  bRotating = 0;
  pHeroHeadFrame = 0;
  HanaFrame_GetRelPosition(pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fFoeX, &fFoeY, &fFoeZ);
  HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
  SekBody__UpdateTo2_401647(pHeroBody, 17);
  SekBody__UpdateTo2_401647(pFoeBody, 7);
  pFoeFaceFrame = FindFrameByName_47E999(pFoeBody->pSekHeader_220->pFrameT3EC, "_kao_");
  pHeroHeadFrame = FindFrameByName_47E999(pHeroBody->pSekHeader_220->pFrameT3EC, "head");
  pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
  CHanaObj__NewFrameT3EC_S354(pHeroBody->pSekHeader_220->pFrameT3EC, &pCamPivotFrame);
  SetFramePosRel_4069DD(pCamPivotFrame, pHeroBody->pSekHeader_220->pFrameT3EC, 0.0, 0.0, -5.0);
  AttachChild_4055FD(pCamPivotFrame, g_pCameraFrame);
  SetFramePosRel_4069DD(g_pCameraFrame, pCamPivotFrame, 0.0, 13.0, 8.0);
  pV.x = 0.0;
  pV.y = 1.0;
  pV.z = 0.0;
  Frame_RotateMatrixAxis_4070AB(pCamPivotFrame, 1, &pV, 2.355);
  MtxStackStartMultLocal_4074EF();
  HanaFrame_GetRelPosition(pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fPivotX, &y, &z);
  fFoeZOff = fHeroZ - 3.0;
  fFoeYOff = fHeroY + 14.0;
  SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fFoeYOff, fFoeZOff);
  HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, 0);
  SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPivotX, y, z);
  SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, pHeroBody->pSekHeader_220->pFrameT3EC, 0.0, 0.0, -5.0);
  HanaMatrix_BuildLookAtAndTransform(
    pFoeBody->pSekHeader_220->pFrameT3EC,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    g_pSceneFrame,
    0);
  Frame_RotateMatrixAxis_4070AB(pFoeBody->pSekHeader_220->pFrameT3EC, 1, &pV, 3.1400001);
  fFovSaved = GetProjectionFov_40DCF6();
  MtxStackStartMultLocal_4074EF();
  g_dwBattleMode = -1;
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  Time = timeGetTime();
  while ( 1 )
  {
    DoPeekMessages();
    dwElapsedNow = timeGetTime() - dwClockBase2;
    dwSeqElapsed = timeGetTime() - Time;
    fDelta = GetFrameTime();
    if ( fDelta > 5.0 )
      fDelta = 5.0;
    FrameTimer_EndFrame();
    if ( (int)(nSeqBudgetMs - dwSeqElapsed) <= 3000 )
    {
      UpdateVoicePlayback_458B3A(pFoeBody, pFoeFaceFrame, pArrVoiceBufs, fDelta);
      SetProjectionFov_40DCA3(0.5);
      if ( bRotating )
      {
        pV.x = 1.0;
        pV.y = 0.0;
        pV.z = 0.0;
        fRotPitch = -fDelta / 180.0 * 3.1400001 / 5.0;
        Frame_RotateMatrixAxis_4070AB(pCamPivotFrame, 1, &pV, fRotPitch);
        pV.x = 0.0;
        pV.y = 1.0;
        pV.z = 0.0;
        fRotYaw = -fDelta / 180.0 * 3.1400001 / 5.0;
        Frame_RotateMatrixAxis_4070AB(pCamPivotFrame, 1, &pV, fRotYaw);
      }
      else
      {
        bRotating = 1;
        Frame_RotateMatrixAxis_4070AB(pCamPivotFrame, 1, &pV, -2.9655557);
      }
      HanaFrame_GetRelPosition(pCamPivotFrame, g_pSceneFrame, &fPivotX, &y, &z);
      fPivotYLow = y - 16.0;
      SetFramePosRel_4069DD(pCamPivotFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPivotX, fPivotYLow, z);
      UpdateCameraFollowChase_445525(pCamPivotFrame, g_pCameraFrame, pMapFrame, fDelta, 1);
      fPivotYHigh = y + 12.0;
      SetFramePosRel_4069DD(pCamPivotFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPivotX, fPivotYHigh, z);
      if ( pFoeFaceFrame )
        HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeFaceFrame, g_pSceneFrame, 0);
      else
        HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pCamPivotFrame, g_pSceneFrame, 0);
      SetFramePosRel_4069DD(pCamPivotFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPivotX, y, z);
      updated = UpdateBodiesInput_4542E0(pHeroBody, pFoeBody, fDelta, 1u, (__int16 *)&lastX, (__int16 *)&lastY);
      fFoeHpSave = fFoeHpSave - updated;
    }
    else
    {
      UpdateVoicePlayback_458B3A(pFoeBody, pFoeFaceFrame, pArrVoiceBufs, fDelta);
      if ( nSeqBudgetMs - dwSeqElapsed > 0xC80 )
      {
        fFovZoom = ((double)(nSeqBudgetMs - dwSeqElapsed) - 3000.0) / 1000.0
          + ((double)(nSeqBudgetMs - dwSeqElapsed) - 3000.0) / 1000.0
          + 0.2;
        SetProjectionFov_40DCA3(fFovZoom);
      }
      if ( pHeroHeadFrame )
        HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pHeroHeadFrame, g_pSceneFrame, 0);
      UpdateBodiesInput_4542E0(pHeroBody, pFoeBody, fDelta, 0, (__int16 *)&lastX, (__int16 *)&lastY);
    }
    if ( pOdsScene )
    {
      UpdateSceneTimer_4827FE(pOdsScene, &pfExecTime, fDelta);
      UpdateSceneAtTime_4825D5(pOdsScene, &pfExecTime, fDelta);
      if ( pfExecTime >= (double)pOdsScene->pAnimSet_148->fRes_13C )
        pfExecTime = 0.0;
    }
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
    if ( pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 >= 1808.0
      && pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 <= 1813.0
      && pArrHudMeters[11]
      && pArrHudMeters[12] )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
      fFlashY1 = ConvertFrom640AsFloat(240.0);
      fFlashX1 = ConvertFrom640AsFloat(320.0);
      CHanaSpriteT3F6__SetPos_414A5D(pArrHudMeters[12], fFlashX1, fFlashY1);
      fFlashPulse = sin((pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 - 1808.0) / 5.0 * 3.1400001);
      CHanaObj__SetAlpha3F6(pArrHudMeters[12], fFlashPulse);
      CHanaSpriteT3F6__SetScale_414B09(pArrHudMeters[12], 20.0);
      DrawSprite2D(pArrHudMeters[12]);
      fFlashY2 = ConvertFrom640AsFloat(240.0);
      fFlashX2 = ConvertFrom640AsFloat(320.0);
      CHanaSpriteT3F6__SetPos_414A5D(pArrHudMeters[11], fFlashX2, fFlashY2);
      fFlashScale = (pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 - 1806.0) * 5.0 / 4.0;
      CHanaSpriteT3F6__SetScale_414B09(pArrHudMeters[11], fFlashScale);
      fFlashAlpha = (pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 - 1808.0) / 10.0 + 0.5;
      CHanaObj__SetAlpha3F6(pArrHudMeters[11], fFlashAlpha);
      DrawSprite2D(pArrHudMeters[11]);
    }
    DrawDuelHealthHud_44B23B(
      pArrSprites,
      (__int64)fHeroHpSave / 100,
      (__int64)fHeroHpMax / 100,
      (__int64)fFoeHpSave / 100,
      (__int64)fFoeHpMax / 100);
    DrawClockHud_44AF5B(pArrHudMeters, 320, 16, 100 * nClockBaseSave - dwElapsedNow / 0xA);
    fProgress = 1.0 - (double)(nSeqBudgetMs - dwSeqElapsed) / 3750.0;
    DrawCountdownFlash_4532D0(pHudSprite, fProgress);
    fElapsedF = (float)dwSeqElapsed;
    DrawTransitionFlash_453FAC(pArrSprites, nSeqBudgetMs - dwSeqElapsed, fElapsedF);
    if ( nSeqBudgetMs - dwSeqElapsed <= 0xBB8 )
      DrawHudSpriteNumber_44B97B(pArrHudMeters, 320, 448, (int)(nSeqBudgetMs - dwSeqElapsed) / 10);

    PresentFrame();
    HanaVert_DrawBatch(0);
    if ( dwSeqElapsed >= nSeqBudgetMs || 100 * nClockBaseSave == dwElapsedNow / 0xA || fFoeHpSave <= 0.0 )
      break;
    Sleep(0);
  }
  AttachChild_4055FD(g_pSceneFrame, g_pCameraFrame);
  if ( pCamPivotFrame )
  {
    CHanaObj__Free_40C1E4(pCamPivotFrame);
    pCamPivotFrame = 0;
  }
  SetProjectionFov_40DCA3(fFovSaved);
  *pfHeroHpCur = fHeroHpSave;
  *pfFoeHpCur = fFoeHpSave;
  *nClockBase = nClockBaseSave;
  *dwElapsedMs = dwElapsedNow;
  if ( 100 * nClockBaseSave != dwElapsedNow / 10 && fFoeHpSave > 0.0 )
  {
    SekBody__UpdateTo2_401647(pHeroBody, 9);
    SekBody__UpdateTo2_401647(pFoeBody, 20);
    CSekBody__UpdateAnim_4019E1(pFoeBody, 0.0);
  }
  if ( fFoeHpSave > 0.0 )
  {
    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
    SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fFoeX, fFoeY, fFoeZ);
  }
  UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, fDelta, 1);
  UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, fDelta, 0);
}

// Input-'struggle' pump for duel cinematics. Drains the shared 30-slot input-sample ring under
// the input mutex: consumes up to 20 active samples, accumulating fIntensity - 
// kbdMove_5 samples add per-sample mouse deltas (|dx|,|dy| each clamped to 9, /40);
// other samples add |cursor - screen center| per axis /1000 (hold the cursor away from center).
// Every consumed slot and the rest of the ring is reset to center (320,240) with all flags cleared;
// *pnLastX/Y are then re-read from slot 0, i.e. ALWAYS screen center afterwards (cross-frame deltas effectively measure
// from center - likely unintended). Then advances both bodies' anims by fBaseDelta + bInputDrive*min(2*fIntensity,1)
// (fBaseDelta = caller's frame delta; bInputDrive is a 0/1 GATE, not a scale - callers push 0 or 1);
// if an anim item finishes, bodyA replays item 17 and bodyB item 7 (the old comment's '1/7' was wrong).
// Returns fIntensity*350*bInputDrive - the time credit the caller subtracts from its cutscene timeline.
// Callers: PlayTugOfWar (bInputDrive 
double __cdecl hook_UpdateBodiesInput_4542E0(
  CSekBody *pBodyA,
  CSekBody *pBodyB,
  float fBaseDelta,
  int bInputDrive,
  __int16 *pnLastX,
  __int16 *pnLastY)
{
  float fDeltaA; // [esp+4h] [ebp-4Ch]
  float fDeltaB; // [esp+4h] [ebp-4Ch]
  float fIntensity; // [esp+2Ch] [ebp-24h]
  float fAccMove; // [esp+2Ch] [ebp-24h]
  float fAccDist; // [esp+2Ch] [ebp-24h]
  int iSample; // [esp+30h] [ebp-20h]
  CInputState *pInputRing; // [esp+34h] [ebp-1Ch]
  __int16 nCenterY; // [esp+38h] [ebp-18h]
  __int16 nCenterX; // [esp+3Ch] [ebp-14h]
  __int16 nPrevY; // [esp+40h] [ebp-10h]
  __int16 nPrevX; // [esp+44h] [ebp-Ch]
  float fDx; // [esp+48h] [ebp-8h]
  float fDy; // [esp+48h] [ebp-8h]
  float fNorm; // [esp+4Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] UpdateBodiesInput_4542E0\n");
  
  fIntensity = 0.0;
  nCenterX = ConvertFrom640AsInt(320);
  nCenterY = ConvertFrom640AsInt(240);
  nPrevX = *pnLastX;
  nPrevY = *pnLastY;
  MutexThreadWait_4815EC(1);
  pInputRing = GetInputState_481B37();
  for ( iSample = 0; iSample < 20 && pInputRing[iSample].true_6; ++iSample )
  {
    if ( pInputRing[iSample].kbdMove_5 == 1 )
    {
      fDx = fabs((double)(pInputRing[iSample].mouseX_0 - nPrevX));
      if ( fDx > 9.0 )
        fDx = 9.0;
      fAccMove = fDx / 40.0 + fIntensity;
      fDy = fabs((double)(pInputRing[iSample].mouseY_2 - nPrevY));
      if ( fDy > 9.0 )
        fDy = 9.0;
      fIntensity = fDy / 40.0 + fAccMove;
    }
    else
    {
      fAccDist = fabs((double)(pInputRing[iSample].mouseX_0 - nCenterX)) / 1000.0 + fIntensity;
      fIntensity = fabs((double)(pInputRing[iSample].mouseY_2 - nCenterY)) / 1000.0 + fAccDist;
    }
    nPrevX = pInputRing[iSample].mouseX_0;
    nPrevY = pInputRing[iSample].mouseY_2;
    pInputRing[iSample].mouseBtn_4 = 0;
    pInputRing[iSample].mouseX_0 = nCenterX;
    pInputRing[iSample].mouseY_2 = nCenterY;
    pInputRing[iSample].true_6 = 0;
    pInputRing[iSample].kbdMove_5 = 0;
  }
  while ( iSample < 30 )
  {
    pInputRing[iSample].mouseBtn_4 = 0;
    pInputRing[iSample].mouseX_0 = nCenterX;
    pInputRing[iSample].mouseY_2 = nCenterY;
    pInputRing[iSample++].true_6 = 0;
  }
  *pnLastX = pInputRing->mouseX_0;
  *pnLastY = pInputRing->mouseY_2;
  MutexThreadWait_4815EC(0);
  if ( pBodyA && pBodyB )
  {
    if ( fIntensity + fIntensity <= 1.0 )
      fNorm = fIntensity + fIntensity;
    else
      fNorm = 1.0;
    fDeltaA = (double)(unsigned __int8)bInputDrive * fNorm + fBaseDelta;
    if ( CSekBody__UpdateAnim_4019E1(pBodyA, fDeltaA) )
      CSekBody__PlayAnim_40179D(pBodyA, 17);
    fDeltaB = (double)(unsigned __int8)bInputDrive * fNorm + fBaseDelta;
    if ( CSekBody__UpdateAnim_4019E1(pBodyB, fDeltaB) )
      CSekBody__PlayAnim_40179D(pBodyB, 7);
  }
  return fIntensity * 350.0 * (double)(unsigned __int8)bInputDrive;
}

// Countdown 'time running out' flash overlay (old name DrawProgressFill was wrong). Caller (PlayTugOfWar, sole call) passes fRemainFrac = 1 - elapsed/3750 computed right next to DrawClockHud; fPhase = fRemainFrac*3.5 and the guard fPhase<=1 means the overlay only draws during the LAST ~28.6% (~1.07s) of the 3.75s countdown. Two 6-segment key tables: alpha keys [0,1,1,.75,.5,.25,0] and width keys [1,.5,0,...]; per segment lerps fAlpha/fBleed with t = frac(fPhase*6)/1.6666 (note: 5/3 divisor, so each step spans only 60% of its segment - stepped envelope, not smooth). The quad is (0-w, -80-w)-(640+w, 560+w) with w = 200*fBleed: base rect already covers the 640x480 screen, so fBleed is EDGE BLEED stretching the sprite texture outward (NOT a left-to-right bar grow as previously noted). Net effect: flash ramps up, peaks at full alpha during segment 1 (fPhase 1/6..2/6), then fades through stepped 0.85/0.6/0.35/0.1 levels while the bleed collapses, vanishing exactly at expiry (fPhase=0 -> alpha key 0). At exac
void __cdecl hook_DrawCountdownFlash_4532D0(CHanaSpriteT3F6_S1FC *pSprite, float fRemainFrac)
{
  int nBottomAdd; // esi
  int nRightAdd; // esi
  int nTopRev; // esi
  int nLeftMost; // esi
  int nLeft; // eax
  int nTop; // [esp-Ch] [ebp-8Ch]
  int nRight; // [esp-8h] [ebp-88h]
  int nBottom; // [esp-4h] [ebp-84h]
  int nSeg; // [esp+4h] [ebp-7Ch]
  float arrAlphaKeys[11]; // [esp+14h] [ebp-6Ch] BYREF
  float arrWidthKeys[9]; // [esp+40h] [ebp-40h] BYREF
  float *pfAlphaKeys; // [esp+64h] [ebp-1Ch]
  float fAlpha; // [esp+68h] [ebp-18h]
  struct tagRECT rc; // [esp+6Ch] [ebp-14h] BYREF
  float fBleed; // [esp+7Ch] [ebp-4h]
  float fPhase; // [esp+8Ch] [ebp+Ch]

  ONCELOG("[HNH:SUB] DrawCountdownFlash_4532D0\n");
  
  fPhase = fRemainFrac * 3.5;
  arrAlphaKeys[0] = 0.0;
  arrAlphaKeys[1] = 1.0;
  arrAlphaKeys[2] = 1.0;
  arrAlphaKeys[3] = 0.75;
  arrAlphaKeys[4] = 0.5;
  arrAlphaKeys[5] = 0.25;
  memset(&arrAlphaKeys[6], 0, 20);
  pfAlphaKeys = arrAlphaKeys;
  arrWidthKeys[0] = 1.0;
  arrWidthKeys[1] = 0.5;
  memset(&arrWidthKeys[2], 0, 28);
  fBleed = 1.0;
  nSeg = (__int64)(fPhase * 6.0);
  if ( pSprite )
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
    if ( fPhase <= 1.0 && fPhase >= 0.0 )
    {
      fAlpha = (pfAlphaKeys[nSeg + 1] - pfAlphaKeys[nSeg]) * ((fPhase * 6.0 - (double)nSeg) / 1.6666)
        + pfAlphaKeys[nSeg];
      fBleed = (arrWidthKeys[nSeg + 1] - arrWidthKeys[nSeg]) * ((fPhase * 6.0 - (double)nSeg) / 1.6666)
        + arrWidthKeys[nSeg];
      if ( fPhase * 6.0 - (double)nSeg <= 0.0 )
        fBleed = arrWidthKeys[nSeg];
      if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
      {
        nBottomAdd = ConvertFrom640AsInt(560);
        nBottom = ConvertFrom640AsInt((__int64)(200.0 * fBleed)) + nBottomAdd;
        nRightAdd = ConvertFrom640AsInt(640);
        nRight = ConvertFrom640AsInt((__int64)(200.0 * fBleed)) + nRightAdd;
        nTopRev = ConvertFrom640AsInt(-80);
        nTop = nTopRev - ConvertFrom640AsInt((__int64)(200.0 * fBleed));
        nLeftMost = ConvertFrom640AsInt(0);
        nLeft = ConvertFrom640AsInt((__int64)(200.0 * fBleed));
        SetRect(&rc, nLeftMost - nLeft, nTop, nRight, nBottom);
        if ( fPhase * 6.0 - (double)nSeg == 0.0 )
          CHanaObj__SetAlpha3F6(pSprite, pfAlphaKeys[nSeg]);
        else
          CHanaObj__SetAlpha3F6(pSprite, fAlpha);
        DrawSpriteLTRB(pSprite, rc.left, rc.top, rc.right, rc.bottom);
        g_pD3DDevice_4D6C78->EndScene();
      }
    }
  }
}

// Snapshot/shutter transition overlay (sole caller: PlayTugOfWar, right after DrawClockHud + DrawCountdownFlash). Active while dwElapsed <= 3000ms. pSprites[4..7] = 4-frame cycling icon (frame index dwElapsed/100 % 4, i.e. 100ms per frame) drawn centered at (320,412) scale 1.0. pSprites[3] = frame/outline box sprite drawn as rect (192,330)-(448,394): first normal alpha blend at fFrameAlpha, then DESTBLEND switched to 2 (D3DBLEND_ONE => ADDITIVE glow) with a sin pulse alpha sin(dwElapsed/90)/4+0.25 (0..0.5, ~565ms period), then 10 expanding outline echoes - each pass grows the rect +10/+8 per side and multiplies fAlpha_144 by 0.6 (shockwave rings). fStartClock: caller passes the sequence START TICK (var_18), not a 0..500 progress - so fFade = 1-fStartClock/500 clamps to 0 and fFrameAlpha is effectively always 1; the fFade*300 store and the rect offsets (+/-fFade) are DEAD code (offsets hardcoded to 0). Looks like leftover dev math intended to expand/fade the box with progress.
void __cdecl hook_DrawTransitionFlash_453FAC(CHanaSpriteT3F6_S1FC *pSprites[], unsigned int dwElapsed, float fStartClock)
{
  int v3; // eax
  int nTop; // [esp+30h] [ebp-38h]
  float fIconX; // [esp+34h] [ebp-34h]
  int nRight; // [esp+34h] [ebp-34h]
  float fFrameAlpha; // [esp+38h] [ebp-30h]
  float fIconScale; // [esp+38h] [ebp-30h]
  float fIconY; // [esp+38h] [ebp-30h]
  int nBottom; // [esp+38h] [ebp-30h]
  float fPulseAlpha; // [esp+38h] [ebp-30h]
  int iRing; // [esp+4Ch] [ebp-1Ch]
  struct tagRECT rc; // [esp+54h] [ebp-14h] BYREF
  float fFade; // [esp+64h] [ebp-4h]

  ONCELOG("[HNH:SUB] DrawTransitionFlash_453FAC\n");
 
  fFade = 1.0 - fStartClock / 500.0;
  if ( dwElapsed <= 0xBB8 )
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
    if ( fFade > 1.0 )
      fFade = 1.0;
    if ( fFade < 0.0 )
      fFade = 0.0;
    fFrameAlpha = 1.0 - fFade;
    CHanaObj__SetAlpha3F6(pSprites[3], fFrameAlpha);
    fFade = fFade * 300.0;
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      fIconScale = ConvertFrom640AsFloat(1.0);
      CHanaSpriteT3F6__SetScale_414B09(pSprites[dwElapsed / 0x64 % 4 + 4], fIconScale);
      fIconY = ConvertFrom640AsFloat(412.0);
      fIconX = ConvertFrom640AsFloat(320.0);
      CHanaSpriteT3F6__SetPos_414A5D(pSprites[dwElapsed / 0x64 % 4 + 4], fIconX, fIconY);
      DrawSprite2D(pSprites[dwElapsed / 0x64 % 4 + 4]);
      fFade = 0.0;
      nBottom = ConvertFrom640AsInt((__int64)0.0 + 394);
      nRight = ConvertFrom640AsInt((__int64)0.0 + 448);
      nTop = ConvertFrom640AsInt(330 - (__int64)0.0);
      v3 = ConvertFrom640AsInt(192 - (__int64)0.0);
      SetRect(&rc, v3, nTop, nRight, nBottom);
      DrawSpriteLTRB(pSprites[3], rc.left, rc.top, rc.right, rc.bottom);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
      fPulseAlpha = sin((double)dwElapsed / 90.0) / 4.0 + 0.25;
      CHanaObj__SetAlpha3F6(pSprites[3], fPulseAlpha);
      DrawSpriteLTRB(pSprites[3], rc.left, rc.top, rc.right, rc.bottom);
      for ( iRing = 0; iRing < 10; ++iRing )
      {
        pSprites[3]->fAlpha_144 = pSprites[3]->fAlpha_144 * 0.60000002;
        SetRect(&rc, rc.left - 10, rc.top - 8, rc.right + 10, rc.bottom + 8);
        DrawSpriteLTRB(pSprites[3], rc.left, rc.top, rc.right, rc.bottom);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }
  }
}

// Digital clock/timer HUD: renders nTimeLeft (clamped >= 0) as XXX:XX - a 3-digit high group (decimal places 10^4..10^2, i.e. value/100) then the colon sprite (arrSprNumbers[10]) then a 2-digit low group (places 10^1..10^0, value%100). Digit k = (t % 10^(k+1)) / 10^k via IntPow_44AF15. Layout anchored at nXAnchor = nX+24 with a 26px digit pitch: high-group quads run left from nXAnchor-26*k (58 wide, y nY+16..nY+48), low-group quads at nXAnchor+6-26*j (52 wide, y nY+22..nY+48 - 6px LOWER than the high group), colon at (nX+8, nY+20)-(nX+40, nY+52). Quads overlap on the 26px pitch - sprite art carries its own padding. Caller (PlayTugOfWar, sole) computes nTimeLeft = var_68*100 - var_4/10 (a countdown against the duel clock drawn right before DrawCountdownFlash).
void __cdecl hook_DrawClockHud_44AF5B(CHanaSpriteT3F6_S1FC **arrSprNumbers, int nX, int nY, int nTimeLeft)
{
  int v4; // eax
  int v5; // esi
  int v6; // eax
  int v7; // eax
  int v8; // esi
  int v9; // [esp-Ch] [ebp-30h]
  int v10; // [esp-Ch] [ebp-30h]
  int v11; // [esp-Ch] [ebp-30h]
  int v12; // [esp-8h] [ebp-2Ch]
  int v13; // [esp-8h] [ebp-2Ch]
  int v14; // [esp-8h] [ebp-2Ch]
  int v15; // [esp-4h] [ebp-28h]
  int v16; // [esp-4h] [ebp-28h]
  int v17; // [esp-4h] [ebp-28h]
  int nDigitHi; // [esp+4h] [ebp-20h]
  int nDigitLo; // [esp+4h] [ebp-20h]
  int kPlaceHi; // [esp+8h] [ebp-1Ch]
  int kPlaceLo; // [esp+8h] [ebp-1Ch]
  int nXAnchor; // [esp+Ch] [ebp-18h]
  struct tagRECT rc; // [esp+14h] [ebp-10h] BYREF
  ONCELOG("[HNH:SUB] DrawClockHud_44AF5B\n");
  
  nXAnchor = nX + 24;
  if ( nTimeLeft <= 0 )
    nTimeLeft = 0;
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
  if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
  {
    for ( kPlaceHi = 2; kPlaceHi < 5; ++kPlaceHi )
    {
      v15 = ConvertFrom640AsInt(nY + 48);
      v12 = ConvertFrom640AsInt(26 * (1 - kPlaceHi) + nXAnchor + 32);
      v9 = ConvertFrom640AsInt(nY + 16);
      v4 = ConvertFrom640AsInt(nXAnchor - 26 * kPlaceHi);
      SetRect(&rc, v4, v9, v12, v15);
      v5 = nTimeLeft % IntPow_44AF15(10, kPlaceHi + 1);
      nDigitHi = v5 / IntPow_44AF15(10, kPlaceHi);
      if ( arrSprNumbers[nDigitHi] )
        DrawSpriteLTRB(arrSprNumbers[nDigitHi], rc.left, rc.top, rc.right, rc.bottom);
    }
    v16 = ConvertFrom640AsInt(nY + 52);
    v13 = ConvertFrom640AsInt(nX + 40);
    v10 = ConvertFrom640AsInt(nY + 20);
    v6 = ConvertFrom640AsInt(nX + 8);
    SetRect(&rc, v6, v10, v13, v16);
    DrawSpriteLTRB(arrSprNumbers[10], rc.left, rc.top, rc.right, rc.bottom);
    for ( kPlaceLo = 0; kPlaceLo < 2; ++kPlaceLo )
    {
      v17 = ConvertFrom640AsInt(nY + 48);
      v14 = ConvertFrom640AsInt(26 * (1 - kPlaceLo) + nXAnchor + 32);
      v11 = ConvertFrom640AsInt(nY + 22);
      v7 = ConvertFrom640AsInt(-26 * kPlaceLo + nXAnchor + 6);
      SetRect(&rc, v7, v11, v14, v17);
      v8 = nTimeLeft % IntPow_44AF15(10, kPlaceLo + 1);
      nDigitLo = v8 / IntPow_44AF15(10, kPlaceLo);
      if ( arrSprNumbers[nDigitLo] )
        DrawSpriteLTRB(arrSprNumbers[nDigitLo], rc.left, rc.top, rc.right, rc.bottom);
    }
    g_pD3DDevice_4D6C78->EndScene();
  }
}

// Compact digit-sprite number renderer, sibling of DrawClockHud_44AF5B: renders nValue (clamped >= 0) as X:XX - ONE hundreds digit (value/100) via the degenerate loop `for (kPlaceHi = 2; kPlaceHi < 3)` (runs exactly once - copy-paste vestige of the clock's 2..4 loop), colon sprite pDigitSprites[10], then tens/ones (value%100). 20px digit pitch (clock uses 26), anchor nXAnchor = xScreen+8: high digit quad (nXAnchor-40, yScreen)-(nXAnchor+20, yScreen+20); colon (xScreen-12, yScreen+4)-(nXAnchor, yScreen+24); low group at nXAnchor+6-20*j, y yScreen+6..+20 (6px lower, same offset pattern as the clock). No null-check on digit sprites (the clock has one). Sole caller PlayTugOfWar draws the elapsed STOPWATCH at (320,448) with nValue = elapsed_ms/10 => S:CC format, only while elapsed <= 3000ms - the running photo-timer under the DrawTransitionFlash snapshot overlay.
void __cdecl hook_DrawHudSpriteNumber_44B97B(CHanaSpriteT3F6_S1FC **pDigitSprites, int xScreen, int yScreen, int nValue)
{
  int v4; // eax
  int nMod; // esi
  int v6; // eax
  int v7; // eax
  int v8; // esi
  int v9; // [esp-Ch] [ebp-30h]
  int v10; // [esp-Ch] [ebp-30h]
  int v11; // [esp-Ch] [ebp-30h]
  int v12; // [esp-8h] [ebp-2Ch]
  int v13; // [esp-8h] [ebp-2Ch]
  int v14; // [esp-8h] [ebp-2Ch]
  int v15; // [esp-4h] [ebp-28h]
  int v16; // [esp-4h] [ebp-28h]
  int v17; // [esp-4h] [ebp-28h]
  int nDigitHi; // [esp+4h] [ebp-20h]
  int nDigitLo; // [esp+4h] [ebp-20h]
  int kPlaceHi; // [esp+8h] [ebp-1Ch]
  int kPlaceLo; // [esp+8h] [ebp-1Ch]
  int nXAnchor; // [esp+Ch] [ebp-18h]
  struct tagRECT rc; // [esp+14h] [ebp-10h] BYREF
  ONCELOG("[HNH:SUB] DrawHudSpriteNumber_44B97B\n");
 
  nXAnchor = xScreen + 8;
  if ( nValue <= 0 )
    nValue = 0;
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
  if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
  {
    for ( kPlaceHi = 2; kPlaceHi < 3; ++kPlaceHi )
    {
      v15 = ConvertFrom640AsInt(yScreen + 20);
      v12 = ConvertFrom640AsInt(nXAnchor + 20 * (1 - kPlaceHi));
      v9 = ConvertFrom640AsInt(yScreen);
      v4 = ConvertFrom640AsInt(nXAnchor - 20 * kPlaceHi);
      SetRect(&rc, v4, v9, v12, v15);
      nMod = nValue % IntPow_44AF15(10, kPlaceHi + 1);
      nDigitHi = nMod / IntPow_44AF15(10, kPlaceHi);
      DrawSpriteLTRB(pDigitSprites[nDigitHi], rc.left, rc.top, rc.right, rc.bottom);
    }
    v16 = ConvertFrom640AsInt(yScreen + 24);
    v13 = ConvertFrom640AsInt(nXAnchor);
    v10 = ConvertFrom640AsInt(yScreen + 4);
    v6 = ConvertFrom640AsInt(xScreen - 12);
    SetRect(&rc, v6, v10, v13, v16);
    DrawSpriteLTRB(pDigitSprites[10], rc.left, rc.top, rc.right, rc.bottom);
    for ( kPlaceLo = 0; kPlaceLo < 2; ++kPlaceLo )
    {
      v17 = ConvertFrom640AsInt(yScreen + 20);
      v14 = ConvertFrom640AsInt(nXAnchor + 20 * (1 - kPlaceLo));
      v11 = ConvertFrom640AsInt(yScreen + 6);
      v7 = ConvertFrom640AsInt(-20 * kPlaceLo + nXAnchor + 6);
      SetRect(&rc, v7, v11, v14, v17);
      v8 = nValue % IntPow_44AF15(10, kPlaceLo + 1);
      nDigitLo = v8 / IntPow_44AF15(10, kPlaceLo);
      DrawSpriteLTRB(pDigitSprites[nDigitLo], rc.left, rc.top, rc.right, rc.bottom);
    }
    g_pD3DDevice_4D6C78->EndScene();
  }

}
void register_PlayTugOfWar_45460A() {

  hnh_HookOff();
  hnh_Hook(0x45460a, hook_PlayTugOfWar_45460A, "PlayTugOfWar_45460A"); // [OK]
  hnh_Hook(0x4532d0, hook_DrawCountdownFlash_4532D0, "DrawCountdownFlash_4532D0"); // [OK]
  hnh_Hook(0x4542e0, hook_UpdateBodiesInput_4542E0, "UpdateBodiesInput_4542E0"); // [OK]
  hnh_Hook(0x453fac, hook_DrawTransitionFlash_453FAC, "DrawTransitionFlash_453FAC"); // [OK]
  hnh_Hook(0x44af5b, hook_DrawClockHud_44AF5B, "DrawClockHud_44AF5B"); // [OK]
  hnh_Hook(0x44b97b, hook_DrawHudSpriteNumber_44B97B, "DrawHudSpriteNumber_44B97B"); // [OK]
  hnh_HookOn();
}