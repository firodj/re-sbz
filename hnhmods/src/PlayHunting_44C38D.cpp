// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayHunting_44C38D)(int nDuelIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x44C38D);
double (__cdecl*UpdateFoeAI_446A38)(CSekBody *pFoeBody, CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame, char *pOccGrid, float fDeltaTime,
  float *pfAggro, float *pfHeroHp, float fFoeHpMax, int nDuelIndex) =
  reinterpret_cast<double(__cdecl*)(CSekBody *, CSekBody *,
    CHanaFrameT3EC_S354 *, char *, float, float *, float *, float, int)>(0x446A38);

typedef double(__cdecl *WanaAttackFn)(CSekBody *pWanaBody, CSekBody *pHeroBody,
  float *pfHeroHp, float fDeltaTime, float fAttackParam,
  CHanaFrameT3EC_S354 *pMapFrame);

double (__cdecl*WanaAttack_Close_4535DE)(CSekBody *pWanaBody, CSekBody *pHeroBody,
  float *pfHeroHp, float fDeltaTime, float fAttackParam,
  CHanaFrameT3EC_S354 *pMapFrame) = reinterpret_cast<WanaAttackFn>(0x4535DE);

double (__cdecl*WanaAttack_Knockback_4536F6)(CSekBody *pWanaBody,
  CSekBody *pHeroBody, float *pfHeroHp, float fDeltaTime, float fAttackParam,
  CHanaFrameT3EC_S354 *pMapFrame) = reinterpret_cast<WanaAttackFn>(0x4536F6);

double (__cdecl*WanaAttack_Charged_4539D1)(CSekBody *pWanaBody,
  CSekBody *pHeroBody, float *pfHeroHp, float fDeltaTime, float fAttackParam,
  CHanaFrameT3EC_S354 *pMapFrame) = reinterpret_cast<WanaAttackFn>(0x4539D1);

double (__cdecl*WanaAttack_Projectile_453B92)(CSekBody *pWanaBody,
  CSekBody *pHeroBody, float *pfHeroHp, float fDeltaTime, float fAttackParam,
  CHanaFrameT3EC_S354 *pMapFrame) = reinterpret_cast<WanaAttackFn>(0x453B92);

bool (__cdecl*IsFoeWithinHeroRange_448B78)(CSekBody *pHeroBody,
  CSekBody *pFoeBody) =
  reinterpret_cast<bool(__cdecl*)(CSekBody *, CSekBody *)>(0x448B78);

void (__cdecl*RenderDualBodyScene_456807)(CHanaSceneT458_S504 *pScene,
  CSekBody *pBodyA, CSekBody *pBodyB, CHanaSpriteT3F6_S1FC **pSprites,
  CHanaSpriteT3F6_S1FC **pSounds) =
  reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
    CSekBody *, CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **)>(0x456807);

void (__cdecl*PlayResultsCutscene_455BE2)(CSekBody *pHeroBody, CSekBody *pFoeBody,
  CSekBody **pWanaBodies, CHanaFrameT3EC_S354 *pMapFrame, CHanaSpriteT3F6_S1FC **pArrHudMeters,
  CHanaSpriteT3F6_S1FC **pArrSprites, float fHeroHpCur, float fHeroHpMax, float fFoeHpCur,
  float fFoeHpMax, int nClockBase, unsigned int dwElapsedMs) =
  reinterpret_cast<void(__cdecl*)(CSekBody *, CSekBody *, CSekBody **,
    CHanaFrameT3EC_S354 *, CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **, float, float, float, float, int,
    unsigned int)>(0x455BE2);

void (__cdecl*PlayVictoryCutscene_455183)(CHanaSceneT458_S504 *pOdsScene,
  CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame,
  CHanaSpriteT3F6_S1FC **pArrHudMeters, CHanaSpriteT3F6_S1FC **pArrSprites, float fHeroHpCur, float fHeroHpMax,
  float fFoeHpCur, float fFoeHpMax, int nClockBase, unsigned int dwElapsedMs,
  CSoundStruct_8 **pArrVoiceBufs) =
  reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
    CSekBody *, CHanaFrameT3EC_S354 *, CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **, float, float, float,
    float, int, unsigned int, CSoundStruct_8 **)>(0x455183);

void(__cdecl*DrawDuelHealthHud_44B23B)(CHanaSpriteT3F6_S1FC **pSprites,
  int nHeroHpCur, int nHeroHpMax, int nFoeHp, int nFoeHpMax) =
  reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, int, int,
    int)>(0x44B23B);

void (__cdecl*DrawFadeOutSpark_45669F)(CHanaSpriteT3F6_S1FC **arrSparkSprites,
  float fDeltaTime) =
  reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, float)>(0x45669F);

void (__cdecl*BindSceneActors_448C72)(CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 **pActorFrames, unsigned int nCount) =
  reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *,
    CHanaFrameT3EC_S354 **, unsigned int)>(0x448C72);

void (__cdecl*UpdateCameraFollowDuel_44C028)(CSekBody *pHeroBody, CSekBody *pFoeBody,
  float fDeltaTime) =
  reinterpret_cast<void(__cdecl*)(CSekBody *, CSekBody *, float)>(0x44C028);

void (__cdecl *StepFoeTurn_457EE0)(CSekBody *pFoeBody, CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame, float fDelta, float fSpinRate, float fSpeed)
= reinterpret_cast<void (__cdecl *)(CSekBody *, CSekBody *, CHanaFrameT3EC_S354 *, float, float, float)>(0x457EE0);
void (__cdecl *StepFoeSpin_45804C)(CSekBody *pFoeBody, CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame, float fDelta, float fSpinRate, float fSpeed)
= reinterpret_cast<void (__cdecl *)(CSekBody *, CSekBody *, CHanaFrameT3EC_S354 *, float, float, float)>(0x45804C);


BOOL (__cdecl *StepWanaProjectile_4581B0)(CSekBody *pWanaBody, CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame, float fDelta, float fSpinRate, float fProjSpeed, int bAlreadyLaunched, int bProjMode)
= reinterpret_cast<BOOL (__cdecl*)(CSekBody *pWanaBody, CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame, float fDelta, float fSpinRate, float fProjSpeed, int bAlreadyLaunched, int bProjMode)>(0x4581b0);

void (__cdecl *DrawSprite3D)(CHanaSpriteT3F6_S1FC *pSprite, D3DXVECTOR3 *parrPos)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC *pSprite, D3DXVECTOR3 *parrPos)>(0x414344);

// === PINNED GLOBALS ===

// UI/menu SFX slots 0..9 (0 Start, 1 GameOver, 2 On, 3 per-duel act\HitN)
LPDIRECTSOUNDBUFFER (&g_pArrSndBuf_5244AC)[10] =
*(LPDIRECTSOUNDBUFFER(*)[10])0x5244AC;


// foe 3D hit SFX (each foot wav loaded twice, one per side)
CSoundStruct_8 *(&g_pFoeHitSfxB_5244D4)[4] = *(CSoundStruct_8 *(*)[4])0x5244D4; // WRITABLE (also pinned in PlayHunting)
CSoundStruct_8 *(&g_pFoeHitSfxE_5244E4)[5] = *(CSoundStruct_8 *(*)[5])0x5244E4; // WRITABLE (also pinned in PlayHunting)

int &g_dwFoeAiState_52ED20 = *(int *)0x52ED20; // WRITABLE
int &g_bFoeCharging_52ED24 = *(int *)0x52ED24; // WRITABLE (dword store despite the b- name: C7 05 at 0x44767E)
float &g_fFoeAiTimer_52ED28 = *(float *)0x52ED28; // WRITABLE
float &g_fFoeCdTimer_52ED18 = *(float *)0x52ED18; // WRITABLE
int &g_dwFoeCellXCache_52E4F8 = *(int *)0x52E4F8; // WRITABLE (right after g_arrFloodPathXY_5244F8, which spans 0x5244F8..0x52E4F8)
int &g_dwFoeCellYCache_52E4FC = *(int *)0x52E4FC; // WRITABLE

uint32_t &g_bFoeSpun_52EF60 = *(uint32_t *)0x52EF60; // WRITABLE
float &g_fFoeSpinDir_52EF5C = *(float *)0x52EF5C; // WRITABLE

// === FUNCTIONS ===


// PlayHunting_44C38D(nDuelIndex) -- GameMainLoop scene-type 3: the VS-battle/
// duel screen (called with iModeId[2*stc9C.sceneSteps]; single arg). A duel
// against one main foe plus up to 8 'wana' mobs, on a timed 3-minute clock.
//
// nDuelIndex 0..11: 12 duel entries; arrChapterActMap[12] =
// {0,0,1,1,2,2,3,3,3,4,4,4} maps them to the 5 acts (BGM/voice/SE/face-BMP
// tables). Each 4-slot record in the duel-entry table:
//   [k+0] foe body base 'run\\oN\\oNx' (o2a,o2b,o5a,o5b,o4a,o4b,o3a,o3b,o3c,
//         o1a,o1b,o1c), [k+1] map ODF 'Map\\MNN\\atNN.ODF', [k+2] map object
//         ODF 'atNN_o.ODF', [k+3] map ODS scene 'run\\MapNN.ods'.
//
// One-time setup: Start/GameOver/On UI wavs into g_pArrSndBuf[0..2] (loop
// stops at the first empty name), the per-duel foe-attack SE act\\Hit{,2,3,4}
// over g_pArrSndBuf[3], foot4/2/3/5 loaded twice each as 3D foe-hit SFX
// g_pFoeHitSfxE/B[4], 20 voice wavs of the act's block act\\A0X_NN.wav
// (arrVoiceBufs[20], empty names still go through LoadWavToDS3DBuffer);
// BGM s010/s006/s011.wav (duels {0,2,4,6,9}->s010, {7,10}->s011, else s006)
// volume-clamped from g_fVolumeDbTable[avi volume]-1000; InitHudResources per
// act; foe face BMP pair run\\oNa.bmp + run\\oNb.bmp (arrFoeBmpNames, LoadBmp-
// WithMask); hero body otoko\\otoko.sek/.odf; foe body 'run\\oN\\oNx.sek/.odf';
// wana bodies wana\\<sek>.sek + wana\\<odf>.ODF, 8 per duel (arrWanaSekNames /
// arrWanaOdfNames, per-duel rows 8*nDuelIndex, loading stops at an empty sek
// name); map + map-object ODFs (ParseODFFile, anim set freed, attached to
// g_pSceneFrame, BuildMapSubFrames each) + BuildMapCollisionGrid from the map
// object; ODS scene 'run\\MapNN.ods' (WCurNum = 100000); hero+foe+wana frames
// bound to the scene (BindSceneActors, nCount = 2..10); D3DRS_AMBIENT =
// 0xFFFFFF; plays 'Start', renders one test frame (RenderDualBodyScene),
// plays 'On'.
//
// Round loop (LABEL_68): park hero+foe at their captured pos/rot, camera
// follow, per-duel foe-aggro max arrFoeHpMax[12] (20000..45000), CutToAnim
// (1)/PlayAnim on hero+foe, CutToAnim(0) on the wanas; the foe face frame is
// found by name "_04_kao" (@0x4B5220). Inner frame loop: DoPeekMessages +
// GetFrameTime (fDelta capped 5.0, divided by 4 while a cinematic is queued);
// menu key snapshots the live scene state, restores the entry state, stops
// the voices and opens SystemMenu on the BGM (>= 1 exits, else live state is
// put back); cinematic branch plays PlayTugOfWar (cut-in; wanas are
// alpha-deferred for it, dead ones stay hidden) and grants a 120-unit action
// timer; otherwise: hero action-intro timer (x3/2 speed while it lasts) or
// UpdateHeroBody (bActing), toggle-C camera (UpdateCameraFollow_44C028 while
// hero field_200 <= 6, else the free 445525 follow), UpdateFoeAI (returns an
// action-timer increment; a non-zero return triggers the foe-attack SE),
// UpdateVoicePlayback on the face frame, one of four wana attack functions
// per wana (Close/Knockback/Charged/Projectile, type arrWanaAtkTypes, param
// arrWanaAtkParams = 5000..9000), and IsFoeWithinHeroRange may queue the
// cinematic for the next frame. ODS scene clock loops at fRes_13C. Draw:
// Clear(3) + 3D pass (alpha blend off for HanaFrame_Draw, on for
// HanaVert_DrawBatch), DrawDuelHealthHud (hero HP/max, foe aggro/max bars),
// DrawClockHud (100*nClockBase - elapsed/10), DrawFadeOutSpark; the first
// frame fades in instead of presenting (dwClockBase2 skips the menu time).
//
// The round ends when the clock hits 0, the foe aggro <= 0, the hero HP <= 0,
// or hero field_200 in {10,11,13,16}. Defeat/timeout: GameOver SE at -500 dB,
// PlayResultsCutscene, ShowMouseSelectDialog (choose "keep going" -> reveal
// the next live wana (field_24C = 1, alpha-deferred) and restart the round at
// LABEL_154/LABEL_68; quit -> nSysMenuResult = 1); the persisted current HP
// is raised by 15% of max (clamped). Victory (aggro <= 0): persist HP/max,
// PlayVictoryCutscene, fade. Finally: stop/release the SFX, 3D-hit, BGM and
// voice buffers; release hero/foe/wana bodies (wanas are re-attached to
// g_pSceneFrame first, as in the original), the ODS scene, both map frames,
// the HUD/viewfinder/spark sprites and the face BMP. Returns the SystemMenu
// result (0 = won, 1 = quit), or -1 on a load failure.
//
// Label map vs the original decompilation: LABEL_68 -> round start (top of
// the outer while), LABEL_154 -> next-wana reveal scan.


int __cdecl hook_PlayHunting_44C38D(int nDuelIndex)
{
  CSceneState sceneStateLive;         // live copy saved around SystemMenu
  CSceneState sceneMenuCopy;          // SystemMenu by-value arg / scratch
  CSceneState sceneStateEntry;        // entry snapshot of g_sceneState
  CHanaFrameT3EC_S354 *pActorFrames[10]; // hero + foe + wanas
  CHanaAnimSetT3F1_S150 *pMapOdfAnimSet;  // map ODF throw-away anim set
  CHanaAnimSetT3F1_S150 *pMapObjAnimSet;  // map-object ODF ditto
  //String 50 szArrWanaSeks[200];        // wana\\<sek>.sek base names
  //String 50 azArrWanaOdfs[200];        // wana\\<odf>.ODF base names
  //String 50 arrFoeAtkWavs[15];         // per-duel g_pArrSndBuf[3] override
  //String 50 arrBgmWavs[15];            // s010/s006/s011.wav
  //String 50 arrUiWavNames[10];         // Start/GameOver/On + "" filler
  //String 50 arrFootWavNames[5];        // foot4/2/3/5
  //String 50 arrVoiceWavNames[140];     // 5 acts x 20 slots act\A0X_NN.wav
  //String 50 arrDuelEntries[20][4];     // 12 duel records (foe/map/ods)
  //char arrFoeBmpNames[6][2][25];      // foe face bmp/mask pairs per act
  //DWORD arrChapterActMap[20];         // duel -> act
  //DWORD arrWanaAtkTypes[160];         // per (duel,wana) attack fn index
  //float arrWanaAtkParams[160];        // per (duel,wana) attack param
  //float arrFoeHpMax[20];           // per-duel foe aggro max
  //WanaAttackFn arrWanaAtkFns[4];      // Close/Knockback/Charged/Projectile
  CSoundStruct_8 *arrVoiceBufs[20];
  CSekBody *pWanaBodies[10];
  CHanaSpriteT3F6_S1FC *pArrHudMeters[15];  // clock digits (InitHudResources)
  CHanaSpriteT3F6_S1FC *pArrSprites[20];    // viewfinder gauges
  CHanaSpriteT3F6_S1FC *arrSparkSprites[15];// fade-out sparks
  CHanaSpriteT3F6_S1FC *pMaskBmp;           // foe face bmp+mask sprite
  CHanaFrameT3EC_S354 *pFoeTalkFrame;       // foe "_04_kao" face frame
  CHanaFrameT3EC_S354 *pMapFrame;
  CHanaFrameT3EC_S354 *pMapObjFrame;
  CHanaSceneT458_S504 *pOdsScene;
  CSekBody *pHeroBody;
  CSekBody *pFoeBody;
  char *pOccGrid;                       // map-object collision grid
  LPDIRECTSOUNDBUFFER pBgmBuf;
  CHAR pszFileName[256];
  CHAR String[256];
  float fHeroX, fHeroY, fHeroZ;        // hero pos saved between rounds
  float dirForwardX, dirForwardY, dirForwardZ;
  float dirUpX, dirUpY, dirUpZ;        // hero rot saved between rounds
  float x, y, z;                       // foe pos saved between rounds
  float pXpos, pYpos, pZpos;           // foe rot saved between rounds
  float pXYrot, pYXrot, pZXrot;
  float fHeroHpCur; // was: p fHeroHP
  float fHeroHpMax; // was: fFieldC4
  float fFoeHpCur;  // was: p FAggro
  float fFoeHpMax;
  float fActTimer;
  float pfExecTime;
  float fDelta = 0.0;
  double updated;                      // UpdateFoeAI / wana-attack return
  double fActTimerNew;
  unsigned int nCount;
  DWORD dwElapsedMs;
  DWORD Time;
  int dwClockBase2;
  int nClockBase;
  int nSysMenuResult;
  int nSelectResult;
  int bFirstFrameDone;
  bool bActing;
  bool bCinematic;
  int i;

  ONCELOG("[HNH:SUB] PlayHunting_44C38D\n");

  // --- wana attack dispatch: 0 Close, 1 Knockback, 2 Charged, 3 Projectile
  WanaAttackFn arrWanaAtkFns[4] = {
    WanaAttack_Close_4535DE,
    WanaAttack_Knockback_4536F6,
    WanaAttack_Charged_4539D1,
    WanaAttack_Projectile_453B92,
  };

  // --- wana attack type per (duel, wana): rows of 8, duels 0..11
  // (memset'd rows are all-Close; rows past duel 11 are dead data)
#if 1
  DWORD arrWanaAtkTypes[20][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 1 }, // row 0 (indices 0-7, index 8 maps to [1][0])
    { 1, 0, 0, 0, 0, 0, 0, 0 }, // row 1 (index 17 is at [2][1])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, // row 2 (indices 16, 17: arrWanaAtkTypes[17] = 1 -> [2][1])
    { 0, 1, 0, 0, 0, 0, 0, 0 }, // row 3 (indices 24 = 1, 25 = 2 -> [3][0], [3][1])
    { 2, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 0, 0, 0, 0, 0, 0, 3 }, // row 4 (index 32 = 3 -> [4][0])
    { 0, 0, 2, 3, 1, 0, 0, 0 }, // row 5 (indices 40=2, 42=3, 43=1 -> [5][0], [5][2], [5][3])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 2, 0, 0, 0, 0, 0, 0 }, // row 6 (indices 48=2, 49=2 -> [6][0], [6][1])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 2, 2, 0, 0, 0, 0, 0 }, // row 7 (indices 56=2, 57=2, 58=2 -> [7][0], [7][1], [7][2])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 3, 3, 0, 0, 0, 0, 0, 0 }, // row 8 (indices 64=3, 65=3 -> [8][0], [8][1])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 3, 3, 2, 1, 1, 0, 0, 0 }, // row 9 (indices 72=3, 73=3, 74=2, 75=1, 76=1 -> [9][0..4])
    { 0, 0, 0, 1, 0, 0, 0, 0 }, // row 10 (index 83 = 1 -> [10][3])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 2, 2, 2, 3, 3, 3, 3, 3 }, // row 11 (indices 88-90=2, 91-95=3 -> [11][0..7])
    { 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 0, 0, 0, 0, 0, 0, 0 }  // row 19
  };
#else
  memset(arrWanaAtkTypes, 0, sizeof(arrWanaAtkTypes));
  arrWanaAtkTypes[8] = 1;                                        // duel 1
  arrWanaAtkTypes[17] = 1;                                       // duel 2
  arrWanaAtkTypes[24] = 1;  arrWanaAtkTypes[25] = 2;             // duel 3
  arrWanaAtkTypes[32] = 3;                                       // duel 4
  arrWanaAtkTypes[40] = 2;  arrWanaAtkTypes[42] = 3;
  arrWanaAtkTypes[43] = 1;                                       // duel 5
  arrWanaAtkTypes[48] = 2;  arrWanaAtkTypes[49] = 2;             // duel 6
  arrWanaAtkTypes[56] = 2;  arrWanaAtkTypes[57] = 2;
  arrWanaAtkTypes[58] = 2;                                       // duel 7
  arrWanaAtkTypes[64] = 3;  arrWanaAtkTypes[65] = 3;             // duel 8
  arrWanaAtkTypes[72] = 3;  arrWanaAtkTypes[73] = 3;
  arrWanaAtkTypes[74] = 2;  arrWanaAtkTypes[75] = 1;
  arrWanaAtkTypes[76] = 1;                                       // duel 9
  arrWanaAtkTypes[83] = 1;                                       // duel 10
  arrWanaAtkTypes[88] = 2;  arrWanaAtkTypes[89] = 2;
  arrWanaAtkTypes[90] = 2;                                       // duel 11
  arrWanaAtkTypes[91] = 3;  arrWanaAtkTypes[92] = 3;
  arrWanaAtkTypes[93] = 3;  arrWanaAtkTypes[94] = 3;
  arrWanaAtkTypes[95] = 3;
#endif

  // --- wana attack param per (duel, wana)
