/* PP loader.  Should compile in either C or C++.
 * Algorithm from Alamar's source.
 */
#include "Shrink.h"
#include "pp.h"
#include "Util.h"

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

unsigned char HakoTrialCode[] =
{
	0x11,0x73,0x10,0x21,0x5A,0xA1,0xD0,0x8B,
	0x32,0x91,0x63,0x50,0xE9,0xA8,0xF6,0xD8,
	0x40,0x72,0x80,0xF9,0xEC,0x79,0x6E,0x8D,
	0x36,0x72,0x2B,0xA1,0x76,0xB6,0x67,0x92
};

unsigned char AG3WelcomeCode[] =
{
	0xE5,0x77,0x64,0x05,0xD2,0x37,0x4D,0x2E,
	0xB7,0x4A,0xB7,0x2B,0x22,0x70,0xF1,0xD6,
	0xC7,0xE7,0x61,0x6D,0x10,0xED,0xF5,0xC1,
	0xD9,0x08,0x28,0xEC,0xE2,0x09,0xEA,0xD7
};

unsigned char SMTrialCode[] =
{
	0x7C,0xF2,0x35,0x77,0x54,0x18,0x20,0x6E,
	0x9C,0x7B,0x9E,0x85,0x1F,0xB5,0x71,0x40,
	0x25,0xAD,0x71,0x43,0x64,0x20,0x20,0x7E,
	0xCF,0xE3,0x85,0xC0,0x41,0xDE,0x23,0x12
};

unsigned char SMRetailCode[] =
{
	0x1E,0x5D,0x13,0xDD,0x7D,0x4C,0x4F,0xA7,
	0xDB,0xA7,0x29,0x14,0x10,0xF8,0xC0,0xBE,
	0x44,0x7F,0xD0,0x63,0x1C,0x22,0x7C,0x9F,
	0xE8,0xB9,0xF8,0xBE,0x58,0xB3,0xEF,0xF4
};

unsigned char AtHomeFigureCode[] =
{
	0xAB,0x2C,0xC4,0x4E,0x7B,0xDF,0xBD,0x17,
	0xDC,0x2E,0x23,0x1E,0x4B,0xE5,0x80,0x3C,
	0x93,0xB1,0x1D,0x8C,0x81,0x36,0xB3,0x88,
	0x35,0x2D,0x30,0x4B,0x10,0x66,0xC8,0xE6
};

unsigned char AtHomeTrialCode[] =
{
	0x67,0xF9,0x30,0x5A,0x09,0xAB,0xF5,0x60,
	0xD6,0x9F,0xFD,0x93,0xBA,0x9C,0xF5,0x60,
	0x11,0x6A,0xBA,0x79,0x4C,0x41,0x4A,0x8D,
	0xC7,0xBA,0xBB,0x9C,0x26,0x34,0x0F,0xEF
};

unsigned char RGTrialCode[] =
{
	0x58,0x62,0x86,0xD2,0x3B,0x2F,0xC4,0x5F,
	0xEE,0x58,0x76,0x2D,0xB4,0x02,0x02,0xCD,
	0x0A,0x08,0x40,0x30,0x08,0x66,0x1D,0xE8,
	0x9B,0xA6,0x61,0xCB,0x63,0xF3,0xF3,0xB4,
};

unsigned short YuushaRetailCode[] =
{
	0xA82B,0x1EF2,0x1DDD,0xC895,
	0xD47E,0x764F,0x416F,0xC7BF,
};

unsigned char YuushaTrialCode[] =
{
	0xD1,0xEC,0x08,0xA1,0x48,0x7F,0xD6,0x8F,
	0xAD,0x34,0xB2,0xA2,0x35,0x4D,0x55,0xD1,
	0x1F,0xC1,0xB4,0x47,0x2F,0x54,0x89,0x24,
	0x61,0xCE,0xB7,0xA5,0x22,0x80,0x05,0x29,
};

unsigned char EskMateCode[] =
{
	0xE9,0xEC,0xFC,0x9F,0x67,0x4A,0x91,0x8D,
	0x72,0x4F,0x5F,0xAE,0xBB,0xA5,0xF7,0x0A,
	0x12,0xB9,0x03,0xC5,0x4E,0x1C,0xE3,0x7A,
	0x7E,0xF4,0x05,0x48,0x51,0x18,0x16,0x99
};

unsigned char CharacolleBenchCode[] =
{
	0xEB,0xD6,0x6B,0x29,0x21,0x03,0xA9,0x2C,
	0x5F,0x5F,0xEF,0xBB,0xEC,0x10,0xFC,0x4C,
	0x51,0xED,0xD4,0xBE,0x99,0x4D,0x45,0x06,
	0x65,0x51,0x8E,0x25,0x33,0x5C,0x05,0x53,
};

unsigned char SBZeroRetailCode[] =
{
	0x14,0x6F,0x07,0xB8,0x9A,0x0E,0x84,0x44,
	0x59,0x25,0x8E,0x18,0xBC,0x39,0x9E,0x5C,
	0x99,0x7A,0xA0,0x92,0xD4,0xB7,0xBC,0x55,
	0x1E,0x2E,0x88,0x27,0x14,0xA1,0xE6,0x27,
};

unsigned short AtHomeRetailCode[] =
{
	0x717e, 0x0e78, 0xafe7, 0x8fa7,
	0x9e1f, 0xc5e3, 0x0008, 0x713a
};

unsigned short AG3RetailCode[] =
{
	0x00CA,0x006E,0x000D,0x00B3,
	0x009C,0x0036,0x001E,0x00E8,
};

unsigned short HakoRetailCode[] =
{
	0xCBEE,0x1675,0x3533,0x4CE6,
	0x2F68,0x936D,0xF40D,0x0539,
};

unsigned short DGRetailCode[] =
{
	0x2110, 0x8BD0, 0x5063, 0xD8F6,
	0x7311, 0xA15A, 0x9132, 0xA8E9
};

unsigned short SMSweetsRetailCode[] =
{
	0x3F86, 0xB8D5, 0x4AB4, 0x06F4,
	0x70F6, 0x078A, 0x2F26, 0x3572
};

