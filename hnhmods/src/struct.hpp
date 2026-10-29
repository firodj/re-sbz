#pragma once

#include "dx7.hpp"

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
    D3DXMATRIX mat4x4_fmrx;
    D3DXMATRIX mat4x4_after;
    D3DXMATRIX mat4x4_before;
    int bIsRoot_1F8;
    CHanaFrameT3EC_S354 *pParent;
    CHanaNode *pFirstChildNode;
    CHanaNode *pLastChildNode;
    unsigned int nCountChild;
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
    unsigned int nCountVertT3EA_138;
    CHanaVertT3EA_S1FC **refsVertT3EA_13C;
    void **pArrObjs_140;
    CHanaSubObj_S8 arrSubObj_144[64];
    unsigned int nCount_344;
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
    float fDiffuseA;
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

struct CHanaAnimKeyT3F0_S78 // was CHanaSubObj_S78
{
  float fTime; // t_0
  float fPosX; // x_4
  float fPosY; // y_8
  float fPosZ; // z_C
  float fScaleX; // sx_10
  float fScaleY; // sy_14
  float fScaleZ; // sz_18
  D3DXQUATERNION quatRot; // quart_1C
  int bKeyPos;   // b_2C
  int bKeyScale; // b_30
  int bKeyRot;   // b_34
  D3DXMATRIX matLocal; // mat4x4_38
};

struct CHanaAnimT3F0_S240: public CHanaBase_S130
{
    CHanaFrameT3EC_S354 *pHanaFram_130;
    float fDuration_134; // fResolution_134
    int   nKeyCount_138; // nCount_138
    CHanaAnimKeyT3F0_S78* arrKeys_13C; // arroS78_13C
    int field_140;
    int field_144;
    int field_148;
    CHanaAnimKeyT3F0_S78 keyCur_14C;  // objS78_14C
    CHanaAnimKeyT3F0_S78 keyPrev_1C4; // objS78_1C4
    float float_23C;
};

struct CHanaMeshT3F5_S138 : public CHanaBase_S130
{
    unsigned int nCountVertT3EA_130;
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
    CHanaBase_S130 *pObj_200;
    int dwObjType_204;
};

struct CHanaVertT3EA_S1FC: public CHanaBase_S130
{
    LPDIRECT3DVERTEXBUFFER7 pD3DVtxBuf_130;
    int dwNumVertices_134;
    int field_138;
    WORD *pVtxIndices;
    int dwNumIndices_140;
    CHanaTextureT3EB_S2CC *pTexT3EB;
    CHanaMateT3EE_S184 *pMateT3EE;
    int dwFVF;
    int dwVertexSize;
    int nPrimType_154[4];
    IUnknown *pUnk_164;
    int gap168[25];
    int flg_1CC;
    int nBlendMode_1D0;
    int flg_1D4;
    int fFlatShade_1D8;
    int flg_1DC;
    int fShadowOnly_1E0;
    int fUnlit_1E4;
    int flg_1E8;
    int nFogDensity_1EC;
    int nAlphaOpSel_1F0;
    int fNoTexPersp_1F4;
    int alpr_1F8;
};

struct  CHanaFrameArrayT3ED_S138 : public CHanaBase_S130
{
    int nFrameCount;
    CHanaFrameT3EC_S354 **arrFrames;
};

struct CHanaSubObj_S38
{
  float t;
  D3DXVECTOR3 pos;
  D3DXVECTOR3 scale;
  D3DXQUATERNION quat;
  int gap2C[3];
};

struct CSoundStruct_8
{
  LPDIRECTSOUNDBUFFER pDSBuffer_0;
  IDirectSound3DBuffer *pDS3DBuffer_4;
};

struct CHanaObj_S118
{
  CSoundStruct_8 *pSnd_0;
  char sz_name_4[256];
  float flt_104;
  float flt_108;
  int field_10C;
  int field_110;
  CHanaFrameT3EC_S354 *pFrame_114;
};

struct CHanaAnimSetT3F1_S150: public CHanaBase_S130
{
  float field_130;
  int nAnimCount_134;
  CHanaAnimT3F0_S240 **refsAnimT3F0_138;
  float fRes_13C;
  unsigned int nManmCount_140;
  CHanaMateAnimT3F7_S148 **pRefsManmT3F7;
  unsigned int nCount_148;
  CHanaObj_S118 *arroS118_14C;
};

