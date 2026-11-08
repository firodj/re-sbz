// GAME: [011019][TEATIME] ‰Ø‚Ì”ß–Â@`‰ó‚ê‚½ÉŽq‚ÌS`
#include "dx7.hpp"
#include "struct.hpp"
#include "console.hpp"

// g_ascSndFileNames_4C78A0 -- [011019][TEATIME] global wav-name lookup table
//
// Extracted verbatim from the game binary (.rdata @0x4C78A0, 0x2710 bytes).
// Layout: 200 records x char[50], NUL-padded (String50 in hnhmods/src/struct.hpp).
// Consumed by MenuMusicSelect_47D163 ([1..14], 7x2 music grid) and by the
// scene-script sound command in ParseExecStr_487D89 (idx -> WAV dir 1006 -> LoadWav).
//
// [1..14]  s###.wav   BGM/music      [100..131] SE###.wav sound effects
// '-'-filled rows are literal placeholder strings in the binary; the zero rows
// are padding and are initialized here as "".

extern const char g_ascSndFileNames_4C78A0[200][50] = {
  "--------",    // 0
  // BGM / music-select entries
  "s001.wav",    // 1
  "s002.wav",    // 2
  "s003.wav",    // 3
  "s004.wav",    // 4
  "s005.wav",    // 5
  "s006.wav",    // 6
  "s007.wav",    // 7
  "s008.wav",    // 8
  "s009.wav",    // 9
  "s010.wav",    // 10
  "s011.wav",    // 11
  "s012.wav",    // 12
  "s013.wav",    // 13
  "s014.wav",    // 14
  "---------",   // 15
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 20
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 25
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 30
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 35
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 40
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 45
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 50
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 55
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 60
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 65
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 70
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 75
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 80
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 85
  "---------",
  "---------",
  "---------",
  "---------",
  // zero padding
  "",            // 90
  "",
  "",
  "",
  "",
  "",            // 95
  "",
  "",
  "",
  "",
  // sound-effect entries (script sound command)
  "SE000.wav",   // 100
  "SE001.wav",   // 101
  "SE002.wav",   // 102
  "SE003.wav",   // 103
  "SE004.wav",   // 104
  "SE005.wav",   // 105
  "SE006.wav",   // 106
  "SE007.wav",   // 107
  "SE008.wav",   // 108
  "SE009.wav",   // 109
  "SE010.wav",   // 110
  "SE011.wav",   // 111
  "SE012.wav",   // 112
  "SE013.wav",   // 113
  "SE014.wav",   // 114
  "SE015.wav",   // 115
  "SE016.wav",   // 116
  "SE017.wav",   // 117
  "SE018.wav",   // 118
  "SE019.wav",   // 119
  "SE020.wav",   // 120
  "SE021.wav",   // 121
  "SE022.wav",   // 122
  "SE023.wav",   // 123
  "SE024.wav",   // 124
  "SE025.wav",   // 125
  "SE026.wav",   // 126
  "SE027.wav",   // 127
  "SE028.wav",   // 128
  "SE029.wav",   // 129
  "SE030.wav",   // 130
  "SE031.wav",   // 131
  "---------",
  "---------",
  "---------",
  "---------",   // 135
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 140
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 145
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 150
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 155
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 160
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 165
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 170
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 175
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 180
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 185
  "---------",
  "---------",
  "---------",
  "---------",
  "---------",   // 190
  "---------",
  "---------",
  "---------",
  "---------",
  // zero padding
  "",            // 195
  "",
  "",
  "",
  ""
};

// g_aG00Gp_4C9FB0 -- [011019][TEATIME] picture-name lookup table (GRP dir)
//
// Extracted verbatim from the game binary (.rdata @0x4C9FB0, 0x1388 bytes).
// Layout: 100 records x char[50], NUL-padded (String50 in hnhmods/src/struct.hpp).
// [0..98] G%02d.GP event pictures; [99] is a literal '-' placeholder row.
// Consumed by the scene-script picture command in ParseExecStr_487D89
// (idx -> GRP dir -> LoadBmp). Sits directly after g_ascSndFileNames_4C78A0
// (0x4C78A0+0x2710 == 0x4C9FB0): one contiguous String50-table rdata block.