unsigned char SM2TrialCode[] =
{
	0x85,0x45,0x1B,0xBC,0x6E,0xDA,0x0E,0xA6,
	0x3F,0xCE,0x98,0x7D,0xD7,0x68,0xD9,0xEF,
	0xB4,0x3C,0x86,0xEF,0x4B,0x0D,0x08,0x28,
	0xF7,0xDE,0x12,0xA6,0xB7,0x0A,0x61,0x7A,
};

unsigned short SM2RetailCode[] =
{
	0xCE43, 0x6F31, 0xFC65, 0x9D2F,
	0x4182, 0xC473, 0x9D75, 0xD5B7
};


	//0xCE43, 0x6F31, 0x077D, 0x9EDC,
	//0x4182, 0xC473, 0x97E9, 0xD4A1

// For figuring out formats.
unsigned short NullCode[] =
{
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
};

static FormatData FormatInfo[] =
{
	// SB3 prefix.  Also a redundant fallback entry, as a number of other files tend
	// to use the sb3 format.
	{
		SB3,
		TYPE_SB3,
		L"SB3",
		L".pp",
		0,
		L"SB3"
	},
	// RL prefix, same format as SB3.
	{
		SB3,
		TYPE_SB3,
		L"RPP_",
		L".pp",
		0,
		L"RL"
	},
	// Base.pp for all games seems to use this format.  Don't ask me why.
	{
		SB3,
		TYPE_SB3,
		L"base",
		L".pp",
		0,
		L"base"
	},

	{
		AG3,
		TYPE_AG3,
		L"js3",
		L".pp",
		(unsigned char*) AG3RetailCode,
		L"AG3"
	},

	{
		HAKO,
		TYPE_AG3,
		L"hk",
		L".pp",
		(unsigned char*) HakoRetailCode,
		L"hako"
	},

	{
		HAKO_TRIAL,
		TYPE_SM,
		L"hkt",
		L".pp",
		(unsigned char*) HakoTrialCode
	},

	{
		HAKO,
		TYPE_AG3,
		L"tmd",
		L".pp",
		(unsigned char*) HakoRetailCode
	},

	{
		HAKO,
		TYPE_AG3,
		L"tma",
		L".pp",
		(unsigned char*) HakoRetailCode
	},

	{
		AHM,
		TYPE_AG3,
		L"AHM",
		L".pp",
		(unsigned char*) AtHomeRetailCode,
		L"AHM"
	},

	{
		AHM_FIGURE,
		TYPE_SM,
		L"AHM_",
		L".pp",
		(unsigned char*) AtHomeFigureCode
	},

	{
		AHM_TRIAL,
		TYPE_SM,
		L"AHM",
		L".pp",
		(unsigned char*) AtHomeTrialCode
	},


	{
		SM2,
		TYPE_AG3,
		L"qa",
		L".pp",
		(unsigned char*) SM2RetailCode,
		L"Sm2"
	},

	{
		SM2_TRIAL,
		TYPE_SM,
		L"qa",
		L".pp",
		(unsigned char*) SM2TrialCode,
		L"Sm2Trial"
	},


	{
		RG_TRIAL,
		TYPE_SM,
		L"rkt",
		L".pp",
		(unsigned char*) RGTrialCode,
		L"RgTrial"
	},

	{
		RG,
		TYPE_SM,
		L"rk",
		L".pp",
		(unsigned char*) RGTrialCode,
		L"RG"
	},

	//*
	{
		YUUSHA,
		TYPE_AG3,
		L"mo_",
		L".pp",
		(unsigned char*) YuushaRetailCode,
		L"Yuusha"
	},//*/

	{
		YUUSHA_TRIAL,
		TYPE_SM,
		L"mo_",
		L".pp",
		(unsigned char*) YuushaTrialCode,
		L"YuushaTrial"
	},


	{
		ESK_MATE,
		TYPE_SM,
		L"EM.",
		L".pp",
		EskMateCode,
		L"EskMate"
	},


	{
		CHARA_BENCH,
		TYPE_SM,
		L"cc00_",
		L".pp",
		CharacolleBenchCode,
		L"CharacolleBench"
	},

	{
		SB_ZERO,
		TYPE_SM,
		L"sb",
		L".pp",
		SBZeroRetailCode,
		L"SBZeroRetail"
	},

/*	{
		AG3_WELCOME,
		TYPE_SM,
		L"js3",
		L".pp",
		AG3WelcomeCode
	},//*/
	// Must be before SM entry for small speedup.
	{
		SM_SWEETS,
		TYPE_AG3,
		L"smss",
		L".pp",
		(unsigned char*) SMSweetsRetailCode,
		L"SMS"
	},

	{
		SM_TRIAL,
		TYPE_SM,
		L"smt",
		L".pp",
		(unsigned char*) SMTrialCode
	},

	{
		SM,
		TYPE_SM,
		L"sm",
		L".pp",
		(unsigned char*) SMRetailCode,
		L"SM"
	},

	// Default compress to format (And first value 3 format).
	{
		AG3,
		TYPE_AG3,
		// First 0 makes it the default.
		0,
		L".pp",
		(unsigned char*) AG3RetailCode
	},
	// Same header as AG3.  Only use when pp file is data\???\*.pp, and there's a data\##_*.sfd file.
	{
		DT,
		TYPE_AG3,
		// Minor speed optimization, will cause to skip past this entry based on file name.
		// Check if really DG after AG3 header check has passed, since they use the same headers.
		L"*",
		L".pp",
		(unsigned char*) DGRetailCode,
		L"DT"
	},

	// SB3/RL fallback.  Some other files use this format, too.
	{
		SB3,
		TYPE_SB3,
		0,
		L".pp",
	},
	// KISS arc (Already had the code for it, so why not?)
	{
		KS,
		TYPE_KS_ARC,
		0,
		L".arc",
		0
	},
/*
	{
		AUTO_SM,
		TYPE_SM,
		L"",
		L".pp",
		AutoSM
	},
	//*/
	{
		Null,
		TYPE_AG3,
		// Will cause to skip past this entry based on file name.
		L"*",
		L".pp",
		(unsigned char*) NullCode,
		L"null"
	},

	{
		Null,
		TYPE_SM,
		// Will cause to skip past this entry based on file name.
		L"*",
		L".pp",
		(unsigned char*) NullCode,
		L"null"
	},
};

