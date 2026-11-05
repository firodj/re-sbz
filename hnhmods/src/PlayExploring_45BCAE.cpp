#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayExploring_45BCAE)(int nStageIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x45BCAE);

void (__cdecl *CHanaAnimSetT3F1__RemoveAnimT3F0_404349)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)
= reinterpret_cast<void (__cdecl*)(CHanaAnimSetT3F1_S150 *pAnimSet, CHanaAnimT3F0_S240 *pAnim)>(0x404349);

// --- not yet extracted; pinned here call-only ---

CObjItemRef_S208 *(__cdecl*UpdateCameraLinkedObjects_44451F)(CHanaFrameT3EC_S354 *pCameraFrame) =
    reinterpret_cast<CObjItemRef_S208 *(*)(CHanaFrameT3EC_S354 *)>(0x44451F);

int (__cdecl*BuildMapSubFrames_443DD5)(CHanaFrameT3EC_S354 *pMapFrame) =
    reinterpret_cast<int(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x443DD5);

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

char (__cdecl*UpdateLockonEvent_45953B)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameT3EC_S354 *pLockonFrame, int nMapIndex) =
    reinterpret_cast<char(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *,
      CHanaFrameT3EC_S354 *, int)>(0x45953B);

int (__cdecl*CheckSceneTransition_459768)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, int *pnMapIndex, int *pnSubIndex, int nStageIndex) =
    reinterpret_cast<int(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, int *, int *,
      int)>(0x459768);

double (__cdecl*GetSceneEventTime_459CA2)(CHanaSceneT458_S504 *pScene,
    CHanaFrameT3EC_S354 *pHeroFrame, unsigned int *pnCursor, int nMapIndex) =
    reinterpret_cast<double(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, unsigned int *,
      int)>(0x459CA2);

HRESULT (__cdecl*UpdateHudGauge_44BC36)(CHanaSpriteT3F6_S1FC **pHudRes, int nCurHp, int nMaxHp) =
    reinterpret_cast<HRESULT(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, int)>(0x44BC36);


int (__cdecl*PrepareTargetSelect_45B5BD)(CSekBody *pHeroBody, CSekBody **pItemBodies,
    CSekBody **pSpriteNames, CHanaFrameT3EC_S354 *pMapFrame, CHanaSpriteT3F6_S1FC **arrSprNumbers,
    CHanaSpriteT3F6_S1FC **arrSprTimes, float fHeroHp, float fHeroHpMax, int nTimeLimit,
    unsigned int nElapsed) =
    reinterpret_cast<int(__cdecl*)(CSekBody *, CSekBody **, CSekBody **, CHanaFrameT3EC_S354 *,
      CHanaSpriteT3F6_S1FC **, CHanaSpriteT3F6_S1FC **, float, float, int, unsigned int)>(0x45B5BD);

int (__cdecl*ShowMouseSelectDialog_4573CF)(CHanaSpriteT3F6_S1FC **arrSprModes,
    LPDIRECTSOUNDBUFFER *pArrSndBuf) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC **, LPDIRECTSOUNDBUFFER *)>(0x4573CF);

HRESULT (__cdecl*DrawSpriteListEcho_45B2FB)(CHanaSpriteT3F6_S1FC **arrSprites, int nTargetIdx,
    float fDeltaTime) =
    reinterpret_cast<HRESULT(__cdecl*)(CHanaSpriteT3F6_S1FC **, int, float)>(0x45B2FB);


