#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// == GLOBALS ===

void (__cdecl*SetupHeroEntry_4593E0)(CHanaSceneT458_S504 *pScene,CHanaFrameT3EC_S354 *pHeroFrame, int nSubIndex)
= reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, int)>(0x4593E0);
bool (__cdecl*UpdateHeroBody_44474E)(CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame, float fDeltaTime, float *pfHeroHp, float fHeroHpMax)
= reinterpret_cast<bool(__cdecl*)(CSekBody *, CHanaFrameT3EC_S354 *, float, float *, float)>(0x44474E);
void (__cdecl *BuildMapCollisionGrid_445B12)(CHanaFrameT3EC_S354 *pMapFrame, char **pOutGrid)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pMapFrame, char **pOutGrid)>(0x445b12);
void (__cdecl *Frame_RotateMatrixAxis_4070AB)(CHanaFrameT3EC_S354 *pFrame, int nMode, D3DXVECTOR3 *pAxis, float fAngleRad)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame, int nMode, D3DXVECTOR3 *pAxis, float fAngleRad)>(0x4070ab);
int (__cdecl *CheckFrameCollision_443CC5)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, int bTestType)
= reinterpret_cast<int (__cdecl*)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, int bTestType)>(0x443cc5);
BOOL (__cdecl *HitOverheadSurface_442C1C)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)
= reinterpret_cast<BOOL (__cdecl*)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)>(0x442c1c);
int (__cdecl *HitSupportSurface_4423B0)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)
= reinterpret_cast<int (__cdecl*)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)>(0x4423b0);
void (__cdecl *ComputeMeshBounds_407E1F)(CHanaFrameT3EC_S354 *pMapFrame, D3DXVECTOR3 *fMin, D3DXVECTOR3 *fMax)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pMapFrame, D3DXVECTOR3 *fMin, D3DXVECTOR3 *fMax)>(0x407e1f);
bool (__cdecl *TestBodyMoveHit_443EC4)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, float fNewX, float fNewY, float fNewZ, int bIgnoreY)
= reinterpret_cast<bool (__cdecl*)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, float fNewX, float fNewY, float fNewZ, int bIgnoreY)>(0x443ec4);
bool (__cdecl *TestMoveCollision_4443FC)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, float fNewX, float fNewY, float fNewZ, int bTestType)
= reinterpret_cast<bool (__cdecl*)(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, float fNewX, float fNewY, float fNewZ, int bTestType)>(0x4443fc);
bool (__cdecl *HanaGrid_DFSPath)(char *pGrid, int nCellX, int nCellY, int nDepth, __int16 nSteps, int bIgnoreBlocked)
= reinterpret_cast<bool (__cdecl*)(char *pGrid, int nCellX, int nCellY, int nDepth, __int16 nSteps, int bIgnoreBlocked)>(0x445c12);
D3DXVECTOR2 *(__cdecl *HanaGrid_FloodFill_4460AD)(D3DXVECTOR2 *pfOutStep, char *pGrid, POINT *pnFoeCell, POINT pnHeroCell, int *pnDepth, int nMode, POINT *ppOutPathPts, WORD *pnOutNumbPathPts)
= reinterpret_cast<D3DXVECTOR2 *(__cdecl *)(D3DXVECTOR2 *, char *, POINT *, POINT, int *, int, POINT *, WORD *)>(0x4460AD);
void (__cdecl *UpdateVoicePlayback_458B3A)(CSekBody *pFoeBody, CHanaFrameRootT3EC_S2D4 *pFoeFocusFrame, CSoundStruct_8 **arrVoiceBufs, float fDelta)
= reinterpret_cast<void (__cdecl*)(CSekBody *pFoeBody, CHanaFrameRootT3EC_S2D4 *pFoeFocusFrame, CSoundStruct_8 **arrVoiceBufs, float fDelta)>(0x458b3a);
int (__cdecl *DSBufferGetVolume_423C65)(LPDIRECTSOUNDBUFFER pDSBuf)
= reinterpret_cast<int (__cdecl*)(LPDIRECTSOUNDBUFFER pDSBuf)>(0x423c65);




uint8_t &g_bHeroAttackHold = *(uint8_t *)0x52ED08; // WRITABLE
uint8_t &g_bHeroLightAttack = *(uint8_t *)0x52ED09; // WRITABLE
float &g_fHeroLightAttackTime = *(float *)0x52ED0C; // WRITABLE
int &g_dwBattleMode = *(int *)0x52ED04; // WRITABLE
uint8_t &g_bHeroChargeActive = *(uint8_t *)0x52ED0A; // WRITABLE
float &g_fHeroChargeTime = *(float *)0x52ED00; // WRITABLE
float &g_fHeroIdleTime = *(float *)0x52ED10; // WRITABLE

char (&g_pMapCollGrid)[40804] = *(char (*)[40804])0x519D48; // WRITABLE

uint16_t &g_nFloodCandidates_52ED14 = *(uint16_t *)0x52ED14; // WRITABLE
POINT (&g_arrFloodPathXY_5244F8)[1024][5] = *(POINT (*)[1024][5])0x5244F8; // WRITABLE -- [candidate][waypoint]
__int16 (&g_arrFloodPathLen_523CAC)[1024] = *(__int16 (*)[1024])0x523CAC; // WRITABLE
__int16 (&g_arrFloodPathCnt_52E500)[1024] = *(__int16 (*)[1024])0x52E500; // WRITABLE

uint32_t &g_nCurVoiceIdx_52EF64 = *(uint32_t *)0x52EF64; // WRITABLE
uint32_t &g_nVoiceRotate_52EF68 = *(uint32_t *)0x52EF68; // WRITABLE

// === FUNCTIONS ===

// Place the hero at its entry position for this sub-stage. Scans the ODS scene's children for nodes named like aJump (spawn/entry markers), and when the match index equals nSubIndex, loads that node's position/rotation into the hero frame.
void __cdecl hook_SetupHeroEntry_4593E0(CHanaSceneT458_S504 *pScene, CHanaFrameT3EC_S354 *pHeroFrame, int nSubIndex)
{
  int idx; // edx
  float outZ; // [esp+0h] [ebp-2Ch] BYREF
  float outY; // [esp+4h] [ebp-28h] BYREF
  int j; // [esp+8h] [ebp-24h]
  float outX; // [esp+Ch] [ebp-20h] BYREF
  unsigned int i; // [esp+10h] [ebp-1Ch]
  float pZXrot; // [esp+14h] [ebp-18h] BYREF
  float dirForwardZ; // [esp+18h] [ebp-14h] BYREF
  float pYXrot; // [esp+1Ch] [ebp-10h] BYREF
  float pYpos; // [esp+20h] [ebp-Ch] BYREF
  float pXYrot; // [esp+24h] [ebp-8h] BYREF
  float dirForwardX; // [esp+28h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] SetupHeroEntry_4593E0\n");
  
  j = 0;
  if ( pScene )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->refsFrames_134[i] )
      {
        if ( !_strnicmp(pScene->refsFrames_134[i]->szName_8, "JUMP", 4u) )
        {
          idx = j++;
          if ( nSubIndex == idx )
          {
            HanaFrame_GetRelPosition(pScene->refsFrames_134[i], g_pSceneFrame, &outX, &outY, &outZ);
            MtxMult_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, outX, outY, outZ);
            CalculateLocalRotationMatrix(
              pScene->refsFrames_134[i],
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              &dirForwardX,
              &pYpos,
              &dirForwardZ,
              &pXYrot,
              &pYXrot,
              &pZXrot);
            UpdateBoneLookAtTransform(
              pHeroFrame,
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              dirForwardX,
              0.0,
              dirForwardZ,
              0.0,
              1.0,
              0.0);
            MtxMult_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -5.0);
            return;
          }
        }
      }
    }
  }
}

