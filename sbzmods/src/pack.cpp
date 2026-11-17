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

// ---

namespace sbz {

	typedef char(__fastcall* CPack__GetSubFileFromPP_70C9C0Fn)(
		CPack* pThis,
		void* edx,
		CString* pstrFileName_a2,
		CPackFormatSecret* pPackFmtSec_a3,
		char nType,
		CString* pstrSubName,
		LPBYTE* ppData,
		DWORD* pSize);
	static sbz::CPack__GetSubFileFromPP_70C9C0Fn origCPack__GetSubFileFromPP_70C9C0;

	// ---

	LPVOID(__cdecl*MemAllocate_719FA0)(SIZE_T dwBytes) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T)>(0x719FA0);


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
				}
				else {
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


	void register_Pack()
	{
		origCPack__GetSubFileFromPP_70C9C0 = (sbz::CPack__GetSubFileFromPP_70C9C0Fn)util::Hook(0x70C9C0, hookCPack__GetSubFileFromPP_70C9C0, "CPack::GetSubFileFromPP_70C9C0");
	}

}