void (__cdecl *HanaMatrix_BuildLookAtAndTransform)(CHanaFrameT3EC_S354 *pFoeFrame, CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameRootT3EC_S2D4 *pSceneFrame, BOOL bSkip)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFoeFrame, CHanaFrameT3EC_S354 *pHeroFrame, CHanaFrameRootT3EC_S2D4 *pSceneFrame, BOOL bSkip)>(0x407b73);
void (__cdecl *Frame_ComputeWorldAABB)(CHanaFrameT3EC_S354 *pFrame)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x407e5b);
void (__cdecl *HanaTexture_CreateFromFile)(CHanaSpriteT3F6_S1FC **ppOutSprite, CHAR *szFileName)
= reinterpret_cast<void (__cdecl*)(CHanaSpriteT3F6_S1FC **ppOutSprite, CHAR *szFileName)>(0x4135ed);


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
        if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) >= -10000)
        {
            if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) <= 0)
                pBgmBuffer->SetVolume((__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0));
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
        MtxMult_4069DD(pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
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
        StopAndRestartThread(1);
        bFirstFrameDone = 0;
        MtxMult_4069DD(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC,
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
                memcpy(g_sceneState_52F7E0.sceneFlags_98, sceneStateLive.sceneFlags_98,
                       sizeof(g_sceneState_52F7E0.sceneFlags_98));               // ...live flags
                StopAndRestartThread(0);
                sceneMenuCopy = sceneStateLive;
                nMenuResult = SystemMenu_4655AB(pBgmBuffer, -1000, 0, fHeroHp, fHeroHpMax,
                                                sceneMenuCopy);
                StopAndRestartThread(1);
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
                            ProjectToScreen_40E2A5(&pCamFrame->mat4x4_after, &pOut);
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
                    MtxMult_4069DD(pSekBody->pSekHeader_220->pFrameT3EC,
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
                        MtxMult_4069DD(pSekBody->pSekHeader_220->pFrameT3EC,
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
                    MtxMult_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
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
                pScene->flt_480 = 1.0f;
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
                    MtxMult_4069DD(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame, fTgtX, pOutY, pOutZ);
                    UpdateBoneLookAtTransform(pHeroBody->pSekHeader_220->pFrameT3EC,
                            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
                            pXpos, pYpos, pZpos, pXYrot, pYXrot, pZXrot);
                    SekBody__UpdateTo2_401647(pHeroBody, pCursorBody->field_200);
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
                    && pHeroBody->field_200 != 16
                    && pHeroBody->field_200 != 10
                    && pHeroBody->field_200 != 11
                    && pHeroBody->field_200 != 13)
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
        StopAndRestartThread(0);
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
    StopAndRestartThread(0);
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

// Chase/follow camera (the standard camera, 11 call sites). Mount point = hero + 22*localUp + 18*localBack (hero frame used as scratch, restored at end). bSnap=1: SNAP hard to the mount (used right after a manual reposition - PrepareTargetSelect, PlayExploring warp; skips smoothing AND orbit clamps). bSnap=0: smooth follow - lerp fTime/25 toward mount, step capped at fTime per frame, orbit clamps around a point 13 behind the hero (radius 3..10, height 15+/-10), then a 3-try wall pull-in settle loop (overhead test): pulls the camera toward the hero's true position (fHeroX/Y/Z) through walls; when blocked, TestMoveCollision snaps it to the contact point and it is lifted by 1/6 of the blocked distance to see over. Finally BuildLookAt aims the camera at hero + 15*up + 9*forward. The initial MtxMult(+1 localZ)/final (-1) pair is a nudge that cancels out; the trailing double hero-restore MtxMult is a dead duplicate.
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
  
  MtxMult_4069DD(pCameraFrame, pCameraFrame, 0.0, 0.0, 1.0);
  HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fCamPosX, &fCamPosY, &fCamPosZ);
  fCamStartX = fCamPosX;
  fCamStartY = fCamPosY;
  fCamStartZ = fCamPosZ;
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
  HanaFrame_GetRelPosition(pHeroFrame, pHeroFrame, &fMountX, &fMountY, &fMountZ);
  fMountOffZ = fMountZ + 18.0;
  fMountOffY = fMountY + 22.0;
  MtxMult_4069DD(pHeroFrame, pHeroFrame, fMountX, fMountOffY, fMountOffZ);
  HanaFrame_GetRelPosition(pHeroFrame, g_pSceneFrame, &fMountX, &fMountY, &fMountZ);
  MtxMult_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
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
  MtxMult_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fMountX, fMountY, fMountZ);
  if ( !bSnap )
  {
    MtxMult_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, 13.0);
    HanaFrame_GetRelPosition(pCameraFrame, pHeroFrame, &fOrbX, &fOrbY, &fOrbZ);
    fDist = sqrt(fOrbX * fOrbX + fOrbZ * fOrbZ);
    fHeightOff = sqrt((fOrbY - 15.0) * (fOrbY - 15.0));
    if ( fDist > 10.0 )
    {
      fOrbX = 10.0 / fDist * fOrbX;
      fOrbZ = 10.0 / fDist * fOrbZ;
      MtxMult_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
    }
    if ( fHeightOff > 10.0 )
    {
      fOrbY = (fOrbY - 15.0) * 10.0 / fHeightOff + 15.0;
      MtxMult_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
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
      MtxMult_4069DD(pCameraFrame, pHeroFrame, fOrbX, fOrbY, fOrbZ);
    }
    HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fCamPosX, &fCamPosY, &fCamPosZ);
    MtxMult_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -10.0);
  }
  for ( iTry = 0; iTry < 3 && !CheckFrameCollision_443CC5(pCameraFrame, pMapFrame, 1); ++iTry )
  {
    HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fPreX, &fLiftY, &fPreZ);
    if ( TestMoveCollision_4443FC(pCameraFrame, pMapFrame, fHeroX, fHeroY, fHeroZ, 1) )
    {
      HanaFrame_GetRelPosition(pCameraFrame, g_pSceneFrame, &fSnapX, &fSnapY, &fSnapZ);
      fLiftY = sqrt((fPreX - fSnapX) * (fPreX - fSnapX) + (fPreZ - fSnapZ) * (fPreZ - fSnapZ)) / 6.0;
      fLiftedY = fSnapY + fLiftY;
      MtxMult_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fSnapX, fLiftedY, fSnapZ);
    }
    else
    {
      MtxMult_4069DD(pCameraFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fCamStartX, fCamStartY, fCamStartZ);
    }
  }
  HanaFrame_GetRelPosition(pHeroFrame, pHeroFrame, &fMountX, &fMountY, &fMountZ);
  fLookOffZ = fMountZ - 9.0;
  fLookOffY = fMountY + 15.0;
  MtxMult_4069DD(pHeroFrame, pHeroFrame, fMountX, fLookOffY, fLookOffZ);
  HanaMatrix_BuildLookAtAndTransform(pCameraFrame, pHeroFrame, pCameraFrame, 0);
  MtxMult_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  MtxMult_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, fHeroX, fHeroY, fHeroZ);
  MtxMult_4069DD(pCameraFrame, pCameraFrame, 0.0, 0.0, -1.0);
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
    vLook.x = pHeroFrame->mat4x4_after._41 - pFoeFrame->mat4x4_after._41;
    vLook.y = pHeroFrame->mat4x4_after._42 - pFoeFrame->mat4x4_after._42;
    vLook.z = pHeroFrame->mat4x4_after._43 - pFoeFrame->mat4x4_after._43;
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
    pFoeFrame->mat4x4_after._11 = vRight.x;
    pFoeFrame->mat4x4_after._21 = vUp.x;
    pFoeFrame->mat4x4_after._31 = vLook.x;
    pFoeFrame->mat4x4_after._12 = vRight.y;
    pFoeFrame->mat4x4_after._22 = vUp.y;
    pFoeFrame->mat4x4_after._32 = vLook.y;
    pFoeFrame->mat4x4_after._13 = vRight.z;
    pFoeFrame->mat4x4_after._23 = vUp.z;
    pFoeFrame->mat4x4_after._33 = vLook.z;
    D3DXMatrixInverse(&mInverse, 0, &pFoeFrame->mat4x4_before);
    D3DXMatrixMultiply(&pFoeFrame->mat4x4_fmrx, &pFoeFrame->mat4x4_after, &mInverse);
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

