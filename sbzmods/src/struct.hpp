#pragma once

#include <string_view>
#include <type_traits>

namespace sbz {
#pragma pack(push, 1)

  enum ESBZScene
  {
    POP_SCENE = -1,
    LOGO_SCENE = 0,
    TITLE_SCENE = 1,
    MAP_SCENE = 2,
    ROOM_SCENE = 3,
    SHOP_SCENE = 4,
    NIGHTSHOP_SCENE = 5,
    PRESENT_SCENE = 6,
    COSTUME_SCENE = 7,
    LOAD0_SCENE = 8,
    LOAD2_SCENE = 9,
    CONFIG_SCENE = 18,
    STATUS_SCENE = 22,
    ESCAPE_SCENE = 25,
    CHECK_SCENE = 24,
    DIALOG_SCENE = 20,
    LOAD1_SCENE = 10,
    CUSTOM_SCENE = 31,
    EXTRA_SCENE = 21,
    OILCOATING_SCENE = 11,
    HCHECK_SCENE = 12,
    H_SCENE = 13,
    DATE_SCENE = 14,
    PLAY_SCENE = 15,
    KISS_SCENE = 16,
    RESULT_SCENE = 17,
    LOADING_SCENE = 19,
    EDIT_BODY_SCENE = 23,
    SPECIAL_SCENE = 26,
    SP_TITLE_SCENE = 27,
    SP_CHA_SEL_SCENE = 28,
    SP_MAP_SEL_SCENE = 29,
    SP_EMERGENCY_SCENE = 30,
    COL_TEST_SCENE = 1000,
    DATE_TEST_SCENE = 1001,
    TEST_PARENT_SCENE = 1002,
    TEST_PLAY_SCENE = 1003,
    TEST_TTY_SCENE = 1004,
    TOOL_SCENE = 1005,


  };


  struct CSBZGlobal;

  struct CSceneCallbacks
  {
    int nID;
    char bEnableAnaglyph;
    char _gap5[3];
    bool(__cdecl* pfnNew_08)(CSBZGlobal*);
    bool(__cdecl* pfnMainScene_0C)(CSBZGlobal*);
    bool(__cdecl* pfnChildScene_10)(CSBZGlobal*);
    bool(__cdecl* pfn3DRender_14)(CSBZGlobal*);
    bool(__cdecl* pfnMainUIRender_18)(CSBZGlobal*);
    bool(__cdecl* pfnChildUIRender_1C)(CSBZGlobal*);
    bool(__cdecl* pfnCleanUp_20)(CSBZGlobal*);
  };


  struct CSceneEntry
  {
    char nPhase;
    char bKilled;
    char nChildCount;
    char childIndices[10];
    char _gapD[3];
    CSceneCallbacks cb;
  };


  struct CSceneStack
  {
    char nCount;
    char nTopIdx;
    char nPhase;
    char _gap3;
    CSceneEntry scenes[10];
    bool(__cdecl* pfnPreFrame)(CSBZGlobal* pSBZGlobal_a1);
    bool(__cdecl* pfnPostUpdate)(CSBZGlobal*);
    bool(__cdecl* pfnPostUpdate2)(CSBZGlobal*);
    bool(__cdecl* pfnMidRender)(CSBZGlobal*);
    bool(__cdecl* pfnPostRender)(CSBZGlobal*);
    char bFrameDirty;
    char bAnaglyphPass;
    char bRender3D;
    char bRenderUI;
  };


  struct CSceneOp
  {
    char field_0;
    char _gap1[3];
    CSceneCallbacks Scene;
    int nSceneState;
  };

  struct CSceneObjectTemp;
  struct CStructImage_5C;
  struct CScnSubLight_174;
  struct CFXUnk;
  struct CEffectFixedFunctionParams;
  struct CEffectAnaglyphParams;
  struct CEffectSpecularParams;

