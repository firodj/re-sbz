# CURRENT - GOALS coverage in hnhmods/src

Generated 2026-10-31 by `py tools/gen_current.py` from `hnhmods/GOALS.md`
vs `hnhmods/src/*.cpp` (function data via `tools/scan_cpp.py`).

Legend:

- `[HOOK+PTR file]` - detour implemented+registered (`hnh_Hook`) and original pinned as func-ptr
- `[HOOK file]` - detour implemented+registered only
- `[PTR file]` - original pinned as func-ptr only (called, not detoured)
- `[IMPL file]` - implemented but not `hnh_Hook`-registered
- `[todo]` - nothing in hnhmods/src yet

## Call tree with status

```
0x49f107 WinMainCRTStartup                                                                      <- [todo]
    ├──0x485ce0 WinMain_485CE0                                                                  <- [HOOK WinMain_485CE0.cpp]
    │   ├──0x485fc2 WndProcCheckSystem_485FC2                                                   <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   ├──0x418620 Renderer_InitializeTrigTablesAndPixelFormat                             <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │   ├──0x419bcd SetUiFont_419BCD                                                    <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   ├──0x40e0e2 SetFogRange_40E0E2                                                      <- [HOOK+PTR SetFogEnable_40DFB0.cpp]
    │   │   ├──0x4869a7 CheckInstalledDirRoot_4869A7                                            <- [HOOK+PTR CheckInstalledDirRoot_4869A7.cpp]
    │   │   │   └──0x486630 SetupDirPath_486630                                                 <- [HOOK+PTR CheckInstalledDirRoot_4869A7.cpp]
    │   │   │       └──0x47e8ee IsFileExists_47E8EE                                             <- [HOOK+PTR CheckInstalledDirRoot_4869A7.cpp]
    │   │   ├──0x4863c8 CheckRunGame_4863C8                                                     <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   ├──0x427da8 ShowTitleMenu_427DA8                                                <- [HOOK+PTR ShowTitleMenu.cpp]
    │   │   │   │   ├──0x4276c0 D3DSetRenderStates_4276C0                                       <- [HOOK+PTR ShowTitleMenu.cpp]
    │   │   │   │   ├──0x428927 ShowTitleMenuWnd_428927                                         <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   └──0x428c11 WndProcTitleMenu_428C11                                     <- [HOOK WndProcTitleMenu.cpp]
    │   │   │   │   │       ├──0x428a7c GetNthValidDimensionString                              <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       ├──0x428bc1 DrawTextPairs                                           <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       ├──0x4211ed FreeBmp_4211ED                                          <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       ├──0x420df0 LoadBmp_420DF0                                          <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       │   └──0x421095 CreatePaletteFromBuffer                             <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       ├──0x486d9a CHanaObj__GetRealPath_486D9A                            <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       ├──0x42889b UpdateRenderOptionsCheckboxes                           <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   │       └──0x42833c GetNthActiveVideoMode                                   <- [HOOK+PTR WndProcTitleMenu.cpp]
    │   │   │   │   ├──0x427cc9 DisplayConfig_Destroy                                           <- [HOOK+PTR ShowTitleMenu.cpp]
    │   │   │   │   │   └──0x40df64 MemFree_40DF64                                              <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │   │       ├──0x40a813 PrintDebugError                                         <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │   ├──0x40a894 D3DInit_40A894                                                  <- [HOOK+PTR ShowTitleMenu.cpp]
    │   │   │   │   │   ├──0x40af60 D3DInitBuffers_40AF60                                       <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   ├──0x40f720 CHanaObj__NewMateT3EE_S184                              <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   │   ├──0x40bfc3 CHanaObj__AddRef_40BFC3                             <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │   │   │   └──0x40def7 MemAlloc_40DEF7                                     <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │   │   │   ├──0x404720 CHanaObj__NewFrameT3EC_S354                             <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   │   ├──0x4055fd AttachChild_4055FD                                  <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   │   │   ├──0x4058c3 RemoveChild_4058C3                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │   │   │   │   ├──0x4074ef MtxStackStartMultLocal_4074EF                   <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x4075b0 MtxStackRecursiveMulLocal_4075B0            <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   ├──0x408f00 CHanaObj__NewLightT3EF                                  <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   │   ├──0x40bab8 AttachHanaObj_40BAB8                                <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   │   │   └──0x40b848 CHanaObj__AddObjItemRef_40B848                  <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   │   ├──0x40929c ApplyLightSettings                                  <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │   │   ├──0x407047 CalculateLocalRotationMatrix                            <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   ├──0x404908 CHanaObj__NewFrameRootT3EC_S2D4                         <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   ├──0x40c64a MatrixLookAtLH                                          <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   │   │   ├──0x40e420 Vec3Set                                             <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   │   │   ├──0x40e3c0 Vec3LengthSq                                        <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   │   │   ├──0x40e3f0 Vec3Dot                                             <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   │   ├──0x4070ab MatrixRotateCubeMapFace                                 <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   ├──0x40f891 ApplyMaterial                                           <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │   │   ├──0x40dd71 SetLightingState                                        <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │   │   │   ├──0x40ddf3 DegreesToRadians                                        <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   │   ├──0x40cb94 MatrixPerspectiveFovLH                                  <- [HOOK+PTR dx7.cpp]
    │   │   │   │   │   ├──0x40aefb AllocateDevicesDescArray                                    <- [HOOK+PTR ShowTitleMenu.cpp]
    │   │   │   │   │   └──0x40ac7c D3DDeinit                                                   <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │   │   │       ├──0x40c1e4 CHanaObj__Free_40C1E4                                   <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x410501 CHanaObj__FreeT3FC                                  <- [PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   ├──0x40bd4d CHanaObj__RemoveFromGlobalRef                   <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   └──0x4114df CHanaObj__FreeT3FD                              <- [PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x415e05 CHanaObj__FreeTexCoordT3F2                          <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x413f07 CHanaObj__FreeSpriteT3F6                            <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x417a0c CHanaObj__FreeVertT3EA                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x408e8d CHanaObj__FreeFrameArrayT3ED                        <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x433e8d CHanaObj__FreeMorpT457                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x409820 CHanaObj__FreeLightAnimT3F9                         <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   └──0x40922e SetupLightFromD3D                               <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x4054b3 CHanaObj__FreeFrameT3EC                             <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   ├──0x405421 Frame_FreeAttachedLight                         <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   ├──0x40536d Frame_FreeAttachedObject                        <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x4166b5 CHanaObj__FreeT3FB                                  <- [PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x40e6b6 CHanaObj__FreeMateAnimT3F7                          <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x409437 CHanaObj__FreeLightT3EF                             <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x40a3bb CHanaObj__FreeLightAnimSetT3FA                      <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x40fdcb CHanaObj__FreeMeshT3F5                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x401f53 CHanaObj__FreeAnimT3F0                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   └──0x401edf UnlinkHanaAnimFromFrame                         <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x403a55 CHanaObj__FreeAnimSetT3F1                           <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   │   ├──0x422c72 FreeSoundStruct_422C72                          <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x416256 CHanaObj__FreeTextureT3EB                           <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x41595b CHanaObj__FreeTexAnimSetT3F4                        <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x40fc5e CHanaObj__FreeMateT3EE                              <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x414f62 CHanaObj__FreeTexAnimT3F3                           <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x40f3e5 CHanaObj__FreeMateAnimSetT3F8                       <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   ├──0x413075 CHanaObj__FreeT3FE                                  <- [PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │   └──0x4821fa CHanaObj__FreeSceneT458                             <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │       └──0x4012fe CSekBody__Release_4012FE                        <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   │       │           └──0x40d8f6 FreeSekHeader                               <- [HOOK+PTR CHanaObj__Free_40C1E4.cpp]
    │   │   │   │   └──0x4278ff DisplayConfig_Init                                              <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │       ├──0x427f8c DisplayConfig_ProcessDriverDesc                             <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   │       ├──0x427c51 IsVideoModeSupported                                        <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   ├──0x47e7f2 WriteBufferToFile                                                   <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   │   ├──0x47e858 ReadFileToBuffer                                                    <- [HOOK+PTR CheckRunGame_4863C8.cpp]
    │   │   ├──0x40e02d SetFogMode_40E02D                                                       <- [HOOK+PTR SetFogEnable_40DFB0.cpp]
    │   │   ├──0x41888e Renderer_CleanupFont                                                    <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   ├──0x485c50 CDROM_VerifyDiscSignature                                               <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   │   ├──0x485ba8 CDROM_VerifyWin9x                                                   <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   │   │   ├──0x485ada CDROM_IoctlReadSectorWin9x                                      <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   │   │   │   └──0x485b27 CDROM_DeviceIoControlVWIN32                                 <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   │   └──0x4858d0 CDROM_VerifyNT                                                      <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   │       └──0x485a2a CDROM_GetDriveFileSystemType                                    <- [HOOK+PTR CDROM_VerifyDiscSignature_485C50.cpp]
    │   │   ├──0x422d50 HanaDS_Init                                                             <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │   ├──0x423ccc HanaDS_SetErrStr                                                    <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │   │   └──0x423ce7 DSErrToString_423CE7                                            <- [HOOK+PTR DSErrToString_423CE7.cpp]
    │   │   │   └──0x424886 HanaDS_InitHeap                                                     <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │       └──0x427520 HanaHeap_Create                                                 <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   ├──0x422690 HanaDS_Create3DListener                                                 <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   ├──0x40dfb0 SetFogEnable_40DFB0                                                     <- [HOOK+PTR SetFogEnable_40DFB0.cpp]
    │   │   ├──0x40e0b4 SetFogColor_40E0B4                                                      <- [HOOK+PTR SetFogEnable_40DFB0.cpp]
    │   │   ├──0x422e14 HanaDS_Shutdown                                                         <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │   └──0x42489d HanaDS_DestroyHeap                                                  <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   │   │       └──0x427551 HanaHeap_Destroy                                                <- [HOOK+PTR WndProcCheckSystem_485FC2.cpp]
    │   ├──0x43ab06 return1_20                                                                  <- [todo]
    │   ├──0x43ad67 DInputInit_43AD67                                                           <- [HOOK+PTR WinMain_485CE0.cpp]
    │   │   └──0x43af33 DIMouseKeybInit_43AF33                                                  <- [HOOK+PTR WinMain_485CE0.cpp]
    │   ├──0x47f013 GameMainLoop_47F013                                                         <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   ├──0x47df0d MainMenu                                                                <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   ├──0x47d163 MenuMusicSelect                                                     <- [PTR InGameMenu_4799E2.cpp]
    │   │   │   │   ├──0x47e680 ConvertFrom1024AsInt                                            <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   ├──0x40bf01 PresentFrame                                                    <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   ├──0x40be03 RecoverLostDevice                                           <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   └──0x40ceec HanaVert_DrawBatch                                          <- [PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │       ├──0x439d48 UpdateVertTextureFromFrame                              <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x404b80 CalcFrameBound_404B80                               <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │       │   │   ├──0x418544 CHanaObj::LockVtxBufT3EA_418544                 <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │       │   │   ├──0x41858e CHanaObj::UnlockVrxBufT3EA_41858E               <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │       │   │   ├──0x40ff0f CHanaObj::GetChildCountT3F5_40FF0F              <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   │   ├──0x40bbf4 CHanaObj::GetType_40BBF4                        <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │       │   │   └──0x40ff1d CHanaObj::GetChildAtT3F5_40FF1D                 <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x40e2a5 ProjectToScreen_40E2A5                              <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   │   └──0x40e167 BuildViewportMatrix                             <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   │       └──0x40dcf6 GetProjectionFov_40DCF6                     <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x418905 CreateSurfaceWH                                     <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │       │   │   └──0x41898e DDErrToString_41898E                            <- [HOOK+PTR DSErrToString_423CE7.cpp]
    │   │   │   │   │       │   ├──0x417b27 HanaVert_Draw                                       <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   │   └──0x40fb30 HanaMate_SetMaterial                            <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x43bf6f FlushD3DAndTouchSurface                             <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │       │   ├──0x47e6af ConvertFrom640AsInt                                 <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │       │   ├──0x419d16 BltFastWH                                           <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │       ├──0x40c54c HanaVert_DrawByType                                     <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x434fc9 HanaMorpT457_Draw                                   <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   │   └──0x40fe8e HanaMeshT3F5_Draw                               <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       │   ├──0x416776 HanaMesh_Draw                                       <- [PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       ├──0x40ce4e SetLightEnableMask                                      <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │       └──0x40d9c7 SortObjectsByDepth                                      <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │           └──0x408c3e HanaFrame_GetCenterPosition_408C3E                  <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   ├──0x414191 DrawSprite2D                                                    <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   ├──0x45a41b GetMouseBtn_45A41B                                              <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   ├──0x4815ec MutexThreadWait_4815EC                                      <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   │   └──0x481b37 GetInputState_481B37                                        <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   ├──0x47e69b ConvertFrom1024AsFloat                                          <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   ├──0x4655ab SystemMenu_4655AB                                               <- [PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   ├──0x414589 DrawSpriteLTRB                                              <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   ├──0x46380d SysMenu_Graphics_46380D                                     <- [todo]
    │   │   │   │   │   │   ├──0x419b05 DrawTextOnSurfaceSpaced_419B05                          <- [todo]
    │   │   │   │   │   │   │   └──0x4188b4 GetDC_4188B4                                        <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   │   ├──0x47e6c6 ConvertFrom640AsFloat                                   <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   │   ├──0x423baf DSBufferPlay                                            <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   ├──0x4203b3 LoadBmpScaled_4203B3                                    <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   │   ├──0x419de5 GetSurfaceFromBmp_419DE5                            <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   │   │   │   ├──0x421ff4 BMP_Convert24bitTo16bitWithDither               <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   │   │   │   │   └──0x421bd5 BMP_DitherRGB24ToRGB565                     <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   │   │   ├──0x419d89 BltSurface                                          <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   │   ├──0x4147ed DrawSpriteAtRect                                    <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   │   ├──0x4133d0 CHanaObj__NewSprite3F6byWH                          <- [HOOK+PTR CHanaObj__NewFrameT3EC_S354.cpp]
    │   │   │   │   │   │   │   └──0x419a1a GetSurfaceWH                                        <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   └──0x414a5d CHanaSpriteT3F6::SetPos_414A5D                          <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   ├──0x464919 SysMenu_GirlStatusCheck_464919                              <- [todo]
    │   │   │   │   │   │   ├──0x419a4b DrawTextOnSurface                                       <- [todo]
    │   │   │   │   │   │   ├──0x44af15 IntPow_44AF15                                           <- [todo]
    │   │   │   │   │   ├──0x461a9e ShowYesNoDialog_461A9E                                      <- [todo]
    │   │   │   │   │   │   ├──0x414b09 CHanaSpriteT3F6::SetScale_414B09                        <- [todo]
    │   │   │   │   │   │   ├──0x414b28 CHanaSpriteT3F6::SetSrcRect_414B28                      <- [todo]
    │   │   │   │   │   │   ├──0x414aba CHanaObj::SetAlpha3F6                                   <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x481550 DoPeekMessages                                          <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   └──0x414afb CHanaSpriteT3F6::GetAlpha_414AFB                        <- [todo]
    │   │   │   │   │   ├──0x462135 SysMenu_SaveMenu_462135                                     <- [todo]
    │   │   │   │   │   │   ├──0x46164c ShowFadeDialog_46164C                                   <- [todo]
    │   │   │   │   │   │   ├──0x461557 LoadDataFiles_461557                                    <- [todo]
    │   │   │   │   │   │   │   ├──0x461397 FormatFileLastWriteTime_461397                      <- [todo]
    │   │   │   │   │   │   │   │   ├──0x4612b0 ZeroPadNumberToString_4612B0                    <- [todo]
    │   │   │   │   │   ├──0x462851 SysMenu_LoadMenu_462851                                     <- [todo]
    │   │   │   │   │   ├──0x413a5e LoadBmpWithMask_413A5E                                      <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   │   ├──0x43ea05 CreateTextureFromSurfaceWithAlphaMask                   <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   │   ├──0x4874de PlayScreenFade                                              <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x47e782 SetTimerScaleFactor                                     <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x405c06 HanaFrame_Draw                                          <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   ├──0x40600c ProcessFrame                                        <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   │   ├──0x4084c2 AlwaysTrue                                      <- [IMPL ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   │   ├──0x408283 UpdateBoneTransform                             <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x406ca4 UpdateBoneLookAtTransform                   <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   │   │   └──0x40873e CameraLookAtFrame                           <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   │   ├──0x409510 ConfigureLight                                  <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   ├──0x40cd32 AddObjectToPool                                 <- [HOOK+PTR ProcessFrame_40600C.cpp]
    │   │   │   │   │   │   │   ├──0x406ab1 HanaFrame_GetRelPosition                            <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   ├──0x4339b2 HanaFrame_PreDraw                                   <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   ├──0x43383a HanaMorp_ApplyTree                              <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x433569 HanaMorpT457_Animate                        <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   │   │   └──0x47e78f GetTimerScaleFactor                     <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   │   └──0x43315f HanaMorpT457_Interp                         <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   │       ├──0x432db0 HanaVert_BlendMorph                     <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   └──0x433916 HanaMorp_ApplySubTree                           <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │       ├──0x4365b9 HanaMorpT457_ApplySubs                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │       │   ├──0x4363ee HanaVert_Transform                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   └──0x40773f HanaFrame_BuildMatrixStack                          <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │       ├──0x407800 HanaFrame_PushNode                              <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │       │   └──0x405bb4 SetInversTransf_405BB4                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x47e79a GetFrameTime                                            <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x4874a5 RenderFlush_4874A5                                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   ├──0x43f3f7 SelfBltDC                                           <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x41973f RGBToSurfaceFormat                                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x40bf4f RenderClear_40BF4F                                      <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x4137d1 CHanaObj::NewSpriteT3F6bySurf                           <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x47e7d5 FrameTimer_EndFrame                                     <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   ├──0x47e768 FrameTimer_BeginFrame                                   <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   └──0x413d78 CHanaObj::ReplaceTexture3F6                             <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   ├──0x481ae4 StopAndRestartThread                                        <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   │   │   ├──0x4818fd CreateNewThread                                         <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   │   │   │   ├──0x4817b1 InputThread_Absolute_4817B1                         <- [HOOK StopAndRestartThread.cpp]
    │   │   │   │   │   │   │   │   └──0x4811fa GetCursorPos_4811FA                             <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   │   │   │   ├──0x481637 InputThread_Relative_481637                         <- [HOOK StopAndRestartThread.cpp]
    │   │   │   │   │   │   └──0x481a35 TerminateThreadGracefully                               <- [HOOK+PTR StopAndRestartThread.cpp]
    │   │   │   │   │   ├──0x462fe9 SysMenu_Options_462FE9                                      <- [todo]
    │   │   │   │   ├──0x45a4d8 GetMouseXY_45A4D8                                               <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   │   │   ├──0x481170 IsMenuKeyPressed_481170                                         <- [PTR MenuCharSelect.cpp]
    │   │   │   │   └──0x422e7b LoadWav_422E7B                                                  <- [HOOK+PTR LoadSoundEffects_47EB7F.cpp]
    │   │   │   │       ├──0x4248d2 SafeFree                                                    <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │       │   └──0x4275df HeapFreeRef                                             <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │       └──0x4248a7 AllocMem                                                    <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   │           └──0x427591 HeapAllocRef                                            <- [HOOK+PTR MemAlloc_40DEF7.cpp]
    │   │   │   ├──0x47a324 MenuCharSelect                                                      <- [HOOK+PTR MenuCharSelect.cpp]
    │   │   │   │   ├──0x4703ae PlayAssault_4703AE                                              <- [PTR GameMainLoop_47F013.cpp]
    │   │   │   │   │   ├──0x487d89 ParseExecStr_487D89                                         <- [todo]
    │   │   │   │   │   │   ├──0x47f460 Scene_CreateFontTextureFromStringData                   <- [todo]
    │   │   │   │   │   │   │   ├──0x43bcae DrawText_43BCAE                                     <- [todo]
    │   │   │   │   │   │   │   │   ├──0x4198e0 Surface_SetColorKeyRGB                          <- [PTR MenuCharSelect.cpp]
    │   │   │   │   │   │   │   │   └──0x43bffa CheckSpecialJpChar_43BFFA                       <- [todo]
    │   │   │   │   │   │   ├──0x47ff01 ParseStrChunkText_47FF01                                <- [todo]
    │   │   │   │   │   │   ├──0x487d46 FindCharUntil_487D46                                    <- [todo]
    │   │   │   │   │   │   ├──0x487267 Character_ApplyAnimationParamsToChildren                <- [todo]
    │   │   │   │   │   │   │   ├──0x405a0a CHanaObj::NewFrameArrayT3EDFromChildT3EC            <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   ├──0x408dd0 CHanaObj::NewFrameArrayT3ED                     <- [todo]
    │   │   │   │   │   │   │   ├──0x408e75 CHanaObj::GetFramesCountT3ED_408E75                 <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   └──0x408e1b CHanaObj::GetFrameAtIndexT3ED                       <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   ├──0x46f37e RunModeSelectScreen                                         <- [todo]
    │   │   │   │   │   │   ├──0x47e999 FindFrameByName_47E999                                  <- [PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   ├──0x46a2a0 PauseSceneAudio_46A2A0                                  <- [todo]
    │   │   │   │   │   │   ├──0x467420 ApplyFrameEffectOption                                  <- [todo]
    │   │   │   │   │   │   │   ├──0x46739d Frame_ApplyFadeRecursive                            <- [todo]
    │   │   │   │   │   │   │   │   └──0x46724e FrameObject_SetAlpha                            <- [todo]
    │   │   │   │   │   │   ├──0x46d7a7 DrawSpritePulse_46D7A7                                  <- [todo]
    │   │   │   │   │   │   │   ├──0x414a7a CHanaSpriteT3F6::GetPos_414A7A                      <- [todo]
    │   │   │   │   │   │   │   ├──0x414b1a CHanaSpriteT3F6::GetScale_414B1A                    <- [todo]
    │   │   │   │   │   │   ├──0x4825d5 UpdateSceneAtTime_4825D5                                <- [todo]
    │   │   │   │   │   │   │   ├──0x403d61 CHanaAnimSetT3F1::SetTime_403D61                    <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   ├──0x40e969 ApplyMateAnimMaterial                           <- [todo]
    │   │   │   │   │   │   │   │   │   └──0x40ef23 FindMateAnimKeyframeBounds                  <- [todo]
    │   │   │   │   │   │   │   │   ├──0x4248f2 GetSoundBufferAverageLevel                      <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   ├──0x40283c InterpolateHanaAnimFrame                        <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x40344a HanaAnim_FindInterpolationFrame             <- [todo]
    │   │   │   │   │   │   │   │   │   ├──0x402d0e CHanaAnimT3F0::BakeKeyframesT3F0_402D0E     <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x4035b1 ApplyQTS_4035B1                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   ├──0x4019e1 CSekBody::UpdateAnim_4019E1                         <- [todo]
    │   │   │   │   │   │   │   │   ├──0x4018e1 CSekBody::CutToAnim_4018E1                      <- [todo]
    │   │   │   │   │   │   │   │   ├──0x43a643 CHanaAnimSetT3F1::BlendStep_43A643              <- [todo]
    │   │   │   │   │   │   │   │   │   └──0x43a43f HanaAnim_UpdateBoneTransform                <- [todo]
    │   │   │   │   │   │   │   │   └──0x43a371 CHanaAnimSetT3F1::BeginBlend_43A371             <- [todo]
    │   │   │   │   │   │   │   ├──0x401647 SekBody::UpdateTo2_401647                           <- [todo]
    │   │   │   │   │   │   │   └──0x4069dd MtxMult_4069DD                                      <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   ├──0x467af3 SetFrameVisibilityAndGetHIndex                          <- [todo]
    │   │   │   │   │   │   └──0x4827fe UpdateSceneTimer_4827FE                                 <- [todo]
    │   │   │   │   │   ├──0x40dca3 SetProjectionFov_40DCA3                                     <- [todo]
    │   │   │   │   │   ├──0x4667a9 InitializePracticeAnimations_4667A9                         <- [todo]
    │   │   │   │   │   │   ├──0x466738 Frame_SetPracticeAnimRootRecursive                      <- [todo]
    │   │   │   │   │   ├──0x46777d Scene_FindMapFrame                                          <- [todo]
    │   │   │   │   │   ├──0x480b2d DrawDialogueWindow_480B2D                                   <- [todo]
    │   │   │   │   │   │   ├──0x47fa84 DrawAdvanceDialogueText_47FA84                          <- [todo]
    │   │   │   │   │   │   ├──0x4803b5 DrawDialogueText_4803B5                                 <- [todo]
    │   │   │   │   │   ├──0x46d9af RunSceneSelectScreen                                        <- [todo]
    │   │   │   │   │   ├──0x4857ca ResetSceneBodies_4857CA                                     <- [todo]
    │   │   │   │   │   ├──0x48564b LoadOdsEx_48564B                                            <- [todo]
    │   │   │   │   │   │   ├──0x484a38 ParseOdsEx_484A38                                       <- [todo]
    │   │   │   │   │   │   │   ├──0x483704 ParseOds3DON_483704                                 <- [PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   │   │   ├──0x483650 CHanaFrameT3EC::FindByName_483650               <- [todo]
    │   │   │   │   │   │   │   │   └──0x42277e LoadWavToDS3DBuffer_42277E                      <- [HOOK+PTR LoadSoundEffects_47EB7F.cpp]
    │   │   │   │   │   │   │   ├──0x441f19 ParseODFFile_441F19                                 <- [PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   │   │   ├──0x403a00 CHanaObj::NewAnimSetT3F1_S150                   <- [todo]
    │   │   │   │   │   │   │   │   ├──0x4375b3 HanaMorpT457_SetupFunsSubtree                   <- [todo]
    │   │   │   │   │   │   │   │   │   ├──0x4372ba HanaMorpT457_SetupFuns                      <- [todo]
    │   │   │   │   │   │   │   │   │   │   ├──0x436300 HanaVert_LerpMorph                      <- [todo]
    │   │   │   │   │   │   │   │   │   │   ├──0x434fe0 GetT3F5FromObj_434FE0                   <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x40fca0 CHanaObj::NewMeshT3F5               <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x40fceb HanaMesh_AddVertexReference         <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x4363a3 HanaVert_CopyMorph                      <- [todo]
    │   │   │   │   │   │   │   │   │   │   ├──0x4417eb HanaFrame_FindByName                    <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   ├──0x441a5d ParseODFContent_441A5D                          <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x44106a ParseOdfFRAM_44106A                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x40539d AssignLigthtToFrame_40539D              <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x4414eb ParseOdfANIM_4414EB                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x4039c9 HanaAnim_GetDuration                    <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x40ba89 HanaFrame_GetFrameName                  <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x401cd0 CHanaObj::NewAnimT3F0                   <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x403c15 CHanaAnimSetT3F1::AddAnimT3F0_403C15    <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   └──0x401fbd HanaAnim_LinkFrame                      <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x44068c ParseOdfMESH_44068C                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x440288 ParseVERT_440288                        <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x416ce0 CHanaObj::NewVertT3EA               <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x417f88 CalculateVertexSize             <- [HOOK+PTR PlayScreenFade.cpp]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43f460 ParseOdfMeshTEXT_43F460             <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x418103 HanaVert_SetTexture             <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   │   └──0x4181b5 HanaVert_ReleaseTexture     <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40bc13 FindObjByType_40BC13            <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x4400c3 GetVerByName_4400C3                 <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x416e65 CHanaObj::FillVertexBufferT3EA      <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43fdec ParseOdfMeshVFLG_43FDEC             <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43fbd1 ParseOdfMeshMANM_43FBD1             <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40e500 CHanaObj::NewMateAnimT3F7       <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40e782 HanaMateAnim_SetMaterial        <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x4044d9 HanaAnimSet_AddMaterialAnimRef  <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   │   └──0x40e7da HanaMateAnim_InsertKeyframe     <- [todo]
    │   │   │   │   │   │   │   │   │   │   │   └──0x43f8fe ParseOdfMeshMATE_43F8FE             <- [todo]
    │   │   │   │   │   │   │   │   │   │   │       ├──0x418221 HanaVert_SetMaterial            <- [todo]
    │   │   │   │   │   │   │   │   │   │   │       │   └──0x41828c FreeMate_41828C             <- [todo]
    │   │   │   │   │   │   │   │   │   ├──0x440896 ParseOdfMORP_440896                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x439982 HanaMorp_InitSubObject                  <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x433da7 CHanaObj::NewMorpT457                   <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   │   ├──0x434370 HanaMorp_AddVertexRef                   <- [PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   │   ├──0x44189c ParseOdf3DON_44189C                         <- [HOOK+PTR ParseODFContent_441A5D.cpp]
    │   │   │   │   │   │   │   │   └──0x441e7e HanaFrame_PropagateFogToChildren                <- [todo]
    │   │   │   │   │   │   │   ├──0x40179d CSekBody::PlayAnim_40179D                           <- [PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   │   ├──0x401039 ParseSekFile_401039                                 <- [PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   │   │   └──0x40d875 SekHeader_CloneClean                            <- [todo]
    │   │   │   │   │   │   │   ├──0x483567 ReadStrFile_483567                                  <- [HOOK+PTR ParseODS_483932.cpp]
    │   │   │   │   │   │   └──0x481c5f CHanaObj__NewSceneT458_S504                             <- [todo]
    │   │   │   │   │   ├──0x4843cf LoadOds_4843CF                                              <- [todo]
    │   │   │   │   │   │   ├──0x483932 ParseOds_483932                                         <- [HOOK+PTR ParseODS_483932.cpp]
    │   │   │   │   │   ├──0x4445dd LoadSekAndOdf_4445DD                                        <- [todo]
    │   │   │   │   │   │   ├──0x4014e4 SekBody::UpdateTo_4014E4                                <- [todo]
    │   │   │   │   │   ├──0x47e3e1 ToggleWaitScreen                                            <- [todo]
    │   │   │   │   │   │   ├──0x47e1a2 WaitScreenThreadProc                                    <- [todo]
    │   │   │   │   │   ├──0x423c65 DSBufferGetVolume_423C65                                    <- [todo]
    │   │   │   │   │   ├──0x46a76f PlayCutInScene_46A76F                                       <- [todo]
    │   │   │   │   │   │   ├──0x46a503 UpdateSekBodyAnimationMatrix                            <- [todo]
    │   │   │   │   │   │   │   ├──0x46a3e9 UpdateFrameAnimationMatrix                          <- [todo]
    │   │   │   │   │   │   │   ├──0x404277 SetAnimSetTime                                      <- [todo]
    │   │   │   │   │   │   │   └──0x401c67 GetSekBodyCurrentAnimTime                           <- [todo]
    │   │   │   │   │   │   ├──0x47ea86 FindFrameByNameSubstringRecursive                       <- [todo]
    │   │   │   │   │   │   ├──0x466978 CameraFrame_TrackBetweenObjectsAndLookAt                <- [todo]
    │   │   │   │   │   │   ├──0x468da8 DrawFadeTransition_Scene                                <- [todo]
    │   │   │   │   │   │   │   ├──0x468b99 Scene_ApplyFadeToPracticeFrames                     <- [todo]
    │   │   │   │   │   │   ├──0x4668ac Scene_ProcessPracticeFrames                             <- [todo]
    │   │   │   │   │   │   │   └──0x4665f0 Frame_CheckCameraDistanceAndRecurse                 <- [todo]
    │   │   │   │   │   │   ├──0x4690ab SoundAH_ManageEffectsAndAnimate                         <- [todo]
    │   │   │   │   │   │   │   ├──0x481be0 HanaMorp_AnimateAndPropagate                        <- [todo]
    │   │   │   │   │   │   ├──0x4680b4 UpdateCutInSceneBoneTransforms                          <- [todo]
    │   │   │   │   │   │   ├──0x469eb6 Character_UpdateAnimationState                          <- [todo]
    │   │   │   │   │   │   │   └──0x48713f HanaObj_CheckFrameCondition                         <- [todo]
    │   │   │   │   │   │   ├──0x467abe AnimSet_ResetAndTransferAnimations                      <- [todo]
    │   │   │   │   │   │   │   ├──0x4679b2 TransferAnimSetFrameAnimations_4679B2               <- [todo]
    │   │   │   │   │   │   │   │   ├──0x404349 CHanaAnimSetT3F1::RemoveAnimT3F0_404349         <- [todo]
    │   │   │   │   │   │   ├──0x468a4a Cursor_ClampToCenterOrSprite                            <- [todo]
    │   │   │   │   │   │   ├──0x4682d6 MoveCutInSceneFramesWithCameraConstraint                <- [todo]
    │   │   │   │   │   │   ├──0x466bda CameraController_UpdateCutscene                         <- [todo]
    │   │   │   │   │   │   ├──0x46a55d CameraFrame_ProjectFramePairOntoForwardRay              <- [todo]
    │   │   │   │   │   │   ├──0x467160 CutInScene_CyclePosAndApplyCameraOffset                 <- [todo]
    │   │   │   │   │   │   │   ├──0x407945 HanaFrame_GetMatAfter                               <- [todo]
    │   │   │   │   │   │   │   └──0x40799f HanaMatrix_ApplyTransform                           <- [todo]
    │   │   │   │   │   │   ├──0x467dfd IsObjectValidRecursive_467DFD                           <- [todo]
    │   │   │   │   │   │   │   ├──0x467d62 IsParentValid_467D62                                <- [todo]
    │   │   │   │   │   │   │   │   └──0x467bbd SyncObjectAnimationByName_467BBD                <- [todo]
    │   │   │   │   │   │   ├──0x4698e8 SoundSE_ManageEffectsAndPlay                            <- [todo]
    │   │   │   │   │   │   ├──0x467e72 AnimSet_MatchAndSortKutiRTibusaLFrames                  <- [todo]
    │   │   │   │   │   │   ├──0x4686f8 UpdateCutInSceneCameraFollow                            <- [todo]
    │   │   │   ├──0x47ad8d MenuGallery                                                         <- [PTR InGameMenu_4799E2.cpp]
    │   │   ├──0x44c38d PlayHunting_44C38D                                                      <- [PTR GameMainLoop_47F013.cpp]
    │   │   │   ├──0x455183 PlayVictoryCutscene_455183                                          <- [todo]
    │   │   │   │   ├──0x458b3a UpdateVoicePlayback_458B3A                                      <- [todo]
    │   │   │   │   ├──0x44b23b DrawRecViewfinder_44B23B                                        <- [todo]
    │   │   │   │   │   ├──0x414344 DrawSprite3D                                                <- [todo]
    │   │   │   │   ├──0x44af5b DrawClockHud_44AF5B                                             <- [todo]
    │   │   │   │   ├──0x407b73 HanaMatrix_BuildLookAtAndTransform                              <- [todo]
    │   │   │   ├──0x456807 RenderDualBodyScene_456807                                          <- [todo]
    │   │   │   ├──0x45460a PlayCinematicSequence_45460A                                        <- [todo]
    │   │   │   │   ├──0x445525 UpdateCameraFollow_445525                                       <- [todo]
    │   │   │   │   │   ├──0x443cc5 CheckFrameCollision_443CC5                                  <- [todo]
    │   │   │   │   │   │   ├──0x4423b0 HitSupportSurface_4423B0                                <- [todo]
    │   │   │   │   │   │   │   └──0x407e1f ComputeMeshBounds_407E1F                            <- [todo]
    │   │   │   │   │   │   └──0x442c1c HitOverheadSurface_442C1C                               <- [todo]
    │   │   │   │   │   ├──0x4443fc TestMoveCollision_4443FC                                    <- [todo]
    │   │   │   │   │   │   ├──0x443ec4 TestBodyMoveHit_443EC4                                  <- [todo]
    │   │   │   │   ├──0x453fac DrawTransitionFlash_453FAC                                      <- [todo]
    │   │   │   │   ├──0x4532d0 DrawProgressFill_4532D0                                         <- [todo]
    │   │   │   │   ├──0x4542e0 UpdateBodiesInput_4542E0                                        <- [todo]
    │   │   │   │   ├──0x44b97b DrawHudSpriteNumber_44B97B                                      <- [todo]
    │   │   │   ├──0x453b92 WanaAttack_Projectile_453B92                                        <- [todo]
    │   │   │   │   ├──0x4581b0 StepWanaProjectile_4581B0                                       <- [todo]
    │   │   │   ├──0x445b12 BuildMapCollisionGrid_445B12                                        <- [todo]
    │   │   │   ├──0x44451f UpdateCameraLinkedObjects_44451F                                    <- [todo]
    │   │   │   ├──0x45669f DrawFadeOutSpark_45669F                                             <- [todo]
    │   │   │   ├──0x4490a7 InitHudResources_4490A7                                             <- [todo]
    │   │   │   │   ├──0x4135ed HanaTexture_CreateFromFile                                      <- [todo]
    │   │   │   ├──0x44c028 UpdateCameraFollow_44C028                                           <- [todo]
    │   │   │   ├──0x446a38 UpdateFoeAI_446A38                                                  <- [todo]
    │   │   │   │   ├──0x4460ad HanaGrid_FloodFill                                              <- [todo]
    │   │   │   │   │   └──0x445c12 HanaGrid_DFSPath                                            <- [todo]
    │   │   │   │   ├──0x45804c StepFoeSpin_45804C                                              <- [todo]
    │   │   │   │   ├──0x457ee0 StepFoeTurn_457EE0                                              <- [todo]
    │   │   │   ├──0x44474e UpdateHeroBody_44474E                                               <- [todo]
    │   │   │   ├──0x4539d1 WanaAttack_Charged_4539D1                                           <- [todo]
    │   │   │   ├──0x4573cf ShowMouseSelectDialog_4573CF                                        <- [todo]
    │   │   │   ├──0x443dd5 BuildMapSubFrames_443DD5                                            <- [todo]
    │   │   │   │   ├──0x407e5b Frame_ComputeWorldAABB                                          <- [todo]
    │   │   │   ├──0x4535de WanaAttack_Close_4535DE                                             <- [todo]
    │   │   │   ├──0x455be2 PlayResultsCutscene_455BE2                                          <- [todo]
    │   │   │   │   ├──0x455b0e HanaSurface_CreateAndFade                                       <- [todo]
    │   │   │   ├──0x448c72 BindSceneActors_448C72                                              <- [todo]
    │   │   │   ├──0x4536f6 WanaAttack_Knockback_4536F6                                         <- [todo]
    │   │   │   ├──0x448b78 IsFoeWithinHeroRange_448B78                                         <- [todo]
    │   │   ├──0x477f90 PlayOdsDialogueScene_477F90                                             <- [PTR GameMainLoop_47F013.cpp]
    │   │   │   ├──0x41a039 Surface_AlphaBlendRect                                              <- [todo]
    │   │   │   │   └──0x4196fd Surface_LockAndGetPointer                                       <- [todo]
    │   │   ├──0x4791a3 PlayVideoWith3DAudio_4791A3                                             <- [HOOK+PTR GameMainLoop_47F013.cpp]
    │   │   │   ├──0x43a863 AviWndSetVolume_43A863                                              <- [todo]
    │   │   │   ├──0x43a889 AviGetVolume_43A889                                                 <- [todo]
    │   │   │   ├──0x43a6fa CheckIsVideoEndAndFlushMsg_43A6FA                                   <- [todo]
    │   │   │   ├──0x43a6f0 GetAviWnd                                                           <- [todo]
    │   │   │   ├──0x43a7d5 AviWnd_Pause                                                        <- [todo]
    │   │   │   ├──0x43a7f8 AviWnd_Destroy                                                      <- [todo]
    │   │   │   └──0x43a95b CreateAviWnd_43A95B                                                 <- [todo]
    │   │   │       ├──0x43a8b0 IsFileExists_43A8B0                                             <- [todo]
    │   │   │       ├──0x49dfc2 VideoForWindowsVersion                                          <- [todo]
    │   │   │       └──0x49dfbc MCIWndCreateA                                                   <- [todo]
    │   │   ├──0x45bcae PlayExploring_45BCAE                                                    <- [PTR GameMainLoop_47F013.cpp]
    │   │   │   ├──0x459f81 SetupItemBodies_459F81                                              <- [todo]
    │   │   │   ├──0x45a513 UpdateHeroControl_45A513                                            <- [todo]
    │   │   │   ├──0x459ca2 GetSceneEventTime_459CA2                                            <- [todo]
    │   │   │   ├──0x45a1ad FindTargetInRange_45A1AD                                            <- [todo]
    │   │   │   ├──0x44bc36 UpdateHudGauge_44BC36                                               <- [todo]
    │   │   │   ├──0x45a2ba ApplyTargetEffect_45A2BA                                            <- [todo]
    │   │   │   ├──0x45953b UpdateLockonEvent_45953B                                            <- [todo]
    │   │   │   ├──0x45b5bd PrepareTargetSelect_45B5BD                                          <- [todo]
    │   │   │   ├──0x4593e0 SetupHeroEntry_4593E0                                               <- [todo]
    │   │   │   ├──0x459768 CheckSceneTransition_459768                                         <- [todo]
    │   │   │   └──0x45b2fb DrawSpriteListEcho_45B2FB                                           <- [todo]
    │   │   ├──0x4795ce ShowBackdropAndWait_4795CE                                              <- [PTR GameMainLoop_47F013.cpp]
    │   │   ├──0x4799e2 InGameMenu_4799E2                                                       <- [HOOK+PTR InGameMenu_4799E2.cpp]
    │   │   ├──0x47eeec ReleaseSoundEffects                                                     <- [HOOK+PTR LoadSoundEffects_47EB7F.cpp]
    │   │   └──0x47eb7f LoadSoundEffects_47EB7F                                                 <- [HOOK+PTR LoadSoundEffects_47EB7F.cpp]
    ├──0x4a2d6b heap_init_crt                                                                   <- [todo]
    ├──0x4a2a7d crtGetEnvironmentStringsA                                                       <- [todo]
```