#if 1
  float arrWanaAtkParams[20][8] = {
    { 5000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 0 (index 0)
    { 5000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 1 (index 8)
    { 5000.0f, 5000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 2 (indices 16, 17)
    { 5000.0f, 5500.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 3 (indices 24, 25)
    { 7000.0f, 7000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 4 (indices 32, 33)
    { 6000.0f, 5000.0f, 5000.0f, 5000.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 5 (indices 40-43)
    { 6000.0f, 6000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 6 (indices 48, 49)
    { 6000.0f, 7000.0f, 7000.0f, 7000.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 7 (indices 56-59)
    { 7000.0f, 7000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // duel 8 (indices 64, 65)
    { 9000.0f, 9000.0f, 7000.0f, 6000.0f, 5500.0f, 0.0f, 0.0f, 0.0f }, // duel 9 (indices 72-76)
    { 8000.0f, 8000.0f, 8000.0f, 8000.0f, 8000.0f, 8000.0f, 8000.0f, 0.0f }, // duel 10 (indices 80-86)
    { 8000.0f, 8000.0f, 5000.0f, 8500.0f, 5000.0f, 5000.0f, 5000.0f, 5000.0f }, // duel 11 (indices 88-95)
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
  };
#else
  memset(arrWanaAtkParams, 0, sizeof(arrWanaAtkParams));
  arrWanaAtkParams[0] = 5000.0f;                                 // duel 0
  arrWanaAtkParams[8] = 5000.0f;                                 // duel 1
  arrWanaAtkParams[16] = 5000.0f;  arrWanaAtkParams[17] = 5000.0f; // duel 2
  arrWanaAtkParams[24] = 5000.0f;  arrWanaAtkParams[25] = 5500.0f; // duel 3
  arrWanaAtkParams[32] = 7000.0f;  arrWanaAtkParams[33] = 7000.0f; // duel 4
  arrWanaAtkParams[40] = 6000.0f;  arrWanaAtkParams[41] = 5000.0f;
  arrWanaAtkParams[42] = 5000.0f;  arrWanaAtkParams[43] = 5000.0f; // duel 5
  arrWanaAtkParams[48] = 6000.0f;  arrWanaAtkParams[49] = 6000.0f; // duel 6
  arrWanaAtkParams[56] = 6000.0f;  arrWanaAtkParams[57] = 7000.0f;
  arrWanaAtkParams[58] = 7000.0f;  arrWanaAtkParams[59] = 7000.0f; // duel 7
  arrWanaAtkParams[64] = 7000.0f;  arrWanaAtkParams[65] = 7000.0f; // duel 8
  arrWanaAtkParams[72] = 9000.0f;  arrWanaAtkParams[73] = 9000.0f;
  arrWanaAtkParams[74] = 7000.0f;  arrWanaAtkParams[75] = 6000.0f;
  arrWanaAtkParams[76] = 5500.0f;                                 // duel 9
  arrWanaAtkParams[80] = 8000.0f;  arrWanaAtkParams[81] = 8000.0f;
  arrWanaAtkParams[82] = 8000.0f;  arrWanaAtkParams[83] = 8000.0f;
  arrWanaAtkParams[84] = 8000.0f;  arrWanaAtkParams[85] = 8000.0f;
  arrWanaAtkParams[86] = 8000.0f;                                 // duel 10
  arrWanaAtkParams[88] = 8000.0f;  arrWanaAtkParams[89] = 8000.0f;
  arrWanaAtkParams[90] = 5000.0f;  arrWanaAtkParams[91] = 8500.0f;
  arrWanaAtkParams[92] = 5000.0f;  arrWanaAtkParams[93] = 5000.0f;
  arrWanaAtkParams[94] = 5000.0f;  arrWanaAtkParams[95] = 5000.0f; // duel 11
#endif

  // --- wana sek/odf base names, 8 per duel (rows 8*nDuelIndex..+7).
  // Untouched rows are the runtime-empty "" rows (0xFF .data filler) and
  // rows past 111 are dead data, all zero here.
#if 1
  char szArrWanaSeks[25][8][50] = {
    { "i33", "", "", "", "", "", "", ""}, // 0 - 7
    { "i44", "", "", "", "", "", "", ""}, // 8 - 15
    { "i34", "i42", "", "", "", "", "", ""}, // 16 - 23
    { "i42", "i51_57", "", "", "", "", "", ""}, // 24 - 31
    { "i71", "i34", "", "", "", "", "", ""}, // 32 - 39
    { "i51_57", "i33", "i72", "i42", "", "", "", ""}, // 40 - 47
    { "i51_57", "i51_57", "", "", "", "", "", ""}, // 48 - 55
    { "i51_57", "i51_57", "i51_57", "i32", "", "", "", ""}, // 56 - 63
    { "i71", "i71", "", "", "", "", "", ""}, // 64 - 71
    { "i70", "i70", "i51_57", "i32", "i32", "", "", ""}, // 72 - 79
    { "i31", "i31", "i31", "i44", "i31", "i31", "i31", ""}, // 80 - 87
    { "i51_57", "i51_57", "i51_57", "i72", "i72", "i72", "i72", "i72"}, // 88 - 95
    { "", "", "", "", "", "", "", ""}, // 96 - 103
    { "i51_57", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57"}, // 104 - 111
    // Elements 112 through 199 will automatically be initialized to ""
  };
  char azArrWanaOdfs[25][8][50] = {
    { "i33", "", "", "", "", "", "", "" },
    { "i44", "", "", "", "", "", "", "" },
    { "i34", "i42", "", "", "", "", "", "" },
    { "i42", "i52", "", "", "", "", "", "" },
    { "i71", "i34", "", "", "", "", "", "" },
    { "i52", "i33", "i72", "i42", "", "", "", "" },
    { "i51", "i53", "", "", "", "", "", "" },
    { "i53", "i55", "i54", "i32", "", "", "", "" },
    { "i71", "i71", "", "", "", "", "", "" },
    { "i70", "i70", "i56", "i43", "i43", "", "", "" },
    { "i31", "i31", "i31", "i44", "i31", "i31", "i31", "" },
    { "i53", "i53", "i53", "i72", "i72", "i72", "i72", "i72" },
    { "", "", "", "", "", "", "", "" },
    { "i51", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57", "i51_57" },
  };
#else
  memset(szArrWanaSeks, 0, sizeof(szArrWanaSeks));

  strcpy(szArrWanaSeks[0].str, "i33");      strcpy(szArrWanaSeks[8].str, "i44");
  strcpy(szArrWanaSeks[16].str, "i34");     strcpy(szArrWanaSeks[17].str, "i42");
  strcpy(szArrWanaSeks[24].str, "i42");     strcpy(szArrWanaSeks[25].str, "i51_57");
  strcpy(szArrWanaSeks[32].str, "i71");     strcpy(szArrWanaSeks[33].str, "i34");
  strcpy(szArrWanaSeks[40].str, "i51_57");  strcpy(szArrWanaSeks[41].str, "i33");
  strcpy(szArrWanaSeks[42].str, "i72");     strcpy(szArrWanaSeks[43].str, "i42");
  strcpy(szArrWanaSeks[48].str, "i51_57");  strcpy(szArrWanaSeks[49].str, "i51_57");
  strcpy(szArrWanaSeks[56].str, "i51_57");  strcpy(szArrWanaSeks[57].str, "i51_57");
  strcpy(szArrWanaSeks[58].str, "i51_57");  strcpy(szArrWanaSeks[59].str, "i32");
  strcpy(szArrWanaSeks[64].str, "i71");     strcpy(szArrWanaSeks[65].str, "i71");
  strcpy(szArrWanaSeks[72].str, "i70");     strcpy(szArrWanaSeks[73].str, "i70");
  strcpy(szArrWanaSeks[74].str, "i51_57");  strcpy(szArrWanaSeks[75].str, "i32");
  strcpy(szArrWanaSeks[76].str, "i32");
  strcpy(szArrWanaSeks[80].str, "i31");     strcpy(szArrWanaSeks[81].str, "i31");
  strcpy(szArrWanaSeks[82].str, "i31");     strcpy(szArrWanaSeks[83].str, "i44");
  strcpy(szArrWanaSeks[84].str, "i31");     strcpy(szArrWanaSeks[85].str, "i31");
  strcpy(szArrWanaSeks[86].str, "i31");
  strcpy(szArrWanaSeks[88].str, "i51_57");  strcpy(szArrWanaSeks[89].str, "i51_57");
  strcpy(szArrWanaSeks[90].str, "i51_57");  strcpy(szArrWanaSeks[91].str, "i72");
  strcpy(szArrWanaSeks[92].str, "i72");     strcpy(szArrWanaSeks[93].str, "i72");
  strcpy(szArrWanaSeks[94].str, "i72");     strcpy(szArrWanaSeks[95].str, "i72");
  strcpy(szArrWanaSeks[104].str, "i51_57"); strcpy(szArrWanaSeks[105].str, "i51_57");
  strcpy(szArrWanaSeks[106].str, "i51_57"); strcpy(szArrWanaSeks[107].str, "i51_57");
  strcpy(szArrWanaSeks[108].str, "i51_57"); strcpy(szArrWanaSeks[109].str, "i51_57");
  strcpy(szArrWanaSeks[110].str, "i51_57"); strcpy(szArrWanaSeks[111].str, "i51_57");

  memset(azArrWanaOdfs, 0, sizeof(azArrWanaOdfs));
  strcpy(azArrWanaOdfs[0].str, "i33");      strcpy(azArrWanaOdfs[8].str, "i44");
  strcpy(azArrWanaOdfs[16].str, "i34");     strcpy(azArrWanaOdfs[17].str, "i42");
  strcpy(azArrWanaOdfs[24].str, "i42");     strcpy(azArrWanaOdfs[25].str, "i52");
  strcpy(azArrWanaOdfs[32].str, "i71");     strcpy(azArrWanaOdfs[33].str, "i34");
  strcpy(azArrWanaOdfs[40].str, "i52");     strcpy(azArrWanaOdfs[41].str, "i33");
  strcpy(azArrWanaOdfs[42].str, "i72");     strcpy(azArrWanaOdfs[43].str, "i42");
  strcpy(azArrWanaOdfs[48].str, "i51");     strcpy(azArrWanaOdfs[49].str, "i53");
  strcpy(azArrWanaOdfs[56].str, "i53");     strcpy(azArrWanaOdfs[57].str, "i55");
  strcpy(azArrWanaOdfs[58].str, "i54");     strcpy(azArrWanaOdfs[59].str, "i32");
  strcpy(azArrWanaOdfs[64].str, "i71");     strcpy(azArrWanaOdfs[65].str, "i71");
  strcpy(azArrWanaOdfs[72].str, "i70");     strcpy(azArrWanaOdfs[73].str, "i70");
  strcpy(azArrWanaOdfs[74].str, "i56");     strcpy(azArrWanaOdfs[75].str, "i43");
  strcpy(azArrWanaOdfs[76].str, "i43");
  strcpy(azArrWanaOdfs[80].str, "i31");     strcpy(azArrWanaOdfs[81].str, "i31");
  strcpy(azArrWanaOdfs[82].str, "i31");     strcpy(azArrWanaOdfs[83].str, "i44");
  strcpy(azArrWanaOdfs[84].str, "i31");     strcpy(azArrWanaOdfs[85].str, "i31");
  strcpy(azArrWanaOdfs[86].str, "i31");
  strcpy(azArrWanaOdfs[88].str, "i53");     strcpy(azArrWanaOdfs[89].str, "i53");
  strcpy(azArrWanaOdfs[90].str, "i53");     strcpy(azArrWanaOdfs[91].str, "i72");
  strcpy(azArrWanaOdfs[92].str, "i72");     strcpy(azArrWanaOdfs[93].str, "i72");
  strcpy(azArrWanaOdfs[94].str, "i72");     strcpy(azArrWanaOdfs[95].str, "i72");
  strcpy(azArrWanaOdfs[104].str, "i51");    strcpy(azArrWanaOdfs[105].str, "i51_57");
  strcpy(azArrWanaOdfs[106].str, "i51_57"); strcpy(azArrWanaOdfs[107].str, "i51_57");
  strcpy(azArrWanaOdfs[108].str, "i51_57"); strcpy(azArrWanaOdfs[109].str, "i51_57");
  strcpy(azArrWanaOdfs[110].str, "i51_57"); strcpy(azArrWanaOdfs[111].str, "i51_57");
#endif

  // --- duel records: foe body / map ODF / map-object ODF / ODS scene
  // (rows 12..19 all zero)
#if 1
  char arrDuelEntries[20][4][50] = {
    { "run\\o2\\o2a", "Map\\M07\\at07.ODF", "Map\\M07\\at07_o.ODF", "run\\Map04.ods" },
    { "run\\o2\\o2b", "Map\\M07\\at07.ODF", "Map\\M07\\at07_o.ODF", "run\\Map04.ods" },
    { "run\\o5\\o5a", "Map\\M21\\at21.ODF", "Map\\M21\\at21_o.ODF", "run\\Map10.ods" },
    { "run\\o5\\o5b", "Map\\M13\\at13.ODF", "Map\\M13\\at13_o.ODF", "run\\Map06.ods" },
    { "run\\o4\\o4a", "Map\\M16\\at16.ODF", "Map\\M16\\at16_o.ODF", "run\\Map08.ods" },
    { "run\\o4\\o4b", "Map\\M04\\at04.ODF", "Map\\M04\\at04_o.ODF", "run\\Map02.ods" },
    { "run\\o3\\o3a", "Map\\M02\\at02.ODF", "Map\\M02\\at02_o.ODF", "run\\Map01.ods" },
    { "run\\o3\\o3b", "Map\\M14\\at14.ODF", "Map\\M14\\at14_o.ODF", "run\\Map07.ods" },
    { "run\\o3\\o3c", "Map\\M16\\at16.ODF", "Map\\M16\\at16_o.ODF", "run\\Map11.ods" },
    { "run\\o1\\o1a", "Map\\M17\\at17.ODF", "Map\\M17\\at17_o.ODF", "run\\Map09.ods" },
    { "run\\o1\\o1b", "Map\\M06\\at06.ODF", "Map\\M06\\at06_o.ODF", "run\\Map03.ods" },
    { "run\\o1\\o1c", "Map\\M08\\at08.ODF", "Map\\M08\\at08_o.ODF", "run\\Map05.ods" },
  };
#else
  memset(arrDuelEntries, 0, sizeof(arrDuelEntries));
  strcpy(arrDuelEntries[0][0].str, "run\\o2\\o2a");
  strcpy(arrDuelEntries[0][1].str, "Map\\M07\\at07.ODF");
  strcpy(arrDuelEntries[0][2].str, "Map\\M07\\at07_o.ODF");
  strcpy(arrDuelEntries[0][3].str, "run\\Map04.ods");
  strcpy(arrDuelEntries[1][0].str, "run\\o2\\o2b");
  strcpy(arrDuelEntries[1][1].str, "Map\\M07\\at07.ODF");
  strcpy(arrDuelEntries[1][2].str, "Map\\M07\\at07_o.ODF");
  strcpy(arrDuelEntries[1][3].str, "run\\Map04.ods");
  strcpy(arrDuelEntries[2][0].str, "run\\o5\\o5a");
  strcpy(arrDuelEntries[2][1].str, "Map\\M21\\at21.ODF");
  strcpy(arrDuelEntries[2][2].str, "Map\\M21\\at21_o.ODF");
  strcpy(arrDuelEntries[2][3].str, "run\\Map10.ods");
  strcpy(arrDuelEntries[3][0].str, "run\\o5\\o5b");
  strcpy(arrDuelEntries[3][1].str, "Map\\M13\\at13.ODF");
  strcpy(arrDuelEntries[3][2].str, "Map\\M13\\at13_o.ODF");
  strcpy(arrDuelEntries[3][3].str, "run\\Map06.ods");
  strcpy(arrDuelEntries[4][0].str, "run\\o4\\o4a");
  strcpy(arrDuelEntries[4][1].str, "Map\\M16\\at16.ODF");
  strcpy(arrDuelEntries[4][2].str, "Map\\M16\\at16_o.ODF");
  strcpy(arrDuelEntries[4][3].str, "run\\Map08.ods");
  strcpy(arrDuelEntries[5][0].str, "run\\o4\\o4b");
  strcpy(arrDuelEntries[5][1].str, "Map\\M04\\at04.ODF");
  strcpy(arrDuelEntries[5][2].str, "Map\\M04\\at04_o.ODF");
  strcpy(arrDuelEntries[5][3].str, "run\\Map02.ods");
  strcpy(arrDuelEntries[6][0].str, "run\\o3\\o3a");
  strcpy(arrDuelEntries[6][1].str, "Map\\M02\\at02.ODF");
  strcpy(arrDuelEntries[6][2].str, "Map\\M02\\at02_o.ODF");
  strcpy(arrDuelEntries[6][3].str, "run\\Map01.ods");
  strcpy(arrDuelEntries[7][0].str, "run\\o3\\o3b");
  strcpy(arrDuelEntries[7][1].str, "Map\\M14\\at14.ODF");
  strcpy(arrDuelEntries[7][2].str, "Map\\M14\\at14_o.ODF");
  strcpy(arrDuelEntries[7][3].str, "run\\Map07.ods");
  strcpy(arrDuelEntries[8][0].str, "run\\o3\\o3c");
  strcpy(arrDuelEntries[8][1].str, "Map\\M16\\at16.ODF");
  strcpy(arrDuelEntries[8][2].str, "Map\\M16\\at16_o.ODF");
  strcpy(arrDuelEntries[8][3].str, "run\\Map11.ods");
  strcpy(arrDuelEntries[9][0].str, "run\\o1\\o1a");
  strcpy(arrDuelEntries[9][1].str, "Map\\M17\\at17.ODF");
  strcpy(arrDuelEntries[9][2].str, "Map\\M17\\at17_o.ODF");
  strcpy(arrDuelEntries[9][3].str, "run\\Map09.ods");
  strcpy(arrDuelEntries[10][0].str, "run\\o1\\o1b");
  strcpy(arrDuelEntries[10][1].str, "Map\\M06\\at06.ODF");
  strcpy(arrDuelEntries[10][2].str, "Map\\M06\\at06_o.ODF");
  strcpy(arrDuelEntries[10][3].str, "run\\Map03.ods");
  strcpy(arrDuelEntries[11][0].str, "run\\o1\\o1c");
  strcpy(arrDuelEntries[11][1].str, "Map\\M08\\at08.ODF");
  strcpy(arrDuelEntries[11][2].str, "Map\\M08\\at08_o.ODF");
  strcpy(arrDuelEntries[11][3].str, "run\\Map05.ods");
#endif


  // --- duel -> act (rows 12..19 all zero)
#if 1
  DWORD arrChapterActMap[20] = {
    0,  0,
    1,  1,
    2,  2,
    3,  3,
    3,  4,
    14, 14,
  };
#else
  arrChapterActMap[0] = 0;   arrChapterActMap[1] = 0;
  arrChapterActMap[2] = 1;   arrChapterActMap[3] = 1;
  arrChapterActMap[4] = 2;   arrChapterActMap[5] = 2;
  arrChapterActMap[6] = 3;   arrChapterActMap[7] = 3;
  arrChapterActMap[8] = 3;   arrChapterActMap[9] = 4;
  arrChapterActMap[10] = 4;  arrChapterActMap[11] = 4;
  memset(&arrChapterActMap[12], 0, 8 * 4);
#endif

  // --- per-duel foe-attack SE for g_pArrSndBuf[3] (rows 12..14 zero)
#if 1
  char arrFoeAtkWavs[15][50] = {
    "act\\Hit4",
    "act\\Hit4",
    "act\\Hit",
    "act\\Hit",
    "act\\Hit3",
    "act\\Hit3",
    "act\\Hit2",
    "act\\Hit2",
    "act\\Hit",
    "act\\Hit",
    "act\\Hit",
    "act\\Hit",
  };
#else
  strcpy(arrFoeAtkWavs[0].str, "act\\Hit4");
  strcpy(arrFoeAtkWavs[1].str, "act\\Hit4");
  strcpy(arrFoeAtkWavs[2].str, "act\\Hit");
  strcpy(arrFoeAtkWavs[3].str, "act\\Hit");
  strcpy(arrFoeAtkWavs[4].str, "act\\Hit3");
  strcpy(arrFoeAtkWavs[5].str, "act\\Hit3");
  strcpy(arrFoeAtkWavs[6].str, "act\\Hit2");
  strcpy(arrFoeAtkWavs[7].str, "act\\Hit2");
  strcpy(arrFoeAtkWavs[8].str, "act\\Hit");
  strcpy(arrFoeAtkWavs[9].str, "act\\Hit");
  strcpy(arrFoeAtkWavs[10].str, "act\\Hit");
  strcpy(arrFoeAtkWavs[11].str, "act\\Hit");
  memset(&arrFoeAtkWavs[12], 0, 3 * 50);
#endif

  // --- UI wav names (3..9 runtime-empty ""; loop below stops at [3]), Start/GameOver/On + "" filler
#if 1
  char arrUiWavNames[10][50] = {
    "Start",
    "GameOver",
    "On",
  };
#else
  strcpy(arrUiWavNames[0].str, "Start");
  strcpy(arrUiWavNames[1].str, "GameOver");
  strcpy(arrUiWavNames[2].str, "On");
  memset(&arrUiWavNames[3], 0, 7 * 50);
#endif

  // --- foot wavs for the 3D foe-hit SFX ([4] zero)
#if 1
  char arrFootWavNames[5][50] = {
    "foot4",
    "foot2",
    "foot3",
    "foot5",
  };
#else
  strcpy(arrFootWavNames[0].str, "foot4");
  strcpy(arrFootWavNames[1].str, "foot2");
  strcpy(arrFootWavNames[2].str, "foot3");
  strcpy(arrFootWavNames[3].str, "foot5");
  memset(&arrFootWavNames[4], 0, 50);
#endif

  // --- BGM names ([3..14] zero)
#if 1
  char arrBgmWavs[15][50] = {
    "s010.wav",
    "s006.wav",
    "s011.wav",
  };
#else
  strcpy(arrBgmWavs[0].str, "s010.wav");
  strcpy(arrBgmWavs[1].str, "s006.wav");
  strcpy(arrBgmWavs[2].str, "s011.wav");
  memset(&arrBgmWavs[3], 0, 12 * 50);
#endif

  // --- voice wavs: act k holds act\A0k_00..12.wav in a 20-slot block
  // (13..19 of each block and the whole blocks 5..6 stay empty)
#if 1
  char arrVoiceWavNames[7][20][50] = {     // 5 acts x 20 slots act\A0X_NN.wav
    {
      "act\\A00_00.wav", "act\\A00_01.wav", "act\\A00_02.wav", "act\\A00_03.wav", "act\\A00_04.wav",
      "act\\A00_05.wav", "act\\A00_06.wav", "act\\A00_07.wav", "act\\A00_08.wav", "act\\A00_09.wav",
      "act\\A00_10.wav", "act\\A00_11.wav", "act\\A00_12.wav"
    },
    {
      "act\\A01_00.wav", "act\\A01_01.wav", "act\\A01_02.wav", "act\\A01_03.wav", "act\\A01_04.wav",
      "act\\A01_05.wav", "act\\A01_06.wav", "act\\A01_07.wav", "act\\A01_08.wav", "act\\A01_09.wav",
      "act\\A01_10.wav", "act\\A01_11.wav", "act\\A01_12.wav"
    },
    {
      "act\\A02_00.wav", "act\\A02_01.wav", "act\\A02_02.wav", "act\\A02_03.wav", "act\\A02_04.wav",
      "act\\A02_05.wav", "act\\A02_06.wav", "act\\A02_07.wav", "act\\A02_08.wav", "act\\A02_09.wav",
      "act\\A02_10.wav", "act\\A02_11.wav", "act\\A02_12.wav"
    },
    {
      "act\\A03_00.wav", "act\\A03_01.wav", "act\\A03_02.wav", "act\\A03_03.wav", "act\\A03_04.wav",
      "act\\A03_05.wav", "act\\A03_06.wav", "act\\A03_07.wav", "act\\A03_08.wav", "act\\A03_09.wav",
      "act\\A03_10.wav", "act\\A03_11.wav", "act\\A03_12.wav"
    },
    {
      "act\\A04_00.wav", "act\\A04_01.wav", "act\\A04_02.wav", "act\\A04_03.wav", "act\\A04_04.wav",
      "act\\A04_05.wav", "act\\A04_06.wav", "act\\A04_07.wav", "act\\A04_08.wav", "act\\A04_09.wav",
      "act\\A04_10.wav", "act\\A04_11.wav", "act\\A04_12.wav"
    },
  };
#else
  memset(arrVoiceWavNames, 0, sizeof(arrVoiceWavNames));
  for (i = 0; i < 13; ++i)
    wsprintfA(arrVoiceWavNames[0][i], "act\\A00_%02d.wav", i);
  for (i = 0; i < 13; ++i)
    wsprintfA(arrVoiceWavNames[20 + i].str, "act\\A01_%02d.wav", i);
  for (i = 0; i < 13; ++i)
    wsprintfA(arrVoiceWavNames[40 + i].str, "act\\A02_%02d.wav", i);
  for (i = 0; i < 13; ++i)
    wsprintfA(arrVoiceWavNames[60 + i].str, "act\\A03_%02d.wav", i);
  for (i = 0; i < 13; ++i)
    wsprintfA(arrVoiceWavNames[80 + i].str, "act\\A04_%02d.wav", i);
#endif
  // --- foe face bmp/mask pairs per act ([5] runtime-empty)
  // (.data copies spelled out; [4][1] was a literal in the original)
#if 1
  char arrFoeBmpNames[6][2][25] = {
    {"run\\o2a.bmp",  "run\\o2b.bmp"},
    {"run\\o5a.bmp",  "run\\o5b.bmp"},
    {"run\\o4a.bmp",  "run\\o4b.bmp"},
    {"run\\o3a.bmp",  "run\\o3b.bmp"},
    {"run\\o1a.bmp",  "run\\o1b.bmp"},
  };
  float arrFoeHpMax[20] = {
    20000.0f, 20000.0f,
    40000.0f, 40000.0f,
    35000.0f, 35000.0f,
    45000.0f, 45000.0f,
    45000.0f, 40000.0f,
    40000.0f, 40000.0f,
  };
#else
  strcpy(arrFoeBmpNames[0][0], "run\\o2a.bmp");  strcpy(arrFoeBmpNames[0][1], "run\\o2b.bmp");
  strcpy(arrFoeBmpNames[1][0], "run\\o5a.bmp");  strcpy(arrFoeBmpNames[1][1], "run\\o5b.bmp");
  strcpy(arrFoeBmpNames[2][0], "run\\o4a.bmp");  strcpy(arrFoeBmpNames[2][1], "run\\o4b.bmp");
  strcpy(arrFoeBmpNames[3][0], "run\\o3a.bmp");  strcpy(arrFoeBmpNames[3][1], "run\\o3b.bmp");
  strcpy(arrFoeBmpNames[4][0], "run\\o1a.bmp");  strcpy(arrFoeBmpNames[4][1], "run\\o1b.bmp");
  arrFoeBmpNames[5][0][0] = 0;
  arrFoeBmpNames[5][1][0] = 0;

  arrFoeHpMax[0] = 20000.0f;   arrFoeHpMax[1] = 20000.0f;
  arrFoeHpMax[2] = 40000.0f;   arrFoeHpMax[3] = 40000.0f;
  arrFoeHpMax[4] = 35000.0f;   arrFoeHpMax[5] = 35000.0f;
  arrFoeHpMax[6] = 45000.0f;   arrFoeHpMax[7] = 45000.0f;
  arrFoeHpMax[8] = 45000.0f;   arrFoeHpMax[9] = 40000.0f;
  arrFoeHpMax[10] = 40000.0f;  arrFoeHpMax[11] = 40000.0f;
  memset(&arrFoeHpMax[12], 0, 8 * 4);
#endif

  // --- state init
  nSysMenuResult = 0;
  pFoeTalkFrame = 0;
  pBgmBuf = 0;
  pfExecTime = 0.0f;
  pOdsScene = 0;
  pMapFrame = 0;
  pMapObjFrame = 0;
  pHeroBody = 0;
  pFoeBody = 0;
  pMaskBmp = 0;
  bActing = 0;
  bCinematic = 0;
  dwElapsedMs = 0;
  nClockBase = 180;
  fHeroHpMax = g_sceneState_52F7E0.protMaximumHP_C4;
  fHeroHpCur = g_sceneState_52F7E0.protCurrentHP_C0;
  fActTimer = 0.0f;
  fFoeHpMax = 99.0f;
  fFoeHpCur = 99.0f;

  bFirstFrameDone = 0;
  memcpy(&sceneStateEntry, &g_sceneState_52F7E0, sizeof(sceneStateEntry));
  SetProjectionFov_40DCA3(1.2f);
  SetTimerScaleFactor(14.0f);
  memset(pWanaBodies, 0, sizeof(pWanaBodies));
  memset(pArrHudMeters, 0, sizeof(pArrHudMeters));
  memset(pArrSprites, 0, sizeof(pArrSprites));
  memset(arrSparkSprites, 0, sizeof(arrSparkSprites));
  memcpy(&g_sceneState_52F7E0, &sceneStateEntry, sizeof(g_sceneState_52F7E0)); // (round-trip as-is)
  memset(g_pArrSndBuf_5244AC, 0, sizeof(g_pArrSndBuf_5244AC));
  memset(g_pFoeHitSfxE_5244E4, 0, 12); // (sic) original zeroes only 3 slots
  memset(g_pFoeHitSfxB_5244D4, 0, 12); // (sic) ditto
  memset(arrVoiceBufs, 0, sizeof(arrVoiceBufs));

  // --- one-time loads: UI SFX, foe-attack SE, foe-hit 3D SFX, voices, BGM
  for (i = 0; i < 9 && arrUiWavNames[i][0]; ++i)
  {
    wsprintfA(pszFileName, "%s.wav", arrUiWavNames[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
    g_pArrSndBuf_5244AC[i] = LoadWav_422E7B(pszFileName);
  }
  wsprintfA(pszFileName, "%s.wav", arrFoeAtkWavs[nDuelIndex]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1009);          // SE dir
  g_pArrSndBuf_5244AC[3] = LoadWav_422E7B(pszFileName);
  for (i = 0; i < 4; ++i)
  {
    wsprintfA(pszFileName, "%s.wav", arrFootWavNames[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
    g_pFoeHitSfxE_5244E4[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
    g_pFoeHitSfxB_5244D4[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
  }
  for (i = 0; i < 20; ++i)
  {
    wsprintfA(pszFileName, "%s",
      arrVoiceWavNames[arrChapterActMap[nDuelIndex]][i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
    arrVoiceBufs[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
  }
  if (!nDuelIndex || nDuelIndex == 2 || nDuelIndex == 4 || nDuelIndex == 6
    || nDuelIndex == 9)
    i = 0;
  else if (nDuelIndex == 7 || nDuelIndex == 10)
    i = 2;
  else
    i = 1;
  wsprintfA(pszFileName, "%s", arrBgmWavs[i]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1006);          // WAV dir
  pBgmBuf = LoadWav_422E7B(pszFileName);
  if (pBgmBuf)
  {
    // clamp table[aviVolume] - 1000 dB to [-10000, 0]
    if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] - 1000.0) >= -10000)
    {
      if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] - 1000.0) <= 0)
        pBgmBuf->SetVolume((__int64)(g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] - 1000.0));
      else
        pBgmBuf->SetVolume(0);
    }
    else
    {
      pBgmBuf->SetVolume(-10000);
    }
  }
  InitHudResources_4490A7(pArrHudMeters, pArrSprites, arrSparkSprites,
    arrChapterActMap[nDuelIndex]);

  // foe face BMP pair
  wsprintfA(pszFileName, "%s", arrFoeBmpNames[arrChapterActMap[nDuelIndex]][0]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1003);          // SPR dir
  wsprintfA(String, "%s", arrFoeBmpNames[arrChapterActMap[nDuelIndex]][1]);
  CHanaObj__GetRealPath_486D9A(String, 1003);               // SPR dir
  LoadBmpWithMask_413A5E(&pMaskBmp, pszFileName, String);

  // hero, foe and wana bodies
  wsprintfA(pszFileName, "otoko\\otoko.sek");
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
  wsprintfA(String, "otoko\\otoko.odf");
  CHanaObj__GetRealPath_486D9A(String, 1005);               // ODF dir
  if (!LoadSekAndOdf_4445DD(pszFileName, String, &pHeroBody))
    return -1;
  wsprintfA(pszFileName, "%s.sek", arrDuelEntries[nDuelIndex][0]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
  wsprintfA(String, "%s.odf", arrDuelEntries[nDuelIndex][0]);
  CHanaObj__GetRealPath_486D9A(String, 1005);               // ODF dir
  if (!LoadSekAndOdf_4445DD(pszFileName, String, &pFoeBody))
    return -1;
  for (i = 0; i < 8 && szArrWanaSeks[nDuelIndex][i][0]; ++i)
  {
    wsprintfA(pszFileName, "wana\\%s.sek", szArrWanaSeks[nDuelIndex][i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);      // ODF dir
    wsprintfA(String, "wana\\%s.ODF", azArrWanaOdfs[nDuelIndex][i]);
    CHanaObj__GetRealPath_486D9A(String, 1005);           // ODF dir
    if (!LoadSekAndOdf_4445DD(pszFileName, String, &pWanaBodies[i]))
      return -1;
  }

  // map + map-object ODF frames, collision grid, ODS scene
  pMapOdfAnimSet = 0;
  wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][1]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
  if (!ParseODFFile_441F19(pszFileName, &pMapFrame, &pMapOdfAnimSet))
    return -1;
  if (pMapOdfAnimSet)
  {
    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pMapOdfAnimSet);
    pMapOdfAnimSet = 0;
  }
  AttachChild_4055FD(g_pSceneFrame, pMapFrame);
  SetFramePosRel_4069DD(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
  UpdateBoneLookAtTransform(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
  BuildMapSubFrames_443DD5(pMapFrame);

  pMapObjAnimSet = 0;
  wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][2]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
  if (!ParseODFFile_441F19(pszFileName, &pMapObjFrame, &pMapObjAnimSet))
    return -1;
  if (pMapObjAnimSet)
  {
    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pMapObjAnimSet);
    pMapObjAnimSet = 0;
  }
  AttachChild_4055FD(g_pSceneFrame, pMapObjFrame);
  SetFramePosRel_4069DD(pMapObjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
  UpdateBoneLookAtTransform(pMapObjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
  BuildMapSubFrames_443DD5(pMapObjFrame);
  BuildMapCollisionGrid_445B12(pMapObjFrame, &pOccGrid);

  wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][3]);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1002);          // ODS dir
  if (!LoadOds_4843CF(pszFileName, &pOdsScene))
    return -1;
  pOdsScene->WCurNum = 100000.0f;

  // bind hero + foe + wana frames to the scene
  nCount = 2;
  pActorFrames[0] = pHeroBody->pSekHeader_220->pFrameT3EC;
  pActorFrames[1] = pFoeBody->pSekHeader_220->pFrameT3EC;
  for (i = 0; i < 8 && pWanaBodies[i]; ++i)
  {
    pActorFrames[i + 2] = pWanaBodies[i]->pSekHeader_220->pFrameT3EC;
    ++nCount;
  }
  BindSceneActors_448C72(pOdsScene, pActorFrames, nCount);

  // opening jingle + one test frame
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFF);
  if (g_pArrSndBuf_5244AC[0])
  {
    // clamp table[aviVolume] dB to [-10000, 0]
    if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] >= -10000)
    {
      if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] <= 0)
        g_pArrSndBuf_5244AC[0]->SetVolume((__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266]);
      else
        g_pArrSndBuf_5244AC[0]->SetVolume(0);
    }
    else
    {
      g_pArrSndBuf_5244AC[0]->SetVolume(-10000);
    }
    DSBufferPlay(g_pArrSndBuf_5244AC[0], 0);
  }

  RenderDualBodyScene_456807(pOdsScene, pHeroBody, pFoeBody, arrSparkSprites,
    pArrSprites);

  if (g_pArrSndBuf_5244AC[0])
    g_pArrSndBuf_5244AC[0]->Stop();
  if (g_pArrSndBuf_5244AC[2])
  {
    // clamp table[aviVolume] dB to [-10000, 0]
    if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] >= -10000)
    {
      if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266] <= 0)
        g_pArrSndBuf_5244AC[2]->SetVolume((__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266]);
      else
        g_pArrSndBuf_5244AC[2]->SetVolume(0);
    }
    else
    {
      g_pArrSndBuf_5244AC[2]->SetVolume(-10000);
    }
    DSBufferPlay(g_pArrSndBuf_5244AC[2], 0);
  }

  // park the actors; capture pos/rot for the round restarts
  HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame,
    &fHeroX, &fHeroY, &fHeroZ);
  CalculateLocalRotationMatrix(pHeroBody->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &dirForwardX, &dirForwardY, &dirForwardZ, &dirUpX, &dirUpY, &dirUpZ);
  HanaFrame_GetRelPosition(pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame,
    &x, &y, &z);
  CalculateLocalRotationMatrix(pFoeBody->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &pXpos, &pYpos, &pZpos, &pXYrot, &pYXrot, &pZXrot);
  UpdateCameraLinkedObjects_44451F(g_pCameraFrame);
  SetTimerScaleFactor(12.0f);

  while (1)  // outer: one round (restarts after a cinematic or a wana reveal)
  {
  LABEL_68:
    if (pBgmBuf)
      DSBufferPlay(pBgmBuf, 1);
    RestartInputThread(1);
    Sleep(0);
    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
    UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      dirForwardX, dirForwardY, dirForwardZ, dirUpX, dirUpY, dirUpZ);
    SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, z);
    UpdateBoneLookAtTransform(pFoeBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      pXpos, pYpos, pZpos, pXYrot, pYXrot, pZXrot);
    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC,
      g_pCameraFrame, pMapFrame, fDelta, 1);
    fFoeHpMax = arrFoeHpMax[nDuelIndex];
    fFoeHpCur = fFoeHpMax;
    fActTimer = 0.0f;
    pfExecTime = 0.0f;
    bActing = 0;
    bCinematic = 0;
    dwElapsedMs = 0;
    g_dwBattleMode = -1;
    CSekBody__CutToAnim_4018E1(pHeroBody, 1);
    CSekBody__UpdateAnim_4019E1(pHeroBody, 0.0f);
    CSekBody__PlayAnim_40179D(pHeroBody, 1);
    CSekBody__CutToAnim_4018E1(pFoeBody, 1);
    CSekBody__UpdateAnim_4019E1(pFoeBody, 0.0f);
    CSekBody__PlayAnim_40179D(pFoeBody, 1);
    for (i = 0; i < 8; ++i)
    {
      if (pWanaBodies[i])
        CSekBody__CutToAnim_4018E1(pWanaBodies[i], 0);
    }
    pFoeTalkFrame = FindFrameByName_47E999(
      pFoeBody->pSekHeader_220->pFrameT3EC, "_04_kao"); // @0x4B5220
    fHeroHpMax = g_sceneState_52F7E0.protMaximumHP_C4;
    fHeroHpCur = g_sceneState_52F7E0.protCurrentHP_C0;
    Time = timeGetTime();
    dwClockBase2 = Time;
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFF);
    do  // inner: frame loop
    {
      DoPeekMessages();
      dwElapsedMs = timeGetTime() - dwClockBase2;
      fDelta = (float)(GetFrameTime() / ((double)bCinematic * 3.0 + 1.0));
      if (fDelta > 5.0f)
        fDelta = 5.0f;
      FrameTimer_EndFrame();

      // menu key -> system menu on the BGM
      if (IsMenuKeyPressed_481170())
      {
        memcpy(&sceneStateLive, &g_sceneState_52F7E0, sizeof(sceneStateLive));
        memcpy(&g_sceneState_52F7E0, &sceneStateEntry, sizeof(g_sceneState_52F7E0));
        Time = timeGetTime();
        for (i = 0; i < 20; ++i)
        {
          if (arrVoiceBufs[i] && arrVoiceBufs[i]->pDSBuffer_0)
            arrVoiceBufs[i]->pDSBuffer_0->Stop();
        }
        RestartInputThread(0);
        sceneMenuCopy = sceneStateLive;
        nSysMenuResult = SystemMenu_4655AB(pBgmBuf, -1000, 0, fHeroHpCur,
          fHeroHpMax, sceneMenuCopy);
        RestartInputThread(1);
        bFirstFrameDone = 0;
        if (nSysMenuResult >= 1)
          break;
        memcpy(&g_sceneState_52F7E0, &sceneStateLive, sizeof(g_sceneState_52F7E0));
      }

      if (bCinematic)
      {
        // cut-in: hide the wanas, play the sequence, then unhide the
        // live (not yet revealed-dead) ones
        bCinematic = 0;
        bActing = 0;
        fActTimer = 120.0f;
        for (i = 0; i < 8; ++i)
        {
          if (pWanaBodies[i])
            pWanaBodies[i]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
        }
        PlayTugOfWar_45460A(
          pOdsScene, pHeroBody, pFoeBody, pMapFrame,
          pArrHudMeters, pArrSprites,
          &fHeroHpCur, fHeroHpMax, &fFoeHpCur, fFoeHpMax,
          &nClockBase, &dwElapsedMs, dwClockBase2, arrVoiceBufs,
          pMaskBmp);
        for (i = 0; i < 8; ++i)
        {
          if (pWanaBodies[i] && pWanaBodies[i]->field_24C != 1)
            pWanaBodies[i]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
        }
      }
      else
      {
        // hero action-intro timer runs the anim at x3/2 speed
        fActTimerNew = fActTimer - fDelta * 3.0 / 2.0;
        fActTimer = (float)fActTimerNew;
        if (fActTimerNew > 0.0)
        {
          CSekBody__UpdateAnim_4019E1(pHeroBody, fDelta * 3.0f / 2.0f);
        }
        else
        {
          bActing = UpdateHeroBody_44474E(pHeroBody, pMapFrame, fDelta,
            &fHeroHpCur, fHeroHpMax);
          fActTimer = 0.0f;
        }
        if (pHeroBody->field_200 <= 6 && g_nFirstPersonCam_530268)
        {
          UpdateCameraFollowDuel_44C028(pHeroBody, pFoeBody, fDelta);
        }
        else
        {
          pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
          UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC,
            g_pCameraFrame, pMapFrame, fDelta, 0);
        }
        updated = UpdateFoeAI_446A38(pFoeBody, pHeroBody, pMapFrame,
          pOccGrid, fDelta, &fFoeHpCur, &fHeroHpCur,
          fFoeHpMax, nDuelIndex);
        if (updated != 0.0 && g_pArrSndBuf_5244AC[3])
          g_pArrSndBuf_5244AC[3]->Play(0, 0, 0);
        fActTimer = (float)(fActTimer + updated);
        UpdateVoicePlayback_458B3A(pFoeBody,
          (CHanaFrameRootT3EC_S2D4 *)pFoeTalkFrame, arrVoiceBufs,
          fDelta);
        for (i = 0; i < 8; ++i)
        {
          if (pWanaBodies[i])
          {
            updated = arrWanaAtkFns[arrWanaAtkTypes[nDuelIndex][i]](
              pWanaBodies[i], pHeroBody, &fHeroHpCur, fDelta,
              arrWanaAtkParams[nDuelIndex][i], pMapFrame);
            fActTimer = (float)(updated + fActTimer);
          }
        }
        if (bActing && g_dwBattleMode >= 0)
          bCinematic = IsFoeWithinHeroRange_448B78(pHeroBody, pFoeBody);
      }

      // ODS scene clock (loops at the anim-set length)
      if (pOdsScene)
      {
        UpdateSceneTimer_4827FE(pOdsScene, &pfExecTime, fDelta);
        UpdateSceneAtTime_4825D5(pOdsScene, &pfExecTime, fDelta);
        if (pfExecTime >= (double)pOdsScene->pAnimSet_148->fRes_13C)
          pfExecTime = 0.0f;
      }

      // draw: 3D pass + HUD
      MtxStackStartMultLocal_4074EF();
      g_pRenderCtx->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);
      if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
      {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
        HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        HanaVert_DrawBatch(1);
        g_pD3DDevice_4D6C78->EndScene();
      }
      DrawDuelHealthHud_44B23B(pArrSprites,
        (int)((__int64)fHeroHpCur / 100),
        (int)((__int64)fHeroHpMax / 100),
        (int)((__int64)fFoeHpCur / 100),
        (int)((__int64)fFoeHpMax / 100));
      DrawClockHud_44AF5B(pArrHudMeters, 320, 16,
        100 * nClockBase - (int)(dwElapsedMs / 10));
      DrawFadeOutSpark_45669F(arrSparkSprites, fDelta);
      if (bFirstFrameDone)
      {
        PresentFrame();
        HanaVert_DrawBatch(0);
      }
      else
      {
        bFirstFrameDone = 1;
        PlayScreenFade(1, 0, g_pBackBuffer_4D6C90, 50.0f, 0);
        dwClockBase2 += timeGetTime() - Time;
        Time = timeGetTime();
      }
    }
    while ((100 * nClockBase - (int)dwElapsedMs / 10 > 0 && fFoeHpCur > 0.0f
      && fHeroHpCur > 0.0f)
      || pHeroBody->field_200 == 16 || pHeroBody->field_200 == 10
      || pHeroBody->field_200 == 11 || pHeroBody->field_200 == 13);

    // --- round ended: menu exit / defeat-timeout / victory / next wana
    RestartInputThread(0);
    if (pBgmBuf)
      pBgmBuf->Stop();
    for (i = 0; i < 20; ++i)
    {
      if (arrVoiceBufs[i] && arrVoiceBufs[i]->pDSBuffer_0)
        arrVoiceBufs[i]->pDSBuffer_0->Stop();
    }
    pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
    if (nSysMenuResult >= 1)
      break;
    if (fHeroHpCur <= 0.0f || 100 * nClockBase - (int)dwElapsedMs / 10 <= 0)
    {
      // defeat / timeout
      if (g_pArrSndBuf_5244AC[1])
      {
        g_pArrSndBuf_5244AC[1]->SetVolume(-500);
        DSBufferPlay(g_pArrSndBuf_5244AC[1], 0);
      }
      PlayResultsCutscene_455BE2(pHeroBody, pFoeBody, pWanaBodies, pMapFrame,
        pArrHudMeters, pArrSprites, fHeroHpCur, fHeroHpMax,
        fFoeHpCur, fFoeHpMax, nClockBase, dwElapsedMs);
      nSelectResult = ShowMouseSelectDialog_4573CF(arrSparkSprites,
        g_pArrSndBuf_5244AC);
      PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0f, 0);
      // heal 15% of the persisted max HP (clamped)
      g_sceneState_52F7E0.protCurrentHP_C0 =
        g_sceneState_52F7E0.protMaximumHP_C4 * 15.0f / 100.0f
        + g_sceneState_52F7E0.protCurrentHP_C0;
      if (g_sceneState_52F7E0.protCurrentHP_C0 > (double)g_sceneState_52F7E0.protMaximumHP_C4)
        g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4;
      fHeroHpCur = g_sceneState_52F7E0.protCurrentHP_C0;
      if (nSelectResult)
        goto LABEL_154;    // keep going: reveal the next wana
      nSysMenuResult = 1;
      break;
    }
    if (fFoeHpCur <= 0.0f)
    {
      // victory
      g_sceneState_52F7E0.protCurrentHP_C0 = fHeroHpCur;
      g_sceneState_52F7E0.protMaximumHP_C4 = fHeroHpMax;
      PlayVictoryCutscene_455183(pOdsScene, pHeroBody, pFoeBody, pMapFrame,
        pArrHudMeters, pArrSprites, fHeroHpCur, fHeroHpMax,
        fFoeHpCur, fFoeHpMax, nClockBase, dwElapsedMs,
        arrVoiceBufs);
      PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 2.0f, 0);
      break;
    }
  LABEL_154:
    // reveal the next live wana (mark dead + hide) and restart the round
    for (i = 7; i >= 0; --i)
    {
      if (pWanaBodies[i] && !pWanaBodies[i]->field_24C)
      {
        pWanaBodies[i]->field_24C = 1;
        pWanaBodies[i]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
        goto LABEL_68;
      }
    }
  }

  // --- cleanup
  RestartInputThread(0);
  for (i = 0; i < 10; ++i)
  {
    if (g_pArrSndBuf_5244AC[i])
    {
      g_pArrSndBuf_5244AC[i]->Stop();
      g_pArrSndBuf_5244AC[i]->Release();
      g_pArrSndBuf_5244AC[i] = 0;
    }
  }
  for (i = 0; i < 4; ++i)
  {
    if (g_pFoeHitSfxE_5244E4[i])
      FreeSoundStruct_422C72(g_pFoeHitSfxE_5244E4[i]);
    if (g_pFoeHitSfxB_5244D4[i])
      FreeSoundStruct_422C72(g_pFoeHitSfxB_5244D4[i]);
  }
  if (pBgmBuf)
  {
    pBgmBuf->Stop();
    pBgmBuf->Release();
    pBgmBuf = 0;
  }
  for (i = 0; i < 20; ++i)
  {
    if (arrVoiceBufs[i])
      FreeSoundStruct_422C72(arrVoiceBufs[i]);
  }
  CSekBody__Release_4012FE(pHeroBody);
  CSekBody__Release_4012FE(pFoeBody);
  for (i = 0; i < 8; ++i)
  {
    if (pWanaBodies[i])
    {
      AttachChild_4055FD(g_pSceneFrame,       // (sic) re-attach before the
        pWanaBodies[i]->pSekHeader_220->pFrameT3EC); // release
      CSekBody__Release_4012FE(pWanaBodies[i]);
    }
  }
  if (pOdsScene)
  {
    CHanaObj__Free_40C1E4(pOdsScene);
    pOdsScene = 0;
  }
  if (pMapFrame)
  {
    CHanaObj__Free_40C1E4(pMapFrame);
    pMapFrame = 0;
  }
  if (pMapObjFrame)
  {
    CHanaObj__Free_40C1E4(pMapObjFrame);
    pMapObjFrame = 0;
  }
  for (i = 0; i < 20; ++i)
  {
    if (pArrSprites[i])
    {
      CHanaObj__Free_40C1E4(pArrSprites[i]);
      pArrSprites[i] = 0;
    }
  }
  for (i = 0; i < 15; ++i)
  {
    if (pArrHudMeters[i])
    {
      CHanaObj__Free_40C1E4(pArrHudMeters[i]);
      pArrHudMeters[i] = 0;
    }
  }
  for (i = 0; i < 15; ++i)
  {
    if (arrSparkSprites[i])
    {
      CHanaObj__Free_40C1E4(arrSparkSprites[i]);
      arrSparkSprites[i] = 0;
    }
  }
  if (pMaskBmp)
  {
    CHanaObj__Free_40C1E4(pMaskBmp);
    pMaskBmp = 0;
  }
  return nSysMenuResult;
}