GameID GetGameID(wchar_t *id) {
	for (int i=0; i<sizeof(FormatInfo)/sizeof(FormatInfo[0]); i++) {
		if (FormatInfo[i].formatID && !wcsicmp(FormatInfo[i].formatID, id)) return FormatInfo[i].gameID;
	}
	return Dunno;
}

void Decrypt(void *buffer, int size, const FormatData *format) {
	unsigned char *buf = (unsigned char*) buffer;
	int i;
	// SB3/RL/base.pp
	if (format->baseType == TYPE_SB3) {
		for (i=0; i<size; i++) {
			buf[i] = -buf[i];
		}
	}
	// SM and all trials
	else if (format->baseType == TYPE_SM) {
		const unsigned int *code = (const unsigned int *)format->code;

		int len = size/4;
		for (i = 0; i < len; i++) {
			((unsigned int*)buf)[i] = ((unsigned int*)buf)[i] ^ code[i&7];
		}
	}
	// AG3/DT/HAKO
	else if (format->baseType == TYPE_AG3) {
		int len = size/2;
		unsigned short codeA[4], codeB[4];
		memcpy(codeA, format->code, 8);
		memcpy(codeB, format->code+8, 8);

		for (i = 0; i < len; i++) {
			codeA[i&3] += codeB[i&3];
			((unsigned short*)buf)[i] = ((unsigned short*)buf)[i] ^ codeA[i&3];
		}
	}
}

// SM, DT, AG3, HAKO, and their trial versions all used this header
// format for 3 sections (id byte, number of files, and then file names
// sizes and offsets, all encrypted independently).  SB3 and RL use a much
// simpler function, and it's only used on one block, so code to read/write
// their headers is inlined.
void DecryptHeader(void *buffer, int size, int prefix) {
	unsigned char* buf = (unsigned char*)buffer;
	int i;
	unsigned char table[] =
	{
		0xFA, 0x49, 0x7B, 0x1C,
		0xF9, 0x4D, 0x83, 0x0A,
		0x3A, 0xE3, 0x87, 0xC2,
		0xBD, 0x1E, 0xA6, 0xFE
	};
	// Really inefficient, but too lazy to do efficiently.
	for (i = 0; i < prefix; i++) {
		int p = i & 0x7;
		table[p] += table[8 + p];
	}

	for (i = 0; i < size; i++) {
		int p = (i+prefix) & 0x7;
		table[p] += table[8 + p];
		buf[i] ^= table[p];
	}
}

PPHeader *LoadIllusionHeader(HANDLE hFile, __int64 *size, int baseType) {
	int j;
	int dataStart;
	int numFiles;
	DWORD read;
	PPHeader *pp;
	unsigned char type;
	PPFileHeader *headers;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	if (!ReadFile(hFile, &type, 1, &read, 0) || read != 1 ||
		!ReadFile(hFile, &numFiles, 4, &read, 0) || read != 4) {
			return 0;
	}
	DecryptHeader(&type, 1);
	if (type != baseType) return 0;
	DecryptHeader(&numFiles, 4);
	if (numFiles < 0 || ((__int64)numFiles) * sizeof(FileHeader) + 9 > *size) return 0;
	if (!(pp = (PPHeader*) calloc(1, sizeof(PPHeader) + sizeof(FileHeader) * (numFiles-1)))) {
		return 0;
	}
	headers = (PPFileHeader *)malloc(sizeof(PPFileHeader) * numFiles);
	if (!ReadFile(hFile, headers, numFiles * sizeof(PPFileHeader), &read, 0) || read != numFiles * sizeof(PPFileHeader)) {
		free(headers);
		free(pp);
		return 0;
	}
	DecryptHeader(headers, numFiles * sizeof(PPFileHeader));

	dataStart = numFiles * sizeof(PPFileHeader) + 9;
	for (j=0; j<numFiles; j++) {
		pp->headers[j].ppHeader = headers[j];
		pp->headers[j].offset = pp->headers[j].offset32;
	}
	free(headers);
	if (j<numFiles || !ReadFile(hFile, &j, 4, &read, 0) || read != 4) {
		free(pp);
		return 0;
	}
	DecryptHeader(&j, 4);
	if (j != dataStart) {
		free(pp);
		return 0;
	}
	// General purpose sanity check.
	for (j=0; j<numFiles; j++) {
		if (pp->headers[j].offset < dataStart || pp->headers[j].offset + pp->headers[j].size > *size) {
			free(pp);
			return 0;
		}
	}
	pp->numFiles = numFiles;
	return pp;
}

int RecursiveCompleteCode(wchar_t *file, unsigned char *code, int depth = 0) {
	depth++;
	if (depth == 3)
		return 0;
	wchar_t *path = (wchar_t*) malloc(sizeof(wchar_t) * (MAX_PATH + wcslen(file)));
	wcscpy(path, file);
	wchar_t *pos = wcsrchr(path, '\\');
	wchar_t *pos2 = wcsrchr(path, '/');
	if (pos2 > pos) pos = pos2;
	if (pos) {
		while (pos != path && (pos[-1]=='\\' || pos[-1]=='/'))
			pos--;
		pos++;
	}
	else pos = path;

	WIN32_FIND_DATAW findData;
	wcscpy(pos, L"*.exe");
	HANDLE hFind = FindFirstFileW(path, &findData);
	int happy = 0;
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			wcscpy(pos, findData.cFileName);
			HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
			if (hFile != INVALID_HANDLE_VALUE) {
				__int64 size;
				if (GetFileSizeEx(hFile, (LARGE_INTEGER*)&size) && size < 50*1024*1024) {
					unsigned char *data = (unsigned char*) malloc((int)size);
					DWORD read;
					if (ReadFile(hFile, data, (int)size, &read, 0)) {
						for (DWORD i=0; i<read-80; i++) {
							int j;
							for (int delta = 0; delta<100; delta++) {
								for (j=0; j<4; j++) {
									if (data[i+j+delta*j] != code[j]) break;
								}
								if (j==4) {
									j=j;
								}
							}
							for (j=0; j<4; j++) {
								if (data[i+j] != code[j]) break;
							}
							if (j != 4) continue;
							for (j=0; j<8; j++) {
								if (j) memcpy(code+4*j, data+i+10*j, 4);
								if (data[i+4+10*j] != 0x8B) break;
							}
							if (j != 8) continue;
							happy = 1;
							break;
						}
					}
					free(data);
				}
				CloseHandle(hFile);
			}
		}
		while (FindNextFileW(hFind, &findData) && !happy);
		FindClose(hFind);
	}

	if (!happy) {
		if (pos != path) pos--;
		*pos = 0;
		if (pos == path) wcscpy(path, L"..\\");
		else if (pos >= path+2 && (pos[-1] == '.' && pos[-2] == '.' && (pos-3 == path || pos[-3] == '/' || pos[-3]=='\\'))) {
			wcscpy(pos+1, L"..\\");
		}
		happy = RecursiveCompleteCode(path, code, depth);
		free(path);
	}
	return happy;
}