//   bool UpdateHeroBody_44474E(CSBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame,
//                            float fDelta, float *pFCharge, float fChargeMax)
//   // returns bChargedHit -- true while a charge attack (anim 16) is striking
// 
//   1. Input harvest (mutex-guarded): accumulate nMouseDX/nMouseDY from 30 input slots relative to screen center (x5 when
//   kbdMove_5 -- keyboard look), drain deltas back to center; build btnLeft/btnRight edge+hold states that latch
//   g_bHeroAttackHold (LMB) and g_bHeroLightAttack (RMB).
// 
//   2. State machine (g_dwBattleMode 0-5), gated on g_dwBattleMode > -1 (else full reset):
// 
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   |      Condition       |            Anim            |     *pFCharge      |        Body Z-rotation         | Mode |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | no button, idle <    | 1 (idle) / 2 (fidget)      | +0.05*fDelta,      | rotZMouseTurn = nMouseDX/1080  | 0    |
//   | 500                  |                            | clamp max          | (mouse turn)                   |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | LMB held (after      | 6 (guard)                  | -0.25*fDelta       | rotZGuardL = -1.8*fDelta       | 2    |
//   | hold-latch)          |                            |                    |                                |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | LMB tap              | 3                          | -0.15*fDelta       | rotZGuardL... wait -- case 1:  | 1    |
//   |                      |                            |                    | -0.5*fDelta                    |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | RMB tap              | 4                          | -0.15*fDelta       | rotZGuardR = +0.5*fDelta       | 3    |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | MMB (btn==3)         | play 16,                   | -                  | -                              | 5    |
//   |                      | g_bHeroChargeActive        |                    |                                |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | charge active, 1-35  | 16 (charged hit ->         | -0.5*fDelta        | rotZChargeSpin = -1.8*fDelta   | --   |
//   | s                    | bChargedHit=1)             |                    | (after 6 s)                    |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | charge > 70 s        | reset (idle at 68 s)       |                    |                                |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
//   | light attack active, | 13                         | -0.3*fDelta        | rotZAttackSpin = +1.5*fDelta   | 4    |
//   |  < 28 s              |                            |                    |                                |      |
//   +----------------------+----------------------------+--------------------+--------------------------------+------+
// 
//   3. Movement + collision slide: 3 iterations of probe->TestMoveCollision->restore or slide to midpoint (midX/midY/midZ =
//   average of pre/post positions), aborting when CheckFrameCollision_443CC5 returns a ground hit (nHitMode 0-3, material
//   index).
// 
//   4. Footstep audio: when standing on material nHitMode -- position the two g_arrFootstepSound_52F17C/52F18C 3D buffers
//   at the hero, and Play/Stop them gated on the current animation's pAnimData->float_60 frame number falling inside
//   hardcoded footstep-frame ranges (303-310, 403-410, 500-504, ... for one foot; 313-320, 413-420, 507-511, ... for the other
//   -- left/right step windows); all other materials' buffers get stopped and rewound.
bool __cdecl hook_UpdateHeroBody_44474E(
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fDelta,
  float *pFCharge,
  float fChargeMax)
{
  float midX; // [esp+0h] [ebp-80h]
  float midY; // [esp+4h] [ebp-7Ch]
  float fZDelta; // [esp+8h] [ebp-78h]
  float rotZGuardL; // [esp+8h] [ebp-78h]
  float rotZGuardR; // [esp+8h] [ebp-78h]
  float rotZMouseTurn; // [esp+8h] [ebp-78h]
  float rotZChargeSpin; // [esp+8h] [ebp-78h]
  float rotZAttackSpin; // [esp+8h] [ebp-78h]
  float midZ; // [esp+8h] [ebp-78h]
  CSekItem *pAnimData; // [esp+18h] [ebp-68h]
  float pyAfter; // [esp+1Ch] [ebp-64h] BYREF
  float pxAfter; // [esp+20h] [ebp-60h] BYREF
  float pzAfter; // [esp+24h] [ebp-5Ch] BYREF
  float px; // [esp+28h] [ebp-58h] BYREF
  __int16 nMouseDX; // [esp+2Ch] [ebp-54h]
  __int16 nCenterY; // [esp+30h] [ebp-50h]
  char btnRightHold; // [esp+34h] [ebp-4Ch]
  __int16 nCenterX; // [esp+38h] [ebp-48h]
  char btnLeftHold; // [esp+3Ch] [ebp-44h]
  float pOutZ; // [esp+40h] [ebp-40h] BYREF
  char btnRight; // [esp+44h] [ebp-3Ch]
  CHanaBase_S130 *pBodyFrame; // [esp+48h] [ebp-38h]
  CInputState *pInput; // [esp+4Ch] [ebp-34h]
  __int16 i; // [esp+50h] [ebp-30h]
  char btnLeft; // [esp+54h] [ebp-2Ch]
  float pyNew; // [esp+58h] [ebp-28h] BYREF
  BOOL nHitMode; // [esp+5Ch] [ebp-24h]
  __int16 nMouseDY; // [esp+60h] [ebp-20h]
  bool bChargedHit; // [esp+64h] [ebp-1Ch]
  D3DXVECTOR3 pDirUp; // [esp+68h] [ebp-18h] BYREF
  float pz; // [esp+74h] [ebp-Ch] BYREF
  float pOutX; // [esp+78h] [ebp-8h] BYREF
  D3DVALUE py; // [esp+7Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateHeroBody_44474E\n");
 
  pBodyFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  nMouseDX = 0;
  nMouseDY = 0;
  nCenterX = ConvertFrom640AsInt(320);
  nCenterY = ConvertFrom640AsInt(240);
  bChargedHit = 0;
  btnLeft = 0;
  btnLeftHold = 0;
  btnRight = 0;
  btnRightHold = 0;
  pDirUp.x = 0.0;
  pDirUp.y = 1.0;
  pDirUp.z = 0.0;
  MutexThreadWait_4815EC(1);
  pInput = GetInputState_481B37();
  for ( i = 0; i < 30; ++i )
  {
    pInput[i].true_6 = 0;
    if ( pInput[i].kbdMove_5 == 1 )
    {
      nMouseDX += 5 * (pInput[i].mouseX_0 - nCenterX);
      pInput[i].mouseX_0 = nCenterX;
      nMouseDY += 5 * (pInput[i].mouseY_2 - nCenterY);
    }
    else
    {
      nMouseDX += pInput[i].mouseX_0 - nCenterX;
      pInput[i].mouseX_0 = nCenterX;
      nMouseDY += pInput[i].mouseY_2 - nCenterY;
    }
    pInput[i].mouseY_2 = nCenterY;
  }
  for ( i = 0; i < 15; ++i )
  {
    if ( pInput[i].mouseBtn_4 == 1 )
      btnLeft = 1;
    if ( !pInput[i].mouseBtn_4 && btnLeft == 1 )
      btnLeftHold = 1;
    if ( pInput[i].mouseBtn_4 == 1 && btnLeftHold == 1 )
      g_bHeroAttackHold = 1;
    if ( pInput[i].mouseBtn_4 == 2 )
      btnRight = 1;
    if ( !pInput[i].mouseBtn_4 && btnRight == 1 )
      btnRightHold = 1;
    if ( pInput[i].mouseBtn_4 == 2 && btnRightHold == 1 && !g_bHeroLightAttack )
    {
      g_bHeroLightAttack = 1;
      g_fHeroLightAttackTime = 0.0;
    }
  }
  MutexThreadWait_4815EC(0);
  if ( g_dwBattleMode <= -1 )
  {
    g_bHeroAttackHold = 0;
    g_bHeroLightAttack = 0;
    g_bHeroChargeActive = 0;
    g_fHeroLightAttackTime = 0.0;
    g_fHeroChargeTime = 0.0;
    g_fHeroIdleTime = 0.0;
    g_dwBattleMode = 0;
  }
  HanaFrame_GetRelPosition((CHanaFrameT3EC_S354 *)pBodyFrame, g_pSceneFrame, &px, &py, &pz);
  if ( g_bHeroLightAttack || g_bHeroChargeActive )
  {
    if ( g_bHeroChargeActive )
    {
      g_fHeroChargeTime = g_fHeroChargeTime + fDelta;
      *pFCharge = *pFCharge - 0.5 * fDelta;
      if ( g_fHeroChargeTime > 1.0 && g_fHeroChargeTime < 35.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 16);
        bChargedHit = 1;
      }
      if ( g_fHeroChargeTime > 6.0 && g_fHeroChargeTime < 35.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 16);
        rotZChargeSpin = -1.8 * fDelta;
        MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)pBodyFrame, 0.0, 0.0, rotZChargeSpin);
      }
      if ( g_fHeroChargeTime > 70.0 && pHeroBody->field_200 != 16 )
      {
        g_fHeroChargeTime = 0.0;
        g_bHeroChargeActive = 0;
      }
      if ( g_fHeroChargeTime > 68.0 )
        SekBody__UpdateTo2_401647(pHeroBody, 1);
      g_fHeroIdleTime = 0.0;
    }
    else if ( g_bHeroLightAttack )
    {
      g_fHeroLightAttackTime = g_fHeroLightAttackTime + fDelta;
      if ( g_fHeroLightAttackTime < 28.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 13);
        *pFCharge = *pFCharge - 0.30000001 * fDelta;
        rotZAttackSpin = 1.5 * fDelta;
        MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)pBodyFrame, 0.0, 0.0, rotZAttackSpin);
      }
      if ( g_fHeroLightAttackTime > 30.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 1);
        g_bHeroLightAttack = 0;
        g_fHeroLightAttackTime = 0.0;
      }
      g_fHeroIdleTime = 0.0;
      g_dwBattleMode = 4;
    }
  }
  else
  {
    if ( pInput->mouseBtn_4 )
    {
      g_fHeroIdleTime = 0.0;
      if ( g_bHeroAttackHold == 1 && pInput->mouseBtn_4 == 1 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 6);
        *pFCharge = *pFCharge - 0.25 * fDelta;
        fZDelta = -1.8 * fDelta;
        MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)pBodyFrame, 0.0, 0.0, fZDelta);
        g_dwBattleMode = 2;
      }
      else
      {
        switch ( pInput->mouseBtn_4 )
        {
        case 1:
          SekBody__UpdateTo2_401647(pHeroBody, 3);
          *pFCharge = *pFCharge - 0.15000001 * fDelta;
          rotZGuardL = -0.5 * fDelta;
          MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)pBodyFrame, 0.0, 0.0, rotZGuardL);
          g_dwBattleMode = 1;
          break;
        case 2:
          SekBody__UpdateTo2_401647(pHeroBody, 4);
          *pFCharge = *pFCharge - 0.15000001 * fDelta;
          rotZGuardR = 0.5 * fDelta;
          MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)pBodyFrame, 0.0, 0.0, rotZGuardR);
          g_dwBattleMode = 3;
          break;
        case 3:
          g_bHeroChargeActive = 1;
          g_fHeroChargeTime = 0.0;
          CSekBody__PlayAnim_40179D(pHeroBody, 16);
          g_dwBattleMode = 5;
          break;
        }
      }
    }
    else
    {
      g_bHeroAttackHold = 0;
      g_fHeroIdleTime = g_fHeroIdleTime + fDelta;
      if ( g_fHeroIdleTime < 500.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 1);
      }
      else if ( pHeroBody->field_200 == 1 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 2);
      }
      *pFCharge = 0.050000001 * fDelta + *pFCharge;
      if ( *pFCharge >= (double)fChargeMax )
        *pFCharge = fChargeMax;
      g_dwBattleMode = 0;
    }
    rotZMouseTurn = (double)nMouseDX / 360.0 / 3.0;
    Frame_RotateMatrixAxis_4070AB((CHanaFrameT3EC_S354 *)pBodyFrame, 1, &pDirUp, rotZMouseTurn);
  }
  if ( *pFCharge <= 1.0 )
    *pFCharge = 1.0;
  if ( CSekBody__UpdateAnim_4019E1(pHeroBody, fDelta) )
  {
    if ( g_fHeroIdleTime >= 500.0 )
      g_fHeroIdleTime = 0.0;
    if ( pHeroBody->field_200 == 16 )
      SekBody__UpdateTo2_401647(pHeroBody, 1);
  }
  nHitMode = 0;
  for ( i = 0; i < 3; ++i )
  {
    nHitMode = CheckFrameCollision_443CC5((CHanaFrameT3EC_S354 *)pBodyFrame, pMapFrame, 0);
    if ( nHitMode )
      break;
    HanaFrame_GetRelPosition((CHanaFrameT3EC_S354 *)pBodyFrame, g_pSceneFrame, &pOutX, &pyNew, &pOutZ);
    if ( !TestMoveCollision_4443FC((CHanaFrameT3EC_S354 *)pBodyFrame, pMapFrame, px, pyNew, pz, 0) )
      MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, px, pyNew, pz);
    HanaFrame_GetRelPosition((CHanaFrameT3EC_S354 *)pBodyFrame, g_pSceneFrame, &pxAfter, &pyAfter, &pzAfter);
    midZ = pz - (pz - pzAfter) / 2.0;
    midY = py - (py - pyAfter) / 2.0;
    midX = px - (px - pxAfter) / 2.0;
    MtxMult_4069DD((CHanaFrameT3EC_S354 *)pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, midX, midY, midZ);
  }
  if ( (unsigned int)--nHitMode < 4 && g_arrFootstepSound_52F18C[nHitMode] && g_arrFootstepSound_52F17C[nHitMode] )
  {
    pAnimData = &pHeroBody->items_250[pHeroBody->field_200];
    HanaFrame_GetCenterPosition_408C3E((CHanaFrameRootT3EC_S2D4 *)pBodyFrame, g_pSceneFrame, &px, &py, &pz);
    g_arrFootstepSound_52F18C[nHitMode]->pDS3DBuffer_4->SetPosition(
      px,
      py,
      pz,
      1u);
    g_arrFootstepSound_52F17C[nHitMode]->pDS3DBuffer_4->SetPosition(
      px,
      py,
      pz,
      1u);
    if ( !g_arrFootstepSound_52F17C[nHitMode]->pDSBuffer_0
      || g_arrFootstepSound_52F17C[nHitMode]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pAnimData->float_60 < 303.0 || pAnimData->float_60 > 310.0)
        && (pAnimData->float_60 < 403.0 || pAnimData->float_60 > 410.0)
        && (pAnimData->float_60 < 500.0 || pAnimData->float_60 > 504.0)
        && (pAnimData->float_60 < 600.0 || pAnimData->float_60 > 606.0)
        && (pAnimData->float_60 < 1630.0 || pAnimData->float_60 > 1640.0)
        && (pAnimData->float_60 < 1670.0 || pAnimData->float_60 > 1680.0)
        && (pAnimData->float_60 < 2622.0 || pAnimData->float_60 > 2630.0) )
      {
        g_arrFootstepSound_52F17C[nHitMode]->pDSBuffer_0->Stop();
        g_arrFootstepSound_52F17C[nHitMode]->pDSBuffer_0->SetCurrentPosition(0);
      }
      else
      {
        g_arrFootstepSound_52F17C[nHitMode]->pDSBuffer_0->Play(
          0,
          0,
          0);
      }
    }
    if ( !g_arrFootstepSound_52F18C[nHitMode]->pDSBuffer_0
      || g_arrFootstepSound_52F18C[nHitMode]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pAnimData->float_60 < 313.0 || pAnimData->float_60 > 320.0)
        && (pAnimData->float_60 < 413.0 || pAnimData->float_60 > 420.0)
        && (pAnimData->float_60 < 507.0 || pAnimData->float_60 > 511.0)
        && (pAnimData->float_60 < 605.0 || pAnimData->float_60 > 611.0)
        && (pAnimData->float_60 < 1633.0 || pAnimData->float_60 > 1643.0)
        && (pAnimData->float_60 < 1658.0 || pAnimData->float_60 > 1665.0)
        && (pAnimData->float_60 < 2620.0 || pAnimData->float_60 > 2630.0) )
      {
        g_arrFootstepSound_52F18C[nHitMode]->pDSBuffer_0->Stop();
        g_arrFootstepSound_52F18C[nHitMode]->pDSBuffer_0->SetCurrentPosition(
          0);
      }
      else
      {
        g_arrFootstepSound_52F18C[nHitMode]->pDSBuffer_0->Play(
          0,
          0,
          0);
      }
    }
    for ( i = 0; i < 4; ++i )
    {
      if ( nHitMode != i )
      {
        if ( g_arrFootstepSound_52F17C[i] )
        {
          g_arrFootstepSound_52F17C[i]->pDSBuffer_0->Stop();
          g_arrFootstepSound_52F17C[i]->pDSBuffer_0->SetCurrentPosition(
            0);
        }
        if ( g_arrFootstepSound_52F18C[i] )
        {
          g_arrFootstepSound_52F18C[i]->pDSBuffer_0->Stop();
          g_arrFootstepSound_52F18C[i]->pDSBuffer_0->SetCurrentPosition(
            0);
        }
      }
    }
  }
  return bChargedHit;
}