## Stats

- tree functions:      446
- hooked:              222 (HOOK or HOOK+PTR)
- pinned:              252 (PTR or HOOK+PTR)
- fully covered:       218 (HOOK+PTR)
- implemented only:    1
- untouched:           189

## Per-file status (scan_cpp.py)

### hnhmods/src/CDROM_VerifyDiscSignature_485C50.cpp

```
SUMMARY
structs:              0
  globals:              7 (6 func-ptr imports)
  functions:            7 (6 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   6/6
  hnh_Hook-registered:  6/6
  func-ptrs declared:   6/6
  func-ptrs w/o hook:   0/6
```

### hnhmods/src/CHanaObj__Free_40C1E4.cpp

```
SUMMARY
structs:              0
  globals:              57 (37 func-ptr imports)
  functions:            34 (33 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   33/33
  hnh_Hook-registered:  33/33
  func-ptrs declared:   33/33
  func-ptrs w/o hook:   4/37
  no hook function for: CHanaObj__FreeT3FB, CHanaObj__FreeT3FC, CHanaObj__FreeT3FD, CHanaObj__FreeT3FE
```

### hnhmods/src/CHanaObj__NewFrameT3EC_S354.cpp

```
SUMMARY
structs:              0
  globals:              7 (7 func-ptr imports)
  functions:            8 (7 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   6/7
  hnh_Hook-registered:  7/7
  func-ptrs declared:   3/6
  func-ptrs w/o hook:   3/7
  no hook function for: CHanaObj__NewFrameT3EC_S354, CHanaObj__NewFrameRootT3EC_S2D4, CHanaObj__NewMateT3EE_S184
  missing DebugLog:     hook_MtxStackRecursiveMulLocal_4075B0
```