  struct CSceneRenderMgr
  {
    int field_0;
    CSceneObjectTemp* arrSceneObjects[1280];
    int nCountImages_1404;
    CStructImage_5C* arrImages_1408[1024];
    char bools_2408[256];
    CScnSubLight_174* arrLights_2508[64];
    int nCountOpaque_2608;
    void* arrOpaqueQueue_260C[4096];
    int field_660C;
    int nCountAlpha_6610;
    int arrAlphaQueue_6614[4096];
    int _gapA614;
    D3DCOLOR clearColor_A618;
    int _gapA61C;
    BYTE field_A620;
    char _gapA621[3];
    int _gapA624;
    char field_A628;
    char _gapA629[3];
    int _gapA62C[4];
    char _gapA63C;
    BYTE field_A63D;
    char _gapA63E[2];
    int _gapA640[1648];
    char _gapC000[1604];
    int pDX9ScreenBlend_C644;
    int pDX9GaussScreen_C648;
    char byte_C64C;
    char _gapC64D[3];
    CFXUnk* ptr1byte_C650;
    CEffectFixedFunctionParams* pEffectFixedFuncParams_C654;
    char bool_C658;
    char _gapC659[3];
    CEffectAnaglyphParams* pEffectAnaglyphParams_C65C;
    CEffectSpecularParams* pEffectSpecularParams_C660;
    int field_C664;
  };

  struct CSceneManager
  {
    int nFadePhase_3CC;
    int nFadePhaseCur_3D0;
    char bQuitAfterFade_3D4;
    char nQueuedSceneOps_3D5;
    char _gapA[2];
    CSceneOp QueuedSceneOps_3D8[5];
    int nFadeStateA_4B4;
    int nFadeStateB_4B8;
    int nFadeState_4BC;
    int fFadeDoneThreshold_4C0;
    int nFadeInMs_4C4;
    int nFadeOutMs_4C8;
    int nFadeDurMs_4CC;
    int nFadeElapsedMs_4D0;
    float fFadeAlpha_4D4;
    float fFadeAlphaStart_4D8;
    float fFadeAlphaEnd_4DC;
    int field_4E0;
    CSceneRenderMgr renderMgr_4E4;
    int pGameFadeCtrl;
    int pFadeSpriteQuad;
    char nFadeWarmupFrames_C788;
    char _gapC789[3];
  };

  struct CSetting_448
  {
    int field_0;
    int Width_4;
    int Height_8;
    int ColorBit_C;
    char field_10;
    char field_11[2];
    char _gap13;
    char bUseSoftwareRasterizer_14;
    char bHideMessageBoxOnError_CB6D;
    char nMipFilter_16;
    char field_17;
    char field_18;
    char _gap19;
    char field_1A;
    char field_1B;
    char bFogEnabled;
    char field_1D;
    char field_1E;
    char field_1F;
    char field_20;
    char field_21;
    char field_22;
    char field_23;
    char field_24;
    char field_25;
    char field_26;
    char field_27;
    char field_28;
    char field_29;
    char field_2A;
    char field_2B;
    char field_2C;
    char field_2D;
    char _gap2E;
    char bWithoutMouse_CB87;
    char field_30;
    char field_31;
    char _gap32[2];
    char _gap34[1024];
    char field_434;
    char _gap435[3];
    char _gap438[16];
  };

  // From: dcommon.h (include D2d1.h)
  typedef struct D2D_POINT_2F {
    FLOAT x;
    FLOAT y;
  } D2D_POINT_2F;


  struct CString
  {
    char* pszData;
  };

  struct CStructImage_5C;
  struct CStructImage2D_80;

  struct CSpriteSheet
  {
    char bLoaded_0;
    char paths[4][260];
    char _gap411[3];
    int field_414;
    int gap_418[64];
    int nTexCount_518;
    int nSpCount_51C;
    int nColCount_520;
    CStructImage_5C** pTex_524;           ///< Tex=Texture
    CStructImage2D_80** pSp_528;          ///< Sp=Sprite
    CStructImage2D_80** pCol_52C;         ///< Col=Colission
  };

  struct CPackFormatSecret
  {
    char packMode_D500;                   ///< = 0x02
    char nKeyLength;
    char _gap2[2];
    int dwDecryptKey[8];
    char nType_D524;
    char _gap25[3];
  };


