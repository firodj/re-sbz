#include "dx7.hpp"
#include "struct.hpp"
#include "console.hpp"

void AssertHanaStructSize() {
  static_assert(sizeof(D3DMATERIAL7) == 0x44, "D3DMATERIAL7");
  static_assert(sizeof(D3DXVECTOR3) == 0xC, "D3DXVECTOR3");
  static_assert(sizeof(D3DXMATRIX) == 0x40, "D3DXMATRIX");
  static_assert(sizeof(D3DXQUATERNION) == 0x10, "D3DXQUATERNION");
    
  static_assert(sizeof(CHanaVector2) == 0x8, "HanaVector2");
  static_assert(sizeof(CHanaBase_S130) == 0x130, "CHanaBase_S130");
  static_assert(sizeof(CHanaTextureT3EB_S2CC) == 0x2CC, "CHanaTextureT3EB_S2CC");
  static_assert(sizeof(CHanaFrameRootT3EC_S2D4) == 0x2D4, "CHanaFrameRootT3EC_S2D4");
  static_assert(sizeof(CHanaSubObj_S2C) == 0x2C, "CHanaSubObj_S2C");
  static_assert(sizeof(CHanaSubObj_S120) == 0x120, "CHanaSubObj_S120");
  static_assert(sizeof(CHanaSubObj_S8) == 0x8, "CHanaSubObj_S8");
  static_assert(sizeof(CHanaMorpT457_S40C) == 0x40C, "CHanaMorpT457_S40C");
  static_assert(sizeof(CHanaLightT3EF_S1A0) == 0x1A0, "CHanaLightT3EF_S1A0");

  static_assert(sizeof(CHanaSubObj_S50) == 0x50, "CHanaSubObj_S50");
  static_assert(sizeof(CHanaMateAnimT3F7_S148) == 0x148, "CHanaMateAnimT3F7_S148");
  static_assert(sizeof(CHanaMateT3EE_S184) == 0x184, "CHanaMateT3EE_S184");
  static_assert(sizeof(CHanaFrameT3EC_S354) == 0x354, "CHanaFrameT3EC_S354");
  static_assert(sizeof(CHanaSubObj_S78) == 0x78, "CHanaSubObj_S78");
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

  DebugLog("[%s] %s\n", "INFO", "finished AssertHanaStructSize");
}
