#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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
// follow, per-duel foe-aggro max arrFoeAggroMax[12] (20000..45000), CutToAnim
// (1)/PlayAnim on hero+foe, CutToAnim(0) on the wanas; the foe face frame is
// found by name "_04_kao" (@0x4B5220). Inner frame loop: DoPeekMessages +
// GetFrameTime (fDelta capped 5.0, divided by 4 while a cinematic is queued);
// menu key snapshots the live scene state, restores the entry state, stops
// the voices and opens SystemMenu on the BGM (>= 1 exits, else live state is
// put back); cinematic branch plays PlayCinematicSequence (cut-in; wanas are
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
// HanaVert_DrawBatch), DrawRecViewfinder (hero HP/max, foe aggro/max bars),
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

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayHunting_44C38D)(int nDuelIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x44C38D);

// --- not yet extracted; pinned here call-only ---

typedef double(__cdecl *WanaAttackFn)(CSekBody *pWanaBody, CSekBody *pHeroBody,
    float *pfHeroHp, float fDeltaTime, float fAttackParam,
    CHanaFrameT3EC_S354 *pMapFrame);



double (__cdecl*UpdateFoeAI_446A38)(CSekBody *pFoeBody, CSekBody *pHeroBody,
    CHanaFrameT3EC_S354 *pMapFrame, int *pOccGrid, float fDeltaTime,
    float *pfAggro, float *pfHeroHp, float fAggroMax, int nDuelIndex) =
    reinterpret_cast<double(__cdecl*)(CSekBody *, CSekBody *,
      CHanaFrameT3EC_S354 *, int *, float, float *, float *, float, int)>(0x446A38);

void (__cdecl*UpdateVoicePlayback_458B3A)(CSekBody *pFoeBody,
    CHanaFrameRootT3EC_S2D4 *pFoeTalkFrame, CSoundStruct_8 **arrVoiceBufs,
    float fDeltaTime) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, CHanaFrameRootT3EC_S2D4 *,
      CSoundStruct_8 **, float)>(0x458B3A);

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

void (__cdecl*PlayCinematicSequence_45460A)(CHanaSceneT458_S504 *pOdsScene,
    CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame,
    CHanaSpriteT3F6_S1FC **pArrHudMeters, CHanaSpriteT3F6_S1FC **pArrSprites,
    float *pfHeroHp, float fFieldC4, float *pfAggro, float fAggroMax,
    int *pnClockBase, unsigned int *pdwElapsedMs, int dwClockBase2,
    CSoundStruct_8 **arrVoiceBufs, CHanaSpriteT3F6_S1FC *pMaskBmp) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
      CSekBody *, CHanaFrameT3EC_S354 *, CHanaSpriteT3F6_S1FC **,
      CHanaSpriteT3F6_S1FC **, float *, float, float *, float, int *, unsigned int *,
      int, CSoundStruct_8 **, CHanaSpriteT3F6_S1FC *)>(0x45460A);

void (__cdecl*RenderDualBodyScene_456807)(CHanaSceneT458_S504 *pScene,
    CSekBody *pBodyA, CSekBody *pBodyB, CHanaSpriteT3F6_S1FC **pSprites,
    CHanaSpriteT3F6_S1FC **pSounds) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
      CSekBody *, CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **)>(0x456807);

int (__cdecl*PlayResultsCutscene_455BE2)(CSekBody *pHeroBody, CSekBody *pFoeBody,
    CSekBody **pWanaBodies, CHanaFrameT3EC_S354 *pMapFrame, int *pHudClock,
    int *pViewfinder, float fGauge, float fFieldC4, float fAggro,
    float fTargetStart, int nClockBase, unsigned int dwElapsedMs) =
    reinterpret_cast<int(__cdecl*)(CSekBody *, CSekBody *, CSekBody **,
      CHanaFrameT3EC_S354 *, int *, int *, float, float, float, float, int,
      unsigned int)>(0x455BE2);

