# GOALS

```
0x49f107 WinMainCRTStartup
    ├──0x485ce0 WinMain_485CE0
    │   ├──0x485fc2 WndProcCheckSystem_485FC2
    │   │   ├──0x418620 Renderer_InitializeTrigTablesAndPixelFormat
    │   │   │   ├──0x419bcd SetUiFont_419BCD
    │   │   ├──0x40e0e2 SetFogRange_40E0E2
    │   │   ├──0x4869a7 CheckInstalledDirRoot_4869A7
    │   │   │   └──0x486630 SetupDirPath_486630
    │   │   │       └──0x47e8ee IsFileExists_47E8EE
    │   │   ├──0x4863c8 CheckRunGame_4863C8
    │   │   │   ├──0x427da8 ShowTitleMenu_427DA8
    │   │   │   │   ├──0x4276c0 D3DSetRenderStates_4276C0
    │   │   │   │   ├──0x428927 ShowTitleMenuWnd_428927
    │   │   │   │   │   └──0x428c11 WndProcTitleMenu_428C11
    │   │   │   │   │       ├──0x428a7c GetNthValidDimensionString
    │   │   │   │   │       ├──0x428bc1 DrawTextPairs
    │   │   │   │   │       ├──0x4211ed FreeBmp_4211ED
    │   │   │   │   │       ├──0x420df0 LoadBmp_420DF0
    │   │   │   │   │       │   └──0x421095 CreatePaletteFromBuffer
    │   │   │   │   │       ├──0x486d9a CHanaObj__GetRealPath_486D9A
    │   │   │   │   │       ├──0x42889b UpdateRenderOptionsCheckboxes
    │   │   │   │   │       └──0x42833c GetNthActiveVideoMode
    │   │   │   │   ├──0x427cc9 DisplayConfig_Destroy
    │   │   │   │   │   └──0x40df64 MemFree_40DF64
    │   │   │   │   │       ├──0x40a813 PrintDebugError
    │   │   │   │   ├──0x40a894 D3DInit_40A894
    │   │   │   │   │   ├──0x40af60 D3DInitBuffers_40AF60
    │   │   │   │   │   │   ├──0x40f720 CHanaObj__NewMateT3EE_S184
    │   │   │   │   │   │   │   ├──0x40bfc3 CHanaObj__AddRef_40BFC3
    │   │   │   │   │   │   │   └──0x40def7 MemAlloc_40DEF7
    │   │   │   │   │   │   ├──0x404720 CHanaObj__NewFrameT3EC_S354
    │   │   │   │   │   │   │   ├──0x4055fd AttachChild_4055FD
    │   │   │   │   │   │   │   │   ├──0x4058c3 RemoveChild_4058C3
    │   │   │   │   │   │   │   │   ├──0x4074ef MtxStackStartMultLocal_4074EF
    │   │   │   │   │   │   │   │   │   ├──0x4075b0 MtxStackRecursiveMulLocal_4075B0
    │   │   │   │   │   │   ├──0x408f00 CHanaObj__NewLightT3EF
    │   │   │   │   │   │   │   ├──0x40bab8 AttachHanaObj_40BAB8
    │   │   │   │   │   │   │   │   └──0x40b848 CHanaObj__AddObjItemRef_40B848
    │   │   │   │   │   │   │   ├──0x40929c ApplyLightSettings
    │   │   │   │   │   │   ├──0x407047 CalculateLocalRotationMatrix
    │   │   │   │   │   │   ├──0x404908 CHanaObj__NewFrameRootT3EC_S2D4
    │   │   │   │   │   │   ├──0x40c64a MatrixLookAtLH
    │   │   │   │   │   │   │   ├──0x40e420 Vec3Set
    │   │   │   │   │   │   │   ├──0x40e3c0 Vec3LengthSq
    │   │   │   │   │   │   │   ├──0x40e3f0 Vec3Dot
    │   │   │   │   │   │   ├──0x4070ab MatrixRotateCubeMapFace
    │   │   │   │   │   │   ├──0x40f891 ApplyMaterial
    │   │   │   │   │   │   ├──0x40dd71 SetLightingState
    │   │   │   │   │   │   ├──0x40ddf3 DegreesToRadians
    │   │   │   │   │   │   ├──0x40cb94 MatrixPerspectiveFovLH
    │   │   │   │   │   ├──0x40aefb AllocateDevicesDescArray
    │   │   │   │   │   └──0x40ac7c D3DDeinit
    │   │   │   │   │       ├──0x40c1e4 CHanaObj__Free_40C1E4
    │   │   │   │   │       │   ├──0x410501 CHanaObj__FreeT3FC
    │   │   │   │   │       │   │   ├──0x40bd4d CHanaObj__RemoveFromGlobalRef
    │   │   │   │   │       │   │   └──0x4114df CHanaObj__FreeT3FD
    │   │   │   │   │       │   ├──0x415e05 CHanaObj__FreeTexCoordT3F2
    │   │   │   │   │       │   ├──0x413f07 CHanaObj__FreeSpriteT3F6
    │   │   │   │   │       │   ├──0x417a0c CHanaObj__FreeVertT3EA
    │   │   │   │   │       │   ├──0x408e8d CHanaObj__FreeFrameArrayT3ED
    │   │   │   │   │       │   ├──0x433e8d CHanaObj__FreeMorpT457
    │   │   │   │   │       │   ├──0x409820 CHanaObj__FreeLightAnimT3F9
    │   │   │   │   │       │   │   └──0x40922e SetupLightFromD3D
    │   │   │   │   │       │   ├──0x4054b3 CHanaObj__FreeFrameT3EC
    │   │   │   │   │       │   │   ├──0x405421 Frame_FreeAttachedLight
    │   │   │   │   │       │   │   ├──0x40536d Frame_FreeAttachedObject
    │   │   │   │   │       │   ├──0x4166b5 CHanaObj__FreeT3FB
    │   │   │   │   │       │   ├──0x40e6b6 CHanaObj__FreeMateAnimT3F7
    │   │   │   │   │       │   ├──0x409437 CHanaObj__FreeLightT3EF
    │   │   │   │   │       │   ├──0x40a3bb CHanaObj__FreeLightAnimSetT3FA
    │   │   │   │   │       │   ├──0x40fdcb CHanaObj__FreeMeshT3F5
    │   │   │   │   │       │   ├──0x401f53 CHanaObj__FreeAnimT3F0
    │   │   │   │   │       │   │   └──0x401edf UnlinkHanaAnimFromFrame
    │   │   │   │   │       │   ├──0x403a55 CHanaObj__FreeAnimSetT3F1
    │   │   │   │   │       │   │   ├──0x422c72 FreeSoundStruct_422C72
    │   │   │   │   │       │   ├──0x416256 CHanaObj__FreeTextureT3EB
    │   │   │   │   │       │   ├──0x41595b CHanaObj__FreeTexAnimSetT3F4
    │   │   │   │   │       │   ├──0x40fc5e CHanaObj__FreeMateT3EE
    │   │   │   │   │       │   ├──0x414f62 CHanaObj__FreeTexAnimT3F3
    │   │   │   │   │       │   ├──0x40f3e5 CHanaObj__FreeMateAnimSetT3F8
    │   │   │   │   │       │   ├──0x413075 CHanaObj__FreeT3FE
    │   │   │   │   │       │   └──0x4821fa CHanaObj__FreeSceneT458
    │   │   │   │   │       │       └──0x4012fe CSekBody__Release_4012FE
    │   │   │   │   │       │           └──0x40d8f6 FreeSekHeader
    │   │   │   │   └──0x4278ff DisplayConfig_Init
    │   │   │   │       ├──0x427f8c DisplayConfig_ProcessDriverDesc
    │   │   │   │       ├──0x427c51 IsVideoModeSupported
    │   │   │   ├──0x47e7f2 WriteBufferToFile
    │   │   │   ├──0x47e858 ReadFileToBuffer
    │   │   ├──0x40e02d SetFogMode_40E02D
    │   │   ├──0x41888e Renderer_CleanupFont
    │   │   ├──0x485c50 CDROM_VerifyDiscSignature
    │   │   │   ├──0x485ba8 CDROM_VerifyWin9x
    │   │   │   │   ├──0x485ada CDROM_IoctlReadSectorWin9x
    │   │   │   │   │   └──0x485b27 CDROM_DeviceIoControlVWIN32
    │   │   │   └──0x4858d0 CDROM_VerifyNT
    │   │   │       └──0x485a2a CDROM_GetDriveFileSystemType
    │   │   ├──0x422d50 HanaDS_Init
    │   │   │   ├──0x423ccc HanaDS_SetErrStr
    │   │   │   │   └──0x423ce7 DSErrToString_423CE7
    │   │   │   └──0x424886 HanaDS_InitHeap
    │   │   │       └──0x427520 HanaHeap_Create
    │   │   ├──0x422690 HanaDS_Create3DListener
    │   │   ├──0x40dfb0 SetFogEnable_40DFB0
    │   │   ├──0x40e0b4 SetFogColor_40E0B4
    │   │   ├──0x422e14 HanaDS_Shutdown
    │   │   │   └──0x42489d HanaDS_DestroyHeap
    │   │   │       └──0x427551 HanaHeap_Destroy
    │   ├──0x43ad67 DInputInit_43AD67
    │   │   └──0x43af33 DIMouseKeybInit_43AF33
    │   ├──0x47f013 GameMainLoop_47F013
    │   │   ├──0x47df0d MainMenu
    │   │   │   ├──0x47d163 MenuMusicSelect
    │   │   │   │   ├──0x47e680 ConvertFrom1024AsInt
    │   │   │   │   ├──0x40bf01 PresentFrame
    │   │   │   │   │   ├──0x40be03 RecoverLostDevice
    │   │   │   │   │   └──0x40ceec HanaVert_DrawBatch
    │   │   │   │   │       ├──0x439d48 UpdateVertTextureFromFrame
    │   │   │   │   │       │   ├──0x404b80 CalcFrameBound_404B80
    │   │   │   │   │       │   │   ├──0x418544 CHanaObj::LockVtxBufT3EA_418544
    │   │   │   │   │       │   │   ├──0x41858e CHanaObj::UnlockVrxBufT3EA_41858E
    │   │   │   │   │       │   │   ├──0x40ff0f CHanaObj::GetChildCountT3F5_40FF0F
    │   │   │   │   │       │   │   ├──0x40bbf4 CHanaObj::GetType_40BBF4
    │   │   │   │   │       │   │   └──0x40ff1d CHanaObj::GetChildAtT3F5_40FF1D
    │   │   │   │   │       │   ├──0x40e2a5 ProjectToScreen_40E2A5
    │   │   │   │   │       │   │   └──0x40e167 BuildViewportMatrix
    │   │   │   │   │       │   │       └──0x40dcf6 GetProjectionFov_40DCF6
    │   │   │   │   │       │   ├──0x418905 CreateSurfaceWH
    │   │   │   │   │       │   │   └──0x41898e DDErrToString_41898E
    │   │   │   │   │       │   ├──0x417b27 HanaVert_Draw
    │   │   │   │   │       │   │   └──0x40fb30 HanaMate_SetMaterial
    │   │   │   │   │       │   ├──0x43bf6f FlushD3DAndTouchSurface
    │   │   │   │   │       │   ├──0x47e6af ConvertFrom640AsInt
    │   │   │   │   │       │   ├──0x419d16 BltFastWH
    │   │   │   │   │       ├──0x40c54c HanaVert_DrawByType
    │   │   │   │   │       │   ├──0x434fc9 HanaMorpT457_Draw
    │   │   │   │   │       │   │   └──0x40fe8e HanaMeshT3F5_Draw
    │   │   │   │   │       │   ├──0x416776 HanaMesh_Draw
    │   │   │   │   │       ├──0x40ce4e SetLightEnableMask
    │   │   │   │   │       └──0x40d9c7 SortObjectsByDepth
    │   │   │   │   │           └──0x408c3e HanaFrame_GetCenterPosition_408C3E
    │   │   │   │   ├──0x414191 DrawSprite2D
    │   │   │   │   ├──0x45a41b GetMouseBtn_45A41B
    │   │   │   │   │   ├──0x4815ec MutexThreadWait_4815EC
    │   │   │   │   │   └──0x481b37 GetInputState_481B37
    │   │   │   │   ├──0x47e69b ConvertFrom1024AsFloat
    │   │   │   │   ├──0x4655ab SystemMenu_4655AB
    │   │   │   │   │   ├──0x414589 DrawSpriteLTRB
    │   │   │   │   │   ├──0x46380d SysMenu_Graphics_46380D
    │   │   │   │   │   │   ├──0x419b05 DrawTextOnSurfaceSpaced_419B05
    │   │   │   │   │   │   │   └──0x4188b4 GetDC_4188B4
    │   │   │   │   │   │   ├──0x47e6c6 ConvertFrom640AsFloat
    │   │   │   │   │   │   ├──0x423baf DSBufferPlay
    │   │   │   │   │   │   ├──0x4203b3 LoadBmpScaled_4203B3
    │   │   │   │   │   │   │   ├──0x419de5 GetSurfaceFromBmp_419DE5
    │   │   │   │   │   │   │   │   ├──0x421ff4 BMP_Convert24bitTo16bitWithDither
    │   │   │   │   │   │   │   │   │   └──0x421bd5 BMP_DitherRGB24ToRGB565
    │   │   │   │   │   │   │   ├──0x419d89 BltSurface
    │   │   │   │   │   │   │   ├──0x4147ed DrawSpriteAtRect
    │   │   │   │   │   │   │   ├──0x4133d0 CHanaObj__NewSprite3F6byWH
    │   │   │   │   │   │   │   └──0x419a1a GetSurfaceWH
    │   │   │   │   │   │   └──0x414a5d CHanaSpriteT3F6::SetPos_414A5D
    │   │   │   │   │   ├──0x464919 SysMenu_GirlStatusCheck_464919
    │   │   │   │   │   │   ├──0x419a4b DrawTextOnSurface
    │   │   │   │   │   │   ├──0x44af15 IntPow_44AF15
    │   │   │   │   │   ├──0x461a9e ShowYesNoDialog_461A9E
    │   │   │   │   │   │   ├──0x414b09 CHanaSpriteT3F6::SetScale_414B09
    │   │   │   │   │   │   ├──0x414b28 CHanaSpriteT3F6::SetSrcRect_414B28
    │   │   │   │   │   │   ├──0x414aba CHanaObj::SetAlpha3F6
    │   │   │   │   │   │   ├──0x481550 DoPeekMessages
    │   │   │   │   │   │   └──0x414afb CHanaSpriteT3F6::GetAlpha_414AFB
    │   │   │   │   │   ├──0x462135 SysMenu_SaveMenu_462135
    │   │   │   │   │   │   ├──0x46164c ShowFadeDialog_46164C
    │   │   │   │   │   │   ├──0x461557 LoadDataFiles_461557
    │   │   │   │   │   │   │   ├──0x461397 FormatFileLastWriteTime_461397
    │   │   │   │   │   │   │   │   ├──0x4612b0 ZeroPadNumberToString_4612B0
    │   │   │   │   │   ├──0x462851 SysMenu_LoadMenu_462851
    │   │   │   │   │   ├──0x413a5e LoadBmpWithMask_413A5E
    │   │   │   │   │   │   ├──0x43ea05 CreateTextureFromSurfaceWithAlphaMask
    │   │   │   │   │   ├──0x4874de PlayScreenFade
    │   │   │   │   │   │   ├──0x47e782 SetTimerScaleFactor
    │   │   │   │   │   │   ├──0x405c06 HanaFrame_Draw
    │   │   │   │   │   │   │   ├──0x40600c ProcessFrame
    │   │   │   │   │   │   │   │   ├──0x4084c2 AlwaysTrue
    │   │   │   │   │   │   │   │   ├──0x408283 UpdateBoneTransform
    │   │   │   │   │   │   │   │   │   ├──0x406ca4 UpdateBoneLookAtTransform
    │   │   │   │   │   │   │   │   │   └──0x40873e CameraLookAtFrame
    │   │   │   │   │   │   │   │   ├──0x409510 ConfigureLight
    │   │   │   │   │   │   │   │   ├──0x40cd32 AddObjectToPool
    │   │   │   │   │   │   │   ├──0x406ab1 HanaFrame_GetRelPosition
    │   │   │   │   │   │   │   ├──0x4339b2 HanaFrame_PreDraw
    │   │   │   │   │   │   │   │   ├──0x43383a HanaMorp_ApplyTree
    │   │   │   │   │   │   │   │   │   ├──0x433569 HanaMorpT457_Animate
    │   │   │   │   │   │   │   │   │   │   └──0x47e78f GetTimerScaleFactor
    │   │   │   │   │   │   │   │   │   └──0x43315f HanaMorpT457_Interp
    │   │   │   │   │   │   │   │   │       ├──0x432db0 HanaVert_BlendMorph
    │   │   │   │   │   │   │   │   └──0x433916 HanaMorp_ApplySubTree
    │   │   │   │   │   │   │   │       ├──0x4365b9 HanaMorpT457_ApplySubs
    │   │   │   │   │   │   │   │       │   ├──0x4363ee HanaVert_Transform
    │   │   │   │   │   │   │   └──0x40773f HanaFrame_BuildMatrixStack
    │   │   │   │   │   │   │       ├──0x407800 HanaFrame_PushNode
    │   │   │   │   │   │   │       │   └──0x405bb4 SetInversTransf_405BB4
    │   │   │   │   │   │   ├──0x47e79a GetFrameTime
    │   │   │   │   │   │   ├──0x4874a5 RenderFlush_4874A5
    │   │   │   │   │   │   │   ├──0x43f3f7 SelfBltDC
    │   │   │   │   │   │   ├──0x41973f RGBToSurfaceFormat
    │   │   │   │   │   │   ├──0x40bf4f RenderClear_40BF4F
    │   │   │   │   │   │   ├──0x4137d1 CHanaObj::NewSpriteT3F6bySurf
    │   │   │   │   │   │   ├──0x47e7d5 FrameTimer_EndFrame
    │   │   │   │   │   │   ├──0x47e768 FrameTimer_BeginFrame
    │   │   │   │   │   │   └──0x413d78 CHanaObj::ReplaceTexture3F6
    │   │   │   │   │   ├──0x481ae4 StopAndRestartThread
    │   │   │   │   │   │   ├──0x4818fd CreateNewThread
    │   │   │   │   │   │   │   ├──0x4817b1 InputThread_Absolute_4817B1
    │   │   │   │   │   │   │   │   └──0x4811fa GetCursorPos_4811FA
    │   │   │   │   │   │   │   ├──0x481637 InputThread_Relative_481637
    │   │   │   │   │   │   └──0x481a35 TerminateThreadGracefully
    │   │   │   │   │   ├──0x462fe9 SysMenu_Options_462FE9
    │   │   │   │   ├──0x45a4d8 GetMouseXY_45A4D8
    │   │   │   │   ├──0x481170 IsMenuKeyPressed_481170
    │   │   │   │   └──0x422e7b LoadWav_422E7B
    │   │   │   │       ├──0x4248d2 SafeFree
    │   │   │   │       │   └──0x4275df HeapFreeRef
    │   │   │   │       └──0x4248a7 AllocMem
    │   │   │   │           └──0x427591 HeapAllocRef
    │   │   │   ├──0x47a324 MenuCharSelect
    │   │   │   │   ├──0x4703ae PlayAssault_4703AE
    │   │   │   │   │   ├──0x487d89 ParseExecStr_487D89
    │   │   │   │   │   │   ├──0x47f460 Scene_CreateFontTextureFromStringData
    │   │   │   │   │   │   │   ├──0x43bcae DrawText_43BCAE
    │   │   │   │   │   │   │   │   ├──0x4198e0 Surface_SetColorKeyRGB
    │   │   │   │   │   │   │   │   └──0x43bffa CheckSpecialJpChar_43BFFA
    │   │   │   │   │   │   ├──0x47ff01 ParseStrChunkText_47FF01
    │   │   │   │   │   │   ├──0x487d46 FindCharUntil_487D46
    │   │   │   │   │   │   ├──0x487267 Character_ApplyAnimationParamsToChildren
    │   │   │   │   │   │   │   ├──0x405a0a CHanaObj::NewFrameArrayT3EDFromChildT3EC
    │   │   │   │   │   │   │   │   ├──0x408dd0 CHanaObj::NewFrameArrayT3ED
    │   │   │   │   │   │   │   ├──0x408e75 CHanaObj::GetFramesCountT3ED_408E75
    │   │   │   │   │   │   │   └──0x408e1b CHanaObj::GetFrameAtIndexT3ED
    │   │   │   │   │   ├──0x46f37e RunModeSelectScreen
    │   │   │   │   │   │   ├──0x47e999 FindFrameByName_47E999
    │   │   │   │   │   │   ├──0x46a2a0 PauseSceneAudio_46A2A0
    │   │   │   │   │   │   ├──0x467420 ApplyFrameEffectOption
    │   │   │   │   │   │   │   ├──0x46739d Frame_ApplyFadeRecursive
    │   │   │   │   │   │   │   │   └──0x46724e FrameObject_SetAlpha
    │   │   │   │   │   │   ├──0x46d7a7 DrawSpritePulse_46D7A7
    │   │   │   │   │   │   │   ├──0x414a7a CHanaSpriteT3F6::GetPos_414A7A
    │   │   │   │   │   │   │   ├──0x414b1a CHanaSpriteT3F6::GetScale_414B1A
    │   │   │   │   │   │   ├──0x4825d5 UpdateSceneAtTime_4825D5
    │   │   │   │   │   │   │   ├──0x403d61 CHanaAnimSetT3F1::SetTime_403D61
    │   │   │   │   │   │   │   │   ├──0x40e969 ApplyMateAnimMaterial
    │   │   │   │   │   │   │   │   │   └──0x40ef23 FindMateAnimKeyframeBounds
    │   │   │   │   │   │   │   │   ├──0x4248f2 GetSoundBufferAverageLevel
    │   │   │   │   │   │   │   │   ├──0x40283c InterpolateHanaAnimFrame
    │   │   │   │   │   │   │   │   │   ├──0x40344a HanaAnim_FindInterpolationFrame
    │   │   │   │   │   │   │   │   │   ├──0x402d0e CHanaAnimT3F0::BakeKeyframesT3F0_402D0E
    │   │   │   │   │   │   │   │   │   │   ├──0x4035b1 ApplyQTS_4035B1
    │   │   │   │   │   │   │   ├──0x4019e1 CSekBody::UpdateAnim_4019E1
    │   │   │   │   │   │   │   │   ├──0x4018e1 CSekBody::CutToAnim_4018E1
    │   │   │   │   │   │   │   │   ├──0x43a643 CHanaAnimSetT3F1::BlendStep_43A643
    │   │   │   │   │   │   │   │   │   └──0x43a43f HanaAnim_UpdateBoneTransform
    │   │   │   │   │   │   │   │   └──0x43a371 CHanaAnimSetT3F1::BeginBlend_43A371
    │   │   │   │   │   │   │   ├──0x401647 SekBody::UpdateTo2_401647
    │   │   │   │   │   │   │   └──0x4069dd MtxMult_4069DD
    │   │   │   │   │   │   ├──0x467af3 SetFrameVisibilityAndGetHIndex
    │   │   │   │   │   │   └──0x4827fe UpdateSceneTimer_4827FE
    │   │   │   │   │   ├──0x40dca3 SetProjectionFov_40DCA3
    │   │   │   │   │   ├──0x4667a9 InitializePracticeAnimations_4667A9
    │   │   │   │   │   │   ├──0x466738 Frame_SetPracticeAnimRootRecursive
    │   │   │   │   │   ├──0x46777d Scene_FindMapFrame
    │   │   │   │   │   ├──0x480b2d DrawDialogueWindow_480B2D
    │   │   │   │   │   │   ├──0x47fa84 DrawAdvanceDialogueText_47FA84
    │   │   │   │   │   │   ├──0x4803b5 DrawDialogueText_4803B5
    │   │   │   │   │   ├──0x46d9af RunSceneSelectScreen
    │   │   │   │   │   ├──0x4857ca ResetSceneBodies_4857CA
    │   │   │   │   │   ├──0x48564b LoadOdsEx_48564B
    │   │   │   │   │   │   ├──0x484a38 ParseOdsEx_484A38
    │   │   │   │   │   │   │   ├──0x483704 ParseOds3DON_483704
    │   │   │   │   │   │   │   │   ├──0x483650 CHanaFrameT3EC::FindByName_483650
    │   │   │   │   │   │   │   │   └──0x42277e LoadWavToDS3DBuffer_42277E
    │   │   │   │   │   │   │   ├──0x441f19 ParseODFFile_441F19
    │   │   │   │   │   │   │   │   ├──0x403a00 CHanaObj::NewAnimSetT3F1_S150
    │   │   │   │   │   │   │   │   ├──0x4375b3 HanaMorpT457_SetupFunsSubtree
    │   │   │   │   │   │   │   │   │   ├──0x4372ba HanaMorpT457_SetupFuns
    │   │   │   │   │   │   │   │   │   │   ├──0x436300 HanaVert_LerpMorph
    │   │   │   │   │   │   │   │   │   │   ├──0x434fe0 GetT3F5FromObj_434FE0
    │   │   │   │   │   │   │   │   │   │   │   ├──0x40fca0 CHanaObj::NewMeshT3F5
    │   │   │   │   │   │   │   │   │   │   │   ├──0x40fceb HanaMesh_AddVertexReference
    │   │   │   │   │   │   │   │   │   │   ├──0x4363a3 HanaVert_CopyMorph
    │   │   │   │   │   │   │   │   │   │   ├──0x4417eb HanaFrame_FindByName
    │   │   │   │   │   │   │   │   ├──0x441a5d ParseODFContent_441A5D
    │   │   │   │   │   │   │   │   │   ├──0x44106a ParseOdfFRAM_44106A
    │   │   │   │   │   │   │   │   │   │   ├──0x40539d AssignLigthtToFrame_40539D
    │   │   │   │   │   │   │   │   │   ├──0x4414eb ParseOdfANIM_4414EB
    │   │   │   │   │   │   │   │   │   │   ├──0x4039c9 HanaAnim_GetDuration
    │   │   │   │   │   │   │   │   │   │   ├──0x40ba89 HanaFrame_GetFrameName
    │   │   │   │   │   │   │   │   │   │   ├──0x401cd0 CHanaObj::NewAnimT3F0
    │   │   │   │   │   │   │   │   │   │   ├──0x403c15 CHanaAnimSetT3F1::AddAnimT3F0_403C15
    │   │   │   │   │   │   │   │   │   │   └──0x401fbd HanaAnim_LinkFrame
    │   │   │   │   │   │   │   │   │   ├──0x44068c ParseOdfMESH_44068C
    │   │   │   │   │   │   │   │   │   │   ├──0x440288 ParseVERT_440288
    │   │   │   │   │   │   │   │   │   │   │   ├──0x416ce0 CHanaObj::NewVertT3EA
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x417f88 CalculateVertexSize
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43f460 ParseOdfMeshTEXT_43F460
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x418103 HanaVert_SetTexture
    │   │   │   │   │   │   │   │   │   │   │   │   │   └──0x4181b5 HanaVert_ReleaseTexture
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40bc13 FindObjByType_40BC13
    │   │   │   │   │   │   │   │   │   │   │   ├──0x4400c3 GetVerByName_4400C3
    │   │   │   │   │   │   │   │   │   │   │   ├──0x416e65 CHanaObj::FillVertexBufferT3EA
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43fdec ParseOdfMeshVFLG_43FDEC
    │   │   │   │   │   │   │   │   │   │   │   ├──0x43fbd1 ParseOdfMeshMANM_43FBD1
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40e500 CHanaObj::NewMateAnimT3F7
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x40e782 HanaMateAnim_SetMaterial
    │   │   │   │   │   │   │   │   │   │   │   │   ├──0x4044d9 HanaAnimSet_AddMaterialAnimRef
    │   │   │   │   │   │   │   │   │   │   │   │   └──0x40e7da HanaMateAnim_InsertKeyframe
    │   │   │   │   │   │   │   │   │   │   │   └──0x43f8fe ParseOdfMeshMATE_43F8FE
    │   │   │   │   │   │   │   │   │   │   │       ├──0x418221 HanaVert_SetMaterial
    │   │   │   │   │   │   │   │   │   │   │       │   └──0x41828c FreeMate_41828C
    │   │   │   │   │   │   │   │   │   ├──0x440896 ParseOdfMORP_440896
    │   │   │   │   │   │   │   │   │   │   ├──0x439982 HanaMorp_InitSubObject
    │   │   │   │   │   │   │   │   │   │   ├──0x433da7 CHanaObj::NewMorpT457
    │   │   │   │   │   │   │   │   │   │   ├──0x434370 HanaMorp_AddVertexRef
    │   │   │   │   │   │   │   │   │   ├──0x44189c ParseOdf3DON_44189C
    │   │   │   │   │   │   │   │   └──0x441e7e HanaFrame_PropagateFogToChildren
    │   │   │   │   │   │   │   ├──0x40179d CSekBody::PlayAnim_40179D
    │   │   │   │   │   │   │   ├──0x401039 ParseSekFile_401039
    │   │   │   │   │   │   │   │   └──0x40d875 SekHeader_CloneClean
    │   │   │   │   │   │   │   ├──0x483567 ReadStrFile_483567
    │   │   │   │   │   │   └──0x481c5f CHanaObj__NewSceneT458_S504
    │   │   │   │   │   ├──0x4843cf LoadOds_4843CF
    │   │   │   │   │   │   ├──0x483932 ParseOds_483932
    │   │   │   │   │   ├──0x4445dd LoadSekAndOdf_4445DD
    │   │   │   │   │   │   ├──0x4014e4 SekBody::UpdateTo_4014E4
    │   │   │   │   │   ├──0x47e3e1 ToggleWaitScreen
    │   │   │   │   │   │   ├──0x47e1a2 WaitScreenThreadProc
    │   │   │   │   │   ├──0x423c65 DSBufferGetVolume_423C65
    │   │   │   │   │   ├──0x46a76f PlayCutInScene_46A76F
    │   │   │   │   │   │   ├──0x46a503 UpdateSekBodyAnimationMatrix
    │   │   │   │   │   │   │   ├──0x46a3e9 UpdateFrameAnimationMatrix
    │   │   │   │   │   │   │   ├──0x404277 SetAnimSetTime
    │   │   │   │   │   │   │   └──0x401c67 GetSekBodyCurrentAnimTime
    │   │   │   │   │   │   ├──0x47ea86 FindFrameByNameSubstringRecursive
    │   │   │   │   │   │   ├──0x466978 CameraFrame_TrackBetweenObjectsAndLookAt
    │   │   │   │   │   │   ├──0x468da8 DrawFadeTransition_Scene
    │   │   │   │   │   │   │   ├──0x468b99 Scene_ApplyFadeToPracticeFrames
    │   │   │   │   │   │   ├──0x4668ac Scene_ProcessPracticeFrames
    │   │   │   │   │   │   │   └──0x4665f0 Frame_CheckCameraDistanceAndRecurse
    │   │   │   │   │   │   ├──0x4690ab SoundAH_ManageEffectsAndAnimate
    │   │   │   │   │   │   │   ├──0x481be0 HanaMorp_AnimateAndPropagate
    │   │   │   │   │   │   ├──0x4680b4 UpdateCutInSceneBoneTransforms
    │   │   │   │   │   │   ├──0x469eb6 Character_UpdateAnimationState
    │   │   │   │   │   │   │   └──0x48713f HanaObj_CheckFrameCondition
    │   │   │   │   │   │   ├──0x467abe AnimSet_ResetAndTransferAnimations
    │   │   │   │   │   │   │   ├──0x4679b2 TransferAnimSetFrameAnimations_4679B2
    │   │   │   │   │   │   │   │   ├──0x404349 CHanaAnimSetT3F1::RemoveAnimT3F0_404349
    │   │   │   │   │   │   ├──0x468a4a Cursor_ClampToCenterOrSprite
    │   │   │   │   │   │   ├──0x4682d6 MoveCutInSceneFramesWithCameraConstraint
    │   │   │   │   │   │   ├──0x466bda CameraController_UpdateCutscene
    │   │   │   │   │   │   ├──0x46a55d CameraFrame_ProjectFramePairOntoForwardRay
    │   │   │   │   │   │   ├──0x467160 CutInScene_CyclePosAndApplyCameraOffset
    │   │   │   │   │   │   │   ├──0x407945 HanaFrame_GetMatAfter
    │   │   │   │   │   │   │   └──0x40799f HanaMatrix_ApplyTransform
    │   │   │   │   │   │   ├──0x467dfd IsObjectValidRecursive_467DFD
    │   │   │   │   │   │   │   ├──0x467d62 IsParentValid_467D62
    │   │   │   │   │   │   │   │   └──0x467bbd SyncObjectAnimationByName_467BBD
    │   │   │   │   │   │   ├──0x4698e8 SoundSE_ManageEffectsAndPlay
    │   │   │   │   │   │   ├──0x467e72 AnimSet_MatchAndSortKutiRTibusaLFrames
    │   │   │   │   │   │   ├──0x4686f8 UpdateCutInSceneCameraFollow
    │   │   │   ├──0x47ad8d MenuGallery
    │   │   ├──0x44c38d PlayHunting_44C38D
    │   │   │   ├──0x455183 PlayVictoryCutscene_455183
    │   │   │   │   ├──0x458b3a UpdateVoicePlayback_458B3A
    │   │   │   │   ├──0x44b23b DrawRecViewfinder_44B23B
    │   │   │   │   │   ├──0x414344 DrawSprite3D
    │   │   │   │   ├──0x44af5b DrawClockHud_44AF5B
    │   │   │   │   ├──0x407b73 HanaMatrix_BuildLookAtAndTransform
    │   │   │   ├──0x456807 RenderDualBodyScene_456807
    │   │   │   ├──0x45460a PlayCinematicSequence_45460A
    │   │   │   │   ├──0x445525 UpdateCameraFollow_445525
    │   │   │   │   │   ├──0x443cc5 CheckFrameCollision_443CC5
    │   │   │   │   │   │   ├──0x4423b0 HitSupportSurface_4423B0
    │   │   │   │   │   │   │   └──0x407e1f ComputeMeshBounds_407E1F
    │   │   │   │   │   │   └──0x442c1c HitOverheadSurface_442C1C
    │   │   │   │   │   ├──0x4443fc TestMoveCollision_4443FC
    │   │   │   │   │   │   ├──0x443ec4 TestBodyMoveHit_443EC4
    │   │   │   │   ├──0x453fac DrawTransitionFlash_453FAC
    │   │   │   │   ├──0x4532d0 DrawProgressFill_4532D0
    │   │   │   │   ├──0x4542e0 UpdateBodiesInput_4542E0
    │   │   │   │   ├──0x44b97b DrawHudSpriteNumber_44B97B
    │   │   │   ├──0x453b92 WanaAttack_Projectile_453B92
    │   │   │   │   ├──0x4581b0 StepWanaProjectile_4581B0
    │   │   │   ├──0x445b12 BuildMapCollisionGrid_445B12
    │   │   │   ├──0x44451f UpdateCameraLinkedObjects_44451F
    │   │   │   ├──0x45669f DrawFadeOutSpark_45669F
    │   │   │   ├──0x4490a7 InitHudResources_4490A7
    │   │   │   │   ├──0x4135ed HanaTexture_CreateFromFile
    │   │   │   ├──0x44c028 UpdateCameraFollow_44C028
    │   │   │   ├──0x446a38 UpdateFoeAI_446A38
    │   │   │   │   ├──0x4460ad HanaGrid_FloodFill
    │   │   │   │   │   └──0x445c12 HanaGrid_DFSPath
    │   │   │   │   ├──0x45804c StepFoeSpin_45804C
    │   │   │   │   ├──0x457ee0 StepFoeTurn_457EE0
    │   │   │   ├──0x44474e UpdateHeroBody_44474E
    │   │   │   ├──0x4539d1 WanaAttack_Charged_4539D1
    │   │   │   ├──0x4573cf ShowMouseSelectDialog_4573CF
    │   │   │   ├──0x443dd5 BuildMapSubFrames_443DD5
    │   │   │   │   ├──0x407e5b Frame_ComputeWorldAABB
    │   │   │   ├──0x4535de WanaAttack_Close_4535DE
    │   │   │   ├──0x455be2 PlayResultsCutscene_455BE2
    │   │   │   │   ├──0x455b0e HanaSurface_CreateAndFade
    │   │   │   ├──0x448c72 BindSceneActors_448C72
    │   │   │   ├──0x4536f6 WanaAttack_Knockback_4536F6
    │   │   │   ├──0x448b78 IsFoeWithinHeroRange_448B78
    │   │   ├──0x477f90 PlayOdsDialogueScene_477F90
    │   │   │   ├──0x41a039 Surface_AlphaBlendRect
    │   │   │   │   └──0x4196fd Surface_LockAndGetPointer
    │   │   ├──0x4791a3 PlayVideoWith3DAudio_4791A3
    │   │   │   ├──0x43a863 AviWndSetVolume_43A863
    │   │   │   ├──0x43a889 AviGetVolume_43A889
    │   │   │   ├──0x43a6fa CheckIsVideoEndAndFlushMsg_43A6FA
    │   │   │   ├──0x43a6f0 GetAviWnd
    │   │   │   ├──0x43a7d5 AviWnd_Pause
    │   │   │   ├──0x43a7f8 AviWnd_Destroy
    │   │   │   └──0x43a95b CreateAviWnd_43A95B
    │   │   │       ├──0x43a8b0 IsFileExists_43A8B0
    │   │   │       ├──0x49dfc2 VideoForWindowsVersion
    │   │   │       └──0x49dfbc MCIWndCreateA
    │   │   ├──0x45bcae PlayExploring_45BCAE
    │   │   │   ├──0x459f81 SetupItemBodies_459F81
    │   │   │   ├──0x45a513 UpdateHeroControl_45A513
    │   │   │   ├──0x459ca2 GetSceneEventTime_459CA2
    │   │   │   ├──0x45a1ad FindTargetInRange_45A1AD
    │   │   │   ├──0x44bc36 UpdateHudGauge_44BC36
    │   │   │   ├──0x45a2ba ApplyTargetEffect_45A2BA
    │   │   │   ├──0x45953b UpdateLockonEvent_45953B
    │   │   │   ├──0x45b5bd PrepareTargetSelect_45B5BD
    │   │   │   ├──0x4593e0 SetupHeroEntry_4593E0
    │   │   │   ├──0x459768 CheckSceneTransition_459768
    │   │   │   └──0x45b2fb DrawSpriteListEcho_45B2FB
    │   │   ├──0x4795ce ShowBackdropAndWait_4795CE
    │   │   ├──0x4799e2 InGameMenu_4799E2
    │   │   ├──0x47eeec ReleaseSoundEffects
    │   │   └──0x47eb7f LoadSoundEffects_47EB7F
    ├──0x4a2d6b heap_init_crt
    ├──0x4a2a7d crtGetEnvironmentStringsA
```