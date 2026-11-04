#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

DWORD &g_dwBattleMode = *(DWORD*)0x52ED04;

// Main battle/map gameplay scene (GameMainLoop nSceneType=map case). nStageIndex 0..10 selects one of 11 stage entries via mapIndexTable/subStageTable/timeLimitTable: loads the ODF map frame Map\m0X\at0X.ODF (skips 07), the ODS scene sch\map0X.ods, the hero body aOtokoOtokoSek/.odf, a lock-on body aLockonLockSek/.odf, 7 item bodies item\i11..i17, BGM s007/8/9.wav + 10 SFX, and 10 mask sprites. Runs a timer-driven frame loop: hero control (sub_45A513), target lookup (sub_45A1AD/sub_45A2BA), dialogue mode (DrawDialogueWindow_480B2D + ParseExecStr_487D89), clock HUD (DrawClockHud_44AF5B), ESC->SystemMenu_4655AB, right-click x5 speed, middle-click toggles skip. When fHeroHp<=0 or the countdown (100*nTimeLimit - elapsed/10) hits 0, fades out and shows ShowMouseSelectDialog_4573CF to pick the next target; on the last real stage (nStageIndex==7) sets g_sceneState_52F7E0.stc9C.sceneSteps=80. Returns 0 on normal exit, >0 if the system menu asked to leave.
int __cdecl hook_PlayExploring_V2_45BCAE(int nStageIndex)
{
  LPDIRECTSOUNDBUFFER Wav_422E7B; // eax
  double SceneEventTime_459CA2; // st7
  CSceneState sceneState_v4; // [esp-144h] [ebp-31CCh] BYREF
  int nSelectResult; // [esp+54h] [ebp-3034h]
  float pZXrot; // [esp+58h] [ebp-3030h] BYREF
  float pYXrot; // [esp+5Ch] [ebp-302Ch] BYREF
  float pOutZ; // [esp+60h] [ebp-3028h] BYREF
  float pXYrot; // [esp+64h] [ebp-3024h] BYREF
  float pOutY; // [esp+68h] [ebp-3020h] BYREF
  CSekBody *v11; // [esp+6Ch] [ebp-301Ch]
  float fTgtX; // [esp+70h] [ebp-3018h] BYREF
  float pZpos; // [esp+74h] [ebp-3014h] BYREF
  float pYpos; // [esp+78h] [ebp-3010h] BYREF
  float pXpos; // [esp+7Ch] [ebp-300Ch] BYREF
  DWORD dwAntialiasSave; // [esp+80h] [ebp-3008h] BYREF
  POINT pOut; // [esp+84h] [ebp-3004h] BYREF
  CSceneState pSceneState_v18; // [esp+8Ch] [ebp-2FFCh] BYREF
  float fTextSpeed; // [esp+21Ch] [ebp-2E6Ch]
  HGLOBAL pTmpAnimSet; // [esp+220h] [ebp-2E68h] BYREF
  int v21; // [esp+224h] [ebp-2E64h]
  _DWORD mapIndexTable[49]; // [esp+228h] [ebp-2E60h] BYREF
  _DWORD subStageTable[15]; // [esp+2ECh] [ebp-2D9Ch] BYREF
  String20 szSxxWav[3]; // [esp+328h] [ebp-2D60h] BYREF
  float pfExecTime; // [esp+364h] [ebp-2D24h] BYREF
  CSekBody *pItemBodies[20]; // [esp+368h] [ebp-2D20h] BYREF
  DWORD dwTimeBase; // [esp+3B8h] [ebp-2CD0h]
  int nTimeLimit; // [esp+3BCh] [ebp-2CCCh] BYREF
  float fHeroHp; // [esp+3C0h] [ebp-2CC8h] BYREF
  DWORD arrItemPlaceData[6][25][10]; // [esp+3C4h] [ebp-2CC4h] BYREF
  float fHeroZ; // [esp+1B34h] [ebp-1554h] BYREF
  char szSprNames[30][25]; // [esp+1B38h] [ebp-1550h] BYREF
  CHanaSpriteT3F6_S1FC *pMapFrame; // [esp+1E28h] [ebp-1260h] BYREF
  CSekBody *pSpriteNames[20]; // [esp+1E2Ch] [ebp-125Ch] BYREF
  CHanaSpriteT3F6_S1FC *pSprNumbers_v84[25]; // [esp+1E7Ch] [ebp-120Ch] BYREF
  CHAR pszFileName[256]; // [esp+1EE0h] [ebp-11A8h] BYREF
  float fCamFactor; // [esp+1FE0h] [ebp-10A8h] BYREF
  float fHeroY; // [esp+1FE4h] [ebp-10A4h] BYREF
  char szSfxWavNames[15][50]; // [esp+1FE8h] [ebp-10A0h] BYREF
  __int16 v40; // [esp+22D8h] [ebp-DB0h]
  float fHeroX; // [esp+22DCh] [ebp-DACh] BYREF
  char szItemSekNames[30][25]; // [esp+22E0h] [ebp-DA8h] BYREF
  int i; // [esp+25D0h] [ebp-AB8h]
  LPDIRECTSOUNDBUFFER pDSBuf[10]; // [esp+25D4h] [ebp-AB4h] BYREF
  DWORD Time; // [esp+25FCh] [ebp-A8Ch]
  int bFirstFrameDone; // [esp+2600h] [ebp-A88h]
  CHanaFrameT3EC_S354 *pCamFrame; // [esp+2604h] [ebp-A84h]
  _DWORD timeLimitTable[25]; // [esp+2608h] [ebp-A80h] BYREF
  CHanaSpriteT3F6_S1FC *pSprTimes_v120[30]; // [esp+266Ch] [ebp-A1Ch] BYREF
  float fBlendTime; // [esp+26E4h] [ebp-9A4h] BYREF
  float dirForwardZ; // [esp+26E8h] [ebp-9A0h] BYREF
  int nMenuResult; // [esp+26ECh] [ebp-99Ch]
  char szSceneOdsPaths[30][25]; // [esp+26F0h] [ebp-998h] BYREF
  float dirForwardY; // [esp+29E0h] [ebp-6A8h] BYREF
  int nTransResult; // [esp+29E4h] [ebp-6A4h]
  float fHeroHpMax; // [esp+29E8h] [ebp-6A0h] BYREF
  unsigned int pnCursor; // [esp+29ECh] [ebp-69Ch] BYREF
  float dirForwardX; // [esp+29F0h] [ebp-698h] BYREF
  char szMapOdfPaths[30][25]; // [esp+29F4h] [ebp-694h] BYREF
  CSekBody *pHeroBody; // [esp+2CE4h] [ebp-3A4h] BYREF
  int bDialogueMode; // [esp+2CE8h] [ebp-3A0h]
  BOOL bSkipMode; // [esp+2CECh] [ebp-39Ch]
  float dirUpZ; // [esp+2CF0h] [ebp-398h] BYREF
  CHanaSceneT458_S504 *pScene; // [esp+2CF4h] [ebp-394h] BYREF
  float dirUpY; // [esp+2CF8h] [ebp-390h] BYREF
  float fDeltaTime; // [esp+2CFCh] [ebp-38Ch]
  CHAR String[256]; // [esp+2D00h] [ebp-388h] BYREF
  int pnSubIndex; // [esp+2E00h] [ebp-288h] BYREF
  int yMouse; // [esp+2E04h] [ebp-284h]
  float dirUpX; // [esp+2E08h] [ebp-280h] BYREF
  unsigned int nElapsed; // [esp+2E0Ch] [ebp-27Ch]
  unsigned int nElapsedPrev; // [esp+2E10h] [ebp-278h]
  int xMouse; // [esp+2E14h] [ebp-274h]
  LPDIRECTSOUNDBUFFER pBgmBuffer; // [esp+2E18h] [ebp-270h]
  int pnMapIndex; // [esp+2E1Ch] [ebp-26Ch] BYREF
  CHanaSpriteT3F6_S1FC *ppOut[25]; // [esp+2E20h] [ebp-268h] BYREF
  _BYTE Src[400]; // [esp+2E84h] [ebp-204h] BYREF
  CSekBody *pSekBody; // [esp+3014h] [ebp-74h] BYREF
  CHanaSpriteT3F6_S1FC *pSprModes_v221[25]; // [esp+3018h] [ebp-70h] BYREF
  BYTE updated; // [esp+307Ch] [ebp-Ch]
  WORD wBtnMouse; // [esp+3080h] [ebp-8h] BYREF
  int nTargetSprite; // [esp+3084h] [ebp-4h]

  ONCELOG("[HNH:SUB] hook_PlayExploring_V2_45BCAE\n");

  nMenuResult = -1;
  szMapOdfPaths[0][0] = 0;
  memset(&szMapOdfPaths[0][1], 0, 24u);
  strcpy(szMapOdfPaths[1], "Map\\m01\\at01.ODF");
  *(_QWORD *)&szMapOdfPaths[1][17] = 0;
  strcpy(szMapOdfPaths[2], "Map\\m02\\at02.ODF");
  *(_QWORD *)&szMapOdfPaths[2][17] = 0;
  strcpy(szMapOdfPaths[3], "Map\\m03\\at03.ODF");
  *(_QWORD *)&szMapOdfPaths[3][17] = 0;
  strcpy(szMapOdfPaths[4], "Map\\m04\\at04.ODF");
  *(_QWORD *)&szMapOdfPaths[4][17] = 0;
  strcpy(szMapOdfPaths[5], "Map\\m05\\at05.ODF");
  *(_QWORD *)&szMapOdfPaths[5][17] = 0;
  strcpy(szMapOdfPaths[6], "Map\\m06\\at06.ODF");
  *(_QWORD *)&szMapOdfPaths[6][17] = 0;
  szMapOdfPaths[7][0] = 0;
  memset(&szMapOdfPaths[7][1], 0, 0x18u);
  strcpy(szMapOdfPaths[8], "Map\\m08\\at08.ODF");
  *(_QWORD *)&szMapOdfPaths[8][17] = 0;
  strcpy(szMapOdfPaths[9], "Map\\m09\\at09.ODF");
  *(_QWORD *)&szMapOdfPaths[9][17] = 0;
  strcpy(szMapOdfPaths[10], "Map\\m10\\at10.ODF");
  *(_QWORD *)&szMapOdfPaths[10][17] = 0;
  strcpy(szMapOdfPaths[11], "Map\\m11\\at11.ODF");
  *(_QWORD *)&szMapOdfPaths[11][17] = 0;
  strcpy(szMapOdfPaths[12], "Map\\m12\\at12.ODF");
  *(_QWORD *)&szMapOdfPaths[12][17] = 0;
  strcpy(szMapOdfPaths[13], "Map\\m13\\at13.ODF");
  *(_QWORD *)&szMapOdfPaths[13][17] = 0;
  strcpy(szMapOdfPaths[14], "Map\\m14\\at14.ODF");
  *(_QWORD *)&szMapOdfPaths[14][17] = 0;
  strcpy(szMapOdfPaths[15], "Map\\m15\\at15.ODF");
  *(_QWORD *)&szMapOdfPaths[15][17] = 0;
  strcpy(szMapOdfPaths[16], "Map\\m16\\at16.ODF");
  *(_QWORD *)&szMapOdfPaths[16][17] = 0;
  strcpy(szMapOdfPaths[17], "Map\\m17\\at17.ODF");
  *(_QWORD *)&szMapOdfPaths[17][17] = 0;
  strcpy(szMapOdfPaths[18], "Map\\m18\\at18.ODF");
  *(_QWORD *)&szMapOdfPaths[18][17] = 0;
  strcpy(szMapOdfPaths[19], "Map\\m19\\at19.ODF");
  *(_QWORD *)&szMapOdfPaths[19][17] = 0;
  strcpy(szMapOdfPaths[20], "Map\\m20\\at20.ODF");
  *(_QWORD *)&szMapOdfPaths[20][17] = 0;
  strcpy(szMapOdfPaths[21], "Map\\m21\\at21.ODF");
  *(_QWORD *)&szMapOdfPaths[21][17] = 0;
  strcpy(szMapOdfPaths[22], "Map\\m22\\at22.ODF");
  *(_QWORD *)&szMapOdfPaths[22][17] = 0;
  strcpy(szMapOdfPaths[23], "Map\\m23\\at23.ODF");
  *(_QWORD *)&szMapOdfPaths[23][17] = 0;
  memset(szMapOdfPaths[24], 0, 150);
  szSceneOdsPaths[0][0] = 0;
  memset(&szSceneOdsPaths[0][1], 0, 24u);
  strcpy(szSceneOdsPaths[1], "sch\\map01.ods");
  *(_QWORD *)&szSceneOdsPaths[1][14] = 0;
  *(_WORD *)&szSceneOdsPaths[1][22] = 0;
  szSceneOdsPaths[1][24] = 0;
  strcpy(szSceneOdsPaths[2], "sch\\map02.ods");
  *(_QWORD *)&szSceneOdsPaths[2][14] = 0;
  *(_WORD *)&szSceneOdsPaths[2][22] = 0;
  szSceneOdsPaths[2][24] = 0;
  strcpy(szSceneOdsPaths[3], "sch\\map03.ods");
  *(_QWORD *)&szSceneOdsPaths[3][14] = 0;
  *(_WORD *)&szSceneOdsPaths[3][22] = 0;
  szSceneOdsPaths[3][24] = 0;
  strcpy(szSceneOdsPaths[4], "sch\\map04.ods");
  *(_QWORD *)&szSceneOdsPaths[4][14] = 0;
  *(_WORD *)&szSceneOdsPaths[4][22] = 0;
  szSceneOdsPaths[4][24] = 0;
  strcpy(szSceneOdsPaths[5], "sch\\map05.ods");
  *(_QWORD *)&szSceneOdsPaths[5][14] = 0;
  *(_WORD *)&szSceneOdsPaths[5][22] = 0;
  szSceneOdsPaths[5][24] = 0;
  strcpy(szSceneOdsPaths[6], "sch\\map06.ods");
  *(_QWORD *)&szSceneOdsPaths[6][14] = 0;
  *(_WORD *)&szSceneOdsPaths[6][22] = 0;
  szSceneOdsPaths[6][24] = 0;
  szSceneOdsPaths[7][0] = 0;
  memset(&szSceneOdsPaths[7][1], 0, 0x18u);
  strcpy(szSceneOdsPaths[8], "sch\\map08.ods");
  *(_QWORD *)&szSceneOdsPaths[8][14] = 0;
  *(_WORD *)&szSceneOdsPaths[8][22] = 0;
  szSceneOdsPaths[8][24] = 0;
  strcpy(szSceneOdsPaths[9], "sch\\map09.ods");
  *(_QWORD *)&szSceneOdsPaths[9][14] = 0;
  *(_WORD *)&szSceneOdsPaths[9][22] = 0;
  szSceneOdsPaths[9][24] = 0;
  strcpy(szSceneOdsPaths[10], "sch\\map10.ods");
  *(_QWORD *)&szSceneOdsPaths[10][14] = 0;
  *(_WORD *)&szSceneOdsPaths[10][22] = 0;
  szSceneOdsPaths[10][24] = 0;
  strcpy(szSceneOdsPaths[11], "sch\\map11.ods");
  *(_QWORD *)&szSceneOdsPaths[11][14] = 0;
  *(_WORD *)&szSceneOdsPaths[11][22] = 0;
  szSceneOdsPaths[11][24] = 0;
  strcpy(szSceneOdsPaths[12], "sch\\map12.ods");
  *(_QWORD *)&szSceneOdsPaths[12][14] = 0;
  *(_WORD *)&szSceneOdsPaths[12][22] = 0;
  szSceneOdsPaths[12][24] = 0;
  strcpy(szSceneOdsPaths[13], "sch\\map13.ods");
  *(_QWORD *)&szSceneOdsPaths[13][14] = 0;
  *(_WORD *)&szSceneOdsPaths[13][22] = 0;
  szSceneOdsPaths[13][24] = 0;
  strcpy(szSceneOdsPaths[14], "sch\\map14.ods");
  *(_QWORD *)&szSceneOdsPaths[14][14] = 0;
  *(_WORD *)&szSceneOdsPaths[14][22] = 0;
  szSceneOdsPaths[14][24] = 0;
  strcpy(szSceneOdsPaths[15], "sch\\map15.ods");
  *(_QWORD *)&szSceneOdsPaths[15][14] = 0;
  *(_WORD *)&szSceneOdsPaths[15][22] = 0;
  szSceneOdsPaths[15][24] = 0;
  strcpy(szSceneOdsPaths[16], "sch\\map16.ods");
  *(_QWORD *)&szSceneOdsPaths[16][14] = 0;
  *(_WORD *)&szSceneOdsPaths[16][22] = 0;
  szSceneOdsPaths[16][24] = 0;
  strcpy(szSceneOdsPaths[17], "sch\\map17.ods");
  *(_QWORD *)&szSceneOdsPaths[17][14] = 0;
  *(_WORD *)&szSceneOdsPaths[17][22] = 0;
  szSceneOdsPaths[17][24] = 0;
  strcpy(szSceneOdsPaths[18], "sch\\map18.ods");
  *(_QWORD *)&szSceneOdsPaths[18][14] = 0;
  *(_WORD *)&szSceneOdsPaths[18][22] = 0;
  szSceneOdsPaths[18][24] = 0;
  strcpy(szSceneOdsPaths[19], "sch\\map19.ods");
  *(_QWORD *)&szSceneOdsPaths[19][14] = 0;
  *(_WORD *)&szSceneOdsPaths[19][22] = 0;
  szSceneOdsPaths[19][24] = 0;
  strcpy(szSceneOdsPaths[20], "sch\\map20.ods");
  *(_QWORD *)&szSceneOdsPaths[20][14] = 0;
  *(_WORD *)&szSceneOdsPaths[20][22] = 0;
  szSceneOdsPaths[20][24] = 0;
  strcpy(szSceneOdsPaths[21], "sch\\map21.ods");
  *(_QWORD *)&szSceneOdsPaths[21][14] = 0;
  *(_WORD *)&szSceneOdsPaths[21][22] = 0;
  szSceneOdsPaths[21][24] = 0;
  strcpy(szSceneOdsPaths[22], "sch\\map22.ods");
  *(_QWORD *)&szSceneOdsPaths[22][14] = 0;
  *(_WORD *)&szSceneOdsPaths[22][22] = 0;
  szSceneOdsPaths[22][24] = 0;
  strcpy(szSceneOdsPaths[23], "sch\\map23.ods");
  *(_QWORD *)&szSceneOdsPaths[23][14] = 0;
  *(_WORD *)&szSceneOdsPaths[23][22] = 0;
  szSceneOdsPaths[23][24] = 0;
  memset(szSceneOdsPaths[24], 0, 150);
  szItemSekNames[0][0] = 0;
  memset(&szItemSekNames[0][1], 0, 24u);
  strcpy(szItemSekNames[1], "item\\i11");
  *(_QWORD *)&szItemSekNames[1][9] = 0;
  *(_QWORD *)&szItemSekNames[1][17] = 0;
  strcpy(szItemSekNames[2], "item\\i12");
  *(_QWORD *)&szItemSekNames[2][9] = 0;
  *(_QWORD *)&szItemSekNames[2][17] = 0;
  strcpy(szItemSekNames[3], "item\\i13");
  *(_QWORD *)&szItemSekNames[3][9] = 0;
  *(_QWORD *)&szItemSekNames[3][17] = 0;
  strcpy(szItemSekNames[4], "item\\i14");
  *(_QWORD *)&szItemSekNames[4][9] = 0;
  *(_QWORD *)&szItemSekNames[4][17] = 0;
  strcpy(szItemSekNames[5], "item\\i15");
  *(_QWORD *)&szItemSekNames[5][9] = 0;
  *(_QWORD *)&szItemSekNames[5][17] = 0;
  strcpy(szItemSekNames[6], "item\\i16");
  *(_QWORD *)&szItemSekNames[6][9] = 0;
  *(_QWORD *)&szItemSekNames[6][17] = 0;
  strcpy(szItemSekNames[7], "item\\i17");
  *(_QWORD *)&szItemSekNames[7][9] = 0;
  *(_QWORD *)&szItemSekNames[7][17] = 0;
  memset(szItemSekNames[8], 0, 550);
  szSprNames[0][0] = 0;
  memset(&szSprNames[0][1], 0, 24u);
  strcpy(szSprNames[1], "sch\\m01");
  *(_QWORD *)&szSprNames[1][8] = 0;
  *(_QWORD *)&szSprNames[1][16] = 0;
  szSprNames[1][24] = 0;
  strcpy(szSprNames[2], "sch\\m05");
  *(_QWORD *)&szSprNames[2][8] = 0;
  *(_QWORD *)&szSprNames[2][16] = 0;
  szSprNames[2][24] = 0;
  strcpy(szSprNames[3], "sch\\m03");
  *(_QWORD *)&szSprNames[3][8] = 0;
  *(_QWORD *)&szSprNames[3][16] = 0;
  szSprNames[3][24] = 0;
  strcpy(szSprNames[4], "sch\\m06");
  *(_QWORD *)&szSprNames[4][8] = 0;
  *(_QWORD *)&szSprNames[4][16] = 0;
  szSprNames[4][24] = 0;
  strcpy(szSprNames[5], "sch\\m02");
  *(_QWORD *)&szSprNames[5][8] = 0;
  *(_QWORD *)&szSprNames[5][16] = 0;
  szSprNames[5][24] = 0;
  strcpy(szSprNames[6], "sch\\m04");
  *(_QWORD *)&szSprNames[6][8] = 0;
  *(_QWORD *)&szSprNames[6][16] = 0;
  szSprNames[6][24] = 0;
  strcpy(szSprNames[7], "sch\\m07");
  *(_QWORD *)&szSprNames[7][8] = 0;
  *(_QWORD *)&szSprNames[7][16] = 0;
  szSprNames[7][24] = 0;
  szSprNames[8][0] = 0;
  memset(&szSprNames[8][1], 0, 0x18u);
  szSprNames[9][0] = 0;
  memset(&szSprNames[9][1], 0, 0x18u);
  szSprNames[10][0] = 0;
  memset(&szSprNames[10][1], 0, 0x18u);
  memset(szSprNames[11], 0, 475);
  memset(arrItemPlaceData, 0, 40);
  *(_QWORD *)&arrItemPlaceData[0][1][0] = 0x100000001LL;
  *(_QWORD *)&arrItemPlaceData[0][1][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][1][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][1][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][1][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][2][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][2][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][2][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][2][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][2][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][3][0] = 0x200000000LL;
  *(_QWORD *)&arrItemPlaceData[0][3][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][3][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][3][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][3][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][4][0] = 1;
  *(_QWORD *)&arrItemPlaceData[0][4][2] = 3;
  *(_QWORD *)&arrItemPlaceData[0][4][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][4][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][4][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][5][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][5][2] = 4;
  *(_QWORD *)&arrItemPlaceData[0][5][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][5][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][5][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][6][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][6][2] = 5;
  *(_QWORD *)&arrItemPlaceData[0][6][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][6][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][6][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][7][0] = 0;
  *(_QWORD *)&arrItemPlaceData[0][7][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][7][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][7][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][7][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][8][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][8][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][8][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][8][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][8][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][9][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][9][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][9][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][9][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][9][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][10][0] = 3;
  *(_QWORD *)&arrItemPlaceData[0][10][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][10][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][10][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][10][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][11][0] = 1;
  *(_QWORD *)&arrItemPlaceData[0][11][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][11][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][11][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][11][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][12][0] = 1;
  *(_QWORD *)&arrItemPlaceData[0][12][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][12][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][12][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][12][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][13][0] = 3;
  *(_QWORD *)&arrItemPlaceData[0][13][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][13][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][13][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][13][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][14][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[0][14][2] = 1;
  *(_QWORD *)&arrItemPlaceData[0][14][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][14][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][14][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][15][0] = 3;
  *(_QWORD *)&arrItemPlaceData[0][15][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][15][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][15][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][15][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][16][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[0][16][2] = 0x400000003LL;
  *(_QWORD *)&arrItemPlaceData[0][16][4] = 0x600000005LL;
  *(_QWORD *)&arrItemPlaceData[0][16][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][16][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][17][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[0][17][2] = 1;
  *(_QWORD *)&arrItemPlaceData[0][17][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][17][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][17][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][18][0] = 3;
  *(_QWORD *)&arrItemPlaceData[0][18][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][18][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][18][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][18][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][19][0] = 2;
  *(_QWORD *)&arrItemPlaceData[0][19][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][19][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][19][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][19][8] = 0;
  *(_QWORD *)&arrItemPlaceData[0][20][0] = 3;
  *(_QWORD *)&arrItemPlaceData[0][20][2] = 0;
  *(_QWORD *)&arrItemPlaceData[0][20][4] = 0;
  *(_QWORD *)&arrItemPlaceData[0][20][6] = 0;
  *(_QWORD *)&arrItemPlaceData[0][20][8] = 0;
  memset(arrItemPlaceData[0][21], 0, 0xA0u);
  memset(arrItemPlaceData[1], 0, 40);
  *(_QWORD *)&arrItemPlaceData[1][1][0] = 0x100000003LL;
  *(_QWORD *)&arrItemPlaceData[1][1][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][1][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][1][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][1][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][2][0] = 0x200000000LL;
  *(_QWORD *)&arrItemPlaceData[1][2][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][2][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][2][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][2][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][3][0] = 1;
  *(_QWORD *)&arrItemPlaceData[1][3][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][3][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][3][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][3][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][4][0] = 0x100000000LL;
  *(_QWORD *)&arrItemPlaceData[1][4][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][4][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][4][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][4][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][5][0] = 0;
  *(_QWORD *)&arrItemPlaceData[1][5][2] = 4;
  *(_QWORD *)&arrItemPlaceData[1][5][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][5][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][5][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][6][0] = 0;
  *(_QWORD *)&arrItemPlaceData[1][6][2] = 5;
  *(_QWORD *)&arrItemPlaceData[1][6][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][6][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][6][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][7][0] = 0;
  *(_QWORD *)&arrItemPlaceData[1][7][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][7][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][7][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][7][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][8][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[1][8][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][8][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][8][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][8][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][9][0] = 0x200000001LL;
  *(_QWORD *)&arrItemPlaceData[1][9][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][9][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][9][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][9][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][10][0] = 0;
  *(_QWORD *)&arrItemPlaceData[1][10][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][10][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][10][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][10][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][11][0] = 3;
  *(_QWORD *)&arrItemPlaceData[1][11][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][11][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][11][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][11][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][12][0] = 0;
  *(_QWORD *)&arrItemPlaceData[1][12][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][12][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][12][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][12][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][13][0] = 2;
  *(_QWORD *)&arrItemPlaceData[1][13][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][13][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][13][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][13][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][14][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[1][14][2] = 1;
  *(_QWORD *)&arrItemPlaceData[1][14][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][14][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][14][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][15][0] = 1;
  *(_QWORD *)&arrItemPlaceData[1][15][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][15][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][15][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][15][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][16][0] = 1;
  *(_QWORD *)&arrItemPlaceData[1][16][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][16][4] = 0x600000000LL;
  *(_QWORD *)&arrItemPlaceData[1][16][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][16][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][17][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[1][17][2] = 1;
  *(_QWORD *)&arrItemPlaceData[1][17][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][17][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][17][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][18][0] = 3;
  *(_QWORD *)&arrItemPlaceData[1][18][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][18][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][18][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][18][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][19][0] = 4;
  *(_QWORD *)&arrItemPlaceData[1][19][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][19][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][19][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][19][8] = 0;
  *(_QWORD *)&arrItemPlaceData[1][20][0] = 3;
  *(_QWORD *)&arrItemPlaceData[1][20][2] = 0;
  *(_QWORD *)&arrItemPlaceData[1][20][4] = 0;
  *(_QWORD *)&arrItemPlaceData[1][20][6] = 0;
  *(_QWORD *)&arrItemPlaceData[1][20][8] = 0;
  memset(arrItemPlaceData[1][21], 0, 0xA0u);
  memset(arrItemPlaceData[2], 0, 40);
  *(_QWORD *)&arrItemPlaceData[2][1][0] = 5;
  *(_QWORD *)&arrItemPlaceData[2][1][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][1][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][1][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][1][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][2][0] = 4;
  *(_QWORD *)&arrItemPlaceData[2][2][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][2][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][2][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][2][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][3][0] = 0x600000000LL;
  *(_QWORD *)&arrItemPlaceData[2][3][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][3][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][3][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][3][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][4][0] = 0x400000004LL;
  *(_QWORD *)&arrItemPlaceData[2][4][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][4][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][4][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][4][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][5][0] = 0x200000000LL;
  *(_QWORD *)&arrItemPlaceData[2][5][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][5][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][5][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][5][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][6][0] = 0x500000001LL;
  *(_QWORD *)&arrItemPlaceData[2][6][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][6][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][6][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][6][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][7][0] = 0;
  *(_QWORD *)&arrItemPlaceData[2][7][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][7][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][7][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][7][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][8][0] = 1;
  *(_QWORD *)&arrItemPlaceData[2][8][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][8][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][8][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][8][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][9][0] = 0x300000000LL;
  *(_QWORD *)&arrItemPlaceData[2][9][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][9][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][9][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][9][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][10][0] = 3;
  *(_QWORD *)&arrItemPlaceData[2][10][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][10][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][10][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][10][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][11][0] = 0;
  *(_QWORD *)&arrItemPlaceData[2][11][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][11][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][11][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][11][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][12][0] = 0;
  *(_QWORD *)&arrItemPlaceData[2][12][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][12][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][12][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][12][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][13][0] = 3;
  *(_QWORD *)&arrItemPlaceData[2][13][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][13][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][13][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][13][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][14][0] = 0x400000000LL;
  *(_QWORD *)&arrItemPlaceData[2][14][2] = 1;
  *(_QWORD *)&arrItemPlaceData[2][14][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][14][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][14][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][15][0] = 5;
  *(_QWORD *)&arrItemPlaceData[2][15][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][15][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][15][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][15][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][16][0] = 0x200000000LL;
  *(_QWORD *)&arrItemPlaceData[2][16][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][16][4] = 5;
  *(_QWORD *)&arrItemPlaceData[2][16][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][16][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][17][0] = 0;
  *(_QWORD *)&arrItemPlaceData[2][17][2] = 1;
  *(_QWORD *)&arrItemPlaceData[2][17][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][17][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][17][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][18][0] = 4;
  *(_QWORD *)&arrItemPlaceData[2][18][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][18][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][18][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][18][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][19][0] = 3;
  *(_QWORD *)&arrItemPlaceData[2][19][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][19][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][19][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][19][8] = 0;
  *(_QWORD *)&arrItemPlaceData[2][20][0] = 1;
  *(_QWORD *)&arrItemPlaceData[2][20][2] = 0;
  *(_QWORD *)&arrItemPlaceData[2][20][4] = 0;
  *(_QWORD *)&arrItemPlaceData[2][20][6] = 0;
  *(_QWORD *)&arrItemPlaceData[2][20][8] = 0;
  memset(arrItemPlaceData[2][21], 0, 0xA0u);
  memset(arrItemPlaceData[3], 0, 40);
  *(_QWORD *)&arrItemPlaceData[3][1][0] = 0x100000000LL;
  *(_QWORD *)&arrItemPlaceData[3][1][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][1][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][1][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][1][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][2][0] = 0x600000001LL;
  *(_QWORD *)&arrItemPlaceData[3][2][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][2][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][2][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][2][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][3][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[3][3][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][3][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][3][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][3][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][4][0] = 0x600000005LL;
  *(_QWORD *)&arrItemPlaceData[3][4][2] = 3;
  *(_QWORD *)&arrItemPlaceData[3][4][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][4][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][4][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][5][0] = 0x500000003LL;
  *(_QWORD *)&arrItemPlaceData[3][5][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][5][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][5][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][5][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][6][0] = 0x200000004LL;
  *(_QWORD *)&arrItemPlaceData[3][6][2] = 5;
  *(_QWORD *)&arrItemPlaceData[3][6][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][6][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][6][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][7][0] = 0;
  *(_QWORD *)&arrItemPlaceData[3][7][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][7][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][7][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][7][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][8][0] = 0x200000003LL;
  *(_QWORD *)&arrItemPlaceData[3][8][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][8][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][8][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][8][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][9][0] = 1;
  *(_QWORD *)&arrItemPlaceData[3][9][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][9][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][9][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][9][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][10][0] = 3;
  *(_QWORD *)&arrItemPlaceData[3][10][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][10][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][10][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][10][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][11][0] = 4;
  *(_QWORD *)&arrItemPlaceData[3][11][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][11][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][11][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][11][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][12][0] = 3;
  *(_QWORD *)&arrItemPlaceData[3][12][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][12][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][12][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][12][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][13][0] = 4;
  *(_QWORD *)&arrItemPlaceData[3][13][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][13][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][13][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][13][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][14][0] = 0x200000005LL;
  *(_QWORD *)&arrItemPlaceData[3][14][2] = 4;
  *(_QWORD *)&arrItemPlaceData[3][14][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][14][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][14][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][15][0] = 4;
  *(_QWORD *)&arrItemPlaceData[3][15][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][15][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][15][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][15][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][16][0] = 1;
  *(_QWORD *)&arrItemPlaceData[3][16][2] = 0x400000003LL;
  *(_QWORD *)&arrItemPlaceData[3][16][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][16][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][16][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][17][0] = 0x200000002LL;
  *(_QWORD *)&arrItemPlaceData[3][17][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][17][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][17][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][17][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][18][0] = 5;
  *(_QWORD *)&arrItemPlaceData[3][18][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][18][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][18][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][18][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][19][0] = 5;
  *(_QWORD *)&arrItemPlaceData[3][19][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][19][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][19][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][19][8] = 0;
  *(_QWORD *)&arrItemPlaceData[3][20][0] = 5;
  *(_QWORD *)&arrItemPlaceData[3][20][2] = 0;
  *(_QWORD *)&arrItemPlaceData[3][20][4] = 0;
  *(_QWORD *)&arrItemPlaceData[3][20][6] = 0;
  *(_QWORD *)&arrItemPlaceData[3][20][8] = 0;
  memset(arrItemPlaceData[3][21], 0, 0xA0u);
  memset(arrItemPlaceData[4], 0, 0x7D0u);
  strcpy(szSfxWavNames[0], "Start");
  memset(&szSfxWavNames[0][6], 0, 44u);
  strcpy(szSfxWavNames[1], "GameOver");
  memset(&szSfxWavNames[1][9], 0, 0x28u);
  szSfxWavNames[1][49] = 0;
  strcpy(szSfxWavNames[2], "On");
  memset(&szSfxWavNames[2][3], 0, 0x2Cu);
  *(_WORD *)&szSfxWavNames[2][47] = 0;
  szSfxWavNames[2][49] = 0;
  strcpy(szSfxWavNames[3], "On2");
  memset(&szSfxWavNames[3][4], 0, 0x2Cu);
  *(_WORD *)&szSfxWavNames[3][48] = 0;
  szSfxWavNames[4][0] = 0;
  memset(&szSfxWavNames[4][1], 0, 0x30u);
  szSfxWavNames[4][49] = 0;
  szSfxWavNames[5][0] = 0;
  memset(&szSfxWavNames[5][1], 0, 0x30u);
  szSfxWavNames[5][49] = 0;
  szSfxWavNames[6][0] = 0;
  memset(&szSfxWavNames[6][1], 0, 0x30u);
  szSfxWavNames[6][49] = 0;
  szSfxWavNames[7][0] = 0;
  memset(&szSfxWavNames[7][1], 0, 0x30u);
  szSfxWavNames[7][49] = 0;
  szSfxWavNames[8][0] = 0;
  memset(&szSfxWavNames[8][1], 0, 0x30u);
  szSfxWavNames[8][49] = 0;
  szSfxWavNames[9][0] = 0;
  memset(&szSfxWavNames[9][1], 0, 0x30u);
  szSfxWavNames[9][49] = 0;
  strcpy(szSfxWavNames[10], "foot4");
  memset(&szSfxWavNames[10][6], 0, 44u);
  strcpy(szSfxWavNames[11], "foot2");
  memset(&szSfxWavNames[11][6], 0, 0x2Cu);
  strcpy(szSfxWavNames[12], "foot3");
  memset(&szSfxWavNames[12][6], 0, 0x2Cu);
  strcpy(szSfxWavNames[13], "foot5");
  memset(&szSfxWavNames[13][6], 0, 0x2Cu);
  memset(szSfxWavNames[14], 0, sizeof(char[50]));
  pBgmBuffer = 0;
  pfExecTime = 0.0;
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
  fCamFactor = 1.0;
  fBlendTime = 0.0;
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
  v21 = 22;
  mapIndexTable[0] = 2;
  mapIndexTable[1] = 22;
  mapIndexTable[2] = 3;
  mapIndexTable[3] = 22;
  mapIndexTable[4] = 1;
  mapIndexTable[5] = 22;
  mapIndexTable[6] = 0;
  mapIndexTable[7] = 22;
  mapIndexTable[8] = 0;
  mapIndexTable[9] = 2;
  mapIndexTable[10] = 0;
  mapIndexTable[11] = 23;
  mapIndexTable[12] = 2;
  mapIndexTable[13] = 8;
  mapIndexTable[14] = 0;
  mapIndexTable[15] = 16;
  mapIndexTable[16] = 0;
  mapIndexTable[17] = 16;
  mapIndexTable[18] = 1;
  mapIndexTable[19] = 5;
  mapIndexTable[20] = 1;
  memset(&mapIndexTable[21], 0, 0x70u);
  subStageTable[0] = 0;
  subStageTable[1] = 0;
  subStageTable[2] = 1;
  subStageTable[3] = 1;
  subStageTable[4] = 2;
  subStageTable[5] = 2;
  subStageTable[6] = 2;
  subStageTable[7] = 3;
  subStageTable[8] = 3;
  subStageTable[9] = 3;
  subStageTable[10] = 3;
  memset(&subStageTable[11], 0, 16);
  timeLimitTable[0] = 240;
  timeLimitTable[1] = 360;
  timeLimitTable[2] = 140;
  timeLimitTable[3] = 160;
  timeLimitTable[4] = 180;
  timeLimitTable[5] = 190;
  timeLimitTable[6] = 200;
  timeLimitTable[7] = 60;
  timeLimitTable[8] = 170;
  timeLimitTable[9] = 180;
  timeLimitTable[10] = 190;
  memset(&timeLimitTable[11], 0, 0x38u);
  memcpy(Src, &g_sceneState_52F7E0, sizeof(Src));
  memset(pSprNumbers_v84, 0, 0x3Cu);
  memset(pSprTimes_v120, 0, 0x50u);
  memset(pSprModes_v221, 0, 0x3Cu);
  memset(pItemBodies, 0, sizeof(pItemBodies));
  memset(pSpriteNames, 0, sizeof(pSpriteNames));
  memset(ppOut, 0, 0x28u);
  memcpy(&g_sceneState_52F7E0, Src, sizeof(g_sceneState_52F7E0));
  pnMapIndex = mapIndexTable[2 * nStageIndex - 1];
  pnSubIndex = mapIndexTable[2 * nStageIndex];
  fHeroHp = g_sceneState_52F7E0.protCurrentHP_C0;
  fHeroHpMax = g_sceneState_52F7E0.protMaximumHP_C4;
  g_sceneState_52F7E0.bCurrentSubStage = subStageTable[nStageIndex];
  g_sceneState_52F7E0.bMaxSubStage = subStageTable[nStageIndex] + 1;
  nTimeLimit = timeLimitTable[nStageIndex];
  SetProjectionFov_40DCA3(1.2);
  SetTimerScaleFactor(14.0);
  memcpy(Src, &g_sceneState_52F7E0, sizeof(Src));
  memset(pDSBuf, 0, sizeof(pDSBuf));
  for ( i = 0; i < 10; ++i )
  {
    wsprintfA(pszFileName, "%s.wav", szSfxWavNames[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1009);// get g_szSEPath
    Wav_422E7B = LoadWav_422E7B(pszFileName);
    pDSBuf[i] = Wav_422E7B;
  }
  strcpy((char *)szSxxWav[0].str, "s007.wav");
  *(_DWORD *)&szSxxWav[0].str[9] = 0;
  *(_DWORD *)&szSxxWav[0].str[13] = 0;
  *(_WORD *)&szSxxWav[0].str[17] = 0;
  szSxxWav[0].str[19] = 0;
  strcpy((char *)szSxxWav[1].str, "s008.wav");
  *(_DWORD *)&szSxxWav[1].str[9] = 0;
  *(_DWORD *)&szSxxWav[1].str[13] = 0;
  *(_WORD *)&szSxxWav[1].str[17] = 0;
  szSxxWav[1].str[19] = 0;
  strcpy((char *)szSxxWav[2].str, "s009.wav");
  *(_DWORD *)&szSxxWav[2].str[9] = 0;
  *(_DWORD *)&szSxxWav[2].str[13] = 0;
  *(_WORD *)&szSxxWav[2].str[17] = 0;
  szSxxWav[2].str[19] = 0;
  if ( !nStageIndex || nStageIndex == 2 || nStageIndex == 4 || nStageIndex == 6 || nStageIndex == 9 )
  {
    i = 0;
  }
  else if ( nStageIndex == 10 )
  {
    i = 2;
  }
  else
  {
    i = 1;
  }
  wsprintfA(pszFileName, "%s", szSxxWav[i].str);
  CHanaObj__GetRealPath_486D9A(pszFileName, 1006);// get g_szWAVPath
  pBgmBuffer = LoadWav_422E7B(pszFileName);
  if ( pBgmBuffer )
  {
    if ( (int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) >= -10000 )
    {
      if ( (int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0) <= 0 )
        pBgmBuffer->SetVolume(
          (__int64)(g_fVolumeDbTable[(unsigned __int8)g_nAviVolumeIndex_530266] - 1000.0));
      else
        pBgmBuffer->SetVolume(0);
    }
    else
    {
      pBgmBuffer->SetVolume(-10000);
    }
  }
  InitHudResources_4490A7(pSprNumbers_v84, pSprTimes_v120, pSprModes_v221, 1);
  for ( i = 0; i < 10; ++i )
  {
    if ( szSprNames[i][0] )
    {
      wsprintfA(pszFileName, "%s.bmp", szSprNames[i]);
      CHanaObj__GetRealPath_486D9A(pszFileName, 1003);// get g_szSPRPath
      wsprintfA(String, "%sa.bmp", szSprNames[i]);
      CHanaObj__GetRealPath_486D9A(String, 1003);// get g_szSPRPath
      LoadBmpWithMask_413A5E(&ppOut[i], pszFileName, String);
    }
  }
  wsprintfA(pszFileName, "otoko\\otoko.sek");
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);// get g_szODFPath
  wsprintfA(String, "otoko\\otoko.odf");
  CHanaObj__GetRealPath_486D9A(String, 1005);   // get g_szODFPath
  if ( !LoadSekAndOdf_4445DD(pszFileName, String, &pHeroBody) )
    return -1;
  pCamFrame = CHanaFrameT3EC__FindByName_483650(pHeroBody->pSekHeader_220->pFrameT3EC, "Frame5_o00_02_head");
  wsprintfA(pszFileName, "lockon\\lock.sek");
  CHanaObj__GetRealPath_486D9A(pszFileName, 1005);// get g_szODFPath
  wsprintfA(String, "lockon\\lock.odf");
  CHanaObj__GetRealPath_486D9A(String, 1005);   // get g_szODFPath
  if ( !LoadSekAndOdf_4445DD(pszFileName, String, &pSekBody) )
    return -1;
  CSekBody__CutToAnim_4018E1(pSekBody, 0);
  for ( i = 1; i <= 7; ++i )
  {
    wsprintfA(pszFileName, "%s.sek", szItemSekNames[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);// get g_szODFPath
    wsprintfA(String, "%s.odf", szItemSekNames[i]);
    CHanaObj__GetRealPath_486D9A(String, 1005); // get g_szODFPath
    if ( !LoadSekAndOdf_4445DD(pszFileName, String, &pItemBodies[i]) )
      return -1;
    CSekBody__CutToAnim_4018E1(pItemBodies[i], 0);
  }
  if ( pBgmBuffer )
    DSBufferPlay(pBgmBuffer, 1);
  dwTimeBase = timeGetTime();
  Time = timeGetTime();
  while ( 2 )
  {
    for ( i = 0; i < 10; ++i )
    {
      if ( ppOut[i] )
        ppOut[i]->fAlpha_144 = 0.0;
    }
    pTmpAnimSet = 0;
    if ( pMapFrame )
    {
      CHanaObj__Free_40C1E4(pMapFrame);
      pMapFrame = 0;
    }
    wsprintfA(pszFileName, szMapOdfPaths[pnMapIndex]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);
    if ( !ParseODFFile_441F19(pszFileName, (CHanaFrameT3EC_S354 **)&pMapFrame, (CHanaAnimSetT3F1_S150 **)&pTmpAnimSet) )
      return -1;
    if ( pTmpAnimSet )
    {
      CHanaObj__Free_40C1E4((CHanaBase_S130 *)pTmpAnimSet);
      pTmpAnimSet = 0;
    }
    AttachChild_4055FD(g_pSceneFrame, (CHanaFrameT3EC_S354 *)pMapFrame);
    MtxMult_4069DD((CHanaFrameT3EC_S354 *)pMapFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0, 0.0, 0.0);
    UpdateBoneLookAtTransform(
      (CHanaFrameT3EC_S354 *)pMapFrame,
      (CHanaFrameT3EC_S354 *)g_pSceneFrame,
      0.0,
      0.0,
      1.0,
      0.0,
      1.0,
      0.0);
    BuildMapSubFrames_443DD5((CHanaFrameT3EC_S354 *)pMapFrame);
    if ( pScene )
    {
      CHanaObj__Free_40C1E4(pScene);
      pScene = 0;
    }
    wsprintfA(pszFileName, szSceneOdsPaths[pnMapIndex]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1002);
    if ( !LoadOds_4843CF(pszFileName, &pScene) )
      return -1;
    UpdateCameraLinkedObjects_44451F(g_pCameraFrame);
    SetupHeroEntry_4593E0(pScene, pHeroBody->pSekHeader_220->pFrameT3EC, pnSubIndex);
    SetupItemBodies_459F81(pScene, pItemBodies, arrItemPlaceData[subStageTable[nStageIndex]][pnMapIndex], pnMapIndex);
    StopAndRestartThread(1u);
    bFirstFrameDone = 0;
    MtxMult_4069DD(g_pCameraFrame, pHeroBody->pSekHeader_220->pFrameT3EC, 0.0, 25.0, -1.0);
    UpdateCameraFollow_445525(
      pHeroBody->pSekHeader_220->pFrameT3EC,
      g_pCameraFrame,
      (CHanaFrameT3EC_S354 *)pMapFrame,
      1.0,
      0);
    ResetSceneBodies_4857CA(pScene);
    pfExecTime = 0.0;
    nElapsed = 0;
    g_dwBattleMode = -1;
    CSekBody__CutToAnim_4018E1(pHeroBody, 1);
    CSekBody__UpdateAnim_4019E1(pHeroBody, 0.0);
    CSekBody__PlayAnim_40179D(pHeroBody, 1);
    FrameTimer_BeginFrame();
    FrameTimer_EndFrame();
    fBlendTime = 0.0;
    nElapsed = nElapsedPrev;
    dwTimeBase += timeGetTime() - Time;
    Time = timeGetTime();
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFF);
    while ( 1 )
    {
      DoPeekMessages();
      nElapsedPrev = nElapsed;
      nElapsed = timeGetTime() - dwTimeBase;
      fDeltaTime = GetFrameTime();
      if ( fDeltaTime > 5.0 )
        fDeltaTime = 5.0;
      FrameTimer_EndFrame();
      if ( !bDialogueMode && IsMenuKeyPressed_481170() )
      {
        Time = timeGetTime();
        memcpy(&pSceneState_v18, &g_sceneState_52F7E0, sizeof(pSceneState_v18));
        memcpy(&g_sceneState_52F7E0, Src, sizeof(g_sceneState_52F7E0));
        memcpy(
          g_sceneState_52F7E0.sceneFlags_98,
          pSceneState_v18.sceneFlags_98,
          sizeof(g_sceneState_52F7E0.sceneFlags_98));
        StopAndRestartThread(0);
        qmemcpy(&sceneState_v4, &pSceneState_v18, sizeof(sceneState_v4));
        nMenuResult = SystemMenu_4655AB(pBgmBuffer, -1000, 0, fHeroHp, fHeroHpMax, sceneState_v4);
        StopAndRestartThread(1u);
        bFirstFrameDone = 0;
        if ( nMenuResult >= 1 )
          goto LABEL_125;
        memcpy(&g_sceneState_52F7E0, &pSceneState_v18, sizeof(g_sceneState_52F7E0));
        nMenuResult = -1;
      }
      if ( bDialogueMode )
      {
        v40 = wBtnMouse;
        GetMouseBtn_45A41B(&wBtnMouse);
        if ( wBtnMouse == 3 )
          fDeltaTime = fDeltaTime * 5.0;
        if ( wBtnMouse == 2 && wBtnMouse != v40 )
          bSkipMode = !bSkipMode;
        dwTimeBase += nElapsed - nElapsedPrev;
        nElapsed = nElapsedPrev;
      }
      else
      {
        if ( fBlendTime > 0.0 )
        {
          CSekBody__UpdateAnim_4019E1(pHeroBody, fDeltaTime);
          fBlendTime = fBlendTime - fDeltaTime;
          g_dwBattleMode = -1;
        }
        else
        {
          updated = UpdateHeroControl_45A513(
            pHeroBody,
            (CHanaFrameT3EC_S354 *)pMapFrame,
            fDeltaTime,
            &fHeroHp,
            fHeroHpMax,
            pnMapIndex,
            &fCamFactor);
          fBlendTime = 0.0;
          nTargetSprite = FindTargetInRange_45A1AD(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            pItemBodies,
            fDeltaTime,
            pnMapIndex);
          if ( nTargetSprite )
          {
            ApplyTargetEffect_45A2BA(nTargetSprite, &fHeroHp, &fHeroHpMax, &nTimeLimit, &fCamFactor, &fBlendTime);
            if ( nTargetSprite > 4 )
              SekBody__UpdateTo2_401647(pHeroBody, 12);
            else
              SekBody__UpdateTo2_401647(pHeroBody, 14);
            if ( ppOut[nTargetSprite] )
            {
              ppOut[nTargetSprite]->fAlpha_144 = 1.0;
              ProjectToScreen_40E2A5(&pCamFrame->mat4x4_after, &pOut);
              sceneState_v4.fPosY_18C = (float)pOut.y;
              sceneState_v4.fPosX_188 = (float)pOut.x;
              CHanaSpriteT3F6__SetPos_414A5D(ppOut[nTargetSprite], sceneState_v4.fPosX_188, sceneState_v4.fPosY_18C);
            }
          }
        }
        pHeroBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 0;
        UpdateCameraFollow_445525(
          pHeroBody->pSekHeader_220->pFrameT3EC,
          g_pCameraFrame,
          (CHanaFrameT3EC_S354 *)pMapFrame,
          fDeltaTime,
          0);
        UpdateLockonEvent_45953B(
          pScene,
          pHeroBody->pSekHeader_220->pFrameT3EC,
          pSekBody->pSekHeader_220->pFrameT3EC,
          pnMapIndex);
        CSekBody__UpdateAnim_4019E1(pSekBody, fDeltaTime);
        wBtnMouse = 0;
      }
      if ( updated != 1 )
        goto LABEL_84;
      nTransResult = CheckSceneTransition_459768(
        pScene,
        pHeroBody->pSekHeader_220->pFrameT3EC,
        &pnMapIndex,
        &pnSubIndex,
        nStageIndex);
      if ( nTransResult == 1 )
      {
        Time = timeGetTime();
        PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 50.0, 0);
        goto LABEL_125;
      }
      if ( nTransResult != 2 )
        break;
      pfExecTime = 10000.0;
      SetProjectionFov_40DCA3(0.5);
      bDialogueMode = 1;
      updated = 0;
      bSkipMode = 0;
      pScene->dwPtrCurrent_448 = 0;
      pScene->WCurNum = 0.0;
      pScene->flt_464 = pfExecTime;
      HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
      CalculateLocalRotationMatrix(
        pHeroBody->pSekHeader_220->pFrameT3EC,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        &dirForwardX,
        &dirForwardY,
        &dirForwardZ,
        &dirUpX,
        &dirUpY,
        &dirUpZ);
      MtxMult_4069DD(pSekBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 1000.0, 1000.0, 1000.0);
    LABEL_84:
      if ( pScene )
      {
        UpdateSceneTimer_4827FE(pScene, &pfExecTime, fDeltaTime);
        UpdateSceneAtTime_4825D5(pScene, &pfExecTime, fDeltaTime);
        if ( !bDialogueMode && pfExecTime > 1000.0 )
        {
          pfExecTime = 0.0;
          pScene->dwPtrPos_444 = 0;
          pScene->dwPtrCurrent_448 = 0;
        }
      }
      fTextSpeed = 1.0;
      if ( bDialogueMode && !bSkipMode )
      {
        if ( ParseExecStr_487D89(pScene, &pfExecTime) )
        {
          if ( (wBtnMouse == 3 || wBtnMouse == 1 && wBtnMouse != v40) && pScene->by_461 && !pScene->by_460 )
            pScene->by_461 = 0;
          if ( wBtnMouse == 1 && wBtnMouse != v40 )
            fTextSpeed = 1000.0;
        }
        else
        {
          SetProjectionFov_40DCA3(1.2);
          bDialogueMode = 0;
          MtxMult_4069DD(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            fHeroX,
            fHeroY,
            fHeroZ);
          UpdateBoneLookAtTransform(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            dirForwardX,
            dirForwardY,
            dirForwardZ,
            dirUpX,
            dirUpY,
            dirUpZ);
          AttachChild_4055FD(g_pSceneFrame, g_pCameraFrame);
          pScene->by_461 = 0;
          pScene->dwPtrPos_444 = 0;
          pScene->dwPtrCurrent_448 = 0;
          UpdateCameraFollow_445525(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            g_pCameraFrame,
            (CHanaFrameT3EC_S354 *)pMapFrame,
            fDeltaTime,
            1);
        }
      }
      MtxStackStartMultLocal_4074EF();
      if ( pScene && pScene->pSurface_468 )
      {
        g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntialiasSave);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
        FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
        pScene->flt_480 = 1.0;
        g_pBackBuffer_4D6C90->Blt(0, pScene->pSurface_468, 0, 0x1000000, 0);
        FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntialiasSave);
        g_pRenderCtx->Clear(2);
      }
      else
      {
        g_pRenderCtx->Clear(3);
        if ( g_pD3DDevice_4D6C78->BeginScene() >= 0 )
        {
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
          HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
          g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
          HanaVert_DrawBatch(1);
          g_pD3DDevice_4D6C78->EndScene();
        }
      }
      if ( bDialogueMode )
      {
        v11 = 0;
        if ( pfExecTime >= 10000.0 )
        {
          SekBody__UpdateTo2_401647(pHeroBody, 1);
        }
        else
        {
          v11 = pScene->refsSekBody_13C[pnCursor];
          HanaFrame_GetRelPosition(pScene->refsFrames_134[pnCursor], g_pSceneFrame, &fTgtX, &pOutY, &pOutZ);
          CalculateLocalRotationMatrix(
            pScene->refsFrames_134[pnCursor],
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            &pXpos,
            &pYpos,
            &pZpos,
            &pXYrot,
            &pYXrot,
            &pZXrot);
          MtxMult_4069DD(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            fTgtX,
            pOutY,
            pOutZ);
          UpdateBoneLookAtTransform(
            pHeroBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            pXpos,
            pYpos,
            pZpos,
            pXYrot,
            pYXrot,
            pZXrot);
          SekBody__UpdateTo2_401647(pHeroBody, v11->field_200);
        }
        CSekBody__UpdateAnim_4019E1(pHeroBody, fDeltaTime);
        if ( !bSkipMode )
        {
          sceneState_v4.frameTimeMultiplied_180 = fDeltaTime * fTextSpeed;
          DrawDialogueWindow_480B2D(pScene, sceneState_v4.frameTimeMultiplied_180, xMouse, yMouse, wBtnMouse);
        }
      }
      else
      {
        UpdateHudGauge_44BC36(pSprTimes_v120, (__int64)fHeroHp / 100, (__int64)fHeroHpMax / 100);
        DrawClockHud_44AF5B(pSprNumbers_v84, 100, -5, 100 * nTimeLimit - nElapsed / 0xA);
      }
      DrawSpriteListEcho_45B2FB(ppOut, nTargetSprite, fDeltaTime);
      
      if ( bFirstFrameDone )
      {
        PresentFrame();
        HanaVert_DrawBatch(0);
        if ( !bDialogueMode
          && fBlendTime <= 0.0
          && (100 * nTimeLimit - (int)nElapsed / 10 <= 0 || fHeroHp <= 0.0)
          && pHeroBody->field_200 != 16
          && pHeroBody->field_200 != 10
          && pHeroBody->field_200 != 11
          && pHeroBody->field_200 != 13 )
        {
          goto LABEL_125;
        }
      }
      else
      {
        bFirstFrameDone = 1;
        PlayScreenFade(1, 0, g_pBackBuffer_4D6C90, 50.0, 0);
        dwTimeBase += timeGetTime() - Time;
        Time = timeGetTime();
      }
    }
    if ( nTransResult != 3 )
    {
      SceneEventTime_459CA2 = GetSceneEventTime_459CA2(
        pScene,
        pHeroBody->pSekHeader_220->pFrameT3EC,
        &pnCursor,
        pnMapIndex);
      pfExecTime = SceneEventTime_459CA2;
      if ( SceneEventTime_459CA2 != 0.0 )
      {
        SetProjectionFov_40DCA3(0.5);
        bDialogueMode = 1;
        updated = 0;
        bSkipMode = 0;
        pScene->dwPtrCurrent_448 = 0;
        pScene->WCurNum = 0.0;
        pScene->flt_464 = pfExecTime;
        HanaFrame_GetRelPosition(pHeroBody->pSekHeader_220->pFrameT3EC, g_pSceneFrame, &fHeroX, &fHeroY, &fHeroZ);
        CalculateLocalRotationMatrix(
          pHeroBody->pSekHeader_220->pFrameT3EC,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          &dirForwardX,
          &dirForwardY,
          &dirForwardZ,
          &dirUpX,
          &dirUpY,
          &dirUpZ);
        MtxMult_4069DD(
          pSekBody->pSekHeader_220->pFrameT3EC,
          (CHanaFrameT3EC_S354 *)g_pSceneFrame,
          1000.0,
          1000.0,
          1000.0);
      }
      goto LABEL_84;
    }
    PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0, 0);
    nMenuResult = 0;
  LABEL_125:
    bDialogueMode = 0;
    StopAndRestartThread(0);
    if ( nMenuResult < 0 )
    {
      if ( fHeroHp <= 0.0 || 100 * nTimeLimit - (int)nElapsed / 10 <= 0 )
      {
        if ( nStageIndex == 7 )
        {
          g_sceneState_52F7E0.bActiveSceneStep = 80;
          nMenuResult = 0;
          goto LABEL_149;
        }
        if ( pBgmBuffer )
          pBgmBuffer->Stop();
        if ( pDSBuf[1] )
        {
          pDSBuf[1]->SetVolume(-500);
          DSBufferPlay(pDSBuf[1], 0);
        }
        PrepareTargetSelect_45B5BD(
          pHeroBody,
          pItemBodies,
          pSpriteNames,
          (CHanaFrameT3EC_S354 *)pMapFrame,
          pSprNumbers_v84,
          pSprTimes_v120,
          fHeroHp,
          fHeroHpMax,
          nTimeLimit,
          nElapsed);
        nSelectResult = ShowMouseSelectDialog_4573CF(pSprModes_v221, pDSBuf);
        PlayScreenFade(0, 0, g_pBackBuffer_4D6C90, 10.0, 0);
        if ( !nSelectResult )
        {
          nMenuResult = 1;
          goto LABEL_149;
        }
        pnMapIndex = mapIndexTable[2 * nStageIndex - 1];
        pnSubIndex = mapIndexTable[2 * nStageIndex];
        memcpy(&g_sceneState_52F7E0, Src, sizeof(g_sceneState_52F7E0));
        g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4 * 15.0 / 100.0
          + g_sceneState_52F7E0.protCurrentHP_C0;
        if ( g_sceneState_52F7E0.protCurrentHP_C0 > (double)g_sceneState_52F7E0.protMaximumHP_C4 )
          g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4;
        fHeroHp = g_sceneState_52F7E0.protCurrentHP_C0;
        memcpy(Src, &g_sceneState_52F7E0, sizeof(Src));
        g_sceneState_52F7E0.bCurrentSubStage = subStageTable[nStageIndex];
        g_sceneState_52F7E0.bMaxSubStage = subStageTable[nStageIndex] + 1;
        nTimeLimit = timeLimitTable[nStageIndex];
        SetProjectionFov_40DCA3(1.2);
        SetTimerScaleFactor(14.0);
        if ( pBgmBuffer )
          DSBufferPlay(pBgmBuffer, 1);
        dwTimeBase = timeGetTime();
        Time = timeGetTime();
      }
      if ( pMapFrame )
      {
        CHanaObj__Free_40C1E4(pMapFrame);
        pMapFrame = 0;
      }
      if ( pScene )
      {
        CHanaObj__Free_40C1E4(pScene);
        pScene = 0;
      }
      continue;
    }
    break;
  }
  if ( !nMenuResult )
  {
    g_sceneState_52F7E0.protCurrentHP_C0 = fHeroHp;
    g_sceneState_52F7E0.protMaximumHP_C4 = fHeroHpMax;
  }
LABEL_149:
  StopAndRestartThread(0);
  for ( i = 0; i < 10; ++i )
  {
    if ( pDSBuf[i] )
    {
      pDSBuf[i]->Stop();
      pDSBuf[i]->Release();
      pDSBuf[i] = 0;
    }
  }
  if ( pBgmBuffer )
  {
    pBgmBuffer->Stop();
    pBgmBuffer->Release();
    pBgmBuffer = 0;
  }
  CSekBody__Release_4012FE(pHeroBody);
  CSekBody__Release_4012FE(pSekBody);
  for ( i = 0; i < 10; ++i )
  {
    CSekBody__Release_4012FE(pItemBodies[i]);
    CSekBody__Release_4012FE(pSpriteNames[i]);
  }
  if ( pScene )
  {
    CHanaObj__Free_40C1E4(pScene);
    pScene = 0;
  }
  if ( pMapFrame )
  {
    CHanaObj__Free_40C1E4(pMapFrame);
    pMapFrame = 0;
  }
  for ( i = 0; i < 20; ++i )
  {
    if ( pSprTimes_v120[i] )
    {
      CHanaObj__Free_40C1E4(pSprTimes_v120[i]);
      pSprTimes_v120[i] = 0;
    }
  }
  for ( i = 0; i < 15; ++i )
  {
    if ( pSprNumbers_v84[i] )
    {
      CHanaObj__Free_40C1E4(pSprNumbers_v84[i]);
      pSprNumbers_v84[i] = 0;
    }
  }
  for ( i = 0; i < 15; ++i )
  {
    if ( pSprModes_v221[i] )
    {
      CHanaObj__Free_40C1E4(pSprModes_v221[i]);
      pSprModes_v221[i] = 0;
    }
  }
  for ( i = 0; i < 10; ++i )
  {
    if ( ppOut[i] )
    {
      CHanaObj__Free_40C1E4(ppOut[i]);
      ppOut[i] = 0;
    }
  }
  return nMenuResult;
}