void __cdecl hook_Frame_RotateMatrixAxis_4070AB(CHanaFrameT3EC_S354 *pFrame, int nMode, D3DXVECTOR3 *pAxis, float fAngleRad)
{
  D3DXMATRIX matRotLocal; // [esp+8h] [ebp-180h] BYREF
  D3DXVECTOR3 qTransXYZ2; // [esp+78h] [ebp-110h]
  D3DXMATRIX matOldLocal2; // [esp+88h] [ebp-100h] BYREF
  D3DXMATRIX matRotParent; // [esp+C8h] [ebp-C0h] BYREF
  D3DXVECTOR3 qTransXYZ; // [esp+138h] [ebp-50h]
  D3DXMATRIX matOldLocal1; // [esp+148h] [ebp-40h] BYREF
  ONCELOG("[HNH:SUB] Frame_RotateMatrixAxis_4070AB\n");
  
  if ( nMode )
  {
    if ( nMode == 1 )
    {
      D3DXMatrixRotationAxis(&matRotParent, pAxis, fAngleRad);
      qmemcpy(&matOldLocal1, &pFrame->mat4x4_fmrx, sizeof(matOldLocal1));
      memset(matOldLocal1.m[3], 0, 12);
      qTransXYZ = *(D3DXVECTOR3 *)&pFrame->mat4x4_fmrx.m[3][0];
      D3DXMatrixMultiply(&pFrame->mat4x4_fmrx, &matRotParent, &matOldLocal1);
      *(D3DXVECTOR3 *)&pFrame->mat4x4_fmrx.m[3][0] = qTransXYZ;
    }
    else
    {
      D3DXMatrixRotationAxis(&matRotLocal, pAxis, fAngleRad);
      qmemcpy(&matOldLocal2, &pFrame->mat4x4_fmrx, sizeof(matOldLocal2));
      memset(matOldLocal2.m[3], 0, 12);
      qTransXYZ2 = *(D3DXVECTOR3 *)&pFrame->mat4x4_fmrx.m[3][0];
      D3DXMatrixMultiply(&pFrame->mat4x4_fmrx, &matOldLocal2, &matRotLocal);
      *(D3DXVECTOR3 *)&pFrame->mat4x4_fmrx.m[3][0] = qTransXYZ2;
    }
  }
  else                                          // nMode = 0
  {
    D3DXMatrixRotationAxis(&pFrame->mat4x4_fmrx, pAxis, fAngleRad);
  }
  D3DXMatrixMultiply(&pFrame->mat4x4_after, &pFrame->mat4x4_fmrx, &pFrame->mat4x4_before);
}

// Recursive collision query of a CHanaFrame body against the map frame tree. Base test picks sub_4423B0 (bTestType=0) or sub_442C1C (bTestType=1); then recurses into every child frame via the T3ED expansion and ORs the hit masks. Returns non-zero when any part of pBodyFrame collides with pMapFrame.
int __cdecl hook_CheckFrameCollision_443CC5(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame, int bTestType)
{
  BOOL nHitMode; // [esp+0h] [ebp-14h]
  unsigned int nIndex; // [esp+4h] [ebp-10h]
  CHanaFrameT3EC_S354 *pFrame; // [esp+8h] [ebp-Ch] BYREF
  CHanaFrameArrayT3ED_S138 *ppFrameArray; // [esp+Ch] [ebp-8h] BYREF
  unsigned int FramesCountT3ED_408E75; // [esp+10h] [ebp-4h]
  ONCELOG("[HNH:SUB] CheckFrameCollision_443CC5\n");
 
  if ( !pBodyFrame || !pMapFrame )
    return 0;
  if ( bTestType )
    nHitMode = HitOverheadSurface_442C1C(pBodyFrame, pMapFrame);
  else
    nHitMode = HitSupportSurface_4423B0(pBodyFrame, pMapFrame);
  CHanaObj__NewFrameArrayT3EDFromChildT3EC(pMapFrame, &ppFrameArray);
  if ( !ppFrameArray )
    return nHitMode;
  FramesCountT3ED_408E75 = CHanaObj__GetFramesCountT3ED_408E75(ppFrameArray);
  for ( nIndex = 0; nIndex < FramesCountT3ED_408E75; ++nIndex )
  {
    CHanaObj__GetFrameAtIndexT3ED(ppFrameArray, nIndex, &pFrame);
    if ( pFrame )
    {
      nHitMode |= CheckFrameCollision_443CC5(pBodyFrame, pFrame, bTestType);
      if ( pFrame )
      {
        CHanaObj__Free_40C1E4(pFrame);
        pFrame = 0;
      }
    }
  }
  if ( ppFrameArray )
  {
    CHanaObj__Free_40C1E4(ppFrameArray);
    ppFrameArray = 0;
  }
  return nHitMode;
}

float &g_fHitY_4C6F48 = *(float *)0x4C6F48; // WRITABLE