### hnhmods/src/CheckInstalledDirRoot_4869A7.cpp

```
SUMMARY
structs:              0
  globals:              14 (3 func-ptr imports)
  functions:            7 (6 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   3/6
  hnh_Hook-registered:  3/6
  func-ptrs declared:   3/3
  func-ptrs w/o hook:   0/3
  missing DebugLog:     DriveIsScanTarget, MarkerFileMatches, FindMarkerDrive
  not registered:       DriveIsScanTarget, MarkerFileMatches, FindMarkerDrive
```

### hnhmods/src/CheckRunGame_4863C8.cpp

```
SUMMARY
structs:              0
  globals:              50 (14 func-ptr imports)
  functions:            15 (14 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   14/14
  hnh_Hook-registered:  14/14
  func-ptrs declared:   14/14
  func-ptrs w/o hook:   0/14
```

### hnhmods/src/DSErrToString_423CE7.cpp

```
SUMMARY
structs:              0
  globals:              2 (2 func-ptr imports)
  functions:            3 (2 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   2/2
  hnh_Hook-registered:  2/2
  func-ptrs declared:   2/2
  func-ptrs w/o hook:   0/2
```

### hnhmods/src/dx7.cpp

```
SUMMARY
structs:              0
  globals:              35 (34 func-ptr imports)
  functions:            15 (14 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   11/14
  hnh_Hook-registered:  11/14
  func-ptrs declared:   11/11
  func-ptrs w/o hook:   23/34
  no hook function for: D3DXUninitialize, D3DXInitialize, D3DXGetDeviceCount, D3DXGetMaxNumVideoModes, D3DXGetDeviceDescription, D3DXGetVideoMode, D3DXGetDeviceCaps, D3DXCreateContextEx, D3DXCreateContext, D3DXGetSurfaceFormat, D3DXGetMaxSurfaceFormats, D3DXCreateMatrixStack, D3DXCreateTexture, D3DXCheckTextureRequirements, D3DXLoadTextureFromSurface, D3DXDrawSpriteSimple, D3DXDrawSprite3D, D3DXQuaternionSlerp, D3DXMatrixRotationQuaternion, D3DXMatrixTranslation, D3DXMatrixScaling, D3DXVec3TransformCoord, D3DXVec3TransformNormal
  missing DebugLog:     hnh_HookOn, hnh_HookOff, hnh_Hook
  not registered:       hnh_HookOn, hnh_HookOff, hnh_Hook
```