struct CHanaTextureCoordsT3F2_S140: public CHanaBase_S130
{
  int dwNumVertices_130;
  float *tus_134;
  float *tvs_138;
  CHanaVertT3EA_S1FC *pVert_13C;
};

struct CHanaTextureAnimT3F3_S16C: public CHanaBase_S130
{
  int field_130;
  int field_134;
  int field_138;
  int field_13C;
  unsigned int dwCount_140;
  CHanaTextureCoordsT3F2_S140 **pTexCoords;
  CHanaFrameT3EC_S354 *pFrame_148;
  int field_14C;
  int field_150;
  int field_154;
  int field_158;
  int field_15C;
  int field_160;
  int field_164;
  int field_168;
};

struct CHanaTextureAnimSetT3F4_S138: public CHanaBase_S130
{
  unsigned int nCount_130;
  CHanaBase_S130 **pArrObjs_134;
};

struct CHanaSpriteT3F6_S1FC
{
  CHanaBase_S130 base_0;
  float gap130;
  LPDIRECTDRAWSURFACE7 pTexture_134;
  D3DXVECTOR3 destWHZ_138;
  float fAlpha_144;
  float fScale_148;
  float fRotation_14C;
  float fHalfWidth_150;
  float fHalfHeight_154;
  D3DXVECTOR2 *pCenter_158;
  struct tagRECT rect_15C;
  LPRECT pRect_16C;
  DDSURFACEDESC2 ddSurfaceDesc_170;
  int dwRGBBitCount_1EC;
  int dwWidth_1F0;
  int dwHeight_1F4;
  int lPitch_1F8;
};

struct CHanaMateAnimSetT3F8_S140: public CHanaBase_S130
{
  int field_130;
  unsigned int dwObjCount_134;
  CHanaBase_S130 **pArrObjs_138;
  int field_13C;
};

struct CHanaSubObj_S74
{
  float field_0;
  D3DLIGHT7 d3dLight_4;
  int field_6C;
  CHanaSubObj_S74 *pNext_70;
};

struct CHanaLightAnimT3F9_S144: public CHanaBase_S130
{
  CHanaLightT3EF_S1A0 *pLight_130;
  float field_134;
  int field_138;
  CHanaSubObj_S74 *psoS74_13C;
  int field_140;
};

struct CHanaLightAnimSetT3FA_S13C: public CHanaBase_S130
{
  int field_0;
  unsigned int nCount_4;
  CHanaBase_S130 **pArrObjs_8;
};

struct CSekHeader
{
  int field_0;
  int gap4[4];
  CHanaFrameT3EC_S354 *pFrameT3EC;
  int field_18;
  char szName_1C[260];
  CHanaAnimSetT3F1_S150 *pAnimSetT3F1;
  int field_124;
  int field_128;
  CHanaBase_S130 *pObj_12C;
  CHanaBase_S130 *pObj_130;
  CHanaBase_S130 *pObj_134;
  CHanaBase_S130 *pObj_138;
};

struct CSekItem
{
  int flag_0;
  char szName_4[64];
  int field_44;
  int field_48;
  int gap48;
  int field_50;
  float float_54;
  float float_58;
  float float_5C;
  float float_60;
  int field_64;
  int gap68;
  int field_6C;
  int field_70;
  int field_74;
  int gap78;
  int field_7C;
  int gap80[7];
};

struct CSekBody
{
  char szName_0[256];
  char szName_100[256];
  int field_200;
  int field_204;
  int field_208;
  int field_20C;
  int angle_210;
  int angle_214;
  int angle_218;
  float float_21C;
  CSekHeader *pSekHeader_220;
  float float_224;
  int gap228[7];
  int field_244;
  int field_248;
  int field_24C;
  CSekItem items_250[128];
  int count_5050;
};

struct CSekItemProp
{
  float flt_0;
  int index;
};