// Overhead / thin-surface clearance collision base (CheckFrameCollision bTestType=1). Same mesh (type 1013) + XZ-bounds guard.
// For meshes whose name begins with 'DSurface' it computes the plane height -1.0 and snaps Y when within (fPosY-5, fPosY+10);
// for other meshes computes plane height +8.0 and snaps Y into a band, setting bHit=1. Head-bump / under-ledge handling.
// 
//   The math (now readable with the new names)
// 
//    For each triangle whose XZ bounding test contains the body position (fPosX, fPosZ) -- edge-function sign tests walking
//    kScan over 2 edges -- the Y of the triangle's plane at the body's XZ is computed via barycentric coefficients:
// 
//    fCoefX   = SUM (z-differences * vertex Y)       // coefficient of x
//    fCoefZ   = SUM (Y-differences * vertex X)       // coefficient of z
//    fPlaneD  = -(plane constant from cross terms)
//    fPlaneDenom = SUM (X cross products in XZ)      // = -normal.y*2A; 0 -> degenerate triangle
// 
//    planeY = (fCoefX*fPosX + fCoefZ*fPosZ + fPlaneD) / -fPlaneDenom
// 
//    Vertices are 32-byte stride: X +0, Y +4, Z +8, accessed through the locked buffer pointer pVerts and pVtxIndices[...].
// 
//    Two behaviors by mesh name (szNameSuffix = name tail, _strnicmp "_D_surface"):
// 
//    +----------------------+------------------+-------------------------------------------------------+--------------+
//    |      Mesh kind       |   Height used    |                       Snap band                       |   Sets bHit  |
//    +----------------------+------------------+-------------------------------------------------------+--------------+
//    | *_D_surface (thin    | fDSurfY = planeY | fPosY-5 < fDSurfY < fPosY+10 -> snap                  | no (silent   |
//    | surface)             |  - 1.0           |                                                       | snap)        |
//    +----------------------+------------------+-------------------------------------------------------+--------------+
//    | everything else      | fCeilY = planeY  | fCeilY > fPosY and fPosY + g_fHitY_4C6F48*1.5 >       | yes          |
//    | (ceiling)            | + 8.0            | fCeilY -> snap (push-down)                            |              |
//    +----------------------+------------------+-------------------------------------------------------+--------------+
// 
//    Snap = MtxMult_4069DD(pBodyFrame, scene, fPosX, newY, fPosZ) (translation-row rewrite).
BOOL __cdecl hook_HitOverheadSurface_442C1C(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)
{
  double fMinD2; // st6
  double fMinD1; // st6
  unsigned int jTri; // [esp+8h] [ebp-164h]
  unsigned int dwNumIndices_140; // [esp+Ch] [ebp-160h]
  WORD *pVtxIndices; // [esp+10h] [ebp-15Ch]
  unsigned int i; // [esp+14h] [ebp-158h]
  float fCeilY; // [esp+18h] [ebp-154h]
  float fDSurfY; // [esp+18h] [ebp-154h]
  D3DXVECTOR3 fMin; // [esp+1Ch] [ebp-150h] BYREF
  CHAR szNameSuffix[256]; // [esp+28h] [ebp-144h] BYREF
  int cchName; // [esp+128h] [ebp-44h]
  float fPosZ; // [esp+12Ch] [ebp-40h] BYREF
  CHanaMeshT3F5_S138 *pMesh; // [esp+130h] [ebp-3Ch]
  float fPosY; // [esp+134h] [ebp-38h] BYREF
  unsigned int iVtxBuf; // [esp+138h] [ebp-34h]
  float fPosX; // [esp+13Ch] [ebp-30h] BYREF
  BOOL bHit; // [esp+140h] [ebp-2Ch]
  D3DVERTEX *pVerts; // [esp+144h] [ebp-28h] BYREF
  D3DXVECTOR3 fMax; // [esp+148h] [ebp-24h] BYREF
  float fPlaneD; // [esp+154h] [ebp-18h]
  float fCoefZ; // [esp+158h] [ebp-14h]
  float fPlaneDenom; // [esp+15Ch] [ebp-10h]
  float fCoefX; // [esp+160h] [ebp-Ch]
  float fResultX; // [esp+164h] [ebp-8h]
  DWORD dwLockFlags; // [esp+168h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] HitOverheadSurface_442C1C\n");
  bHit = 0;
  pMesh = 0;
  if ( !pMapFrame->pAttachedObj_20C )
    return 0;
  if ( pMapFrame->pAttachedObj_20C->dwType != 0x3F5 )
    return 0;
  pMesh = (CHanaMeshT3F5_S138 *)pMapFrame->pAttachedObj_20C;
  ComputeMeshBounds_407E1F(pMapFrame, &fMin, &fMax);
  HanaFrame_GetRelPosition(pBodyFrame, g_pSceneFrame, &fPosX, &fPosY, &fPosZ);
  if ( fMin.x <= (double)fPosX && fMax.x >= (double)fPosX && fMin.z <= (double)fPosZ && fMax.z >= (double)fPosZ )
  {
    bHit = 0;
    for ( iVtxBuf = 0; iVtxBuf < pMesh->nCountVertT3EA_130; ++iVtxBuf )
    {
      CHanaObj__LockVtxBufT3EA_418544(pMesh->refsVertT3EA[iVtxBuf], &dwLockFlags, (LPVOID *)&pVerts);
      dwNumIndices_140 = pMesh->refsVertT3EA[iVtxBuf]->dwNumIndices_140;
      pVtxIndices = pMesh->refsVertT3EA[iVtxBuf]->pVtxIndices;
      for ( i = 0; i < dwNumIndices_140; i += 3 )
      {
        jTri = i;
        cchName = lstrlenA(pMesh->refsVertT3EA[iVtxBuf]->szName_8);
        if ( cchName < 10 )
          szNameSuffix[0] = 0;
        else
          wsprintfA(szNameSuffix, "%s", (const char *)pMesh->refsVertT3EA[iVtxBuf] + cchName - 2);
        if ( !_strnicmp(szNameSuffix, "_D_surface", 0xAu) )
        {
          if ( (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i]].x) * (fPosZ - pVerts[pVtxIndices[i]].z)
            - (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i]].z) * (fPosX - pVerts[pVtxIndices[i]].x) < 0.0 )
          {
            for ( jTri = i;
              jTri < i + 2
              && (pVerts[pVtxIndices[jTri]].x - pVerts[pVtxIndices[jTri + 1]].x)
              * (fPosZ - pVerts[pVtxIndices[jTri + 1]].z)
              - (pVerts[pVtxIndices[jTri]].z - pVerts[pVtxIndices[jTri + 1]].z)
              * (fPosX - pVerts[pVtxIndices[jTri + 1]].x) < 0.0;
              ++jTri )
            {
              ;
            }
          }
          if ( jTri == i + 2 )
          {
            fCoefX = (pVerts[pVtxIndices[i + 1]].z - pVerts[pVtxIndices[i]].z) * pVerts[pVtxIndices[i + 2]].y
              + (pVerts[pVtxIndices[i]].z - pVerts[pVtxIndices[i + 2]].z) * pVerts[pVtxIndices[i + 1]].y
              + (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i + 1]].z) * pVerts[pVtxIndices[i]].y;
            fPlaneDenom = (pVerts[pVtxIndices[i + 1]].x - pVerts[pVtxIndices[i]].x) * pVerts[pVtxIndices[i + 2]].z
              + (pVerts[pVtxIndices[i]].x - pVerts[pVtxIndices[i + 2]].x) * pVerts[pVtxIndices[i + 1]].z
              + (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i + 1]].x) * pVerts[pVtxIndices[i]].z;
            fCoefZ = (pVerts[pVtxIndices[i + 1]].y - pVerts[pVtxIndices[i]].y) * pVerts[pVtxIndices[i + 2]].x
              + (pVerts[pVtxIndices[i]].y - pVerts[pVtxIndices[i + 2]].y) * pVerts[pVtxIndices[i + 1]].x
              + (pVerts[pVtxIndices[i + 2]].y - pVerts[pVtxIndices[i + 1]].y) * pVerts[pVtxIndices[i]].x;
            fMinD1 = (pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i + 1]].z
              - pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i + 2]].z)
              * pVerts[pVtxIndices[i]].x;
            fPlaneD = -pVerts[pVtxIndices[i + 2]].x
              * (pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i]].z
                - pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 1]].z)
              - (pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 2]].z
                - pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i]].z)
              * pVerts[pVtxIndices[i + 1]].x
              - fMinD1;
            if ( fPlaneDenom == 0.0 )
            {
              fResultX = -1.0;
              fDSurfY = -1.0;
            }
            else
            {
              fDSurfY = (fCoefX * fPosX + fCoefZ * fPosZ + fPlaneD) / -fPlaneDenom - 1.0;
            }
            if ( fPosY - 5.0 < fDSurfY && fPosY + 10.0 > fDSurfY )
              MtxMult_4069DD(pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPosX, fDSurfY, fPosZ);
          }
        }
        else
        {
          if ( (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i]].x) * (fPosZ - pVerts[pVtxIndices[i]].z)
            - (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i]].z) * (fPosX - pVerts[pVtxIndices[i]].x) > 0.0 )
          {
            for ( jTri = i;
              jTri < i + 2
              && (pVerts[pVtxIndices[jTri]].x - pVerts[pVtxIndices[jTri + 1]].x)
              * (fPosZ - pVerts[pVtxIndices[jTri + 1]].z)
              - (pVerts[pVtxIndices[jTri]].z - pVerts[pVtxIndices[jTri + 1]].z)
              * (fPosX - pVerts[pVtxIndices[jTri + 1]].x) > 0.0;
              ++jTri )
            {
              ;
            }
          }
          if ( jTri == i + 2 )
          {
            fCoefX = (pVerts[pVtxIndices[i + 1]].z - pVerts[pVtxIndices[i + 2]].z) * pVerts[pVtxIndices[i]].y
              + (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i]].z) * pVerts[pVtxIndices[i + 1]].y
              + (pVerts[pVtxIndices[i]].z - pVerts[pVtxIndices[i + 1]].z) * pVerts[pVtxIndices[i + 2]].y;
            fPlaneDenom = (pVerts[pVtxIndices[i + 1]].x - pVerts[pVtxIndices[i + 2]].x) * pVerts[pVtxIndices[i]].z
              + (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i]].x) * pVerts[pVtxIndices[i + 1]].z
              + (pVerts[pVtxIndices[i]].x - pVerts[pVtxIndices[i + 1]].x) * pVerts[pVtxIndices[i + 2]].z;
            fCoefZ = (pVerts[pVtxIndices[i + 1]].y - pVerts[pVtxIndices[i + 2]].y) * pVerts[pVtxIndices[i]].x
              + (pVerts[pVtxIndices[i + 2]].y - pVerts[pVtxIndices[i]].y) * pVerts[pVtxIndices[i + 1]].x
              + (pVerts[pVtxIndices[i]].y - pVerts[pVtxIndices[i + 1]].y) * pVerts[pVtxIndices[i + 2]].x;
            fMinD2 = (pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 1]].z
              - pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i]].z)
              * pVerts[pVtxIndices[i + 2]].x;
            fPlaneD = -pVerts[pVtxIndices[i]].x
              * (pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i + 2]].z
                - pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i + 1]].z)
              - (pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i]].z
                - pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 2]].z)
              * pVerts[pVtxIndices[i + 1]].x
              - fMinD2;
            if ( fPlaneDenom == 0.0 )
            {
              fResultX = 8.0;
              fCeilY = 8.0;
            }
            else
            {
              fCeilY = (fCoefX * fPosX + fCoefZ * fPosZ + fPlaneD) / -fPlaneDenom + 8.0;
            }
            bHit = 1;
            if ( fCeilY > (double)fPosY && g_fHitY_4C6F48 * 3.0 / 2.0 + fPosY > fCeilY )
              MtxMult_4069DD(pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPosX, fCeilY, fPosZ);
          }
        }
      }
      CHanaObj__UnlockVrxBufT3EA_41858E(pMesh->refsVertT3EA[iVtxBuf]);
    }
  }
  return bHit;
}

// Support-surface (floor/wall) collision base. pMapFrame[131] must be a mesh child (type 1013); ComputeMeshBounds gives its min/max box and the body's XZ must lie inside it (Y padded by 4 + flt_4C6F48). For each triangle, if the body's XZ point is inside the triangle, solves the triangle's up-axis (Y) plane height (fPlaneY);
// if the body's Y is within 10 of that plane, the body is resting on it -> snaps Y via MtxMult and returns a surface-type code: 1 generic, 2/3/4 decoded from the mesh-name suffix letters (_X/_W -> 2, _Z -> 3, _Y -> 4). Used with CheckFrameCollision bTestType=0.
// 
// 
//   Comparison with HitOverheadSurface_442C1C
// 
//   +--------------+------------------------------+-----------------------------------------------+
//   |              |        Support (this)        |                   Overhead                    |
//   +--------------+------------------------------+-----------------------------------------------+
//   | Y test       | |fPosY - plane| < 10 snap-up | plane+8 push-down band / plane-1 thin-surface |
//   +--------------+------------------------------+-----------------------------------------------+
//   | Edge epsilon | > 0.01 (CW winding)          | < 0 / > 0 (opposite winding for _D_surface)   |
//   +--------------+------------------------------+-----------------------------------------------+
//   | Name suffix  | last 2 chars -> surface code | 10-char _D_surface compare                    |
//   +--------------+------------------------------+-----------------------------------------------+
//   | Returns      | 0-4 surface type             | BOOL hit                                      |
//   +--------------+------------------------------+-----------------------------------------------+
int __cdecl hook_HitSupportSurface_4423B0(CHanaFrameT3EC_S354 *pBodyFrame, CHanaFrameT3EC_S354 *pMapFrame)
{
  double fMinD; // st6
  CHanaVertT3EA_S1FC *pVertEA; // esi
  int cchName; // eax
  CHAR szNameSfx[20]; // [esp+8h] [ebp-74h] BYREF

  unsigned int jTri; // [esp+1Ch] [ebp-60h]
  unsigned int dwNumIndices_140; // [esp+20h] [ebp-5Ch]
  WORD *pVtxIndices; // [esp+24h] [ebp-58h]
  unsigned int i; // [esp+28h] [ebp-54h]
  float fPlaneY; // [esp+2Ch] [ebp-50h]
  D3DXVECTOR3 fMin; // [esp+30h] [ebp-4Ch] BYREF
  float fPosZ; // [esp+3Ch] [ebp-40h] BYREF
  CHanaMeshT3F5_S138 *pMesh; // [esp+40h] [ebp-3Ch]
  float fPosY; // [esp+44h] [ebp-38h] BYREF
  unsigned int iVtxBuf; // [esp+48h] [ebp-34h]
  float fPosX; // [esp+4Ch] [ebp-30h] BYREF
  int nSurfaceHit; // [esp+50h] [ebp-2Ch]
  D3DVERTEX *pVerts; // [esp+54h] [ebp-28h] BYREF
  D3DXVECTOR3 fMax; // [esp+58h] [ebp-24h] BYREF
  float fPlaneD; // [esp+64h] [ebp-18h]
  float fCoefZ; // [esp+68h] [ebp-14h]
  float fPlaneDenom; // [esp+6Ch] [ebp-10h]
  float fCoefX; // [esp+70h] [ebp-Ch]
  float fPlaneYAtZ1; // [esp+74h] [ebp-8h]
  DWORD dwLockFlags; // [esp+78h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] HitSupportSurface_4423B0\n");
  
  nSurfaceHit = 0;
  pMesh = 0;
  if ( !pMapFrame->pAttachedObj_20C )
    return 0;
  if ( pMapFrame->pAttachedObj_20C->dwType != 0x3F5 )
    return 0;
  pMesh = (CHanaMeshT3F5_S138 *)pMapFrame->pAttachedObj_20C;
  ComputeMeshBounds_407E1F(pMapFrame, &fMin, &fMax);
  HanaFrame_GetRelPosition(pBodyFrame, g_pSceneFrame, &fPosX, &fPosY, &fPosZ);
  if ( fMin.x > (double)fPosX
    || fMax.x < (double)fPosX
    || fMin.y - 4.0 > fPosY
    || fMax.y + g_fHitY_4C6F48 + 4.0 < fPosY
    || fMin.z > (double)fPosZ
    || fMax.z < (double)fPosZ )
  {
    return nSurfaceHit;
  }
  nSurfaceHit = 0;
  for ( iVtxBuf = 0; iVtxBuf < pMesh->nCountVertT3EA_130; ++iVtxBuf )
  {
    CHanaObj__LockVtxBufT3EA_418544(pMesh->refsVertT3EA[iVtxBuf], &dwLockFlags, (LPVOID *)&pVerts);
    dwNumIndices_140 = pMesh->refsVertT3EA[iVtxBuf]->dwNumIndices_140;
    pVtxIndices = pMesh->refsVertT3EA[iVtxBuf]->pVtxIndices;
    for ( i = 0; i < dwNumIndices_140; i += 3 )
    {
      jTri = i;
      if ( (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i]].x) * (fPosZ - pVerts[pVtxIndices[i]].z)
        - (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i]].z) * (fPosX - pVerts[pVtxIndices[i]].x) > 0.0099999998 )
      {
        for ( jTri = i;
          jTri < i + 2
          && (pVerts[pVtxIndices[jTri]].x - pVerts[pVtxIndices[jTri + 1]].x)
          * (fPosZ - pVerts[pVtxIndices[jTri + 1]].z)
          - (pVerts[pVtxIndices[jTri]].z - pVerts[pVtxIndices[jTri + 1]].z)
          * (fPosX - pVerts[pVtxIndices[jTri + 1]].x) > 0.0099999998;
          ++jTri )
        {
          ;
        }
      }
      if ( jTri == i + 2 )
      {
        fCoefX = (pVerts[pVtxIndices[i + 1]].z - pVerts[pVtxIndices[i + 2]].z) * pVerts[pVtxIndices[i]].y
          + (pVerts[pVtxIndices[i + 2]].z - pVerts[pVtxIndices[i]].z) * pVerts[pVtxIndices[i + 1]].y
          + (pVerts[pVtxIndices[i]].z - pVerts[pVtxIndices[i + 1]].z) * pVerts[pVtxIndices[i + 2]].y;
        fPlaneDenom = (pVerts[pVtxIndices[i + 1]].x - pVerts[pVtxIndices[i + 2]].x) * pVerts[pVtxIndices[i]].z
          + (pVerts[pVtxIndices[i + 2]].x - pVerts[pVtxIndices[i]].x) * pVerts[pVtxIndices[i + 1]].z
          + (pVerts[pVtxIndices[i]].x - pVerts[pVtxIndices[i + 1]].x) * pVerts[pVtxIndices[i + 2]].z;
        fCoefZ = (pVerts[pVtxIndices[i + 1]].y - pVerts[pVtxIndices[i + 2]].y) * pVerts[pVtxIndices[i]].x
          + (pVerts[pVtxIndices[i + 2]].y - pVerts[pVtxIndices[i]].y) * pVerts[pVtxIndices[i + 1]].x
          + (pVerts[pVtxIndices[i]].y - pVerts[pVtxIndices[i + 1]].y) * pVerts[pVtxIndices[i + 2]].x;
        fMinD = (pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 1]].z
          - pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i]].z)
          * pVerts[pVtxIndices[i + 2]].x;
        fPlaneD = -pVerts[pVtxIndices[i]].x
          * (pVerts[pVtxIndices[i + 1]].y * pVerts[pVtxIndices[i + 2]].z
            - pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i + 1]].z)
          - (pVerts[pVtxIndices[i + 2]].y * pVerts[pVtxIndices[i]].z
            - pVerts[pVtxIndices[i]].y * pVerts[pVtxIndices[i + 2]].z)
          * pVerts[pVtxIndices[i + 1]].x
          - fMinD;
        if ( fPlaneDenom == 0.0 )
        {
          fPlaneYAtZ1 = 0.0;
          fPlaneY = 0.0;
        }
        else
        {
          fPlaneY = (fCoefX * fPosX + fCoefZ * fPosZ + fPlaneD) / -fPlaneDenom;
          fPlaneYAtZ1 = (fCoefX * fPosX + (fPosZ + 1.0) * fCoefZ + fPlaneD) / -fPlaneDenom;
        }
        if ( fPosY + 10.0 > fPlaneY && fPosY - 10.0 < fPlaneY )
        {
          MtxMult_4069DD(pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fPosX, fPlaneY, fPosZ);
          pVertEA = pMesh->refsVertT3EA[iVtxBuf];
          cchName = lstrlenA(pVertEA->szName_8);
          wsprintfA(szNameSfx, "%s", (const char *)pVertEA + cchName - 2);
          nSurfaceHit = 1;
          if ( szNameSfx[0] == '_' )
          {
            if ( szNameSfx[1] == 'x' || szNameSfx[1] == 'X' || szNameSfx[1] == 'w' || szNameSfx[1] == 'W' )
              nSurfaceHit = 2;
            if ( szNameSfx[1] == 'z' || szNameSfx[1] == 'Z' )
              nSurfaceHit = 3;
            if ( szNameSfx[1] == 'y' || szNameSfx[1] == 'Y' )
              nSurfaceHit = 4;
          }
          break;
        }
      }
    }
    CHanaObj__UnlockVrxBufT3EA_41858E(pMesh->refsVertT3EA[iVtxBuf]);
    if ( nSurfaceHit )
      break;
  }
  return nSurfaceHit;
}