### hnhmods/src/GameMainLoop_47F013.cpp

```
SUMMARY
structs:              0
  globals:              28 (15 func-ptr imports)
  functions:            11 (10 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   10/10
  hnh_Hook-registered:  10/10
  func-ptrs declared:   10/10
  func-ptrs w/o hook:   5/15
  no hook function for: PlayOdsDialogueScene_477F90, PlayMapBattle_45BCAE, RunDuelBattle_44C38D, RunPracticeScene_4703AE, ShowBackdropAndWait_4795CE
```

### hnhmods/src/InGameMenu_4799E2.cpp

```
SUMMARY
structs:              0
  globals:              25 (22 func-ptr imports)
  functions:            21 (20 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   19/20
  hnh_Hook-registered:  18/20
  func-ptrs declared:   19/19
  func-ptrs w/o hook:   3/22
  no hook function for: SystemMenu_4655AB, MenuMusicSelect, MenuGallery
  missing DebugLog:     ApplyMenuBgmVolume
  not registered:       ApplyMenuBgmVolume, hook_HanaVert_DrawBatch
```

### hnhmods/src/IsFileExists_47E8EE.cpp

```
SUMMARY
structs:              0
  globals:              0 (0 func-ptr imports)
  functions:            0 (0 implemented + 0 registrar)
  DebugLog [HNH:SUB]:   0/0
  hnh_Hook-registered:  0/0
  func-ptrs declared:   0/0
```

