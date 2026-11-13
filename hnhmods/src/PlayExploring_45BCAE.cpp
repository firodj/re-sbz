// GAME: [011019][TEATIME] âÿÇÃîﬂñ¬Å@Å`âÛÇÍÇΩè…éqÇÃêSÅ`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayExploring_45BCAE)(int nStageIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x45BCAE);

void (__cdecl *CHanaAnimSetT3F1__RemoveAnimT3F0_404349)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)
= reinterpret_cast<void (__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)>(0x404349);

void(__cdecl*UpdateCameraLinkedObjects_44451F)(CHanaFrameT3EC_S354 *pCameraFrame) =
    reinterpret_cast<void(*)(CHanaFrameT3EC_S354 *)>(0x44451F);

void (__cdecl*BuildMapSubFrames_443DD5)(CHanaFrameT3EC_S354 *pMapFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x443DD5);

CHanaSpriteT3F6_S1FC *(__cdecl*InitHudResources_4490A7)(CHanaSpriteT3F6_S1FC **arrSprNumbers,
    CHanaSpriteT3F6_S1FC **arrSprTimes, CHanaSpriteT3F6_S1FC **arrSprModes, int nMode) =
    reinterpret_cast<CHanaSpriteT3F6_S1FC *(*)(CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **,
      CHanaSpriteT3F6_S1FC **, int)>(0x4490A7);

void (__cdecl*UpdateCameraFollowChase_445525)(CHanaFrameT3EC_S354 *pHeroFrame,
    CHanaFrameT3EC_S354 *pCameraFrame, CHanaFrameT3EC_S354 *pMapFrame, float fTime, int bMove) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *, CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, float, int)>(0x445525);


void (__cdecl*SetupItemBodies_459F81)(CHanaSceneT458_S504 *pScene, CSekBody **pItemBodies,
    DWORD *arrItemPlaceData, int nMapIndex) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CSekBody **, DWORD *, int)>(0x459F81);

char (__cdecl*UpdateHeroControl_45A513)(CSekBody *pHeroBody, CHanaFrameT3EC_S354 *pMapFrame,
    float fDeltaTime, float *pfHeroHp, float fHeroHpMax, int nMapIndex, float *pfCamFactor) =
    reinterpret_cast<char(__cdecl*)(CSekBody *, CHanaFrameT3EC_S354 *, float, float *, float,
      int, float *)>(0x45A513);

int (__cdecl*FindTargetInRange_45A1AD)(CHanaFrameRootT3EC_S2D4 *pHeroFrame,
    CSekBody **pItemBodies, float fDeltaTime, int nMapIndex) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameRootT3EC_S2D4 *, CSekBody **, float, int)>(0x45A1AD);

void (__cdecl*ApplyTargetEffect_45A2BA)(int nTargetSprite, float *pfHeroHp, float *pfHeroHpMax,
    int *pnTimeLimit, float *pfCamFactor, float *pfBlendTime) =
    reinterpret_cast<void(__cdecl*)(int, float *, float *, int *, float *, float *)>(0x45A2BA);

void (__cdecl*UpdateLockonEvent_45953B)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameT3EC_S354 *pLockonFrame, int nMapIndex) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, int)>(0x45953B);

int (__cdecl*CheckSceneTransition_459768)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, int *pnMapIndex, int *pnSubIndex, int nStageIndex) =
    reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, int *, int *,
      int)>(0x459768);

double (__cdecl*GetSceneEventTime_459CA2)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, unsigned int *pnCursor, int nMapIndex) =
    reinterpret_cast<double(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, unsigned int *,
      int)>(0x459CA2);

void (__cdecl*UpdateHudGauge_44BC36)(CHanaSpriteT3F6_S1FC **pHudRes, int nCurHp, int nMaxHp) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, int)>(0x44BC36);

void (__cdecl*PrepareTargetSelect_45B5BD)(CSekBody *pHeroBody, CSekBody **pItemBodies,
    CSekBody **pSpriteNames, CHanaFrameT3EC_S354 *pMapFrame, CHanaSpriteT3F6_S1FC **arrSprNumbers,
    CHanaSpriteT3F6_S1FC **arrSprTimes, float fHeroHp, float fHeroHpMax, int nTimeLimit,
    unsigned int nElapsed) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, CSekBody **, CSekBody **, CHanaFrameT3EC_S354 *,
      CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **, float, float, int, unsigned int)>(0x45B5BD);

int (__cdecl*ShowMouseSelectDialog_4573CF)(CHanaSpriteT3F6_S1FC **arrSprModes,
    LPDIRECTSOUNDBUFFER *pArrSndBuf) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC **, LPDIRECTSOUNDBUFFER *)>(0x4573CF);

void (__cdecl*DrawSpriteListEcho_45B2FB)(CHanaSpriteT3F6_S1FC **arrSprites, int nTargetIdx,
    float fDeltaTime) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, float)>(0x45B2FB);

void (__cdecl *HanaMatrix_BuildLookAtAndTransform)(CHanaFrameT3EC_S354 *pFoeFrame, CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameRootT3EC_S2D4 *pSceneFrame, BOOL bSkip)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFoeFrame, CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameRootT3EC_S2D4 *pSceneFrame, BOOL bSkip)>(0x407b73);
void (__cdecl *Frame_ComputeWorldAABB)(CHanaFrameT3EC_S354 *pFrame)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x407e5b);
void (__cdecl *HanaTexture_CreateFromFile)(CHanaSpriteT3F6_S1FC **ppOutSprite, CHAR *szFileName)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC **ppOutSprite, CHAR *szFileName)>(0x4135ed);
void (__cdecl *HanaSurface_CreateAndFade)()
= reinterpret_cast<void (__cdecl*)()>(0x455b0e);

// === GLOBAL VARIABLES ====

#ifdef _USRDLL
float &g_fHeroAnimSpeed = *(float *)0x52EF70; // WRITABLE
bool &g_bSpinActive = *(bool *)0x52EF76; // WRITABLE
float &g_fSpinTimer = *(float *)0x52EF6C; // WRITABLE
float &g_fIdleTimer = *(float *)0x52EF7C; // WRITABLE
bool &g_bAttackHeld = *(bool *)0x52EF74; // WRITABLE
bool &g_bSkillActive = *(bool *)0x52EF75; // WRITABLE
float &g_fSkillTimer = *(float *)0x52EF78; // WRITABLE

#else

float g_fHeroAnimSpeed;
bool  g_bSpinActive   ;
float g_fSpinTimer    ;
float g_fIdleTimer    ;
bool  g_bAttackHeld   ;
bool  g_bSkillActive  ;
float g_fSkillTimer   ;

#endif

// === FUNCTIONS ===


// PlayExploring_45BCAE(nStageIndex) -- GameMainLoop scene-type 2: the map
// exploration/battle screen. nStageIndex 1..10 (0 reads mapIndexTable[-1] in
// the original) selects the stage entry; per stage the map/sub-stage/time
// limit come from three small tables:
//   pnMapIndex = mapIndexTable[2*nStageIndex-1] -> Map\mXX\atXX.ODF +
//     sch\mapXX.ods (index 0 / 7 / >= 24 are the runtime-empty "" rows --
//     raw .data past the image is 0xFF filler, zero at runtime -- so those
//     stages parse an empty path and bail out with -1)
//   pnSubIndex = mapIndexTable[2*nStageIndex]
//   sub-stage  = subStageTable[nStageIndex]  -> arrItemPlaceData row set
//   time limit = timeLimitTable[nStageIndex] (in 1/100 s; HUD shows
//     100*nTimeLimit - nElapsed/10)
// One-time setup: 10 SFX wavs (Start/GameOver/On/On2/foot2..5; slots 4..9 are
// the "" filler), stage BGM s007/8/9.wav (stages 0,2,4,6,9 -> s007, 10 ->
// s009, else s008) volume-clamped from g_fVolumeDbTable[avi volume]-1000,
// InitHudResources_4490A7, 7 mask-sprite bmp pairs sch\mXX.bmp/a.bmp, hero
// body otoko\otoko.sek/.odf (camera frame "Frame5_o00_02_head"), lockon body
// lockon\lock.sek/.odf, item bodies item\i11..i17.
//
// Outer loop: parse the map ODF (free the anim set, attach the map frame to
// the scene frame, BuildMapSubFrames), load the scene ODS, SetupHeroEntry +
// SetupItemBodies, park the camera at (0,25,-1) relative to the hero. Inner
// frame loop: ESC -> SystemMenu_4655AB on the BGM (entry-state snapshot with
// live scene flags; >= 1 leaves); dialogue mode freezes the countdown and
// gives right-button x5 text speed / middle-button skip toggle; otherwise
// UpdateHeroControl (+ FindTargetInRange/ApplyTargetEffect echo sprites),
// camera follow, lockon event. CheckSceneTransition 1 -> fade + exit, 2 ->
// forced dialogue (pfExecTime = 10000), else GetSceneEventTime may start an
// event dialogue; both dialogue entries park the lockon body at (1000,1000,
// 1000) and save the hero pos/rot for the exit path (ParseExecStr returning
// 0) to restore. Render: scene surface blit (Clear(2)) or the 3D pass
// (Clear(3) + HanaFrame_Draw), then the dialogue pose (hero placed at the
// cursor's frame) or the HUD (UpdateHudGauge + DrawClockHud), the target
// echo sprites, present.
//
// Exit (LABEL_125): HP <= 0 or countdown 0 with nMenuResult < 0 -> stage 7
// short-circuits to scene step 80; otherwise BGM stop + GameOver SFX,
// PrepareTargetSelect + ShowMouseSelectDialog to pick the next target (0 ->
// return 1; picked -> heal +15% max HP, reset the stage tables, reload).
// nMenuResult >= 0 breaks to the epilogue: HP is written back to
// g_sceneState only when nMenuResult == 0 (the stage-7 and dialog-abort
// gotos skip that store). Cleanup releases the 10 SFX + BGM buffers, both
// sek bodies, the item/target bodies, the scene, the map frame and all HUD/
// echo sprites. Returns 0 normal, 1 quit-to-menu, -1 load failure.
//
// Label map vs the original decompilation: LABEL_84 -> the render block
// inside the frame loop, LABEL_125 -> stage-exit dispatch, LABEL_149 ->
// cleanup epilogue.


