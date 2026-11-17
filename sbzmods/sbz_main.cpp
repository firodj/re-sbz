#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>
#include <cinttypes>

#include "console.hpp"
#include "globals.hpp"
#include "util.hpp"
#include "sbz_header.hpp"
#include "sbz_intern.hpp"

typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;

static WinMainFn origWinMain;

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);


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

bool CheckIsSBZ() {
	// Check if executable is: Sexy Beach Zero English.exe
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[DLLMain] Found Base Module: %p\n", base);

	if (_stricmp(base, "Sexy Beach Zero English.exe") == 0) {
		return true;
	}

	return false;
}
namespace sbz {
	void register_Pack();
}

void InitSBZ()
{

	origWinMain = (WinMainFn)util::Hook(0x403F90, hookWinMain, "WinMain_403F90");

	sbz::register_Pack();
}


bool SBZCustomRender(const char* window_name)
{
	if (!strcmp("World Viewer", window_name)) {
		static bool isOpen = true;
		// Window flags
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

		if (ImGui::Begin(window_name, &isOpen, flags)) {

			ImGui::Text("Press <INSERT> to toggle menu.");

			ImGui::End();
		}
		return true;
	}
	if (!strcmp("Scene Preview", window_name)) {
		return true;
	}

	return false;
}


//////////////////

std::string getModuleDir()
{
	char buffer[MAX_PATH];
	// Pass NULL to get the path of the current executable. 
	// If inside a DLL, pass the DLL's HMODULE instead.
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}


int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	DebugLog("[DllSBZ] WinMain !\n");

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);


	return ret;
}