  struct CPackItem
  {
    char szName[260];
    int dwSize;
    int dwOffset;
  };


  struct CPack
  {
    void* lpVtbl;
    DWORD hFile;
    int nFormat;
    int nCount;
    CPackItem* lpData;
    int nEndHeader;
    CPackFormatSecret* lpPackFormatSecret;
    BYTE nType;
    char field_1D;
    char field_1E;
    char field_1F;
  };
  struct CSB_CameraBase;
  struct CSB_Key;
  struct CSpriteSheet;
  struct CSB_BackgroundSprite;
  struct CSB_CursorCtrl;

  struct CSB_Path;
  struct CPackFormatSecret;
  struct CSB_Setting;
  struct CSB_Sound;
  struct CIME_Manager;
  struct CSB_GameInfo;
  struct CSB_CharFemale;
  struct CSB_CharMale;
  struct CSB_Map;
  struct CSB_ThreadCtrl;

  struct CSBGameCore
  {
    char nWAAtive;
    char _gap1[3];
    CSB_CameraBase* SBCameraBase;
    CSB_Key* Key_CFBC;
    CSpriteSheet struct530_C;
    CSB_BackgroundSprite* pBackgroundSprite;
    CSB_CursorCtrl* CursorCtrl_D4F4;
    IDirect3DSurface9* pDepthStencilSurface9_D4F8;
    CSB_Path* Path_D4FC;
    CPackFormatSecret PackFormatSecret;
    CSB_Setting* Setting_D528;
    CSB_Sound* Sound_D52C;
    CIME_Manager* pCIME_Manager_D530;
    CSB_GameInfo* GameInfo_D534;
    CSB_CharFemale* arrSBCharFemales_D538[5];
    CSB_CharMale* SBCharMale_598;
    CSB_Map* pMap_59C;
    char _gap5A0[4];
    CSB_ThreadCtrl* ThreadCtrl_D558;
    int _gap5A8[5];
    void(__cdecl* pDropFileHandler_5BC)(CSBZGlobal*, WPARAM);
  };


  struct SBLogoScene;
  struct SBTitleScene;
  struct SBMainMapScene;
  struct SBRoomScene;
  struct SBShopScene;
  struct SBNightShopScene;
  struct SBCosChangeScene;
  struct SBPresentScene;
  struct SBDateScene;
  struct SBPlayScene;
  struct SBKissScene;
  struct SBScriptScene;
  struct SBResultScene;
  struct SBOilCoatingScene;
  struct SBHScene;
  struct SBHCheckScene;
  struct SBExtraScene;
  struct SBStatusScene;
  struct SBEditBodyScene;
  struct SBConfigScene;
  struct SBLoadScene;
  struct SBCheckScene;
  struct SBEscapeScene;
  struct SBLoadingScene;
  struct CBPTestScene;
  struct CSB_ColTestScene;
  struct CSB_DateTestScene;
  struct CTCTestParent;
  struct CTCTestPlayScene;
  struct CTCTestTTY;
  struct CIS_TestScene;
  struct SBToolScene;
  struct SBSpecialScene;
  struct SBSpTitleScene;
  struct SBSpChaSelScene;
  struct SBSpMapSelScene;
  struct CSBSPEmergency;

  struct CDXSoundManager;
  struct CMouseState;

