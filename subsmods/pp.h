#pragma once

#include "Shrink.h"
#include <Windows.h>



enum GameID {
	Dunno,
	HAKO,
	HAKO_TRIAL,
	SM2,
	SM2_TRIAL,
	AHM,
	AHM_FIGURE,
	AHM_TRIAL,
	RG,
	RG_TRIAL,
	YUUSHA,
	YUUSHA_TRIAL,
	ESK_MATE,
	CHARA_BENCH,
	AG3,
	AG3_WELCOME,
	SB3,
	SB3_PLUS,
	SB_ZERO,
	RL,
	DT,
	SM_SWEETS,
	SM,
	SM_TRIAL,
	KS,
	Null,
	AUTO_SM,
	Custom,
};

// Archive header types.
#define TYPE_SB3	0
// SM and trials use the same header format.
#define TYPE_SM		1
// AG3, Hako, and DG use the same header format.
#define TYPE_AG3	3
#define TYPE_KS_ARC	256

struct PP_FILE_HEADER {
	char name[260];
	int size32;
	int offset32;
};
typedef struct PP_FILE_HEADER PPFileHeader;

struct FILE_HEADER {
	union {
		PPFileHeader ppHeader;
		struct {
			char name[260];
			int size;
			int offset32;
		};
	};
	// 64-bit archives supported, but can only hold 32-bit files.
	__int64 offset;
	char *data;
	wchar_t *sourcePath;
};
typedef struct FILE_HEADER FileHeader;

struct FORMAT_DATA {
	GameID gameID;
	unsigned short baseType;
	wchar_t *prefix;
	wchar_t *suffix;
	unsigned char *code;
	wchar_t *formatID;
};
typedef struct FORMAT_DATA FormatData;

#pragma pack(push)
#pragma pack(1)
struct PP_HEADER {
	FormatData *format;
	wchar_t *file;
	int numFiles;
	FileHeader headers[1];
};
typedef struct PP_HEADER PPHeader;
#pragma pack(pop)


PPHeader *LoadPP(wchar_t *file, int *error, GameID gameID);
void FreePP(PPHeader *pp);
void Decrypt(void *buffer, int size, const FormatData *);
int LoadFile(FileHeader *h, wchar_t *path, HANDLE *inf, const FormatData *);
void ExtractPP(PPHeader *pp, int retro, wchar_t **list, int listLen);
PPHeader *CreatePP(wchar_t *prefix, wchar_t *ext, GameID gameID);
PPHeader *UpdatePP(PPHeader *data, char *name, wchar_t *src, int *cantOverwrite);

// outFile must be non-null and not pp->file when copying from one pp file to another.
int SavePP(PPHeader *pp, wchar_t *outFile);

int MergePP(PPHeader *pp);

void DecryptHeader(void *buffer, int size, int prefix=0);

char *LoadDecryptedFile(PPHeader *pp, int index, int *size, HANDLE *inf);
GameID GetGameID(wchar_t *id);
