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

    DebugLog("finished AssertHanaStructSize\n");
}