int __cdecl hook_PlayExploring_45BCAE(int nStageIndex)
{
    CSceneState sceneStateLive;          // [ebp-2FFCh] live copy around SystemMenu
    CSceneState sceneMenuCopy;           // [ebp-31CCh] SystemMenu by-value arg / scratch
    CSceneState Src;                     // [ebp-204h] entry snapshot of g_sceneState
    int nSelectResult;                   // [ebp-3034h]
    float fTgtX, pOutY, pOutZ;           // [ebp-3018h..] cursor target position
    float pXpos, pYpos, pZpos;           // [ebp-3014h..] cursor target rotation
    float pXYrot, pYXrot, pZXrot;
    DWORD dwAntialiasSave;               // [ebp-3008h]
    POINT pOut;                          // [ebp-3004h] ProjectToScreen out
    float fTextSpeed;                    // [ebp-2E6Ch]
    CHanaAnimSetT3F1_S150 *pTmpAnimSet;  // [ebp-2E68h]
    char szBgmWavNames[3][20];           // [ebp-2D60h] s007/s008/s009.wav
    float pfExecTime;                    // [ebp-2D24h] scene event clock
    CSekBody *pItemBodies[20];           // [ebp-2D20h] item\i11..i17 (+0 slot)
    DWORD dwTimeBase;                    // [ebp-2CD0h] countdown epoch
    int nTimeLimit;                      // [ebp-2CCCh] in 1/100 s
    float fHeroHp;                       // [ebp-2CC8h]
    DWORD arrItemPlaceData[6][25][10];   // [ebp-2CC4h] per sub-stage/map item pairs
    float fHeroZ, fHeroY, fHeroX;        // [ebp-1554h..] hero pos saved for dialogue
    char szMapOdfPaths[30][25];          // [ebp-1550h] Map\mXX\atXX.ODF
    char szSprNames[30][25];             // [ebp-1550h..] sch\mXX mask-sprite base names
    CHanaFrameT3EC_S354 *pMapFrame;      // [ebp-1260h]
    CSekBody *pSpriteNames[20];          // [ebp-125Ch] target bodies (filled by 45B5BD)
    CHanaSpriteT3F6_S1FC *pSprNumbers[15]; // [ebp-120Ch] clock digits
    CHAR pszFileName[256];               // [ebp-11A8h]
    float fCamFactor;                    // [ebp-10A8h]
    char szSfxWavNames[15][50];          // [ebp-10A0h]
    WORD wPrevBtn;                       // [ebp-DB0h] last-frame mouse button
    float fHeroHpMax;                    // [ebp-6A0h]
    char szSceneOdsPaths[30][25];        // [ebp-698h] sch\mapXX.ods
    float dirForwardX, dirForwardY, dirForwardZ; // [ebp-6A8h..] hero rot saved for dialogue
    int nTransResult;                    // [ebp-6A4h]
    float dirUpX, dirUpY, dirUpZ;
    char szItemSekPaths[30][25];         // [ebp-694h] item\i1X
    int i;                               // [ebp-AB8h]
    LPDIRECTSOUNDBUFFER pDSBuf[10];      // [ebp-AB4h] SFX buffers
    DWORD Time;                          // [ebp-A8Ch] fade-in reference clock
    int bFirstFrameDone;                 // [ebp-A88h]
    CHanaFrameT3EC_S354 *pCamFrame;      // [ebp-A84h] "Frame5_o00_02_head"
    DWORD timeLimitTable[25];            // [ebp-A80h]
    CHanaSpriteT3F6_S1FC *pSprTimes[20]; // [ebp-A1Ch] HP gauge
    float fBlendTime;                    // [ebp-9A4h] blend-out timer after a target
    int nMenuResult;                     // [ebp-99Ch]
    DWORD mapIndexTable[50];             // (stage -> map/sub index pairs)
    DWORD subStageTable[15];             // [ebp-2D9Ch]
    CSekBody *pHeroBody;                 // [ebp-3A4h]
    int bDialogueMode;                   // [ebp-3A0h]
    BOOL bSkipMode;                      // [ebp-39Ch]
    CHanaSceneT458_S504 *pScene;         // [ebp-394h]
    float fDeltaTime;                    // [ebp-38Ch]
    CHAR String[256];                    // [ebp-388h]
    int pnSubIndex;                      // [ebp-288h]
    int yMouse;                          // [ebp-284h]
    unsigned int nElapsed;               // [ebp-27Ch]
    unsigned int nElapsedPrev;           // [ebp-278h]
    int xMouse;                          // [ebp-274h]
    LPDIRECTSOUNDBUFFER pBgmBuffer;      // [ebp-270h]
    int pnMapIndex;                      // [ebp-26Ch]
    CHanaSpriteT3F6_S1FC *ppOut[10];     // [ebp-268h] mask/echo sprites
    CSekBody *pSekBody;                  // [ebp-74h] lockon body
    CHanaSpriteT3F6_S1FC *pSprModes[15]; // [ebp-70h] target-select sprites
    char updated;                        // [ebp-Ch] UpdateHeroControl result
    WORD wBtnMouse;                      // [ebp-8h]
    int nTargetSprite;                   // [ebp-4h] FindTargetInRange result
    unsigned int pnCursor;               // scene cursor (event dialogue)
    CSekBody *pCursorBody;               // refsSekBody_13C[pnCursor]

    ONCELOG("[HNH:SUB] PlayExploring_45BCAE\n");

    DebugLog("[HNH][PlayExploring] init variables\n");
    // --- path/name tables (entries [0]/[7]/[8..10] etc. are the runtime-empty
    // "" rows -- .data past the raw image, 0xFF in IDA, zero-filled at runtime)
    strcpy(szMapOdfPaths[0], "");            strcpy(szMapOdfPaths[7], "");
    strcpy(szMapOdfPaths[1], "Map\\m01\\at01.ODF");
    strcpy(szMapOdfPaths[2], "Map\\m02\\at02.ODF");
    strcpy(szMapOdfPaths[3], "Map\\m03\\at03.ODF");
    strcpy(szMapOdfPaths[4], "Map\\m04\\at04.ODF");
    strcpy(szMapOdfPaths[5], "Map\\m05\\at05.ODF");
    strcpy(szMapOdfPaths[6], "Map\\m06\\at06.ODF");
    strcpy(szMapOdfPaths[8], "Map\\m08\\at08.ODF");
    strcpy(szMapOdfPaths[9], "Map\\m09\\at09.ODF");
    strcpy(szMapOdfPaths[10], "Map\\m10\\at10.ODF");
    strcpy(szMapOdfPaths[11], "Map\\m11\\at11.ODF");
    strcpy(szMapOdfPaths[12], "Map\\m12\\at12.ODF");
    strcpy(szMapOdfPaths[13], "Map\\m13\\at13.ODF");
    strcpy(szMapOdfPaths[14], "Map\\m14\\at14.ODF");
    strcpy(szMapOdfPaths[15], "Map\\m15\\at15.ODF");
    strcpy(szMapOdfPaths[16], "Map\\m16\\at16.ODF");
    strcpy(szMapOdfPaths[17], "Map\\m17\\at17.ODF");
    strcpy(szMapOdfPaths[18], "Map\\m18\\at18.ODF");
    strcpy(szMapOdfPaths[19], "Map\\m19\\at19.ODF");
    strcpy(szMapOdfPaths[20], "Map\\m20\\at20.ODF");
    strcpy(szMapOdfPaths[21], "Map\\m21\\at21.ODF");
    strcpy(szMapOdfPaths[22], "Map\\m22\\at22.ODF");
    strcpy(szMapOdfPaths[23], "Map\\m23\\at23.ODF");
    memset(szMapOdfPaths[24], 0, 6 * 25);

    strcpy(szSceneOdsPaths[0], "");         strcpy(szSceneOdsPaths[7], "");
    strcpy(szSceneOdsPaths[1], "sch\\map01.ods");
    strcpy(szSceneOdsPaths[2], "sch\\map02.ods");
    strcpy(szSceneOdsPaths[3], "sch\\map03.ods");
    strcpy(szSceneOdsPaths[4], "sch\\map04.ods");
    strcpy(szSceneOdsPaths[5], "sch\\map05.ods");
    strcpy(szSceneOdsPaths[6], "sch\\map06.ods");
    strcpy(szSceneOdsPaths[8], "sch\\map08.ods");
    strcpy(szSceneOdsPaths[9], "sch\\map09.ods");
    strcpy(szSceneOdsPaths[10], "sch\\map10.ods");
    strcpy(szSceneOdsPaths[11], "sch\\map11.ods");
    strcpy(szSceneOdsPaths[12], "sch\\map12.ods");
    strcpy(szSceneOdsPaths[13], "sch\\map13.ods");
    strcpy(szSceneOdsPaths[14], "sch\\map14.ods");
    strcpy(szSceneOdsPaths[15], "sch\\map15.ods");
    strcpy(szSceneOdsPaths[16], "sch\\map16.ods");
    strcpy(szSceneOdsPaths[17], "sch\\map17.ods");
    strcpy(szSceneOdsPaths[18], "sch\\map18.ods");
    strcpy(szSceneOdsPaths[19], "sch\\map19.ods");
    strcpy(szSceneOdsPaths[20], "sch\\map20.ods");
    strcpy(szSceneOdsPaths[21], "sch\\map21.ods");
    strcpy(szSceneOdsPaths[22], "sch\\map22.ods");
    strcpy(szSceneOdsPaths[23], "sch\\map23.ods");
    memset(szSceneOdsPaths[24], 0, 6 * 25);

    strcpy(szItemSekPaths[0], "");
    strcpy(szItemSekPaths[1], "item\\i11");
    strcpy(szItemSekPaths[2], "item\\i12");
    strcpy(szItemSekPaths[3], "item\\i13");
    strcpy(szItemSekPaths[4], "item\\i14");
    strcpy(szItemSekPaths[5], "item\\i15");
    strcpy(szItemSekPaths[6], "item\\i16");
    strcpy(szItemSekPaths[7], "item\\i17");
    memset(szItemSekPaths[8], 0, 22 * 25);

    // mask/echo sprite base names (deliberately out of numeric order)
    memset(szSprNames, 0, sizeof(szSprNames));
    strcpy(szSprNames[0], "");           // runtime-empty
    strcpy(szSprNames[1], "sch\\m01");
    strcpy(szSprNames[2], "sch\\m05");
    strcpy(szSprNames[3], "sch\\m03");
    strcpy(szSprNames[4], "sch\\m06");
    strcpy(szSprNames[5], "sch\\m02");
    strcpy(szSprNames[6], "sch\\m04");
    strcpy(szSprNames[7], "sch\\m07");
    szSprNames[8][0] = 0;                // [8..10] runtime-empty ""; [11..29] zero
    szSprNames[9][0] = 0;
    szSprNames[10][0] = 0;

    // per-map item placement rows: sub-stages 0..3 hold up to five
    // (count, itemId) pairs per map index (rows 1..20); stage 4 unused.
    // (QWORD stores in the original; pairs spelled out here)
    memset(arrItemPlaceData, 0, sizeof(arrItemPlaceData));
    // sub-stage 0
    arrItemPlaceData[0][1][0] = 1;  arrItemPlaceData[0][1][1] = 1;
    arrItemPlaceData[0][2][0] = 1;  arrItemPlaceData[0][2][1] = 2;
    arrItemPlaceData[0][3][0] = 0;  arrItemPlaceData[0][3][1] = 2;
    arrItemPlaceData[0][4][0] = 1;  arrItemPlaceData[0][4][2] = 3;
    arrItemPlaceData[0][5][0] = 1;  arrItemPlaceData[0][5][1] = 2;  arrItemPlaceData[0][5][2] = 4;
    arrItemPlaceData[0][6][0] = 1;  arrItemPlaceData[0][6][1] = 2;  arrItemPlaceData[0][6][2] = 5;
    arrItemPlaceData[0][8][0] = 1;  arrItemPlaceData[0][8][1] = 2;
    arrItemPlaceData[0][9][0] = 1;  arrItemPlaceData[0][9][1] = 2;
    arrItemPlaceData[0][10][0] = 3;
    arrItemPlaceData[0][11][0] = 1;
    arrItemPlaceData[0][12][0] = 1;
    arrItemPlaceData[0][13][0] = 3;
    arrItemPlaceData[0][14][0] = 3; arrItemPlaceData[0][14][1] = 2; arrItemPlaceData[0][14][2] = 1;
    arrItemPlaceData[0][15][0] = 3;
    arrItemPlaceData[0][16][0] = 1; arrItemPlaceData[0][16][1] = 2; arrItemPlaceData[0][16][2] = 3;
    arrItemPlaceData[0][16][3] = 4; arrItemPlaceData[0][16][4] = 5; arrItemPlaceData[0][16][5] = 6;
    arrItemPlaceData[0][17][0] = 3; arrItemPlaceData[0][17][1] = 2; arrItemPlaceData[0][17][2] = 1;
    arrItemPlaceData[0][18][0] = 3;
    arrItemPlaceData[0][19][0] = 2;
    arrItemPlaceData[0][20][0] = 3;
    // sub-stage 1
    arrItemPlaceData[1][1][0] = 3;  arrItemPlaceData[1][1][1] = 1;
    arrItemPlaceData[1][2][0] = 0;  arrItemPlaceData[1][2][1] = 2;
    arrItemPlaceData[1][3][0] = 1;
    arrItemPlaceData[1][4][0] = 0;  arrItemPlaceData[1][4][1] = 1;
    arrItemPlaceData[1][5][2] = 4;
    arrItemPlaceData[1][6][2] = 5;
    arrItemPlaceData[1][8][0] = 1;  arrItemPlaceData[1][8][1] = 2;
    arrItemPlaceData[1][9][0] = 1;  arrItemPlaceData[1][9][1] = 2;
    arrItemPlaceData[1][11][0] = 3;
    arrItemPlaceData[1][13][0] = 2;
    arrItemPlaceData[1][14][0] = 3; arrItemPlaceData[1][14][1] = 2; arrItemPlaceData[1][14][2] = 1;
    arrItemPlaceData[1][15][0] = 1;
    arrItemPlaceData[1][16][0] = 1; arrItemPlaceData[1][16][4] = 0; arrItemPlaceData[1][16][5] = 6;
    arrItemPlaceData[1][17][0] = 3; arrItemPlaceData[1][17][1] = 2; arrItemPlaceData[1][17][2] = 1;
    arrItemPlaceData[1][18][0] = 3;
    arrItemPlaceData[1][19][0] = 4;
    arrItemPlaceData[1][20][0] = 3;
    // sub-stage 2
    arrItemPlaceData[2][1][0] = 5;
    arrItemPlaceData[2][2][0] = 4;
    arrItemPlaceData[2][3][0] = 0;  arrItemPlaceData[2][3][1] = 6;
    arrItemPlaceData[2][4][0] = 4;  arrItemPlaceData[2][4][1] = 4;
    arrItemPlaceData[2][5][0] = 0;  arrItemPlaceData[2][5][1] = 2;
    arrItemPlaceData[2][6][0] = 1;  arrItemPlaceData[2][6][1] = 5;
    arrItemPlaceData[2][8][0] = 1;
    arrItemPlaceData[2][9][0] = 0;  arrItemPlaceData[2][9][1] = 3;
    arrItemPlaceData[2][10][0] = 3;
    arrItemPlaceData[2][13][0] = 3;
    arrItemPlaceData[2][14][0] = 0; arrItemPlaceData[2][14][1] = 4; arrItemPlaceData[2][14][2] = 1;
    arrItemPlaceData[2][15][0] = 5;
    arrItemPlaceData[2][16][0] = 0; arrItemPlaceData[2][16][1] = 2; arrItemPlaceData[2][16][4] = 5;
    arrItemPlaceData[2][17][2] = 1;
    arrItemPlaceData[2][18][0] = 4;
    arrItemPlaceData[2][19][0] = 3;
    arrItemPlaceData[2][20][0] = 1;
    // sub-stage 3
    arrItemPlaceData[3][1][0] = 0;  arrItemPlaceData[3][1][1] = 1;
    arrItemPlaceData[3][2][0] = 1;  arrItemPlaceData[3][2][1] = 6;
    arrItemPlaceData[3][3][0] = 3;  arrItemPlaceData[3][3][1] = 2;
    arrItemPlaceData[3][4][0] = 5;  arrItemPlaceData[3][4][1] = 6;  arrItemPlaceData[3][4][2] = 3;
    arrItemPlaceData[3][5][0] = 3;  arrItemPlaceData[3][5][1] = 5;
    arrItemPlaceData[3][6][0] = 4;  arrItemPlaceData[3][6][1] = 2;  arrItemPlaceData[3][6][2] = 5;
    arrItemPlaceData[3][8][0] = 3;  arrItemPlaceData[3][8][1] = 2;
    arrItemPlaceData[3][9][0] = 1;
    arrItemPlaceData[3][10][0] = 3;
    arrItemPlaceData[3][11][0] = 4;
    arrItemPlaceData[3][12][0] = 3;
    arrItemPlaceData[3][13][0] = 4;
    arrItemPlaceData[3][14][0] = 5; arrItemPlaceData[3][14][1] = 2; arrItemPlaceData[3][14][2] = 4;
    arrItemPlaceData[3][15][0] = 4;
    arrItemPlaceData[3][16][0] = 1; arrItemPlaceData[3][16][2] = 3; arrItemPlaceData[3][16][3] = 4;
    arrItemPlaceData[3][17][0] = 2; arrItemPlaceData[3][17][1] = 2;
    arrItemPlaceData[3][18][0] = 5;
    arrItemPlaceData[3][19][0] = 5;
    arrItemPlaceData[3][20][0] = 5;
    // sub-stage 4: all zero

    strcpy(szSfxWavNames[0], "Start");
    strcpy(szSfxWavNames[1], "GameOver");
    strcpy(szSfxWavNames[2], "On");
    strcpy(szSfxWavNames[3], "On2");
    szSfxWavNames[4][0] = 0;    // runtime-empty "" (slots 4..9)
    szSfxWavNames[5][0] = 0;
    szSfxWavNames[6][0] = 0;
    szSfxWavNames[7][0] = 0;
    szSfxWavNames[8][0] = 0;
    szSfxWavNames[9][0] = 0;
    strcpy(szSfxWavNames[10], "foot4");
    strcpy(szSfxWavNames[11], "foot2");
    strcpy(szSfxWavNames[12], "foot3");
    strcpy(szSfxWavNames[13], "foot5");
    memset(szSfxWavNames[14], 0, 50);

    // --- state init
    nMenuResult = -1;
    pBgmBuffer = 0;
    pfExecTime = 0.0f;
    pScene = 0;
    pMapFrame = 0;
    pHeroBody = 0;
    pSekBody = 0;
    nElapsed = 0;
    nTimeLimit = 120;
    nElapsedPrev = 0;
    Time = 0;
    fHeroHpMax = g_sceneState_52F7E0.protMaximumHP_C4;
    fHeroHp = g_sceneState_52F7E0.protMaximumHP_C4;
    fCamFactor = 1.0f;
    fBlendTime = 0.0f;
    pnMapIndex = 1;
    pnSubIndex = 0;
    bFirstFrameDone = 0;
    updated = 0;
    bDialogueMode = 0;
    pnCursor = 0;
    xMouse = 0;
    yMouse = 0;
    bSkipMode = 0;
    nTargetSprite = 0;
    pCamFrame = 0;

    mapIndexTable[0] = 22;  mapIndexTable[1] = 2;
    mapIndexTable[2] = 22;  mapIndexTable[3] = 3;
    mapIndexTable[4] = 22;  mapIndexTable[5] = 1;
    mapIndexTable[6] = 22;  mapIndexTable[7] = 0;
    mapIndexTable[8] = 22;  mapIndexTable[9] = 0;
    mapIndexTable[10] = 2;  mapIndexTable[11] = 0;
    mapIndexTable[12] = 23; mapIndexTable[13] = 2;
    mapIndexTable[14] = 8;  mapIndexTable[15] = 0;
    mapIndexTable[16] = 16; mapIndexTable[17] = 0;
    mapIndexTable[18] = 16; mapIndexTable[19] = 1;
    mapIndexTable[20] = 5;  mapIndexTable[21] = 1;
    memset(&mapIndexTable[22], 0, 28 * 4);

    subStageTable[0] = 0;  subStageTable[1] = 0;
    subStageTable[2] = 1;  subStageTable[3] = 1;
    subStageTable[4] = 2;  subStageTable[5] = 2;
    subStageTable[6] = 2;  subStageTable[7] = 3;
    subStageTable[8] = 3;  subStageTable[9] = 3;
    subStageTable[10] = 3;
    memset(&subStageTable[11], 0, 4 * 4);
    timeLimitTable[0] = 240;  timeLimitTable[1] = 360;
    timeLimitTable[2] = 140;  timeLimitTable[3] = 160;
    timeLimitTable[4] = 180;  timeLimitTable[5] = 190;
    timeLimitTable[6] = 200;  timeLimitTable[7] = 60;
    timeLimitTable[8] = 170;  timeLimitTable[9] = 180;
    timeLimitTable[10] = 190;
    memset(&timeLimitTable[11], 0, 14 * 4);

    memcpy(&Src, &g_sceneState_52F7E0, sizeof(Src));     // entry snapshot
    memset(pSprNumbers, 0, sizeof(pSprNumbers));
    memset(pSprTimes, 0, sizeof(pSprTimes));
    memset(pSprModes, 0, sizeof(pSprModes));
    memset(pItemBodies, 0, sizeof(pItemBodies));
    memset(pSpriteNames, 0, sizeof(pSpriteNames));
    memset(ppOut, 0, sizeof(ppOut));
    memcpy(&g_sceneState_52F7E0, &Src, sizeof(g_sceneState_52F7E0));  // (round-trip as-is)

    pnMapIndex = mapIndexTable[2 * nStageIndex];
    pnSubIndex = mapIndexTable[2 * nStageIndex + 1];
    fHeroHp = g_sceneState_52F7E0.protCurrentHP_C0;
    fHeroHpMax = g_sceneState_52F7E0.protMaximumHP_C4;
    g_sceneState_52F7E0.bCurrentSubStage = subStageTable[nStageIndex];
    g_sceneState_52F7E0.bMaxSubStage = subStageTable[nStageIndex] + 1;
    nTimeLimit = timeLimitTable[nStageIndex];
    SetProjectionFov_40DCA3(1.2f);
    SetTimerScaleFactor(14.0f);

    DebugLog("[HNH][PlayExploring] loading assets\n");

    // --- one-time loads: 10 SFX + BGM + HUD + mask sprites + bodies
    memset(pDSBuf, 0, sizeof(pDSBuf));
    for (i = 0; i < 10; ++i)
    {
        wsprintfA(pszFileName, "%s.wav", szSfxWavNames[i]);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
        pDSBuf[i] = LoadWav_422E7B(pszFileName);
    }
    strcpy(szBgmWavNames[0], "s007.wav");
    strcpy(szBgmWavNames[1], "s008.wav");
    strcpy(szBgmWavNames[2], "s009.wav");
    if (!nStageIndex || nStageIndex == 2 || nStageIndex == 4 || nStageIndex == 6 || nStageIndex == 9)
        i = 0;
    else if (nStageIndex == 10)
        i = 2;
    else
        i = 1;
    wsprintfA(pszFileName, "%s", szBgmWavNames[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1006);          // WAV dir
    pBgmBuffer = LoadWav_422E7B(pszFileName);
    if (pBgmBuffer)
    {
        // clamp table[aviVolume] - 1000 dB to [-10000, 0]
        if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0) >= -10000)
        {
            if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0) <= 0)
                pBgmBuffer->SetVolume((__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0));
            else
                pBgmBuffer->SetVolume(0);
        }
        else
        {
            pBgmBuffer->SetVolume(-10000);
        }
    }
    InitHudResources_4490A7(pSprNumbers, pSprTimes, pSprModes, 1);

    // mask/echo sprites for the 7 target kinds (sch\mXX.bmp + sch\mXXa.bmp)
    for (i = 0; i < 10; ++i)
    {
        if (szSprNames[i][0])
        {
            wsprintfA(pszFileName, "%s.bmp", szSprNames[i]);
            CHanaObj__GetRealPath_486D9A(pszFileName, 1003);  // SPR dir
            wsprintfA(String, "%sa.bmp", szSprNames[i]);
            CHanaObj__GetRealPath_486D9A(String, 1003);
            LoadBmpWithMask_413A5E(&ppOut[i], pszFileName, String);
        }
    }

    // hero + camera frame
    wsprintfA(pszFileName, "otoko\\otoko.sek");
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    wsprintfA(String, "otoko\\otoko.odf");
    CHanaObj__GetRealPath_486D9A(String, 1005);
    if (!LoadSekAndOdf_4445DD(pszFileName, String, &pHeroBody)) {
      DebugLog("[HNH][PlayExploring] error LoadSekAndOdf otoko\n");
      return -1;
    }
    pCamFrame = CHanaFrameT3EC__FindByName_483650(pHeroBody->pSekHeader_220->pFrameT3EC,
                                                  "Frame5_o00_02_head");

    // lock-on body
    wsprintfA(pszFileName, "lockon\\lock.sek");
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);
    wsprintfA(String, "lockon\\lock.odf");
    CHanaObj__GetRealPath_486D9A(String, 1005);
    if (!LoadSekAndOdf_4445DD(pszFileName, String, &pSekBody)) {
      DebugLog("[HNH][PlayExploring] error LoadSekAndOdf lock\n");
      return -1;
    }
    CSekBody__CutToAnim_4018E1(pSekBody, 0);

    // item bodies item\i11..i17
    for (i = 1; i <= 7; ++i)
    {
        wsprintfA(pszFileName, "%s.sek", szItemSekPaths[i]);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1005);
        wsprintfA(String, "%s.odf", szItemSekPaths[i]);
        CHanaObj__GetRealPath_486D9A(String, 1005);
        if (!LoadSekAndOdf_4445DD(pszFileName, String, &pItemBodies[i])) {
          DebugLog("[HNH][PlayExploring] error LoadSekAndOdf %d %s\n", i, szItemSekPaths[i]);
          return -1;
        }
        CSekBody__CutToAnim_4018E1(pItemBodies[i], 0);
    }

    if (pBgmBuffer)
        DSBufferPlay(pBgmBuffer, 1);
    dwTimeBase = timeGetTime();
    Time = timeGetTime();

    for (;;)  // outer: (re)load the current map + scene
    {
        for (i = 0; i < 10; ++i)
        {
            if (ppOut[i])
                ppOut[i]->fAlpha_144 = 0.0;
        }
        pTmpAnimSet = 0;
        if (pMapFrame)
        {
            CHanaObj__Free_40C1E4(pMapFrame);
            pMapFrame = 0;
        }
        wsprintfA(pszFileName, szMapOdfPaths[pnMapIndex]);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1005);
        if (!ParseODFFile_441F19(pszFileName, &pMapFrame, &pTmpAnimSet)) {
          DebugLog("[HNH][PlayExploring] error ParseODFFile %d\n", pnMapIndex);
          return -1;
        }
        if (pTmpAnimSet)
        {
            CHanaObj__Free_40C1E4((CHanaBase_S130 *)pTmpAnimSet);
            pTmpAnimSet = 0;
        }
        AttachChild_4055FD(g_pSceneFrame, pMapFrame);
        SetFramePosRel_4069DD(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
        UpdateBoneLookAtTransform(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                                  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
        BuildMapSubFrames_443DD5(pMapFrame);

        if (pScene)
        {
            CHanaObj__Free_40C1E4(pScene);
            pScene = 0;
        }
        wsprintfA(pszFileName, szSceneOdsPaths[pnMapIndex]);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1002);      // ODS dir
        if (!LoadOds_4843CF(pszFileName, &pScene)) {
          DebugLog("[HNH][PlayExploring] error LoadOds %d\n", pnMapIndex);
          return -1;
        }
        UpdateCameraLinkedObjects_44451F(g_pCameraFrame);
        SetupHeroEntry_4593E0(pScene, pHeroBody->pSekHeader_220->pFrameT3EC, pnSubIndex);
        SetupItemBodies_459F81(pScene, pItemBodies,
                               arrItemPlaceData[subStageTable[nStageIndex]][pnMapIndex], pnMapIndex);
        RestartInputThread(1);
        bFirstFrameDone = 0;
        SetFramePosRel_4069DD(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC,
                       0.0f, 25.0f, -1.0f);
        UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame,
                                  pMapFrame, 1.0f, 0);
        ResetSceneBodies_4857CA(pScene);
        pfExecTime = 0.0f;
        nElapsed = 0;
        g_dwBattleMode = -1;
        CSekBody__CutToAnim_4018E1(pHeroBody, 1);
        CSekBody__UpdateAnim_4019E1(pHeroBody, 0.0f);
        CSekBody__PlayAnim_40179D(pHeroBody, 1);
        FrameTimer_BeginFrame();
        FrameTimer_EndFrame();
        fBlendTime = 0.0f;
        nElapsed = nElapsedPrev;
        dwTimeBase += timeGetTime() - Time;
        Time = timeGetTime();
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFF);

        for (;;)  // inner: frame loop
        {
            DoPeekMessages();
            nElapsedPrev = nElapsed;
            nElapsed = timeGetTime() - dwTimeBase;
            fDeltaTime = (float)GetFrameTime();
            if (fDeltaTime > 5.0f)
                fDeltaTime = 5.0f;
            FrameTimer_EndFrame();

            // ESC (outside dialogue) -> system menu on the BGM
            if (!bDialogueMode && IsMenuKeyPressed_481170())
            {
                Time = timeGetTime();
                memcpy(&sceneStateLive, &g_sceneState_52F7E0, sizeof(sceneStateLive));
                memcpy(&g_sceneState_52F7E0, &Src, sizeof(g_sceneState_52F7E0)); // entry state...
                memcpy(g_sceneState_52F7E0.girlsStateBits_98, sceneStateLive.girlsStateBits_98,
                       sizeof(g_sceneState_52F7E0.girlsStateBits_98));               // ...live flags
                RestartInputThread(0);
                sceneMenuCopy = sceneStateLive;
                nMenuResult = SystemMenu_4655AB(pBgmBuffer, -1000, 0, fHeroHp, fHeroHpMax,
                                                sceneMenuCopy);
                RestartInputThread(1);
                bFirstFrameDone = 0;
                if (nMenuResult >= 1)
                    goto LABEL_125;
                memcpy(&g_sceneState_52F7E0, &sceneStateLive, sizeof(g_sceneState_52F7E0));
                nMenuResult = -1;
            }

            if (bDialogueMode)
            {
                // right button x5 speed, middle toggles skip; freeze countdown
                wPrevBtn = wBtnMouse;
                GetMouseBtn_45A41B(&wBtnMouse);
                if (wBtnMouse == 3)
                    fDeltaTime = fDeltaTime * 5.0f;
                if (wBtnMouse == 2 && wBtnMouse != wPrevBtn)
                    bSkipMode = !bSkipMode;
                dwTimeBase += nElapsed - nElapsedPrev;
                nElapsed = nElapsedPrev;
            }
            else
            {
                if (fBlendTime > 0.0f)
                {
                    CSekBody__UpdateAnim_4019E1(pHeroBody, fDeltaTime);
                    fBlendTime = fBlendTime - fDeltaTime;
                    g_dwBattleMode = -1;
                }
                else
                {
                    updated = UpdateHeroControl_45A513(pHeroBody, pMapFrame, fDeltaTime,
                                                        &fHeroHp, fHeroHpMax, pnMapIndex,
                                                        &fCamFactor);
                    fBlendTime = 0.0f;
                    nTargetSprite = FindTargetInRange_45A1AD(pHeroBody->pSekHeader_220->pFrameT3EC,
                                                             pItemBodies, fDeltaTime, pnMapIndex);
                    if (nTargetSprite)
                    {
                        ApplyTargetEffect_45A2BA(nTargetSprite, &fHeroHp, &fHeroHpMax,
                                                 &nTimeLimit, &fCamFactor, &fBlendTime);
                        if (nTargetSprite > 4)
                            SekBody__UpdateTo2_401647(pHeroBody, 12);
                        else
                            SekBody__UpdateTo2_401647(pHeroBody, 14);
                        if (ppOut[nTargetSprite])
                        {
                            ppOut[nTargetSprite]->fAlpha_144 = 1.0;
                            ProjectToScreen_40E2A5(&pCamFrame->matWorld_178, &pOut);
                            sceneMenuCopy.fPosY_18C = (float)pOut.y;
                            sceneMenuCopy.fPosX_188 = (float)pOut.x;
                            CHanaSpriteT3F6__SetPos_414A5D(ppOut[nTargetSprite],
                                    sceneMenuCopy.fPosX_188, sceneMenuCopy.fPosY_18C);
                        }
                    }
                }
                pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
                UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame,
                                          pMapFrame, fDeltaTime, 0);
                UpdateLockonEvent_45953B(pScene, pHeroBody->pSekHeader_220->pFrameT3EC,
                                         pSekBody->pSekHeader_220->pFrameT3EC, pnMapIndex);
                CSekBody__UpdateAnim_4019E1(pSekBody, fDeltaTime);
                wBtnMouse = 0;
            }

            if (updated == 1)
            {
                nTransResult = CheckSceneTransition_459768(pScene,
                        pHeroBody->pSekHeader_220->pFrameT3EC,
                        &pnMapIndex, &pnSubIndex, nStageIndex);
                if (nTransResult == 1)
                {
                    Time = timeGetTime();
                    PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 50.0f, 0);
                    goto LABEL_125;
                }
                if (nTransResult == 2)
                {
                    // forced event dialogue
                    pfExecTime = 10000.0f;
                    SetProjectionFov_40DCA3(0.5f);
                    bDialogueMode = 1;
                    updated = 0;
                    bSkipMode = 0;
                    pScene->dwPtrCurrent_448 = 0;
                    pScene->WCurNum = 0.0f;
                    pScene->flt_464 = pfExecTime;
                    HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC,
                                             g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
                    CalculateLocalRotationMatrix(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            &dirForwardX, &dirForwardY, &dirForwardZ,
                            &dirUpX, &dirUpY, &dirUpZ);
                    SetFramePosRel_4069DD(pSekBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame, 1000.0f, 1000.0f, 1000.0f);
                    goto LABEL_84;
                }
                if (nTransResult != 3)
                {
                    // hero reached an event spot?
                    pfExecTime = (float)GetSceneEventTime_459CA2(pScene,
                            pHeroBody->pSekHeader_220->pFrameT3EC, &pnCursor, pnMapIndex);
                    if (pfExecTime != 0.0f)
                    {
                        SetProjectionFov_40DCA3(0.5f);
                        bDialogueMode = 1;
                        updated = 0;
                        bSkipMode = 0;
                        pScene->dwPtrCurrent_448 = 0;
                        pScene->WCurNum = 0.0f;
                        pScene->flt_464 = pfExecTime;
                        HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC,
                                                 g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
                        CalculateLocalRotationMatrix(pHeroBody->pSekHeader_220->pFrameT3EC,
                                (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                                &dirForwardX, &dirForwardY, &dirForwardZ,
                                &dirUpX, &dirUpY, &dirUpZ);
                        SetFramePosRel_4069DD(pSekBody->pSekHeader_220->pFrameT3EC,
                                (CHanaFrameT3EC_S354 *)g_pSceneFrame, 1000.0f, 1000.0f, 1000.0f);
                    }
                    goto LABEL_84;
                }
                // nTransResult == 3: stage finished
                PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0f, 0);
                nMenuResult = 0;
                goto LABEL_125;
            }