### hnhmods/src/LoadSoundEffects_47EB7F.cpp

```
SUMMARY
structs:              0
  globals:              5 (4 func-ptr imports)
  functions:            5 (4 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   4/4
  hnh_Hook-registered:  4/4
  func-ptrs declared:   4/4
  func-ptrs w/o hook:   0/4
```

### hnhmods/src/MemAlloc_40DEF7.cpp

```
SUMMARY
structs:              0
  globals:              14 (9 func-ptr imports)
  functions:            9 (8 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   6/8
  hnh_Hook-registered:  8/8
  func-ptrs declared:   6/6
  func-ptrs w/o hook:   1/9
  no hook function for: D3DXGetErrorString
  missing DebugLog:     hook_D3DXDprintf, hook_PrintDebugError
```

### hnhmods/src/MenuCharSelect.cpp

```
SUMMARY
structs:              0
  globals:              4 (4 func-ptr imports)
  functions:            2 (1 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   1/1
  hnh_Hook-registered:  1/1
  func-ptrs declared:   1/1
  func-ptrs w/o hook:   3/4
  no hook function for: IsMenuKeyPressed_481170, Surface_SetColorKeyRGB, PlayAssault_4703AE
```

### hnhmods/src/MtxStackRecursiveMulLocal.cpp

