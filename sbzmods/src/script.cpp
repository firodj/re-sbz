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
#include "struct.hpp"
#include "pack.hpp"
#include "script.hpp"

// ---
namespace sbz {
	extern bool(__cdecl* ParseSDTFile_42D4A0)(CSDTScript* pSDTScript, BYTE* lpMem, int nSize) =
		reinterpret_cast<bool(__cdecl*)(CSDTScript * pSDTScript, BYTE * lpMem, int nSize)>(0x42D4A0);
	extern bool(__cdecl*ISE_ScriptFileOpen_42D420)(CSDTScript* pSDTStruct, CString* strPPFileName, CPackFormatSecret* packFmtSec, char nFmtType, CString* strSubFileName) =
		reinterpret_cast<bool(__cdecl*)(CSDTScript* pSDTStruct, CString* strPPFileName, CPackFormatSecret* packFmtSec, char nFmtType, CString* strSubFileName)>(0x42D420);
	LPBYTE(__cdecl* LoadSubFile_695220)(DWORD* pOutSize, CString* pstrFileName, CPackFormatSecret* pPackFmtSec, char nFmtType, CString* pstrSubFileName) =
		reinterpret_cast<LPBYTE(__cdecl*)(DWORD* pOutSize, CString* pstrFileName, CPackFormatSecret* pPackFmtSec, char nFmtType, CString* pstrSubFileName)>(0x695220);
	
	void(__cdecl*MemFree_71A030)(void* lpMem) = reinterpret_cast<void(__cdecl*)(void* lpMem)>(0x71A030);

	bool (*LogPrintF_416630)(char nSeverity, char nLevel, bool bLogging, const char* Format, ...) =
		reinterpret_cast<bool(*)(char nSeverity_a1, char nLevel, bool bLogging, const char *Format, ...)>(0x416630);

	bool (*CSDTScript__WriteToSDT_42D960)(CSDTScript* pSDTScript, CString strFilename) =
		reinterpret_cast<bool(*)(CSDTScript *pSDTScript, CString strFilename)>(0x42D960);
	bool (*CSDTScript__WriteToTXT_440480)(CSDTScript* pSDTScript, CString strFileName) =
		reinterpret_cast<bool (*)(CSDTScript* pSDTScript, CString strFileName)>(0x440480);
	void(__thiscall* CString__SetStringOrByResID_402C50)(CString* This, const char* Str) = reinterpret_cast<void(__thiscall* )(CString* This, const  char* Str)>(0x402C50);
	void(__thiscall* CString__Destructor1)(CString* This) = reinterpret_cast<void(__thiscall*)(CString* This)>(0x62EC00);

	class Text : public CString {
	public:
		Text() = default; // Keep the default constructor

		// Disallow copy constructor
		Text(const Text&) = delete;

		// Disallow copy assignment operator
		Text& operator=(const Text&) = delete;

		Text(const char* txt) {
			CString__SetStringOrByResID_402C50(this, txt);
		}
		~Text() {
			CString__Destructor1(this);
		}
	};

	bool __cdecl hookISE_ScriptFileOpen_42D420(
		CSDTScript* pSDTScript,
		CString* strPPFileName,
		CPackFormatSecret* packFmtSec,
		char nFmtType,
		CString* strSubFileName)
	{
		bool v6; // [esp+3h] [ebp-9h]
		BYTE *lpMem; // [esp+4h] [ebp-8h]
		int nMemSize_v8; // [esp+8h] [ebp-4h] BYREF

		nMemSize_v8 = 0;
		lpMem = (BYTE *)LoadSubFile_695220((DWORD *)&nMemSize_v8, strPPFileName, packFmtSec, nFmtType, strSubFileName);
		if ( lpMem )
		{
			v6 = ParseSDTFile_42D4A0(pSDTScript, lpMem, nMemSize_v8);
			MemFree_71A030(lpMem);

			DebugLog("[SDT] Load from %s %s\n", strPPFileName->pszData, strSubFileName->pszData);
			PPRedirect ppRedirect(strPPFileName);
			std::string filename, fileext;
			ppRedirect.getSubFile(strSubFileName, filename, fileext);
			std::string outTxt = ppRedirect.getPPFolder() + "/" + filename + ".decoded.txt";
			if (!ppRedirect.fileExists(outTxt)) {
				Text strOut(outTxt.c_str());
				bool ret = CSDTScript__WriteToTXT_440480(pSDTScript, strOut);
				DebugLog("[SDT] Write to %s, ret = %d\n", strOut.pszData, ret);
			}

			return v6;
		}
		else
		{
			LogPrintF_416630(2u, 1, 0, "ISE: ScriptFileOpenError");
			return 0;
		}
	}


	void register_Script()
	{
		util::Hook(0x42D420, hookISE_ScriptFileOpen_42D420, "ISE_ScriptFileOpen_42D420", nullptr);
	
	}
};
