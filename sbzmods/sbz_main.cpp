#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>

#include <pybind11/embed.h> // Essential header for embedding
namespace py = pybind11;

#include "console.hpp"
#include "globals.hpp"
#include "sbz_header.hpp"
#include "sbz_intern.hpp"

typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;

static WinMainFn origWinMain;

char __fastcall hookCPack__GetSubFileFromPP_70C9C0(
	sbz::CPack* pThis,
	void* edx,
	sbz::CString* pstrFileName,
	sbz::CPackFormatSecret* pPackFmtSec,
	char nType,
	sbz::CString* pstrSubName,
	LPBYTE* pData,
	DWORD* pSize);

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

static sbz::CPack__GetSubFileFromPP_70C9C0Fn origCPack__GetSubFileFromPP_70C9C0;



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

void InitSBZ() {

	LPVOID pfnWinMain = (LPVOID)0x403F90;
	MH_STATUS res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
	if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
	DebugLog("[DllMain] Hooked WinMain with status %d\n", res);

	LPVOID pfnCPack__GetSubFileFromPP_70C9C0 = (LPVOID)0x70C9C0;
	res = MH_CreateHook(pfnCPack__GetSubFileFromPP_70C9C0, reinterpret_cast<LPVOID>(hookCPack__GetSubFileFromPP_70C9C0),
		reinterpret_cast<LPVOID*>(&origCPack__GetSubFileFromPP_70C9C0));
	if (res == MH_OK) res = MH_EnableHook(pfnCPack__GetSubFileFromPP_70C9C0);
	DebugLog("[DllMain] Hooked CPack::GetSubFileFromPP_70C9C0 with status %d\n", res);

}

void FinishSBZ() {
	
}

void DrawSBZ() {
	static bool isOpen = true;
	// Window flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

	ImGui::Begin("SBZ Menu", &isOpen, flags);

	ImGui::Text("Press <INSERT> to toggle menu.");

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

std::string getPPFolder(std::string path_str) {
	size_t last_slash_idx = path_str.find_last_of("\\/");

	std::string filename;
	std::string path;
	if (std::string::npos != last_slash_idx) {
		filename = path_str.substr(last_slash_idx + 1);
		path = path_str.substr(0, last_slash_idx + 1);
	}
	else {
		return {};
	}

	std::string ext;
	size_t last_dot_idx = filename.find_last_of(".");
	if (std::string::npos != last_dot_idx) {
		ext = filename.substr(last_dot_idx + 1);
		if (ext != "pp") {
			return {};
		}

		filename = filename.substr(0, last_dot_idx);
	}
	else {
		return {};
	}

	return path + filename;
}

namespace sbz {
	LPVOID __cdecl MemAllocate_719FA0(SIZE_T dwBytes) {
		sbz::MemAllocate_719FA0Fn pfn = (sbz::MemAllocate_719FA0Fn)0x719FA0;
		return pfn(dwBytes);
	}

}

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[DllSBZ] WinMain !\n");

	py::scoped_interpreter guard{};

	InitializePythonInterpreter();

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	FinishSBZ();

	return ret;
}

char __fastcall hookCPack__GetSubFileFromPP_70C9C0(
	sbz::CPack* pThis,
	void* edx,
	sbz::CString* pstrFileName,
	sbz::CPackFormatSecret* pPackFmtSec,
	char nType,
	sbz::CString* pstrSubName,
	LPBYTE* ppData,
	DWORD* pSize)
{
	auto ppdir_str = getPPFolder(pstrFileName->pszData);

	//DebugLog("[CPack] fileName = %s, subFile = %s, type = %x, type(2) = %x, packmode = %x, getPPFolder = %s\n", 
	//	pstrFileName->pszData, pstrSubName->pszData, nType, pPackFmtSec->nType_D524, pPackFmtSec->packMode_D500, ppdir_str.c_str());

	//// ensure subpath only filename

	auto subpath_str = std::string{ pstrSubName->pszData };

	size_t last_slash_idx = subpath_str.find_last_of("\\/");

	std::string filename;
	if (std::string::npos != last_slash_idx) {
		filename = subpath_str.substr(last_slash_idx + 1);
	}
	else {
		filename = subpath_str;
	}

	filename = ppdir_str + "/" + filename;

	////
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);

		// 1a. Check if the file exists / opened successfully
		if (!file.is_open()) {
			DebugLog("[CPack] File does not exist %s, fallback\n", filename.c_str());
		}
		else {
			// 2. Check file size
			// Since we opened it with std::ios::ate, tellg() gives us the current position (the end)
			std::streamsize file_size = file.tellg();
			DebugLog("[CPack] File %s, size: %d\n", filename.c_str(), file_size);

			// 3. Read file into buffer
			// Move the file pointer back to the beginning to start reading
			file.seekg(0, std::ios::beg);

			char* buffer = (char*)sbz::MemAllocate_719FA0(file_size);
			if (buffer && file.read(buffer, file_size)) {
				if (ppData)
					*ppData = (LPBYTE)buffer;
				if (pSize)
					*pSize = file_size;
				return 1;
			} else {
				DebugLog("[CPack] Error: Could not read the entire file.\n");
			}
		}
	}

	globals::IncRefForDisableFileAccessLog();

	DebugLog("[CPack] fileName = %s, subFile = %s, type = %x, packmode = %x\n",
		pstrFileName->pszData, pstrSubName->pszData, nType, pPackFmtSec->packMode_D500);

	char res = origCPack__GetSubFileFromPP_70C9C0(pThis, edx, pstrFileName, pPackFmtSec, nType, pstrSubName, ppData, pSize);

	globals::DecRefForDisableFileAccessLog();

	return res;
}
