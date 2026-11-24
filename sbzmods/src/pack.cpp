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
#include "stb_image.h"

// Explicitly pack the structure to match the 18-byte TGA file format specification
#pragma pack(push, 1)
struct TGAHeader {
	uint8_t  id_length        = 0;
	uint8_t  color_map_type   = 0;
	uint8_t  image_type       = 2;    // 2 = Uncompressed true-color image
	uint16_t color_map_origin = 0;
	uint16_t color_map_length = 0;
	uint8_t  color_map_depth  = 0;
	uint16_t x_origin         = 0;
	uint16_t y_origin         = 0;
	uint16_t width            = 0;
	uint16_t height           = 0;
	uint8_t  pixel_depth      = 0;    // 24 or 32 bits per pixel
	uint8_t  image_descriptor = 0x20; // 0x20 sets origin to top-left (matches PNG)
};
#pragma pack(pop)

bool convert_png_to_tga_memory(const char* filename, LPBYTE* ppData, DWORD* pSize) {
	int width = 0;
	int height = 0;
	int channels = 0;

	// Force 4 channels (RGBA) to ensure consistent 32-bit TGA conversion.
	// If you explicitly want 24-bit TGA when alpha is missing, change 4 to 0.
	unsigned char* png_pixels = stbi_load(filename, &width, &height, &channels, 4);
	if (!png_pixels) {
		DebugLog("{CPack:PPredirect:PNG} Failed to load PNG: %s\n", stbi_failure_reason());
		return false;
	}

	// Set up TGA file layout properties
	TGAHeader header;
	header.width = static_cast<uint16_t>(width);
	header.height = static_cast<uint16_t>(height);
	header.pixel_depth = 32; // 4 forced channels * 8 bits = 32bpp

	size_t pixel_data_size = width * height * 4;
	size_t total_tga_size = sizeof(TGAHeader) + pixel_data_size;

	// Allocate memory buffer for the raw TGA file

	//std::vector<uint8_t> tga_buffer(total_tga_size);
	uint8_t* tga_buffer = (uint8_t*)sbz::MemAllocate_719FA0(total_tga_size);

	// 1. Copy the TGA header into place
	std::memcpy(tga_buffer, &header, sizeof(TGAHeader));

	// 2. Swizzle channels from RGBA to BGRA and write payload
	uint8_t* destination_pixels = tga_buffer + sizeof(TGAHeader);
	for (size_t i = 0; i < pixel_data_size; i += 4) {
		destination_pixels[i + 0] = png_pixels[i + 2]; // Blue  <- Red
		destination_pixels[i + 1] = png_pixels[i + 1]; // Green <- Green
		destination_pixels[i + 2] = png_pixels[i + 0]; // Red   <- Blue
		destination_pixels[i + 3] = png_pixels[i + 3]; // Alpha <- Alpha
	}

	// Free the memory allocated by stb_image
	stbi_image_free(png_pixels);

	if (ppData)
		*ppData = (LPBYTE)tga_buffer;
	if (pSize)
		*pSize = total_tga_size;
	return true;

}


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

	CString*(__thiscall* CString__SetStringOrByResourceID)(CString*, const char*) = reinterpret_cast<CString*(__thiscall*)(CString *, const char *)>(0x403610);
	LPVOID(__cdecl*MemAllocate_719FA0)(SIZE_T dwBytes) = reinterpret_cast<LPVOID(__cdecl*)(SIZE_T)>(0x719FA0);
	char(__cdecl* InitScene_4951A0)(ESBZScene nNextSceneID, unsigned __int8 nStackOp, unsigned __int8 nSceneState, unsigned __int8 nTransition, int nFadeInMs, int nFadeOutMs)
		= reinterpret_cast<char (__cdecl*)(ESBZScene nNextSceneID, unsigned __int8 nStackOp, unsigned __int8 nSceneState, unsigned __int8 nTransition, int nFadeInMs, int nFadeOutMs)
		>(0x4951A0);

	// ---

	

	class PPRedirect {
	private:
		CString* m_pStrPPFile;
	public:
		PPRedirect(CString* pStrPPFile) {
			m_pStrPPFile = pStrPPFile;
		}

		std::string getPPFolder() {
			auto path_str = std::string{ m_pStrPPFile->pszData };

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

#if 1
			path = path + "../out/";
#endif

			return path + filename;
		}

		bool swapFile(CString* pStrSubFile, std::string &filename, std::string &fileext) {
			std::string try_filename, try_fileext;
			getSubFile(pStrSubFile, filename, fileext);
			auto ppdir_str = getPPFolder();
			
			try_filename = filename;
			try_fileext = fileext;

			if (_stricmp(".wav", fileext.c_str()) == 0) {
				
				if (_stricmp("bgm_14_00", filename.c_str()) == 0) { // there is duplicate bgm_14_00.wav and bgm_14_00.ogg, so use it with as bgm_14_01.ogg
					try_filename = "bgm_14_01";
				}
				try_fileext = ".ogg";

				std::string try_path = ppdir_str + "/" + try_filename + try_fileext;
				std::ifstream file(try_path, std::ios::binary | std::ios::ate);
				if (file.is_open()) {
					filename = try_filename;
					fileext = try_fileext;
					return true;
				}
			}

			if (_stricmp(".bmp", fileext.c_str()) == 0) {
				try_fileext = ".png";

				std::string try_path = ppdir_str + "/" + try_filename + try_fileext;
				std::ifstream file(try_path, std::ios::binary | std::ios::ate);
				if (file.is_open()) {
					filename = try_filename;
					fileext = ".tga"; // convert on the fly as TGA data
					return true;
				}
			}

			return false;
		}

		void getSubFile(CString* pStrSubFile, std::string& filename, std::string& fileext) {
			auto subpath_str = std::string{ pStrSubFile->pszData };

			size_t last_slash_idx = subpath_str.find_last_of("\\/");
			
			if (std::string::npos != last_slash_idx) {
				filename = subpath_str.substr(last_slash_idx + 1);
			}
			else {
				filename = subpath_str;
			}

			size_t last_dot_idx = filename.find_last_of(".");
			if (std::string::npos != last_dot_idx) {
				fileext = filename.substr(last_dot_idx);
				filename = filename.substr(0, last_dot_idx);
			}
		}

		bool subFileReadAsPNG(std::string filename,std::string fileext, LPBYTE* ppData, DWORD* pSize)
		{
			auto ppdir_str = getPPFolder();
			
			if (_stricmp(".tga", fileext.c_str()) != 0) return false;

			fileext = ".png";
			std::string filefullpath = ppdir_str + "/" + filename + fileext;
			{
				std::ifstream file(filefullpath, std::ios::binary | std::ios::ate);
				// 1a. Check if the file exists / opened successfully
				if (!file.is_open()) {
					DebugLog("{CPack::PPRedirect:PNG} File does not exist %s, fallback\n", filefullpath.c_str());
					return false;
				}
			}

			if (convert_png_to_tga_memory(filefullpath.c_str(), ppData, pSize)) {
				DebugLog("[CPack::PPRedirect:PNG] load into TGA %s, size %d\n", filefullpath.c_str(), *pSize);
				return true;
			}

			return false;
		}

		bool subFileRead(CString* pStrSubFile, LPBYTE* ppData, DWORD* pSize)
		{
			auto ppdir_str = getPPFolder();
			std::string filename, fileext;

			bool swapped = swapFile(pStrSubFile, filename, fileext);

			if (subFileReadAsPNG(filename, fileext, ppData, pSize)) {
				if (swapped) {
					std::string subFileName = filename + fileext;
					CString__SetStringOrByResourceID(pStrSubFile, subFileName.c_str());
				}
				return true;
			}
			
			std::string filefullpath = ppdir_str + "/" + filename + fileext;

			std::ifstream file(filefullpath, std::ios::binary | std::ios::ate);
			// 1a. Check if the file exists / opened successfully
			if (!file.is_open()) {
				DebugLog("{CPack::PPRedirect} File does not exist %s, fallback\n", filefullpath.c_str());
				return false;
			}
			
			// 2. Check file size
			// Since we opened it with std::ios::ate, tellg() gives us the current position (the end)
			std::streamsize file_size = file.tellg();
			DebugLog("[CPack::PPRedirect] File %s, size: %d\n", filefullpath.c_str(), file_size);

			// 3. Read file into buffer
			// Move the file pointer back to the beginning to start reading
			file.seekg(0, std::ios::beg);

			char* buffer = (char*)sbz::MemAllocate_719FA0(file_size);
			if (buffer && file.read(buffer, file_size)) {
				if (ppData)
					*ppData = (LPBYTE)buffer;
				if (pSize)
					*pSize = file_size;

				if (swapped) {
					std::string subFileName = filename + fileext;
					CString__SetStringOrByResourceID(pStrSubFile, subFileName.c_str());
				}
				return true;
			}
			
			DebugLog("{CPack::PPRedirect} Error: Could not read the entire file.\n");
			return false;
		}
	};

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
		PPRedirect ppRedirect(pstrFileName);
		
		DebugLog("[CPack::GetSubFileFromPP] fileName = %s, subFile = %s, type = %x, type(2) = %x, packmode = %x, getPPFolder = %s\n", 
			pstrFileName->pszData, pstrSubName->pszData, nType, pPackFmtSec->nType_D524, pPackFmtSec->packMode_D500, ppRedirect.getPPFolder().c_str());

		if (ppRedirect.subFileRead(pstrSubName, ppData, pSize)) {
			return 1;
		}
		
		/// Fallback using default

		globals::IncRefForDisableFileAccessLog();

		DebugLog("[CPack::GetSubFileFromPP] fileName = %s, subFile = %s, type = %x, packmode = %x\n",
			pstrFileName->pszData, pstrSubName->pszData, nType, pPackFmtSec->packMode_D500);

		char res = origCPack__GetSubFileFromPP_70C9C0(pThis, edx, pstrFileName, pPackFmtSec, nType, pstrSubName, ppData, pSize);

		globals::DecRefForDisableFileAccessLog();

		return res;
	}

	void(__cdecl* origLoadOldPPFile_7196F0)(sbz::CString* pstrFileName, sbz::CString* pstrSubName, LPBYTE* ppData, DWORD* pSize) = nullptr;

	void __cdecl hookLoadOldPPFile_7196F0(sbz::CString *pstrFileName, sbz::CString *pstrSubName, LPBYTE* ppData, DWORD* pSize)
	{
		PPRedirect ppRedirect(pstrFileName);
	

		DebugLog("[LoadOldPPFile] fileName = %s, subFile = %s, getPPFolder = %s\n", 
			pstrFileName->pszData, pstrSubName->pszData, ppRedirect.getPPFolder().c_str());

		//// ensure subpath only filename

		if (ppRedirect.subFileRead(pstrSubName, ppData, pSize)) {
			return;
		}

		/// Fallback using default

		globals::IncRefForDisableFileAccessLog();

		DebugLog("[LoadOldPPFile] fileName = %s, subFile = %s\n",
			pstrFileName->pszData, pstrSubName->pszData);

		origLoadOldPPFile_7196F0(pstrFileName, pstrSubName, ppData, pSize);

		globals::DecRefForDisableFileAccessLog();
	}

	CStructImage_5C* (__cdecl* origLoadImage_705D60)(
		CString* pstrFileName,
		CPackFormatSecret* pPackFmtSec,
		char nFmtType,
		CString* pstrSubFileName,
		CString* pstrSubFileName2,
		char bSrcSkip) = nullptr;

	CStructImage_5C* hookLoadImage_705D60(
	CString* pstrFileName,
		CPackFormatSecret* pPackFmtSec,
		char nFmtType,
		CString* pstrSubFileName,
		CString* pstrSubFileName2,
		char bSrcSkip) {

		DebugLog("[SBZ:LoadImage_705D60] fileName = %s, subFile = %s, subFile2 = %s\n", 
			pstrFileName->pszData, pstrSubFileName->pszData, pstrSubFileName2->pszData);

		PPRedirect ppRedirect(pstrFileName);
		std::string filename, fileext;

		if (ppRedirect.swapFile(pstrSubFileName2, filename, fileext)) {
			std::string subFileName = filename + fileext;
			CString__SetStringOrByResourceID(pstrSubFileName2, subFileName.c_str());
		}

		CStructImage_5C* pImage = origLoadImage_705D60(pstrFileName, pPackFmtSec, nFmtType, pstrSubFileName, pstrSubFileName2, bSrcSkip);

		DebugLog("[SBZ:LoadImage_705D60] after fileName = %s, subFile = %s, subFile2 = %s\n", 
			pstrFileName->pszData, pstrSubFileName->pszData, pstrSubFileName2->pszData);

		return pImage;
	};

	char __fastcall accessor_2Ch_52C440(void* This, void* edx) {
		DWORD lpVtbl = *(DWORD*)This;
		char data = *((char*)This + 0x2C);
		DebugLog("[SBZ:accessor_2Ch_52C440] This=0x%08x, lpVtbl=0x%08X, data=%d\n", (UINT_PTR)This, lpVtbl, data);
		return data;
	}

	void register_Pack()
	{
		util::Hook(0x70C9C0, hookCPack__GetSubFileFromPP_70C9C0, "CPack::GetSubFileFromPP_70C9C0", (LPVOID*)&origCPack__GetSubFileFromPP_70C9C0);
		util::Hook(0x7196F0, hookLoadOldPPFile_7196F0, "LoadOldPPFile_7196F0", (LPVOID*)&origLoadOldPPFile_7196F0);
		util::Hook(0x705D60, hookLoadImage_705D60, "LoadImage_705D60", (LPVOID*)&origLoadImage_705D60);
		util::Hook(0x52C440, accessor_2Ch_52C440, "accessor_2Ch_52C440", nullptr);
	}

}