```
SUMMARY
structs:              0
  globals:              0 (0 func-ptr imports)
  functions:            1 (0 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   0/0
  hnh_Hook-registered:  0/0
  func-ptrs declared:   0/0
```

### hnhmods/src/ParseODFContent_441A5D.cpp

```
SUMMARY
structs:              0
  globals:              31 (31 func-ptr imports)
  functions:            19 (18 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   18/18
  hnh_Hook-registered:  18/18
  func-ptrs declared:   18/18
  func-ptrs w/o hook:   13/31
  no hook function for: InterpolateHanaAnimFrame, CHanaAnimSetT3F1__SetTime_403D61, CHanaObj__NewMeshT3F5, ParseVERT_440288, HanaMesh_AddVertexReference, CHanaObj__NewMorpT457, HanaMorp_AddVertexRef, HanaMorp_InitSubObject, HanaFrame_FindByName, CHanaObj__NewFrameArrayT3EDFromChildT3EC, CHanaObj__GetFramesCountT3ED_408E75, CHanaObj__GetFrameAtIndexT3ED, MtxMult_4069DD
```

### hnhmods/src/ParseODS_483932.cpp

```
SUMMARY
structs:              0
  globals:              7 (7 func-ptr imports)
  functions:            3 (2 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   2/2
  hnh_Hook-registered:  2/2
  func-ptrs declared:   2/2
  func-ptrs w/o hook:   5/7
  no hook function for: ParseODFFile_441F19, ParseSekFile_401039, CSekBody__PlayAnim_40179D, FindFrameByName_47E999, ParseOds3DON_483704
```