// UpdateFoeAI -- the duel foe brain (one call per frame per foe)
//
//   Returns fHitImpact (0 normally, 122 when the foe lands its tackle).
//
//   Tuning table arrFoeTuning[20][10] (rows 0..11 inlined per duel, 12..19 zeroed) -- columns by usage:
//
//   +-----+----------------------------------------+
//   | Col |          Meaning (from usage)          |
//   +-----+----------------------------------------+
//   | [0] | move speed multiplier                  |
//   | [1] | tackle damage (*pfHeroHpCur -= 2000*[1])  |
//   | [2] | engage radius (compared squared)       |
//   | [3] | tackle lunge Z offset                  |
//   | [4] | tackle hit radius                      |
//   | [5] | turn/blend rate                        |
//   | [6] | attack approach radius (squared cmp)   |
//   +-----+----------------------------------------+
//
//   Flow
//
//   fAggroSpeedMul = *fFoeHpCur/fFoeHpMax/8 + 0.975         // aggro -> speed scaling
//   save hero (x2,y2,z2), foe (fFoeSaveX/Y/Z); g_fFoeCdTimer -= fDelta
//   hero cell (nHeroCellX/Z = pos/5); dist squared = |foe->hero| squared
//
//   IF engaged (state 5, or within engage radius & anim preconditions):
//       blend toward hero: two CalculateLocalRotationMatrix passes around
//       HanaMatrix_BuildLookAtAndTransform, dirForwardX/Z = lerp(fwd->-fwd, 0.3*fBlendFactor)
//       UpdateBoneLookAtTransform (face the hero)
//       anim dispatch by pFoeBody->field_200: 6 = strafe (fZNudge, reversed duels 4/5),
//         20 = tackle (lunge [3]; if within [4] -> hero anim 9, -2000*[1] HP,
//                      g_dwBattleMode = -1, fHitImpact = 122), else anim 6
//       anim rate fAnimRateA = 2f - f*fAggroSpeedMul; on finish of 21/3 -> anim 2
//
//   ELSE (not engaged) -- patrol/approach:
//       timer/charge state -> g_dwFoeAiState = rand()%3, g_fFoeAiTimer = rand()%250+50
//       nFoeAction: 0 idle / 1 (duels 2,3) / 2 (duels 4,5) / from state / 3 if boxed in
//         (occupancy-grid probe of 5 neighbor cells)
//       if nFoeAction >= 2: StepFoeTurn/StepFoeSpin, aggro -8*f
//       else: HanaGrid_FloodFill(floodCtx, pOccGrid, foe cell, hero cell, &nFloodParm=5, ...)
//         -> first step (fStepX0/fStepZ0) + waypoint list ppOutPathPts
//         fMoveBudget = fTimeDiv*speed[0]*fAggroSpeedMul (clamp 4)
//         walk the step, then consume remaining budget through waypoints (x5 cell scale)
//         stuck check (moved < 0.01 & cell cached) -> face hero faster, aggro +8.1*f (clamp max)
//         else gentle look + 0.2 degrees/frame spin when facing >=0.99
//       SekBody__UpdateTo2(nMoveMode 1..5)
//
//   min-distance separation: if |foe->hero| < 5, push foe to exactly 5 units (fSepX/fSepZ)
//   3x move/collision loop (CheckFrameCollision + TestMoveCollision -- same as hero)
//   aggro floor 10; footstep SFX via g_pFoeHitSfxB/E[nHitSfx] with anim-frame windows
//     (100-108 / 200-206 / 400-406 and 107-115 / 205-212 / 407-415)
//   restore hero pos; return fHitImpact
// pFoeBody, pHeroBody, pMapFrame, pOccGrid, fDelta, &fFoeHpCur, &fHeroHpCur, fFoeHpMax, nDuelIndex
double __cdecl hook_UpdateFoeAI_446A38(
  CSekBody *pFoeBody,
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  char *pOccGrid,
  float fDelta,
  float *pfFoeHpCur,
  float *pfHeroHpCur,
  float fFoeHpMax,
  int nDuelIndex)
{
  D3DXVECTOR2 *pFloodStep; // eax
  float dirForwardX; // [esp+0h] [ebp-4F4h]
  float dirForwardXa; // [esp+0h] [ebp-4F4h]
  float dirForwardXb; // [esp+0h] [ebp-4F4h]
  float dirForwardZ; // [esp+8h] [ebp-4ECh]
  float dirForwardZa; // [esp+8h] [ebp-4ECh]
  float dirForwardZb; // [esp+8h] [ebp-4ECh]
  float fMoveX_a; // [esp+Ch] [ebp-4E8h]
  float fMoveX_b; // [esp+Ch] [ebp-4E8h]
  float fHeroPushX; // [esp+Ch] [ebp-4E8h]
  float fTurnRate2; // [esp+10h] [ebp-4E4h]
  float fZNudge; // [esp+14h] [ebp-4E0h]
  float fAnimRateA; // [esp+14h] [ebp-4E0h]
  float fMoveZ_a; // [esp+14h] [ebp-4E0h]
  float fMoveZ_b; // [esp+14h] [ebp-4E0h]
  float fSpeedMul; // [esp+14h] [ebp-4E0h]
  float fAnimRateB; // [esp+14h] [ebp-4E0h]
  float fWpMoveX; // [esp+Ch] [ebp-4E8h]
  float fWpMoveZ; // [esp+14h] [ebp-4E0h]
  float fHeroPushZ; // [esp+14h] [ebp-4E0h]
  D3DXVECTOR2 pfOutStep; // [esp+40h] [ebp-4B4h] BYREF
  CSekItem *pAnimItem; // [esp+48h] [ebp-4ACh]
  float pz2; // [esp+4Ch] [ebp-4A8h] BYREF
  float py2; // [esp+50h] [ebp-4A4h] BYREF
  float px2; // [esp+54h] [ebp-4A0h] BYREF
  float fSepZ; // [esp+58h] [ebp-49Ch]
  float v41; // [esp+5Ch] [ebp-498h]
  float fSepX; // [esp+60h] [ebp-494h]
  float fLookTgtX; // [esp+64h] [ebp-490h] BYREF
  float fFoeZ2; // [esp+68h] [ebp-48Ch] BYREF
  float xpos; // [esp+6Ch] [ebp-488h] BYREF
  float fFoeY2; // [esp+70h] [ebp-484h] BYREF
  float fFoeX2; // [esp+74h] [ebp-480h] BYREF
  float fLookTgtZ; // [esp+78h] [ebp-47Ch] BYREF
  float zrot; // [esp+7Ch] [ebp-478h] BYREF
  float zpos; // [esp+80h] [ebp-474h] BYREF
  float yrot; // [esp+84h] [ebp-470h] BYREF
  float xrot; // [esp+88h] [ebp-46Ch] BYREF
  float v53; // [esp+8Ch] [ebp-468h] BYREF
  float ypos; // [esp+90h] [ebp-464h] BYREF
  float fTurnBlend; // [esp+94h] [ebp-460h]
  float fFwdAfterX; // [esp+98h] [ebp-45Ch] BYREF
  float fFwdBeforeX; // [esp+9Ch] [ebp-458h] BYREF
  float fFwdAfterZ; // [esp+A0h] [ebp-454h] BYREF
  float v59; // [esp+A4h] [ebp-450h] BYREF
  float fFwdBeforeZ; // [esp+A8h] [ebp-44Ch] BYREF
  float v61; // [esp+ACh] [ebp-448h] BYREF
  float v62; // [esp+B0h] [ebp-444h] BYREF
  float fFwdAfterY; // [esp+B4h] [ebp-440h] BYREF
  float fFwdBeforeY; // [esp+B8h] [ebp-43Ch] BYREF
  float fWpProbeZ; // [esp+BCh] [ebp-438h] BYREF
  float fWpProbeY; // [esp+C0h] [ebp-434h] BYREF
  float fWpProbeX; // [esp+C4h] [ebp-430h] BYREF
  float fMoveBudget; // [esp+C8h] [ebp-42Ch]
  float fStepX; // [esp+CCh] [ebp-428h]
  float fStepZ; // [esp+D0h] [ebp-424h]
  WORD pnOutNumbPathPts; // [esp+D4h] [ebp-420h] BYREF
  float fStepSpeed; // [esp+D8h] [ebp-41Ch]
  float fTimeDiv; // [esp+DCh] [ebp-418h]
  float fHeroDist; // [esp+E0h] [ebp-414h]
  int nMoveMode; // [esp+E4h] [ebp-410h]
  float fBlendFactor; // [esp+E8h] [ebp-40Ch]
  float fLookX1; // [esp+ECh] [ebp-408h] BYREF
  float pXpos; // [esp+F0h] [ebp-404h] BYREF
  float fLookZ1; // [esp+F4h] [ebp-400h] BYREF
  float pZXrot; // [esp+F8h] [ebp-3FCh] BYREF
  float pZpos; // [esp+FCh] [ebp-3F8h] BYREF
  float pYXrot; // [esp+100h] [ebp-3F4h] BYREF
  float pXYrot; // [esp+104h] [ebp-3F0h] BYREF
  float v84; // [esp+108h] [ebp-3ECh] BYREF
  float pYpos; // [esp+10Ch] [ebp-3E8h] BYREF
  __int16 v86; // [esp+110h] [ebp-3E4h]
  float px; // [esp+114h] [ebp-3E0h] BYREF
  int nMode; // [esp+118h] [ebp-3DCh]
  __int16 v89; // [esp+11Ch] [ebp-3D8h]
  __int16 v90; // [esp+120h] [ebp-3D4h]
  float fAggroSpeedMul; // [esp+124h] [ebp-3D0h]
  float pOutZ; // [esp+128h] [ebp-3CCh] BYREF
  float z; // [esp+12Ch] [ebp-3C8h] BYREF
  float y; // [esp+130h] [ebp-3C4h] BYREF
  float fAggroSaved; // [esp+134h] [ebp-3C0h]
  float x; // [esp+138h] [ebp-3BCh] BYREF
  float fStepX0; // [esp+13Ch] [ebp-3B8h]
  float fStepZ0; // [esp+140h] [ebp-3B4h]
  POINT pnHeroCell; // [esp+144h] [ebp-3B0h]
  CHanaFrameT3EC_S354 *pFrame; // [esp+14Ch] [ebp-3A8h]
  int i; // [esp+150h] [ebp-3A4h]
  POINT ppOutPathPts[5]; // [esp+154h] [ebp-3A0h] BYREF
  float pOutY; // [esp+17Ch] [ebp-378h] BYREF
  float fDist; // [esp+180h] [ebp-374h]
  POINT pnFoeCell; // [esp+184h] [ebp-370h] BYREF
  float z2; // [esp+18Ch] [ebp-368h] BYREF
  float fHitImpact; // [esp+190h] [ebp-364h]
  float y2; // [esp+194h] [ebp-360h] BYREF
  int bFoeActDone; // [esp+198h] [ebp-35Ch]
  float x2; // [esp+19Ch] [ebp-358h] BYREF
  int nHitSfx; // [esp+1A0h] [ebp-354h]
  __int16 v112; // [esp+1A4h] [ebp-350h]
  D3DXVECTOR3 pV; // [esp+1A8h] [ebp-34Ch] BYREF
  float fFoeSaveZ; // [esp+1B4h] [ebp-340h]
  float pOutX; // [esp+1B8h] [ebp-33Ch] BYREF
  float fFoeSaveY; // [esp+1BCh] [ebp-338h]
  float arrFoeTuning[20][10]; // [esp+1C0h] [ebp-334h] BYREF
  float pz; // [esp+4E0h] [ebp-14h] BYREF
  float fFoeSaveX; // [esp+4E4h] [ebp-10h]
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+4E8h] [ebp-Ch]
  float py; // [esp+4ECh] [ebp-8h] BYREF
  int pnDepth; // [esp+4F0h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateFoeAI_446A38\n");

  pFrame = pFoeBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  v86 = 0;
  v112 = 0;
  v90 = ConvertFrom640AsInt(320);
  v89 = ConvertFrom640AsInt(240);
  fFoeSaveX = 0.0;
  fFoeSaveY = 0.0;
  fFoeSaveZ = 0.0;
  pV.x = 0.0;
  pV.y = 1.0;
  pV.z = 0.0;
  pnDepth = 5;
  fAggroSpeedMul = *pfFoeHpCur / fFoeHpMax / 8.0 + 0.97500002;
  fAggroSaved = *pfFoeHpCur;
  fHitImpact = 0.0;
  nMode = 0;
  bFoeActDone = 0;
  memset(arrFoeTuning, 0, sizeof(arrFoeTuning));   // rows 12..19 and cols [7..9] stay zero (original memsets the tail after packing QWORDs)
  // duel 0
  arrFoeTuning[0][0] = 0.8;    arrFoeTuning[0][1] = 1.0;
  arrFoeTuning[0][2] = 10.0;   arrFoeTuning[0][3] = -2.5;
  arrFoeTuning[0][4] = 5.0;    arrFoeTuning[0][5] = 1.0;
  arrFoeTuning[0][6] = 25.0;
  // duel 1
  arrFoeTuning[1][0] = 0.9;    arrFoeTuning[1][1] = 1.3;
  arrFoeTuning[1][2] = 6.0;    arrFoeTuning[1][3] = -2.5;
  arrFoeTuning[1][4] = 5.0;    arrFoeTuning[1][5] = 1.0;
  arrFoeTuning[1][6] = 30.0;
  // duel 2
  arrFoeTuning[2][0] = 1.0;    arrFoeTuning[2][1] = 6.0;
  arrFoeTuning[2][2] = 20.0;   arrFoeTuning[2][3] = -2.0;
  arrFoeTuning[2][4] = 6.0;    arrFoeTuning[2][5] = 1.3;
  arrFoeTuning[2][6] = 38.0;
  // duel 3
  arrFoeTuning[3][0] = 0.9;    arrFoeTuning[3][1] = 6.5;
  arrFoeTuning[3][2] = 15.0;   arrFoeTuning[3][3] = -2.0;
  arrFoeTuning[3][4] = 6.0;    arrFoeTuning[3][5] = 1.3;
  arrFoeTuning[3][6] = 38.0;
  // duel 4
  arrFoeTuning[4][0] = 0.8;    arrFoeTuning[4][1] = 2.5;
  arrFoeTuning[4][2] = 15.0;   arrFoeTuning[4][3] = -0.8;
  arrFoeTuning[4][4] = 8.0;    arrFoeTuning[4][5] = 2.3;
  arrFoeTuning[4][6] = 30.0;
  // duel 5
  arrFoeTuning[5][0] = 0.9;    arrFoeTuning[5][1] = 2.5;
  arrFoeTuning[5][2] = 15.0;   arrFoeTuning[5][3] = -0.8;
  arrFoeTuning[5][4] = 8.0;    arrFoeTuning[5][5] = 2.3;
  arrFoeTuning[5][6] = 30.0;
  // duel 6
  arrFoeTuning[6][0] = 1.0;    arrFoeTuning[6][1] = 4.0;
  arrFoeTuning[6][2] = 12.0;   arrFoeTuning[6][3] = -0.8;
  arrFoeTuning[6][4] = 8.0;    arrFoeTuning[6][5] = 1.8;
  arrFoeTuning[6][6] = 22.0;
  // duel 7
  arrFoeTuning[7][0] = 1.0;    arrFoeTuning[7][1] = 4.0;
  arrFoeTuning[7][2] = 12.0;   arrFoeTuning[7][3] = -0.8;
  arrFoeTuning[7][4] = 8.0;    arrFoeTuning[7][5] = 1.8;
  arrFoeTuning[7][6] = 25.0;
  // duel 8
  arrFoeTuning[8][0] = 1.2;    arrFoeTuning[8][1] = 4.5;
  arrFoeTuning[8][2] = 20.0;   arrFoeTuning[8][3] = -2.0;
  arrFoeTuning[8][4] = 6.0;    arrFoeTuning[8][5] = 1.8;
  arrFoeTuning[8][6] = 60.0;
  // duel 9
  arrFoeTuning[9][0] = 1.5;    arrFoeTuning[9][1] = 3.0;
  arrFoeTuning[9][2] = 15.0;   arrFoeTuning[9][3] = -2.5;
  arrFoeTuning[9][4] = 6.0;    arrFoeTuning[9][5] = 0.8;
  arrFoeTuning[9][6] = 54.0;
  // duel 10
  arrFoeTuning[10][0] = 1.2;   arrFoeTuning[10][1] = 3.5;
  arrFoeTuning[10][2] = 15.0;  arrFoeTuning[10][3] = -2.5;
  arrFoeTuning[10][4] = 6.0;   arrFoeTuning[10][5] = 1.0;
  arrFoeTuning[10][6] = 30.0;
  // duel 11
  arrFoeTuning[11][0] = 1.6;   arrFoeTuning[11][1] = 3.5;
  arrFoeTuning[11][2] = 6.0;   arrFoeTuning[11][3] = -2.5;
  arrFoeTuning[11][4] = 6.0;   arrFoeTuning[11][5] = 0.8;
  arrFoeTuning[11][6] = 41.0;
  g_fFoeAiTimer_52ED28 = g_fFoeAiTimer_52ED28 - fDelta;
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &x2, &y2, &z2);
  HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px, &py, &pz);
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &x, &y, &z);
  if ( g_fFoeCdTimer_52ED18 >= 0.0 )
    g_fFoeCdTimer_52ED18 = g_fFoeCdTimer_52ED18 - fDelta;
  else
    g_fFoeCdTimer_52ED18 = 0.0;
  pnHeroCell.x = (int)x / 5;
  pnHeroCell.y = (int)z / 5;
  HanaFrame_GetRelPosition(pFrame, (CHanaFrameRootT3EC_S2D4 *)pHeroFrame, &pOutX, &pOutY, &pOutZ);
  fDist = pOutX * pOutX + pOutZ * pOutZ;
  if ( (g_dwFoeAiState_52ED20 == 5 && pFoeBody->field_200 != 3)
    || (g_dwFoeAiState_52ED20 <= 3
      && pFoeBody->field_200 != 3
      && (arrFoeTuning[nDuelIndex][2] * arrFoeTuning[nDuelIndex][2] > fDist
        || pFoeBody->field_200 > 10
        || pFoeBody->field_200 == 6)
      && (g_dwBattleMode >= 0 || pFoeBody->field_200 > 10 || pFoeBody->field_200 == 6)) )
  {
    fFoeSaveX = px;
    fFoeSaveY = py;
    fFoeSaveZ = pz;
    fBlendFactor = fDelta * arrFoeTuning[nDuelIndex][5];
    if ( fBlendFactor > 1.0 )
      fBlendFactor = 1.0;
    HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &x, &y, &z);
    HanaFrame_GetRelPosition(pHeroFrame, (CHanaFrameRootT3EC_S2D4 *)pFrame, &px, &py, &pz);
    SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -2.0);
    CalculateLocalRotationMatrix(
      pFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      &pXpos,
      &pYpos,
      &pZpos,
      &pXYrot,
      &pYXrot,
      &pZXrot);
    HanaMatrix_BuildLookAtAndTransform(pFrame, pHeroFrame, g_pSceneFrame, 0);
    CalculateLocalRotationMatrix(
      pFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      &fLookX1,
      &v84,
      &fLookZ1,
      &pXYrot,
      &pYXrot,
      &pZXrot);
    fLookX1 = -fLookX1;
    fLookZ1 = -fLookZ1;
    dirForwardZ = (fLookZ1 - pZpos) * fBlendFactor * 0.30000001 + pZpos;
    dirForwardX = (fLookX1 - pXpos) * fBlendFactor * 0.30000001 + pXpos;
    UpdateBoneLookAtTransform(
      pFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      dirForwardX,
      0.0,
      dirForwardZ,
      0.0,
      1.0,
      0.0);
    SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, z);
    if ( pFoeBody->field_200 == 6 )
    {
      fBlendFactor = fDelta * arrFoeTuning[nDuelIndex][0];
      if ( nDuelIndex >= 4 && nDuelIndex <= 5 )
        fBlendFactor = fBlendFactor * -1.0;
      fZNudge = -fBlendFactor;
      SetFramePosRel_4069DD(pFrame, pFrame, 0.0, 0.0, fZNudge);
    }
    else if ( pFoeBody->field_200 != 19 )
    {
      if ( pFoeBody->field_200 == 20 )
      {
        if ( pHeroBody->field_200 != 13 )
        {
          HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px, &py, &pz);
          SetFramePosRel_4069DD(pFrame, pFrame, 0.0, 0.0, arrFoeTuning[nDuelIndex][3]);
          HanaFrame_GetRelPosition(pFrame, (CHanaFrameRootT3EC_S2D4 *)pHeroFrame, &pOutX, &pOutY, &pOutZ);
          SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, px, py, pz);
          fDist = sqrt(pOutX * pOutX + pOutZ * pOutZ);
          if ( fDist < (double)arrFoeTuning[nDuelIndex][4] && g_dwBattleMode >= 0 )
          {
            SekBody__UpdateTo_4014E4(pHeroBody, 9);
            *pfHeroHpCur = *pfHeroHpCur - 2000.0 * arrFoeTuning[nDuelIndex][1];
            g_dwBattleMode = -1;
            fHitImpact = 122.0;
          }
        }
      }
      else if ( pFoeBody->field_200 != 21 )
      {
        SekBody__UpdateTo_4014E4(pFoeBody, 6);
      }
    }
    fAnimRateA = fDelta + fDelta - fDelta * fAggroSpeedMul;
    if ( CSekBody__UpdateAnim_4019E1(pFoeBody, fAnimRateA) )
    {
      if ( pFoeBody->field_200 == 21 )
      {
        if ( g_dwFoeAiState_52ED20 == 5 )
          g_dwFoeAiState_52ED20 = 0;
        bFoeActDone = 1;
      }
      if ( pFoeBody->field_200 == 21 || pFoeBody->field_200 == 3 )
        SekBody__UpdateTo_4014E4(pFoeBody, 2);
    }
  }
  else
  {
    nMoveMode = 1;
    HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px, &py, &pz);
    HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &x, &y, &z);
    pnFoeCell.x = (int)px / 5;
    pnFoeCell.y = (int)pz / 5;
    pnHeroCell.x = (int)(x / 5.0);
    pnHeroCell.y = (int)(z / 5.0);
    x = x2;
    y = y2;
    z = z2;
    if ( g_bFoeCharging_52ED24 )
    {
      g_dwFoeAiState_52ED20 = 0;
      if ( g_fFoeAiTimer_52ED28 <= 0.0 )
        g_bFoeCharging_52ED24 = 0;
      nMode = 3;
    }
    else if ( g_fFoeAiTimer_52ED28 < 0.0 || g_bFoeCharging_52ED24 )
    {
      g_fFoeAiTimer_52ED28 = (float)(rand() % 250 + 50);
      g_dwFoeAiState_52ED20 = rand() % 3;
      nMode = 0;
    }
    if ( g_dwFoeAiState_52ED20 )
    {
      if ( nDuelIndex < 2
        || arrFoeTuning[nDuelIndex][6] * arrFoeTuning[nDuelIndex][6] <= (x - px) * (x - px) + (z - pz) * (z - pz)
        || fFoeHpMax / 3.0 >= *pfFoeHpCur
        || g_dwBattleMode < 0 )
      {
        nMode = 0;
      }
      else if ( nDuelIndex == 2 || nDuelIndex == 3 )
      {
        nMode = 1;
      }
      else if ( nDuelIndex == 4 || nDuelIndex == 5 )
      {
        nMode = 2;
      }
      else
      {
        nMode = g_dwFoeAiState_52ED20;
      }
    }
    if ( !pOccGrid[200 * ((int)pz / 5) + 20100 + (int)px / 5]
      && !pOccGrid[200 * ((int)pz / 5) + 20101 + (int)px / 5]
      && !pOccGrid[200 * ((int)pz / 5) + 20099 + (int)px / 5]
      && !pOccGrid[200 * ((int)pz / 5) + 20300 + (int)px / 5]
      && !pOccGrid[200 * ((int)pz / 5) + 19900 + (int)px / 5]
      && nMode != 2 )
    {
      nMode = 3;
    }
    if ( nMode >= 2 )
    {
      nMoveMode = 2;
      fFoeSaveX = px;
      fFoeSaveY = py;
      fFoeSaveZ = pz;
      fSpeedMul = fAggroSpeedMul * arrFoeTuning[nDuelIndex][0];
      fTurnRate2 = fAggroSpeedMul * arrFoeTuning[nDuelIndex][5] + fAggroSpeedMul * arrFoeTuning[nDuelIndex][5];
      if ( nMode == 2 )
        StepFoeTurn_457EE0(pFoeBody, pHeroBody, pMapFrame, fDelta, fTurnRate2, fSpeedMul);
      else
        StepFoeSpin_45804C(pFoeBody, pHeroBody, pMapFrame, fDelta, fTurnRate2, fSpeedMul);
      *pfFoeHpCur = *pfFoeHpCur - 8.0 * fDelta;
    }
    else
    {
      pFloodStep = HanaGrid_FloodFill_4460AD(
        &pfOutStep,
        pOccGrid,
        &pnFoeCell,
        pnHeroCell,
        &pnDepth,
        nMode,
        ppOutPathPts,
        &pnOutNumbPathPts);
      fStepX0 = pFloodStep->x;
      fStepZ0 = pFloodStep->y;
      fHeroDist = sqrt((x - px) * (x - px) + (z - pz) * (z - pz));
      if ( pFoeBody->field_200 == 3
        || fHeroDist >= (double)arrFoeTuning[nDuelIndex][6]
        && (pFoeBody->field_200 != 2 || arrFoeTuning[nDuelIndex][6] + 10.0 <= fHeroDist) )
      {
        if ( pFoeBody->field_200 == 3 )
        {
          fTimeDiv = fDelta / 3.0;
          nMoveMode = 3;
        }
        else
        {
          if ( fFoeHpMax / 5.0 <= *pfFoeHpCur )
            nMoveMode = 1;
          else
            nMoveMode = 4;
          fTimeDiv = (fDelta + fDelta) / 3.0;
          *pfFoeHpCur = *pfFoeHpCur - 5.0 * fDelta;
        }
      }
      else
      {
        nMoveMode = 2;
        if ( 500.0 * fDelta >= 1.0 && rand() % (int)(500.0 * fDelta) == 1 )
          nMoveMode = 3;
        fTimeDiv = fDelta;
        *pfFoeHpCur = *pfFoeHpCur - 8.0 * fDelta;
      }
      fStepSpeed = fTimeDiv * arrFoeTuning[nDuelIndex][0] * fAggroSpeedMul;
      if ( fStepSpeed > 4.0 )
        fStepSpeed = 4.0;
      fMoveBudget = fStepSpeed;
      if ( fStepSpeed < 0.0 )
        fMoveBudget = 0.0;
      fStepX = fStepX0;
      fStepZ = fStepZ0;
      fDist = sqrt(fStepX0 * fStepX0 + fStepZ0 * fStepZ0);
      if ( fDist != 0.0 )
      {
        if ( fDist < (double)fMoveBudget )
        {
          fMoveZ_a = fStepZ + pz;
          fMoveX_a = fStepX + px;
        }
        else
        {
          fMoveZ_a = fStepZ / fDist * fMoveBudget + pz;
          fMoveX_a = fStepX / fDist * fMoveBudget + px;
        }
        SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fMoveX_a, py, fMoveZ_a);
      }
      if ( fMoveBudget > (double)fDist )
      {
        for ( i = 0; i < (__int16)pnOutNumbPathPts; ++i )
        {
          fMoveBudget = fMoveBudget - fDist;
          if ( fMoveBudget <= 0.0 )
            break;
          HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &fWpProbeX, &fWpProbeY, &fWpProbeZ);
          fStepX = (double)(5 * ppOutPathPts[i].x) - fWpProbeX;
          fStepZ = (double)(5 * ppOutPathPts[i].y) - fWpProbeZ;
          fDist = sqrt(fStepX * fStepX + fStepZ * fStepZ);
          if ( fDist != 0.0 )
          {
            if ( fDist >= (double)fMoveBudget )
            {
              fMoveZ_b = fStepZ / fDist * fMoveBudget + fWpProbeZ;
              fMoveX_b = fStepX / fDist * fMoveBudget + fWpProbeX;
              SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fMoveX_b, fWpProbeY, fMoveZ_b);
              break;
            }
            fWpMoveZ = (float)(5 * ppOutPathPts[i].y);
            fWpMoveX = (float)(5 * ppOutPathPts[i].x);
            SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fWpMoveX, fWpProbeY, fWpMoveZ);
          }
        }
      }
      fFoeSaveX = px;
      fFoeSaveY = py;
      fFoeSaveZ = pz;
      HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px, &py, &pz);
      if ( sqrt(
        (fFoeSaveX - px) * (fFoeSaveX - px)
        + (fFoeSaveY - py) * (fFoeSaveY - py)
        + (fFoeSaveZ - pz) * (fFoeSaveZ - pz)) < 0.0099999998
        && g_dwFoeCellXCache_52E4F8 == pnFoeCell.x
        && g_dwFoeCellYCache_52E4FC == pnFoeCell.y )
      {
        fTurnBlend = fDelta / 4.0 * arrFoeTuning[nDuelIndex][5];
        if ( fTurnBlend > 1.0 )
          fTurnBlend = 1.0;
        CalculateLocalRotationMatrix(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          &fFwdBeforeX,
          &fFwdBeforeY,
          &fFwdBeforeZ,
          &v62,
          &v61,
          &v59);
        HanaMatrix_BuildLookAtAndTransform(pFrame, pHeroFrame, g_pSceneFrame, 0);
        CalculateLocalRotationMatrix(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          &fFwdAfterX,
          &fFwdAfterY,
          &fFwdAfterZ,
          &v62,
          &v61,
          &v59);
        fFwdAfterX = -fFwdAfterX;
        fFwdAfterZ = -fFwdAfterZ;
        dirForwardZa = (fFwdAfterZ - fFwdBeforeZ) * fTurnBlend * 0.30000001 + fFwdBeforeZ;
        dirForwardXa = (fFwdAfterX - fFwdBeforeX) * fTurnBlend * 0.30000001 + fFwdBeforeX;
        UpdateBoneLookAtTransform(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          dirForwardXa,
          0.0,
          dirForwardZa,
          0.0,
          1.0,
          0.0);
        nMoveMode = 5;
        *pfFoeHpCur = 8.1000004 * fDelta + fAggroSaved;
        if ( *pfFoeHpCur > (double)fFoeHpMax )
          *pfFoeHpCur = fFoeHpMax;
      }
      else
      {
        fTimeDiv = fDelta / 2.0 * arrFoeTuning[nDuelIndex][5];
        if ( fDelta > 1.0 )
          fTimeDiv = 1.0;
        CalculateLocalRotationMatrix(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          &xpos,
          &ypos,
          &zpos,
          &xrot,
          &yrot,
          &zrot);
        HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &fFoeX2, &fFoeY2, &fFoeZ2);
        fHeroPushZ = pz - (pz - fFoeSaveZ) * 10.0;
        fHeroPushX = px - (px - fFoeSaveX) * 10.0;
        SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroPushX, fFoeY2, fHeroPushZ);
        HanaMatrix_BuildLookAtAndTransform(pFrame, pHeroFrame, g_pSceneFrame, 0);
        CalculateLocalRotationMatrix(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          &fLookTgtX,
          &v53,
          &fLookTgtZ,
          &xrot,
          &yrot,
          &zrot);
        dirForwardZb = (fLookTgtZ - zpos) * fTimeDiv * 0.30000001 + zpos;
        dirForwardXb = (fLookTgtX - xpos) * fTimeDiv * 0.30000001 + xpos;
        UpdateBoneLookAtTransform(
          pFrame,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          dirForwardXb,
          0.0,
          dirForwardZb,
          0.0,
          1.0,
          0.0);
        CalculateLocalRotationMatrix(pFrame, pHeroFrame, &xpos, &ypos, &zpos, &xrot, &yrot, &zrot);
        if ( zpos >= 0.99000001 )
          Frame_RotateMatrixAxis_4070AB(pFrame, 1, &pV, 0.0034908999);
        SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, z);
      }
      g_dwFoeCellXCache_52E4F8 = pnFoeCell.x;
      g_dwFoeCellYCache_52E4FC = pnFoeCell.y;
      SekBody__UpdateTo2_401647(pFoeBody, nMoveMode);
    }
    fAnimRateB = fDelta * fAggroSpeedMul;
    if ( CSekBody__UpdateAnim_4019E1(pFoeBody, fAnimRateB) )
    {
      if ( pFoeBody->field_200 == 21 || pFoeBody->field_200 == 3 )
        nMoveMode = 1;
      SekBody__UpdateTo2_401647(pFoeBody, nMoveMode);
    }
  }
  if ( g_dwBattleMode >= 0 )
  {
    HanaFrame_GetRelPosition(pFrame, (CHanaFrameRootT3EC_S2D4 *)pHeroFrame, &pOutX, &pOutY, &pOutZ);
    fDist = sqrt(pOutX * pOutX + pOutZ * pOutZ);
    if ( fDist < 5.0 && fDist != 0.0 )
    {
      v41 = pOutY;
      fSepX = 5.0 / fDist * pOutX;
      fSepZ = 5.0 / fDist * pOutZ;
      SetFramePosRel_4069DD(pFrame, pHeroFrame, fSepX, pOutY, fSepZ);
      if ( bFoeActDone != 1 )
        g_dwFoeAiState_52ED20 = 5;
    }
  }
  px = fFoeSaveX;
  py = fFoeSaveY;
  pz = fFoeSaveZ;
  nHitSfx = 0;
  for ( i = 0; i < 3; ++i )
  {
    nHitSfx = CheckFrameCollision_443CC5(pFrame, pMapFrame, 0);
    if ( nHitSfx )
      break;
    HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px2, &py2, &pz2);
    if ( TestMoveCollision_4443FC(pFrame, pMapFrame, px, py2, pz, 0) )
      HanaFrame_GetRelPosition(pFrame, g_pSceneFrame, &px, &py2, &pz);
    else
      SetFramePosRel_4069DD(pFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, px, py2, pz);
  }
  if ( *pfFoeHpCur < 10.0 )
    *pfFoeHpCur = 10.0;
  if ( (unsigned int)--nHitSfx < 4 && g_pFoeHitSfxE_5244E4[nHitSfx] && g_pFoeHitSfxB_5244D4[nHitSfx] )
  {
    pAnimItem = &pFoeBody->items_250[pFoeBody->field_200];
    HanaFrame_GetCenterPosition_408C3E((CHanaFrameRootT3EC_S2D4 *)pFrame, g_pSceneFrame, &px, &py, &pz);
    g_pFoeHitSfxE_5244E4[nHitSfx]->pDS3DBuffer_4->SetPosition(px, py, pz, 1u);
    g_pFoeHitSfxB_5244D4[nHitSfx]->pDS3DBuffer_4->SetPosition(px, py, pz, 1u);
    if ( !g_pFoeHitSfxB_5244D4[nHitSfx]->pDSBuffer_0
      || g_pFoeHitSfxB_5244D4[nHitSfx]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pAnimItem->float_60 < 100.0 || pAnimItem->float_60 > 108.0)
        && (pAnimItem->float_60 < 200.0 || pAnimItem->float_60 > 206.0)
        && (pAnimItem->float_60 < 400.0 || pAnimItem->float_60 > 406.0) )
      {
        g_pFoeHitSfxB_5244D4[nHitSfx]->pDSBuffer_0->Stop();
        g_pFoeHitSfxB_5244D4[nHitSfx]->pDSBuffer_0->SetCurrentPosition(0);
      }
      else
      {
        g_pFoeHitSfxB_5244D4[nHitSfx]->pDSBuffer_0->Play(0, 0, 0);
      }
    }
    if ( !g_pFoeHitSfxE_5244E4[nHitSfx]->pDSBuffer_0
      || g_pFoeHitSfxE_5244E4[nHitSfx]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pAnimItem->float_60 < 107.0 || pAnimItem->float_60 > 115.0)
        && (pAnimItem->float_60 < 205.0 || pAnimItem->float_60 > 212.0)
        && (pAnimItem->float_60 < 407.0 || pAnimItem->float_60 > 415.0) )
      {
        g_pFoeHitSfxE_5244E4[nHitSfx]->pDSBuffer_0->Stop();
        g_pFoeHitSfxE_5244E4[nHitSfx]->pDSBuffer_0->SetCurrentPosition(0);
      }
      else
      {
        g_pFoeHitSfxE_5244E4[nHitSfx]->pDSBuffer_0->Play(0, 0, 0);
      }
    }
    for ( i = 0; i < 4; ++i )
    {
      if ( nHitSfx != i )
      {
        if ( g_pFoeHitSfxB_5244D4[i] )
        {
          g_pFoeHitSfxB_5244D4[i]->pDSBuffer_0->Stop();
          g_pFoeHitSfxB_5244D4[i]->pDSBuffer_0->SetCurrentPosition(0);
        }
        if ( g_pFoeHitSfxE_5244E4[i] )
        {
          g_pFoeHitSfxE_5244E4[i]->pDSBuffer_0->Stop();
          g_pFoeHitSfxE_5244E4[i]->pDSBuffer_0->SetCurrentPosition(0);
        }
      }
    }
  }
  SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, x2, y2, z2);
  return fHitImpact;
}