// Rebind/reset camera-linked objects after an ODS scene load. Walks g_pObjTable; for each object of dwType_204==1009, calls sub_404349 on the sub-objects whose [304] slot equals nCameraId (the freshly loaded camera frame).
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

// Recursively expand a parsed ODF map frame into child frame nodes: marks [0]=1, normalizes via sub_407E5B, builds the T3ED representation (CHanaObj::NewT3EDFromChildT3EC), and for each frame recurses then frees the temp T3ED. Called right after ParseODFFile on the map frame.
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
  D3DXMATRIX cornerMatrices; // [esp+18h] [ebp-27Ch] BYREF
  D3DXMATRIX cornerM1; // [esp+58h] [ebp-23Ch] BYREF
  D3DXMATRIX cornerM2; // [esp+98h] [ebp-1FCh] BYREF
  D3DXMATRIX cornerM3; // [esp+D8h] [ebp-1BCh] BYREF
  D3DXMATRIX cornerM4; // [esp+118h] [ebp-17Ch] BYREF
  D3DXMATRIX cornerM5; // [esp+158h] [ebp-13Ch] BYREF
  D3DXMATRIX cornerM6; // [esp+198h] [ebp-FCh] BYREF
  D3DXMATRIX cornerM7; // [esp+1D8h] [ebp-BCh] BYREF
  D3DXVECTOR3 maxBound; // [esp+218h] [ebp-7Ch]
  D3DXVECTOR3 transformed[8]; // [esp+224h] [ebp-70h] BYREF
  int n; // [esp+284h] [ebp-10h]
  D3DXVECTOR3 minBound; // [esp+288h] [ebp-Ch]

  ONCELOG("[HNH:SUB] Frame_ComputeWorldAABB_407E5B\n");
 
  i = 8;
  for ( k = &cornerMatrices; --i >= 0; ++k )
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
  D3DXMatrixTranslation(&cornerMatrices, maxBound.x, minBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerM1, maxBound.x, minBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerM2, minBound.x, minBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerM3, minBound.x, minBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerM4, maxBound.x, maxBound.y, maxBound.z);
  D3DXMatrixTranslation(&cornerM5, maxBound.x, maxBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerM6, minBound.x, maxBound.y, minBound.z);
  D3DXMatrixTranslation(&cornerM7, minBound.x, maxBound.y, maxBound.z);
  for ( n = 0; n < 8; ++n )
  {
    D3DXMatrixMultiply(&cornerMatrices + n, &cornerMatrices + n, &pFrame->mat4x4_after);
    transformed[n].x = cornerMatrices.m[4 * n + 3][0];
    transformed[n].y = cornerMatrices.m[4 * n + 3][1];
    transformed[n].z = cornerMatrices.m[4 * n + 3][2];
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

// Initialize the battle HUD / select resource arrays (three int* tables, flag=1 at start): the clock-HUD table fed to DrawClockHud, the gauge table fed to UpdateHudGauge, and the mouse-select table fed to ShowMouseSelectDialog. Large setup routine; called once at battle start.
CHanaSpriteT3F6_S1FC *__cdecl hook_InitHudResources_4490A7(
  CHanaSpriteT3F6_S1FC **arrSprNumbers,
  CHanaSpriteT3F6_S1FC **arrSprTimes,
  CHanaSpriteT3F6_S1FC **arrSprModes,
  int nMode)
{
  CHanaSpriteT3F6_S1FC *result; // eax
  String25 arrBmpModes_v5[30]; // [esp+4h] [ebp-DC8h] BYREF
  String25 arrBmpTimes_v32[15]; // [esp+2F4h] [ebp-AD8h] BYREF
  _BYTE v7[250]; // [esp+46Bh] [ebp-961h] BYREF
  String25 arrBmpNumbers_v49[15]; // [esp+568h] [ebp-864h] BYREF
  CHAR FileName[256]; // [esp+6E0h] [ebp-6ECh] BYREF
  int i; // [esp+7E0h] [ebp-5ECh]
  CHanaSpriteT3F6_S1FC *pSprite_v63; // [esp+7E4h] [ebp-5E8h] BYREF
  CHAR szPath[256]; // [esp+7E8h] [ebp-5E4h] BYREF
  String25 szOfaBMP_v66[32]; // [esp+8E8h] [ebp-4E4h] BYREF
  _BYTE v14[448]; // [esp+C08h] [ebp-1C4h] BYREF
  __int16 v15; // [esp+DC8h] [ebp-4h]
  CHanaSpriteT3F6_S1FC **arrSprTimes_a2a; // [esp+DD8h] [ebp+Ch]
  CHanaSpriteT3F6_S1FC **arrSprLifes_v99; // [esp+DD8h] [ebp+Ch]

  ONCELOG("[HNH:SUB] InitHudResources_4490A7\n");
  
  pSprite_v63 = 0;
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
  for ( i = 0; i < 11; ++i )
  {
    wsprintfA(FileName, "%s", arrBmpNumbers_v49[i].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, FileName);
    *arrSprNumbers++ = pSprite_v63;
  }
  for ( i = 11; i <= 13; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpNumbers_v49[i].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpNumbers_v49[i + 1].str);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprNumbers++ = pSprite_v63;
  }
  for ( i = 0; i <= 1; ++i )
  {
    wsprintfA(FileName, "%s", arrBmpTimes_v32[2 * i].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpTimes_v32[2 * i + 1].str);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprTimes++ = pSprite_v63;
  }
  wsprintfA(FileName, "%s", arrBmpTimes_v32[4].str);
  CHanaObj__GetRealPath_486D9A(FileName, 1003); // get g_szSPRPath
  HanaTexture_CreateFromFile(&pSprite_v63, FileName);
  *arrSprTimes = pSprite_v63;
  arrSprTimes_a2a = arrSprTimes + 1;
  for ( i = 0; i < 10; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpTimes_v32[i + 5].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpTimes_v32[i + 6].str);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    *arrSprTimes_a2a++ = pSprite_v63;
  }
  wsprintfA(FileName, "%s", szOfaBMP_v66[0].str);
  CHanaObj__GetRealPath_486D9A(FileName, 1003); // get g_szSPRPath
  wsprintfA(szPath, "%s", szOfaBMP_v66[1].str);
  CHanaObj__GetRealPath_486D9A(szPath, 1003);   // get g_szSPRPath
  LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
  *arrSprTimes_a2a = pSprite_v63;
  result = (CHanaSpriteT3F6_S1FC *)(arrSprTimes_a2a + 1);
  arrSprLifes_v99 = arrSprTimes_a2a + 1;
  for ( i = 0; i < 6; i += 2 )
  {
    wsprintfA(FileName, "%s", szOfaBMP_v66[6 * nMode + 2 + i].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", szOfaBMP_v66[6 * nMode + 3 + i].str);
    CHanaObj__GetRealPath_486D9A(szPath, 1003); // get g_szSPRPath
    LoadBmpWithMask_413A5E(&pSprite_v63, FileName, szPath);
    result = pSprite_v63;
    *arrSprLifes_v99++ = pSprite_v63;
  }
  for ( i = 0; i < 28; i += 2 )
  {
    wsprintfA(FileName, "%s", arrBmpModes_v5[i].str);
    CHanaObj__GetRealPath_486D9A(FileName, 1003);// get g_szSPRPath
    wsprintfA(szPath, "%s", arrBmpModes_v5[i + 1].str);
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
  int v5; // [esp+10h] [ebp-Ch]
  HRESULT hr; // [esp+14h] [ebp-8h]
  int v7; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaTexture_CreateFromFile_4135ED\n");
 
  dwFlags = 256;
  pSprite = (CHanaSpriteT3F6_S1FC *)MemAlloc_40DEF7(0x1FCu);
  memset(pSprite, 0, sizeof(CHanaSpriteT3F6_S1FC));
  pSprite->dwType = 0x3F6;
  v5 = -1;
  v7 = -1;
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

void register_PlayExploring_45BCAE() {

  hnh_Hook(0x45BCAE, hook_PlayExploring_45BCAE, "PlayExploring_45BCAE"); // [OK]
  hnh_Hook(0x445525, hook_UpdateCameraFollowChase_445525, "UpdateCameraFollowChase_445525");
  hnh_Hook(0x407b73, hook_HanaMatrix_BuildLookAtAndTransform, "HanaMatrix_BuildLookAtAndTransform_407B73");
  hnh_Hook(0x404349, hook_CHanaAnimSetT3F1__RemoveAnimT3F0_404349, "CHanaAnimSetT3F1__RemoveAnimT3F0_404349");
  hnh_Hook(0x44451f, hook_UpdateCameraLinkedObjects_44451F, "UpdateCameraLinkedObjects_44451F");
  hnh_Hook(0x443dd5, hook_BuildMapSubFrames_443DD5, "BuildMapSubFrames_443DD5");
  hnh_Hook(0x407e5b, hook_Frame_ComputeWorldAABB, "Frame_ComputeWorldAABB_407E5B");
  hnh_Hook(0x4490a7, hook_InitHudResources_4490A7, "InitHudResources_4490A7");
  hnh_Hook(0x4135ed, hook_HanaTexture_CreateFromFile, "HanaTexture_CreateFromFile_4135ED");

  hnh_HookOff();
  hnh_HookOn();
}