int FigureOutSMCode(PPHeader *pp, wchar_t *file, FormatData *autoFormat) {
	PPHeader *check = pp;
	WIN32_FIND_DATAW findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	pp->format = autoFormat;
	unsigned char *code = autoFormat->code;

	int happy = 0;
	while (!happy) {
		if (!check) {
			wchar_t path[MAX_PATH*2];
			wcscpy(path, file);
			wchar_t *pos = wcsrchr(path, '\\');
			wchar_t *pos2 = wcsrchr(path, '/');
			if (pos2 > pos) pos = pos2;
			if (!pos) pos = path;
			else pos++;
			if (hFind == INVALID_HANDLE_VALUE) {
				wcscpy(pos, L"*.pp");
				hFind = FindFirstFileW(path, &findData);
				if (hFind == INVALID_HANDLE_VALUE) break;
			}
			else {
				if (!FindNextFileW(hFind, &findData)) break;
			}
			wcscpy(pos, findData.cFileName);
			HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
			if (hFile == INVALID_HANDLE_VALUE) continue;
			__int64 size;
			if (!GetFileSizeEx(hFile, (LARGE_INTEGER*)&size)) {
				size = 0;
			}
			check = LoadIllusionHeader(hFile, &size, pp->format->baseType);
			CloseHandle(hFile);
			if (!check) continue;
		}
		int i;
		for (i=0; i<check->numFiles; i++) {
			if (ExtensionIs(check->headers[i].name, ".wav")) break;
		}
		if (i < check->numFiles) {
			memset(code, 0, 32);
			HANDLE hFile = INVALID_HANDLE_VALUE;
			char *data = 0;
			if (LoadFile(check->headers+i, file, &hFile, autoFormat) && check->headers[i].size >= 4) {
				char *data = check->headers[i].data;
				char header[32];
				strcpy(header, "RIFF____WAVEfmt ");
				*(int*)(header+4) = check->headers[i].size-8;
				*(int*)(header+16) = 16;
				*(short*)(header+20) = 1;
				*(short*)(header+22) = 1;
				*(int*)(header+24) = 22050;
				*(int*)(header+28) = 22050 * 1 * 2;
				for (int i=0; i<32; i++) {
					code[i] = header[i] ^ data[i];
				}

				//*
				code[0] = data[0] ^ 'R';
				code[1] = data[1] ^ 'I';
				code[2] = data[2] ^ 'F';
				code[3] = data[3] ^ 'F';
				//*/
				if (RecursiveCompleteCode(file, code)) happy = 1;
			}
			if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
		}
		if (check != pp) FreePP(check);
		check = 0;
	}
	if (hFind != INVALID_HANDLE_VALUE) FindClose(hFind);
	return happy;
}