double __cdecl hook_WanaAttack_Close_4535DE(
  CSekBody *pWanaBody,
  CSekBody *pHeroBody,
  float *pfHeroHpCur,
  float fDelta,
  float fAmount,
  CHanaFrameT3EC_S354 *pMapFrame)
{
  float fRelX; // [esp+8h] [ebp-14h] BYREF
  CHanaFrameT3EC_S354 *pFrame; // [esp+Ch] [ebp-10h]
  float fRelZ; // [esp+10h] [ebp-Ch] BYREF
  CHanaFrameRootT3EC_S2D4 *pHeroFrame; // [esp+14h] [ebp-8h]
  float fRelY; // [esp+18h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] WanaAttack_Close_4535DE\n");

  pFrame = pWanaBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  if ( pWanaBody->field_24C == 1 )
    return 0.0;
  if ( g_dwBattleMode == 2
    && (HanaFrame_GetRelPosition(pFrame, pHeroFrame, &fRelX, &fRelY, &fRelZ),
      sqrt(fRelX * fRelX + fRelY * fRelY + fRelZ * fRelZ) < 5.0) )
  {
    g_dwBattleMode = -1;
    SekBody__UpdateTo2_401647(pWanaBody, 1);
    SekBody__UpdateTo2_401647(pHeroBody, 8);
    *pfHeroHpCur = *pfHeroHpCur - fAmount;
    if ( *pfHeroHpCur <= 0.0 )
      *pfHeroHpCur = 0.0;
    return 130.0;
  }
  else
  {
    if ( CSekBody__UpdateAnim_4019E1(pWanaBody, fDelta) == 1 )
      SekBody__UpdateTo2_401647(pWanaBody, 0);
    return 0.0;
  }
}