LABEL_84:
            // scene clock + event script
            if (pScene)
            {
                UpdateSceneTimer_4827FE(pScene, &pfExecTime, fDeltaTime);
                UpdateSceneAtTime_4825D5(pScene, &pfExecTime, fDeltaTime);
                if (!bDialogueMode && pfExecTime > 1000.0f)
                {
                    pfExecTime = 0.0f;
                    pScene->dwPtrPos_444 = 0;
                    pScene->dwPtrCurrent_448 = 0;
                }
            }
            fTextSpeed = 1.0f;
            if (bDialogueMode && !bSkipMode)
            {
                if (ParseExecStr_487D89(pScene, &pfExecTime))
                {
                    if ((wBtnMouse == 3 || wBtnMouse == 1 && wBtnMouse != wPrevBtn)
                            && pScene->by_461 && !pScene->by_460)
                        pScene->by_461 = 0;
                    if (wBtnMouse == 1 && wBtnMouse != wPrevBtn)
                        fTextSpeed = 1000.0f;
                }
                else
                {
                    // script exhausted -> leave dialogue, restore the hero
                    SetProjectionFov_40DCA3(1.2f);
                    bDialogueMode = 0;
                    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
                    UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            dirForwardX, dirForwardY, dirForwardZ, dirUpX, dirUpY, dirUpZ);
                    AttachChild_4055FD(g_pSceneFrame, g_pCameraFrame);
                    pScene->by_461 = 0;
                    pScene->dwPtrPos_444 = 0;
                    pScene->dwPtrCurrent_448 = 0;
                    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC,
                            g_pCameraFrame, pMapFrame, fDeltaTime, 1);
                }
            }

            // background: dialogue still or the 3D map
            MtxStackStartMultLocal_4074EF();
            if (pScene && pScene->pSurface_468)
            {
                g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntialiasSave);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
                FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
                pScene->fAplhaCrossfade_480 = 1.0f;
                g_pBackBuffer_4D6C90->Blt(NULL, pScene->pSurface_468, NULL, DDBLT_WAIT, NULL);
                FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntialiasSave);
                g_pRenderCtx->Clear(2);
            }
            else
            {
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

            if (bDialogueMode)
            {
                // pose the hero at the cursor's body/frame
                pCursorBody = 0;
                if (pfExecTime >= 10000.0f)
                {
                    SekBody__UpdateTo2_401647(pHeroBody, 1);
                }
                else
                {
                    pCursorBody = pScene->refsSekBody_13C[pnCursor];
                    HanaFrame_GetRelPosition(pScene->refsFrames_134[pnCursor], g_pSceneFrame,
                                             &fTgtX, &pOutY, &pOutZ);
                    CalculateLocalRotationMatrix(pScene->refsFrames_134[pnCursor],
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            &pXpos, &pYpos, &pZpos, &pXYrot, &pYXrot, &pZXrot);
                    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame, fTgtX, pOutY, pOutZ);
                    UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            pXpos, pYpos, pZpos, pXYrot, pYXrot, pZXrot);
                    SekBody__UpdateTo2_401647(pHeroBody, pCursorBody->nCurItem_200);
                }
                CSekBody__UpdateAnim_4019E1(pHeroBody, fDeltaTime);
                if (!bSkipMode)
                {
                    sceneMenuCopy.frameTimeMultiplied_180 = fDeltaTime * fTextSpeed;
                    DrawDialogueWindow_480B2D(pScene, sceneMenuCopy.frameTimeMultiplied_180,
                                              xMouse, yMouse, wBtnMouse);
                }
            }
            else
            {
                UpdateHudGauge_44BC36(pSprTimes, (__int64)fHeroHp / 100, (__int64)fHeroHpMax / 100);
                DrawClockHud_44AF5B(pSprNumbers, 100, -5, 100 * nTimeLimit - (int)nElapsed / 10);
            }
            DrawSpriteListEcho_45B2FB(ppOut, nTargetSprite, fDeltaTime);

            if (bFirstFrameDone)
            {
                PresentFrame();
                HanaVert_DrawBatch(0);
                // HP gone or countdown over (and not mid-special-anim) -> exit
                if (!bDialogueMode
                  && fBlendTime <= 0.0f
                  && (100 * nTimeLimit - (int)nElapsed / 10 <= 0 || fHeroHp <= 0.0f)
                  && pHeroBody->nCurItem_200 != 16
                  && pHeroBody->nCurItem_200 != 10
                  && pHeroBody->nCurItem_200 != 11
                  && pHeroBody->nCurItem_200 != 13)
                {
                    goto LABEL_125;
                }
            }
            else
            {
                bFirstFrameDone = 1;
                PlayScreenFade(1, 0, g_pBackBuffer_4D6C90, 50.0f, 0);
                dwTimeBase += timeGetTime() - Time;
                Time = timeGetTime();
            }
        }  // frame loop

LABEL_125:
        bDialogueMode = 0;
        RestartInputThread(0);
        if (nMenuResult < 0)
        {
            if (fHeroHp <= 0.0f || 100 * nTimeLimit - (int)nElapsed / 10 <= 0)
            {
                if (nStageIndex == 7)
                {
                    g_sceneState_52F7E0.bActiveSceneStep = 80;   // jump past the chapter table
                    nMenuResult = 0;
                    goto LABEL_149;      // (skips the HP write-back below)
                }
                if (pBgmBuffer)
                    pBgmBuffer->Stop();
                if (pDSBuf[1])
                {
                    pDSBuf[1]->SetVolume(-500);
                    DSBufferPlay(pDSBuf[1], 0);          // "GameOver"
                }
                PrepareTargetSelect_45B5BD(pHeroBody, pItemBodies, pSpriteNames, pMapFrame,
                                           pSprNumbers, pSprTimes, fHeroHp, fHeroHpMax,
                                           nTimeLimit, nElapsed);
                nSelectResult = ShowMouseSelectDialog_4573CF(pSprModes, pDSBuf);
                PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0f, 0);
                if (!nSelectResult)
                {
                    nMenuResult = 1;
                    goto LABEL_149;      // (skips the HP write-back below)
                }
                // retry the stage with the newly picked target
                pnMapIndex = mapIndexTable[2 * nStageIndex];
                pnSubIndex = mapIndexTable[2 * nStageIndex + 1];
                memcpy(&g_sceneState_52F7E0, &Src, sizeof(g_sceneState_52F7E0));
                g_sceneState_52F7E0.protCurrentHP_C0 =
                        g_sceneState_52F7E0.protMaximumHP_C4 * 15.0f / 100.0f
                        + g_sceneState_52F7E0.protCurrentHP_C0;
                if (g_sceneState_52F7E0.protCurrentHP_C0 > (double)g_sceneState_52F7E0.protMaximumHP_C4)
                    g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4;
                fHeroHp = g_sceneState_52F7E0.protCurrentHP_C0;
                memcpy(&Src, &g_sceneState_52F7E0, sizeof(Src));
                g_sceneState_52F7E0.bCurrentSubStage = subStageTable[nStageIndex];
                g_sceneState_52F7E0.bMaxSubStage = subStageTable[nStageIndex] + 1;
                nTimeLimit = timeLimitTable[nStageIndex];
                SetProjectionFov_40DCA3(1.2f);
                SetTimerScaleFactor(14.0f);
                if (pBgmBuffer)
                    DSBufferPlay(pBgmBuffer, 1);
                dwTimeBase = timeGetTime();
                Time = timeGetTime();
            }
            if (pMapFrame)
            {
                CHanaObj__Free_40C1E4(pMapFrame);
                pMapFrame = 0;
            }
            if (pScene)
            {
                CHanaObj__Free_40C1E4(pScene);
                pScene = 0;
            }
            continue;                   // reload the (same) map
        }
        break;                          // leave the stage
    }

    if (!nMenuResult)
    {
        g_sceneState_52F7E0.protCurrentHP_C0 = fHeroHp;
        g_sceneState_52F7E0.protMaximumHP_C4 = fHeroHpMax;
    }

LABEL_149:
    RestartInputThread(0);
    for (i = 0; i < 10; ++i)
    {
        if (pDSBuf[i])
        {
            pDSBuf[i]->Stop();
            pDSBuf[i]->Release();
            pDSBuf[i] = 0;
        }
    }
    if (pBgmBuffer)
    {
        pBgmBuffer->Stop();
        pBgmBuffer->Release();
        pBgmBuffer = 0;
    }
    CSekBody__Release_4012FE(pHeroBody);
    CSekBody__Release_4012FE(pSekBody);
    for (i = 0; i < 10; ++i)
    {
        CSekBody__Release_4012FE(pItemBodies[i]);
        CSekBody__Release_4012FE(pSpriteNames[i]);
    }
    if (pScene)
    {
        CHanaObj__Free_40C1E4(pScene);
        pScene = 0;
    }
    if (pMapFrame)
    {
        CHanaObj__Free_40C1E4(pMapFrame);
        pMapFrame = 0;
    }
    for (i = 0; i < 20; ++i)
    {
        if (pSprTimes[i])
        {
            CHanaObj__Free_40C1E4(pSprTimes[i]);
            pSprTimes[i] = 0;
        }
    }
    for (i = 0; i < 15; ++i)
    {
        if (pSprNumbers[i])
        {
            CHanaObj__Free_40C1E4(pSprNumbers[i]);
            pSprNumbers[i] = 0;
        }
    }
    for (i = 0; i < 15; ++i)
    {
        if (pSprModes[i])
        {
            CHanaObj__Free_40C1E4(pSprModes[i]);
            pSprModes[i] = 0;
        }
    }
    for (i = 0; i < 10; ++i)
    {
        if (ppOut[i])
        {
            CHanaObj__Free_40C1E4(ppOut[i]);
            ppOut[i] = 0;
        }
    }
    DebugLog("[HNH][PlayExploring] normal return %d\n", nMenuResult);
    return nMenuResult;
}

// Chase/follow camera (the standard camera, 11 call sites). Mount point = hero + 22*localUp + 18*localBack (hero frame used as scratch, restored at end).
// bSnap=1: SNAP hard to the mount (used right after a manual reposition - PrepareTargetSelect, PlayExploring warp; skips smoothing AND orbit clamps).
// bSnap=0: smooth follow - lerp fTime/25 toward mount, step capped at fTime per frame, orbit clamps around a point 13 behind the hero (radius 3..10, height 15+/-10),
// then a 3-try wall pull-in settle loop (overhead test): pulls the camera toward the hero's true position (fHeroX/Y/Z) through walls; when blocked,
// TestMoveCollision snaps it to the contact point and it is lifted by 1/6 of the blocked distance to see over.
// Finally BuildLookAt aims the camera at hero + 15*up + 9*forward. The initial MtxMult(+1 localZ)/final (-1) pair is a nudge that cancels out;
// the trailing double hero-restore MtxMult is a dead duplicate.
void __cdecl hook_UpdateCameraFollowChase_445525(
  CHanaFrameT3EC_S354 *pHeroFrame,
  CHanaFrameT3EC_S354 *pCameraFrame,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fTime,
  int bSnap)
{
  double v5; // st7
  float fMountOffY; // [esp+0h] [ebp-70h]
  float fLiftedY; // [esp+0h] [ebp-70h]
  float fLookOffY; // [esp+0h] [ebp-70h]
  float fMountOffZ; // [esp+4h] [ebp-6Ch]
  float fLookOffZ; // [esp+4h] [ebp-6Ch]
  float fSnapY; // [esp+Ch] [ebp-64h] BYREF
  float fPreZ; // [esp+10h] [ebp-60h] BYREF
  float fLiftY; // [esp+14h] [ebp-5Ch] BYREF
  float fPreX; // [esp+18h] [ebp-58h] BYREF
  float fSnapX; // [esp+1Ch] [ebp-54h] BYREF
  float fSnapZ; // [esp+20h] [ebp-50h] BYREF
  float fCamPosX; // [esp+24h] [ebp-4Ch] BYREF
  float fOrbZ; // [esp+28h] [ebp-48h] BYREF
  float fCamStartZ; // [esp+2Ch] [ebp-44h]
  float fLerp; // [esp+30h] [ebp-40h]
  int iTry; // [esp+34h] [ebp-3Ch]
  float fMountZ; // [esp+38h] [ebp-38h] BYREF
  float fOrbY; // [esp+3Ch] [ebp-34h] BYREF
  float fDist; // [esp+40h] [ebp-30h]
  float fMountY; // [esp+44h] [ebp-2Ch] BYREF
  float fHeroZ; // [esp+48h] [ebp-28h] BYREF
  float fCamStartY; // [esp+4Ch] [ebp-24h]
  float fMountX; // [esp+50h] [ebp-20h] BYREF
  float fHeroY; // [esp+54h] [ebp-1Ch] BYREF
  float fHeroX; // [esp+58h] [ebp-18h] BYREF
  float fOrbX; // [esp+5Ch] [ebp-14h] BYREF
  float fHeightOff; // [esp+60h] [ebp-10h]
  float fCamPosZ; // [esp+64h] [ebp-Ch] BYREF
  float fCamStartX; // [esp+68h] [ebp-8h]
  float fCamPosY; // [esp+6Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateCameraFollowChase_445525\n");
  
  SetFramePosRel_4069DD(pCameraFrame, pCameraFrame, 0.0, 0.0, 1.0);
  HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fCamPosX, &fCamPosY, &fCamPosZ);
  fCamStartX = fCamPosX;
  fCamStartY = fCamPosY;
  fCamStartZ = fCamPosZ;
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
  HanaFrame_GetRelPosition(pHeroFrame, pHeroFrame, &fMountX, &fMountY, &fMountZ);
  fMountOffZ = fMountZ + 18.0;
  fMountOffY = fMountY + 22.0;
  SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, fMountX, fMountOffY, fMountOffZ);
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fMountX, &fMountY, &fMountZ);
  SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  fLerp = 1.0;
  if ( !bSnap )
  {
    fLerp = fTime / 25.0;
    if ( fLerp > 1.0 )
      fLerp = 1.0;
  }
  fMountX = (fMountX - fCamPosX) * fLerp + fCamPosX;
  fMountY = (fMountY - fCamPosY) * fLerp + fCamPosY;
  fMountZ = (fMountZ - fCamPosZ) * fLerp + fCamPosZ;
  if ( !bSnap )
  {
    v5 = sqrt(
      (fMountX - fCamPosX) * (fMountX - fCamPosX)
      + (fMountY - fCamPosY) * (fMountY - fCamPosY)
      + (fMountZ - fCamPosZ) * (fMountZ - fCamPosZ));
    fDist = v5;
    if ( v5 > fTime )
    {
      fMountX = (fMountX - fCamPosX) * fTime / fDist + fCamPosX;
      fMountY = (fMountY - fCamPosY) * fTime / fDist + fCamPosY;
      fMountZ = (fMountZ - fCamPosZ) * fTime / fDist + fCamPosZ;
    }
  }
  fCamPosX = fMountX;
  fCamPosY = fMountY;
  fCamPosZ = fMountZ;
  SetFramePosRel_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fMountX, fMountY, fMountZ);
  if ( !bSnap )
  {
    SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, 13.0);
    HanaFrame_GetRelPosition(pCameraFrame, pHeroFrame, &fOrbX, &fOrbY, &fOrbZ);
    fDist = sqrt(fOrbX * fOrbX + fOrbZ * fOrbZ);
    fHeightOff = sqrt((fOrbY - 15.0) * (fOrbY - 15.0));
    if ( fDist > 10.0 )
    {
      fOrbX = 10.0 / fDist * fOrbX;
      fOrbZ = 10.0 / fDist * fOrbZ;
      SetFramePosRel_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
    }
    if ( fHeightOff > 10.0 )
    {
      fOrbY = (fOrbY - 15.0) * 10.0 / fHeightOff + 15.0;
      SetFramePosRel_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
    }
    if ( fDist < 3.0 && fHeightOff < 5.0 )
    {
      if ( fDist < 3.0 && fDist != 0.0 )
      {
        fOrbX = 3.0 / fDist * fOrbX;
        fOrbZ = 3.0 / fDist * fOrbZ;
      }
      if ( fHeightOff < 5.0 && fHeightOff != 0.0 )
        fOrbY = (fOrbY - 15.0) * 5.0 / fHeightOff + 15.0;
      SetFramePosRel_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
    }
    HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fCamPosX, &fCamPosY, &fCamPosZ);
    SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -10.0);
  }
  for ( iTry = 0; iTry < 3 && !CheckFrameCollision_443CC5(pCameraFrame, pMapFrame, 1); ++iTry )
  {
    HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fPreX, &fLiftY, &fPreZ);
    if ( TestMoveCollision_4443FC(pCameraFrame, pMapFrame, fHeroX, fHeroY, fHeroZ, 1) )
    {
      HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fSnapX, &fSnapY, &fSnapZ);
      fLiftY = sqrt((fPreX - fSnapX) * (fPreX - fSnapX) + (fPreZ - fSnapZ) * (fPreZ - fSnapZ)) / 6.0;
      fLiftedY = fSnapY + fLiftY;
      SetFramePosRel_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fSnapX, fLiftedY, fSnapZ);
    }
    else
    {
      SetFramePosRel_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fCamStartX, fCamStartY, fCamStartZ);
    }
  }
  HanaFrame_GetRelPosition(pHeroFrame, pHeroFrame, &fMountX, &fMountY, &fMountZ);
  fLookOffZ = fMountZ - 9.0;
  fLookOffY = fMountY + 15.0;
  SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, fMountX, fLookOffY, fLookOffZ);
  HanaMatrix_BuildLookAtAndTransform(pCameraFrame, pHeroFrame, pCameraFrame, 0);
  SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  SetFramePosRel_4069DD(pCameraFrame, pCameraFrame, 0.0, 0.0, -1.0);
}

void __cdecl hook_HanaMatrix_BuildLookAtAndTransform(
  CHanaFrameT3EC_S354 *pFoeFrame,
  CHanaFrameT3EC_S354 *pHeroFrame,
  CHanaFrameRootT3EC_S2D4 *pSceneFrame,
  BOOL bSkip)
{
  D3DXVECTOR3 fUpTemp_x; // [esp+0h] [ebp-7Ch]
  D3DXVECTOR3 fRightTemp_x; // [esp+Ch] [ebp-70h]
  D3DXMATRIX mInverse; // [esp+18h] [ebp-64h] BYREF
  D3DXVECTOR3 vRight; // [esp+58h] [ebp-24h] BYREF
  D3DXVECTOR3 vLook; // [esp+64h] [ebp-18h] BYREF
  D3DXVECTOR3 vUp; // [esp+70h] [ebp-Ch] BYREF

  ONCELOG("[HNH:SUB] HanaMatrix_BuildLookAtAndTransform_407B73\n");
 
  if ( !bSkip )
  {
    vLook.x = pHeroFrame->matWorld_178._41 - pFoeFrame->matWorld_178._41;
    vLook.y = pHeroFrame->matWorld_178._42 - pFoeFrame->matWorld_178._42;
    vLook.z = pHeroFrame->matWorld_178._43 - pFoeFrame->matWorld_178._43;
    D3DXVec3Normalize(&vLook, &vLook);
    vUp.x = 0.0;
    vUp.y = 1.0;
    vUp.z = 0.0;
    fRightTemp_x.x = 1.0 * vLook.z - 0.0 * vLook.y;
    fRightTemp_x.y = 0.0 * vLook.x - 0.0 * vLook.z;
    fRightTemp_x.z = 0.0 * vLook.y - 1.0 * vLook.x;
    vRight = fRightTemp_x;
    D3DXVec3Normalize(&vRight, &vRight);
    fUpTemp_x.x = vLook.y * vRight.z - vLook.z * vRight.y;
    fUpTemp_x.y = vLook.z * vRight.x - vLook.x * vRight.z;
    fUpTemp_x.z = vLook.x * vRight.y - vLook.y * vRight.x;
    vUp = fUpTemp_x;
    D3DXVec3Normalize(&vUp, &vUp);
    pFoeFrame->matWorld_178._11 = vRight.x;
    pFoeFrame->matWorld_178._21 = vUp.x;
    pFoeFrame->matWorld_178._31 = vLook.x;
    pFoeFrame->matWorld_178._12 = vRight.y;
    pFoeFrame->matWorld_178._22 = vUp.y;
    pFoeFrame->matWorld_178._32 = vLook.y;
    pFoeFrame->matWorld_178._13 = vRight.z;
    pFoeFrame->matWorld_178._23 = vUp.z;
    pFoeFrame->matWorld_178._33 = vLook.z;
    D3DXMatrixInverse(&mInverse, 0, &pFoeFrame->matParentWorld_1B8);
    D3DXMatrixMultiply(&pFoeFrame->matLocal_138, &pFoeFrame->matWorld_178, &mInverse);
  }
}

