// GAME: [011019][TEATIME] ‰Ø‚Ì”ß–Â@`‰ó‚ê‚½ÉŽq‚ÌS`
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

  static_assert(sizeof(g_ascSndFileNames_4C78A0) == 0x2710, "table size");

  DebugLog("[%s] %s\n", "INFO", "finished AssertHanaStructSize");
}