extern const char g_aG00Gp_4C9FB0[100][50] = {
    "G00.GP",      // 0
    "G01.GP",      // 1
    "G02.GP",      // 2
    "G03.GP",      // 3
    "G04.GP",      // 4
    "G05.GP",      // 5
    "G06.GP",      // 6
    "G07.GP",      // 7
    "G08.GP",      // 8
    "G09.GP",      // 9
    "G10.GP",      // 10
    "G11.GP",      // 11
    "G12.GP",      // 12
    "G13.GP",      // 13
    "G14.GP",      // 14
    "G15.GP",      // 15
    "G16.GP",      // 16
    "G17.GP",      // 17
    "G18.GP",      // 18
    "G19.GP",      // 19
    "G20.GP",      // 20
    "G21.GP",      // 21
    "G22.GP",      // 22
    "G23.GP",      // 23
    "G24.GP",      // 24
    "G25.GP",      // 25
    "G26.GP",      // 26
    "G27.GP",      // 27
    "G28.GP",      // 28
    "G29.GP",      // 29
    "G30.GP",      // 30
    "G31.GP",      // 31
    "G32.GP",      // 32
    "G33.GP",      // 33
    "G34.GP",      // 34
    "G35.GP",      // 35
    "G36.GP",      // 36
    "G37.GP",      // 37
    "G38.GP",      // 38
    "G39.GP",      // 39
    "G40.GP",      // 40
    "G41.GP",      // 41
    "G42.GP",      // 42
    "G43.GP",      // 43
    "G44.GP",      // 44
    "G45.GP",      // 45
    "G46.GP",      // 46
    "G47.GP",      // 47
    "G48.GP",      // 48
    "G49.GP",      // 49
    "G50.GP",      // 50
    "G51.GP",      // 51
    "G52.GP",      // 52
    "G53.GP",      // 53
    "G54.GP",      // 54
    "G55.GP",      // 55
    "G56.GP",      // 56
    "G57.GP",      // 57
    "G58.GP",      // 58
    "G59.GP",      // 59
    "G60.GP",      // 60
    "G61.GP",      // 61
    "G62.GP",      // 62
    "G63.GP",      // 63
    "G64.GP",      // 64
    "G65.GP",      // 65
    "G66.GP",      // 66
    "G67.GP",      // 67
    "G68.GP",      // 68
    "G69.GP",      // 69
    "G70.GP",      // 70
    "G71.GP",      // 71
    "G72.GP",      // 72
    "G73.GP",      // 73
    "G74.GP",      // 74
    "G75.GP",      // 75
    "G76.GP",      // 76
    "G77.GP",      // 77
    "G78.GP",      // 78
    "G79.GP",      // 79
    "G80.GP",      // 80
    "G81.GP",      // 81
    "G82.GP",      // 82
    "G83.GP",      // 83
    "G84.GP",      // 84
    "G85.GP",      // 85
    "G86.GP",      // 86
    "G87.GP",      // 87
    "G88.GP",      // 88
    "G89.GP",      // 89
    "G90.GP",      // 90
    "G91.GP",      // 91
    "G92.GP",      // 92
    "G93.GP",      // 93
    "G94.GP",      // 94
    "G95.GP",      // 95
    "G96.GP",      // 96
    "G97.GP",      // 97
    "G98.GP",      // 98
    "-------"
};