// ComputeMeshBounds - compute the min and max (float[3] each) axis-aligned bounding box of a mesh/frame's geometry. Outputs min into the first out param and max into the second. Feeds the fast-reject boxes in the collision kernels.
void __cdecl hook_ComputeMeshBounds_407E1F(CHanaFrameT3EC_S354 *pMapFrame, D3DXVECTOR3 *fMin, D3DXVECTOR3 *fMax)
{
  ONCELOG("[HNH:SUB] ComputeMeshBounds_407E1F\n");
  
  *fMin = pMapFrame->worldAABBMin;
  *fMax = pMapFrame->worldAABBMax;
}

// Per-mesh base for TestMoveCollision_4443FC: would the body, moving horizontally to (fNewX,fNewY,fNewZ), hit a wall? Guards on the mesh child (type 1013) and its XZ bounds containing the target. For each triangle edge, tests whether the movement line in the XZ plane crosses the edge within the triangle (barycentric params in [0,1]); if so, and the Y clearance condition holds (skipped when bIgnoreY is set), the move is blocked -> bBlocked=1 and the body is snapped via MtxMult to the contact point.
bool __cdecl hook_TestBodyMoveHit_443EC4(
  CHanaFrameT3EC_S354 *pBodyFrame,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fNewX,
  float fNewY,
  float fNewZ,
  int bIgnoreY)
{
  double fCheckDist; // st7
  float fDist; // [esp+8h] [ebp-98h]
  unsigned int kNext; // [esp+18h] [ebp-88h]
  unsigned int jTri; // [esp+1Ch] [ebp-84h]
  unsigned int dwNumIndices_140; // [esp+20h] [ebp-80h]
  WORD *pVtxIndices; // [esp+24h] [ebp-7Ch]
  float fMoveT; // [esp+28h] [ebp-78h]
  unsigned int i; // [esp+2Ch] [ebp-74h]
  D3DXVECTOR3 fMin; // [esp+30h] [ebp-70h] BYREF
  float fContactZ; // [esp+3Ch] [ebp-64h]
  float e2z; // [esp+40h] [ebp-60h]
  float fCurZ; // [esp+44h] [ebp-5Ch] BYREF
  float fDenom; // [esp+48h] [ebp-58h]
  float fContactX; // [esp+4Ch] [ebp-54h]
  float e1z; // [esp+50h] [ebp-50h]
  CHanaMeshT3F5_S138 *pMesh; // [esp+54h] [ebp-4Ch]
  float fCurY; // [esp+58h] [ebp-48h] BYREF
  unsigned int iVtxBuf; // [esp+5Ch] [ebp-44h]
  float fCurX; // [esp+60h] [ebp-40h] BYREF
  float fIntZ; // [esp+64h] [ebp-3Ch]
  bool bBlocked; // [esp+68h] [ebp-38h]
  float e2y; // [esp+6Ch] [ebp-34h]
  D3DVERTEX *pVerts; // [esp+70h] [ebp-30h] BYREF
  float fBestDist; // [esp+74h] [ebp-2Ch]
  float fIntX; // [esp+78h] [ebp-28h]
  float y; // [esp+7Ch] [ebp-24h]
  D3DXVECTOR3 fMax; // [esp+80h] [ebp-20h] BYREF
  float e2x; // [esp+8Ch] [ebp-14h]
  float e1x; // [esp+90h] [ebp-10h]
  float fProjT; // [esp+94h] [ebp-Ch]
  float fEdgeT; // [esp+98h] [ebp-8h]
  DWORD dwLockFlags; // [esp+9Ch] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] TestBodyMoveHit_443EC4\n");
  bBlocked = 0;
  fContactX = 0.0;
  fContactZ = 0.0;
  fBestDist = 0.0;
  fIntX = 1000.0;
  fIntZ = 100.0;
  pMesh = 0;
  if ( !pMapFrame->pAttachedObj_20C )
    return 0;
  if ( pMapFrame->pAttachedObj_20C->dwType != 0x3F5 )
    return 0;
  pMesh = (CHanaMeshT3F5_S138 *)pMapFrame->pAttachedObj_20C;
  ComputeMeshBounds_407E1F(pMapFrame, &fMin, &fMax);
  HanaFrame_GetRelPosition(pBodyFrame, g_pSceneFrame, &fCurX, &fCurY, &fCurZ);
  if ( fMin.x <= (double)fNewX
    && fMax.x >= (double)fNewX
    && fMin.y - 4.0 <= fNewY
    && fMax.y + g_fHitY_4C6F48 + 4.0 >= fNewY
    && fMin.z <= (double)fNewZ
    && fMax.z >= (double)fNewZ )
  {
    bBlocked = 0;
    for ( iVtxBuf = 0; iVtxBuf < pMesh->nCountVertT3EA_130; ++iVtxBuf )
    {
      CHanaObj__LockVtxBufT3EA_418544(pMesh->refsVertT3EA[iVtxBuf], &dwLockFlags, (LPVOID *)&pVerts);
      dwNumIndices_140 = pMesh->refsVertT3EA[iVtxBuf]->dwNumIndices_140;
      pVtxIndices = pMesh->refsVertT3EA[iVtxBuf]->pVtxIndices;
      for ( i = 0; i < dwNumIndices_140; i += 3 )
      {
        for ( jTri = i; jTri < i + 3; ++jTri )
        {
          kNext = jTri + 1;
          if ( jTri + 1 == i + 3 )
            kNext = i;
          e1x = pVerts[pVtxIndices[jTri]].x;
          y = pVerts[pVtxIndices[jTri]].y;
          e1z = pVerts[pVtxIndices[jTri]].z;
          e2x = pVerts[pVtxIndices[kNext]].x;
          e2y = pVerts[pVtxIndices[kNext]].y;
          e2z = pVerts[pVtxIndices[kNext]].z;
          fDenom = (fNewX - fCurX) * (e2z - e1z) - (fNewZ - fCurZ) * (e2x - e1x);
          if ( fDenom != 0.0 )
          {
            fMoveT = ((fCurZ - e1z) * (e2x - e1x) - (fCurX - e1x) * (e2z - e1z)) / fDenom;
            fEdgeT = ((fCurZ - e1z) * (fNewX - fCurX) - (fCurX - e1x) * (fNewZ - fCurZ)) / fDenom;
            if ( (bIgnoreY || (e2y - y) * (e2y - y) + 100.0 > (fNewY - fCurY) * (fNewY - fCurY))
              && fMoveT >= 0.0
              && fMoveT <= 1.0
              && fEdgeT >= 0.0
              && fEdgeT <= 1.0 )
            {
              fDenom = (e2x - e1x) * (e2x - e1x) + (e2z - e1z) * (e2z - e1z);
              if ( fDenom != 0.0 )
              {
                fProjT = ((fCurX - e1x) * (e2x - e1x) + (fCurZ - e1z) * (e2z - e1z)) / fDenom;
                fContactX = (e2x - e1x) * fProjT + e1x;
                fContactZ = (e2z - e1z) * fProjT + e1z;
                if ( !bIgnoreY )
                {
                  fContactX = fContactX - fCurX + fContactX - fCurX + fContactX;
                  fContactZ = fContactZ - fCurZ + fContactZ - fCurZ + fContactZ;
                }
                fIntX = (fCurX - fNewX) * fMoveT + fNewX;
                fIntZ = (fCurZ - fNewZ) * fMoveT + fNewZ;
                fCheckDist = sqrt((fIntX - fCurX) * (fIntX - fCurX) + (fIntZ - fCurZ) * (fIntZ - fCurZ));
                if ( fCheckDist > fBestDist )
                {
                  bBlocked = 1;
                  MtxMult_4069DD(pBodyFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fContactX, fCurY, fContactZ);
                  fDist = sqrt((fIntX - fCurX) * (fIntX - fCurX) + (fIntZ - fCurZ) * (fIntZ - fCurZ));
                  fBestDist = fDist;
                }
              }
            }
          }
        }
      }
      CHanaObj__UnlockVrxBufT3EA_41858E(pMesh->refsVertT3EA[iVtxBuf]);
    }
  }
  return bBlocked;
}