bool __cdecl hook_IsFoeWithinHeroRange_448B78(CSekBody *pHeroBody, CSekBody *pFoeBody)
{
  float fAfterHeroX; // [esp+8h] [ebp-2Ch] BYREF
  float fFoeZ; // [esp+Ch] [ebp-28h] BYREF
  float fFoeY; // [esp+10h] [ebp-24h] BYREF
  float fFoeX; // [esp+14h] [ebp-20h] BYREF
  float fHeroZ; // [esp+18h] [ebp-1Ch] BYREF
  CHanaFrameT3EC_S354 *pFoeFrame; // [esp+1Ch] [ebp-18h]
  float fHeroY; // [esp+20h] [ebp-14h] BYREF
  float fHeroX; // [esp+24h] [ebp-10h] BYREF
  float fAfterHeroZ; // [esp+28h] [ebp-Ch] BYREF
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+2Ch] [ebp-8h]
  float fAfterHeroY; // [esp+30h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] IsFoeWithinHeroRange_448B78\n");


  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  pFoeFrame = pFoeBody->pSekHeader_220->pFrameT3EC;
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
  SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -2.5);
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fAfterHeroX, &fAfterHeroY, &fAfterHeroZ);
  HanaFrame_GetRelPosition(pFoeFrame, g_pSceneFrame, &fFoeX, &fFoeY, &fFoeZ);
  SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  return sqrt((fAfterHeroX - fFoeX) * (fAfterHeroX - fFoeX) + (fAfterHeroZ - fFoeZ) * (fAfterHeroZ - fFoeZ)) < 5.0;
}

//  It's one of the four trap ("wana") attack behaviors dispatched from PlayHunting_44C38D (index 1). It's a two-phase
//   state machine keyed off g_dwBattleMode:
// 
//   Phase 1 -- trigger check (idle, g_dwBattleMode != -1):
//   - Bails out if the wana is disabled (pWanaBody->field_24C == 1).
//   - Skips the hit if the hero is invulnerable: its anim set's field_130 is in the 1600-1645 family, or its current anim
//   item (field_200, confirmed via CutToAnim_4018E1 which assigns it) is 13 -- or simply out of range (fDist >= 5.0 or ==
//   0).
//   - On hit: sets g_dwBattleMode = -1 (locks out other attacks), wana -> anim item 1 (attack), hero -> anim item 7
//   (knockback reaction -- Close uses 8 instead), subtracts fAmount from hero HP (clamped at 0), returns 33.0 -- a non-zero
//   return both triggers the hit SFX and adds to the caller's fActTimer.
// 
//   Phase 2 -- knockback push (attacking, g_dwBattleMode == -1): while hero is within 6 units of the wana, it gets shoved
//   to exactly 6 units away (rel vector scaled in XZ, Y kept). Then the interesting part -- a ground-settle loop (iTry <
//   3): if the shove left the hero not touching the support surface (CheckFrameCollision type 0), the code tries to walk
//   it back toward its pre-shove position:
// 
//   - TestMoveCollision(hero, map, anchor, 0) casts the segment current -> anchor; if a wall edge blocks it,
//   TestBodyMoveHit_443EC4 snaps the hero to the wall contact point as a side effect -- then the anchor is re-armed to that
//   snapped spot (fAnchorX/fCurY/fAnchorZ := current).
//   - If unblocked, the hero is placed at (fAnchorX, fCurY, fAnchorZ) -- pulled back to the old XZ but keeping its current
//   height.
//   - Loop exits early once the hero is back on the ground.
// 
//   When the attack is over/out of range (LABEL_23), the wana's anim is advanced and reset to item 0 when finished,
//   returning 0.
double __cdecl hook_WanaAttack_Knockback_4536F6(
  CSekBody *pWanaBody,
  CSekBody *pHeroBody,
  float *pfHeroHpCur,
  float fDelta,
  float fAmount,
  CHanaFrameT3EC_S354 *pMapFrame)
{
  float fStartY; // [esp+8h] [ebp-34h] BYREF
  float fAnchorX; // [esp+Ch] [ebp-30h] BYREF
  float fCurY; // [esp+10h] [ebp-2Ch] BYREF
  float fCurX; // [esp+14h] [ebp-28h] BYREF
  float fCurZ; // [esp+18h] [ebp-24h] BYREF
  float fAnchorZ; // [esp+1Ch] [ebp-20h] BYREF
  float fRelX; // [esp+20h] [ebp-1Ch] BYREF
  CHanaFrameT3EC_S354 *pWanaFrame; // [esp+24h] [ebp-18h]
  int iTry; // [esp+28h] [ebp-14h]
  float fDist; // [esp+2Ch] [ebp-10h]
  float fRelZ; // [esp+30h] [ebp-Ch] BYREF
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+34h] [ebp-8h]
  float fRelY; // [esp+38h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] WanaAttack_Knockback_4536F6\n");

  pWanaFrame = pWanaBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  if ( pWanaBody->field_24C == 1 )
    return 0.0;
  HanaFrame_GetRelPosition(pHeroFrame, pWanaFrame, &fRelX, &fRelY, &fRelZ);
  fDist = sqrt(fRelX * fRelX + fRelY * fRelY + fRelZ * fRelZ);
  if ( g_dwBattleMode == -1 )
  {
    if ( fDist <= 6.0 && fDist != 0.0 )
    {
      HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fAnchorX, &fStartY, &fAnchorZ);
      fRelX = 6.0 / fDist * fRelX;
      fRelZ = 6.0 / fDist * fRelZ;
      SetFramePosRel_4069DD(pHeroFrame, pWanaFrame, fRelX, fRelY, fRelZ);
      for ( iTry = 0; iTry < 3 && !CheckFrameCollision_443CC5(pHeroFrame, pMapFrame, 0); ++iTry )
      {
        HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fCurX, &fCurY, &fCurZ);
        if ( TestMoveCollision_4443FC(pHeroFrame, pMapFrame, fAnchorX, fStartY, fAnchorZ, 0) )
          HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fAnchorX, &fCurY, &fAnchorZ);
        else
          SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fAnchorX, fCurY, fAnchorZ);
      }
    }
    goto LABEL_23;
  }
  if ( pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 >= 1600.0
    && pHeroBody->pSekHeader_220->pAnimSetT3F1->field_130 <= 1645.0
    || pHeroBody->field_200 == 13
    || fDist >= 5.0
    || fDist == 0.0 )
  {
  LABEL_23:
    if ( CSekBody__UpdateAnim_4019E1(pWanaBody, fDelta) )
    {
      SekBody__UpdateTo_4014E4(pWanaBody, 0);
      CSekBody__UpdateAnim_4019E1(pWanaBody, 0.0);
    }
    return 0.0;
  }
  g_dwBattleMode = -1;
  SekBody__UpdateTo2_401647(pWanaBody, 1);
  SekBody__UpdateTo2_401647(pHeroBody, 7);
  *pfHeroHpCur = *pfHeroHpCur - fAmount;
  if ( *pfHeroHpCur <= 0.0 )
    *pfHeroHpCur = 0.0;
  return 33.0;
}

// Wana charged attack: accumulates charge in the wana frame's CHanaBase flt_110 (seconds). Hero within 10u starts charging while wana idle 
// (anim item 0); once started it keeps charging regardless of distance. At >5s wana switches to charge-up anim (item 1).
// At >20s the charge is RESET (before the range test - a missed discharge wastes the charge) and if the hero is within 5u in the XZ plane only
// (Y ignored) it discharges: battle mode -1, hero -> anim 12 (shock), wana -> anim 2 (discharge), HP -= fAmount,
// returns 26.0 (hit: act-timer contribution + hit SFX in PlayHunting). pMapFrame is UNUSED here (no knockback/collision).
// 
// Note: rel position is wana-relative-to-hero here (opposite of Knockback). Also: hero anim 13 blocks charging but NOT the discharge; 
// no 1600-1645 anim-set invulnerability check unlike Knockback.
// 
// Pending renaming field struct from CHanaBase_S130, generic scratch slots (nIndex_108, bOk_10C, iAlpha1000_120),
// flt_110 -> fChargeTime.

double __cdecl hook_WanaAttack_Charged_4539D1(
  CSekBody *pWanaBody,
  CSekBody *pHeroBody,
  float *pfHeroHpCur,
  float fDelta,
  float fAmount,
  CHanaFrameT3EC_S354 *pMapFrame)
{
  float fRelX; // [esp+8h] [ebp-14h] BYREF
  CHanaFrameT3EC_S354 *pWanaFrame; // [esp+Ch] [ebp-10h]
  float fRelZ; // [esp+10h] [ebp-Ch] BYREF
  CHanaFrameRootT3EC_S2D4 *pHeroFrame; // [esp+14h] [ebp-8h]
  float fRelY; // [esp+18h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] WanaAttack_Charged_4539D1\n");

  pWanaFrame = pWanaBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  if ( pWanaBody->field_24C == 1 )
    return 0.0;
  if ( g_dwBattleMode == -1 )
    goto LABEL_16;
  HanaFrame_GetRelPosition(pWanaFrame, pHeroFrame, &fRelX, &fRelY, &fRelZ);
  if ( pHeroBody->field_200 != 13
    && (sqrt(fRelX * fRelX + fRelY * fRelY + fRelZ * fRelZ) < 10.0 && !pWanaBody->field_200 || pWanaFrame->flt_110 != 0.0) )
  {
    if ( pWanaFrame->flt_110 > 5.0 )
      SekBody__UpdateTo2_401647(pWanaBody, 1);
    pWanaFrame->flt_110 = fDelta + pWanaFrame->flt_110;
  }
  if ( pWanaFrame->flt_110 <= 20.0 || (pWanaFrame->flt_110 = 0.0, sqrt(fRelX * fRelX + fRelZ * fRelZ) >= 5.0) )
  {
  LABEL_16:
    CSekBody__UpdateAnim_4019E1(pWanaBody, fDelta);
    return 0.0;
  }
  else
  {
    g_dwBattleMode = -1;
    SekBody__UpdateTo2_401647(pHeroBody, 12);
    SekBody__UpdateTo2_401647(pWanaBody, 2);
    *pfHeroHpCur = *pfHeroHpCur - fAmount;
    if ( *pfHeroHpCur <= 0.0 )
      *pfHeroHpCur = 0.0;
    return 26.0;
  }
}

double __cdecl hook_WanaAttack_Projectile_453B92(
  CSekBody *pWanaBody,
  CSekBody *pHeroBody,
  float *pfHeroHpCur,
  float fDelta,
  float fAmount,
  CHanaFrameT3EC_S354 *pMapFrame)
{
  float v7; // [esp+0h] [ebp-54h]
  float X_4; // [esp+8h] [ebp-4Ch]
  float x; // [esp+Ch] [ebp-48h] BYREF
  float heroRelY; // [esp+10h] [ebp-44h] BYREF
  float heroRelX; // [esp+14h] [ebp-40h] BYREF
  float wanaRelY; // [esp+18h] [ebp-3Ch] BYREF
  float wanaRelX; // [esp+1Ch] [ebp-38h] BYREF
  CHanaFrameT3EC_S354 *pWanaFrame; // [esp+20h] [ebp-34h]
  int v15; // [esp+24h] [ebp-30h]
  float fDist; // [esp+28h] [ebp-2Ch]
  int bProjMode; // [esp+2Ch] [ebp-28h]
  float fHitValue; // [esp+30h] [ebp-24h]
  float fProjSpeed; // [esp+34h] [ebp-20h]
  float wanaRelZ; // [esp+38h] [ebp-1Ch] BYREF
  int v21; // [esp+3Ch] [ebp-18h]
  float z; // [esp+40h] [ebp-14h] BYREF
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+44h] [ebp-10h]
  float y; // [esp+48h] [ebp-Ch] BYREF
  float heroRelZ; // [esp+4Ch] [ebp-8h] BYREF
  int fProjHit; // [esp+50h] [ebp-4h]

  ONCELOG("[HNH:SUB] WanaAttack_Projectile_453B92\n");

  pWanaFrame = pWanaBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  v21 = 0;
  v15 = 0;
  fHitValue = 0.0;
  fProjSpeed = 1.2;
  if ( pWanaBody->field_24C == 1 )
    return 0.0;
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &heroRelX, &heroRelY, &heroRelZ);
  HanaFrame_GetRelPosition(pWanaFrame, g_pSceneFrame, &x, &y, &z);
  AttachChild_4055FD(g_pSceneFrame, pWanaFrame);
  SetFramePosRel_4069DD(pWanaFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, z);
  if ( fAmount <= 5000.0 || fAmount > 7000.0 )
  {
    bProjMode = 0;
  }
  else
  {
    bProjMode = 1;
    fProjSpeed = 1.5;
  }
  if ( fAmount <= 5000.0 )
    fProjSpeed = 0.80000001;
  fProjHit = StepWanaProjectile_4581B0(
    pWanaBody,
    pHeroBody,
    pMapFrame,
    fDelta,
    5.0,
    fProjSpeed,
    pWanaFrame->bOk_10C,
    bProjMode);
  if ( fAmount > 5000.0 && fAmount <= 7000.0 )
  {
    HanaFrame_GetRelPosition(pWanaFrame, g_pSceneFrame, &wanaRelX, &wanaRelY, &wanaRelZ);
    SetFramePosRel_4069DD(pWanaFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, wanaRelX, heroRelY, wanaRelZ);
  }
  HanaFrame_GetRelPosition(pHeroFrame, pWanaFrame, &x, &y, &z);
  fDist = sqrt(x * x + z * z);
  if ( g_dwBattleMode != -1 )
  {
    pWanaFrame->bOk_10C = 0;
    if ( fProjHit )
    {
      if ( pWanaBody->field_200 == 3 )
        SekBody__UpdateTo2_401647(pWanaBody, 2);
    }
    else if ( fDist < 40.0 && sqrt(x * x) < 2.0 && z < -10.0 && pWanaBody->field_200 == 1 )
    {
      CSekBody__CutToAnim_4018E1(pWanaBody, 3);
    }
    if ( pHeroBody->field_200 != 13 && fDist < 4.0 )
    {
      if ( pWanaBody->field_200 == 3 )
      {
        pWanaFrame->bOk_10C = 1;
        g_dwBattleMode = -1;
        SekBody__UpdateTo2_401647(pHeroBody, 9);
        *pfHeroHpCur = *pfHeroHpCur - fAmount;
        if ( *pfHeroHpCur <= 0.0 )
          *pfHeroHpCur = 0.0;
        fHitValue = 123.0;
      }
      else if ( fAmount > 7000.0 )
      {
        pWanaFrame->bOk_10C = 1;
        g_dwBattleMode = -1;
        SekBody__UpdateTo2_401647(pHeroBody, 7);
        *pfHeroHpCur = *pfHeroHpCur - fAmount / 10.0;
        if ( *pfHeroHpCur <= 0.0 )
          *pfHeroHpCur = 0.0;
        fHitValue = 20.0;
      }
    }
  }
  if ( fDist < 4.0 && fDist != 0.0 && fAmount > 7000.0 )
  {
    X_4 = z * 4.0 / fDist;
    v7 = x * 4.0 / fDist;
    SetFramePosRel_4069DD(pHeroFrame, pWanaFrame, v7, y, X_4);
    if ( !CheckFrameCollision_443CC5(pHeroFrame, pMapFrame, 0) )
      SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, heroRelX, heroRelY, heroRelZ);
  }
  CSekBody__UpdateAnim_4019E1(pWanaBody, fDelta);
  return fHitValue;
}

// DrawFadeOutSpark(pSprites, fDecay)
// pSprites[1] (at pSprites+4) is a spark sprite. Each call decays its alpha by fDecay/10;
// while alpha>0 it draws 3 copies at x = 320 + (1-alpha)*{off0,off1,off2} (v6), y = 350,
// for a horizontally-dissipating spark effect.
void __cdecl hook_DrawFadeOutSpark_45669F(CHanaSpriteT3F6_S1FC **pSprites, float fDecay)
{
  float fCalcX; // [esp+28h] [ebp-1Ch]
  float fPosX; // [esp+28h] [ebp-1Ch]
  float fPosY; // [esp+2Ch] [ebp-18h]
  int i; // [esp+30h] [ebp-14h]
  float arrfXwaves[4]; // [esp+34h] [ebp-10h]

  ONCELOG("[HNH:SUB] DrawFadeOutSpark_45669F\n");

  arrfXwaves[3] = 0.0;
  arrfXwaves[0] = -128.0;
  arrfXwaves[1] = 0.0;
  arrfXwaves[2] = 128.0;
  if ( pSprites[1] )
  {
    if ( pSprites[1]->fAlpha_144 > 0.0 )
    {
      pSprites[1]->fAlpha_144 = pSprites[1]->fAlpha_144 - fDecay / 10.0;
      if ( pSprites[1]->fAlpha_144 > 0.0 )
      {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
        if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
        {
          for ( i = 0; i < 3; ++i )
          {
            fPosY = ConvertFrom640AsFloat(350.0);
            fCalcX = (1.0 - pSprites[1]->fAlpha_144) * arrfXwaves[i] + 320.0;
            fPosX = ConvertFrom640AsFloat(fCalcX);
            CHanaSpriteT3F6__SetPos_414A5D(pSprites[1], fPosX, fPosY);
            DrawSprite2D(pSprites[1]);
          }
          g_pD3DDevice_4D6C78->EndScene();
        }
      }
    }
  }
}