struct CHanaSceneT458_S504: public CHanaBase_S130
{
  unsigned int dwCount_130;
  CHanaFrameT3EC_S354 **refsFrames_134;
  CHanaAnimSetT3F1_S150 **refsAnimeSets_138;
  CSekBody **refsSekBody_13C;
  CSekItemProp **arrSekItemProps_140;
  DWORD *arrSizes_144;
  CHanaAnimSetT3F1_S150 *pAnimSet_148;
  CHanaSpriteT3F6_S1FC *pSprWindowBmp[5];
  char szName_160[256];
  void *ptrStrData_260;
  CHanaSpriteT3F6_S1FC *textSprites_264[4][29];
  int gap434[4];
  int dwPtrPos_444;
  int dwPtrCurrent_448;
  int dwPtrSize_44C;
  float flt_450;
  float flt_454;
  float flt_458;
  float WCurNum;
  char by_460;
  char by_461;
  char by_462;
  char by_463;
  float flt_464;
  LPDIRECTDRAWSURFACE7 pSurface_468;
  LPDIRECTSOUNDBUFFER pSndBuf_46C;
  int field_470;
  char by_474;
  char gap475[3];
  CHanaFrameT3EC_S354 *pCamFrame_478;
  IUnknown *pUnk_47C;
  float flt_480;
  int gap484[9];
  int field_4A8[23];
};

struct CD3DInitParam // sizeof=0x34
{
  int nDeviceIndex;
  int dwCreationFlags;
  HWND hWnd;
  HWND hWndFocus;
  int nNumColorBits;
  int nNumAlphaBits;
  int nNumDepthBits;
  int nNumStencilBits;
  int nNumBackBuffers;
  int nWidth;
  int nHeight;
  int nRefreshRate;
  int bUseCreateEx;
};

struct CVideoModeDesc // sizeof=0x18
{
  int nMaxNumVideoModes;
  D3DX_VIDMODEDESC *pModeDesc;
  D3DDEVICEDESC7 *pD3DCaps;
  DDCAPS *pDDHALCaps;
  DDCAPS *pDDHELCaps;
  BOOL *parrbActive;
};

struct CDisplayConfig // sizeof=0x10
{
  unsigned int nDeviceCount;
  int nDeviceIdxHal;
  D3DX_DEVICEDESC *parrD3DXDevDesc;
  CVideoModeDesc *parrVidModeDesc;
};

struct CVideoModeWhiteItem
{
  int Width;
  int Height;
  int bpp;
};

struct CBMPColor // sizeof=0x8
{
  __int16 size;
  __int16 count;
  RGBQUAD colors[1];
};


struct CBMPFile  // sizeof=0x10
{
  BITMAPFILEHEADER *pBitmapFileHeader;
  BITMAPINFOHEADER *pBitmapInfoHeader;
  CBMPColor *pBMPColor;
  LPBYTE pData;
};


// Scene-chapter table entry @0x4C6F78 (g_arrSceneNo_4C6F78, CSceneSlot[10]).
struct CSceneSlot  // sizeof=0x8
{
  int nSceneType;  // 0=terminator 1=ODS dialogue 2=map battle 3=duel 4=practice 5=ending 6=backdrop
  int nStrIndex;   // Str/ODS table index handed to the scene function
};

// In-game menu / scene progress block at CSceneState+0.
struct CSceneMenuState_9C  // sizeof=0x9C
{
  BYTE  bActiveSceneStep;      // +0x00 index into g_arrSceneNo_4C6F78
  BYTE  bCurrentSubStage;      // +0x01
  BYTE  gap02[2];              // +0x02
  int   nSceneDataIndices[11]; // +0x04
  BYTE  gap30[2];              // +0x30
  char  field_32;              // +0x32
  BYTE  gap33;                 // +0x33
  int   nSceneExtraData[24];   // +0x34
  BYTE  gap94[3];              // +0x94
  char  bMaxSubStage;          // +0x97
  int   pSceneFlags[1];        // +0x98
};

// Whole per-playthrough scene state @0x52F7E0 (g_sceneState_52F7E0).
struct CSceneState  // sizeof=0x190
{
  CSceneMenuState_9C menuState;          // +0x000
  int   field_9C[9];                     // +0x09C
  float field_C0;                        // +0x0C0
  float field_C4;                        // +0x0C4
  __int64 qwSelMask_C8;                  // +0x0C8
  int   field_D0;                        // +0x0D0
  int   gapD4[10];                       // +0x0D4
  int   field_FC;                        // +0x0FC
  int   gap100[31];                      // +0x100
  int   field_17C;                       // +0x17C
  float frameTimeMultiplied_180;         // +0x180
  LPDIRECTDRAWSURFACE7 lpDDSurface_184;  // +0x184
  float fPosX_188;                       // +0x188
  float fPosY_18C;                       // +0x18C
};

struct CInputState // sizeof=0x8
{
  __int16 mouseX_0;
  __int16 mouseY_2;
  char mouseBtn_4;
  char kbdMove_5;
  char true_6;
  char gap_7;
};

#pragma pack(pop)

void AssertHanaStructSize();