int (__cdecl*PlayVictoryCutscene_455183)(CHanaSceneT458_S504 *pOdsScene,
    CSekBody *pHeroBody, CSekBody *pFoeBody, CHanaFrameT3EC_S354 *pMapFrame,
    int *pHudSprites, int *pViewfinder, float fGauge, float fFieldC4,
    float fAggro, float fTargetStart, int nClockBase, unsigned int dwElapsedMs,
    int *pVoiceMem) =
    reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, CSekBody *,
      CSekBody *, CHanaFrameT3EC_S354 *, int *, int *, float, float, float,
      float, int, unsigned int, int *)>(0x455183);

void *(__cdecl*DrawRecViewfinder_44B23B)(CHanaSpriteT3F6_S1FC **pSprites,
    int nHeroHp, int nHeroHpMax, int nAggro, int nAggroMax) =
    reinterpret_cast<void *(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, int, int,
      int)>(0x44B23B);

void (__cdecl*DrawFadeOutSpark_45669F)(CHanaSpriteT3F6_S1FC **arrSparkSprites,
    float fDeltaTime) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, float)>(0x45669F);

int (__cdecl*BuildMapCollisionGrid_445B12)(CHanaFrameT3EC_S354 *pMapFrame,
    int **ppOutGrid) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameT3EC_S354 *, int **)>(0x445B12);

unsigned int (__cdecl*BindSceneActors_448C72)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 **pActorFrames, unsigned int nCount) =
    reinterpret_cast<unsigned int(__cdecl*)(CHanaSceneT458_S504 *,
      CHanaFrameT3EC_S354 **, unsigned int)>(0x448C72);

int (__cdecl*UpdateCameraFollow_44C028)(CSekBody *pHeroBody, CSekBody *pFoeBody,
    float fDeltaTime) =
    reinterpret_cast<int(__cdecl*)(CSekBody *, CSekBody *, float)>(0x44C028);


// === PINNED GLOBALS ===

// UI/menu SFX slots 0..9 (0 Start, 1 GameOver, 2 On, 3 per-duel act\HitN)
LPDIRECTSOUNDBUFFER (&g_pArrSndBuf_5244AC)[10] =
    *(LPDIRECTSOUNDBUFFER(*)[10])0x5244AC;
// foe 3D hit SFX (each foot wav loaded twice, one per side)
CSoundStruct_8 *(&g_pFoeHitSfxB_5244D4)[4] = *(CSoundStruct_8 *(*)[4])0x5244D4;
CSoundStruct_8 *(&g_pFoeHitSfxE_5244E4)[5] = *(CSoundStruct_8 *(*)[5])0x5244E4;

// === FUNCTIONS ===