// Step the projectile wana one frame (the wana's own frame IS the projectile; local -Z is its forward axis). Called only from 
// WanaAttack_Projectile_453B92 (spinRate=5; speed/variant picked there from fAmount;
// bAlreadyLaunched = frame's CHanaBase bOk_10C latch, set on hero impact). Returns TRUE = projectile went AIRBORNE
// (its step left it not intersecting the map) - caller then cuts flying anim 3 -> 2. Tracking radius: mode 0 = 100u,
// mode 1 (hover variant) = ~180u; mode 1 also uses overhead-surface tests (bTestType=1) and keeps the ORIGINAL anchor Y
// in the settle loop. Aim phase (hero near, not launched): probe 1.5x step along -Z (unless anim 4);
// steer yaw toward hero side (fForwardSign from hero local X; unless anim 3). Probe still on map -> keep it, anim 1 (rolling).
// Probe airborne -> back off (+1x, net 0.5x), anim 2, then 2-try settle loop pulling the projectile back toward its frame-entry
// anchor until it touches the map again (same wall-slide pattern as WanaAttack_Knockback: TestMoveCollision snaps to contac
BOOL __cdecl hook_StepWanaProjectile_4581B0(
  CSekBody *pWanaBody,
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fDelta,
  float fSpinRate,
  float fProjSpeed,
  int bAlreadyLaunched,
  int bProjMode)
{
  float fProbeZ; // [esp+0h] [ebp-58h]
  float fSteerRad; // [esp+0h] [ebp-58h]
  float fBackoffZ; // [esp+0h] [ebp-58h]
  float fIdleProbeZ; // [esp+0h] [ebp-58h]
  float fIdleProbe2Z; // [esp+0h] [ebp-58h]
  float fIdleRevertZ; // [esp+0h] [ebp-58h]
  float fIdleSpinRad; // [esp+0h] [ebp-58h]
  float fFlyStepZ; // [esp+0h] [ebp-58h]
  float fFlyRevertZ; // [esp+0h] [ebp-58h]
  float fFlySpinRad; // [esp+0h] [ebp-58h]
  float fHeroRelX; // [esp+8h] [ebp-50h] BYREF
  float fAnchorY; // [esp+Ch] [ebp-4Ch] BYREF
  float fAnchorX; // [esp+10h] [ebp-48h] BYREF
  float fCurY; // [esp+14h] [ebp-44h] BYREF
  float fCurX; // [esp+18h] [ebp-40h] BYREF
  CHanaFrameT3EC_S354 *pProjFrame; // [esp+1Ch] [ebp-3Ch]
  int iTry; // [esp+20h] [ebp-38h]
  BOOL bAirborne; // [esp+24h] [ebp-34h]
  float fHeroSideZ; // [esp+28h] [ebp-30h] BYREF
  D3DXVECTOR3 vecSpinAxis; // [esp+2Ch] [ebp-2Ch] BYREF
  float fHeroSideY; // [esp+38h] [ebp-20h] BYREF
  float fCurZ; // [esp+3Ch] [ebp-1Ch] BYREF
  float fHeroSideX; // [esp+40h] [ebp-18h] BYREF
  float fForwardSign; // [esp+44h] [ebp-14h]
  float fHeroRelZ; // [esp+48h] [ebp-10h] BYREF
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+4Ch] [ebp-Ch]
  float fHeroRelY; // [esp+50h] [ebp-8h] BYREF
  float fAnchorZ; // [esp+54h] [ebp-4h] BYREF
  float fSpeed; // [esp+74h] [ebp+1Ch]

  ONCELOG("[HNH:SUB] StepWanaProjectile_4581B0\n");

  pProjFrame = pWanaBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  bAirborne = 0;
  vecSpinAxis.x = 0.0;
  vecSpinAxis.y = 1.0;
  vecSpinAxis.z = 0.0;
  HanaFrame_GetRelPosition(pProjFrame, g_pSceneFrame, &fAnchorX, &fAnchorY, &fAnchorZ);
  fSpeed = fProjSpeed * 0.80000001;
  HanaFrame_GetRelPosition(pHeroFrame, pProjFrame, &fHeroSideX, &fHeroSideY, &fHeroSideZ);
  if ( fHeroSideX <= 2.0 )
  {
    if ( fHeroSideX >= 2.0 )
      fForwardSign = 0.0;
    else
      fForwardSign = 1.0;
  }
  else
  {
    fForwardSign = -1.0;
  }
  HanaFrame_GetRelPosition(pHeroFrame, pProjFrame, &fHeroRelX, &fHeroRelY, &fHeroRelZ);
  if ( (double)(22500 * bProjMode + 10000) <= fHeroRelX * fHeroRelX + fHeroRelY * fHeroRelY + fHeroRelZ * fHeroRelZ
    || bAlreadyLaunched == 1 )
  {
    if ( bAlreadyLaunched == 1 )
    {
      SekBody__UpdateTo2_401647(pWanaBody, 1);
      fFlyStepZ = -fDelta * fSpeed;
      SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fFlyStepZ);
      if ( !CheckFrameCollision_443CC5(pProjFrame, pMapFrame, bProjMode) )
      {
        bAirborne = 1;
        fFlyRevertZ = fDelta * fSpeed;
        SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fFlyRevertZ);
        fFlySpinRad = -fDelta * 3.1418099 / 180.0 * fSpinRate;
        Frame_RotateMatrixAxis_4070AB(pProjFrame, 1, &vecSpinAxis, fFlySpinRad);
      }
    }
    else
    {
      SekBody__UpdateTo2_401647(pWanaBody, 2);
      fIdleProbeZ = -fDelta * fSpeed / 2.0;
      SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fIdleProbeZ);
      if ( !CheckFrameCollision_443CC5(pProjFrame, pMapFrame, bProjMode) )
      {
        fIdleProbe2Z = -fDelta * fSpeed / 2.0;
        SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fIdleProbe2Z);
        if ( !CheckFrameCollision_443CC5(pProjFrame, pMapFrame, bProjMode) )
        {
          bAirborne = 1;
          fIdleRevertZ = fDelta * fSpeed;
          SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fIdleRevertZ);
          fIdleSpinRad = fDelta * 3.1418099 / 180.0 * fSpinRate;
          Frame_RotateMatrixAxis_4070AB(pProjFrame, 1, &vecSpinAxis, fIdleSpinRad);
        }
      }
    }
  }
  else
  {
    if ( pWanaBody->field_200 != 4 )
    {
      fProbeZ = -fDelta * fSpeed * 1.5;
      SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fProbeZ);
    }
    if ( pWanaBody->field_200 != 3 )
    {
      fSteerRad = fDelta * 3.1418099 / 180.0 * fForwardSign * fSpinRate;
      Frame_RotateMatrixAxis_4070AB(pProjFrame, 1, &vecSpinAxis, fSteerRad);
    }
    if ( CheckFrameCollision_443CC5(pProjFrame, pMapFrame, 0) )
    {
      SekBody__UpdateTo2_401647(pWanaBody, 1);
    }
    else
    {
      bAirborne = 1;
      SekBody__UpdateTo2_401647(pWanaBody, 2);
      fBackoffZ = (fDelta * fSpeed * 1.5 + fDelta * fSpeed * 1.5) / 3.0;
      SetFramePosRel_4069DD(pProjFrame, pProjFrame, 0.0, 0.0, fBackoffZ);
      if ( bProjMode )
      {
        for ( iTry = 0; iTry < 2 && !CheckFrameCollision_443CC5(pProjFrame, pMapFrame, 1); ++iTry )
        {
          HanaFrame_GetRelPosition(pProjFrame, g_pSceneFrame, &fCurX, &fCurY, &fCurZ);
          if ( TestMoveCollision_4443FC(pProjFrame, pMapFrame, fAnchorX, fAnchorY, fAnchorZ, 1) )
            HanaFrame_GetRelPosition(pProjFrame, g_pSceneFrame, &fAnchorX, &fCurY, &fAnchorZ);
          else
            SetFramePosRel_4069DD(pProjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fAnchorX, fAnchorY, fAnchorZ);
        }
      }
      else
      {
        for ( iTry = 0; iTry < 2 && !CheckFrameCollision_443CC5(pProjFrame, pMapFrame, 0); ++iTry )
        {
          HanaFrame_GetRelPosition(pProjFrame, g_pSceneFrame, &fCurX, &fCurY, &fCurZ);
          if ( TestMoveCollision_4443FC(pProjFrame, pMapFrame, fAnchorX, fCurY, fAnchorZ, 0) )
            HanaFrame_GetRelPosition(pProjFrame, g_pSceneFrame, &fAnchorX, &fCurY, &fAnchorZ);
          else
            SetFramePosRel_4069DD(pProjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fAnchorX, fCurY, fAnchorZ);
        }
      }
    }
  }
  return bAirborne;
}

void __cdecl hook_BindSceneActors_448C72(
  CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 **pActorFrames,
  unsigned int nCount)
{
  float fRelZ; // [esp+4h] [ebp-248h] BYREF
  //size_t nNameLen[10]; // [esp+8h] [ebp-244h]
  float fRelY; // [esp+30h] [ebp-21Ch] BYREF
  float fRelX; // [esp+34h] [ebp-218h] BYREF
  unsigned int j; // [esp+38h] [ebp-214h]
  unsigned int i; // [esp+3Ch] [ebp-210h]
  float fDirUpZ; // [esp+40h] [ebp-20Ch] BYREF
  //String 50 szActorNames[9]; // [esp+44h] [ebp-208h] BYREF
  //String 50 szWana8; // [esp+206h] [ebp-46h] BYREF
  float dirForwardZ; // [esp+238h] [ebp-14h] BYREF
  float fDirUpY; // [esp+23Ch] [ebp-10h] BYREF
  float fDirForwardY; // [esp+240h] [ebp-Ch] BYREF
  float fDirUpX; // [esp+244h] [ebp-8h] BYREF
  float dirForwardX; // [esp+248h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] BindSceneActors_448C72\n");

#if 1
  char szActorNames[10][50] = {
    "otoko",
    "onna",
    "wana1",
    "wana2",
    "wana3",
    "wana4",
    "wana5",
    "wana6",
    "wana7",
    "wana8"
  };
  size_t nNameLen[10] = {
    5,
    4,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
  };
#else
  strcpy((char *)szActorNames[0].str, "otoko");
  memset((void *)&szActorNames[0].str[6], 0, 44u);
  strcpy((char *)szActorNames[1].str, "onna");
  memset((void *)&szActorNames[1].str[5], 0, 0x2Cu);
  szActorNames[1].str[49] = 0;
  strcpy((char *)szActorNames[2].str, "wana1");
  memset((void *)&szActorNames[2].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[3].str, "wana2");
  memset((void *)&szActorNames[3].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[4].str, "wana3");
  memset((void *)&szActorNames[4].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[5].str, "wana4");
  memset((void *)&szActorNames[5].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[6].str, "wana5");
  memset((void *)&szActorNames[6].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[7].str, "wana6");
  memset((void *)&szActorNames[7].str[6], 0, 0x2Cu);
  strcpy((char *)szActorNames[8].str, "wana7");
  memset((void *)&szActorNames[8].str[6], 0, 0x2Cu);
  strcpy((char *)szWana8.str, "wana8");
  memset((void *)&szWana8.str[6], 0, 0x2Cu);
  nNameLen[0] = 5;
  nNameLen[1] = 4;
  nNameLen[2] = 5;
  nNameLen[3] = 5;
  nNameLen[4] = 5;
  nNameLen[5] = 5;
  nNameLen[6] = 5;
  nNameLen[7] = 5;
  nNameLen[8] = 5;
  nNameLen[9] = 5;
#endif

  if ( pScene )
  {
    for ( i = 0; i < nCount; ++i )
    {
      for ( j = 0; j < pScene->dwCount_130; ++j )
      {
        if ( pScene->refsFrames_134[j]
          && !_strnicmp(pScene->refsFrames_134[j]->szName_8, szActorNames[i], nNameLen[i]) )
        {
          if ( i >= 2 )
          {
            AttachChild_4055FD(pScene->refsFrames_134[j], pActorFrames[i]);
            SetFramePosRel_4069DD(pActorFrames[i], pScene->refsFrames_134[j], 0.0, 0.0, 0.0);
            UpdateBoneLookAtTransform(pActorFrames[i], pScene->refsFrames_134[j], 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
          }
          else
          {
            HanaFrame_GetRelPosition(pScene->refsFrames_134[j], g_pSceneFrame, &fRelX, &fRelY, &fRelZ);
            SetFramePosRel_4069DD(pActorFrames[i], (CHanaFrameT3EC_S354 *)g_pSceneFrame, fRelX, fRelY, fRelZ);
            CalculateLocalRotationMatrix(
              pScene->refsFrames_134[j],
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              &dirForwardX,
              &fDirForwardY,
              &dirForwardZ,
              &fDirUpX,
              &fDirUpY,
              &fDirUpZ);
            UpdateBoneLookAtTransform(
              pActorFrames[i],
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              dirForwardX,
              0.0,
              dirForwardZ,
              0.0,
              1.0,
              0.0);
          }
          break;
        }
      }
    }
  }
}

// Duel overhead camera (hunting mode, hero anim <= 6 + g_nFirstPersonCam_530268). Moves g_pCameraFrame to 16 units above
// the hero (hero-local Y up), aims it at the FOE (foe frame temporarily repositioned to 2x hero-foe horizontal
// distance along hero-local Z and re-posed to anim time 0 for a stable look-at target, both restored at the end),
// flags the hero frame bAlphaDeferred_130=1 (deferred/ghosted so it does not block the overhead view),
// then SMOOTHS the camera transform from its entry state toward the snapped state (lerp factor fDelta/5)
// via UpdateBoneLookAtTransform. Uses foe AND hero bodies; NO map collision handling
// (unlike UpdateCameraFollowChase_445525). CalculateLocalRotationMatrix returns the camera transform in hero space: col-3 = position, col-2 = up vector.
void __cdecl hook_UpdateCameraFollowDuel_44C028(CSekBody *pHeroBody, CSekBody *pFoeBody, float fDelta)
{
  double fHorizDist; // st7
  float fSmPosX; // [esp+0h] [ebp-64h]
  float fSmPosY; // [esp+4h] [ebp-60h]
  float fSmPosZ; // [esp+8h] [ebp-5Ch]
  float fAimUpX; // [esp+Ch] [ebp-58h]
  float fSmUpX; // [esp+Ch] [ebp-58h]
  float fAimUpY; // [esp+10h] [ebp-54h]
  float fSmUpY; // [esp+10h] [ebp-54h]
  float fFoeScratchZ; // [esp+14h] [ebp-50h]
  float fAimUpZ; // [esp+14h] [ebp-50h]
  float fSmUpZ; // [esp+14h] [ebp-50h]
  float fNegHeight; // [esp+18h] [ebp-4Ch]
  float fFoeAnimTime; // [esp+20h] [ebp-44h]
  float fFoeRelZ; // [esp+24h] [ebp-40h] BYREF
  float fStartUpZ; // [esp+28h] [ebp-3Ch] BYREF
  float fStartPosZ; // [esp+2Ch] [ebp-38h] BYREF
  int iAnim; // [esp+30h] [ebp-34h]
  float fTgtPosZ; // [esp+34h] [ebp-30h] BYREF
  float fTgtPosY; // [esp+38h] [ebp-2Ch] BYREF
  float fFoeRelY; // [esp+3Ch] [ebp-28h] BYREF
  float fStartUpY; // [esp+40h] [ebp-24h] BYREF
  float fTgtPosX; // [esp+44h] [ebp-20h] BYREF
  float fStartPosY; // [esp+48h] [ebp-1Ch] BYREF
  float fTgtUpZ; // [esp+4Ch] [ebp-18h] BYREF
  float fTgtUpY; // [esp+50h] [ebp-14h] BYREF
  float fFoeRelX; // [esp+54h] [ebp-10h] BYREF
  float fStartUpX; // [esp+58h] [ebp-Ch] BYREF
  float fTgtUpX; // [esp+5Ch] [ebp-8h] BYREF
  float fStartPosX; // [esp+60h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateCameraFollowDuel_44C028\n");

  HanaFrame_GetRelPosition(
    pFoeBody->pSekHeader_220->pFrameT3EC,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    &fFoeRelX,
    &fFoeRelY,
    &fFoeRelZ);
  fHorizDist = sqrt(fFoeRelX * fFoeRelX + fFoeRelZ * fFoeRelZ);
  fFoeScratchZ = fHorizDist + fHorizDist;
  SetFramePosRel_4069DD(
    pFoeBody->pSekHeader_220->pFrameT3EC,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    0.0,
    fFoeRelY,
    fFoeScratchZ);
  CalculateLocalRotationMatrix(
    g_pCameraFrame,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    &fStartPosX,
    &fStartPosY,
    &fStartPosZ,
    &fStartUpX,
    &fStartUpY,
    &fStartUpZ);
  pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
  SetFramePosRel_4069DD(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC, 0.0, 16.0, 0.0);
  fFoeAnimTime = pFoeBody->pSekHeader_220->pAnimSetT3F1->field_130;
  for ( iAnim = 0; iAnim < pFoeBody->pSekHeader_220->pAnimSetT3F1->nAnimCount_134; ++iAnim )
    InterpolateHanaAnimFrame(pFoeBody->pSekHeader_220->pAnimSetT3F1->refsAnimT3F0_138[iAnim], 0.0);
  MtxStackStartMultLocal_4074EF();
  HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, 0);
  for ( iAnim = 0; iAnim < pFoeBody->pSekHeader_220->pAnimSetT3F1->nAnimCount_134; ++iAnim )
    InterpolateHanaAnimFrame(pFoeBody->pSekHeader_220->pAnimSetT3F1->refsAnimT3F0_138[iAnim], fFoeAnimTime);
  MtxStackStartMultLocal_4074EF();
  CalculateLocalRotationMatrix(
    g_pCameraFrame,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    &fTgtPosX,
    &fTgtPosY,
    &fTgtPosZ,
    &fTgtUpX,
    &fTgtUpY,
    &fTgtUpZ);
  fAimUpZ = fTgtUpZ;
  fAimUpY = fTgtUpY;
  fAimUpX = fTgtUpX;
  fNegHeight = -sqrt(fTgtPosY * fTgtPosY);
  UpdateBoneLookAtTransform(
    g_pCameraFrame,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    0.0,
    fNegHeight,
    -1.0,
    fAimUpX,
    fAimUpY,
    fAimUpZ);
  CalculateLocalRotationMatrix(
    g_pCameraFrame,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    &fTgtPosX,
    &fTgtPosY,
    &fTgtPosZ,
    &fTgtUpX,
    &fTgtUpY,
    &fTgtUpZ);
  fSmUpZ = (fTgtUpZ - fStartUpZ) / 5.0 * fDelta + fStartUpZ;
  fSmUpY = (fTgtUpY - fStartUpY) / 5.0 * fDelta + fStartUpY;
  fSmUpX = (fTgtUpX - fStartUpX) / 5.0 * fDelta + fStartUpX;
  fSmPosZ = (fTgtPosZ - fStartPosZ) / 5.0 * fDelta + fStartPosZ;
  fSmPosY = (fTgtPosY - fStartPosY) / 5.0 * fDelta + fStartPosY;
  fSmPosX = (fTgtPosX - fStartPosX) / 5.0 * fDelta + fStartPosX;
  UpdateBoneLookAtTransform(
    g_pCameraFrame,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    fSmPosX,
    fSmPosY,
    fSmPosZ,
    fSmUpX,
    fSmUpY,
    fSmUpZ);
  SetFramePosRel_4069DD(
    pFoeBody->pSekHeader_220->pFrameT3EC,
    pHeroBody->pSekHeader_220->pFrameT3EC,
    fFoeRelX,
    fFoeRelY,
    fFoeRelZ);
}

// REC-viewfinder HUD for duel/cutscene scenes (drawn only when nHeroHpMax > 0).
// Args are percent-scaled ints (callers pass value/100); only the ratios matter. 
// Gauge A = nHeroHpCur/nHeroHpMax -> bottom-right meter: bracket sprites + fill quad growing
// left-to-right 354->600 across the 246px strip (fMeterA_X1/X3), src-rect 
// needle x = texCenter - cur*(texW/2-1)/max. Gauge B = nFoeHpCur/pFoeHpMax -> top-left meter on
// pSprites[0]: fill quad grows RIGHT-to-left 286->40 (fMeterB_X0/X2 use (1-ratio)*246),
// plus a 3-level indicator: nLevelIdx = 2 - 3*ratio clamped 0..2 picks sprite [9..11].
// pSprites[2] = meters texture (dwWidth_1F0 packed W/H), pSprites[1] = top strip bar (320,0)-(640,30),
// pSprites[8] = bottom-right timer label (480,30)-(640,78). PlayHunting passes
// hero HP / hero max HP / foe aggro / aggro max; other scenes feed their own gauges into the same two meters.
// The 4-iteration pointer walk at the top is DEAD code (stores nothing; likely a stripped vertex-array initializer).
// GetSurfaceWH result nTexW/nTexH is lik
void __cdecl hook_DrawDuelHealthHud_44B23B(
  CHanaSpriteT3F6_S1FC **pSprites,
  int nHeroHpCur,
  int nHeroHpMax,
  int nFoeHpCur,
  int pFoeHpMax)
{
  int left1; // eax
  int left2; // eax
  int left3; // eax
  int left4; // eax
  int top1; // [esp+20h] [ebp-6Ch]
  int top2; // [esp+20h] [ebp-6Ch]
  int top3; // [esp+20h] [ebp-6Ch]
  int top4; // [esp+20h] [ebp-6Ch]
  int right1; // [esp+24h] [ebp-68h]
  int right2; // [esp+24h] [ebp-68h]
  int right3; // [esp+24h] [ebp-68h]
  int right4; // [esp+24h] [ebp-68h]
  float fMeterA_X1; // [esp+28h] [ebp-64h]
  float fMeterA_X3; // [esp+28h] [ebp-64h]
  int bottom1; // [esp+28h] [ebp-64h]
  int bottom2; // [esp+28h] [ebp-64h]
  float fMeterB_X0; // [esp+28h] [ebp-64h]
  float fMeterB_X2; // [esp+28h] [ebp-64h]
  int bottom3; // [esp+28h] [ebp-64h]
  int bottom4; // [esp+28h] [ebp-64h]
  int nDeadInitCtr; // [esp+34h] [ebp-58h]
  D3DXVECTOR3 *pDeadInitPtr; // [esp+3Ch] [ebp-50h]
  int nLevelIdx; // [esp+40h] [ebp-4Ch]
  DWORD nTexW; // [esp+44h] [ebp-48h] BYREF
  D3DXVECTOR3 parrPos[4]; // [esp+48h] [ebp-44h] BYREF
  struct tagRECT rc; // [esp+78h] [ebp-14h] BYREF
  DWORD nTexH; // [esp+88h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] DrawDuelHealthHud_44B23B\n");

  nDeadInitCtr = 4;
  for ( pDeadInitPtr = parrPos; --nDeadInitCtr >= 0; ++pDeadInitPtr )
    ;
  if ( nHeroHpMax > 0 )
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      GetSurfaceWH(pSprites[2]->pTexture_134, &nTexW, &nTexH);
      SetRect(&rc, 253, 0, 254, 48);
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[2], &rc);
      parrPos[0].x = ConvertFrom640AsFloat(354.0);
      parrPos[0].y = ConvertFrom640AsFloat(13.0);
      parrPos[0].z = 0.0;
      parrPos[1].x = ConvertFrom640AsFloat(600.0);
      parrPos[1].y = ConvertFrom640AsFloat(13.0);
      parrPos[1].z = 0.0;
      parrPos[2].x = ConvertFrom640AsFloat(614.0);
      parrPos[2].y = ConvertFrom640AsFloat(28.0);
      parrPos[2].z = 0.0;
      parrPos[3].x = ConvertFrom640AsFloat(368.0);
      parrPos[3].y = ConvertFrom640AsFloat(28.0);
      parrPos[3].z = 0.0;
      DrawSprite3D(pSprites[2], parrPos);
      parrPos[0].x = ConvertFrom640AsFloat(40.0);
      parrPos[0].y = ConvertFrom640AsFloat(13.0);
      parrPos[0].z = 0.0;
      parrPos[1].x = ConvertFrom640AsFloat(286.0);
      parrPos[1].y = ConvertFrom640AsFloat(13.0);
      parrPos[1].z = 0.0;
      parrPos[2].x = ConvertFrom640AsFloat(272.0);
      parrPos[2].y = ConvertFrom640AsFloat(28.0);
      parrPos[2].z = 0.0;
      parrPos[3].x = ConvertFrom640AsFloat(26.0);
      parrPos[3].y = ConvertFrom640AsFloat(28.0);
      parrPos[3].z = 0.0;
      DrawSprite3D(pSprites[2], parrPos);

      // IDA Hex-Rays bug: CDQ SUB EAX, EDX, SAR sequence
      int xBer = (pSprites[2]->dwWidth_1F0 / 2) - nHeroHpCur * (pSprites[2]->dwWidth_1F0 / 2 - 1) / nHeroHpMax;

      SetRect(
        &rc,
        xBer,
        0,
        xBer,
        48);
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[2], &rc);
      parrPos[0].x = ConvertFrom640AsFloat(354.0);
      parrPos[0].y = ConvertFrom640AsFloat(13.0);
      parrPos[0].z = 0.0;
      fMeterA_X1 = (double)(246 * nHeroHpCur / nHeroHpMax) + 354.0;
      parrPos[1].x = ConvertFrom640AsFloat(fMeterA_X1);
      parrPos[1].y = ConvertFrom640AsFloat(13.0);
      parrPos[1].z = 0.0;
      fMeterA_X3 = (double)(246 * nHeroHpCur / nHeroHpMax) + 368.0;
      parrPos[2].x = ConvertFrom640AsFloat(fMeterA_X3);
      parrPos[2].y = ConvertFrom640AsFloat(28.0);
      parrPos[2].z = 0.0;
      parrPos[3].x = ConvertFrom640AsFloat(368.0);
      parrPos[3].y = ConvertFrom640AsFloat(28.0);
      parrPos[3].z = 0.0;
      DrawSprite3D(pSprites[2], parrPos);
      bottom1 = ConvertFrom640AsInt(30);
      right1 = ConvertFrom640AsInt(640);
      top1 = ConvertFrom640AsInt(-2);
      left1 = ConvertFrom640AsInt(320);
      SetRect(&rc, left1, top1, right1, bottom1);
      DrawSpriteLTRB(pSprites[1], rc.left, rc.top, rc.right, rc.bottom);
      SetRect(&rc, 0, 1, 160, 47);
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[8], &rc);
      bottom2 = ConvertFrom640AsInt(78);
      right2 = ConvertFrom640AsInt(640);
      top2 = ConvertFrom640AsInt(30);
      left2 = ConvertFrom640AsInt(480);
      SetRect(&rc, left2, top2, right2, bottom2);
      DrawSpriteLTRB(pSprites[8], rc.left, rc.top, rc.right, rc.bottom);

      // IDA Hex-Rays bug: CDQ SUB EAX, EDX, SAR sequence
      xBer = (pSprites[2]->dwWidth_1F0 / 2) - nFoeHpCur * (pSprites[2]->dwWidth_1F0 / 2) / pFoeHpMax + 1;

      SetRect(
        &rc,
        xBer,
        0,
        xBer,
        48);
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[2], &rc);
      fMeterB_X0 = (1.0 - (double)nFoeHpCur / (double)pFoeHpMax) * 246.0 + 40.0;
      parrPos[0].x = ConvertFrom640AsFloat(fMeterB_X0);
      parrPos[0].y = ConvertFrom640AsFloat(13.0);
      parrPos[0].z = 0.0;
      parrPos[1].x = ConvertFrom640AsFloat(286.0);
      parrPos[1].y = ConvertFrom640AsFloat(13.0);
      parrPos[1].z = 0.0;
      parrPos[2].x = ConvertFrom640AsFloat(272.0);
      parrPos[2].y = ConvertFrom640AsFloat(28.0);
      parrPos[2].z = 0.0;
      fMeterB_X2 = (1.0 - (double)nFoeHpCur / (double)pFoeHpMax) * 246.0 + 26.0;
      parrPos[3].x = ConvertFrom640AsFloat(fMeterB_X2);
      parrPos[3].y = ConvertFrom640AsFloat(28.0);
      parrPos[3].z = 0.0;
      DrawSprite3D(pSprites[2], parrPos);
      bottom3 = ConvertFrom640AsInt(30);
      right3 = ConvertFrom640AsInt(320);
      top3 = ConvertFrom640AsInt(-2);
      left3 = ConvertFrom640AsInt(0);
      SetRect(&rc, left3, top3, right3, bottom3);
      DrawSpriteLTRB(*pSprites, rc.left, rc.top, rc.right, rc.bottom);
      nLevelIdx = 2 - (__int64)((double)nFoeHpCur / (double)pFoeHpMax * 3.0);
      if ( nLevelIdx > 2 )
        nLevelIdx = 2;
      if ( nLevelIdx < 0 )
        nLevelIdx = 0;
      SetRect(&rc, 0, 1, 160, 47);
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[nLevelIdx + 9], &rc);
      bottom4 = ConvertFrom640AsInt(78);
      right4 = ConvertFrom640AsInt(160);
      top4 = ConvertFrom640AsInt(30);
      left4 = ConvertFrom640AsInt(0);
      SetRect(&rc, left4, top4, right4, bottom4);
      DrawSpriteLTRB(pSprites[nLevelIdx + 9], rc.left, rc.top, rc.right, rc.bottom);
      g_pD3DDevice_4D6C78->EndScene();
    }
  }
}