void __cdecl hook_CHanaAnimSetT3F1__RemoveAnimT3F0_404349(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)
{
  CHanaAnimT3F0_S240 **pOldAnimRefs; // [esp+0h] [ebp-8h]
  unsigned int foundIdx; // [esp+4h] [ebp-4h]
  ONCELOG("[HNH:SUB] CHanaAnimSetT3F1__RemoveAnimT3F0_404349\n");
  
  if ( pAnimSet->nAnimCount_134 )
  {
    for ( foundIdx = 0; foundIdx < pAnimSet->nAnimCount_134; ++foundIdx )
    {
      if ( pAnimSet->refsAnimT3F0_138[foundIdx] == pAnim )
      {
        if ( pAnimSet->nAnimCount_134 <= 1u )
        {
          MemFree_40DF64(pAnimSet->refsAnimT3F0_138);
          pAnimSet->refsAnimT3F0_138 = 0;
          pAnimSet->nAnimCount_134 = 0;
        }
        else
        {
          pOldAnimRefs = pAnimSet->refsAnimT3F0_138;
          pAnimSet->refsAnimT3F0_138 = 0;
          pAnimSet->refsAnimT3F0_138 = (CHanaAnimT3F0_S240 **)MemAlloc_40DEF7(4 * pAnimSet->nAnimCount_134 + 64);
          memcpy(pAnimSet->refsAnimT3F0_138, pOldAnimRefs, 4 * foundIdx + 4);
          pAnimSet->refsAnimT3F0_138[foundIdx] = 0;
          memcpy(
            &pAnimSet->refsAnimT3F0_138[foundIdx],
            &pOldAnimRefs[foundIdx + 1],
            4 * (pAnimSet->nAnimCount_134 - foundIdx) - 4);
          MemFree_40DF64(pOldAnimRefs);
          --pAnimSet->nAnimCount_134;
        }
        CHanaObj__Free_40C1E4(pAnim);
        return;
      }
    }
  }
  else
  {
    pAnimSet->refsAnimT3F0_138 = 0;
  }
}

// Rebind/reset camera-linked objects after an ODS scene load. Walks g_pObjTable; for each object of dwType_204==1009, 
// calls CHanaAnimSetT3F1__RemoveAnimT3F0_404349 on the sub-objects
// whose [304] slot equals nCameraId (the freshly loaded camera frame).
void __cdecl hook_UpdateCameraLinkedObjects_44451F(CHanaFrameT3EC_S354 *nCameraId)
{
  int j; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  CObjItemRef_S208 *pObjItem; // [esp+8h] [ebp-8h]
  CHanaAnimSetT3F1_S150 *pAnimSet; // [esp+Ch] [ebp-4h]

  ONCELOG("[HNH:SUB] UpdateCameraLinkedObjects_44451F\n");
  
  pObjItem = g_pObjectTable;
  for ( i = 0; i < g_nObjectTableCount; ++i )
  {
    if ( pObjItem->dwObjType_204 == 0x3F1 )
    {
      pAnimSet = (CHanaAnimSetT3F1_S150 *)pObjItem->pObj_200;
      for ( j = 0; j < pAnimSet->nAnimCount_134; ++j )
      {
        if ( pAnimSet->refsAnimT3F0_138[j]->pHanaFram_130 == nCameraId )
          CHanaAnimSetT3F1__RemoveAnimT3F0_404349(pAnimSet, pAnimSet->refsAnimT3F0_138[j]);
      }
    }
    ++pObjItem;
  }
}

// Recursively expand a parsed ODF map frame into child frame nodes: marks [0]=1, normalizes via Frame_ComputeWorldAABB, builds the T3ED representation
// (CHanaObj::NewT3EDFromChildT3EC), and for each frame recurses then frees the temp T3ED. Called right after ParseODFFile on the map frame.
void __cdecl hook_BuildMapSubFrames_443DD5(CHanaFrameT3EC_S354 *pFrame)
{
  unsigned int nIndex; // [esp+4h] [ebp-10h]
  CHanaFrameT3EC_S354 *pFrameAtIndex; // [esp+8h] [ebp-Ch] BYREF
  CHanaFrameArrayT3ED_S138 *pFrameArray; // [esp+Ch] [ebp-8h] BYREF
  unsigned int FramesCountT3ED_408E75; // [esp+10h] [ebp-4h]
  ONCELOG("[HNH:SUB] BuildMapSubFrames_443DD5\n");
  
  FramesCountT3ED_408E75 = 0;
  if ( pFrame )
  {
    pFrame->bAlphaDeferred_130 = 1;
    Frame_ComputeWorldAABB(pFrame);
    CHanaObj__NewFrameArrayT3EDFromChildT3EC(pFrame, &pFrameArray);
    if ( pFrameArray )
    {
      FramesCountT3ED_408E75 = CHanaObj__GetFramesCountT3ED_408E75(pFrameArray);
      for ( nIndex = 0; nIndex < FramesCountT3ED_408E75; ++nIndex )
      {
        CHanaObj__GetFrameAtIndexT3ED(pFrameArray, nIndex, &pFrameAtIndex);
        if ( pFrameAtIndex )
        {
          Frame_ComputeWorldAABB(pFrameAtIndex);
          BuildMapSubFrames_443DD5(pFrameAtIndex);
          if ( pFrameAtIndex )
          {
            CHanaObj__Free_40C1E4(pFrameAtIndex);
            pFrameAtIndex = 0;
          }
        }
      }
      if ( pFrameArray )
        CHanaObj__Free_40C1E4(pFrameArray);
    }
  }
}

void __cdecl hook_Frame_ComputeWorldAABB(CHanaFrameT3EC_S354 *pFrame)
{
  D3DXVECTOR3 *pVertexOut; // edx
  D3DXVECTOR3 *pTrasnformedCorner; // eax
  int j; // [esp+0h] [ebp-294h]
  D3DXVECTOR3 *m; // [esp+8h] [ebp-28Ch]
  int i; // [esp+Ch] [ebp-288h]
  D3DXMATRIX *k; // [esp+14h] [ebp-280h]
  D3DXMATRIX cornerMatrices[8]; // [esp+18h] [ebp-27Ch] BYREF
  D3DXVECTOR3 maxBound; // [esp+218h] [ebp-7Ch]
  D3DXVECTOR3 transformed[8]; // [esp+224h] [ebp-70h] BYREF
  int n; // [esp+284h] [ebp-10h]
  D3DXVECTOR3 minBound; // [esp+288h] [ebp-Ch]

  ONCELOG("[HNH:SUB] Frame_ComputeWorldAABB_407E5B\n");
 
  i = 8;
  for ( k = cornerMatrices; --i >= 0; ++k )
    ;
  j = 8;
  for ( m = transformed; --j >= 0; ++m )
    ;
  maxBound = pFrame->vertBoundMax_220;
  minBound = pFrame->vertBoundMin_22C;
  if ( maxBound.x == minBound.x )
  {
    maxBound.x = maxBound.x - 0.00050000002;
    minBound.x = minBound.x + 0.00050000002;
  }
  if ( maxBound.y == minBound.y )
  {
    maxBound.y = maxBound.y - 0.00050000002;
    minBound.y = minBound.y + 0.00050000002;
  }
  if ( maxBound.z == minBound.z )
  {
    maxBound.z = maxBound.z - 0.00050000002;
    minBound.z = minBound.z + 0.00050000002;
  }
  D3DXMatrixTranslation(cornerMatrices, maxBound.x, minBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerMatrices[1], maxBound.x, minBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerMatrices[2], minBound.x, minBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerMatrices[3], minBound.x, minBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerMatrices[4], maxBound.x, maxBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerMatrices[5], maxBound.x, maxBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerMatrices[6], minBound.x, maxBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerMatrices[7], minBound.x, maxBound.y, maxBound.z);
  for ( n = 0; n < 8; ++n )
  {
    D3DXMatrixMultiply(&cornerMatrices[n], &cornerMatrices[n], &pFrame->matWorld_178);
    transformed[n].x = cornerMatrices[0].m[4 * n + 3][0];
    transformed[n].y = cornerMatrices[0].m[4 * n + 3][1];
    transformed[n].z = cornerMatrices[0].m[4 * n + 3][2];
    pVertexOut = &transformed[n];
    pTrasnformedCorner = &pFrame->transformedCorners[n];
    pTrasnformedCorner->x = pVertexOut->x;
    pTrasnformedCorner->y = pVertexOut->y;
    pTrasnformedCorner->z = pVertexOut->z;
  }
  pFrame->worldAABBMax = transformed[0];
  pFrame->worldAABBMin = pFrame->worldAABBMax;
  for ( n = 1; n < 8; ++n )
  {
    if ( transformed[n].x < (double)pFrame->worldAABBMin.x )
      pFrame->worldAABBMin.x = transformed[n].x;
    if ( transformed[n].y < (double)pFrame->worldAABBMin.y )
      pFrame->worldAABBMin.y = transformed[n].y;
    if ( transformed[n].z < (double)pFrame->worldAABBMin.z )
      pFrame->worldAABBMin.z = transformed[n].z;
    if ( transformed[n].x > (double)pFrame->worldAABBMax.x )
      pFrame->worldAABBMax.x = transformed[n].x;
    if ( transformed[n].y > (double)pFrame->worldAABBMax.y )
      pFrame->worldAABBMax.y = transformed[n].y;
    if ( transformed[n].z > (double)pFrame->worldAABBMax.z )
      pFrame->worldAABBMax.z = transformed[n].z;
  }
}

// Initialize the battle HUD / select resource arrays (three int* tables, flag=1 at start): the clock-HUD table fed to DrawClockHud, the gauge table fed to
// UpdateHudGauge, and the mouse-select table fed to ShowMouseSelectDialog. Large setup routine; called once at battle start.
CHanaSpriteT3F6_S1FC *__cdecl hook_InitHudResources_4490A7(
  CHanaSpriteT3F6_S1FC **arrSprNumbers,
  CHanaSpriteT3F6_S1FC **arrSprTimes,
  CHanaSpriteT3F6_S1FC **arrSprModes,
  int nMode)
{
  CHanaSpriteT3F6_S1FC *result; // eax
 
  CHAR FileName[256]; // [esp+6E0h] [ebp-6ECh] BYREF
  int i; // [esp+7E0h] [ebp-5ECh]
  CHanaSpriteT3F6_S1FC *pSprite_v63; // [esp+7E4h] [ebp-5E8h] BYREF
  CHAR szPath[256]; // [esp+7E8h] [ebp-5E4h] BYREF
  CHanaSpriteT3F6_S1FC **arrSprTimes_a2a; // [esp+DD8h] [ebp+Ch]
  CHanaSpriteT3F6_S1FC **arrSprLifes_v99; // [esp+DD8h] [ebp+Ch]

  ONCELOG("[HNH:SUB] InitHudResources_4490A7\n");
  
  pSprite_v63 = 0;

#if 1
  static const char *arrBmpNumbers_v49[15] = {
    "0.BMP", "1.BMP", "2.BMP", "3.BMP", "4.BMP", "5.BMP", "6.BMP", "7.BMP", "8.BMP", "9.BMP",
    "c.BMP",                                         // clock 'c' digit
    "ef01.bmp", "ef02.bmp", "ef03.bmp", "ef03.bmp",  // NOTE: [14] dup of [13] - ef03 masks itself
  };
#else
  String25 arrBmpNumbers_v49[15]; // [esp+568h] [ebp-864h] BYREF

  strcpy((char *)arrBmpNumbers_v49[0].str, "0.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[0].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[0].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[0].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[0].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[0].str[22] = 0;
  arrBmpNumbers_v49[0].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[1].str, "1.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[1].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[1].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[1].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[1].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[1].str[22] = 0;
  arrBmpNumbers_v49[1].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[2].str, "2.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[2].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[2].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[2].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[2].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[2].str[22] = 0;
  arrBmpNumbers_v49[2].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[3].str, "3.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[3].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[3].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[3].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[3].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[3].str[22] = 0;
  arrBmpNumbers_v49[3].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[4].str, "4.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[4].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[4].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[4].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[4].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[4].str[22] = 0;
  arrBmpNumbers_v49[4].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[5].str, "5.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[5].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[5].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[5].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[5].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[5].str[22] = 0;
  arrBmpNumbers_v49[5].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[6].str, "6.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[6].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[6].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[6].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[6].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[6].str[22] = 0;
  arrBmpNumbers_v49[6].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[7].str, "7.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[7].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[7].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[7].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[7].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[7].str[22] = 0;
  arrBmpNumbers_v49[7].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[8].str, "8.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[8].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[8].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[8].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[8].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[8].str[22] = 0;
  arrBmpNumbers_v49[8].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[9].str, "9.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[9].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[9].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[9].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[9].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[9].str[22] = 0;
  arrBmpNumbers_v49[9].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[10].str, "c.BMP");
  *(_DWORD *)&arrBmpNumbers_v49[10].str[6] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[10].str[10] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[10].str[14] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[10].str[18] = 0;
  *(_WORD *)&arrBmpNumbers_v49[10].str[22] = 0;
  arrBmpNumbers_v49[10].str[24] = 0;
  strcpy((char *)arrBmpNumbers_v49[11].str, "ef01.bmp");
  *(_DWORD *)&arrBmpNumbers_v49[11].str[9] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[11].str[13] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[11].str[17] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[11].str[21] = 0;
  strcpy((char *)arrBmpNumbers_v49[12].str, "ef02.bmp");
  *(_DWORD *)&arrBmpNumbers_v49[12].str[9] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[12].str[13] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[12].str[17] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[12].str[21] = 0;
  strcpy((char *)arrBmpNumbers_v49[13].str, "ef03.bmp");
  *(_DWORD *)&arrBmpNumbers_v49[13].str[9] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[13].str[13] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[13].str[17] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[13].str[21] = 0;
  strcpy((char *)arrBmpNumbers_v49[14].str, "ef03.bmp");
  *(_DWORD *)&arrBmpNumbers_v49[14].str[9] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[14].str[13] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[14].str[17] = 0;
  *(_DWORD *)&arrBmpNumbers_v49[14].str[21] = 0;
#endif

#if 1
  static const char *arrBmpTimes_v32[15] = {
    "run\\timeA.BMP",  "run\\timeB.BMP",   // clock body/mask
    "run\\lifeA.BMP",  "run\\lifeB.BMP",   // life gauge body/mask
    "run\\Ber.BMP",                          // plain texture, no mask
    "run\\rendaA.bmp", "run\\rendaB.bmp",  // combo (renda) body/mask
    "run\\button01.bmp", "run\\button03.bmp",
    "run\\button05.bmp", "run\\button06.bmp",
    "run\\button02.bmp", "run\\button04.bmp",
    "run\\button05.bmp", "run\\button06.bmp",  // [13]/[14] repeat [9]/[10] - 3rd button pair reuses 05/06
  };
#else
  String25 arrBmpTimes_v32[15]; // [esp+2F4h] [ebp-AD8h] BYREF

  strcpy((char *)arrBmpTimes_v32[0].str, "run\\timeA.BMP");
  *(_DWORD *)&arrBmpTimes_v32[0].str[14] = 0;
  *(_DWORD *)&arrBmpTimes_v32[0].str[18] = 0;
  *(_WORD *)&arrBmpTimes_v32[0].str[22] = 0;
  arrBmpTimes_v32[0].str[24] = 0;
  strcpy((char *)arrBmpTimes_v32[1].str, "run\\timeB.BMP");
  *(_DWORD *)&arrBmpTimes_v32[1].str[14] = 0;
  *(_DWORD *)&arrBmpTimes_v32[1].str[18] = 0;
  *(_WORD *)&arrBmpTimes_v32[1].str[22] = 0;
  arrBmpTimes_v32[1].str[24] = 0;
  strcpy((char *)arrBmpTimes_v32[2].str, "run\\lifeA.BMP");
  *(_DWORD *)&arrBmpTimes_v32[2].str[14] = 0;
  *(_DWORD *)&arrBmpTimes_v32[2].str[18] = 0;
  *(_WORD *)&arrBmpTimes_v32[2].str[22] = 0;
  arrBmpTimes_v32[2].str[24] = 0;
  strcpy((char *)arrBmpTimes_v32[3].str, "run\\lifeB.BMP");
  *(_DWORD *)&arrBmpTimes_v32[3].str[14] = 0;
  *(_DWORD *)&arrBmpTimes_v32[3].str[18] = 0;
  *(_WORD *)&arrBmpTimes_v32[3].str[22] = 0;
  arrBmpTimes_v32[3].str[24] = 0;
  strcpy((char *)arrBmpTimes_v32[4].str, "run\\Ber.BMP");
  *(_DWORD *)&arrBmpTimes_v32[4].str[12] = 0;
  *(_DWORD *)&arrBmpTimes_v32[4].str[16] = 0;
  *(_DWORD *)&arrBmpTimes_v32[4].str[20] = 0;
  arrBmpTimes_v32[4].str[24] = 0;
  strcpy((char *)arrBmpTimes_v32[5].str, "run\\rendaA.bmp");
  *(_DWORD *)&arrBmpTimes_v32[5].str[15] = 0;
  *(_DWORD *)&arrBmpTimes_v32[5].str[19] = 0;
  *(_WORD *)&arrBmpTimes_v32[5].str[23] = 0;
  strcpy((char *)arrBmpTimes_v32[6].str, "run\\rendaB.bmp");
  *(_DWORD *)&arrBmpTimes_v32[6].str[15] = 0;
  *(_DWORD *)&arrBmpTimes_v32[6].str[19] = 0;
  *(_WORD *)&arrBmpTimes_v32[6].str[23] = 0;
  strcpy((char *)arrBmpTimes_v32[7].str, "run\\button01.bmp");
  *(_DWORD *)&arrBmpTimes_v32[7].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[7].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[8].str, "run\\button03.bmp");
  *(_DWORD *)&arrBmpTimes_v32[8].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[8].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[9].str, "run\\button05.bmp");
  *(_DWORD *)&arrBmpTimes_v32[9].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[9].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[10].str, "run\\button06.bmp");
  *(_DWORD *)&arrBmpTimes_v32[10].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[10].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[11].str, "run\\button02.bmp");
  *(_DWORD *)&arrBmpTimes_v32[11].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[11].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[12].str, "run\\button04.bmp");
  *(_DWORD *)&arrBmpTimes_v32[12].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[12].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[13].str, "run\\button05.bmp");
  *(_DWORD *)&arrBmpTimes_v32[13].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[13].str[21] = 0;
  strcpy((char *)arrBmpTimes_v32[14].str, "run\\button06.bmp");
  *(_DWORD *)&arrBmpTimes_v32[14].str[17] = 0;
  *(_DWORD *)&arrBmpTimes_v32[14].str[21] = 0;
#endif

#if 1
  static const char *arrBmpModes_v5[28] = {
    "mode03.bmp", "mode04.bmp",
    "hunt01.bmp", "hunt02.bmp", "hunt03.bmp", "hunt04.bmp",
    "hunt05.bmp", "hunt06.bmp", "hunt07.bmp", "hunt08.bmp",
    "hunt09.bmp", "hunt10.bmp",
    "mode01.bmp", "mode02.bmp",
    "huntXX2.bmp", "huntXX1.bmp",
    "run\\cont01.bmp", "run\\cont02.bmp",
    "run\\yes01.bmp", "run\\yes02.bmp", "run\\no01.bmp", "run\\no02.bmp",
    "run\\yes03.bmp", "run\\yes04.bmp", "run\\no03.bmp", "run\\no04.bmp",
    "", "",                                   // NOTE: empty 14th pair - table slots 26..29 were zero-filled
  };
#else
  String25 arrBmpModes_v5[30]; // [esp+4h] [ebp-DC8h] BYREF
  _BYTE v7[250]; // [esp+46Bh] [ebp-961h] BYREF

  memset(v7, 0, sizeof(v7));

  strcpy((char *)arrBmpModes_v5[0].str, "mode03.bmp");
  *(_DWORD *)&arrBmpModes_v5[0].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[0].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[0].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[0].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[1].str, "mode04.bmp");
  *(_DWORD *)&arrBmpModes_v5[1].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[1].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[1].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[1].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[2].str, "hunt01.bmp");
  *(_DWORD *)&arrBmpModes_v5[2].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[2].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[2].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[2].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[3].str, "hunt02.bmp");
  *(_DWORD *)&arrBmpModes_v5[3].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[3].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[3].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[3].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[4].str, "hunt03.bmp");
  *(_DWORD *)&arrBmpModes_v5[4].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[4].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[4].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[4].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[5].str, "hunt04.bmp");
  *(_DWORD *)&arrBmpModes_v5[5].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[5].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[5].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[5].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[6].str, "hunt05.bmp");
  *(_DWORD *)&arrBmpModes_v5[6].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[6].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[6].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[6].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[7].str, "hunt06.bmp");
  *(_DWORD *)&arrBmpModes_v5[7].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[7].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[7].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[7].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[8].str, "hunt07.bmp");
  *(_DWORD *)&arrBmpModes_v5[8].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[8].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[8].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[8].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[9].str, "hunt08.bmp");
  *(_DWORD *)&arrBmpModes_v5[9].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[9].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[9].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[9].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[10].str, "hunt09.bmp");
  *(_DWORD *)&arrBmpModes_v5[10].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[10].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[10].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[10].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[11].str, "hunt10.bmp");
  *(_DWORD *)&arrBmpModes_v5[11].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[11].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[11].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[11].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[12].str, "mode01.bmp");
  *(_DWORD *)&arrBmpModes_v5[12].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[12].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[12].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[12].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[13].str, "mode02.bmp");
  *(_DWORD *)&arrBmpModes_v5[13].str[11] = 0;
  *(_DWORD *)&arrBmpModes_v5[13].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[13].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[13].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[14].str, "huntXX2.bmp");
  *(_DWORD *)&arrBmpModes_v5[14].str[12] = 0;
  *(_DWORD *)&arrBmpModes_v5[14].str[16] = 0;
  *(_DWORD *)&arrBmpModes_v5[14].str[20] = 0;
  arrBmpModes_v5[14].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[15].str, "huntXX1.bmp");
  *(_DWORD *)&arrBmpModes_v5[15].str[12] = 0;
  *(_DWORD *)&arrBmpModes_v5[15].str[16] = 0;
  *(_DWORD *)&arrBmpModes_v5[15].str[20] = 0;
  arrBmpModes_v5[15].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[16].str, "run\\cont01.bmp");
  *(_DWORD *)&arrBmpModes_v5[16].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[16].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[16].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[17].str, "run\\cont02.bmp");
  *(_DWORD *)&arrBmpModes_v5[17].str[15] = 0;
  *(_DWORD *)&arrBmpModes_v5[17].str[19] = 0;
  *(_WORD *)&arrBmpModes_v5[17].str[23] = 0;
  strcpy((char *)arrBmpModes_v5[18].str, "run\\yes01.bmp");
  *(_DWORD *)&arrBmpModes_v5[18].str[14] = 0;
  *(_DWORD *)&arrBmpModes_v5[18].str[18] = 0;
  *(_WORD *)&arrBmpModes_v5[18].str[22] = 0;
  arrBmpModes_v5[18].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[19].str, "run\\yes02.bmp");
  *(_DWORD *)&arrBmpModes_v5[19].str[14] = 0;
  *(_DWORD *)&arrBmpModes_v5[19].str[18] = 0;
  *(_WORD *)&arrBmpModes_v5[19].str[22] = 0;
  arrBmpModes_v5[19].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[20].str, "run\\no01.bmp");
  *(_DWORD *)&arrBmpModes_v5[20].str[13] = 0;
  *(_DWORD *)&arrBmpModes_v5[20].str[17] = 0;
  *(_DWORD *)&arrBmpModes_v5[20].str[21] = 0;
  strcpy((char *)arrBmpModes_v5[21].str, "run\\no02.bmp");
  *(_DWORD *)&arrBmpModes_v5[21].str[13] = 0;
  *(_DWORD *)&arrBmpModes_v5[21].str[17] = 0;
  *(_DWORD *)&arrBmpModes_v5[21].str[21] = 0;
  strcpy((char *)arrBmpModes_v5[22].str, "run\\yes03.bmp");
  *(_DWORD *)&arrBmpModes_v5[22].str[14] = 0;
  *(_DWORD *)&arrBmpModes_v5[22].str[18] = 0;
  *(_WORD *)&arrBmpModes_v5[22].str[22] = 0;
  arrBmpModes_v5[22].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[23].str, "run\\yes04.bmp");
  *(_DWORD *)&arrBmpModes_v5[23].str[14] = 0;
  *(_DWORD *)&arrBmpModes_v5[23].str[18] = 0;
  *(_WORD *)&arrBmpModes_v5[23].str[22] = 0;
  arrBmpModes_v5[23].str[24] = 0;
  strcpy((char *)arrBmpModes_v5[24].str, "run\\no03.bmp");
  *(_DWORD *)&arrBmpModes_v5[24].str[13] = 0;
  *(_DWORD *)&arrBmpModes_v5[24].str[17] = 0;
  *(_DWORD *)&arrBmpModes_v5[24].str[21] = 0;
  strcpy((char *)arrBmpModes_v5[25].str, "run\\no04.bmp");
  *(_DWORD *)&arrBmpModes_v5[25].str[13] = 0;
  *(_DWORD *)&arrBmpModes_v5[25].str[17] = 0;
  *(_DWORD *)&arrBmpModes_v5[25].str[21] = 0;
  memset(&arrBmpModes_v5[26], 0, 100u);
