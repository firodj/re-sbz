#pragma once

#include <string_view>
#include <type_traits>

namespace sbz {

	class PPRedirect {
	private:
		CString* m_pStrPPFile;
	public:
		PPRedirect(CString* pStrPPFile);

		std::string getPPFolder();
		int swapFile(CString* pStrSubFile, std::string& filename, std::string& fileext);
		void getSubFile(CString* pStrSubFile, std::string& filename, std::string& fileext);
		bool fileExists(std::string filefullpath);

		bool subFileReadToTGA(std::string filename, std::string fileext, LPBYTE* ppData, DWORD* pSize, int swapped);
		bool subFileRead(CString* pStrSubFile, LPBYTE* ppData, DWORD* pSize);
	};


  extern LPVOID(__cdecl* MemAllocate_719FA0)(SIZE_T dwBytes);
  extern char(__cdecl* InitScene_4951A0)(ESBZScene nNextSceneID, unsigned __int8 nStackOp, unsigned __int8 nSceneState, unsigned __int8 nTransition, int nFadeInMs, int nFadeOutMs);

};