### hnhmods/src/PlayScreenFade.cpp

```
SUMMARY
structs:              0
  globals:              39 (32 func-ptr imports)
  functions:            33 (32 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   32/32
  hnh_Hook-registered:  32/32
  func-ptrs declared:   32/32
  func-ptrs w/o hook:   0/32
```

### hnhmods/src/ProcessFrame_40600C.cpp

```
SUMMARY
structs:              0
  globals:              22 (19 func-ptr imports)
  functions:            22 (21 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   18/21
  hnh_Hook-registered:  18/21
  func-ptrs declared:   18/18
  func-ptrs w/o hook:   1/19
  no hook function for: HanaT3FB_Draw
  missing DebugLog:     VertNeedsAlphaPass, FixAlphaAndAddToPool, AlwaysTrue
  not registered:       VertNeedsAlphaPass, FixAlphaAndAddToPool, AlwaysTrue
```

### hnhmods/src/SetFogEnable_40DFB0.cpp

```
SUMMARY
structs:              0
  globals:              10 (4 func-ptr imports)
  functions:            5 (4 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   4/4
  hnh_Hook-registered:  4/4
  func-ptrs declared:   4/4
  func-ptrs w/o hook:   0/4
```

### hnhmods/src/ShowTitleMenu.cpp

```
SUMMARY
structs:              0
  globals:              25 (5 func-ptr imports)
  functions:            6 (5 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   5/5
  hnh_Hook-registered:  5/5
  func-ptrs declared:   5/5
  func-ptrs w/o hook:   0/5
```