// Recursive 'would a move to (fNewX,fNewY,fNewZ) collide' test of the body frame against the map. Base test sub_443EC4; recurses into child frames until a hit is found. Returns non-zero if the proposed move is blocked.
bool __cdecl hook_TestMoveCollision_4443FC(
  CHanaFrameT3EC_S354 *pBodyFrame,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fNewX,
  float fNewY,
  float fNewZ,
  int bTestType)
{
  bool result; // al
  bool bAnyBlocked; // [esp+0h] [ebp-14h]
  unsigned int iFrame; // [esp+4h] [ebp-10h]
  CHanaFrameT3EC_S354 *pChildFrame; // [esp+8h] [ebp-Ch] BYREF
  CHanaFrameArrayT3ED_S138 *ppFrameArray; // [esp+Ch] [ebp-8h] BYREF
  unsigned int nFrames; // [esp+10h] [ebp-4h]
  ONCELOG("[HNH:SUB] TestMoveCollision_4443FC\n");
  if ( !pBodyFrame || !pMapFrame )
    return 0;
  result = TestBodyMoveHit_443EC4(pBodyFrame, pMapFrame, fNewX, fNewY, fNewZ, bTestType);
  bAnyBlocked = result;
  if ( !result )
  {
    CHanaObj__NewFrameArrayT3EDFromChildT3EC(pMapFrame, &ppFrameArray);
    if ( ppFrameArray )
    {
      nFrames = CHanaObj__GetFramesCountT3ED_408E75(ppFrameArray);
      for ( iFrame = 0; iFrame < nFrames; ++iFrame )
      {
        CHanaObj__GetFrameAtIndexT3ED(ppFrameArray, iFrame, &pChildFrame);
        if ( pChildFrame )
        {
          bAnyBlocked = TestMoveCollision_4443FC(pBodyFrame, pChildFrame, fNewX, fNewY, fNewZ, bTestType);
          if ( pChildFrame )
          {
            CHanaObj__Free_40C1E4(pChildFrame);
            pChildFrame = 0;
          }
          if ( bAnyBlocked )
            break;
        }
      }
      if ( ppFrameArray )
      {
        CHanaObj__Free_40C1E4(ppFrameArray);
        ppFrameArray = 0;
      }
      return bAnyBlocked;
    }
    else
    {
      return bAnyBlocked;
    }
  }
  return result;
}

void __cdecl hook_BuildMapCollisionGrid_445B12(CHanaFrameT3EC_S354 *pMapFrame, char **pOutGrid)
{
  char bHit; // al
  float fProbeX; // [esp+0h] [ebp-20h]
  float fProbeZ; // [esp+8h] [ebp-18h]
  int nCol; // [esp+14h] [ebp-Ch]
  CHanaFrameT3EC_S354 *pProbeFrame; // [esp+18h] [ebp-8h] BYREF
  int nRow; // [esp+1Ch] [ebp-4h]
  ONCELOG("[HNH:SUB] BuildMapCollisionGrid_445B12\n");

  pProbeFrame = 0;
  memset(g_pMapCollGrid, 0, sizeof(g_pMapCollGrid));
  CHanaObj__NewFrameT3EC_S354((CHanaFrameT3EC_S354 *)g_pSceneFrame, &pProbeFrame);
  for ( nRow = 1; nRow < 200; ++nRow )
  {
    for ( nCol = 1; nCol < 200; ++nCol )
    {
      fProbeZ = (double)(nRow - 100) * 5.0;
      fProbeX = (double)(nCol - 100) * 5.0;
      MtxMult_4069DD(pProbeFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fProbeX, 0.0, fProbeZ);
      bHit = CheckFrameCollision_443CC5(pProbeFrame, pMapFrame, 1);
      g_pMapCollGrid[200 * nRow + nCol] = bHit;
    }
  }
  if ( pProbeFrame )
  {
    CHanaObj__Free_40C1E4(pProbeFrame);
    pProbeFrame = 0;
  }
  *pOutGrid = g_pMapCollGrid;
}

// HanaGrid_DFSPath -- depth-limited DFS that enumerates every walkable path branch
// 
//   The recursive worker behind HanaGrid_FloodFill. For a starting cell it explores cardinal directions only (W->E->S->N, no
//   diagonals -- those exist only in FloodFill's direction tables) and records every distinct path endpoint as a
//   "candidate" in the global tables.
bool __cdecl hook_HanaGrid_DFSPath(char *pGrid, int nCellX, int nCellY, int nDepth, __int16 nSteps, int bIgnoreBlocked)
{
  int nDepthLeft; // [esp+14h] [ebp+14h]
  __int16 nStepsNext; // [esp+18h] [ebp+18h]

  ONCELOG("[HNH:SUB] HanaGrid_DFSPath_445C12\n");

  nDepthLeft = nDepth - 1;
  if ( nCellX <= -100
    || nCellX >= 100
    || nCellY <= -100
    || nCellY >= 100
    || pGrid[200 * nCellY + 20100 + nCellX] != 1 && !bIgnoreBlocked )
  {
    return 0;
  }
  if ( nDepthLeft > 0 )
  {
    g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nSteps].x = nCellX;
    g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nSteps].y = nCellY;
    nStepsNext = nSteps + 1;
    if ( !HanaGrid_DFSPath(pGrid, nCellX - 1, nCellY, nDepthLeft, nStepsNext, bIgnoreBlocked) && !bIgnoreBlocked )
    {
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].x = nCellX;
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].y = nCellY;
      g_arrFloodPathLen_523CAC[g_nFloodCandidates_52ED14] = nDepthLeft;
      g_arrFloodPathCnt_52E500[g_nFloodCandidates_52ED14] = nStepsNext;
      memcpy(
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14 + 1],
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14],
        8 * nStepsNext + 8);                    // (1+nStepsNext) * sizeof(POINT)
      ++g_nFloodCandidates_52ED14;
    }
    if ( !HanaGrid_DFSPath(pGrid, nCellX + 1, nCellY, nDepthLeft, nStepsNext, bIgnoreBlocked) && !bIgnoreBlocked )
    {
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].x = nCellX;
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].y = nCellY;
      g_arrFloodPathLen_523CAC[g_nFloodCandidates_52ED14] = nDepthLeft;
      g_arrFloodPathCnt_52E500[g_nFloodCandidates_52ED14] = nStepsNext;
      memcpy(
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14 + 1],
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14],
        sizeof(POINT[5]));
      ++g_nFloodCandidates_52ED14;
    }
    if ( !HanaGrid_DFSPath(pGrid, nCellX, nCellY - 1, nDepthLeft, nStepsNext, bIgnoreBlocked) && !bIgnoreBlocked )
    {
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].x = nCellX;
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].y = nCellY;
      g_arrFloodPathLen_523CAC[g_nFloodCandidates_52ED14] = nDepthLeft;
      g_arrFloodPathCnt_52E500[g_nFloodCandidates_52ED14] = nStepsNext;
      memcpy(
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14 + 1],
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14],
        sizeof(POINT[5]));
      ++g_nFloodCandidates_52ED14;
    }
    if ( !HanaGrid_DFSPath(pGrid, nCellX, nCellY + 1, nDepthLeft, nStepsNext, bIgnoreBlocked) && !bIgnoreBlocked )
    {
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].x = nCellX;
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nStepsNext].y = nCellY;
      g_arrFloodPathLen_523CAC[g_nFloodCandidates_52ED14] = nDepthLeft;
      g_arrFloodPathCnt_52E500[g_nFloodCandidates_52ED14] = nStepsNext;
      memcpy(
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14 + 1],
        g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14],
        sizeof(POINT[5]));
      ++g_nFloodCandidates_52ED14;
    }
    return 1;
  }
  else
  {
    g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nSteps].x = nCellX;
    g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14][nSteps].y = nCellY;
    g_arrFloodPathLen_523CAC[g_nFloodCandidates_52ED14] = nDepthLeft;
    g_arrFloodPathCnt_52E500[g_nFloodCandidates_52ED14] = nSteps;
    memcpy(
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14 + 1],
      g_arrFloodPathXY_5244F8[g_nFloodCandidates_52ED14],
      8 * nSteps + 8);                          // (1+nSteps) * sizeof(POINT)
    ++g_nFloodCandidates_52ED14;
    return 1;
  }
}

