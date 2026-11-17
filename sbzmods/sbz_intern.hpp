#pragma once
#include <wtypes.h>


namespace sbz {
	struct CPackItem
	{
		char szName[260];
		int dwSize;
		int dwOffset;
	};

	struct CPackFormatSecret
	{
		char packMode_D500;                   ///< = 0x02
		char nKeyLength;
		BYTE gap2[2];
		int dwDecryptKey[8];
		char nType_D524;
		BYTE gap25[3];
	};

	struct CPack
	{
		void* lpVtbl;
		DWORD hFile;
		int nFormat;
		int nCount;
		CPackItem* lpData;
		int nEndHeader;
		CPackFormatSecret* lpPackFormatSecret;
		BYTE nType;
		char field_1D;
		char field_1E;
		char field_1F;
	};

	struct CString
	{
		char* pszData;
	};




};