#endif


#if 1
  // [0]=ofa body, [1]=lifeC mask, then five 3-face gauge groups (saf/maf/tsf/mif/tof),
  // every face paired with the shared timeC mask; nMode 0..4 selects the group.
  static const char *szOfaBMP_v66[32] = {
    "run\\ofa.bmp",  "run\\lifeC.BMP",
    "run\\safa.bmp", "run\\timeC.BMP", "run\\safb.bmp", "run\\timeC.BMP", "run\\safc.bmp", "run\\timeC.BMP",
    "run\\mafa.bmp", "run\\timeC.BMP", "run\\mafb.bmp", "run\\timeC.BMP", "run\\mafc.bmp", "run\\timeC.BMP",
    "run\\tsfa.bmp", "run\\timeC.BMP", "run\\tsfb.bmp", "run\\timeC.BMP", "run\\tsfc.bmp", "run\\timeC.BMP",
    "run\\mifa.bmp", "run\\timeC.BMP", "run\\mifb.bmp", "run\\timeC.BMP", "run\\mifc.bmp", "run\\timeC.BMP",
    "run\\tofa.bmp", "run\\timeC.BMP", "run\\tofb.bmp", "run\\timeC.BMP", "run\\tofc.bmp", "run\\timeC.BMP",
  };
#else
  String25 szOfaBMP_v66[32]; // [esp+8E8h] [ebp-4E4h] BYREF

  _BYTE v14[448]; // [esp+C08h] [ebp-1C4h] BYREF
  __int16 v15; // [esp+DC8h] [ebp-4h]

  strcpy((char *)szOfaBMP_v66[0].str, "run\\ofa.bmp");
  *(_DWORD *)&szOfaBMP_v66[0].str[12] = 0;
  *(_DWORD *)&szOfaBMP_v66[0].str[16] = 0;
  *(_DWORD *)&szOfaBMP_v66[0].str[20] = 0;
  szOfaBMP_v66[0].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[1].str, "run\\lifeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[1].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[1].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[1].str[22] = 0;
  szOfaBMP_v66[1].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[2].str, "run\\safa.bmp");
  *(_DWORD *)&szOfaBMP_v66[2].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[2].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[2].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[3].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[3].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[3].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[3].str[22] = 0;
  szOfaBMP_v66[3].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[4].str, "run\\safb.bmp");
  *(_DWORD *)&szOfaBMP_v66[4].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[4].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[4].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[5].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[5].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[5].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[5].str[22] = 0;
  szOfaBMP_v66[5].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[6].str, "run\\safc.bmp");
  *(_DWORD *)&szOfaBMP_v66[6].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[6].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[6].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[7].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[7].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[7].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[7].str[22] = 0;
  szOfaBMP_v66[7].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[8].str, "run\\mafa.bmp");
  *(_DWORD *)&szOfaBMP_v66[8].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[8].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[8].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[9].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[9].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[9].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[9].str[22] = 0;
  szOfaBMP_v66[9].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[10].str, "run\\mafb.bmp");
  *(_DWORD *)&szOfaBMP_v66[10].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[10].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[10].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[11].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[11].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[11].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[11].str[22] = 0;
  szOfaBMP_v66[11].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[12].str, "run\\mafc.bmp");
  *(_DWORD *)&szOfaBMP_v66[12].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[12].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[12].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[13].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[13].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[13].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[13].str[22] = 0;
  szOfaBMP_v66[13].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[14].str, "run\\tsfa.bmp");
  *(_DWORD *)&szOfaBMP_v66[14].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[14].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[14].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[15].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[15].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[15].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[15].str[22] = 0;
  szOfaBMP_v66[15].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[16].str, "run\\tsfb.bmp");
  *(_DWORD *)&szOfaBMP_v66[16].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[16].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[16].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[17].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[17].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[17].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[17].str[22] = 0;
  szOfaBMP_v66[17].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[18].str, "run\\tsfc.bmp");
  *(_DWORD *)&szOfaBMP_v66[18].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[18].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[18].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[19].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[19].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[19].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[19].str[22] = 0;
  szOfaBMP_v66[19].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[20].str, "run\\mifa.bmp");
  *(_DWORD *)&szOfaBMP_v66[20].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[20].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[20].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[21].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[21].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[21].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[21].str[22] = 0;
  szOfaBMP_v66[21].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[22].str, "run\\mifb.bmp");
  *(_DWORD *)&szOfaBMP_v66[22].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[22].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[22].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[23].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[23].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[23].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[23].str[22] = 0;
  szOfaBMP_v66[23].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[24].str, "run\\mifc.bmp");
  *(_DWORD *)&szOfaBMP_v66[24].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[24].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[24].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[25].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[25].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[25].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[25].str[22] = 0;
  szOfaBMP_v66[25].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[26].str, "run\\tofa.bmp");
  *(_DWORD *)&szOfaBMP_v66[26].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[26].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[26].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[27].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[27].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[27].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[27].str[22] = 0;
  szOfaBMP_v66[27].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[28].str, "run\\tofb.bmp");
  *(_DWORD *)&szOfaBMP_v66[28].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[28].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[28].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[29].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[29].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[29].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[29].str[22] = 0;
  szOfaBMP_v66[29].str[24] = 0;
  strcpy((char *)szOfaBMP_v66[30].str, "run\\tofc.bmp");
  *(_DWORD *)&szOfaBMP_v66[30].str[13] = 0;
  *(_DWORD *)&szOfaBMP_v66[30].str[17] = 0;
  *(_DWORD *)&szOfaBMP_v66[30].str[21] = 0;
  strcpy((char *)szOfaBMP_v66[31].str, "run\\timeC.BMP");
  *(_DWORD *)&szOfaBMP_v66[31].str[14] = 0;
  *(_DWORD *)&szOfaBMP_v66[31].str[18] = 0;
  *(_WORD *)&szOfaBMP_v66[31].str[22] = 0;
  szOfaBMP_v66[31].str[24] = 0;
  memset(v14, 0, sizeof(v14));
  v15 = 0;
#endif


  for ( i = 0; i < 11; ++i )
  {
    wsprintfA(FileName, "%s", arrBmpNumbers_v49[i]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, FileName);
    *arrSprNumbers++ = pSprite_v63;
  }
  for ( i = 11; i <= 13; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpNumbers_v49[i]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpNumbers_v49[i + 1]);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprNumbers++ = pSprite_v63;
  }
  for ( i = 0; i <= 1; ++i )
  {
    wsprintfA(FileName, "%s", arrBmpTimes_v32[2 * i]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpTimes_v32[2 * i + 1]);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprTimes++ = pSprite_v63;
  }
  wsprintfA(FileName, "%s", arrBmpTimes_v32[4]);
  CHanaObj__GetRealPath_486D9A(FileName, 1003); // get g_szSPRPath
  HanaTexture_CreateFromFile(&pSprite_v63, FileName);
  *arrSprTimes = pSprite_v63;
  arrSprTimes_a2a = arrSprTimes + 1;
  for ( i = 0; i < 10; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpTimes_v32[i + 5]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpTimes_v32[i + 6]);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprTimes_a2a++ = pSprite_v63;
  }
  wsprintfA(FileName, "%s", szOfaBMP_v66[0]);
  CHanaObj__GetRealPath_486D9A(FileName, 1003); // get g_szSPRPath
  wsprintfA(szPath, "%s", szOfaBMP_v66[1]);
  CHanaObj__GetRealPath_486D9A(szPath, 1003);   // get g_szSPRPath
  LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
  *arrSprTimes_a2a = pSprite_v63;
  result = (CHanaSpriteT3F6_S1FC *)(arrSprTimes_a2a + 1);
  arrSprLifes_v99 = arrSprTimes_a2a + 1;
  for ( i = 0; i < 6; i += 2 )
  {
    wsprintfA(FileName, "%s", szOfaBMP_v66[6 * nMode + 2 + i]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", szOfaBMP_v66[6 * nMode + 3 + i]);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    result = pSprite_v63;
    *arrSprLifes_v99++ = pSprite_v63;
  }
  for ( i = 0; i < 28; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpModes_v5[i]);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpModes_v5[i + 1]);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    result = pSprite_v63;
    *arrSprModes++ = pSprite_v63;
  }
  return result;
}

void __cdecl hook_HanaTexture_CreateFromFile(CHanaSpriteT3F6_S1FC **ppOutSprite, CHAR *szFileName)
{
  DWORD dwFlags; // [esp+8h] [ebp-14h] BYREF
  CHanaSpriteT3F6_S1FC *pSprite; // [esp+Ch] [ebp-10h]
  //int v5; // [esp+10h] [ebp-Ch]
  HRESULT hr; // [esp+14h] [ebp-8h]
  //int v7; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaTexture_CreateFromFile_4135ED\n");
 
  dwFlags = 256;
  pSprite = (CHanaSpriteT3F6_S1FC *)MemAlloc_40DEF7(sizeof(CHanaSpriteT3F6_S1FC));
  memset(pSprite, 0, sizeof(CHanaSpriteT3F6_S1FC));
  pSprite->dwType = 0x3F6;
  // v5 = -1;
  // v7 = -1;
  hr = D3DXCreateTextureFromFile(g_pD3DDevice_4D6C78, &dwFlags, 0, 0, 0, 0, &pSprite->pTexture_134, 0, szFileName, D3DX_FT_DEFAULT);
  if ( hr >= 0 )
  {
    wsprintfA(pSprite->szName_8, "%s", szFileName);
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
    CHanaObj__AddRef_40BFC3(pSprite);
    *ppOutSprite = pSprite;
  }
}

// Activate the item/enemy bodies for a map. Marks up to 10 item bodies active (sets [304]=1 on each body's pose target), then configures the scene's item slots
// from pItemConfig for nMapIndex.
void __cdecl hook_SetupItemBodies_459F81(
  CHanaSceneT458_S504 *pScene,
  CSekBody **pItemBodies,
  DWORD *arrItemPlaceData,
  int nMapIndex)
{
  int v4; // eax
  float a5; // [esp+4h] [ebp-30h] BYREF
  float a4; // [esp+8h] [ebp-2Ch] BYREF
  unsigned int i; // [esp+Ch] [ebp-28h]
  float a3; // [esp+10h] [ebp-24h] BYREF
  unsigned int j; // [esp+14h] [ebp-20h]
  CHanaFrameT3EC_S354 *pChild; // [esp+18h] [ebp-1Ch]
  float pZXrot; // [esp+1Ch] [ebp-18h] BYREF
  float dirForwardZ; // [esp+20h] [ebp-14h] BYREF
  float pYXrot; // [esp+24h] [ebp-10h] BYREF
  float pYpos; // [esp+28h] [ebp-Ch] BYREF
  float pXYrot; // [esp+2Ch] [ebp-8h] BYREF
  float dirForwardX; // [esp+30h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] SetupItemBodies_459F81\n");
  
  pChild = 0;
  for ( i = 0; i < 0xA; ++i )
  {
    if ( pItemBodies[i] )
      pItemBodies[i]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
  }
  if ( pScene )
  {
    for ( i = 0; i < 0xA; ++i )
    {
      if ( (arrItemPlaceData[i] & 0x80000000) == 0
        && pItemBodies[arrItemPlaceData[i]]
        && ((1 << arrItemPlaceData[i]) & g_sceneState_52F7E0.ary33[nMapIndex + 50]) == 0 )
      {
        for ( j = 0; j < pScene->dwCount_130; ++j )
        {
          if ( pScene->refsFrames_134[j] )
          {
            if ( !_strnicmp(pScene->refsFrames_134[j]->szName_8, "item", 4u) )
            {
              v4 = atoi(&pScene->refsFrames_134[j]->szName_8[4]);
              if ( v4 == i + 1 )
              {
                pChild = pItemBodies[arrItemPlaceData[i]]->pSekHeader_220->pFrameT3EC;
                HanaFrame_GetRelPosition(pScene->refsFrames_134[j], g_pSceneFrame, &a3, &a4, &a5);
                SetFramePosRel_4069DD(pChild, (CHanaFrameT3EC_S354 *)g_pSceneFrame, a3, a4, a5);
                CalculateLocalRotationMatrix(
                  pScene->refsFrames_134[j],
                  (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                  &dirForwardX,
                  &pYpos,
                  &dirForwardZ,
                  &pXYrot,
                  &pYXrot,
                  &pZXrot);
                UpdateBoneLookAtTransform(
                  pChild,
                  (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                  dirForwardX,
                  0.0,
                  dirForwardZ,
                  0.0,
                  1.0,
                  0.0);
                pChild->bAlphaDeferred_130 = 0;
                break;
              }
            }
          }
        }
      }
    }
  }
}

// Hero control / AI / input + movement step for one frame. (1) Input: under MutexThreadWait, GetInputState; sums mouse delta across 30 devices into
// nMouseDX/nMouseDY (x10 sensitivity when the device is 'ok'), centered on ConvertFrom640AsInt(320/240); tracks left/middle button press/release into
// g_bAttackHeld / g_bSkillActive (+g_dwSkillTimer). (2) If g_dwBattleMode<=-1, reset all action flags. (3) Battle state: g_bSpinActive
// (g_dwSpinTimer, pose 16, hp-0.5/s, spin -1.8*camF, exits >80s) or g_bSkillActive (g_dwSkillTimer, pose 13, hp-0.3/s, spin +1.5, pose 1 after 28s, clear >33s);
// else on held mouse button pick the attack pose - special maps (16,17,21,22,23) use pose 6/hp-0.25, others pose 5/hp-0.2, plus per-button poses 3/4/1 - or when
// idle (500ms) pose 1 / random 2|14 and regen hp +0.05/s (clamped to fHeroHpMax).
// Yaw from nMouseDX/360/3 via MatrixRotateCubeMapFace about pYawAxis=(0,1,0). (4) Ramp *pfCamFactor toward 1.0 at fFrameTime/1000.
// (5) Collision/move: up to 3 iterations of CheckFrameCollision; on bloc
char __cdecl hook_UpdateHeroControl_45A513(
  CSekBody *pHeroBody,
  CHanaFrameT3EC_S354 *pMapFrame,
  float fFrameTime,
  float *pfHeroHp,
  float fHeroHpMax,
  int nMapIndex,
  float *pfCamFactor)
{
  float x; // [esp+0h] [ebp-80h]
  float y; // [esp+4h] [ebp-7Ch]
  float angle; // [esp+8h] [ebp-78h]
  float anglea; // [esp+8h] [ebp-78h]
  float angleb; // [esp+8h] [ebp-78h]
  float anglec; // [esp+8h] [ebp-78h]
  float angled; // [esp+8h] [ebp-78h]
  float anglee; // [esp+8h] [ebp-78h]
  float anglef; // [esp+8h] [ebp-78h]
  float angleg; // [esp+8h] [ebp-78h]
  float fLerpZ; // [esp+8h] [ebp-78h]
  CSekItem *pCurItem; // [esp+18h] [ebp-68h]
  float fPos2Y; // [esp+1Ch] [ebp-64h] BYREF
  float fPos2X; // [esp+20h] [ebp-60h] BYREF
  float fPos2Z; // [esp+24h] [ebp-5Ch] BYREF
  float px; // [esp+28h] [ebp-58h] BYREF
  __int16 nMouseDX; // [esp+2Ch] [ebp-54h]
  __int16 nCenterY; // [esp+30h] [ebp-50h]
  char bBtn2Released; // [esp+34h] [ebp-4Ch]
  __int16 nCenterX; // [esp+38h] [ebp-48h]
  char bBtn1Released; // [esp+3Ch] [ebp-44h]
  float a5; // [esp+40h] [ebp-40h] BYREF
  char bBtn2Down; // [esp+44h] [ebp-3Ch]
  CHanaFrameT3EC_S354 *pHeroFrame; // [esp+48h] [ebp-38h]
  CInputState *InputState_481B37; // [esp+4Ch] [ebp-34h]
  __int16 i; // [esp+50h] [ebp-30h]
  char bBtn1Down; // [esp+54h] [ebp-2Ch]
  float a4; // [esp+58h] [ebp-28h] BYREF
  int nCollision; // [esp+5Ch] [ebp-24h]
  __int16 nMouseDY; // [esp+60h] [ebp-20h]
  char bActing; // [esp+64h] [ebp-1Ch]
  D3DXVECTOR3 pYawAxis; // [esp+68h] [ebp-18h] BYREF
  float pz; // [esp+74h] [ebp-Ch] BYREF
  float a3; // [esp+78h] [ebp-8h] BYREF
  D3DVALUE py; // [esp+7Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateHeroControl_45A513\n");

  pHeroFrame = pHeroBody->pSekHeader_220->pFrameT3EC;
  nMouseDX = 0;
  nMouseDY = 0;
  nCenterX = ConvertFrom640AsInt(320);
  nCenterY = ConvertFrom640AsInt(240);
  bActing = 0;
  bBtn1Down = 0;
  bBtn1Released = 0;
  bBtn2Down = 0;
  bBtn2Released = 0;
  pYawAxis.x = 0.0;
  pYawAxis.y = 1.0;
  pYawAxis.z = 0.0;
  MutexThreadWait_4815EC(1);
  InputState_481B37 = GetInputState_481B37();
  for ( i = 0; i < 30; ++i )
  {
    InputState_481B37[i].true_6 = 0;
    if ( InputState_481B37[i].kbdMove_5 == 1 )
    {
      nMouseDX += 10 * (InputState_481B37[i].mouseX_0 - nCenterX);
      InputState_481B37[i].mouseX_0 = nCenterX;
      nMouseDY += 10 * (InputState_481B37[i].mouseY_2 - nCenterY);
    }
    else
    {
      nMouseDX += InputState_481B37[i].mouseX_0 - nCenterX;
      InputState_481B37[i].mouseX_0 = nCenterX;
      nMouseDY += InputState_481B37[i].mouseY_2 - nCenterY;
    }
    InputState_481B37[i].mouseY_2 = nCenterY;
  }
  for ( i = 0; i < 15; ++i )
  {
    if ( InputState_481B37[i].mouseBtn_4 == 1 )
      bBtn1Down = 1;
    if ( !InputState_481B37[i].mouseBtn_4 && bBtn1Down == 1 )
      bBtn1Released = 1;
    if ( InputState_481B37[i].mouseBtn_4 == 1 && bBtn1Released == 1 )
      g_bAttackHeld = 1;
    if ( InputState_481B37[i].mouseBtn_4 == 2 )
      bBtn2Down = 1;
    if ( !InputState_481B37[i].mouseBtn_4 && bBtn2Down == 1 )
      bBtn2Released = 1;
    if ( InputState_481B37[i].mouseBtn_4 == 2 && bBtn2Released == 1 && !g_bSkillActive )
    {
      g_bSkillActive = 1;
      g_fSkillTimer = 0.0;
    }
  }
  MutexThreadWait_4815EC(0);
  if ( g_dwBattleMode <= -1 )
  {
    g_bAttackHeld = 0;
    g_bSkillActive = 0;
    g_bSpinActive = 0;
    g_fSkillTimer = 0.0;
    g_fSpinTimer = 0.0;
    g_fIdleTimer = 0.0;
    g_dwBattleMode = 0;
  }
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &px, &py, &pz);
  if ( g_bSkillActive || g_bSpinActive )
  {
    if ( g_bSpinActive )
    {
      g_fSpinTimer = g_fSpinTimer + fFrameTime;
      SekBody__UpdateTo2_401647(pHeroBody, 16);
      *pfHeroHp = *pfHeroHp - 0.5 * fFrameTime;
      if ( g_fSpinTimer <= 10.0 || g_fSpinTimer >= 35.0 )
      {
        if ( g_fSpinTimer > 80.0 )
        {
          g_bSpinActive = 0;
          g_fSpinTimer = 0.0;
          SekBody__UpdateTo2_401647(pHeroBody, 1);
        }
      }
      else
      {
        anglee = -1.8 * fFrameTime * *pfCamFactor;
        SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, anglee);
      }
      g_fIdleTimer = 0.0;
    }
    else if ( g_bSkillActive )
    {
      g_fSkillTimer = g_fSkillTimer + fFrameTime;
      if ( g_fSkillTimer < 25.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 13);
        *pfHeroHp = *pfHeroHp - 0.30000001 * fFrameTime;
        anglef = 1.5 * fFrameTime * *pfCamFactor;
        SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, anglef);
      }
      if ( g_fSkillTimer > 28.0 )
        SekBody__UpdateTo2_401647(pHeroBody, 1);
      if ( g_fSkillTimer > 33.0 )
      {
        g_bSkillActive = 0;
        g_fSkillTimer = 0.0;
      }
      g_fIdleTimer = 0.0;
      g_dwBattleMode = 4;
    }
  }
  else
  {
    if ( InputState_481B37->mouseBtn_4 )
    {
      g_fIdleTimer = 0.0;
      if ( g_bAttackHeld == 1 && InputState_481B37->mouseBtn_4 == 1 )
      {
        if ( nMapIndex == 16 || nMapIndex == 17 || nMapIndex == 21 || nMapIndex == 22 || nMapIndex == 23 )
        {
          SekBody__UpdateTo2_401647(pHeroBody, 6);
          *pfHeroHp = *pfHeroHp - 0.25 * fFrameTime;
          angle = -2.0 * fFrameTime * *pfCamFactor;
          SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, angle);
        }
        else
        {
          SekBody__UpdateTo2_401647(pHeroBody, 5);
          *pfHeroHp = *pfHeroHp - 0.2 * fFrameTime;
          anglea = -1.0 * fFrameTime * *pfCamFactor;
          SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, anglea);
        }
        g_dwBattleMode = 2;
        bActing = 1;
      }
      else
      {
        switch ( InputState_481B37->mouseBtn_4 )
        {
        case 1:
          SekBody__UpdateTo2_401647(pHeroBody, 3);
          *pfHeroHp = *pfHeroHp - 0.15000001 * fFrameTime;
          angleb = -0.5 * fFrameTime * *pfCamFactor;
          SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, angleb);
          g_dwBattleMode = 1;
          bActing = 1;
          break;
        case 2:
          SekBody__UpdateTo2_401647(pHeroBody, 4);
          *pfHeroHp = *pfHeroHp - 0.15000001 * fFrameTime;
          anglec = 0.5 * fFrameTime * *pfCamFactor;
          SetFramePosRel_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, anglec);
          g_dwBattleMode = 3;
          break;
        case 3:
          SekBody__UpdateTo2_401647(pHeroBody, 1);
          break;
        }
      }
    }
    else
    {
      g_bAttackHeld = 0;
      g_fIdleTimer = g_fIdleTimer + fFrameTime;
      if ( g_fIdleTimer < 500.0 )
      {
        SekBody__UpdateTo2_401647(pHeroBody, 1);
      }
      else if ( pHeroBody->nCurItem_200 == 1 )
      {
        if ( rand() % 3 )
          SekBody__UpdateTo2_401647(pHeroBody, 2);
        else
          SekBody__UpdateTo2_401647(pHeroBody, 14);
      }
      *pfHeroHp = 0.050000001 * fFrameTime + *pfHeroHp;
      if ( *pfHeroHp >= (double)fHeroHpMax )
        *pfHeroHp = fHeroHpMax;
      g_dwBattleMode = 0;
    }
    angled = (double)nMouseDX / 360.0 / 3.0;
    Frame_RotateMatrixAxis_4070AB(pHeroFrame, 1, &pYawAxis, angled);
  }
  if ( *pfHeroHp <= 1.0 )
    *pfHeroHp = 1.0;
  if ( pHeroBody->nCurItem_200 >= 7 )
    g_fHeroAnimSpeed = 1.0;
  else
    g_fHeroAnimSpeed = (*pfCamFactor + 2.0) / 3.0;
  angleg = fFrameTime * g_fHeroAnimSpeed;
  if ( CSekBody__UpdateAnim_4019E1(pHeroBody, angleg) && g_fIdleTimer >= 500.0 )
    g_fIdleTimer = 0.0;
  if ( *pfCamFactor <= 1.01 )
  {
    if ( *pfCamFactor >= 0.99000001 )
      *pfCamFactor = 1.0;
    else
      *pfCamFactor = fFrameTime / 1000.0 + *pfCamFactor;
  }
  else
  {
    *pfCamFactor = *pfCamFactor - fFrameTime / 1000.0;
  }
  nCollision = 0;
  for ( i = 0; i < 3; ++i )
  {
    nCollision = CheckFrameCollision_443CC5(pHeroFrame, pMapFrame, 0);
    if ( nCollision )
      break;
    HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &a3, &a4, &a5);
    if ( !TestMoveCollision_4443FC(pHeroFrame, pMapFrame, px, a4, pz, 0) )
      SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, px, a4, pz);
    HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fPos2X, &fPos2Y, &fPos2Z);
    fLerpZ = pz - (pz - fPos2Z) / 2.0;
    y = py - (py - fPos2Y) / 2.0;
    x = px - (px - fPos2X) / 2.0;
    SetFramePosRel_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, x, y, fLerpZ);
  }
  if ( (unsigned int)--nCollision < 4 && g_arrFootstepSound_52F18C[nCollision] && g_arrFootstepSound_52F17C[nCollision] )
  {
    pCurItem = &pHeroBody->items_250[pHeroBody->nCurItem_200];
    HanaFrame_GetCenterPosition_408C3E(pHeroFrame, g_pSceneFrame, &px, &py, &pz);
    g_arrFootstepSound_52F18C[nCollision]->pDS3DBuffer_4->SetPosition(
      px,
      py,
      pz,
      1u);
    g_arrFootstepSound_52F17C[nCollision]->pDS3DBuffer_4->SetPosition(
      px,
      py,
      pz,
      1u);
    if ( !g_arrFootstepSound_52F17C[nCollision]->pDSBuffer_0
      || g_arrFootstepSound_52F17C[nCollision]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pCurItem->fCurTime_60 < 303.0 || pCurItem->fCurTime_60 > 310.0)
        && (pCurItem->fCurTime_60 < 403.0 || pCurItem->fCurTime_60 > 410.0)
        && (pCurItem->fCurTime_60 < 500.0 || pCurItem->fCurTime_60 > 504.0)
        && (pCurItem->fCurTime_60 < 600.0 || pCurItem->fCurTime_60 > 606.0)
        && (pCurItem->fCurTime_60 < 1630.0 || pCurItem->fCurTime_60 > 1640.0)
        && (pCurItem->fCurTime_60 < 1670.0 || pCurItem->fCurTime_60 > 1680.0)
        && (pCurItem->fCurTime_60 < 2622.0 || pCurItem->fCurTime_60 > 2630.0) )
      {
        for ( i = 0; i < 4; ++i )
        {
          if ( g_arrFootstepSound_52F17C[i] )
            g_arrFootstepSound_52F17C[i]->pDSBuffer_0->Stop();
        }
      }
      else
      {
        g_arrFootstepSound_52F17C[nCollision]->pDSBuffer_0->Play(
          0,
          0,
          0);
      }
    }
    if ( !g_arrFootstepSound_52F18C[nCollision]->pDSBuffer_0
      || g_arrFootstepSound_52F18C[nCollision]->pDSBuffer_0->GetStatus(
        &g_dwDSStatus_5185C8)
      || (g_dwDSStatus_5185C8 & 1) == 0 )
    {
      if ( (pCurItem->fCurTime_60 < 313.0 || pCurItem->fCurTime_60 > 320.0)
        && (pCurItem->fCurTime_60 < 413.0 || pCurItem->fCurTime_60 > 420.0)
        && (pCurItem->fCurTime_60 < 507.0 || pCurItem->fCurTime_60 > 511.0)
        && (pCurItem->fCurTime_60 < 605.0 || pCurItem->fCurTime_60 > 611.0)
        && (pCurItem->fCurTime_60 < 1633.0 || pCurItem->fCurTime_60 > 1643.0)
        && (pCurItem->fCurTime_60 < 1658.0 || pCurItem->fCurTime_60 > 1665.0)
        && (pCurItem->fCurTime_60 < 2620.0 || pCurItem->fCurTime_60 > 2630.0) )
      {
        for ( i = 0; i < 4; ++i )
        {
          if ( g_arrFootstepSound_52F18C[i] )
            g_arrFootstepSound_52F18C[i]->pDSBuffer_0->Stop();
        }
      }
      else
      {
        g_arrFootstepSound_52F18C[nCollision]->pDSBuffer_0->Play(
          0,
          0,
          0);
      }
    }
  }
  return bActing;
}