// g_arrSceneNo_4C6F78 -- [011019][TEATIME] scene-chapter sequence table
//
// Extracted verbatim from the game binary (.data @0x4C6F78, 0x320 bytes).
// Element type CSceneSlot (mirrors hnhmods/src/struct.hpp):
//   struct CSceneSlot { int nSceneType; int nStrIndex; };  // 0x8
//
// The game walks this with bActiveSceneStep (CSceneState): GameMainLoop_47F013
// dispatches on nSceneType and pre-increments the step until type == 0.
// SysMenu_SaveMenu/LoadMenu_46 read table[saved step].nSceneType for status text.
//
// [0]      {0,0}  pad -- loop pre-increments past it (fresh game starts at step 1)
// [1..82]  live:  47 dialogues (1, str 0..46), 11 explores (2), 12 hunts (3),
//                10 assaults (4), ending at [80] (5: ED.avi, step:=0, progress:=2),
//                dialogue [81], backdrop [82] (6: step:=0, progress:=1 when 0)
// [83..99] {0,0}  zero tail (terminator + padding). Next rdata object @0x4C7298
//                is the "door" string -- confirms the [100] allocation bound.

extern const CSceneSlot g_arrSceneNo_4C6F78[100] = {
  { 0,  0 },    // 0 pad (pre-incremented past)
  { 1,  0 },
  { 1,  1 },
  { 1,  2 },
  { 1,  3 },
  { 1,  4 },    // 5 ODS dialogue
  { 1,  5 },
  { 3,  0 },
  { 1,  6 },
  { 3,  1 },
  { 4,  0 },    // 10 assault
  { 4,  1 },
  { 1,  7 },
  { 1,  8 },
  { 1,  9 },
  { 1, 10 },    // 15 ODS dialogue
  { 1, 11 },
  { 1, 12 },
  { 1, 13 },
  { 1, 14 },
  { 2,  0 },    // 20 explore
  { 3,  2 },
  { 1, 15 },
  { 2,  1 },
  { 3,  3 },
  { 4,  2 },    // 25 assault
  { 4,  3 },
  { 1, 16 },
  { 1, 17 },
  { 1, 18 },
  { 1, 19 },    // 30 ODS dialogue
  { 1, 20 },
  { 1, 21 },
  { 1, 22 },
  { 2,  2 },
  { 3,  4 },    // 35 hunt
  { 1, 23 },
  { 2,  3 },
  { 3,  5 },
  { 4,  4 },
  { 4,  5 },    // 40 assault
  { 1, 24 },
  { 1, 25 },
  { 1, 26 },
  { 1, 27 },
  { 1, 28 },    // 45 ODS dialogue
  { 1, 29 },
  { 1, 30 },
  { 2,  4 },
  { 3,  6 },
  { 1, 31 },    // 50 ODS dialogue
  { 2,  5 },
  { 3,  7 },
  { 1, 32 },
  { 2,  6 },
  { 3,  8 },    // 55 hunt
  { 4,  6 },
  { 4,  7 },
  { 1, 33 },
  { 1, 34 },
  { 1, 35 },    // 60 ODS dialogue
  { 1, 36 },
  { 1, 37 },
  { 2,  7 },
  { 1, 38 },
  { 1, 39 },    // 65 ODS dialogue
  { 2,  8 },
  { 3,  9 },
  { 1, 40 },
  { 2,  9 },
  { 3, 10 },    // 70 hunt
  { 1, 41 },
  { 2, 10 },
  { 3, 11 },
  { 4,  8 },
  { 4,  9 },    // 75 assault
  { 1, 42 },
  { 1, 43 },
  { 1, 44 },
  { 1, 45 },
  { 5,  0 },    // 80 ending
  { 1, 46 },
  { 6,  0 },    // 82 backdrop
  { 0,  0 },    // 83 terminator
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },    // 90
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 },
  { 0,  0 }
};


// g_arrSceneStepThresholds_4B5970 -- [011019][TEATIME] scene-step -> info-chapter thresholds
//
// Extracted verbatim from the game binary (.rdata @0x4B5970, 0xA0 bytes).
// Used by SysMenu_SaveMenu_462135 / SysMenu_LoadMenu_462851 to pick the save-slot
// description block: j = first index with threshold > bActiveSceneStep (scan stops
// at j < 39), then draw g_szSceneInfoText_4B5A10[1600*j .. ] (40 blocks x 1600).
// [0..25] real thresholds 1..77; [26]=99 catch-all (>= any reachable step <= 82);
// [27..39] zero padding, unreachable past the 99 stopper.