PPHeader *GetFormat(wchar_t *path, HANDLE hFile, __int64 *size, GameID gameID) {
	wchar_t *name = path;
	wchar_t *name2 = wcsrchr(name, '\\');
	if (name2) name = name2+1;
	name2 = wcsrchr(name, '/');
	if (name2) name = name2+1;

	int i, j, k;

	if (hFile != INVALID_HANDLE_VALUE) {
		if (!GetFileSizeEx(hFile, (LARGE_INTEGER*)size)) {
			*size = 0;
		}
	}

	for (i=0; i<sizeof(FormatInfo)/sizeof(FormatData); i++) {
		DWORD read;
		PPHeader *pp = 0;
		if (FormatInfo[i].gameID == AUTO_SM) {
			if (hFile == INVALID_HANDLE_VALUE) continue;
		}
		if (gameID == Dunno) {
			if (FormatInfo[i].prefix && wcsnicmp(name, FormatInfo[i].prefix, (int)wcslen(FormatInfo[i].prefix))) continue;
			if (FormatInfo[i].suffix && !ExtensionIsW(name, FormatInfo[i].suffix)) continue;
		}
		else {
			if (gameID != FormatInfo[i].gameID) continue;
		}
		// Illusion types
		if (FormatInfo[i].baseType < 256) {
			// SB3 / RL
			if (FormatInfo[i].baseType == TYPE_SB3) {
				int dataStart;
				if (hFile != INVALID_HANDLE_VALUE) {
					SetFilePointer(hFile, 0, 0, FILE_BEGIN);
					int numFiles;
					if (!ReadFile(hFile, &numFiles, 4, &read, 0) || read != 4 || numFiles <= 0 ||
						!ReadFile(hFile, &k, 4, &read, 0) || read != 4 || k + ((__int64)numFiles) * 36+8 != *size) {
							continue;
					}
					pp = (PPHeader*) calloc(1, sizeof(PPHeader) + sizeof(FileHeader) * (numFiles-1));
					if (!pp) continue;
					pp->numFiles = numFiles;
					dataStart = numFiles*36+8;
					for (k=0; k<numFiles; k++) {
						if (!ReadFile(hFile, pp->headers[k].name, 32, &read, 0) || read != 32) break;
						for (j=0; j<31; j++) {
							pp->headers[k].name[j] = -pp->headers[k].name[j];
						}
						if (!pp->headers[k].name[0]) break;
					}

					if (k!=numFiles){
						free(pp);
						continue;
					}
					j = dataStart;
					for (k=0; k<numFiles; k++) {
						if (!ReadFile(hFile, &pp->headers[k].size, 4, &read, 0) || read != 4 || pp->headers[k].size < 0) break;
						pp->headers[k].offset = j;
						j += pp->headers[k].size;
						if (j > *size) break;
					}
					if (k!=numFiles || j != *size) {
						free(pp);
						continue;
					}
				}

				// General purpose sanity check.
				for (j=0; j<pp->numFiles; j++) {
					if (pp->headers[j].offset < dataStart || pp->headers[j].offset + pp->headers[j].size > *size) {
						free(pp);
						pp = 0;
						break;
					}
				}
				if (!pp) continue;
			}
			// Everything else uses same header format.
			else if (hFile != INVALID_HANDLE_VALUE) {
				pp = LoadIllusionHeader(hFile, size, FormatInfo[i].baseType);
				if (!pp) continue;
			}
		}
		else {
			if (FormatInfo[i].baseType == TYPE_KS_ARC) {
				if (hFile != INVALID_HANDLE_VALUE) {
					int dataStart;
					__int64 s;
					int numFiles;
					SetFilePointer(hFile, 0, 0, FILE_BEGIN);
					if (!ReadFile(hFile, &numFiles, 4, &read, 0) || read != 4 ||
						numFiles < 0 || numFiles > 30000 ||
						!(pp = (PPHeader*) calloc(1, sizeof(PPHeader) + sizeof(FileHeader) * (numFiles-1)))) continue;
					dataStart = 4 + 8 * numFiles;
					pp->numFiles = numFiles;
					for (k=0; k<numFiles; k++) {
						for (j=0; j<260; j++) {
							dataStart++;
							if (!ReadFile(hFile, pp->headers[k].name+j, 1, &read, 0) || read != 1) {
								j = 260;
								break;
							}
							if (!pp->headers[k].name[j]) break;
						}
						if (j == 260 ||
							!ReadFile(hFile, &pp->headers[k].offset, 8, &read, 0) || read != 8 ||
							pp->headers[k].offset > *size) {
								break;
						}
						if (k) {
							s = pp->headers[k].offset - pp->headers[k-1].offset;
							pp->headers[k-1].size = (int) s;
							if (s < 0 || s != pp->headers[k-1].size) break;
						}
					}
					s = *size - pp->headers[k-1].offset;
					pp->headers[k-1].size = (int) s;
					if (s < 0 || s != pp->headers[k-1].size || k < numFiles || pp->headers[0].offset < dataStart) {
						free(pp);
						continue;
					}

					// General purpose sanity check.
					for (j=0; j<pp->numFiles; j++) {
						if (pp->headers[j].offset < dataStart || pp->headers[j].offset + pp->headers[j].size > *size) {
							free(pp);
							pp = 0;
							break;
						}
					}
					if (!pp) continue;
				}
			}
		}
		if (!pp) {
			if (!(pp = (PPHeader*) calloc(1, sizeof(PPHeader))))
				continue;
		}
		if (FormatInfo[i].gameID == AG3) {
			int ReallyDG = 0;
			wchar_t temp[MAX_PATH*2], temp2[MAX_PATH*2];
			if (GetFullPathNameW(name, sizeof(temp)/sizeof(wchar_t), temp, 0)) {
				wchar_t *fileName = wcsrchr(temp, '\\');
				if (fileName) {
					wchar_t *dir;
					*fileName = 0;
					fileName++;
					dir = wcsrchr(temp, '\\');
					if (dir) {
						wchar_t *dir2;
						*dir = 0;
						dir++;
						dir2 = wcsrchr(temp, '\\');
						if (dir2) {
							dir2++;
							if (!wcsicmp(dir2, L"data")) {
								WIN32_FIND_DATAW findData;
								HANDLE hFind;
								wsprintfW(temp2, L"%s\\??_*.sfd", temp);
								hFind = FindFirstFileW(temp2, &findData);
								if (hFind != INVALID_HANDLE_VALUE) {
									do {
										if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && iswdigit(findData.cFileName[0]) && iswdigit(findData.cFileName[1])) {
											ReallyDG = 1;
											break;
										}
									}
									while (FindNextFileW(hFind, &findData));
									FindClose(hFind);
								}
							}
						}
					}
				}
			}
			if (ReallyDG) {
				for (j=i+1; j<sizeof(FormatInfo)/sizeof(FormatData); j++) {
					if (FormatInfo[j].gameID == DT) {
						i = j;
						break;
					}
				}
			}
		}
		pp->format = &FormatInfo[i];
		/*if (FormatInfo[i].gameID == AUTO_SM) {
			if (!FigureOutSMCode(pp, path, &FormatInfo[i])) {
				free(pp);
				pp = 0;
			}
		}/*/
		if (pp) {
			return pp;
		}
	}
	// default to hako trial if unrecognized prefix but first byte decodes right.
	// if (in && !wcsnicmp(f, L"hkt", 3)) return GetFormat(L"hkt.pp", in, size);

	return 0;
}