// Find the nearest item/enemy body in acquisition range. For each item body computes its position relative to pHeroFrame; if within radius (~8, dist^2<64) and not
// already flagged, sets [304]=1, records the hit in the global per-map target bitmask (MEMORY[0x52F845][nMapIndex] |= 1<<i) and returns that index.
int __cdecl hook_FindTargetInRange_45A1AD(
  CHanaFrameRootT3EC_S2D4 *pFrame,
  CSekBody **pItemBodies,
  float fDeltaTime,
  int nMapIndex)
{
  float a5; // [esp+0h] [ebp-18h] BYREF
  float a4; // [esp+4h] [ebp-14h] BYREF
  float a3; // [esp+8h] [ebp-10h] BYREF
  int i; // [esp+Ch] [ebp-Ch]
  CHanaFrameT3EC_S354 *a1; // [esp+10h] [ebp-8h]
  int v10; // [esp+14h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindTargetInRange_45A1AD\n");
 
  v10 = 0;
  a1 = 0;
  for ( i = 0; i < 10; ++i )
  {
    if ( pItemBodies[i] )
    {
      a1 = pItemBodies[i]->pSekHeader_220->pFrameT3EC;
      if ( !a1->bAlphaDeferred_130 )
      {
        HanaFrame_GetRelPosition(a1, pFrame, &a3, &a4, &a5);
        if ( a3 * a3 + a5 * a5 < 64.0 )
        {
          v10 = i;
          g_sceneState_52F7E0.ary33[nMapIndex + 50] |= 1 << i;
          a1->bAlphaDeferred_130 = 1;
          break;
        }
      }
    }
  }
  for ( i = 0; i < 10; ++i )
  {
    if ( pItemBodies )
      CSekBody__UpdateAnim_4019E1(pItemBodies[i], fDeltaTime);
  }
  return v10;
}

// Apply the effect of the acquired target type to the hero. switch(nType): 1 -> +20000 hp (clamped to max) +10 blend;
// 2 -> +6600 hp & max +10 blend; 3 -> camera factor=1.25; 4 -> +30s time limit (capped 999); 5+ -> further item effects.
void __cdecl hook_ApplyTargetEffect_45A2BA(
  int nTargetSprite,
  float *pfHeroHp,
  float *pfHeroHpMax,
  int *pnTimeLimit,
  float *pfCamFactor,
  float *pfBlendTime)
{
  ONCELOG("[HNH:SUB] ApplyTargetEffect_45A2BA\n");
  switch ( nTargetSprite )
  {
  case 1:
    *pfBlendTime = *pfBlendTime + 10.0;
    *pfHeroHp = *pfHeroHp + 20000.0;
    if ( *pfHeroHpMax < (double)*pfHeroHp )
      *pfHeroHp = *pfHeroHpMax;
    break;
  case 2:
    *pfBlendTime = *pfBlendTime + 10.0;
    *pfHeroHp = *pfHeroHp + 6600.0;
    *pfHeroHpMax = *pfHeroHpMax + 6600.0;
    break;
  case 3:
    *pfCamFactor = 1.5;
    break;
  case 4:
    *pfBlendTime = *pfBlendTime + 10.0;
    if ( (unsigned int)(*pnTimeLimit + 30) <= 999 )
      *pnTimeLimit += 30;
    else
      *pnTimeLimit = 999;
    break;
  case 5:
    *pfBlendTime = *pfBlendTime + 20.0;
    *pfHeroHp = *pfHeroHp - 20000.0;
    break;
  case 6:
    *pfCamFactor = 0.5;
    break;
  case 7:
    *pfBlendTime = *pfBlendTime + 20.0;
    if ( *pnTimeLimit >= 30 )
      *pnTimeLimit -= 30;
    else
      *pnTimeLimit = 0;
    break;
  default:
    return;
  }
}

// Lock-on targeter. Scans pScene->refsFrames_134 for 'Event' children whose bit (nEventNo=atoi(name+5)) is not yet set in the per-map event bitmask
// g_sceneState_52F7E0.ary33[nMapIndex]; keeps the nearest one within reach (default 400.0, squared). For the closest it positions/aims pLockonFrame
// at that node (rel pos + local rotation vs g_pSceneFrame -> MtxMult + UpdateBoneLookAtTransform) and sets bFound=1. If none found, parks pLockonFrame
// far off-screen (10000,1000,-1000). Returns bFound.
void __cdecl hook_UpdateLockonEvent_45953B(
  CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 *pHeroFrame,
  CHanaFrameT3EC_S354 *pLockonFrame,
  int nMapIndex)
{
  float fRelZ; // [esp+0h] [ebp-34h] BYREF
  float fRelY; // [esp+4h] [ebp-30h] BYREF
  float fRelX; // [esp+8h] [ebp-2Ch] BYREF
  unsigned int i; // [esp+Ch] [ebp-28h]
  float fSqDist; // [esp+10h] [ebp-24h]
  bool bFound; // [esp+14h] [ebp-20h]
  int nEventNo; // [esp+18h] [ebp-1Ch]
  float dirUpZ; // [esp+1Ch] [ebp-18h] BYREF
  float dirForwardZ; // [esp+20h] [ebp-14h] BYREF
  float dirUpY; // [esp+24h] [ebp-10h] BYREF
  float dirForwardY; // [esp+28h] [ebp-Ch] BYREF
  float dirUpX; // [esp+2Ch] [ebp-8h] BYREF
  float dirForwardX[2]; // [esp+30h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateLockonEvent_45953B\n");
  
  fSqDist = 400.0;
  bFound = 0;
  if ( pScene )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->refsFrames_134[i] )
      {
        if ( !_strnicmp(pScene->refsFrames_134[i]->szName_8, "event", 5u) )
        {
          nEventNo = atoi(&pScene->refsFrames_134[i]->szName_8[5]);
          if ( ((1 << nEventNo) & g_sceneState_52F7E0.ary33[nMapIndex]) == 0 )
          {
            HanaFrame_GetRelPosition(pScene->refsFrames_134[i], pHeroFrame, &fRelX, &fRelY, &fRelZ);
            CalculateLocalRotationMatrix(
              pScene->refsFrames_134[i],
              pHeroFrame,
              dirForwardX,
              &dirForwardY,
              &dirForwardZ,
              &dirUpX,
              &dirUpY,
              &dirUpZ);
            if ( fRelX * fRelX + fRelZ * fRelZ <= fSqDist )
            {
              fSqDist = fRelX * fRelX + fRelZ * fRelZ;
              HanaFrame_GetRelPosition(pScene->refsFrames_134[i], g_pSceneFrame, &fRelX, &fRelY, &fRelZ);
              CalculateLocalRotationMatrix(
                pScene->refsFrames_134[i],
                (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                dirForwardX,
                &dirForwardY,
                &dirForwardZ,
                &dirUpX,
                &dirUpY,
                &dirUpZ);
              SetFramePosRel_4069DD(pLockonFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fRelX, fRelY, fRelZ);
              UpdateBoneLookAtTransform(
                pLockonFrame,
                (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                dirForwardX[0],
                dirForwardY,
                dirForwardZ,
                dirUpX,
                dirUpY,
                dirUpZ);
              bFound = 1;
            }
          }
        }
      }
    }
  }
  if ( !bFound )
    SetFramePosRel_4069DD(pLockonFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 10000.0, 1000.0, -1000.0);
}

// Scene advance / transition detector. Scans the ODS scene's refsFrames_134 for 'Jump' marker frames; when the hero is within radius ~5.2 of one
// (fRelX^2+fRelZ^2<=27.04) and it is in front (fRelZ... pZpos < -0.85), reads the destination from the name: nTargetMap=atoi(name+4), nTargetSub=atoi(name+7).
// (1) If nTargetMap is in this stage's engageMapTable slot (30*nStageIndex), set nResult=2 -> caller engages the scripted-cam/dialogue mode.
// (2) Else if nTargetMap==finalMapTable[nStageIndex] ({21,13,16,4,2,14,16,22,17,6,8}) -> return 3 = stage complete.
// (3) Otherwise advance: write *pnMapIndex=nTargetMap / *pnSubIndex=nTargetSub and, unless it is an excluded target ((23,0),(23,1), map 8, (22,10)),
// play a transition SFX (pDSBuf for maps 16/17 else arrUIEffectDSBuffer_52F168) and return 1 = fade & load the next map. Leaves nResult (0/2) when nothing triggered.
int __cdecl hook_CheckSceneTransition_459768(
  CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 *pHeroFrame,
  int *pnMapIndex,
  int *pnSubIndex,
  int nStageIndex)
{
  int nTargetSub; // [esp+4h] [ebp-778h]
  DWORD engageMapTable[450]; // [esp+8h] [ebp-774h] BYREF
  float fRelZ; // [esp+710h] [ebp-6Ch] BYREF
  float fRelY; // [esp+714h] [ebp-68h] BYREF
  float fRelX; // [esp+718h] [ebp-64h] BYREF
  unsigned int i; // [esp+71Ch] [ebp-60h]
  int nResult; // [esp+720h] [ebp-5Ch]
  float pZXrot; // [esp+724h] [ebp-58h] BYREF
  DWORD finalMapTable[15]; // [esp+728h] [ebp-54h] BYREF
  float pZpos; // [esp+764h] [ebp-18h] BYREF
  float pYXrot; // [esp+768h] [ebp-14h] BYREF
  float pYpos; // [esp+76Ch] [ebp-10h] BYREF
  float pXYrot; // [esp+770h] [ebp-Ch] BYREF
  int nTargetMap; // [esp+774h] [ebp-8h]
  float pXpos; // [esp+778h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] CheckSceneTransition_459768\n");
  
  nResult = 0;
  engageMapTable[0] = 0;
  engageMapTable[1] = 4;
  engageMapTable[2] = 3;
  engageMapTable[3] = 10;
  engageMapTable[4] = 11;
  engageMapTable[5] = 13;
  engageMapTable[6] = 12;
  engageMapTable[7] = 19;
  engageMapTable[8] = 16;
  engageMapTable[9] = 23;
  engageMapTable[10] = -1;
  memset(&engageMapTable[11], 0, 80);
  engageMapTable[31] = 16;
  engageMapTable[32] = 21;
  engageMapTable[33] = 23;
  engageMapTable[34] = -1;
  memset(&engageMapTable[35], 0, 104);
  engageMapTable[61] = 17;
  engageMapTable[62] = 23;
  engageMapTable[63] = -1;
  memset(&engageMapTable[64], 0, 108);
  engageMapTable[91] = 17;
  engageMapTable[92] = 23;
  engageMapTable[93] = -1;
  memset(&engageMapTable[94], 0, 108);
  engageMapTable[121] = 8;
  engageMapTable[122] = 17;
  engageMapTable[123] = -1;
  memset(&engageMapTable[124], 0, 108);
  engageMapTable[151] = 8;
  engageMapTable[152] = 17;
  engageMapTable[153] = -1;
  memset(&engageMapTable[154], 0, 108);
  engageMapTable[181] = 8;
  engageMapTable[182] = 17;
  engageMapTable[183] = -1;
  memset(&engageMapTable[184], 0, 108);
  engageMapTable[211] = -1;
  memset(&engageMapTable[212], 0, 116);
  engageMapTable[241] = -1;
  memset(&engageMapTable[242], 0, 116);
  engageMapTable[271] = -1;
  memset(&engageMapTable[272], 0, 116);
  engageMapTable[301] = -1;
  memset(&engageMapTable[302], 0, 592);
  finalMapTable[0] = 21;
  finalMapTable[1] = 13;
  finalMapTable[2] = 16;
  finalMapTable[3] = 4;
  finalMapTable[4] = 2;
  finalMapTable[5] = 14;
  finalMapTable[6] = 16;
  finalMapTable[7] = 22;
  finalMapTable[8] = 17;
  finalMapTable[9] = 6;
  finalMapTable[10] = 8;
  memset(&finalMapTable[11], 0, 16);
  if ( pScene )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->refsFrames_134[i] )
      {
        if ( !_strnicmp(pScene->refsFrames_134[i]->szName_8, "JUMP", 4u) )
        {
          HanaFrame_GetRelPosition(pScene->refsFrames_134[i], pHeroFrame, &fRelX, &fRelY, &fRelZ);
          CalculateLocalRotationMatrix(
            pScene->refsFrames_134[i],
            pHeroFrame,
            &pXpos,
            &pYpos,
            &pZpos,
            &pXYrot,
            &pYXrot,
            &pZXrot);
          if ( fRelX * fRelX + fRelZ * fRelZ <= 27.039997 && pZpos < -0.85000002 )
          {
            nTargetMap = atoi(&pScene->refsFrames_134[i]->szName_8[4]);
            nTargetSub = atoi(&pScene->refsFrames_134[i]->szName_8[7]);
            for ( i = 0; i < 0x1E && engageMapTable[30 * nStageIndex + i] != -1; ++i )
            {
              if ( engageMapTable[30 * nStageIndex + i] == nTargetMap )
              {
                nResult = 2;
                break;
              }
            }
            if ( nResult != 2 )
            {
              if ( finalMapTable[nStageIndex] == nTargetMap )
              {
                return 3;
              }
              else
              {
                *pnMapIndex = nTargetMap;
                *pnSubIndex = nTargetSub;
                if ( (*pnMapIndex != 23 || *pnSubIndex)
                  && (*pnMapIndex != 23 || *pnSubIndex != 1)
                  && *pnMapIndex != 8
                  && (*pnMapIndex != 22 || *pnSubIndex != 10) )
                {
                  if ( *pnMapIndex == 16 && *pnSubIndex == 1 || *pnMapIndex == 17 )
                  {
                    if ( g_arrUIEffectDSBuffer_52F168[1] )
                      DSBufferPlay(g_arrUIEffectDSBuffer_52F168[1], 0);
                  }
                  else if ( g_arrUIEffectDSBuffer_52F168[0] )
                  {
                    DSBufferPlay(g_arrUIEffectDSBuffer_52F168[0], 0);
                  }
                }
                return 1;
              }
            }
            return nResult;
          }
        }
      }
    }
  }
  return nResult;
}