### hnhmods/src/StopAndRestartThread.cpp

```
SUMMARY
structs:              0
  globals:              25 (6 func-ptr imports)
  functions:            9 (8 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   8/8
  hnh_Hook-registered:  8/8
  func-ptrs declared:   6/8
  func-ptrs w/o hook:   0/6
```

### hnhmods/src/struct.cpp

```
SUMMARY
structs:              0
  globals:              0 (0 func-ptr imports)
  functions:            1 (1 implemented + 0 registrar)
  DebugLog [HNH:SUB]:   0/1
  hnh_Hook-registered:  0/1
  func-ptrs declared:   0/0
  missing DebugLog:     AssertHanaStructSize
  not registered:       AssertHanaStructSize
```

### hnhmods/src/WinMain_485CE0.cpp

```
SUMMARY
structs:              0
  globals:              10 (2 func-ptr imports)
  functions:            4 (3 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   3/3
  hnh_Hook-registered:  3/3
  func-ptrs declared:   2/3
  func-ptrs w/o hook:   0/2
```

### hnhmods/src/WndProcCheckSystem_485FC2.cpp

```
SUMMARY
structs:              0
  globals:              62 (13 func-ptr imports)
  functions:            17 (16 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   13/16
  hnh_Hook-registered:  13/16
  func-ptrs declared:   13/13
  func-ptrs w/o hook:   0/13
  missing DebugLog:     QuitWindow, OnCheckSystem, OnEngineInit
  not registered:       QuitWindow, OnCheckSystem, OnEngineInit
```

### hnhmods/src/WndProcTitleMenu.cpp

```
SUMMARY
structs:              0
  globals:              18 (7 func-ptr imports)
  functions:            29 (28 implemented + 1 registrar)
  DebugLog [HNH:SUB]:   7/28
  hnh_Hook-registered:  8/28
  func-ptrs declared:   6/7
  func-ptrs w/o hook:   0/7
  missing DebugLog:     PtInBox, PollLMouseButton, FreeTitleMenuBitmaps, BlitBmp, BlitBmpPart, DrawTip, StartGame, PickDefaultRenderHALMode, ClampConfigSelections, UpdateConfigScreenInput, UpdateMainMenuInput, TitleMenu_OnTimer, PaintConfigHotButtons, PaintConfigToggleOverlays, PaintConfigInfoText, PaintConfigScreen, PaintMainMenu, TitleMenu_OnPaint, TitleMenu_OnCreate, TitleMenu_OnDestroy, hook_CreatePaletteFromBuffer
  not registered:       PtInBox, PollLMouseButton, FreeTitleMenuBitmaps, BlitBmp, BlitBmpPart, DrawTip, StartGame, PickDefaultRenderHALMode, ClampConfigSelections, UpdateConfigScreenInput, UpdateMainMenuInput, TitleMenu_OnTimer, PaintConfigHotButtons, PaintConfigToggleOverlays, PaintConfigInfoText, PaintConfigScreen, PaintMainMenu, TitleMenu_OnPaint, TitleMenu_OnCreate, TitleMenu_OnDestroy
```