PPHeader *LoadPP(wchar_t *file, int *error, GameID gameID) {
	HANDLE hFile = CreateFileW(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	__int64 size = 0;
	PPHeader *pp;
	*error = 0;
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	*error = 1;
	pp = GetFormat(file, hFile, &size, gameID);
	if (!pp) {
		CloseHandle(hFile);
		return 0;
	}
	// Extra bytes needed for merge option.
	pp->file = (wchar_t*) malloc(sizeof(wchar_t)*(wcslen(file) + 20));
	wcscpy(pp->file, file);
	CloseHandle(hFile);
	*error = 0;
	return pp;
}

PPHeader *CreatePP(wchar_t *prefix, wchar_t *ext, GameID gameID) {
	HANDLE hFile;
	PPHeader *pp;
	__int64 size;
	wchar_t temp[2*MAX_PATH];
	wchar_t temp2[2*MAX_PATH];
	if (!ext) {
		wsprintfW(temp, L"%s.arc", prefix);
		wsprintfW(temp2, L"%s.old", temp);
		hFile = CreateFileW(temp2, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			hFile = CreateFileW(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		}
		if (hFile == INVALID_HANDLE_VALUE) {
			ext = L"pp";
		}
	}
	if (ext) {
		wsprintfW(temp, L"%s.%s", prefix, ext);
		wsprintfW(temp2, L"%s.old", temp);
		hFile = CreateFileW(temp2, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			hFile = CreateFileW(temp, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		}
	}
	pp = GetFormat(temp, hFile, &size, gameID);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	if (!pp) {
		pp = GetFormat(temp, INVALID_HANDLE_VALUE, 0, gameID);
		if (!pp) {
			wprintf(L"Can't determine output format: %s\n", temp);
			return 0;
		}
	}
	pp->file = (wchar_t*) malloc((wcslen(temp)+10)*sizeof(wchar_t));
	wcscpy(pp->file, temp);
	pp->numFiles = 0;
	return pp;
}

// Returns file in an encrypted format.  Reason for this is it's
// more efficient.  If used to copy a file from one pp to another
// (No program currently does this, but some older stuff of mine did)
// then it's a waste of time.  Other two uses are to extract or create
// a new pp, and either way, need to call decrypt exactly once, so
// doesn't matter which way this function does it.
int LoadFile(FileHeader *h, wchar_t *path, HANDLE *inf, const FormatData *format) {
	LONG temp;
	HANDLE hFile;
	DWORD read;
	int res;

	if (h->data) return 1;
	if (h->sourcePath) {
		// Only want inf populated if it's a pp file, so can be reused.
		HANDLE hFile = CreateFileW(h->sourcePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		res = 0;
		if (hFile != INVALID_HANDLE_VALUE) {
			int size;
			char *extension = strrchr(h->name, '.');
			__int64 size64;
			if (!GetFileSizeEx(hFile, (LARGE_INTEGER*)&size64) ||
				size64 >= 2*(__int64)1024*1024*1024) {
					CloseHandle(hFile);
					return 0;
			}
			size = (int)size64;
			h->data = (char*) malloc(size+13);
			if (h->data) {
				DWORD read;
				int prefix = 0;
				if (ExtensionIs(h->name, ".ema")) {
					if (ExtensionIsW(h->sourcePath, L".tga")) {
						((int*)h->data)[0] = ((short*)(&h->data[13+12]))[0];
						((int*)h->data)[1] = ((short*)(&h->data[13+12]))[1];
						strcpy(h->data+8, ".tga");
						prefix = 13;
					}
					else if (ExtensionIsW(h->sourcePath, L".bmp")) {
						((int*)h->data)[0] = ((int*)(&h->data[13+18]))[0];
						((int*)h->data)[1] = ((int*)(&h->data[13+18]))[1];
						strcpy(h->data+8, ".bmp");
						h->data[13] = h->data[14] = 0;
						prefix = 13;
					}
				}
				if (ReadFile(hFile, h->data+prefix, size, &read, 0) && read == (DWORD)size) {
					h->size = size + prefix;
					Decrypt(h->data, h->size, format);
					res = 1;
				}
				else {
					free(h->data);
					h->data = 0;
				}
				CloseHandle(hFile);
			}
		}
		return res;
	}
	if (inf && *inf != INVALID_HANDLE_VALUE) {
		hFile = *inf;
	}
	else if (!path || INVALID_HANDLE_VALUE == (hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0))) {
		return 0;
	}
	else if (inf) *inf = hFile;

	temp = (LONG) (h->offset>>32);
	res = 1;
	if (!(h->data = (char*) malloc(h->size)) ||
		(SetFilePointer(hFile, (DWORD)h->offset, &temp, FILE_BEGIN) == INVALID_SET_FILE_POINTER && GetLastError()) ||
		!ReadFile(hFile, h->data, h->size, &read, 0) || read != (DWORD)h->size) {
			free(h->data);
			h->data = 0;
			res = 0;
	}
	if (!inf) CloseHandle(hFile);
	return res;
}

int MergePP(PPHeader *pp) {
	HANDLE hFile = CreateFileW(pp->file, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	int i;
	if (hFile == INVALID_HANDLE_VALUE) {
		wprintf(L"Can't open file: %s\n", pp->file);
		return 0;
	}

	for (i=0; i<pp->numFiles; i++) {
		int unhappy = 0;
		if (!pp->headers[i].sourcePath) continue;
		printf("[%4i/%4i] ", 1+i, pp->numFiles);
		if (!LoadFile(pp->headers+i, pp->file, &hFile, pp->format)) {
			pp->headers[i].size = 0;
			unhappy = 1;
			printf("Error reading %s, skipping\n", pp->headers[i].name);
		}
		else {
			LONG high = (LONG)(pp->headers[i].offset>>32);
			if (SetFilePointer(hFile, (LONG) pp->headers[i].offset, &high, FILE_BEGIN) == INVALID_SET_FILE_POINTER &&
				GetLastError()) {
					printf("Error writing %s, skipping\n", pp->headers[i].name);
			}
			else {
				DWORD written;
				printf("Writing %s\n", pp->headers[i].name);
				if (!WriteFile(hFile, pp->headers[i].data, pp->headers[i].size, &written, 0) ||
					written != (DWORD)pp->headers[i].size) {
						printf("\t\tWrite error, file corrupted.\n");
				}
				free(pp->headers[i].data);
				pp->headers[i].data = 0;
			}
		}
	}
	CloseHandle(hFile);
	return 1;
}

int SavePP(PPHeader *pp, wchar_t *file) {
	HANDLE hFileOut, hFileIn = INVALID_HANDLE_VALUE;
	int i, j;
	int headerSize;
	__int64 pos;
	DWORD written;
	if (!file) file = pp->file;
	hFileOut = CreateFileW(file, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (hFileOut == INVALID_HANDLE_VALUE) {
		wprintf(L"Can't open file: %s\n", file);
		return 0;
	}

	if (pp->format->baseType == TYPE_AG3 || pp->format->baseType == TYPE_SM) {
		headerSize = 9 + sizeof(PPFileHeader) * pp->numFiles;
	}
	else if (pp->format->baseType == TYPE_SB3) {
		headerSize = 8 + pp->numFiles * 36;
	}
	else if (pp->format->baseType == TYPE_KS_ARC) {
		headerSize = 4;
		for (i=0; i<pp->numFiles; i++) {
			headerSize += 9 + (int)strlen(pp->headers[i].name);
		}
	}
	WriteFile(hFileOut, pp, headerSize, &written, 0);

	pos = headerSize;
	for (i=0; i<pp->numFiles; i++) {
		int unhappy = 0;
		printf("[%4i/%4i] ", 1+i, pp->numFiles);
		if (!LoadFile(pp->headers+i, pp->file, &hFileIn, pp->format)) {
			pp->headers[i].size = 0;
			unhappy = 1;
			printf("Error reading %s, truncating to length 0\n", pp->headers[i].name);
		}
		else printf("Writing %s\n", pp->headers[i].name);
		if (!WriteFile(hFileOut, pp->headers[i].data, pp->headers[i].size, &written, 0)) {
			printf("\t\tError writing, pp corrupted\n");
		}
		free(pp->headers[i].data);
		pp->headers[i].data = 0;
		pp->headers[i].offset32 = (int) (pp->headers[i].offset = pos);
		pos += pp->headers[i].size;
	}
	// Should never happen.
	if (hFileIn != INVALID_HANDLE_VALUE) CloseHandle(hFileIn);

	{
		SetFilePointer(hFileOut, 0, 0, FILE_BEGIN);
		if (pp->format->baseType == TYPE_AG3 || pp->format->baseType == TYPE_SM) {
			unsigned char s = (unsigned char) pp->format->baseType;
			PPFileHeader *tempHeaders;
			i = pp->numFiles;
			DecryptHeader(&s, 1);
			DecryptHeader(&i, 4);
			WriteFile(hFileOut, &s, 1, &written, 0);
			WriteFile(hFileOut, &i, 4, &written, 0);
			tempHeaders = (PPFileHeader*) malloc(sizeof(PPFileHeader) * pp->numFiles);
			for (i=0; i<pp->numFiles; i++) {
				tempHeaders[i] = pp->headers[i].ppHeader;
			}

			DecryptHeader(tempHeaders, sizeof(PPFileHeader) * pp->numFiles);
			WriteFile(hFileOut, tempHeaders, sizeof(PPFileHeader) * pp->numFiles, &written, 0);
			free(tempHeaders);
			DecryptHeader(&headerSize, 4);
			WriteFile(hFileOut, &headerSize, 4, &written, 0);
		}
		else if (pp->format->baseType == TYPE_SB3) {
			WriteFile(hFileOut, &pp->numFiles, 4, &written, 0);
			pos -= pp->numFiles*36+8;
			WriteFile(hFileOut, &pos, 4, &written, 0);
			for (i=0; i<pp->numFiles; i++) {
				char name[32];
				name[31] = 0;
				for (j=0; j<31; j++) {
					name[j] = -pp->headers[i].name[j];
				}
				if (pp->headers[i].name[32])
					wprintf(L"Warning: File name \"%s\" exceeds 31 characters, so it was truncated.\n", pp->headers[i].name);
				WriteFile(hFileOut, name, 32, &written, 0);
			}
			for (i=0; i<pp->numFiles; i++) {
				WriteFile(hFileOut, &pp->headers[i].size, 4, &written, 0);
			}
		}
		else if (pp->format->baseType == TYPE_KS_ARC) {
			WriteFile(hFileOut, &pp->numFiles, 4, &written, 0);
			for (i=0; i<pp->numFiles; i++) {
				WriteFile(hFileOut, pp->headers[i].name, strlen(pp->headers[i].name)+1, &written, 0);
				WriteFile(hFileOut, &pp->headers[i].offset, 8, &written, 0);
			}
		}
	}
	CloseHandle(hFileOut);
	return 1;
}

PPHeader *UpdatePP(PPHeader *data, char *name, wchar_t *src, int *cantOverwrite) {
	int i;
	char destName[MAX_PATH*2];
	int ema = 0;
	strcpy(destName, name);
	if (ExtensionIs(destName, ".ema.bmp") || ExtensionIs(destName, ".ema.tga")) {
		char *ext = destName+strlen(destName)-4;
		*ext = 0;
		ema = 1;
	}
	else if (!strnicmp(destName, "ema-", 4)) {
		if (ExtensionIs(name, ".bmp") || ExtensionIs(name, ".tga")) {
			char *ext2;
			memmove(destName, destName+4, strlen(destName)+1);
			ext2 = strrchr(destName, '.');
			strcpy(ext2, ".ema");
			ema = 1;
		}
	}
	for (i=0; i<data->numFiles; i++) {
		if (!stricmp(data->headers[i].name, destName)) {
			if (cantOverwrite) {
				WIN32_FILE_ATTRIBUTE_DATA info;
				if (!GetFileAttributesExW(src, GetFileExInfoStandard, &info) ||
					(!ema && (info.nFileSizeHigh || (int)info.nFileSizeLow != data->headers[i].size)) ||
					( ema && (info.nFileSizeHigh || (int)info.nFileSizeLow+13 != data->headers[i].size))) {
					*cantOverwrite = 1;
				}
			}
			if (data->headers[i].data) free(data->headers[i].data);

			if (data->headers[i].sourcePath) {
				wprintf(L"Warning: %s and %s are mapped to the same file.  Picking the first, arbitrarily\n\n",
					data->headers[i].sourcePath, src);
				return data;
			}
			if (data->headers[i].sourcePath) free(data->headers[i].sourcePath);
			data->headers[i].data = 0;
			data->headers[i].sourcePath = wcsdup(src);
			return data;
		}
	}
	if (cantOverwrite) *cantOverwrite = 1;
	data = (PPHeader *) realloc(data, sizeof(PPHeader) + sizeof(FileHeader) * data->numFiles);
	memset (&data->headers[data->numFiles], 0, sizeof(FileHeader));
	strcpy(data->headers[data->numFiles].name, destName);
	data->headers[data->numFiles].sourcePath = wcsdup(src);
	data->numFiles++;
	return data;
}

// Used to add file to pp, using data in memory.
PPHeader *UpdatePPWithData(PPHeader *data, char *name, char *buf, int len, int convert, const FormatData *format) {
	char *temp = (char*) malloc(len);
	int i;
	if (!temp) return 0;
	memcpy(temp, buf, len);
	if (convert) {
		Decrypt(temp, len, format);
	}
	for (i=0; i<data->numFiles; i++) {
		if (!stricmp(data->headers[i].name, name)) {
			if (data->headers[i].data) free(data->headers[i].data);
			data->headers[i].data = temp;
			data->headers[i].size = len;
			return data;
		}
	}
	data = (PPHeader *) realloc(data, sizeof(PPHeader) + sizeof(FileHeader) * data->numFiles);
	memset (&data->headers[data->numFiles], 0, sizeof(FileHeader));
	strcpy(data->headers[data->numFiles].name, name);
	data->headers[data->numFiles].size = len;
	data->headers[data->numFiles].data = temp;
	data->numFiles++;
	return data;
}

char *GetFile(PPHeader *header, int pos, int *size, const FormatData *format, HANDLE *hFile) {
	char *out;
	if (!header) return 0;
	if (!LoadFile(header->headers+pos, header->file, hFile, format)) {
		if (*hFile != INVALID_HANDLE_VALUE) CloseHandle(*hFile);
		*hFile = INVALID_HANDLE_VALUE;
		return 0;
	}
	out = header->headers[pos].data;
	*size = header->headers[pos].size;
	header->headers[pos].data = 0;
	Decrypt(out, *size, format);
	return out;
}

/*
char *GetFileByName(PPHeader *header, char *name, int *size, int format, FILE **in) {
	int i;
	if (!header) return 0;
	for (i=0; i<header->numFiles; i++) {
		if (!stricmp(header->headers[i].name, name)) {
			return GetFile(header, i, size, format, in);
		}
	}
	return 0;
}//*/

void FreePP(PPHeader *pp) {
	int i;
	if (!pp) return;
	for (i=0; i<pp->numFiles; i++) {
		if (pp->headers[i].data) free(pp->headers[i].data);
		if (pp->headers[i].sourcePath) free(pp->headers[i].sourcePath);
	}
	free(pp->file);
	free(pp);
}

void ExtractPP(PPHeader *pp, int retro, wchar_t **list, int listLen) {
	HANDLE hFile = INVALID_HANDLE_VALUE;
	int i,j;
	wchar_t outPath[3*MAX_PATH];
	wchar_t *file;
	char *retroInfo = 0;
	wcscpy(outPath, pp->file);
	file = wcsrchr(outPath, '.');
	if (!file) return;
	if (wcsrchr(file, '\\')) return;
	file[0] = 0;
	CreateDirectoryW(outPath, 0);
	file[0] = '\\';
	file++;
	if (listLen) {
		retroInfo = (char*) malloc(listLen);
		memset(retroInfo, retro, listLen);
	}
	for (j=0; j<listLen; j++) {
		if (ExtensionIsW(list[j], L".ema.tga") || ExtensionIsW(list[j], L".ema.bmp")) {
			retroInfo[j] = 0;
			wcsrchr(list[j], '.')[0] = 0;
		}
		else if (!wcsnicmp(list[j], L"ema-", 4) &&
				 (ExtensionIsW(list[j], L".tga") || ExtensionIsW(list[j], L".bmp"))) {
					 int w = (int) wcslen(list[j]);
					 wcscpy(list[j]+w-4, L".ema");
					 memmove(list[j], list[j]+4, sizeof(wchar_t*) * (w+1));
					 retroInfo[j] = 1;
		}
	}
	for (i=0; i<pp->numFiles; i++) {
		int size=0;
		char *out;
		MultiByteToWideChar(932, 0, pp->headers[i].name, -1, file, MAX_PATH);
		if (list) {
			for (j=0; j<listLen; j++) {
				if (!wcsicmp(file, list[j])) break;
			}
			if (j==listLen) continue;
			retro = retroInfo[j];
		}
		//if (strcmp(pp->headers[i].name, "AA00_00_00_00.bmp")) continue;
		printf("[%4i/%4i] ", 1+i, pp->numFiles);
		out = GetFile(pp, i, &size, pp->format, &hFile);
		if (out) {
			int prefix = 0;
			DWORD written;
			HANDLE hWrite;
			// Shift JIS
			if (ExtensionIsW(outPath, L".ema") && size > 20) {
				if (!strnicmp(out+8, ".tga", 4)) {
					prefix = 13;
					if (retro) {
						int len = (int) wcslen(file);
						memmove(file+4, file, (1+wcslen(file))*sizeof(wchar_t));
						memmove(file, L"ema-", 8);
						file[len] = 0;
					}
					wcscat(file, L".tga");
				}
				else if (!strnicmp(out+8, ".bmp", 4)) {
					out[13] = 0x42;
					out[14] = 0x4D;
					prefix = 13;
					if (retro) {
						int len = (int) wcslen(file);
						memmove(file+4, file, (1+wcslen(file))*sizeof(wchar_t));
						memmove(file, L"ema-", 8);
						file[len] = 0;
					}
					wcscat(file, L".bmp");
				}
			}
			hWrite = CreateFileW(outPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (hWrite == INVALID_HANDLE_VALUE) {
				wprintf(L"Can't open file: %s\n", outPath);
			}
			else {
				wprintf(L"Writing %s\n", outPath);
				if (!WriteFile(hWrite, out+prefix, size-prefix, &written, 0)) {
					printf("\t\tError writing file, corrupted\n", outPath);
				}
				CloseHandle(hWrite);
			}
			free(out);
		}
		else wprintf(L"Can't read file: %s\n", outPath);
	}
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	if (retroInfo) free(retroInfo);
}

// Simpler to use than LoadFile().
char *LoadDecryptedFile(PPHeader *pp, int index, int *size, HANDLE *inf) {
	char *out;
	if (!LoadFile(pp->headers+index, pp->file, inf, pp->format)) return 0;
	Decrypt(pp->headers[index].data, pp->headers[index].size, pp->format);
	if (size) *size = pp->headers[index].size;
	out = pp->headers[index].data;
	pp->headers[index].data = 0;
	return out;
}