// Trigger the ODS 'Event' the hero has reached and return its script time. Scans pScene->refsFrames_134 for children named 'Event'; when one is within radius 5
// of pHeroFrame (fRelX^2+fRelZ^2<=25) and oriented/positioned (pZpos<-0.25 || pZXrot>0.25), reads nEventNo=atoi(name+13). If its bit is not yet set in
// g_sceneState_52F7E0.stc9C.gap30[nMapIndex+3] (the per-map event bitmask), sets fEventTime=1000*nEventNo and marks the bit. On a trigger it also finds
// the 'Otoko' child -> *pnCursor (hero node index) and the 'Camera' child, re-parenting/re-aiming g_pCameraFrame onto it (AttachChild + MtxMult + UpdateBoneLookAt).
// Returns fEventTime (0 = no event).
double __cdecl hook_GetSceneEventTime_459CA2(
  CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 *pHeroFrame,
  unsigned int *pnCursor,
  int nMapIndex)
{
  float fRelZ; // [esp+4h] [ebp-30h] BYREF
  float fRelY; // [esp+8h] [ebp-2Ch] BYREF
  float fRelX; // [esp+Ch] [ebp-28h] BYREF
  unsigned int i; // [esp+10h] [ebp-24h]
  float fEventTime; // [esp+14h] [ebp-20h]
  int nEventNo; // [esp+18h] [ebp-1Ch]
  float pZXrot; // [esp+1Ch] [ebp-18h] BYREF
  float pZpos; // [esp+20h] [ebp-14h] BYREF
  float pYXrot; // [esp+24h] [ebp-10h] BYREF
  float pYpos; // [esp+28h] [ebp-Ch] BYREF
  float pXYrot; // [esp+2Ch] [ebp-8h] BYREF
  float pXpos; // [esp+30h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] GetSceneEventTime_459CA2\n");
  
  fEventTime = 0.0;
  if ( pScene )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->refsFrames_134[i] )
      {
        if ( !_strnicmp(pScene->refsFrames_134[i]->szName_8, "EVENT", 5u) )
        {
          HanaFrame_GetRelPosition(pScene->refsFrames_134[i], pHeroFrame, &fRelX, &fRelY, &fRelZ);
          CalculateLocalRotationMatrix(
            pScene->refsFrames_134[i],
            pHeroFrame,
            &pXpos,
            &pYpos,
            &pZpos,
            &pXYrot,
            &pYXrot,
            &pZXrot);
          if ( fRelX * fRelX + fRelZ * fRelZ <= 25.0 && (pZpos < -0.25 || pZXrot > 0.25) )
          {
            nEventNo = atoi(&pScene->refsFrames_134[i]->szName_8[5]);
            if ( ((1 << nEventNo) & g_sceneState_52F7E0.ary33[nMapIndex]) == 0 )
            {
              fEventTime = (float)(1000 * nEventNo);
              g_sceneState_52F7E0.ary33[nMapIndex] |= 1 << nEventNo;
            }
            break;
          }
        }
      }
    }
    if ( fEventTime != 0.0 )
    {
      for ( i = 0; i < pScene->dwCount_130; ++i )
      {
        if ( pScene->refsFrames_134[i] && !_strnicmp(pScene->refsFrames_134[i]->szName_8, "OTOKO", 5u) )
        {
          *pnCursor = i;
          break;
        }
      }
      for ( i = 0; i < pScene->dwCount_130; ++i )
      {
        if ( pScene->refsFrames_134[i] && !_strnicmp(pScene->refsFrames_134[i]->szName_8, "camera", 6u) )
        {
          AttachChild_4055FD(pScene->refsFrames_134[i], g_pCameraFrame);
          SetFramePosRel_4069DD(g_pCameraFrame, pScene->refsFrames_134[i], 0.0, 0.0, 0.0);
          UpdateBoneLookAtTransform(g_pCameraFrame, pScene->refsFrames_134[i], 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
          return fEventTime;
        }
      }
    }
  }
  return fEventTime;
}


// Update the on-screen health gauge HUD from the pHudRes resource table, using fHpPct/fHpMaxPct (hero hp and max, passed divided by 100).
void __cdecl hook_UpdateHudGauge_44BC36(CHanaSpriteT3F6_S1FC **pHudRes, int fHpPct, int fHpMaxPct)
{
  
  int v4; // eax
  int v5; // eax
  int v6; // [esp+20h] [ebp-68h]
  int v7; // [esp+20h] [ebp-68h]
  int v8; // [esp+24h] [ebp-64h]
  int v9; // [esp+24h] [ebp-64h]
  float v10; // [esp+28h] [ebp-60h]
  float v11; // [esp+28h] [ebp-60h]
  int v12; // [esp+28h] [ebp-60h]
  int v13; // [esp+28h] [ebp-60h]
  int nCount; // [esp+34h] [ebp-54h]
  D3DXVECTOR3 *i; // [esp+3Ch] [ebp-4Ch]
  DWORD texW; // [esp+40h] [ebp-48h] BYREF
  D3DXVECTOR3 parrPos[4]; // [esp+44h] [ebp-44h] BYREF
  struct tagRECT rc; // [esp+74h] [ebp-14h] BYREF
  DWORD texH; // [esp+84h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] UpdateHudGauge_44BC36\n");
 
  nCount = 4;
  for ( i = parrPos; --nCount >= 0; ++i )
    ;
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
  if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
  {
    GetSurfaceWH(pHudRes[2]->pTexture_134, &texW, &texH);
    SetRect(&rc, 253, 0, 254, 48);  // select red:area
    CHanaSpriteT3F6__SetSrcRect_414B28(pHudRes[2], &rc);
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
    DrawSprite3D(pHudRes[2], parrPos);  // draw red

    // IDA HexRays issues related to, cdq, sub eax, edx, and sar 1.

    //EVERYLOG(60, "[HNH:UpdateHudGauge] fHpPct=%d, fHpMaxPct=%d\n", fHpPct, fHpMaxPct);

    // select: green area
    int xBer = (pHudRes[2]->dwWidth_1F0 / 2) - fHpPct * (pHudRes[2]->dwWidth_1F0 / 2 - 1) / fHpMaxPct;

    //EVERYLOG(60, "[HNH:UpdateHudGauge] xBer=%d\n", xBer);

    SetRect(
      &rc,
      xBer,
      0,
      xBer,
      48);
    CHanaSpriteT3F6__SetSrcRect_414B28(pHudRes[2], &rc);
    parrPos[0].x = ConvertFrom640AsFloat(354.0);
    parrPos[0].y = ConvertFrom640AsFloat(13.0);
    parrPos[0].z = 0.0;
    v10 = (double)(246 * fHpPct / fHpMaxPct) + 354.0;
    parrPos[1].x = ConvertFrom640AsFloat(v10);
    parrPos[1].y = ConvertFrom640AsFloat(13.0);
    parrPos[1].z = 0.0;
    v11 = (double)(246 * fHpPct / fHpMaxPct) + 368.0;
    parrPos[2].x = ConvertFrom640AsFloat(v11);
    parrPos[2].y = ConvertFrom640AsFloat(28.0);
    parrPos[2].z = 0.0;
    parrPos[3].x = ConvertFrom640AsFloat(368.0);
    parrPos[3].y = ConvertFrom640AsFloat(28.0);
    parrPos[3].z = 0.0;
    DrawSprite3D(pHudRes[2], parrPos); // draw greaan
    
    v12 = ConvertFrom640AsInt(30);
    v8 = ConvertFrom640AsInt(640);
    v6 = ConvertFrom640AsInt(-2);
    v4 = ConvertFrom640AsInt(320);
    SetRect(&rc, v4, v6, v8, v12);

    //EVERYLOG(60, "[HNH:UpdateHudGauge] pHudRes[1] name=%s\n", pHudRes[1]->szName_8);

    DrawSpriteLTRB(pHudRes[1], rc.left, rc.top, rc.right, rc.bottom);
    SetRect(&rc, 0, 1, 160, 47);
    CHanaSpriteT3F6__SetSrcRect_414B28(pHudRes[8], &rc);
    v13 = ConvertFrom640AsInt(78);
    v9 = ConvertFrom640AsInt(640);
    v7 = ConvertFrom640AsInt(30);
    v5 = ConvertFrom640AsInt(480);
    SetRect(&rc, v5, v7, v9, v13);

    //EVERYLOG(60, "[HNH:UpdateHudGauge] pHudRes[8] name=%s\n", pHudRes[8]->szName_8);

    DrawSpriteLTRB(pHudRes[8], rc.left, rc.top, rc.right, rc.bottom);
    g_pD3DDevice_4D6C78->EndScene();
  }
}

/*
 * Target-select INTRO: the camera sweep played before ShowMouseSelectDialog.
 A timed shot list drives it - arrShotAngles/arrShotDurations = [(-90,10),(45,10),(180,900),(99999,sentinel)]:
 two 10s profile shots of the hero (yaw -90 then +45), then a long 180 rear shot ended only by the global 280s timeout.
 Per frame the hero frame is yawed to the shot angle (hero frame as scratch, restored after),
 the chase cam is snapped twice (bSnap=1) - once from hero+18up (look-at hero),
 once from hero - fCrane - 13 (fCrane rises -10 -> +5 at delta/13: a crane-down) -
 then the head node ('head', FindFrameByName) frames the shot: camera pushed out to a 5u minimum XZ distance from
 the head and BuildLookAt at it. After the crane bottoms out the FOV creeps 0.8 -> 1.2 (delta/80, zoom-out).
 Hero plays anim item 15 (select pose, skipped if already in 8/9) until done; the 8 target bodies (pSpriteNames)
 animate at delta/fAnimRate, go idle (item 0) after t>30, are hard re-cut to their current item (fBlendWeight_224=1)
 on each shot chan
 */
void __cdecl hook_PrepareTargetSelect_45B5BD(
  CSekBody *pHeroBody,
  CSekBody **pItemBodies,
  CSekBody **pSpriteNames,
  CHanaFrameT3EC_S354 *pMapFrame,
  CHanaSpriteT3F6_S1FC **pHudA,
  CHanaSpriteT3F6_S1FC **pTargets,
  float fHeroHp,
  float fHeroHpMax,
  int nTimeLimit,
  unsigned int nElapsed)
{
  double v10; // st7
  float fMountHiY; // [esp+0h] [ebp-D0h]
  float fMountCraneY; // [esp+0h] [ebp-D0h]
  float fYawNegRad; // [esp+4h] [ebp-CCh]
  float fYawRad; // [esp+4h] [ebp-CCh]
  float fTargetStep; // [esp+10h] [ebp-C0h]
  float fHeroStep; // [esp+14h] [ebp-BCh]
  float fHeadRelZ; // [esp+18h] [ebp-B8h] BYREF
  float fHeadDist; // [esp+1Ch] [ebp-B4h]
  float fHeadRelY; // [esp+20h] [ebp-B0h] BYREF
  float fHeadRelX; // [esp+24h] [ebp-ACh] BYREF
  float arrShotDurations[10]; // [esp+28h] [ebp-A8h] BYREF
  float TimerScaleFactor; // [esp+50h] [ebp-80h]
  __int16 bMouseReleased; // [esp+54h] [ebp-7Ch]
  int nShot; // [esp+58h] [ebp-78h]
  int v25; // [esp+5Ch] [ebp-74h]
  float fShotTime; // [esp+60h] [ebp-70h]
  int nMouseY; // [esp+64h] [ebp-6Ch] BYREF
  int iTarget; // [esp+68h] [ebp-68h]
  int nMouseX; // [esp+6Ch] [ebp-64h] BYREF
  int bHeroIntroDone; // [esp+70h] [ebp-60h]
  float fDelta; // [esp+74h] [ebp-5Ch]
  float ProjectionFov_40DCF6; // [esp+78h] [ebp-58h]
  CHanaFrameT3EC_S354 *pHeadFrame; // [esp+7Ch] [ebp-54h]
  float fHeroZ; // [esp+80h] [ebp-50h] BYREF
  float fFovTmp; // [esp+84h] [ebp-4Ch]
  float fHeroY; // [esp+88h] [ebp-48h] BYREF
  D3DXVECTOR3 vecUpAxis; // [esp+8Ch] [ebp-44h] BYREF
  float fHeroX; // [esp+98h] [ebp-38h] BYREF
  __int16 bMouseBtn; // [esp+9Ch] [ebp-34h] BYREF
  float fAnimRate; // [esp+A0h] [ebp-30h]
  float fCrane; // [esp+A4h] [ebp-2Ch]
  float arrShotAngles[10]; // [esp+A8h] [ebp-28h] BYREF

  ONCELOG("[HNH:SUB] PrepareTargetSelect_45B5BD\n");
  
  fCrane = -10.0;
  fShotTime = 0.0;
  fAnimRate = 1.0;
  bMouseReleased = 0;
  bHeroIntroDone = 0;
  nShot = 0;
  v25 = 0;
  arrShotDurations[0] = 10.0;
  arrShotDurations[1] = 10.0;
  arrShotDurations[2] = 900.0;
  arrShotDurations[3] = 99999.0;
  memset(&arrShotDurations[4], 0, 0x18u);
  arrShotAngles[0] = -90.0;
  arrShotAngles[1] = 45.0;
  arrShotAngles[2] = 180.0;
  arrShotAngles[3] = 99999.0;
  memset(&arrShotAngles[4], 0, 0x18u);
  pHeadFrame = FindFrameByName_47E999(pHeroBody->pSekHeader_220->pFrameT3EC, "head");
  vecUpAxis.x = 0.0;
  vecUpAxis.y = 1.0;
  vecUpAxis.z = 0.0;
  if ( pHeroBody->nCurItem_200 != 9 && pHeroBody->nCurItem_200 != 8 )
    SekBody__UpdateTo2_401647(pHeroBody, 15);
  ProjectionFov_40DCF6 = GetProjectionFov_40DCF6();
  SetProjectionFov_40DCA3(0.80000001);
  TimerScaleFactor = GetTimerScaleFactor();
  SetTimerScaleFactor(10.0);
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  while ( 1 )
  {
    DoPeekMessages();
    fDelta = GetFrameTime();
    if ( fDelta > 5.0 )
      fDelta = 5.0;
    FrameTimer_EndFrame();
    GetCursorPos_4811FA(&nMouseX, &nMouseY, &bMouseBtn);
    if ( bMouseBtn )
    {
      if ( bMouseReleased == 1 )
        fDelta = fDelta * 5.0;
    }
    else if ( !bMouseReleased )
    {
      bMouseReleased = 1;
    }
    if ( !bHeroIntroDone )
    {
      fHeroStep = fDelta / fAnimRate;
      if ( CSekBody__UpdateAnim_4019E1(pHeroBody, fHeroStep) )
      {
        fAnimRate = 1.0;
        bHeroIntroDone = 1;
      }
    }
    if ( fShotTime < 90.0 )
    {
      for ( iTarget = 0; iTarget < 8; ++iTarget )
      {
        if ( pSpriteNames[iTarget] )
        {
          if ( fShotTime > 30.0 )
            SekBody__UpdateTo2_401647(pSpriteNames[iTarget], 0);
          fTargetStep = fDelta / fAnimRate;
          CSekBody__UpdateAnim_4019E1(pSpriteNames[iTarget], fTargetStep);
        }
      }
    }
    fYawNegRad = -arrShotAngles[nShot] * 3.1400001 / 180.0;
    Frame_RotateMatrixAxis_4070AB(pHeroBody->pSekHeader_220->pFrameT3EC, 1, &vecUpAxis, fYawNegRad);
    HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
    fMountHiY = fHeroY + 18.0;
    SetFramePosRel_4069DD(
      pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      fHeroX,
      fMountHiY,
      fHeroZ);
    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, fDelta, 1);
    HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, 0);
    fMountCraneY = fHeroY - fCrane - 13.0;
    SetFramePosRel_4069DD(
      pHeroBody->pSekHeader_220->pFrameT3EC,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      fHeroX,
      fMountCraneY,
      fHeroZ);
    UpdateCameraFollowChase_445525(pHeroBody->pSekHeader_220->pFrameT3EC, g_pCameraFrame, pMapFrame, fDelta, 1);
    SetFramePosRel_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
    fYawRad = arrShotAngles[nShot] * 3.1400001 / 180.0;
    Frame_RotateMatrixAxis_4070AB(pHeroBody->pSekHeader_220->pFrameT3EC, 1, &vecUpAxis, fYawRad);
    fCrane = fDelta / 13.0 + fCrane;
    if ( fCrane > 5.0 )
    {
      fCrane = 5.0;
      fFovTmp = GetProjectionFov_40DCF6();
      fFovTmp = fDelta / 80.0 + fFovTmp;
      if ( fFovTmp >= 1.2 )
        fFovTmp = 1.2;
      SetProjectionFov_40DCA3(fFovTmp);
    }
    v10 = fShotTime + fDelta;
    fShotTime = v10;
    if ( v10 > 280.0 )
      break;
    if ( fShotTime > (double)arrShotDurations[nShot] )
    {
      fCrane = -10.0;
      fShotTime = 0.0;
      if ( ++nShot > 1 )
        fAnimRate = 3.0;
      for ( iTarget = 0; iTarget < 8; ++iTarget )
      {
        if ( pSpriteNames[iTarget] )
        {
          CSekBody__CutToAnim_4018E1(pSpriteNames[iTarget], pSpriteNames[iTarget]->nCurItem_200);
          pSpriteNames[iTarget]->fBlendWeight_224 = 1.0;
        }
      }
    }
    if ( pHeadFrame )
    {
      HanaFrame_GetRelPosition(g_pCameraFrame, pHeadFrame, &fHeadRelX, &fHeadRelY, &fHeadRelZ);
      fHeadDist = sqrt(fHeadRelX * fHeadRelX + fHeadRelZ * fHeadRelZ);
      if ( fHeadDist < 5.0 && fHeadDist != 0.0 )
      {
        fHeadRelX = 5.0 / fHeadDist * fHeadRelX;
        fHeadRelZ = 5.0 / fHeadDist * fHeadRelZ;
        SetFramePosRel_4069DD(g_pCameraFrame, pHeadFrame, fHeadRelX, fHeadRelY, fHeadRelZ);
      }
      HanaMatrix_BuildLookAtAndTransform(g_pCameraFrame, pHeadFrame, g_pSceneFrame, 0);
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
    UpdateHudGauge_44BC36(pTargets, (__int64)fHeroHp / 100, (__int64)fHeroHpMax / 100);
    DrawClockHud_44AF5B(pHudA, 100, -5, 100 * nTimeLimit - nElapsed / 0xA);
    
    PresentFrame();
    HanaVert_DrawBatch(0);
  }
  SetTimerScaleFactor(TimerScaleFactor);
  SetProjectionFov_40DCA3(ProjectionFov_40DCF6);
  HanaSurface_CreateAndFade();
}

void hook_HanaSurface_CreateAndFade()
{
  DWORD w; // eax
  int w2; // eax
  COLORREF v2; // eax
  COLORREF colorFade; // eax
  int h2; // [esp-8h] [ebp-Ch]
  DWORD h; // [esp-4h] [ebp-8h]
  LPDIRECTDRAWSURFACE7 pSurface; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaSurface_CreateAndFade_455B0E\n");

  h = ConvertFrom640AsInt(480);
  w = ConvertFrom640AsInt(640);
  pSurface = CreateSurfaceWH(w, h);
  h2 = ConvertFrom640AsInt(480);
  w2 = ConvertFrom640AsInt(640);
  BltFastWH(pSurface, 0, 0, g_pPrimaryBuffer_4D6C88, 0, 0, w2, h2, 0);
  v2 = RGBToSurfaceFormat(pSurface, 0xFF0000u);
  PlayScreenFade(0, v2, pSurface, 25.0, 0);
  SelfBltDC(pSurface);
  colorFade = RGBToSurfaceFormat(pSurface, 0xFF0000u);
  PlayScreenFade(1, colorFade, pSurface, 25.0, 0);
  pSurface->Release();
}

// DrawSpriteListEcho(pSprites, nUnused, fFadeStep)
// pSprites = CHanaSpriteT3F6_S1FC*[10]. Draws each sprite with alpha>0 plus a vertical
// echo/ghost trail: SetScale 0.5f, SetPos (nX, nY - echoOff*dir), draw main; then 3 echo
// copies stepping Y by +10*dir with alpha*0.3 each, restoring alpha/pos after. fFadeStep is
// the per-frame alpha drop (fFadeStep/50). dir = +1 for i<5, -1 for i>=5.
void __cdecl hook_DrawSpriteListEcho_45B2FB(CHanaSpriteT3F6_S1FC **arrSprites, int nTargetIdx, float fDeltaTime)
{
  double fPosYmin; // st7
  float alpha; // [esp+4Ch] [ebp-18h]
  float alphaa; // [esp+4Ch] [ebp-18h]
  float fPosY3; // [esp+4Ch] [ebp-18h]
  float alphac; // [esp+4Ch] [ebp-18h]
  float fPosY2; // [esp+4Ch] [ebp-18h]
  float fPosX; // [esp+50h] [ebp-14h] BYREF
  int i; // [esp+54h] [ebp-10h]
  float fOldAlpha; // [esp+58h] [ebp-Ch]
  float fPosY; // [esp+5Ch] [ebp-8h] BYREF
  float fDir; // [esp+60h] [ebp-4h]
  int nTargetIdxa; // [esp+70h] [ebp+Ch]

  ONCELOG("[HNH:SUB] DrawSpriteListEcho_45B2FB\n");
  
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
  if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
  {
    for ( nTargetIdxa = 0; nTargetIdxa < 10; ++nTargetIdxa )
    {
      if ( arrSprites[nTargetIdxa] && arrSprites[nTargetIdxa]->fAlpha_144 > 0.0 )
      {
        if ( nTargetIdxa >= 5 )
          fDir = -1.0;
        else
          fDir = 1.0;
        CHanaSpriteT3F6__SetScale_414B09(arrSprites[nTargetIdxa], 2.5);
        CHanaSpriteT3F6__GetPos_414A7A(arrSprites[nTargetIdxa], &fPosX, &fPosY);
        alpha = arrSprites[nTargetIdxa]->fAlpha_144 - fDeltaTime / 50.0;
        CHanaObj__SetAlpha3F6(arrSprites[nTargetIdxa], alpha);
        alphaa = 150.0 - 150.0 * arrSprites[nTargetIdxa]->fAlpha_144;
        fPosYmin = ConvertFrom640AsFloat(alphaa);
        fPosY3 = fPosY - fPosYmin * fDir;
        CHanaSpriteT3F6__SetPos_414A5D(arrSprites[nTargetIdxa], fPosX, fPosY3);
        DrawSprite2D(arrSprites[nTargetIdxa]);
        fOldAlpha = arrSprites[nTargetIdxa]->fAlpha_144;
        arrSprites[nTargetIdxa]->fAlpha_144 = arrSprites[nTargetIdxa]->fAlpha_144 * 0.30000001;
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
        for ( i = 0; i < 3; ++i )
        {
          alphac = arrSprites[nTargetIdxa]->fAlpha_144 - fDeltaTime / 50.0;
          CHanaObj__SetAlpha3F6(arrSprites[nTargetIdxa], alphac);
          fPosY2 = 10.0 * fDir + arrSprites[nTargetIdxa]->destWHZ_138.y;
          CHanaSpriteT3F6__SetPos_414A5D(arrSprites[nTargetIdxa], fPosX, fPosY2);
          DrawSprite2D(arrSprites[nTargetIdxa]);
        }
        arrSprites[nTargetIdxa]->fAlpha_144 = fOldAlpha;
        CHanaSpriteT3F6__SetPos_414A5D(arrSprites[nTargetIdxa], fPosX, fPosY);
      }
    }
    g_pD3DDevice_4D6C78->EndScene();
  }
}