// HanaGrid_FloodFill -- foe steering on the 200x200 occupancy grid (built by BuildMapCollisionGrid_445B12).
//
//   Grid layout: byte at pGrid[200*(cy+100) + cx+100], cells -100..99 (each cell = 5 world units; ==1 = walkable).
//
//   Direction tables (the decoded bit patterns):
//   jDir:        0    1    2    3    4      5      6      7
//   arrDirDx:   -1   +1    0    0   -1     +1     -1     +1
//   arrDirDy:    0    0   -1   +1   -1     -1     +1     +1
//   arrDirFx:  -1.0  1.0  0.0  0.0 -0.58  0.58  -0.58  0.58
//   arrDirFz:   0.0  0.0 -1.0  1.0 -0.58 -0.58  0.58  0.58
//   Cardinals first (0-3), diagonals after (4-7) at 0.58 magnitude. Entry [8] is zeroed by the tail memset
//   and read when no direction improved -> nBestDir==8 yields a (0,0) step.
//
//   Algorithm:
//   save hero-cell byte; if nMode==0, clear it (hero's cell isn't a wall while wandering)
//   seed ppOutPathPts with 5 copies of the foe cell; ptBestEnd = foe cell
//
//   for jDir in 0..7:                        // try each neighbor direction
//       if foe cell on-grid & walkable: seed candidate 0 (foe cell itself), 1 candidate
//       if jDir==4 && !nMode: nDepth--        // halfway through, shorten search
//       HanaGrid_DFSPath(grid, cx+dx, cy+dy, nDepth, 0, 0)   // fills g_arrFloodPathXY/Len/Cnt + g_nFloodCandidates
//       nBestDx/Dy = nMode ? (9999,9999) : (0,0)   // approach: min-dist seed / wander: max-dist seed
//       for each candidate i:
//           nEndDx/nEndDy = candidate endpoint - hero cell
//           keep extreme (min if approach, max if wander); tie-break: longer g_arrFloodPathLen[i] > nBestLen
//       if the round's extreme beats the incumbent (same tie-break):
//           adopt its endpoint, nBestDir = jDir, nBestLen, and copy its 5 waypoints + count out
//
//   if no direction improved: nBestDir = 8 (-> zero step), nBestLen = 5
//   restore hero-cell byte
//   *pfOutStep = (arrDirFx[nBestDir], arrDirFz[nBestDir])   // (0,0) when nBestDir==8
//   *pnFoeCell = ptBestEnd; *pnDepth = nBestLen
D3DXVECTOR2 *__cdecl hook_HanaGrid_FloodFill_4460AD(
  D3DXVECTOR2 *pfOutStep,        // OUT: unit-ish step direction (X, Z) to move this frame
  char *pGrid,                   // occupancy grid (200-byte rows, centered at +100)
  POINT *pnFoeCell,              // IN/OUT: foe cell (x, y) -- updated to the path's end cell
  POINT pnHeroCell,              // hero cell x, y
  int *pnDepth,                  // IN/OUT: search depth (starts 5 from nFloodParm)
  int nMode,                     // 0 = wander (pick endpoint FARTHEST from hero), else approach (NEAREST)
  POINT *ppOutPathPts,           // OUT: up to 5 waypoint cells (x,y pairs)
  WORD *pnOutNumbPathPts)        // OUT: waypoint count
{
  int nDepth; // [esp+54h] [ebp-12Ch]
  int bIgnoredBlocked; // [esp+A4h] [ebp-DCh]
  int nEndDy; // [esp+A8h] [ebp-D8h]
  int nEndDx; // [esp+ACh] [ebp-D4h]
  int jDir; // [esp+B0h] [ebp-D0h]
  int nBestDir; // [esp+B4h] [ebp-CCh]
  __int16 i; // [esp+B8h] [ebp-C8h]
  char bHeroCellSaved; // [esp+BCh] [ebp-C4h]
  POINT pnCell; // [esp+C0h] [ebp-C0h]
  D3DXVECTOR2 fOutDir; // [esp+C8h] [ebp-B8h]
  int nBestDy; // [esp+D0h] [ebp-B0h]
  int nBestDx; // [esp+D4h] [ebp-ACh]
  __int16 nBestLen; // [esp+D8h] [ebp-A8h]
  __int16 iBestCand; // [esp+174h] [ebp-Ch]
  POINT ptBestEnd; // [esp+178h] [ebp-8h]
  float arrDirFx[19]; // [esp+8h] [ebp-178h]
  int arrDirDx[19]; // [esp+58h] [ebp-128h]
  float arrDirFz[19]; // [esp+DCh] [ebp-A4h]
  int arrDirDy[19]; // [esp+128h] [ebp-58h]

  ONCELOG("[HNH:SUB] HanaGrid_FloodFill_4460AD\n");

  arrDirDx[0] = -1;
  arrDirDx[1] = 1;
  arrDirDx[2] = 0;
  arrDirDx[3] = 0;
  arrDirDx[4] = -1;
  arrDirDx[5] = 1;
  arrDirDx[6] = -1;
  arrDirDx[7] = 1;
  memset(&arrDirDx[8], 0, 44);
  arrDirDy[0] = 0;
  arrDirDy[1] = 0;
  arrDirDy[2] = -1;
  arrDirDy[3] = 1;
  arrDirDy[4] = -1;
  arrDirDy[5] = -1;
  arrDirDy[6] = 1;
  arrDirDy[7] = 1;
  memset(&arrDirDy[8], 0, 44);
  arrDirFx[0] = -1.0;
  arrDirFx[1] = 1.0;
  arrDirFx[2] = 0.0;
  arrDirFx[3] = 0.0;
  arrDirFx[4] = -0.57999998;
  arrDirFx[5] = 0.57999998;
  arrDirFx[6] = -0.57999998;
  arrDirFx[7] = 0.57999998;
  memset(&arrDirFx[8], 0, 44);
  arrDirFz[0] = 0.0;
  arrDirFz[1] = 0.0;
  arrDirFz[2] = -1.0;
  arrDirFz[3] = 1.0;
  arrDirFz[4] = -0.57999998;
  arrDirFz[5] = -0.57999998;
  arrDirFz[6] = 0.57999998;
  arrDirFz[7] = 0.57999998;
  memset(&arrDirFz[8], 0, 44);
  nBestDir = 8;
  nDepth = *pnDepth;
  nBestLen = 0;
  bIgnoredBlocked = 0;
  fOutDir.x = 0.0;
  fOutDir.y = 0.0;
  pnCell = *pnFoeCell;
  bHeroCellSaved = pGrid[200 * pnHeroCell.y + 20100 + pnHeroCell.x];
  if ( !nMode )
    pGrid[200 * pnHeroCell.y + 20100 + pnHeroCell.x] = 0;
  ptBestEnd = pnCell;
  for ( i = 0; i < 5; ++i )
    ppOutPathPts[i] = pnCell;
  *pnOutNumbPathPts = 0;
  for ( jDir = 0; jDir < 8; ++jDir )
  {
    g_nFloodCandidates_52ED14 = 0;
    if ( pnCell.x > -100
      && pnCell.x < 100
      && pnCell.y > -100
      && pnCell.y < 100
      && pGrid[200 * pnCell.y + 20100 + pnCell.x] == 1 )
    {
      for ( i = 0; i < 5; ++i )
      {
        g_arrFloodPathXY_5244F8[0][i].x = pnCell.x;
        g_arrFloodPathXY_5244F8[0][i].y = pnCell.y;
      }
      g_arrFloodPathLen_523CAC[0] = nDepth;
      g_arrFloodPathCnt_52E500[0] = 0;
      g_nFloodCandidates_52ED14 = 1;
    }
    if ( jDir == 4 && !nMode )
      --nDepth;
    HanaGrid_DFSPath(pGrid, arrDirDx[jDir] + pnCell.x, arrDirDy[jDir] + pnCell.y, nDepth, 0, bIgnoredBlocked);
    if ( nMode )
    {
      nBestDx = 9999;
      nBestDy = 9999;
    }
    else
    {
      nBestDx = 0;
      nBestDy = 0;
    }
    iBestCand = 0;
    for ( i = 0; i < g_nFloodCandidates_52ED14; ++i )
    {
      nEndDx = g_arrFloodPathXY_5244F8[i][g_arrFloodPathCnt_52E500[i]].x - pnHeroCell.x;
      nEndDy = g_arrFloodPathXY_5244F8[i][g_arrFloodPathCnt_52E500[i]].y - pnHeroCell.y;
      if ( nMode )
      {
        if ( nBestDy * nBestDy + nBestDx * nBestDx >= nEndDy * nEndDy + nEndDx * nEndDx
          && (nBestDy * nBestDy + nBestDx * nBestDx != nEndDy * nEndDy + nEndDx * nEndDx
            || g_arrFloodPathLen_523CAC[i] > nBestLen) )
        {
          nBestDx = nEndDx;
          nBestDy = nEndDy;
          iBestCand = i;
        }
      }
      else if ( nBestDy * nBestDy + nBestDx * nBestDx <= nEndDy * nEndDy + nEndDx * nEndDx
        && (nBestDy * nBestDy + nBestDx * nBestDx != nEndDy * nEndDy + nEndDx * nEndDx
          || g_arrFloodPathLen_523CAC[i] > nBestLen) )
      {
        nBestDx = nEndDx;
        nBestDy = nEndDy;
        iBestCand = i;
      }
    }
    if ( nMode )
    {
      if ( nBestDy * nBestDy + nBestDx * nBestDx <= (ptBestEnd.y - pnHeroCell.y) * (ptBestEnd.y - pnHeroCell.y)
        + (ptBestEnd.x - pnHeroCell.x) * (ptBestEnd.x - pnHeroCell.x)
        && (nBestDy * nBestDy + nBestDx * nBestDx != (ptBestEnd.y - pnHeroCell.y) * (ptBestEnd.y - pnHeroCell.y)
          + (ptBestEnd.x - pnHeroCell.x) * (ptBestEnd.x - pnHeroCell.x)
          || g_arrFloodPathLen_523CAC[iBestCand] > nBestLen) )
      {
        ptBestEnd.x = g_arrFloodPathXY_5244F8[iBestCand][g_arrFloodPathCnt_52E500[iBestCand]].x;
        ptBestEnd.y = g_arrFloodPathXY_5244F8[iBestCand][g_arrFloodPathCnt_52E500[iBestCand]].y;
        nBestDir = jDir;
        nBestLen = g_arrFloodPathLen_523CAC[iBestCand];
        for ( i = 0; i < 5; ++i )
          ppOutPathPts[i] = g_arrFloodPathXY_5244F8[iBestCand][i];
        *pnOutNumbPathPts = g_arrFloodPathCnt_52E500[iBestCand];
      }
    }
    else if ( nBestDy * nBestDy + nBestDx * nBestDx >= (ptBestEnd.y - pnHeroCell.y) * (ptBestEnd.y - pnHeroCell.y)
      + (ptBestEnd.x - pnHeroCell.x) * (ptBestEnd.x - pnHeroCell.x)
      && (nBestDy * nBestDy + nBestDx * nBestDx != (ptBestEnd.y - pnHeroCell.y) * (ptBestEnd.y - pnHeroCell.y)
        + (ptBestEnd.x - pnHeroCell.x) * (ptBestEnd.x - pnHeroCell.x)
        || g_arrFloodPathLen_523CAC[iBestCand] > nBestLen) )
    {
      ptBestEnd.x = g_arrFloodPathXY_5244F8[iBestCand][g_arrFloodPathCnt_52E500[iBestCand]].x;
      ptBestEnd.y = g_arrFloodPathXY_5244F8[iBestCand][g_arrFloodPathCnt_52E500[iBestCand]].y;
      nBestDir = jDir;
      nBestLen = g_arrFloodPathLen_523CAC[iBestCand];
      for ( i = 0; i < 5; ++i )
        ppOutPathPts[i] = g_arrFloodPathXY_5244F8[iBestCand][i];
      *pnOutNumbPathPts = g_arrFloodPathCnt_52E500[iBestCand];
    }
  }
  if ( ptBestEnd.x == pnCell.x && ptBestEnd.y == pnCell.y )
  {
    nBestDir = 8;
    nBestLen = 5;
  }
  pGrid[200 * pnHeroCell.y + 20100 + pnHeroCell.x] = bHeroCellSaved;
  fOutDir.x = arrDirFx[nBestDir];
  fOutDir.y = arrDirFz[nBestDir];
  *pnFoeCell = ptBestEnd;
  *pnDepth = nBestLen;
  *pfOutStep = fOutDir;
  return pfOutStep;
}

int __cdecl hook_DSBufferGetVolume_423C65(LPDIRECTSOUNDBUFFER pDSBuf)
{
  LONG nVolume; // [esp+0h] [ebp-4h] BYREF

  nVolume = 0;
  if ( !g_bDSInited_5185C4 || !pDSBuf )
    return -100000;
  pDSBuf->GetVolume(&nVolume);
  return nVolume;
}