int __cdecl hook_PlayHunting_44C38D(int nDuelIndex)
{
    CSceneState sceneStateLive;         // live copy saved around SystemMenu
    CSceneState sceneMenuCopy;          // SystemMenu by-value arg / scratch
    CSceneState sceneStateEntry;        // entry snapshot of g_sceneState
    CHanaFrameT3EC_S354 *pActorFrames[10]; // hero + foe + wanas
    CHanaAnimSetT3F1_S150 *pMapOdfAnimSet;  // map ODF throw-away anim set
    CHanaAnimSetT3F1_S150 *pMapObjAnimSet;  // map-object ODF ditto
    String50 szArrWanaSeks[200];        // wana\\<sek>.sek base names
    String50 azArrWanaOdfs[200];        // wana\\<odf>.ODF base names
    String50 arrFoeAtkWavs[15];         // per-duel g_pArrSndBuf[3] override
    String50 arrBgmWavs[15];            // s010/s006/s011.wav
    String50 arrUiWavNames[10];         // Start/GameOver/On + "" filler
    String50 arrFootWavNames[5];        // foot4/2/3/5
    String50 arrVoiceWavNames[140];     // 5 acts x 20 slots act\A0X_NN.wav
    String50 arrDuelEntries[20][4];     // 12 duel records (foe/map/ods)
    char arrFoeBmpNames[6][2][25];      // foe face bmp/mask pairs per act
    DWORD arrChapterActMap[20];         // duel -> act
    DWORD arrWanaAtkTypes[160];         // per (duel,wana) attack fn index
    float arrWanaAtkParams[160];        // per (duel,wana) attack param
    float arrFoeAggroMax[20];           // per-duel foe aggro max
    WanaAttackFn arrWanaAtkFns[4];      // Close/Knockback/Charged/Projectile
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
    int *pOccGrid;                       // map-object collision grid
    LPDIRECTSOUNDBUFFER pBgmBuf;
    CHAR pszFileName[256];
    CHAR String[256];
    float fHeroX, fHeroY, fHeroZ;        // hero pos saved between rounds
    float dirForwardX, dirForwardY, dirForwardZ;
    float dirUpX, dirUpY, dirUpZ;        // hero rot saved between rounds
    float x, y, z;                       // foe pos saved between rounds
    float pXpos, pYpos, pZpos;           // foe rot saved between rounds
    float pXYrot, pYXrot, pZXrot;
    float pfHeroHP;
    float fFieldC4;
    float pFAggro;
    float fAggroMax;
    float fActTimer;
    float pfExecTime;
    float fDelta = 0.0;
    double updated;                      // UpdateFoeAI / wana-attack return
    double fActTimerNew;
    unsigned int nCount;
    unsigned int dwElapsedMs;
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
    arrWanaAtkFns[0] = WanaAttack_Close_4535DE;
    arrWanaAtkFns[1] = WanaAttack_Knockback_4536F6;
    arrWanaAtkFns[2] = WanaAttack_Charged_4539D1;
    arrWanaAtkFns[3] = WanaAttack_Projectile_453B92;

    // --- wana attack type per (duel, wana): rows of 8, duels 0..11
    // (memset'd rows are all-Close; rows past duel 11 are dead data)
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

    // --- wana attack param per (duel, wana)
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

    // --- wana sek/odf base names, 8 per duel (rows 8*nDuelIndex..+7).
    // Untouched rows are the runtime-empty "" rows (0xFF .data filler) and
    // rows past 111 are dead data, all zero here.
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

    // --- duel records: foe body / map ODF / map-object ODF / ODS scene
    // (rows 12..19 all zero)
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

    // --- duel -> act (rows 12..19 all zero)
    arrChapterActMap[0] = 0;   arrChapterActMap[1] = 0;
    arrChapterActMap[2] = 1;   arrChapterActMap[3] = 1;
    arrChapterActMap[4] = 2;   arrChapterActMap[5] = 2;
    arrChapterActMap[6] = 3;   arrChapterActMap[7] = 3;
    arrChapterActMap[8] = 3;   arrChapterActMap[9] = 4;
    arrChapterActMap[10] = 4;  arrChapterActMap[11] = 4;
    memset(&arrChapterActMap[12], 0, 8 * 4);

    // --- per-duel foe-attack SE for g_pArrSndBuf[3] (rows 12..14 zero)
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

    // --- UI wav names (3..9 runtime-empty ""; loop below stops at [3])
    strcpy(arrUiWavNames[0].str, "Start");
    strcpy(arrUiWavNames[1].str, "GameOver");
    strcpy(arrUiWavNames[2].str, "On");
    memset(&arrUiWavNames[3], 0, 7 * 50);

    // --- foot wavs for the 3D foe-hit SFX ([4] zero)
    strcpy(arrFootWavNames[0].str, "foot4");
    strcpy(arrFootWavNames[1].str, "foot2");
    strcpy(arrFootWavNames[2].str, "foot3");
    strcpy(arrFootWavNames[3].str, "foot5");
    memset(&arrFootWavNames[4], 0, 50);

    // --- BGM names ([3..14] zero)
    strcpy(arrBgmWavs[0].str, "s010.wav");
    strcpy(arrBgmWavs[1].str, "s006.wav");
    strcpy(arrBgmWavs[2].str, "s011.wav");
    memset(&arrBgmWavs[3], 0, 12 * 50);

    // --- voice wavs: act k holds act\A0k_00..12.wav in a 20-slot block
    // (13..19 of each block and the whole blocks 5..6 stay empty)
    memset(arrVoiceWavNames, 0, sizeof(arrVoiceWavNames));
    for (i = 0; i < 13; ++i)
        wsprintfA(arrVoiceWavNames[i].str, "act\\A00_%02d.wav", i);
    for (i = 0; i < 13; ++i)
        wsprintfA(arrVoiceWavNames[20 + i].str, "act\\A01_%02d.wav", i);
    for (i = 0; i < 13; ++i)
        wsprintfA(arrVoiceWavNames[40 + i].str, "act\\A02_%02d.wav", i);
    for (i = 0; i < 13; ++i)
        wsprintfA(arrVoiceWavNames[60 + i].str, "act\\A03_%02d.wav", i);
    for (i = 0; i < 13; ++i)
        wsprintfA(arrVoiceWavNames[80 + i].str, "act\\A04_%02d.wav", i);

    // --- foe face bmp/mask pairs per act ([5] runtime-empty)
    // (.data copies spelled out; [4][1] was a literal in the original)
    strcpy(arrFoeBmpNames[0][0], "run\\o2a.bmp");  strcpy(arrFoeBmpNames[0][1], "run\\o2b.bmp");
    strcpy(arrFoeBmpNames[1][0], "run\\o5a.bmp");  strcpy(arrFoeBmpNames[1][1], "run\\o5b.bmp");
    strcpy(arrFoeBmpNames[2][0], "run\\o4a.bmp");  strcpy(arrFoeBmpNames[2][1], "run\\o4b.bmp");
    strcpy(arrFoeBmpNames[3][0], "run\\o3a.bmp");  strcpy(arrFoeBmpNames[3][1], "run\\o3b.bmp");
    strcpy(arrFoeBmpNames[4][0], "run\\o1a.bmp");  strcpy(arrFoeBmpNames[4][1], "run\\o1b.bmp");
    arrFoeBmpNames[5][0][0] = 0;
    arrFoeBmpNames[5][1][0] = 0;

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
    fFieldC4 = g_sceneState_52F7E0.protMaximumHP_C4;
    pfHeroHP = g_sceneState_52F7E0.protCurrentHP_C0;
    fActTimer = 0.0f;
    fAggroMax = 99.0f;
    pFAggro = 99.0f;
    arrFoeAggroMax[0] = 20000.0f;   arrFoeAggroMax[1] = 20000.0f;
    arrFoeAggroMax[2] = 40000.0f;   arrFoeAggroMax[3] = 40000.0f;
    arrFoeAggroMax[4] = 35000.0f;   arrFoeAggroMax[5] = 35000.0f;
    arrFoeAggroMax[6] = 45000.0f;   arrFoeAggroMax[7] = 45000.0f;
    arrFoeAggroMax[8] = 45000.0f;   arrFoeAggroMax[9] = 40000.0f;
    arrFoeAggroMax[10] = 40000.0f;  arrFoeAggroMax[11] = 40000.0f;
    memset(&arrFoeAggroMax[12], 0, 8 * 4);
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
    for (i = 0; i < 9 && arrUiWavNames[i].str[0]; ++i)
    {
        wsprintfA(pszFileName, "%s.wav", arrUiWavNames[i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
        g_pArrSndBuf_5244AC[i] = LoadWav_422E7B(pszFileName);
    }
    wsprintfA(pszFileName, "%s.wav", arrFoeAtkWavs[nDuelIndex].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1009);          // SE dir
    g_pArrSndBuf_5244AC[3] = LoadWav_422E7B(pszFileName);
    for (i = 0; i < 4; ++i)
    {
        wsprintfA(pszFileName, "%s.wav", arrFootWavNames[i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
        g_pFoeHitSfxE_5244E4[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
        g_pFoeHitSfxB_5244D4[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
    }
    for (i = 0; i < 20; ++i)
    {
        wsprintfA(pszFileName, "%s",
                  arrVoiceWavNames[20 * arrChapterActMap[nDuelIndex] + i].str);
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
    wsprintfA(pszFileName, "%s", arrBgmWavs[i].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1006);          // WAV dir
    pBgmBuf = LoadWav_422E7B(pszFileName);
    if (pBgmBuf)
    {
        // clamp table[aviVolume] - 1000 dB to [-10000, 0]
        if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) >= -10000)
        {
            if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) <= 0)
                pBgmBuf->SetVolume((__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0));
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
    wsprintfA(pszFileName, "%s.sek", arrDuelEntries[nDuelIndex][0].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    wsprintfA(String, "%s.odf", arrDuelEntries[nDuelIndex][0].str);
    CHanaObj__GetRealPath_486D9A(String, 1005);               // ODF dir
    if (!LoadSekAndOdf_4445DD(pszFileName, String, &pFoeBody))
        return -1;
    for (i = 0; i < 8 && szArrWanaSeks[8 * nDuelIndex + i].str[0]; ++i)
    {
        wsprintfA(pszFileName, "wana\\%s.sek", szArrWanaSeks[8 * nDuelIndex + i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1005);      // ODF dir
        wsprintfA(String, "wana\\%s.ODF", azArrWanaOdfs[8 * nDuelIndex + i].str);
        CHanaObj__GetRealPath_486D9A(String, 1005);           // ODF dir
        if (!LoadSekAndOdf_4445DD(pszFileName, String, &pWanaBodies[i]))
            return -1;
    }

    // map + map-object ODF frames, collision grid, ODS scene
    pMapOdfAnimSet = 0;
    wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][1].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    if (!ParseODFFile_441F19(pszFileName, &pMapFrame, &pMapOdfAnimSet))
        return -1;
    if (pMapOdfAnimSet)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130 *)pMapOdfAnimSet);
        pMapOdfAnimSet = 0;
    }
    AttachChild_4055FD(g_pSceneFrame, pMapFrame);
    MtxMult_4069DD(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
    UpdateBoneLookAtTransform(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                              0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    BuildMapSubFrames_443DD5(pMapFrame);

    pMapObjAnimSet = 0;
    wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][2].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    if (!ParseODFFile_441F19(pszFileName, &pMapObjFrame, &pMapObjAnimSet))
        return -1;
    if (pMapObjAnimSet)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130 *)pMapObjAnimSet);
        pMapObjAnimSet = 0;
    }
    AttachChild_4055FD(g_pSceneFrame, pMapObjFrame);
    MtxMult_4069DD(pMapObjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
    UpdateBoneLookAtTransform(pMapObjFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                              0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    BuildMapSubFrames_443DD5(pMapObjFrame);
    BuildMapCollisionGrid_445B12(pMapObjFrame, &pOccGrid);

    wsprintfA(pszFileName, arrDuelEntries[nDuelIndex][3].str);
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
        if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] >= -10000)
        {
            if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] <= 0)
                g_pArrSndBuf_5244AC[0]->SetVolume((__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266]);
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
        if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] >= -10000)
        {
            if ((int)(__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] <= 0)
                g_pArrSndBuf_5244AC[2]->SetVolume((__int64)g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266]);
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
        StopAndRestartThread(1);
        Sleep(0);
        MtxMult_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
                       (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
        UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC,
                (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                dirForwardX, dirForwardY, dirForwardZ, dirUpX, dirUpY, dirUpZ);
        MtxMult_4069DD(pFoeBody->pSekHeader_220->pFrameT3EC,
                       (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, z);
        UpdateBoneLookAtTransform(pFoeBody->pSekHeader_220->pFrameT3EC,
                (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                pXpos, pYpos, pZpos, pXYrot, pYXrot, pZXrot);
        UpdateCameraFollow_445525(pHeroBody->pSekHeader_220->pFrameT3EC,
                                  g_pCameraFrame, pMapFrame, fDelta, 1);
        fAggroMax = arrFoeAggroMax[nDuelIndex];
        pFAggro = fAggroMax;
        fActTimer = 0.0f;
        pfExecTime = 0.0f;
        bActing = 0;
        bCinematic = 0;
        dwElapsedMs = 0;
        g_dwBattleMode_52ED04 = -1;
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
        fFieldC4 = g_sceneState_52F7E0.protMaximumHP_C4;
        pfHeroHP = g_sceneState_52F7E0.protCurrentHP_C0;
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
                StopAndRestartThread(0);
                sceneMenuCopy = sceneStateLive;
                nSysMenuResult = SystemMenu_4655AB(pBgmBuf, -1000, 0, pfHeroHP,
                                                   fFieldC4, sceneMenuCopy);
                StopAndRestartThread(1);
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
                PlayCinematicSequence_45460A(
                        pOdsScene, pHeroBody, pFoeBody, pMapFrame,
                        pArrHudMeters, pArrSprites,
                        &pfHeroHP, fFieldC4, &pFAggro, fAggroMax,
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
                                                    &pfHeroHP, fFieldC4);
                    fActTimer = 0.0f;
                }
                if (pHeroBody->field_200 <= 6 && g_btoggleC_530268)
                {
                    UpdateCameraFollow_44C028(pHeroBody, pFoeBody, fDelta);
                }
                else
                {
                    pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
                    UpdateCameraFollow_445525(pHeroBody->pSekHeader_220->pFrameT3EC,
                                              g_pCameraFrame, pMapFrame, fDelta, 0);
                }
                updated = UpdateFoeAI_446A38(pFoeBody, pHeroBody, pMapFrame,
                                             pOccGrid, fDelta, &pFAggro, &pfHeroHP,
                                             fAggroMax, nDuelIndex);
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
                        updated = arrWanaAtkFns[arrWanaAtkTypes[8 * nDuelIndex + i]](
                                pWanaBodies[i], pHeroBody, &pfHeroHP, fDelta,
                                arrWanaAtkParams[8 * nDuelIndex + i], pMapFrame);
                        fActTimer = (float)(updated + fActTimer);
                    }
                }
                if (bActing && g_dwBattleMode_52ED04 >= 0)
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
            g_pRenderCtx->Clear(3);
            if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
            {
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
                HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                HanaVert_DrawBatch(1);
                g_pD3DDevice_4D6C78->EndScene();
            }
            DrawRecViewfinder_44B23B(pArrSprites,
                    (int)((__int64)pfHeroHP / 100),
                    (int)((__int64)fFieldC4 / 100),
                    (int)((__int64)pFAggro / 100),
                    (int)((__int64)fAggroMax / 100));
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
        while ((100 * nClockBase - (int)dwElapsedMs / 10 > 0 && pFAggro > 0.0f
                && pfHeroHP > 0.0f)
            || pHeroBody->field_200 == 16 || pHeroBody->field_200 == 10
            || pHeroBody->field_200 == 11 || pHeroBody->field_200 == 13);

        // --- round ended: menu exit / defeat-timeout / victory / next wana
        StopAndRestartThread(0);
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
        if (pfHeroHP <= 0.0f || 100 * nClockBase - (int)dwElapsedMs / 10 <= 0)
        {
            // defeat / timeout
            if (g_pArrSndBuf_5244AC[1])
            {
                g_pArrSndBuf_5244AC[1]->SetVolume(-500);
                DSBufferPlay(g_pArrSndBuf_5244AC[1], 0);
            }
            PlayResultsCutscene_455BE2(pHeroBody, pFoeBody, pWanaBodies, pMapFrame,
                    (int *)pArrHudMeters, (int *)pArrSprites, pfHeroHP, fFieldC4,
                    pFAggro, fAggroMax, nClockBase, dwElapsedMs);
            nSelectResult = ShowMouseSelectDialog_4573CF(arrSparkSprites,
                                                         g_pArrSndBuf_5244AC);
            PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0f, 0);
            // heal 15% of the persisted max HP (clamped)
            g_sceneState_52F7E0.protCurrentHP_C0 =
                g_sceneState_52F7E0.protMaximumHP_C4 * 15.0f / 100.0f
                + g_sceneState_52F7E0.protCurrentHP_C0;
            if (g_sceneState_52F7E0.protCurrentHP_C0 > (double)g_sceneState_52F7E0.protMaximumHP_C4)
                g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4;
            pfHeroHP = g_sceneState_52F7E0.protCurrentHP_C0;
            if (nSelectResult)
                goto LABEL_154;    // keep going: reveal the next wana
            nSysMenuResult = 1;
            break;
        }
        if (pFAggro <= 0.0f)
        {
            // victory
            g_sceneState_52F7E0.protCurrentHP_C0 = pfHeroHP;
            g_sceneState_52F7E0.protMaximumHP_C4 = fFieldC4;
            PlayVictoryCutscene_455183(pOdsScene, pHeroBody, pFoeBody, pMapFrame,
                    (int *)pArrHudMeters, (int *)pArrSprites, pfHeroHP, fFieldC4,
                    pFAggro, fAggroMax, nClockBase, dwElapsedMs,
                    (int *)arrVoiceBufs);
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
    StopAndRestartThread(0);
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

void register_PlayHunting_44C38D() {
  hnh_Hook(0x44C38D, hook_PlayHunting_44C38D, "PlayHunting_44C38D"); // [OK]

  hnh_HookOff();
  hnh_HookOn();
}