extern const int g_arrSceneStepThresholds_4B5970[40] = {
  1,  6,  9, 13, 15,  // [0..4]
  19, 22, 25, 28, 30,
  34, 37, 39, 42, 44,
  48, 50, 53, 56, 59,
  61, 64, 68, 71, 74,
  77, 99,  0,  0,  0,  // [25]=77, [26]=99 catch-all
  0,  0,  0,  0,  0,  // [30..] zero padding
  0,  0,  0,  0, 0 
};

extern const int g_texCoordSizeLUT_4A8368[4] = { 2 ,3 ,4, 1 };

// g_fVolumeDbTable -- [011019][TEATIME] AVI volume index -> DirectSound dB attenuation
//
// Extracted verbatim from the game binary (.rdata @0x4C6F4C, 0x28 bytes).
// Indexed by g_nAviVolumeIndex_530266 (BYTE; default 2). Consumers clamp the
// result (often - 1000 dB for BGM) to DirectSound's [-10000, 0] before
// IDirectSoundBuffer::SetVolume: GameMainLoop AVI audio, InGameMenu/MenuCharSelect
// menu BGM, SystemMenu, ParseExecStr script sound, PlayExploring/PlayHunting/
// PlayAssault BGM and voice buffers.
// [0..4] = mute/-20dB/-10dB/-5dB/0dB ladder; [5..9] zero (unreachable indices).


extern const float g_fVolumeDbTable[10] = {
  -10000.0f, -2000.0f, -1000.0f, -500.0f, 0.0f,   // [0..4] volume ladder
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f                    // [5..9] unused
};


