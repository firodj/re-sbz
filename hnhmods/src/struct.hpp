#pragma once

#include "dx7.hpp"
#include "..\d3dx\ida_defs.hpp";

#pragma pack(push, 1)

struct CHanaVector2
{
    int x;
    int y;
};

struct CHanaBase_S130
{
    void *field_0;
    int dwType;
    char szName_8[256];
    int nIndex_108;
    int field_10C;
    float flt_110;
    int field_114;
    int field_118;
    int field_11C;
    int iAlpha1000_120;
    int iAlpha1000_124;
    int field_128;
    int nCount_12C;
};

struct CHanaTextureT3EB_S2CC : public CHanaBase_S130
{
    LPDIRECTDRAWSURFACE7 pDDSurf;
    DDSURFACEDESC2 ddSurfDesc;
    int dwRGBBitCount;
    int dwWidth;
    int dwHeight;
    int lPitch;
    int szName_1C0[66];
    int bHasMask;
};

struct CHanaNode;
struct CHanaMorpT457_S40C;
struct CHanaAnimT3F0_S240;
struct CHanaLightT3EF_S1A0;
struct CHanaFrameT3EC_S354;

struct CHanaFrameRootT3EC_S2D4: public CHanaBase_S130
{
    int bFlagAlpha_130;
    int flag_134;
    D3DMATRIX mat4x4_fmrx;
    D3DMATRIX mat4x4_after;
    D3DMATRIX mat4x4_before;
    int bIsRoot_1F8;
    CHanaFrameT3EC_S354 *pParent;
    CHanaNode *pFirstChildNode;
    CHanaNode *pLastChildNode;
    int nCountChild;
    CHanaMorpT457_S40C *pAttachedObj_20C;
    CHanaLightT3EF_S1A0 *pHanaLight_210;
    int bHasLight_214;
    int flag_218;
    int flag_21C;
    D3DXVECTOR3 vertBoundMax_220;
    D3DXVECTOR3 vertBoundMin_22C;
    D3DXVECTOR3 transformedCorners[8];
    D3DXVECTOR3 worldAABBMin;
    D3DXVECTOR3 worldAABBMax;
    int vertMagnitude_2B0;
    D3DXVECTOR3 vertCenter_2B4;
    CHanaAnimT3F0_S240 *pHanaAnim_2C0;
    char bHasFog;
    char gap2C5[3];
    int dwFogColor;
    float fFogStart;
    float fFogEnd;
};

struct CHanaMeshT3F5_S138;
struct CHanaVertT3EA_S1FC;

struct CHanaSubObj_S2C
{
    int nSrc_0;
    int nFrame_4;
    float fWeight_8;
    D3DXVECTOR3 pos;
    D3DXVECTOR3 norm;
    int gap24;
    int gap28;
};

struct CHanaSubObj_S120
{
    int nCountS2C_0;
    int n0;
    int n1;
    int n2;
    int n3;
    CHanaVertT3EA_S1FC *pVert_14;
    char szName_18[256];
    CHanaFrameT3EC_S354 *pFrame_118;
    CHanaSubObj_S2C *parrS2C;
};

struct CHanaSubObj_S8
{
    CHanaSubObj_S120 *pSubObj_0;
    void (__cdecl **arrFun_4)(float *, float *, float);
};

struct  CHanaMorpT457_S40C : public CHanaBase_S130
{
    float fMorpParam_130;
    CHanaMeshT3F5_S138 *pMeshT3F5_138;
    int nCountVertT3EA_138;
    CHanaVertT3EA_S1FC **refsVertT3EA_13C;
    void *ptr_140;
    CHanaSubObj_S8 arrSubObj_144[64];
    int nCount_344;
    int *arrMKeyT3EASize_348;
    CHanaVector2 **arrMKeyT3EA_34C;
    int gap350[2];
    int dwEaseEnabled_358;
    int fEaseFrom_35C;
    int fEaseTo_360;
    int field_364;
    int field_368;
    int nTimeState_36C;
    int fTimeFrom_370;
    int fTimeTo_374;
    int fNextTo_37C[2];
    int dwClockStart_380;
    int fEaseProgress_384;
    int fEaseRate_388;
    int gap38C[32];
};

struct CHanaLightT3EF_S1A0: public CHanaBase_S130
{
    int dwLightIndex_130;
    D3DLIGHT7 d3dLight_134;
    CHanaFrameT3EC_S354 *pHanaT3EC_19C;
};

struct CHanaSubObj_S50
{
    float fNum_0;
    D3DMATERIAL7 d3dMat7_4;
    CHanaSubObj_S50 *psoS50_48;
    CHanaSubObj_S50 *psoS50_4C;
};

struct CHanaMateT3EE_S184;

struct CHanaMateAnimT3F7_S148: public CHanaBase_S130
{
    CHanaMateT3EE_S184 *pMateT3EE;
    float flt_134;
    int dwCount_138;
    CHanaSubObj_S50 *pObj_S50;
    CHanaSubObj_S50 *psoS50_140;
    int field_144;
};

struct CHanaMateT3EE_S184 : public CHanaBase_S130
{
    D3DMATERIAL7 d3dMaterial_130;
    float flt_174;
    CHanaTextureT3EB_S2CC *pTexture_178;
    int fMatDirty;
    CHanaMateAnimT3F7_S148 *pManmT3F7;
};

struct CHanaFrameT3EC_S354: public CHanaFrameRootT3EC_S2D4
{
    int field_2D4;
    int field_2D8;
    int field_2DC;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int field_2EC;
    int field_2F0;
    int field_2F4;
    int field_2F8;
    int field_2FC;
    int field_300;
    int field_304;
    int field_308;
    int field_30C;
    int field_310;
    int gap314[16];
};

struct CHanaSubObj_S78
{
    float t_0;
    float x_4;
    float y_8;
    float z_C;
    float sx_10;
    float sy_14;
    float sz_18;
    D3DXQUATERNION quart_1C;
    int b_2C;
    int b_30;
    int b_34;
    D3DXMATRIX mat4x4_38;
};

struct CHanaAnimT3F0_S240: public CHanaBase_S130
{
    CHanaFrameT3EC_S354 *pHanaFram_130;
    float fResolution_134;
    int nCount_138;
    CHanaSubObj_S78 *arroS78_13C;
    int field_140;
    int field_144;
    int field_148;
    CHanaSubObj_S78 objS78_14C;
    CHanaSubObj_S78 objS78_1C4;
    float float_23C;
};

struct CHanaMeshT3F5_S138 : public CHanaBase_S130
{
    int nCountVertT3EA_130;
    CHanaVertT3EA_S1FC **refsVertT3EA;
};

struct CHanaNode
{
    CHanaFrameT3EC_S354 *pData;
    CHanaNode *pPrev;
    CHanaNode *pNext;
};

struct CObjPoolMgr_S58
{
    D3DXMATRIX mat_0;
    CHanaFrameT3EC_S354 *pFrame;
    CHanaVertT3EA_S1FC *pVert;
    int fNeedsDraw_48;
    int nAmbient;
    QWORD bitLightEnables;
};

struct CObjItemRef_S208
{
    char szName_0[256];
    char szName_100[256];
    CHanaBase_S130 *ptr_200;
    int dwType_204;
};


#pragma pack(pop)

void AssertHanaStructSize();