// DrawSprite3D - draw pSprite's texture as a 3D billboard at a position taken from
// the D3DMATRIX pMatrix (pos = pMatrix[0], pMatrix[1], pMatrix[2]; remaining matrix
// elements loaded to the stack but unused). Same texture/modulate + no-lighting/
// no-fog + default-material setup as DrawSprite2DCenter, drawn via
// D3DXDrawSprite3D(pSprite+0x134, pos, w=+0x144, h=+0x16C); restores fog/lighting
// and clears g_pCurMatObj.
void __cdecl hook_DrawSprite3D(CHanaSpriteT3F6_S1FC *pSprite, D3DXVECTOR3 *parrPos)
{
  //HRESULT v2; // eax
  int nCount; // [esp+0h] [ebp-54h]
  D3DXVECTOR4 *i; // [esp+8h] [ebp-4Ch]
  DWORD dwFogState; // [esp+Ch] [ebp-48h] BYREF
  HRESULT hrDraw; // [esp+10h] [ebp-44h]
  D3DXVECTOR4 quadPos[4]; // [esp+14h] [ebp-40h] BYREF

  ONCELOG("[HNH:SUB] DrawSprite3D_414344\n");

  nCount = 4;
  for ( i = quadPos; --nCount >= 0; ++i )
    ;
  quadPos[0].x = parrPos->x;
  quadPos[0].y = parrPos->y;
  quadPos[0].z = parrPos->z;
  quadPos[0].w = 0.0;
  quadPos[1].x = parrPos[1].x;
  quadPos[1].y = parrPos[1].y;
  quadPos[1].z = parrPos[1].z;
  quadPos[1].w = 0.0;
  quadPos[2].x = parrPos[2].x;
  quadPos[2].y = parrPos[2].y;
  quadPos[2].z = parrPos[2].z;
  quadPos[2].w = 0.0;
  quadPos[3].x = parrPos[3].x;
  quadPos[3].y = parrPos[3].y;
  quadPos[3].z = parrPos[3].z;
  quadPos[3].w = 0.0;
  g_pD3DDevice_4D6C78->SetTexture(0, pSprite->pTexture_134);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_ALPHAOP, 4);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG1, 2);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLORARG2, 0);
  g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_COLOROP, 4);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 0);
  HanaMate_SetMaterial(g_pMaterial);
  //hrDraw = v2;
  g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogState);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
  hrDraw = D3DXDrawSprite3D(
    pSprite->pTexture_134,
    g_pD3DDevice_4D6C78,
    quadPos,
    pSprite->fAlpha_144,
    pSprite->pRect_16C);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwFogState);
  g_pCurMatObj = 0;
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
}

// Two-body cutscene compositor (only caller: PlayHunting - composites the duel's hero+foe bodies into an ODS cutscene scene). Swaps LIVE bodies into STAND-IN slots:
// finds the scene frame whose world position exactly equals the body frame's (float == match over pScene->refsFrames_134[0..dwCount_130)),
// reparents the body frame under it (AttachChild + MtxMult 0 + look-at identity). pStandInA/B = the scene's own CSekBody at that slot; its field_200
// (current anim item) is copied onto the live body EVERY FRAME, so the scene animation drives the live bodies' anims.
// Playback loop: mouse button held = 5x speed (GetCursorPos 3rd out; X/Y unused); advances pfExecTime to pAnimSet_148->fRes_13C under a cinematic 0.4 FOV.
// Fade choreography vs remaining time: pSprites[0] expanding box at (320,350) +/- 32*fBoxExpand grows in while >150 left, fades out after (fBoxAlpha);
// pSprites[1] centered sprite does the OPPOSITE (fCenterAlpha out then in); pSprites[7] letterbox banner (top band (0,380)-(640,700) + flipped (640,320)-(0,0) ba
void __cdecl hook_RenderDualBodyScene_456807(
  CHanaSceneT458_S504 *pScene,
  CSekBody *pBodyA,
  CSekBody *pBodyB,
  CHanaSpriteT3F6_S1FC **pSprites,
  CHanaSpriteT3F6_S1FC **pViewfinder)
{
  double v5; // st7
  float fCenterX; // [esp+0h] [ebp-B8h]
  float fCenterY; // [esp+4h] [ebp-B4h]
  float fCenterScale; // [esp+4h] [ebp-B4h]
  int v9; // [esp+8h] [ebp-B0h]
  int v10; // [esp+Ch] [ebp-ACh]
  int v11; // [esp+1Ch] [ebp-9Ch]
  int v12; // [esp+20h] [ebp-98h]
  int v13; // [esp+24h] [ebp-94h]
  RECT Rect; // [esp+28h] [ebp-90h] BYREF
  int nBoxL; // [esp+38h] [ebp-80h]
  int nBoxT; // [esp+3Ch] [ebp-7Ch]
  int nBoxR; // [esp+40h] [ebp-78h]
  int nBoxB; // [esp+44h] [ebp-74h]
  int nMouseY; // [esp+48h] [ebp-70h] BYREF
  int nMouseX; // [esp+4Ch] [ebp-6Ch] BYREF
  float pfExecTime; // [esp+50h] [ebp-68h] BYREF
  float fScnZ; // [esp+54h] [ebp-64h] BYREF
  float fBannerAlpha; // [esp+58h] [ebp-60h]
  float fScnY[3]; // [esp+5Ch] [ebp-5Ch] BYREF
  float fScnX; // [esp+68h] [ebp-50h] BYREF
  float fBoxExpand; // [esp+6Ch] [ebp-4Ch]
  int i; // [esp+70h] [ebp-48h]
  float fBodyZ; // [esp+74h] [ebp-44h] BYREF
  float fBodyY; // [esp+78h] [ebp-40h] BYREF
  float fBoxAlpha; // [esp+7Ch] [ebp-3Ch]
  float fBodyX; // [esp+80h] [ebp-38h] BYREF
  float fCenterAlpha; // [esp+84h] [ebp-34h]
  float fViewAlpha; // [esp+88h] [ebp-30h]
  CSekBody *pStandInA; // [esp+8Ch] [ebp-2Ch]
  CSekBody *pStandInB; // [esp+90h] [ebp-28h]
  float fAxisZ; // [esp+94h] [ebp-24h] BYREF
  float fFov; // [esp+98h] [ebp-20h]
  float fLookFwdZ; // [esp+9Ch] [ebp-1Ch] BYREF
  float fAxisY; // [esp+A0h] [ebp-18h] BYREF
  float fDelta; // [esp+A4h] [ebp-14h]
  float fLookFwdY; // [esp+A8h] [ebp-10h] BYREF
  float fAxisX; // [esp+ACh] [ebp-Ch] BYREF
  float fLookFwdX; // [esp+B0h] [ebp-8h] BYREF
  __int16 bMouseBtn; // [esp+B4h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] RenderDualBodyScene_456807\n");

  pfExecTime = 0.0;
  fBoxAlpha = 0.0;
  fCenterAlpha = 0.0;
  fBannerAlpha = 0.80000001;
  fViewAlpha = 0.0;
  fBoxExpand = 0.0;
  pStandInA = 0;
  pStandInB = 0;
  fScnY[1] = 1.0;
  fScnY[2] = -1.0;

  if (!pScene) return;


  fFov = GetProjectionFov_40DCF6();
  SetProjectionFov_40DCA3(0.40000001);
  HanaFrame_GetRelPosition(pBodyA->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fBodyX, &fBodyY, &fBodyZ);
  for ( i = 0; i < pScene->dwCount_130; ++i )
  {
    HanaFrame_GetRelPosition(pScene->refsFrames_134[i], g_pSceneFrame, &fScnX, fScnY, &fScnZ);
    if ( fBodyX == fScnX && fBodyY == fScnY[0] && fBodyZ == fScnZ )
    {
      pStandInA = pScene->refsSekBody_13C[i];
      AttachChild_4055FD(pScene->refsFrames_134[i], pBodyA->pSekHeader_220->pFrameT3EC);
      SetFramePosRel_4069DD(pBodyA->pSekHeader_220->pFrameT3EC, pScene->refsFrames_134[i], 0.0, 0.0, 0.0);
      UpdateBoneLookAtTransform(
        pBodyA->pSekHeader_220->pFrameT3EC,
        pScene->refsFrames_134[i],
        0.0,
        0.0,
        1.0,
        0.0,
        1.0,
        0.0);
      break;
    }
  }
  HanaFrame_GetRelPosition(pBodyB->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fBodyX, &fBodyY, &fBodyZ);
  for ( i = 0; i < pScene->dwCount_130; ++i )
  {
    HanaFrame_GetRelPosition(pScene->refsFrames_134[i], g_pSceneFrame, &fScnX, fScnY, &fScnZ);
    if ( fBodyX == fScnX && fBodyY == fScnY[0] && fBodyZ == fScnZ )
    {
      pStandInB = pScene->refsSekBody_13C[i];
      AttachChild_4055FD(pScene->refsFrames_134[i], pBodyB->pSekHeader_220->pFrameT3EC);
      SetFramePosRel_4069DD(pBodyB->pSekHeader_220->pFrameT3EC, pScene->refsFrames_134[i], 0.0, 0.0, 0.0);
      UpdateBoneLookAtTransform(
        pBodyB->pSekHeader_220->pFrameT3EC,
        pScene->refsFrames_134[i],
        0.0,
        0.0,
        1.0,
        0.0,
        1.0,
        0.0);
      break;
    }
  }
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  while ( 1 )
  {
    DoPeekMessages();
    fDelta = GetFrameTime();
    FrameTimer_EndFrame();
    GetCursorPos_4811FA(&nMouseX, &nMouseY, &bMouseBtn);
    if ( bMouseBtn )
      fDelta = fDelta * 5.0;
    UpdateSceneTimer_4827FE(pScene, &pfExecTime, fDelta);
    if ( pfExecTime >= (double)pScene->pAnimSet_148->fRes_13C )
      break;
    UpdateSceneAtTime_4825D5(pScene, &pfExecTime, fDelta);
    SekBody__UpdateTo2_401647(pBodyA, pStandInA->field_200);
    SekBody__UpdateTo2_401647(pBodyB, pStandInB->field_200);
    if ( pBodyA )
      CSekBody__UpdateAnim_4019E1(pBodyA, fDelta);
    if ( pBodyB )
      CSekBody__UpdateAnim_4019E1(pBodyB, fDelta);
    if ( pfExecTime + 150.0 < pScene->pAnimSet_148->fRes_13C )
    {
      fBoxAlpha = fDelta / 200.0 + fBoxAlpha;
      fBoxExpand = fDelta / 100.0 * 3.0 + fBoxExpand;
      if ( fBoxExpand > 1.0 )
        fBoxExpand = 1.0;
    }
    else
    {
      fBoxAlpha = fBoxAlpha - fDelta / 100.0;
      fBoxExpand = fDelta / 100.0 + fDelta / 100.0 + fBoxExpand;
    }
    if ( fBoxAlpha >= 1.0 )
      fBoxAlpha = 1.0;
    if ( fBoxAlpha <= 0.0 )
      fBoxAlpha = 0.0;
    CHanaObj__SetAlpha3F6(*pSprites, fBoxAlpha);
    if ( pSprites[1] )
    {
      if ( pfExecTime + 150.0 < pScene->pAnimSet_148->fRes_13C )
        v5 = fCenterAlpha - fDelta / 100.0;
      else
        v5 = fDelta / 100.0 + fCenterAlpha;
      fCenterAlpha = v5;
      if ( fCenterAlpha >= 1.0 )
        fCenterAlpha = 1.0;
      if ( fCenterAlpha <= 0.0 )
        fCenterAlpha = 0.0;
      CHanaObj__SetAlpha3F6(pSprites[1], fCenterAlpha);
      fCenterY = ConvertFrom640AsFloat(350.0);
      fCenterX = ConvertFrom640AsFloat(320.0);
      CHanaSpriteT3F6__SetPos_414A5D(pSprites[1], fCenterX, fCenterY);
      fCenterScale = ConvertFrom640AsFloat(1.0);
      CHanaSpriteT3F6__SetScale_414B09(pSprites[1], fCenterScale);
    }
    if ( pfExecTime + 20.0 >= pScene->pAnimSet_148->fRes_13C )
      fBannerAlpha = fBannerAlpha - fDelta / 20.0;
    if ( fBannerAlpha <= 0.0 )
      fBannerAlpha = 0.0;
    if ( pfExecTime + 130.0 >= pScene->pAnimSet_148->fRes_13C )
      fViewAlpha = fDelta / 130.0 + fViewAlpha;
    if ( fViewAlpha >= 1.0 )
      fViewAlpha = 1.0;
    MtxStackStartMultLocal_4074EF();
    g_pRenderCtx->Clear(3);
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
      HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      HanaVert_DrawBatch(1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      if ( pSprites[1] )
      {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
        DrawSprite2D(pSprites[1]);
      }
      if ( *pSprites )
      {
        nBoxL = ConvertFrom640AsInt(192);
        nBoxT = ConvertFrom640AsInt(350 - (__int64)(32.0 * fBoxExpand));
        nBoxR = ConvertFrom640AsInt(448);
        nBoxB = ConvertFrom640AsInt((__int64)(32.0 * fBoxExpand) + 350);
        DrawSpriteLTRB(*pSprites, nBoxL, nBoxT, nBoxR, nBoxB);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }
    for ( i = 0; i < 15; ++i )
    {
      if ( pViewfinder[i] )
        CHanaObj__SetAlpha3F6(pViewfinder[i], fViewAlpha);
    }
    DrawDuelHealthHud_44B23B(
      pViewfinder,
      (__int64)(fViewAlpha * 100.0 * g_sceneState_52F7E0.protCurrentHP_C0),
      (__int64)(100.0 * g_sceneState_52F7E0.protMaximumHP_C4),
      (__int64)(fViewAlpha * 100.0),
      100);
    if ( pSprites[7] && g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      v11 = ConvertFrom640AsInt(380);
      v12 = ConvertFrom640AsInt(640);
      v13 = ConvertFrom640AsInt(700);
      v9 = ConvertFrom640AsInt(640);
      v10 = ConvertFrom640AsInt(320);
      Rect.left = 1;
      Rect.top = 1;
      Rect.right = pSprites[7]->dwWidth_1F0 - 1;
      Rect.bottom = pSprites[7]->dwHeight_1F4 - 1;
      CHanaSpriteT3F6__SetSrcRect_414B28(pSprites[7], &Rect);
      CHanaObj__SetAlpha3F6(pSprites[7], fBannerAlpha);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
      DrawSpriteLTRB(pSprites[7], 0, v11, v12, v13);
      DrawSpriteLTRB(pSprites[7], v9, v10, 0, 0);
      g_pD3DDevice_4D6C78->EndScene();
    }

    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  pfExecTime = pScene->pAnimSet_148->fRes_13C - 1.0;
  UpdateSceneAtTime_4825D5(pScene, &pfExecTime, 0.0);
  MtxStackStartMultLocal_4074EF();
  HanaFrame_GetRelPosition(pBodyA->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fBodyX, &fBodyY, &fBodyZ);
  CalculateLocalRotationMatrix(
    pBodyA->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &fLookFwdX,
    &fLookFwdY,
    &fLookFwdZ,
    &fAxisX,
    &fAxisY,
    &fAxisZ);
  AttachChild_4055FD(g_pSceneFrame, pBodyA->pSekHeader_220->pFrameT3EC);
  SetFramePosRel_4069DD(pBodyA->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fBodyX, fBodyY, fBodyZ);
  UpdateBoneLookAtTransform(
    pBodyA->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    fLookFwdX,
    0.0,
    fLookFwdZ,
    0.0,
    1.0,
    0.0);
  HanaFrame_GetRelPosition(pBodyB->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fBodyX, &fBodyY, &fBodyZ);
  CalculateLocalRotationMatrix(
    pBodyB->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    &fLookFwdX,
    &fLookFwdY,
    &fLookFwdZ,
    &fAxisX,
    &fAxisY,
    &fAxisZ);
  AttachChild_4055FD(g_pSceneFrame, pBodyB->pSekHeader_220->pFrameT3EC);
  SetFramePosRel_4069DD(pBodyB->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fBodyX, fBodyY, fBodyZ);
  UpdateBoneLookAtTransform(
    pBodyB->pSekHeader_220->pFrameT3EC,
    (CHanaFrameT3EC_S354 *)g_pSceneFrame,
    fLookFwdX,
    0.0,
    fLookFwdZ,
    0.0,
    1.0,
    0.0);

  SetProjectionFov_40DCA3(fFov);
  for ( i = 0; i < 15; ++i )
  {
    if ( pViewfinder[i] )
      CHanaObj__SetAlpha3F6(pViewfinder[i], 1.0);
  }
}

void __cdecl hook_StepFoeTurn_457EE0(
  CSekBody *pFoeBody,
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fDelta,
  float fSpinRate,
  float fSpeed)
{
  double fAngleHalf; // st7
  float fAngleOne; // [esp+0h] [ebp-28h]
  float anglea; // [esp+0h] [ebp-28h]
  float z; // [esp+0h] [ebp-28h]
  float fAngleTwo; // [esp+0h] [ebp-28h]
  CHanaFrameT3EC_S354 *pSpinFrame; // [esp+4h] [ebp-24h]
  float fRelZ; // [esp+8h] [ebp-20h] BYREF
  D3DXVECTOR3 vecSpinAxis; // [esp+Ch] [ebp-1Ch] BYREF
  float fRelY; // [esp+18h] [ebp-10h] BYREF
  float fRelX; // [esp+1Ch] [ebp-Ch] BYREF
  float fTurnDir; // [esp+20h] [ebp-8h]
  CHanaFrameRootT3EC_S2D4 *pHeroFrame; // [esp+24h] [ebp-4h]

  ONCELOG("[HNH:SUB] StepFoeTurn_457EE0\n");

  pSpinFrame = pFoeBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  vecSpinAxis.x = 0.0;
  vecSpinAxis.y = 1.0;
  vecSpinAxis.z = 0.0;
  HanaFrame_GetRelPosition(pSpinFrame, pHeroFrame, &fRelX, &fRelY, &fRelZ);
  if ( fRelX <= 0.5 )
  {
    if ( fRelX >= -0.5 )
      fTurnDir = 0.0;
    else
      fTurnDir = 1.0;
  }
  else
  {
    fTurnDir = -1.0;
  }
  if ( pFoeBody->field_200 <= 5 && pFoeBody->field_200 != 3 )
  {
    SekBody__UpdateTo_4014E4(pFoeBody, 2);
    fAngleOne = fDelta * 3.1418099 / 180.0 * fTurnDir * fSpinRate;
    Frame_RotateMatrixAxis_4070AB(pSpinFrame, 1, &vecSpinAxis, fAngleOne);
    anglea = -fDelta * fSpeed;
    SetFramePosRel_4069DD(pSpinFrame, pSpinFrame, 0.0, 0.0, anglea);
    if ( !CheckFrameCollision_443CC5(pSpinFrame, pMapFrame, 0) )
    {
      z = fDelta * fSpeed;
      SetFramePosRel_4069DD(pSpinFrame, pSpinFrame, 0.0, 0.0, z);
      fAngleHalf = fDelta * 3.1418099 / 180.0 * fTurnDir * fSpinRate;
      fAngleTwo = fAngleHalf + fAngleHalf;
      Frame_RotateMatrixAxis_4070AB(pSpinFrame, 1, &vecSpinAxis, fAngleTwo);
    }
  }
}

void __cdecl hook_StepFoeSpin_45804C(
  CSekBody *pFoeBody,
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fDelta,
  float fSpinRate,
  float fSpeed)
{
  double fAngleHalf; // st7
  float fAngleOne; // [esp+0h] [ebp-24h]
  float fAngleRev; // [esp+0h] [ebp-24h]
  float fAngleTwo; // [esp+0h] [ebp-24h]
  CHanaFrameT3EC_S354 *pFoeSpinFrame; // [esp+4h] [ebp-20h]
  float fRelZ; // [esp+8h] [ebp-1Ch] BYREF
  float fRelY; // [esp+Ch] [ebp-18h] BYREF
  D3DXVECTOR3 vecSpinAxis; // [esp+10h] [ebp-14h] BYREF
  float fRelX; // [esp+1Ch] [ebp-8h] BYREF
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+20h] [ebp-4h]

  ONCELOG("[HNH:SUB] StepFoeSpin_45804C\n");

  pFoeSpinFrame = pFoeBody->pSekHeader_220->pFrameT3EC;
  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  vecSpinAxis.x = 0.0;
  vecSpinAxis.y = 1.0;
  vecSpinAxis.z = 0.0;
  if ( pFoeBody->field_200 <= 5 && pFoeBody->field_200 != 3 )
  {
    SekBody__UpdateTo_4014E4(pFoeBody, 2);
    fAngleOne = -fDelta * fSpeed;
    SetFramePosRel_4069DD(pFoeSpinFrame, pFoeSpinFrame, 0.0, 0.0, fAngleOne);
    if ( CheckFrameCollision_443CC5(pFoeSpinFrame, pMapFrame, 0) )
    {
      g_bFoeSpun_52EF60 = 0;
    }
    else
    {
      if ( !g_bFoeSpun_52EF60 )
      {
        HanaFrame_GetRelPosition(pHeroFrame, pFoeSpinFrame, &fRelX, &fRelY, &fRelZ);
        if ( fRelX < 0.0 )
          g_fFoeSpinDir_52EF5C = 1.0;
        else
          g_fFoeSpinDir_52EF5C = -1.0;
      }
      g_bFoeSpun_52EF60 = 1;
      fAngleRev = fDelta * fSpeed;
      SetFramePosRel_4069DD(pFoeSpinFrame, pFoeSpinFrame, 0.0, 0.0, fAngleRev);
      fAngleHalf = fDelta * 3.1418099 / 180.0 * g_fFoeSpinDir_52EF5C * fSpinRate;
      fAngleTwo = fAngleHalf + fAngleHalf;
      Frame_RotateMatrixAxis_4070AB(pFoeSpinFrame, 1, &vecSpinAxis, fAngleTwo);
    }
  }
}

/**
PlayVictoryCutscene_455183 -- post-duel neck-zoom cutscene

Hero plays anim 26, foe anim 9, FOV forced to 0.8, then two phases:
- Phase 1 (fScriptTime 0->10): time scale dropped to 1/3 (fSlowMoScale3), FOV eases 1.5->0.9 (fFovSwing), and the camera
swings between two anchors -- 60 % behind the hero looking at the foe (fCamSwingX1/Y1/Z1), then same X/Z but Y+10
looking at the foe's kubi (neck) frame (fCamSwingX2/Y2/Z2), each pulled 8/6 units back on its foe-relative X
(fFoeRelXBack8/6).
- Phase 2 (fSwingTime 0->50): time scale 1/5 (fSlowMoScale5), camera dollies onto the neck (fDollyOffset -10>0,
fKubiCamX/Y/Z, fKubiPullbackZ), FOV narrows to 0.2 (fFovZoomIn).
*/
void __cdecl hook_PlayVictoryCutscene_455183(
  CHanaSceneT458_S504 *pOdsScene,
  CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame,
  CHanaSpriteT3F6_S1FC **pArrHudMeters, CHanaSpriteT3F6_S1FC **pArrSprites,
  float fHeroHpCur, float fHeroHpMax,
  float fFoeHpCur, float fFoeHpMax, int nClockBase, unsigned int dwElapsedMs,
  CSoundStruct_8 **pArrVoiceBufs) //   CHanaSpriteT3F6_S1FC **pSounds) =

{
  double v13; // st7
  int nRectLeft; // eax
  float fKubiCamX; // [esp+7Ch] [ebp-90h]
  float fCamSwingX1; // [esp+7Ch] [ebp-90h]
  float fFoeRelXBack8; // [esp+7Ch] [ebp-90h]
  float fCamSwingX2; // [esp+7Ch] [ebp-90h]
  float fFoeRelXBack6; // [esp+7Ch] [ebp-90h]
  int nRectTop; // [esp+7Ch] [ebp-90h]
  float fKubiCamY; // [esp+80h] [ebp-8Ch]
  float fCamSwingY1; // [esp+80h] [ebp-8Ch]
  float fCamSwingY2; // [esp+80h] [ebp-8Ch]
  int nRectRight; // [esp+80h] [ebp-8Ch]
  float fSlowMoScale5; // [esp+84h] [ebp-88h]
  float fKubiCamZ; // [esp+84h] [ebp-88h]
  float fKubiPullbackZ; // [esp+84h] [ebp-88h]
  float fSlowMoScale3; // [esp+84h] [ebp-88h]
  float fCamSwingZ1; // [esp+84h] [ebp-88h]
  float fCamSwingZ2; // [esp+84h] [ebp-88h]
  int nRectBottom; // [esp+84h] [ebp-88h]
  float fViewfinderAlpha; // [esp+84h] [ebp-88h]
  float fHudAlpha; // [esp+84h] [ebp-88h]
  float fFovSwing; // [esp+8Ch] [ebp-80h]
  float fTimeScaleSaved; // [esp+90h] [ebp-7Ch]
  float fCamFoeRelZ; // [esp+94h] [ebp-78h] BYREF
  CHanaFrameT3EC_S354 *pOdsMapFrame; // [esp+98h] [ebp-74h]
  float fCamFoeRelY; // [esp+9Ch] [ebp-70h] BYREF
  float fCamFoeRelX; // [esp+A0h] [ebp-6Ch] BYREF
  float fSwingTime; // [esp+A4h] [ebp-68h]
  float fZoomOffset; // [esp+A8h] [ebp-64h]
  CHanaFrameT3EC_S354 *pFoeKubiFrame; // [esp+ACh] [ebp-60h]
  int i; // [esp+B0h] [ebp-5Ch]
  float fHudFade; // [esp+B4h] [ebp-58h]
  float fDelta; // [esp+B8h] [ebp-54h]
  struct tagRECT rc; // [esp+BCh] [ebp-50h] BYREF
  float fFoeRelZ; // [esp+CCh] [ebp-40h] BYREF
  float fFovSaved; // [esp+D0h] [ebp-3Ch]
  float fScriptTime; // [esp+D4h] [ebp-38h]
  DWORD dwLastTickMs; // [esp+D8h] [ebp-34h]
  float fRelZ; // [esp+DCh] [ebp-30h] BYREF
  float fFoeRelY; // [esp+E0h] [ebp-2Ch] BYREF
  float fFovZoomIn; // [esp+E4h] [ebp-28h]
  float fRelY; // [esp+E8h] [ebp-24h] BYREF
  float arrFoeRelPos[4]; // [esp+ECh] [ebp-20h] BYREF
  float fRelX; // [esp+FCh] [ebp-10h] BYREF
  float fDollyOffset; // [esp+100h] [ebp-Ch]
  HGLOBAL pDimOverlay; // [esp+104h] [ebp-8h] BYREF
  int bClearedOnce; // [esp+108h] [ebp-4h]

  ONCELOG("[HNH:SUB] PlayVictoryCutscene_455183\n");

  fTimeScaleSaved = GetTimerScaleFactor();
  fDollyOffset = -10.0;
  fSwingTime = 0.0;
  fScriptTime = 0.0;
  fHudFade = 0.0;
  fZoomOffset = 0.0;
  pFoeKubiFrame = 0;
  pOdsMapFrame = 0;
  pDimOverlay = 0;
  bClearedOnce = 0;
  dwLastTickMs = timeGetTime();
  CHanaObj__NewSprite3F6byWH((CHanaSpriteT3F6_S1FC **)&pDimOverlay, 0x20u, 0x20u);
  pFoeKubiFrame = FindFrameByName_47E999(pFoeBody->pSekHeader_220->pFrameT3EC, "_kubi");
  for ( i = 0; i < pOdsScene->dwCount_130; ++i )
  {
    if ( !_strnicmp(pOdsScene->refsFrames_134[i]->szName_8, "Map\\", 4u) )
    {
      pOdsMapFrame = pOdsScene->refsFrames_134[i];
      break;
    }
  }
  arrFoeRelPos[1] = 0.0;
  arrFoeRelPos[2] = 1.0;
  arrFoeRelPos[3] = 0.0;
  CSekBody__PlayAnim_40179D(pHeroBody, 26);
  CSekBody__PlayAnim_40179D(pFoeBody, 9);
  fFovSaved = GetProjectionFov_40DCF6();
  SetProjectionFov_40DCA3(0.80000001);
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  while ( 1 )
  {
    do
      DoPeekMessages();
    while ( timeGetTime() - dwLastTickMs <= 0x64 && fHudFade >= 0.80000001 );
    dwLastTickMs = timeGetTime();
    fDelta = GetFrameTime();
    if ( fDelta > 5.0 )
      fDelta = 5.0;
    FrameTimer_EndFrame();
    CSekBody__UpdateAnim_4019E1(pHeroBody, fDelta);
    CSekBody__UpdateAnim_4019E1(pFoeBody, fDelta);
    fScriptTime = fScriptTime + fDelta;
    UpdateVoicePlayback_458B3A(pFoeBody, pFoeKubiFrame, pArrVoiceBufs, fDelta);
    fHudFade = fDelta / 5.0 + fHudFade;
    if ( fScriptTime > 10.0 )
      break;
    fFovSwing = 1.5 - fScriptTime / 25.0;
    if ( fFovSwing < 0.89999998 )
      fFovSwing = 0.89999998;
    fSlowMoScale3 = fTimeScaleSaved / 3.0;
    SetTimerScaleFactor(fSlowMoScale3);
    SetProjectionFov_40DCA3(fFovSwing);
    HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fRelX, &fRelY, &fRelZ);
    HanaFrame_GetRelPosition(pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, arrFoeRelPos, &fFoeRelY, &fFoeRelZ);
    fCamSwingZ1 = (fFoeRelZ - fRelZ) * -0.60000002 + fRelZ;
    fCamSwingY1 = fRelY - 1.0;
    fCamSwingX1 = (arrFoeRelPos[0] - fRelX) * -0.60000002 + fRelX;
    SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fCamSwingX1, fCamSwingY1, fCamSwingZ1);
    HanaFrame_GetRelPosition(
      g_pCameraFrame,
      pFoeBody->pSekHeader_220->pFrameT3EC,
      &fCamFoeRelX,
      &fCamFoeRelY,
      &fCamFoeRelZ);
    fFoeRelXBack8 = fCamFoeRelX - 8.0;
    SetFramePosRel_4069DD(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, fFoeRelXBack8, fCamFoeRelY, fCamFoeRelZ);
    HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, 0);
    fCamSwingZ2 = (fFoeRelZ - fRelZ) * -0.60000002 + fRelZ;
    fCamSwingY2 = fRelY + 10.0;
    fCamSwingX2 = (arrFoeRelPos[0] - fRelX) * -0.60000002 + fRelX;
    SetFramePosRel_4069DD(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fCamSwingX2, fCamSwingY2, fCamSwingZ2);
    HanaFrame_GetRelPosition(
      g_pCameraFrame,
      pFoeBody->pSekHeader_220->pFrameT3EC,
      &fCamFoeRelX,
      &fCamFoeRelY,
      &fCamFoeRelZ);
    fFoeRelXBack6 = fCamFoeRelX - 6.0;
    SetFramePosRel_4069DD(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, fFoeRelXBack6, fCamFoeRelY, fCamFoeRelZ);
    HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeKubiFrame, g_pSceneFrame, 0);
  LABEL_23:
    MtxStackStartMultLocal_4074EF();
    if ( fHudFade >= 1.0 )
    {
      fHudFade = 1.0;
      FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
      FlushD3DAndTouchSurface(g_pPrimaryBuffer_4D6C88);
      g_pBackBuffer_4D6C90->BltFast(0, 0, g_pPrimaryBuffer_4D6C88, 0, 16);
      g_pRenderCtx->Clear(2);
    }
    else
    {
      g_pRenderCtx->Clear(3);
      if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
      {
        pOdsMapFrame->bAlphaDeferred_130 = 0;
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
        HanaFrame_Draw(pOdsMapFrame);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        HanaVert_DrawBatch(1);
        g_pD3DDevice_4D6C78->EndScene();
      }
    }
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
      nRectBottom = ConvertFrom640AsInt(480);
      nRectRight = ConvertFrom640AsInt(640);
      nRectTop = ConvertFrom640AsInt(0);
      nRectLeft = ConvertFrom640AsInt(0);
      SetRect(&rc, nRectLeft, nRectTop, nRectRight, nRectBottom);
      CHanaObj__SetAlpha3F6((CHanaSpriteT3F6_S1FC *)pDimOverlay, 0.30000001);
      DrawSpriteLTRB((CHanaSpriteT3F6_S1FC *)pDimOverlay, rc.left, rc.top, rc.right, rc.bottom);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
      g_pD3DDevice_4D6C78->EndScene();
    }
    if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
    {
      pOdsMapFrame->bAlphaDeferred_130 = 1;
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
      HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      HanaVert_DrawBatch(1);
      g_pD3DDevice_4D6C78->EndScene();
    }
    for ( i = 0; i < 20 && pArrSprites[i]; ++i )
    {
      fViewfinderAlpha = 1.0 - fHudFade;
      CHanaObj__SetAlpha3F6(pArrSprites[i], fViewfinderAlpha);
    }
    for ( i = 0; i < 15 && pArrHudMeters[i]; ++i )
    {
      fHudAlpha = 1.0 - fHudFade;
      CHanaObj__SetAlpha3F6(pArrHudMeters[i], fHudAlpha);
    }
    DrawDuelHealthHud_44B23B(
      (CHanaSpriteT3F6_S1FC **)pArrSprites,
      (__int64)fHeroHpCur / 100,
      (__int64)fHeroHpMax / 100,
      (__int64)fFoeHpCur / 100,
      (__int64)fFoeHpMax / 100);
    DrawClockHud_44AF5B(pArrHudMeters, 320, 16, 100 * nClockBase - dwElapsedMs / 0xA);

    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  if ( !bClearedOnce )
  {
    bClearedOnce = 1;
    g_pRenderCtx->Clear(3);
  }
  fSlowMoScale5 = fTimeScaleSaved / 5.0;
  SetTimerScaleFactor(fSlowMoScale5);
  HanaFrame_GetRelPosition(
    pHeroBody->pSekHeader_220->pFrameT3EC,
    pFoeBody->pSekHeader_220->pFrameT3EC,
    &fRelX,
    &fRelY,
    &fRelZ);
  fKubiCamZ = fRelZ + 12.0 - fZoomOffset + 6.0;
  fKubiCamY = fRelY + 18.0 - fDollyOffset / 2.0;
  fKubiCamX = fRelX + 10.0 + fDollyOffset;
  SetFramePosRel_4069DD(g_pCameraFrame, pFoeBody->pSekHeader_220->pFrameT3EC, fKubiCamX, fKubiCamY, fKubiCamZ);
  HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pFoeKubiFrame, g_pSceneFrame, 0);
  fKubiPullbackZ = -(fDollyOffset + 10.0) - (fDollyOffset + 10.0);
  SetFramePosRel_4069DD(g_pCameraFrame, g_pCameraFrame, 0.0, 0.0, fKubiPullbackZ);
  fDollyOffset = fDollyOffset + fDelta;
  fZoomOffset = fDelta / 15.0 + fZoomOffset;
  if ( fDollyOffset > 0.0 )
  {
    fDollyOffset = 0.0;
    fFovZoomIn = GetProjectionFov_40DCF6();
    fFovZoomIn = fFovZoomIn - fDelta / 25.0;
    if ( fFovZoomIn <= 0.2 )
      fFovZoomIn = 0.2;
    SetProjectionFov_40DCA3(fFovZoomIn);
  }
  v13 = fSwingTime + fDelta;
  fSwingTime = v13;
  if ( v13 <= 50.0 )
    goto LABEL_23;
  SetTimerScaleFactor(fTimeScaleSaved);
  SetProjectionFov_40DCA3(fFovSaved);
  if ( pDimOverlay )
    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pDimOverlay);
}

