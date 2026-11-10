#include "pch.h"
#include "hnh_intern.hpp"
#include "console.hpp"

void DirectHookHNH() {
	AssertHanaStructSize();

	register_WinMain_485CE0(1);
	register_D3DXMatrixIdentity_40E450(1);
	register_CDROM_VerifyDiscSignature_485C50(1);
	register_DSErrToString_423CE7(1);
	register_RestartInputThread_481AE4(1);

	register_ParseODFContent_441A5D(1);
	register_ParseODS_483932(1);
	register_ParseExecStr_487D89(1);

	register_WndProcCheckSystem_485FC2(1);
	register_CheckInstalledDirRoot_4869A7(1);
	register_MemAlloc_40DEF7(1);
	register_SetFogEnable_40DFB0(1);
	register_CHanaObj__Free_40C1E4(1);
	register_CheckRunGame_4863C8(1);
	register_InGameMenu_4799E2(1);
	register_MenuCharSelect_47A324(1);
	register_PlayScreenFade_4874DE(1);
	register_ShowTitleMenu_427DA8(1); 
	register_CHanaObj__NewFrameT3EC_S354_404720(1);
	register_WndProcTitleMenu_428C11(1);
	register_LoadSoundEffects_47EB7F(1);

	register_GameMainLoop_47F013(1);
	register_PlayExploring_45BCAE(1);

	register_ProcessFrame_40600C(1);
	register_SystemMenu_4655AB(1);
	register_PlayOdsDialogueScene_477F90(1);
	register_RunModeSelectScreen(1);
	register_SetupHeroEntry_4593E0(1);
	register_PlayHunting_44C38D(2);
	register_PlayAssault_4703AE(1);
	register_PlayCutInScene_46A76F(1);
	register_PlayTugOfWar_45460A(2);
	register_PlayVideoWith3DAudio_4791A3(1);

	DebugLog("[HNH] All hooking done !\n");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	console::Alloc();
	DirectHookHNH();
  MessageBoxA(NULL, "Hello World from ANSI WinMain!", "Simple Win32 ANSI", MB_OK | MB_ICONINFORMATION);
	
	console::Free();
  return 0;
}