void AssertHanaStructSize() {
  static_assert(sizeof(D3DMATERIAL7) == 0x44, "D3DMATERIAL7");
  static_assert(sizeof(D3DXVECTOR3) == 0xC, "D3DXVECTOR3");
  static_assert(sizeof(D3DXMATRIX) == 0x40, "D3DXMATRIX");
  static_assert(sizeof(D3DXQUATERNION) == 0x10, "D3DXQUATERNION");
    
  static_assert(sizeof(POINT) == 0x8, "POINT");
  static_assert(sizeof(CHanaBase_S130) == 0x130, "CHanaBase_S130");
  static_assert(sizeof(CHanaTextureT3EB_S2CC) == 0x2CC, "CHanaTextureT3EB_S2CC");
  static_assert(sizeof(CHanaFrameRootT3EC_S2D4) == 0x2D4, "CHanaFrameRootT3EC_S2D4");
  static_assert(sizeof(CHanaSubObj_S2C) == 0x2C, "CHanaSubObj_S2C");
  static_assert(sizeof(CHanaSubObj_S120) == 0x120, "CHanaSubObj_S120");
  static_assert(sizeof(CHanaSubObj_S8) == 0x8, "CHanaSubObj_S8");
  static_assert(sizeof(CHanaMorpT457_S40C) == 0x40C, "CHanaMorpT457_S40C");
  static_assert(sizeof(CHanaLightT3EF_S1A0) == 0x1A0, "CHanaLightT3EF_S1A0");

  static_assert(sizeof(CHanaMateAnimKeyT3F7_S50) == 0x50, "CHanaMateAnimKeyT3F7_S50");
  static_assert(sizeof(CHanaMateAnimT3F7_S148) == 0x148, "CHanaMateAnimT3F7_S148");
  static_assert(sizeof(CHanaMateT3EE_S184) == 0x184, "CHanaMateT3EE_S184");
  static_assert(sizeof(CHanaFrameT3EC_S354) == 0x354, "CHanaFrameT3EC_S354");
  static_assert(sizeof(CHanaAnimKeyT3F0_S78) == 0x78, "CHanaAnimKeyT3F0_S78");
  static_assert(sizeof(CHanaAnimT3F0_S240) == 0x240, "CHanaAnimT3F0_S240");
  static_assert(sizeof(CHanaMeshT3F5_S138) == 0x138, "CHanaMeshT3F5_S138");

  static_assert(sizeof(CObjPoolMgr_S58) == 0x58, "CObjPoolMgr_S58");
  static_assert(sizeof(CObjItemRef_S208) == 0x208, "CObjItemRef_S208");

  static_assert(sizeof(CHanaVertT3EA_S1FC) == 0x1FC, "CHanaVertT3EA_S1FC");
  static_assert(sizeof(CHanaFrameArrayT3ED_S138) == 0x138, "CHanaFrameArrayT3ED_S138");
  static_assert(sizeof(CHanaAnimSetT3F1_S150) == 0x150, "CHanaAnimSetT3F1_S150");
  static_assert(sizeof(CHanaSubObj_S38) == 0x38, "CHanaSubObj_S38");
  static_assert(sizeof(CHanaObj_S118) == 0x118, "CHanaObj_S118");
  static_assert(sizeof(CSoundStruct_8) == 0x8, "CSoundStruct_8");
  static_assert(sizeof(CHanaTextureCoordsT3F2_S140) == 0x140, "CHanaTextureCoordsT3F2_S140");
  static_assert(sizeof(CHanaTextureAnimT3F3_S16C) == 0x16C, "CHanaTextureAnimT3F3_S16C");
  static_assert(sizeof(CHanaTextureAnimSetT3F4_S138) == 0x138, "CHanaTextureAnimSetT3F4_S138");
  static_assert(sizeof(CHanaSpriteT3F6_S1FC) == 0x1FC, "CHanaSpriteT3F6_S1FC");
  static_assert(sizeof(CHanaMateAnimSetT3F8_S140) == 0x140, "CHanaMateAnimSetT3F8_S140");
  static_assert(sizeof(CHanaLightAnimT3F9_S144) == 0x144, "CHanaLightAnimT3F9_S144");
  static_assert(sizeof(CHanaSubObj_S74) == 0x74, "CHanaSubObj_S74");
  static_assert(sizeof(CHanaLightAnimSetT3FA_S13C) == 0x13C, "CHanaLightAnimSetT3FA_S13C");

  static_assert(sizeof(CHanaSceneT458_S504) == 0x504, "CHanaSceneT458_S504");
  static_assert(sizeof(CSekBody) == 0x5054, "CSekBody");
  static_assert(sizeof(CSekHeader) == 0x13C, "CSekHeader");
  static_assert(sizeof(CSekItem) == 0x9C, "CSekItem");
  static_assert(sizeof(CSekItemProp) == 0x8, "CSekItemProp");

  static_assert(sizeof(CD3DInitParam) == 0x34, "CD3DInitParam");
  static_assert(sizeof(CVideoModeDesc) ==0x18, "CVideoModeDesc");
  static_assert(sizeof(CDisplayConfig) == 0x10, "CDisplayConfig");
  static_assert(sizeof(CBMPColor) == 0x8, "CBMPColor");
  static_assert(sizeof(CBMPFile) == 0x10, "CBMPFile");

  static_assert(sizeof(CSceneSlot) == 0x8, "CSceneSlot");

  static_assert(sizeof(CSceneState) == 0x190, "CSceneState");
  static_assert(sizeof(CInputState) == 0x8, "CInputState");

  static_assert(sizeof(g_ascSndFileNames_4C78A0) == 0x2710, "g_ascSndFileNames_4C78A0");
  static_assert(sizeof(g_aG00Gp_4C9FB0) == 0x1388, "g_aG00Gp_4C9FB0");
  static_assert(sizeof(g_arrSceneNo_4C6F78) == 0x320, "g_arrSceneNo_4C6F78");
  static_assert(sizeof(g_arrSceneStepThresholds_4B5970) == 0xA0, "g_arrSceneStepThresholds_4B5970");
  static_assert(sizeof(g_texCoordSizeLUT_4A8368) == 16, "g_texCoordSizeLUT_4A8368");
  static_assert(sizeof(g_fVolumeDbTable) == 0x28, "g_fVolumeDbTable");

  DebugLog("[%s] %s\n", "INFO", "finished AssertHanaStructSize");
}