/**

PlayResultsCutscene_455BE2 -- results pose loop

Victory-pose showcase cycling 3 phases (arrPhaseDurations {10, 10, 900}, sentinel 99999): each phase rotates the hero
by arrPhaseAngles {-90, 45, 180} degrees around vecRotAxis (Y) to re-face the camera, cuts hero/foe/wana anims (foe back to
saved nFoeAnimItem/float_224), and after phase 2 all anims run at 3x (fAnimTimeScale = 3). Camera chases a point 18
above the hero's head (fHeroRaisedY), drifts up (fCameraDrift -10->5) then widens FOV to 1.2 (fFovWide). During t=30-90
the hero advances toward the foe at (t-30)/5 with map-collision response (fColPushBackZ/fColNudgeZ/fColPushFwdZ); the
foe (anim 5) is pushed to >=8 units from the camera (fFoeCamDist). Holding the mouse button after one release
(bBtnLatch) fast-forwards x5. Ends at t > 280, restores FOV/time scale, HanaSurface_CreateAndFade.
*/
void __cdecl hook_PlayResultsCutscene_455BE2(
  CSekBody *pHeroBody,
  CSekBody *pFoeBody,
  CSekBody **pWanaBodies,
  CHanaFrameT3EC_S354 *pMapFrame,
  CHanaSpriteT3F6_S1FC **pArrHudMeters, // arrHudClock
  CHanaSpriteT3F6_S1FC **pArrSprites, // arrViewfinder
  float fHeroHp,
  float fHeroHpMax,
  float fFoeHpCur,
  float fFoeHpMax,
  int nClockBase,
  unsigned int dwElapsedMs)
{
  double v12; // st7
  float fHeroRaisedY; // [esp+0h] [ebp-100h]
  float fHeroDroppedY; // [esp+0h] [ebp-100h]
  float fRotNegRad; // [esp+4h] [ebp-FCh]
  float fRotRad; // [esp+4h] [ebp-FCh]
  float fColPushBackZ; // [esp+4h] [ebp-FCh]
  float fColNudgeZ; // [esp+4h] [ebp-FCh]
  float fColPushFwdZ; // [esp+4h] [ebp-FCh]
  float fWanaDelta; // [esp+10h] [ebp-F0h]
  float fFoeDelta; // [esp+14h] [ebp-ECh]
  float fHeroDelta; // [esp+18h] [ebp-E8h]
  float dirUpY; // [esp+1Ch] [ebp-E4h] BYREF
  float fFoeRelZ; // [esp+20h] [ebp-E0h] BYREF
  float dirForwardZ; // [esp+24h] [ebp-DCh] BYREF
  float dirUpX; // [esp+28h] [ebp-D8h] BYREF
  float fFoeCamDist; // [esp+2Ch] [ebp-D4h]
  float fFoeRelY; // [esp+30h] [ebp-D0h] BYREF
  float dirForwardY; // [esp+34h] [ebp-CCh] BYREF
  float fFoeRelX; // [esp+38h] [ebp-C8h] BYREF
  float dirForwardX; // [esp+3Ch] [ebp-C4h] BYREF
  float dirUpZ; // [esp+40h] [ebp-C0h] BYREF
  float arrPhaseDurations[10]; // [esp+44h] [ebp-BCh] BYREF
  float fSavedFoeFloat224; // [esp+6Ch] [ebp-94h]
  float fTimeScale; // [esp+70h] [ebp-90h]
  __int16 bBtnLatch; // [esp+74h] [ebp-8Ch]
  int iPhase; // [esp+78h] [ebp-88h]
  int nUnused; // [esp+7Ch] [ebp-84h]
  float fScriptTime; // [esp+80h] [ebp-80h]
  int i; // [esp+84h] [ebp-7Ch]
  int yMouse; // [esp+88h] [ebp-78h] BYREF
  float fHeroFoeDist; // [esp+8Ch] [ebp-74h]
  float fHeroZ; // [esp+90h] [ebp-70h] BYREF
  int bHeroAnimDone; // [esp+94h] [ebp-6Ch]
  int xMouse; // [esp+98h] [ebp-68h] BYREF
  float fHeroY; // [esp+9Ch] [ebp-64h] BYREF
  float FrameTime; // [esp+A0h] [ebp-60h]
  float fHeroX; // [esp+A4h] [ebp-5Ch] BYREF
  float fOldFov; // [esp+A8h] [ebp-58h]
  float fRelZ; // [esp+ACh] [ebp-54h] BYREF
  float fFovWide; // [esp+B0h] [ebp-50h]
  float fRelY; // [esp+B4h] [ebp-4Ch] BYREF
  D3DXVECTOR3 vecRotAxis; // [esp+B8h] [ebp-48h] BYREF
  float fRelX; // [esp+C4h] [ebp-3Ch] BYREF
  int nFoeAnimItem; // [esp+C8h] [ebp-38h]
  __int16 btnMouse; // [esp+CCh] [ebp-34h] BYREF
  float fAnimTimeScale; // [esp+D0h] [ebp-30h]
  float fCameraDrift; // [esp+D4h] [ebp-2Ch]
  float arrPhaseAngles[10]; // [esp+D8h] [ebp-28h] BYREF

  ONCELOG("[HNH:SUB] PlayResultsCutscene_455BE2\n");

  fCameraDrift = -10.0;
  fScriptTime = 0.0;
  fAnimTimeScale = 1.0;
  bHeroAnimDone = 0;
  bBtnLatch = 0;
  iPhase = 0;
  nUnused = 0;
  arrPhaseDurations[0] = 10.0;
  arrPhaseDurations[1] = 10.0;
  arrPhaseDurations[2] = 900.0;
  arrPhaseDurations[3] = 99999.0;
  memset(&arrPhaseDurations[4], 0, 0x18u);
  arrPhaseAngles[0] = -90.0;
  arrPhaseAngles[1] = 45.0;
  arrPhaseAngles[2] = 180.0;
  arrPhaseAngles[3] = 99999.0;
  memset(&arrPhaseAngles[4], 0, 0x18u);
  vecRotAxis.x = 0.0;
  vecRotAxis.y = 1.0;
  vecRotAxis.z = 0.0;
  if ( pHeroBody->field_200 != '\t' && pHeroBody->field_200 != '\b' )
    SekBody__UpdateTo2_401647(pHeroBody, 15);
  nFoeAnimItem = pFoeBody->field_200;
  fSavedFoeFloat224 = pFoeBody->float_224;
  fOldFov = GetProjectionFov_40DCF6();
  SetProjectionFov_40DCA3(0.80000001);
  fTimeScale = GetTimerScaleFactor();
  SetTimerScaleFactor(10.0);
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  while ( 1 )
  {
    DoPeekMessages();
    FrameTime = GetFrameTime();
    if ( FrameTime > 5.0 )
      FrameTime = 5.0;
    FrameTimer_EndFrame();
    GetCursorPos_4811FA(&xMouse, &yMouse, &btnMouse);
    if ( btnMouse )
    {
      if ( bBtnLatch == 1 )
        FrameTime = FrameTime * 5.0;
    }
    else if ( !bBtnLatch )
    {
      bBtnLatch = 1;
    }
    if ( !bHeroAnimDone )
    {
      fHeroDelta = FrameTime / fAnimTimeScale;
      if ( CSekBody__UpdateAnim_4019E1(pHeroBody, fHeroDelta) )
      {
        fAnimTimeScale = 1.0;
        bHeroAnimDone = 1;
      }
    }
    fFoeDelta = FrameTime / fAnimTimeScale;
    CSekBody__UpdateAnim_4019E1(pFoeBody, fFoeDelta);
    if ( fScriptTime < 90.0 )
    {
      for ( i = 0; i < 8; ++i )
      {
        if ( pWanaBodies[i] )
        {
          if ( fScriptTime > 30.0 )
            SekBody__UpdateTo2_401647(pWanaBodies[i], 0);
          fWanaDelta = FrameTime / fAnimTimeScale;
          CSekBody__UpdateAnim_4019E1(pWanaBodies[i], fWanaDelta);
        }
      }
    }
    fRotNegRad = -arrPhaseAngles[iPhase] * 3.1400001 / 180.0;
    Frame_RotateMatrixAxis_4070AB(pHeroBody->pSekHeader_220->pFrameT3EC, 1, &vecRotAxis, fRotNegRad);
    HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fRelX, &fRelY, &fRelZ);
    fHeroRaisedY = fRelY + 18.0;
    SetFramePosRel_4069DD(
      pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      fRelX,
      fHeroRaisedY,
      fRelZ);
    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, FrameTime, 1);
    HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, 0);
    fHeroDroppedY = fRelY - fCameraDrift - 13.0;
    SetFramePosRel_4069DD(
      pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      fRelX,
      fHeroDroppedY,
      fRelZ);
    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, FrameTime, 1);
    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fRelX, fRelY, fRelZ);
    fRotRad = arrPhaseAngles[iPhase] * 3.1400001 / 180.0;
    Frame_RotateMatrixAxis_4070AB(pHeroBody->pSekHeader_220->pFrameT3EC, 1, &vecRotAxis, fRotRad);
    fCameraDrift = FrameTime / 13.0 + fCameraDrift;
    if ( fCameraDrift > 5.0 )
    {
      fCameraDrift = 5.0;
      fFovWide = GetProjectionFov_40DCF6();
      fFovWide = FrameTime / 80.0 + fFovWide;
      if ( fFovWide >= 1.2 )
        fFovWide = 1.2;
      SetProjectionFov_40DCA3(fFovWide);
    }
    v12 = fScriptTime + FrameTime;
    fScriptTime = v12;
    if ( v12 > 280.0 )
      break;
    if ( fScriptTime > 30.0 && fScriptTime < 90.0 )
    {
      nFoeAnimItem = 5;
      SekBody__UpdateTo2_401647(pFoeBody, 5);
      HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
      SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC, pHeroBody->pSekHeader_220->pFrameT3EC, 0.0, 0.0, -4.5);
      HanaFrame_GetRelPosition(
        pFoeBody->pSekHeader_220->pFrameT3EC,
        pHeroBody->pSekHeader_220->pFrameT3EC,
        &fRelX,
        &fRelY,
        &fRelZ);
      fHeroFoeDist = sqrt(fRelX * fRelX + fRelZ * fRelZ);
      if ( (fScriptTime - 30.0) / 5.0 > fHeroFoeDist && fHeroFoeDist != 0.0 )
      {
        fRelX = (fScriptTime - 30.0) / 5.0 / fHeroFoeDist * fRelX;
        fRelZ = (fScriptTime - 30.0) / 5.0 / fHeroFoeDist * fRelZ;
        SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, pHeroBody->pSekHeader_220->pFrameT3EC, fRelX, fRelY, fRelZ);
      }
      SetFramePosRel_4069DD(
        pHeroBody->pSekHeader_220->pFrameT3EC,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        fHeroX,
        fHeroY,
        fHeroZ);
      if ( CheckFrameCollision_443CC5(pHeroBody->pSekHeader_220->pFrameT3EC, pMapFrame, 0) )
      {
        fColPushBackZ = -(fScriptTime - 30.0) / 4.0;
        SetFramePosRel_4069DD(
          pHeroBody->pSekHeader_220->pFrameT3EC,
          pHeroBody->pSekHeader_220->pFrameT3EC,
          0.0,
          0.0,
          fColPushBackZ);
        if ( !CheckFrameCollision_443CC5(pHeroBody->pSekHeader_220->pFrameT3EC, pMapFrame, 0) )
        {
          fColNudgeZ = FrameTime / 4.0;
          SetFramePosRel_4069DD(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            pHeroBody->pSekHeader_220->pFrameT3EC,
            0.0,
            0.0,
            fColNudgeZ);
        }
        fColPushFwdZ = (fScriptTime - 30.0) / 4.0;
        SetFramePosRel_4069DD(
          pHeroBody->pSekHeader_220->pFrameT3EC,
          pHeroBody->pSekHeader_220->pFrameT3EC,
          0.0,
          0.0,
          fColPushFwdZ);
      }
    }
    if ( fScriptTime > (double)arrPhaseDurations[iPhase] )
    {
      fCameraDrift = -10.0;
      fScriptTime = 0.0;
      if ( ++iPhase > 1 )
        fAnimTimeScale = 3.0;
      CSekBody__CutToAnim_4018E1(pHeroBody, pHeroBody->field_200);
      pHeroBody->float_224 = 1.0;
      CSekBody__CutToAnim_4018E1(pFoeBody, nFoeAnimItem);
      pFoeBody->float_224 = fSavedFoeFloat224;
      for ( i = 0; i < 8; ++i )
      {
        if ( pWanaBodies[i] )
        {
          CSekBody__CutToAnim_4018E1(pWanaBodies[i], pWanaBodies[i]->field_200);
          pWanaBodies[i]->float_224 = 1.0;
        }
      }
    }
    if ( fScriptTime > 50.0 )
    {
      CalculateLocalRotationMatrix(
        g_pCameraFrame,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        &dirForwardX,
        &dirForwardY,
        &dirForwardZ,
        &dirUpX,
        &dirUpY,
        &dirUpZ);
      UpdateBoneLookAtTransform(g_pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
      HanaFrame_GetRelPosition(pFoeBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, &fFoeRelX, &fFoeRelY, &fFoeRelZ);
      fFoeCamDist = sqrt(fFoeRelX * fFoeRelX + fFoeRelZ * fFoeRelZ);
      if ( fFoeCamDist < 8.0 && fFoeCamDist != 0.0 )
      {
        fFoeRelX = 8.0 / fFoeCamDist * fFoeRelX;
        fFoeRelZ = 8.0 / fFoeCamDist * fFoeRelZ;
        SetFramePosRel_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, fFoeRelX, fFoeRelY, fFoeRelZ);
      }
      UpdateBoneLookAtTransform(
        g_pCameraFrame,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        dirForwardX,
        dirForwardY,
        dirForwardZ,
        dirUpX,
        dirUpY,
        dirUpZ);
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
    DrawDuelHealthHud_44B23B(
      (CHanaSpriteT3F6_S1FC **)pArrSprites,
      (__int64)fHeroHp / 100,
      (__int64)fHeroHpMax / 100,
      (__int64)fFoeHpCur / 100,
      (__int64)fFoeHpMax / 100);
    DrawClockHud_44AF5B((CHanaSpriteT3F6_S1FC **)pArrHudMeters, 320, 16, 100 * nClockBase - dwElapsedMs / 0xA);

    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  SetTimerScaleFactor(fTimeScale);
  SetProjectionFov_40DCA3(fOldFov);
  HanaSurface_CreateAndFade();
}


void register_PlayHunting_44C38D(int cfg) {
  if (cfg == 1) {
    PlayHunting_44C38D = hook_PlayHunting_44C38D;
    UpdateFoeAI_446A38 = hook_UpdateFoeAI_446A38;
    WanaAttack_Close_4535DE = hook_WanaAttack_Close_4535DE;
    WanaAttack_Knockback_4536F6 = hook_WanaAttack_Knockback_4536F6;
    WanaAttack_Charged_4539D1 = hook_WanaAttack_Charged_4539D1;
    WanaAttack_Projectile_453B92 = hook_WanaAttack_Projectile_453B92;
    IsFoeWithinHeroRange_448B78 = hook_IsFoeWithinHeroRange_448B78;
    RenderDualBodyScene_456807 = hook_RenderDualBodyScene_456807;
    StepFoeTurn_457EE0 = hook_StepFoeTurn_457EE0;
    StepFoeSpin_45804C = hook_StepFoeSpin_45804C;
    StepWanaProjectile_4581B0 = hook_StepWanaProjectile_4581B0;
    DrawSprite3D = hook_DrawSprite3D;

    PlayResultsCutscene_455BE2 = hook_PlayResultsCutscene_455BE2;
    PlayVictoryCutscene_455183 = hook_PlayVictoryCutscene_455183;
    DrawDuelHealthHud_44B23B = hook_DrawDuelHealthHud_44B23B;
    DrawFadeOutSpark_45669F = hook_DrawFadeOutSpark_45669F;
    BindSceneActors_448C72 = hook_BindSceneActors_448C72;
    UpdateCameraFollowDuel_44C028 = hook_UpdateCameraFollowDuel_44C028;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x44C38D, hook_PlayHunting_44C38D, "PlayHunting_44C38D"); // [OK]
  hnh_Hook(0x456807, hook_RenderDualBodyScene_456807, "RenderDualBodyScene_456807"); // [OK]
  hnh_Hook(0x414344, hook_DrawSprite3D, "DrawSprite3D_414344"); // [OK]

  hnh_Hook(0x446a38, hook_UpdateFoeAI_446A38, "UpdateFoeAI_446A38"); // [OK]
  hnh_Hook(0x4535de, hook_WanaAttack_Close_4535DE, "WanaAttack_Close_4535DE"); // [OK]
  hnh_Hook(0x448b78, hook_IsFoeWithinHeroRange_448B78, "IsFoeWithinHeroRange_448B78"); // [OK]
  hnh_Hook(0x4536f6, hook_WanaAttack_Knockback_4536F6, "WanaAttack_Knockback_4536F6"); // [OK]
  hnh_Hook(0x45669f, hook_DrawFadeOutSpark_45669F, "DrawFadeOutSpark_45669F"); // [OK]
  hnh_Hook(0x448c72, hook_BindSceneActors_448C72, "BindSceneActors_448C72"); // [OK]
  hnh_Hook(0x44b23b, hook_DrawDuelHealthHud_44B23B, "DrawDuelHealthHud_44B23B"); // [OK]
  hnh_Hook(0x44c028, hook_UpdateCameraFollowDuel_44C028, "UpdateCameraFollowDuel_44C028"); // [OK]

  hnh_Hook(0x45804c, hook_StepFoeSpin_45804C, "StepFoeSpin_45804C"); // [OK]
  hnh_Hook(0x455be2, hook_PlayResultsCutscene_455BE2, "PlayResultsCutscene_455BE2"); // [OK]
  hnh_Hook(0x453b92, hook_WanaAttack_Projectile_453B92, "WanaAttack_Projectile_453B92"); // [OK]
  hnh_Hook(0x4581b0, hook_StepWanaProjectile_4581B0, "StepWanaProjectile_4581B0"); // [OK]
  hnh_Hook(0x457ee0, hook_StepFoeTurn_457EE0, "StepFoeTurn_457EE0"); // [OK]
  hnh_Hook(0x455183, hook_PlayVictoryCutscene_455183, "PlayVictoryCutscene_455183"); // [OK]
  hnh_Hook(0x4539d1, hook_WanaAttack_Charged_4539D1, "WanaAttack_Charged_4539D1"); // [OK]

  hnh_HookOn();
}
