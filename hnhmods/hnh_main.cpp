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

typedef std::add_pointer_t<void __cdecl(const char*, int)> PlayVideoWith3DAudioFn;
static PlayVideoWith3DAudioFn origPlayVideoWith3DAudio = nullptr;

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

void __cdecl hookPlayVideoWith3DAudio(const char* szFileName, int flagsLoop) {
	DebugLog("[HNH] PlayVideoWith3DAudio %s %d\n", szFileName, flagsLoop);
}

void InitHNH() {
	LPVOID pfnWinMain = (LPVOID)0x485CE0;

	MH_STATUS res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
	if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
	DebugLog("[HNH] Hooked WinMain; %s\n", MH_StatusToString(res));

	user32hook::Init();

	//LPVOID pfnWndProc = (LPVOID)0x428c11;
	//res = MH_CreateHook(pfnWndProc, reinterpret_cast<LPVOID>(hookWndProc), reinterpret_cast<LPVOID*>(&origWndProc));
	//if (res == MH_OK) res = MH_EnableHook(pfnWndProc);
	//DebugLog("[HNH] Hooked WndProc; %s\n", MH_StatusToString(res));

	LPVOID pfnPlayVideoWith3DAudio = (LPVOID)0x4791A3;
	res = MH_CreateHook(pfnPlayVideoWith3DAudio, reinterpret_cast<LPVOID>(hookPlayVideoWith3DAudio), reinterpret_cast<LPVOID*>(&origPlayVideoWith3DAudio));
	if (res == MH_OK) res = MH_EnableHook(pfnPlayVideoWith3DAudio);
	DebugLog("[HNH] Hooked PlayVideoWith3DAudio; %s\n", MH_StatusToString(res));

}

void FinishHNH() {
	
}

void DrawHNH() {
	static bool isOpen = true;
	// Window flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("HNH Menu", &isOpen, flags)) {

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

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[HNH] WinMain !\n");

	py::scoped_interpreter guard{};

	InitializePythonInterpreter();

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	FinishHNH();

	return ret;
}