  struct CGameData
  {
    CSBGameCore gameCore_CFB4;
    SBLogoScene* pSBLogoScene_D574;
    SBTitleScene* pSBTitleScene_D578;
    SBMainMapScene* pSBMainMapScene_D57C;
    SBRoomScene* pSBRoomScene;
    SBShopScene* pSBShopScene_D584;
    SBNightShopScene* pSBNightShopScene;
    SBCosChangeScene* pSBCostumeChangeScene_D58C;
    SBPresentScene* pSBPresentScene_D590;
    SBDateScene* pSB_DateScene_D594;
    SBPlayScene* pSBPlayScene;
    SBKissScene* pSBKissScene_D59C;
    SBScriptScene* pSBScriptScene_D5A0;
    SBResultScene* pSBResultScene_D5A4;
    SBOilCoatingScene* pSBOilCoatingScene_D5A8;
    SBHScene* pSB_HScene_D5AC;
    SBHCheckScene* pSBHentaiCheckScene_5FC;
    SBExtraScene* pSBExtraScene_600;
    SBStatusScene* pSBStatusScene_604;
    SBEditBodyScene* pSBEditBodyScene_608;
    SBConfigScene* pSBConfigScene_60C;
    int field_610;
    SBLoadScene* pSBLoadScene_614;
    SBCheckScene* pSBCheckScene_618;
    SBEscapeScene* pSBEscapeScene_61C;
    SBLoadingScene* pSBLoadingScene_620;
    CBPTestScene* pCBPTestScene_D5D8;
    CSB_ColTestScene* pSBColTestScene_628;
    CSB_DateTestScene* pCSBDateTestScene_62C;
    CTCTestParent* pTestParentScene_630;
    CTCTestPlayScene* pTestPlayScene_634;
    CTCTestTTY* pTestTTYScene_638;
    CIS_TestScene* pIS_TestScene;
    SBToolScene* pSBToolScene_640;
    SBSpecialScene* pSBSpecialScene_644;
    SBSpTitleScene* pSBSpTitleScene_648;
    SBSpChaSelScene* pSBSpChaSelScene_64C;
    SBSpMapSelScene* pSBSpMapSelScene_650;
    CSBSPEmergency* pSBSpEmergencyScene_654;
    float fFPS_D60C;
    float fDeltaMs_D610;
    int field_660;
    CSceneRenderMgr pSceneRenderMgr;
    CDXSoundManager* pDXSoundMgr;
    CMouseState* pMouseState0;
    CMouseState* pMouseState1;
    float fMouseScale_19C8C;
    D2D_POINT_2F cursorPos_19C90;
    D2D_POINT_2F cursorRawPos_19C98;
    int arrSceneIDsStack[6];
    char field_CD04;
    char bCaptureScreen_CD05;
    char _gapCD06[2];
    int field_CD08;
  };

  // From: ID3DXFont (D3dx9core.h)
  typedef struct ID3DXFont* LPD3DXFONT;
  struct CInput_70;
  struct CDlgCtrl;

  struct CSBZGlobal
  {
    HINSTANCE hInstance;
    int hMutex;
    HWND hWnd;
    int hMenu;
    int gap_10[2];
    char szWindowName[128];
    char szClassName[128];
    RECT Rect_118;
    RECT Rect_128;
    RECT rcWindowArea_138;
    RECT Rect_148;
    int nWidth_158;
    int nHeight_15C;
    int nWidth_160;
    int nHeight_164;
    int dwStyle;
    int dwExStyle;
    int nCursorX;
    int nCursorY;
    WNDCLASSEXA WndClassExA;
    CSceneStack sceneStack_1A8;
    CSceneManager sceneMgr_3CC;
    CSetting_448 setting_CB58;
    float fTexScale_CFA0;
    char field_CFA4;
    char _gapCFA5[3];
    D2D_POINT_2F scrollSpeed;
    char bRequestQuit_CFB0;
    char _gapCFB1[3];
    CGameData gameData_CFB4;
    int nFontSize_19CC0;
    D3DCOLOR textColor_19CC4;
    LPD3DXFONT pFontMSGothic_19CC8;
    CInput_70* pInput70;
    CDlgCtrl* pDlgCtrl_19CD0;
  };

#pragma pack(pop)

  void AssertStructSize();
  std::string_view SceneToString(ESBZScene scene);

  typedef bool(*EnumerateSceneCb)(ESBZScene scene, std::string_view name);

  void EnumerateScenes(EnumerateSceneCb callback);


  extern LPVOID(__cdecl* MemAllocate_719FA0)(SIZE_T dwBytes);
  extern char(__cdecl* InitScene_4951A0)(ESBZScene nNextSceneID, unsigned __int8 nStackOp, unsigned __int8 nSceneState, unsigned __int8 nTransition, int nFadeInMs, int nFadeOutMs);

};