// Mouse target-select dialog (follows PrepareTargetSelect's fade-in). Three temp sprites: pCursorSpr = mouse.bmp masked cursor (path id 1003) tracking the mouse;
// pBackdropSpr = 512x512 ONE-TIME capture of the frozen screen (D3DXLoadTextureFromSurface) drawn fullscreen as the backdrop; pWaveSpr = 128x128 LIVE-Capture
// shimmer - its texture is reloaded from the framebuffer EVERY frame and drawn as a full-screen quad whose corners wobble +/-1px (sin(fSinPhase)/cos(fCosPhase),
// rates 0.1/-0.07 per sec) - a water/heat-shimmer distortion over the whole dialog (the old 'swimming fish cursor' note was wrong).
// Sprites: [3] prompt (320,100) fades in; [8] hint (320,200); LEFT target pair [9]+glow[11] at (200,300); RIGHT pair [10]+glow[12] at (440,300);
// glows cross-fade +/- delta/10, active side re-drawn with DESTBLEND=2 (additive). Hover test: XZ distance to an anchor < 50px (2500 = 50^2).
// nChoice = 1 LEFT / 0 RIGHT / -1 neither. Loop exits on a fresh button PRESS edge (bCurBtn==1 != bPrevBtn) while hovering a side - so -1 
int __cdecl hook_ShowMouseSelectDialog_4573CF(CHanaSpriteT3F6_S1FC **arrSprModes, LPDIRECTSOUNDBUFFER *pArrSndBuf)
{
  float X; // [esp+30h] [ebp-1D4h]
  float Xa; // [esp+30h] [ebp-1D4h]
  float Xb; // [esp+30h] [ebp-1D4h]
  float Xc; // [esp+30h] [ebp-1D4h]
  float Xd; // [esp+30h] [ebp-1D4h]
  float Xe; // [esp+30h] [ebp-1D4h]
  float fCurX; // [esp+30h] [ebp-1D4h]
  float X_4; // [esp+34h] [ebp-1D0h]
  float X_4a; // [esp+34h] [ebp-1D0h]
  float X_4b; // [esp+34h] [ebp-1D0h]
  float X_4c; // [esp+34h] [ebp-1D0h]
  float X_4d; // [esp+34h] [ebp-1D0h]
  float X_4e; // [esp+34h] [ebp-1D0h]
  float fCurY; // [esp+34h] [ebp-1D0h]
  float fW0X; // [esp+34h] [ebp-1D0h]
  float fW0Y; // [esp+34h] [ebp-1D0h]
  float fW1X; // [esp+34h] [ebp-1D0h]
  float fW1Y; // [esp+34h] [ebp-1D0h]
  float fW2X; // [esp+34h] [ebp-1D0h]
  float fW2Y; // [esp+34h] [ebp-1D0h]
  float fW3X; // [esp+34h] [ebp-1D0h]
  float fW3Y; // [esp+34h] [ebp-1D0h]
  float fGlowLIn; // [esp+34h] [ebp-1D0h]
  float fGlowROut; // [esp+34h] [ebp-1D0h]
  float fGlowLOut; // [esp+34h] [ebp-1D0h]
  float fGlowRIn; // [esp+34h] [ebp-1D0h]
  float fGlowLOut2; // [esp+34h] [ebp-1D0h]
  float fGlowROut2; // [esp+34h] [ebp-1D0h]
  float fDyRight; // [esp+38h] [ebp-1CCh]
  float fDxRightSq; // [esp+3Ch] [ebp-1C8h]
  float fDxRight; // [esp+40h] [ebp-1C4h]
  float fDyLeft; // [esp+44h] [ebp-1C0h]
  float fDxLeftSq; // [esp+48h] [ebp-1BCh]
  float fDxLeft; // [esp+4Ch] [ebp-1B8h]
  int nDeadInitCtr; // [esp+70h] [ebp-194h]
  D3DXVECTOR3 *pDeadInitPtr; // [esp+78h] [ebp-18Ch]
  float fTargetAlpha; // [esp+7Ch] [ebp-188h]
  CHanaSpriteT3F6_S1FC *pBackdropSpr; // [esp+80h] [ebp-184h] BYREF
  CHAR szBmpPath[256]; // [esp+84h] [ebp-180h] BYREF
  float fPromptAlpha; // [esp+184h] [ebp-80h]
  CHanaSpriteT3F6_S1FC *pWaveSpr; // [esp+188h] [ebp-7Ch] BYREF
  __int16 bPrevBtn; // [esp+18Ch] [ebp-78h]
  CInputState *pInput; // [esp+190h] [ebp-74h]
  int nChoice; // [esp+194h] [ebp-70h]
  D3DXVECTOR3 vtxQuadWave[4]; // [esp+198h] [ebp-6Ch] BYREF
  float fSinPhase; // [esp+1C8h] [ebp-3Ch]
  float fCosPhase; // [esp+1CCh] [ebp-38h]
  int nMouseY; // [esp+1D0h] [ebp-34h]
  float fDelta; // [esp+1D4h] [ebp-30h]
  CHanaSpriteT3F6_S1FC *pCursorSpr; // [esp+1D8h] [ebp-2Ch] BYREF
  int nMouseX; // [esp+1DCh] [ebp-28h]
  __int16 bCurBtn; // [esp+1E0h] [ebp-24h]
  struct tagRECT rcScreen; // [esp+1E4h] [ebp-20h] BYREF
  struct tagRECT rc; // [esp+1F4h] [ebp-10h] BYREF

  ONCELOG("[HNH:SUB] ShowMouseSelectDialog_4573CF\n");

  nChoice = -1;
  fSinPhase = 0.0;
  fCosPhase = 0.0;
  fPromptAlpha = 0.0;
  fTargetAlpha = -0.85000002;
  rcScreen.left = 0;
  rcScreen.top = 0;
  rcScreen.right = ConvertFrom640AsInt(640);
  rcScreen.bottom = ConvertFrom640AsInt(480);
  nDeadInitCtr = 4;
  for ( pDeadInitPtr = vtxQuadWave; --nDeadInitCtr >= 0; ++pDeadInitPtr )
    ;
  pCursorSpr = 0;
  pWaveSpr = 0;
  pBackdropSpr = 0;
  g_sceneState_52F7E0.byte_32 = 127;
  RestartInputThread(0);
  RestartInputThread(2u);
  wsprintfA(szBmpPath, "mouse.bmp");
  CHanaObj__GetRealPath_486D9A(szBmpPath, 1003);
  LoadBmpWithMask_413A5E(&pCursorSpr, szBmpPath, szBmpPath);
  CHanaObj__NewSprite3F6byWH(&pWaveSpr, 0x80u, 0x80u);
  SetRect(&rc, 2, 2, pWaveSpr->dwWidth_1F0 - 2, pWaveSpr->dwHeight_1F4 - 2);
  CHanaSpriteT3F6__SetSrcRect_414B28(pWaveSpr, &rc);
  CHanaObj__SetAlpha3F6(pWaveSpr, 0.80000001);
  CHanaObj__NewSprite3F6byWH(&pBackdropSpr, 0x200u, 0x200u);
  SetRect(&rc, 0, 0, pBackdropSpr->dwWidth_1F0, pBackdropSpr->dwHeight_1F4);
  CHanaSpriteT3F6__SetSrcRect_414B28(pBackdropSpr, &rc);
  CHanaObj__SetAlpha3F6(pBackdropSpr, 1.0);
  FlushD3DAndTouchSurface(g_pPrimaryBuffer_4D6C88);
  D3DXLoadTextureFromSurface(
    g_pD3DDevice_4D6C78,
    pBackdropSpr->pTexture_134,
    0xFFFFFFFF,
    g_pPrimaryBuffer_4D6C88,
    &rcScreen,
    0,
    D3DX_FT_POINT);
  X_4 = ConvertFrom640AsFloat(100.0);
  X = ConvertFrom640AsFloat(320.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[3], X, X_4);
  X_4a = ConvertFrom640AsFloat(200.0);
  Xa = ConvertFrom640AsFloat(320.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[8], Xa, X_4a);
  X_4b = ConvertFrom640AsFloat(300.0);
  Xb = ConvertFrom640AsFloat(200.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[9], Xb, X_4b);
  X_4c = ConvertFrom640AsFloat(300.0);
  Xc = ConvertFrom640AsFloat(440.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[10], Xc, X_4c);
  X_4d = ConvertFrom640AsFloat(300.0);
  Xd = ConvertFrom640AsFloat(200.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[11], Xd, X_4d);
  X_4e = ConvertFrom640AsFloat(300.0);
  Xe = ConvertFrom640AsFloat(440.0);
  CHanaSpriteT3F6__SetPos_414A5D(arrSprModes[12], Xe, X_4e);
  CHanaObj__SetAlpha3F6(arrSprModes[11], 0.0);
  CHanaObj__SetAlpha3F6(arrSprModes[12], 0.0);
  g_pRenderCtx->Clear(2);
  FrameTimer_BeginFrame();
  FrameTimer_EndFrame();
  MutexThreadWait_4815EC(1);
  pInput = GetInputState_481B37();
  bCurBtn = (unsigned __int8)pInput->mouseBtn_4;
  MutexThreadWait_4815EC(1);
  do
  {
    Sleep(0);
    MutexThreadWait_4815EC(1);
    pInput = GetInputState_481B37();
    bPrevBtn = bCurBtn;
    nMouseX = pInput->mouseX_0;
    nMouseY = pInput->mouseY_2;
    bCurBtn = (unsigned __int8)pInput->mouseBtn_4;
    MutexThreadWait_4815EC(0);
    fDelta = GetFrameTime();
    FrameTimer_EndFrame();
    fSinPhase = 0.1 * fDelta + fSinPhase;
    fCosPhase = fCosPhase - 0.07 * fDelta;
    fPromptAlpha = fDelta / 50.0 + fPromptAlpha;
    if ( fPromptAlpha > 1.0 )
      fPromptAlpha = 1.0;
    CHanaObj__SetAlpha3F6(arrSprModes[3], fPromptAlpha);
    fTargetAlpha = fDelta / 50.0 + fTargetAlpha;
    if ( fTargetAlpha > 1.0 )
      fTargetAlpha = 1.0;
    CHanaObj__SetAlpha3F6(arrSprModes[8], fTargetAlpha);
    CHanaObj__SetAlpha3F6(arrSprModes[9], fTargetAlpha);
    CHanaObj__SetAlpha3F6(arrSprModes[10], fTargetAlpha);
    CHanaObj__SetAlpha3F6(pCursorSpr, fTargetAlpha);
    fCurY = (float)nMouseY;
    fCurX = (float)nMouseX;
    CHanaSpriteT3F6__SetPos_414A5D(pCursorSpr, fCurX, fCurY);
    FlushD3DAndTouchSurface(g_pPrimaryBuffer_4D6C88);
    D3DXLoadTextureFromSurface(
      g_pD3DDevice_4D6C78,
      pWaveSpr->pTexture_134,
      0xFFFFFFFF,
      g_pPrimaryBuffer_4D6C88,
      &rcScreen,
      0,
      D3DX_FT_POINT);
    nChoice = -1;
    if ( SUCCEEDED(g_pD3DDevice_4D6C78->BeginScene()) )
    {
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
      DrawSpriteLTRB(pBackdropSpr, rcScreen.left, rcScreen.top, rcScreen.right, rcScreen.bottom);

      fW0X = sin(fSinPhase) - 1.0;
      vtxQuadWave[0].x = ConvertFrom640AsFloat(fW0X);
      fW0Y = cos(fCosPhase) - 1.0;
      vtxQuadWave[0].y = ConvertFrom640AsFloat(fW0Y);
      vtxQuadWave[0].z = 0.0;
      fW1X = sin(fSinPhase) + 640.0 + 1.0;
      vtxQuadWave[1].x = ConvertFrom640AsFloat(fW1X);
      fW1Y = cos(fCosPhase) - 1.0;
      vtxQuadWave[1].y = ConvertFrom640AsFloat(fW1Y);
      vtxQuadWave[1].z = 0.0;
      fW2X = sin(fSinPhase) + 640.0 + 1.0;
      vtxQuadWave[2].x = ConvertFrom640AsFloat(fW2X);
      fW2Y = cos(fCosPhase) + 480.0 + 1.0;
      vtxQuadWave[2].y = ConvertFrom640AsFloat(fW2Y);
      vtxQuadWave[2].z = 0.0;
      fW3X = sin(fSinPhase) - 1.0;
      vtxQuadWave[3].x = ConvertFrom640AsFloat(fW3X);
      fW3Y = cos(fCosPhase) + 480.0 + 1.0;
      vtxQuadWave[3].y = ConvertFrom640AsFloat(fW3Y);
      vtxQuadWave[3].z = 0.0;

      DrawSprite3D(pWaveSpr, vtxQuadWave);
      DrawSprite2D(arrSprModes[3]);
      if ( fTargetAlpha > 0.0 )
      {
        DrawSprite2D(arrSprModes[8]);
        fDxLeft = ConvertFrom640AsFloat(200.0) - (double)nMouseX;
        fDxLeftSq = (ConvertFrom640AsFloat(200.0) - (double)nMouseX) * fDxLeft;
        fDyLeft = ConvertFrom640AsFloat(300.0) - (double)nMouseY;
        if ( (ConvertFrom640AsFloat(300.0) - (double)nMouseY) * fDyLeft + fDxLeftSq >= 2500.0 )
        {
          fDxRight = ConvertFrom640AsFloat(440.0) - (double)nMouseX;
          fDxRightSq = (ConvertFrom640AsFloat(440.0) - (double)nMouseX) * fDxRight;
          fDyRight = ConvertFrom640AsFloat(300.0) - (double)nMouseY;
          if ( (ConvertFrom640AsFloat(300.0) - (double)nMouseY) * fDyRight + fDxRightSq >= 2500.0 )
          {
            nChoice = -1;
            DrawSprite2D(arrSprModes[9]);
            DrawSprite2D(arrSprModes[10]);
            fGlowLOut2 = arrSprModes[11]->fAlpha_144 - fDelta / 10.0;
            CHanaObj__SetAlpha3F6(arrSprModes[11], fGlowLOut2);
            fGlowROut2 = arrSprModes[12]->fAlpha_144 - fDelta / 10.0;
            CHanaObj__SetAlpha3F6(arrSprModes[12], fGlowROut2);
            DrawSprite2D(arrSprModes[11]);
          }
          else
          {
            nChoice = 0;
            fGlowLOut = arrSprModes[11]->fAlpha_144 - fDelta / 10.0;
            CHanaObj__SetAlpha3F6(arrSprModes[11], fGlowLOut);
            fGlowRIn = fDelta / 10.0 + arrSprModes[12]->fAlpha_144;
            CHanaObj__SetAlpha3F6(arrSprModes[12], fGlowRIn);
            DrawSprite2D(arrSprModes[9]);
            DrawSprite2D(arrSprModes[11]);
            DrawSprite2D(arrSprModes[12]);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
          }
          DrawSprite2D(arrSprModes[12]);
        }
        else
        {
          nChoice = 1;
          fGlowLIn = fDelta / 10.0 + arrSprModes[11]->fAlpha_144;
          CHanaObj__SetAlpha3F6(arrSprModes[11], fGlowLIn);
          fGlowROut = arrSprModes[12]->fAlpha_144 - fDelta / 10.0;
          CHanaObj__SetAlpha3F6(arrSprModes[12], fGlowROut);
          DrawSprite2D(arrSprModes[11]);
          DrawSprite2D(arrSprModes[12]);
          DrawSprite2D(arrSprModes[10]);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 2);
          DrawSprite2D(arrSprModes[11]);
        }
        DrawSprite2D(pCursorSpr);
      }
      g_pD3DDevice_4D6C78->EndScene();
    }
 
    PresentFrame();
  }
  while ( bCurBtn != 1 || bCurBtn == bPrevBtn || nChoice < 0 );
  if ( pArrSndBuf[2] )
    DSBufferPlay(pArrSndBuf[2], 0);
  if ( pCursorSpr )
  {
    CHanaObj__Free_40C1E4(pCursorSpr);
    pCursorSpr = 0;
  }
  if ( pWaveSpr )
  {
    CHanaObj__Free_40C1E4(pWaveSpr);
    pWaveSpr = 0;
  }
  if ( pBackdropSpr )
  {
    CHanaObj__Free_40C1E4(pBackdropSpr);
    pBackdropSpr = 0;
  }
  RestartInputThread(0);
  return nChoice;
}

void register_PlayExploring_45BCAE(int cfg) {
  if (cfg == 1) {
    PlayExploring_45BCAE = hook_PlayExploring_45BCAE;
    CHanaAnimSetT3F1__RemoveAnimT3F0_404349 = hook_CHanaAnimSetT3F1__RemoveAnimT3F0_404349;
    InitHudResources_4490A7 = hook_InitHudResources_4490A7;
    UpdateCameraFollowChase_445525 = hook_UpdateCameraFollowChase_445525;
    SetupItemBodies_459F81 = hook_SetupItemBodies_459F81;
    UpdateHeroControl_45A513 = hook_UpdateHeroControl_45A513;
    FindTargetInRange_45A1AD = hook_FindTargetInRange_45A1AD;
    ApplyTargetEffect_45A2BA = hook_ApplyTargetEffect_45A2BA;
    CheckSceneTransition_459768 = hook_CheckSceneTransition_459768;
    GetSceneEventTime_459CA2 = hook_GetSceneEventTime_459CA2;
    ShowMouseSelectDialog_4573CF = hook_ShowMouseSelectDialog_4573CF;
    HanaMatrix_BuildLookAtAndTransform = hook_HanaMatrix_BuildLookAtAndTransform;
    Frame_ComputeWorldAABB = hook_Frame_ComputeWorldAABB;
    HanaTexture_CreateFromFile = hook_HanaTexture_CreateFromFile;
    HanaSurface_CreateAndFade = hook_HanaSurface_CreateAndFade;

    UpdateCameraLinkedObjects_44451F = hook_UpdateCameraLinkedObjects_44451F;
    BuildMapSubFrames_443DD5 = hook_BuildMapSubFrames_443DD5;
    UpdateLockonEvent_45953B = hook_UpdateLockonEvent_45953B;
    UpdateHudGauge_44BC36 = hook_UpdateHudGauge_44BC36;
    PrepareTargetSelect_45B5BD = hook_PrepareTargetSelect_45B5BD;
    DrawSpriteListEcho_45B2FB = hook_DrawSpriteListEcho_45B2FB;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x45BCAE, hook_PlayExploring_45BCAE, "PlayExploring_45BCAE"); // [OK]

  hnh_Hook(0x45b5bd, hook_PrepareTargetSelect_45B5BD, "PrepareTargetSelect_45B5BD");
  hnh_Hook(0x44bc36, hook_UpdateHudGauge_44BC36, "UpdateHudGauge_44BC36"); // [OK]
  hnh_Hook(0x4490a7, hook_InitHudResources_4490A7, "InitHudResources_4490A7");  // [OK]

  hnh_Hook(0x407b73, hook_HanaMatrix_BuildLookAtAndTransform, "HanaMatrix_BuildLookAtAndTransform_407B73"); // [OK]
  hnh_Hook(0x407e5b, hook_Frame_ComputeWorldAABB, "Frame_ComputeWorldAABB_407E5B");  // [OK]
  hnh_Hook(0x443dd5, hook_BuildMapSubFrames_443DD5, "BuildMapSubFrames_443DD5");  // [OK]
  hnh_Hook(0x4135ed, hook_HanaTexture_CreateFromFile, "HanaTexture_CreateFromFile_4135ED"); // [OK]
  hnh_Hook(0x459f81, hook_SetupItemBodies_459F81, "SetupItemBodies_459F81");  // [OK]
  hnh_Hook(0x45a513, hook_UpdateHeroControl_45A513, "UpdateHeroControl_45A513");  // [OK]
  hnh_Hook(0x45a1ad, hook_FindTargetInRange_45A1AD, "FindTargetInRange_45A1AD");  // [OK]
  hnh_Hook(0x45953b, hook_UpdateLockonEvent_45953B, "UpdateLockonEvent_45953B"); // [OK]
  hnh_Hook(0x459768, hook_CheckSceneTransition_459768, "CheckSceneTransition_459768");  // [OK]
  hnh_Hook(0x459ca2, hook_GetSceneEventTime_459CA2, "GetSceneEventTime_459CA2"); // [OK]
  hnh_Hook(0x45b2fb, hook_DrawSpriteListEcho_45B2FB, "DrawSpriteListEcho_45B2FB"); // [OK]
  hnh_Hook(0x445525, hook_UpdateCameraFollowChase_445525, "UpdateCameraFollowChase_445525"); // [OK]
  hnh_Hook(0x45a2ba, hook_ApplyTargetEffect_45A2BA, "ApplyTargetEffect_45A2BA"); // [OK]
  hnh_Hook(0x404349, hook_CHanaAnimSetT3F1__RemoveAnimT3F0_404349, "CHanaAnimSetT3F1__RemoveAnimT3F0_404349"); // [OK] 
  hnh_Hook(0x44451f, hook_UpdateCameraLinkedObjects_44451F, "UpdateCameraLinkedObjects_44451F"); // [OK]
  hnh_Hook(0x455b0e, hook_HanaSurface_CreateAndFade, "HanaSurface_CreateAndFade_455B0E"); // [OK]
  hnh_Hook(0x4573cf, hook_ShowMouseSelectDialog_4573CF, "ShowMouseSelectDialog_4573CF"); // [OK]

  hnh_HookOn();
}
