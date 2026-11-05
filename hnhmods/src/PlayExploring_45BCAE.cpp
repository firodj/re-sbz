#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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

// === PINNED GAME FUNCTIONS ===
int (__cdecl*PlayExploring_45BCAE)(int nStageIndex) =
    reinterpret_cast<int(__cdecl*)(int)>(0x45BCAE);

// --- not yet extracted; pinned here call-only ---

int (__cdecl*LoadSekAndOdf_4445DD)(LPCSTR pszSekFile, LPCSTR pszOdfFile, CSekBody **ppSekBody) =
    reinterpret_cast<int(__cdecl*)(LPCSTR, LPCSTR, CSekBody **)>(0x4445DD);

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


// === FUNCTIONS ===

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

void register_PlayExploring_45BCAE() {

  hnh_Hook(0x45BCAE, hook_PlayExploring_45BCAE, "PlayExploring_45BCAE"); // [OK]
  hnh_Hook(0x445525, hook_UpdateCameraFollowChase_445525, "UpdateCameraFollowChase_445525");
  hnh_Hook(0x407b73, hook_HanaMatrix_BuildLookAtAndTransform, "HanaMatrix_BuildLookAtAndTransform_407B73");


  hnh_HookOff();
  hnh_HookOn();
}
