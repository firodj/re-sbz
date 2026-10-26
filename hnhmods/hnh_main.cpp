#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>

#include <pybind11/embed.h> // Essential header for embedding
namespace py = pybind11;

#include "console.hpp"
#include "globals.hpp"
#include "menu.hpp"
#include "hnh_header.hpp"
#include "hnh_intern.hpp"
#include "util.hpp"
#include "user32hook.hpp"

// WinMain
typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;

static WinMainFn origWinMain;

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

// WndProc
static WNDPROC origWndProc;

LRESULT CALLBACK hookWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	DebugLog("[HNH] WndProc hWnd=0x%x Msg=0x%x\n", hWnd, Msg);
	return origWndProc(hWnd, Msg, wParam, lParam);
}

LPVOID GetTargetEntryPoint() {
	// Passing NULL retrieves the base address of the main executable (.exe)
	HMODULE hModule = GetModuleHandleA(NULL);
	if (!hModule) return nullptr;

	// Cast the base address to access PE structures
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;

	// Check for valid DOS signature ('MZ')
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;

	// Locate the NT headers
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	// Check for valid NT signature ('PE')
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) return nullptr;

	// Calculate absolute Virtual Address: Base Address + AddressOfEntryPoint
	DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
	return (LPVOID)((BYTE*)hModule + entryPointRVA);
}

bool CheckIsHNH() {
	// Check if executable is: HaNaHiMe.exe
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[HNH] Found Base Module: %p\n", base);

	if (_stricmp(base, "HaNaHiMe.exe") == 0) {
		DebugLog("[HNH] Detected HaNaHiMe. Applying HNH-specific hooks.\n");
		return true;
	}

	return false;
}

void register_WinMain_485CE0();
void register_WndProcCheckSystem_485FC2();
void register_CheckInstalledDirRoot_4869A7();
void register_MemAlloc_40DEF7();
void register_SetFogEnable_40DFB0();
void register_IsFileExists_47E8EE();
void register_DSErrToString_423CE7();
void AssertHanaStructSize();
void register_CHanaObj__Free_40C1E4();
void register_MatrixLookAtLH_40C64A();
void register_CheckRunGame_4863C8();
void register_CDROM_VerifyDiscSignature_485C50();
void register_ShowTitleMenu_427DA8();
void register_CHanaObj__NewFrameT3EC_S354_404720();
void register_WndProcTitleMenu_428C11();
void register_GameMainLoop_47F013();
void register_StopAndRestartThread_481AE4();
void register_LoadSoundEffects_47EB7F();

HANDLE g_hStartEvent = NULL;

void InitHNH() {
	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	MH_STATUS res;

	LPVOID pfnWinMain = (LPVOID)0x485CE0;
	res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
	if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
	DebugLog("[HNH] Hooked WinMain; %s\n", MH_StatusToString(res));

	user32hook::Init();

	//LPVOID pfnWndProc = (LPVOID)0x428c11;
	//res = MH_CreateHook(pfnWndProc, reinterpret_cast<LPVOID>(hookWndProc), reinterpret_cast<LPVOID*>(&origWndProc));
	//if (res == MH_OK) res = MH_EnableHook(pfnWndProc);
	//DebugLog("[HNH] Hooked WndProc; %s\n", MH_StatusToString(res));

	AssertHanaStructSize();
	
	register_MatrixLookAtLH_40C64A();
	register_WndProcCheckSystem_485FC2();
	register_CDROM_VerifyDiscSignature_485C50();
	register_CheckInstalledDirRoot_4869A7();
	register_MemAlloc_40DEF7();
	register_SetFogEnable_40DFB0();
	register_IsFileExists_47E8EE();
	register_DSErrToString_423CE7();
	register_CHanaObj__Free_40C1E4();
	register_CheckRunGame_4863C8();
	register_ShowTitleMenu_427DA8();
	register_CHanaObj__NewFrameT3EC_S354_404720();
	register_WndProcTitleMenu_428C11();
	register_GameMainLoop_47F013();
	register_StopAndRestartThread_481AE4();
	register_LoadSoundEffects_47EB7F();

	DebugLog("[HNH] All hooking done !\n");
	if (g_hStartEvent)
	{
		// Signal the event, waking up the main thread
		SetEvent(g_hStartEvent);
	}
}

void FinishHNH() {
	
}

void DrawHNH(const char *window_name) {
	static bool isOpen = true;
	// Window flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(window_name, &isOpen, flags)) {

		ImGui::Text("Press <INSERT> to toggle menu.");
	}
	ImGui::End();
}

//////////////////

std::string getModuleDir() {
	char buffer[MAX_PATH];
	// Pass NULL to get the path of the current executable. 
	// If inside a DLL, pass the DLL's HMODULE instead.
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

int __stdcall WinMain_485CE0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[HNH] WinMain ! Wait for Start Event...\n");
	WaitForSingleObject(g_hStartEvent, INFINITE);
	DebugLog("[HNH] WinMain continue ...\n");

	py::scoped_interpreter guard{};

	InitializePythonInterpreter();

	int ret = WinMain_485CE0(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	FinishHNH();

	return ret;
}