// UpdateVoicePlayback(pSpeaker, pPosFrame, pVoices, fFrameTime)
// DirectSound voice/line scheduler (uses g_pds3DListener + g_dwDSStatus_5185C8). pVoices is a 20-slot
// array of positionable sound objects. sub_408C3E gets a 3D position from pPosFrame; each voice is
// placed there (vtable slot +76) and its play state probed (slot +36 -> g_dwDSStatus). Based on the
// speaker state v12 = *(pSpeaker+512) it selects which voice(s) to start (slot +48/+52), stop, or
// seek to a playback offset (slot +60; sub_423C65 returns the current play position). Case groups:
// state 1/2/4 = random one-shot line; 3/6/9 = timed line with stop-on-finish; 7/8 = alternating pair.
void __cdecl hook_UpdateVoicePlayback_458B3A(
  CSekBody *pFoeBody,
  CHanaFrameRootT3EC_S2D4 *pFoeFocusFrame,
  CSoundStruct_8 **arrVoiceBufs,
  float fDelta)
{
  int Volume_423C65; // esi
  int nVolTmp2; // esi
  _WORD arrSndIdx6[4]; // [esp+8h] [ebp-9Ch]
  BOOL bAlreadyPlaying; // [esp+10h] [ebp-94h]
  _WORD arrSndIdxRand[6]; // [esp+14h] [ebp-90h]
  D3DVALUE px; // [esp+20h] [ebp-84h] BYREF
  int nVoiceSel; // [esp+24h] [ebp-80h]
  int nSpeakerState; // [esp+28h] [ebp-7Ch]
  int i; // [esp+2Ch] [ebp-78h]
  D3DVALUE pz; // [esp+30h] [ebp-74h] BYREF
  int nFadeVol; // [esp+34h] [ebp-70h]
  int bAnyPlaying; // [esp+38h] [ebp-6Ch]
  _DWORD arrIsPlaying[25]; // [esp+3Ch] [ebp-68h]
  D3DVALUE py; // [esp+A0h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] UpdateVoicePlayback_458B3A\n");
 
  nSpeakerState = 0;
  nVoiceSel = 0;
  bAnyPlaying = 0;
  if ( g_pds3DListener && pFoeBody )
  {
    if ( fDelta < 0.0099999998 )
      fDelta = 0.0099999998;
    nSpeakerState = pFoeBody->field_200;
    HanaFrame_GetCenterPosition_408C3E(pFoeFocusFrame, g_pSceneFrame, &px, &py, &pz);
    for ( i = 0; i < 20; ++i )
    {
      if ( arrVoiceBufs[i] )
      {
        arrVoiceBufs[i]->pDS3DBuffer_4->SetPosition(px, py, pz, 1u);
        if ( arrVoiceBufs[i]->pDSBuffer_0
          && !arrVoiceBufs[i]->pDSBuffer_0->GetStatus(&g_dwDSStatus_5185C8)
          && (g_dwDSStatus_5185C8 & 1) != 0 )
        {
          arrIsPlaying[i] = 1;
          bAnyPlaying = 1;
          nFadeVol = DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0);
          if ( nFadeVol <= -7142 )
            arrVoiceBufs[i]->pDSBuffer_0->Stop();
        }
        else
        {
          arrIsPlaying[i] = 0;
        }
      }
    }
    switch ( nSpeakerState )
    {
    case 1:
    case 2:
    case 4:
      if ( !bAnyPlaying )
      {
        arrSndIdxRand[0] = 0;
        arrSndIdxRand[1] = 1;
        arrSndIdxRand[2] = 2;
        arrSndIdxRand[3] = 9;
        arrSndIdxRand[4] = 10;
        nVoiceSel = (__int16)arrSndIdxRand[rand() % 5];
        if ( g_nCurVoiceIdx_52EF64 == nVoiceSel )
          nVoiceSel = 0;
        else
          g_nCurVoiceIdx_52EF64 = nVoiceSel;
        if ( arrVoiceBufs[nVoiceSel] )
        {
          if ( arrVoiceBufs[nVoiceSel]->pDSBuffer_0 )
            arrVoiceBufs[nVoiceSel]->pDSBuffer_0->SetVolume(0);
          arrVoiceBufs[nVoiceSel]->pDSBuffer_0->SetCurrentPosition(0);
          arrVoiceBufs[nVoiceSel]->pDSBuffer_0->Play(0, 0, 0);
        }
      }
      break;
    case 6:
    case 3:
    case 9:
      bAlreadyPlaying = 0;
      if ( nSpeakerState == 3 )
      {
        nVoiceSel = 7;
        bAlreadyPlaying = g_nCurVoiceIdx_52EF64 == 7;
        g_nCurVoiceIdx_52EF64 = 7;
      }
      if ( nSpeakerState == 6 )
      {
        arrSndIdx6[0] = 5;
        arrSndIdx6[1] = 4;
        arrSndIdx6[2] = 12;
        arrSndIdx6[3] = 11;
        if ( !arrIsPlaying[4] && !arrIsPlaying[5] && !arrIsPlaying[11] && !arrIsPlaying[12] )
        {
          ++g_nVoiceRotate_52EF68;
          g_nVoiceRotate_52EF68 %= 4;
        }
        nVoiceSel = (__int16)arrSndIdx6[g_nVoiceRotate_52EF68];
        g_nCurVoiceIdx_52EF64 = nVoiceSel;
      }
      if ( nSpeakerState == 9 )
      {
        nVoiceSel = 8;
        if ( g_nCurVoiceIdx_52EF64 == 8 )
          bAlreadyPlaying = 1;
        g_nCurVoiceIdx_52EF64 = nVoiceSel;
      }
      for ( i = 0; i < 20; ++i )
      {
        if ( arrVoiceBufs[i] && g_nCurVoiceIdx_52EF64 != i && arrVoiceBufs[i]->pDSBuffer_0 )
        {
          if ( (int)(DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0) - (__int64)(500.0 * fDelta)) >= -10000 )
          {
            if ( (int)(DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0) - (__int64)(500.0 * fDelta)) <= 0 )
            {
              Volume_423C65 = DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0);
              arrVoiceBufs[i]->pDSBuffer_0->SetVolume(
                Volume_423C65 - (__int64)(500.0 * fDelta));
            }
            else
            {
              arrVoiceBufs[i]->pDSBuffer_0->SetVolume(0);
            }
          }
          else
          {
            arrVoiceBufs[i]->pDSBuffer_0->SetVolume(-10000);
          }
        }
      }
      if ( !bAlreadyPlaying && arrVoiceBufs[g_nCurVoiceIdx_52EF64] && !arrIsPlaying[g_nCurVoiceIdx_52EF64] )
      {
        if ( arrVoiceBufs[g_nCurVoiceIdx_52EF64]->pDSBuffer_0 )
          arrVoiceBufs[g_nCurVoiceIdx_52EF64]->pDSBuffer_0->SetVolume(
            0);
        arrVoiceBufs[g_nCurVoiceIdx_52EF64]->pDSBuffer_0->SetCurrentPosition(
          0);
        arrVoiceBufs[g_nCurVoiceIdx_52EF64]->pDSBuffer_0->Play(
          0,
          0,
          0);
      }
      break;
    case 7:
    case 8:
      nVoiceSel = 3;
      if ( g_nCurVoiceIdx_52EF64 == 3 )
        nVoiceSel = 6;
      else
        g_nCurVoiceIdx_52EF64 = nVoiceSel;
      if ( bAnyPlaying )
      {
        for ( i = 0; i < 20; ++i )
        {
          if ( arrVoiceBufs[i] && i != 6 && i != 3 && arrVoiceBufs[i]->pDSBuffer_0 )
          {
            if ( (int)(DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0) - (__int64)(300.0 * fDelta)) >= -10000 )
            {
              if ( (int)(DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0) - (__int64)(300.0 * fDelta)) <= 0 )
              {
                nVolTmp2 = DSBufferGetVolume_423C65(arrVoiceBufs[i]->pDSBuffer_0);
                arrVoiceBufs[i]->pDSBuffer_0->SetVolume(
                  nVolTmp2 - (__int64)(300.0 * fDelta));
              }
              else
              {
                arrVoiceBufs[i]->pDSBuffer_0->SetVolume(0);
              }
            }
            else
            {
              arrVoiceBufs[i]->pDSBuffer_0->SetVolume(-10000);
            }
          }
        }
      }
      if ( arrVoiceBufs[nVoiceSel] && !arrIsPlaying[6] && !arrIsPlaying[3] )
      {
        if ( arrVoiceBufs[nVoiceSel]->pDSBuffer_0 )
          arrVoiceBufs[nVoiceSel]->pDSBuffer_0->SetVolume(0);
        arrVoiceBufs[nVoiceSel]->pDSBuffer_0->SetCurrentPosition(0);
        arrVoiceBufs[nVoiceSel]->pDSBuffer_0->Play(0, 0, 0);
      }
      break;
    default:
      if ( nSpeakerState != 5 )
        g_nCurVoiceIdx_52EF64 = 0;
      break;
    }
  }
}

/*

+L82    func-ptr  UpdateHeroBody_44474E         0x44474E    [no hook fn]
+L88    func-ptr  UpdateFoeAI_446A38            0x446A38    [no hook fn]
+L94    func-ptr  UpdateVoicePlayback_458B3A    0x458B3A    [no hook fn]
L100   func-ptr  WanaAttack_Close_4535DE       0x4535DE    [no hook fn]
L104   func-ptr  WanaAttack_Knockback_4536F6   0x4536F6    [no hook fn]
L108   func-ptr  WanaAttack_Charged_4539D1     0x4539D1    [no hook fn]
L112   func-ptr  WanaAttack_Projectile_453B92  0x453B92    [no hook fn]
L116   func-ptr  IsFoeWithinHeroRange_448B78   0x448B78    [no hook fn]
L120   func-ptr  PlayCinematicSequence_45460A  0x45460A    [no hook fn]
L131   func-ptr  RenderDualBodyScene_456807    0x456807    [no hook fn]
L137   func-ptr  PlayResultsCutscene_455BE2    0x455BE2    [no hook fn]
L145   func-ptr  PlayVictoryCutscene_455183    0x455183    [no hook fn]
L154   func-ptr  DrawRecViewfinder_44B23B      0x44B23B    [no hook fn]
L159   func-ptr  DrawFadeOutSpark_45669F       0x45669F    [no hook fn]
+L163   func-ptr  BuildMapCollisionGrid_445B12  0x445B12    [no hook fn]
L167   func-ptr  BindSceneActors_448C72        0x448C72    [no hook fn]
L172   func-ptr  UpdateCameraFollow_44C028     0x44C028    [no hook fn]

*/


void register_SetupHeroEntry_4593E0() {
  hnh_Hook(0x4593e0, hook_SetupHeroEntry_4593E0, "SetupHeroEntry_4593E0"); // [OK]
  hnh_Hook(0x44474e, hook_UpdateHeroBody_44474E, "UpdateHeroBody_44474E");
  hnh_Hook(0x4070ab, hook_Frame_RotateMatrixAxis_4070AB, "Frame_RotateMatrixAxis_4070AB");
  hnh_Hook(0x443cc5, hook_CheckFrameCollision_443CC5, "CheckFrameCollision_443CC5");
  hnh_Hook(0x442c1c, hook_HitOverheadSurface_442C1C, "HitOverheadSurface_442C1C");
  hnh_Hook(0x4423b0, hook_HitSupportSurface_4423B0, "HitSupportSurface_4423B0");
  hnh_Hook(0x407e1f, hook_ComputeMeshBounds_407E1F, "ComputeMeshBounds_407E1F");
  hnh_Hook(0x443ec4, hook_TestBodyMoveHit_443EC4, "TestBodyMoveHit_443EC4");
  hnh_Hook(0x4443fc, hook_TestMoveCollision_4443FC, "TestMoveCollision_4443FC");
  hnh_Hook(0x445b12, hook_BuildMapCollisionGrid_445B12, "BuildMapCollisionGrid_445B12");
  hnh_Hook(0x445c12, hook_HanaGrid_DFSPath, "HanaGrid_DFSPath_445C12");
  hnh_Hook(0x4460ad, hook_HanaGrid_FloodFill_4460AD, "HanaGrid_FloodFill_4460AD");
  hnh_Hook(0x458b3a, hook_UpdateVoicePlayback_458B3A, "UpdateVoicePlayback_458B3A");
  hnh_Hook(0x423c65, hook_DSBufferGetVolume_423C65, "DSBufferGetVolume_423C65");


}
