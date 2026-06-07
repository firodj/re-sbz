#pragma once

// For GameIDs
#include "pp.h"

class GameHandler;

struct CharInfo {
	GameID game;
	char *name;
	wchar_t *file;
	unsigned long color;
	unsigned long outline;
	unsigned long shadow;
};

#define EXE_REG_ONLY	1
#define EXE_FORCE_REG	2
#define EXE_EXCLUDE		4

struct ExeSettings {
	wchar_t *exe;
	int setting;
};

// Not actually used, values hard coded everywhere.
#define USE_FIRST	0
#define USE_LAST	1
#define USE_FILE	2

// Stuff that has nothing to do with the config file.
// Basically things that should not be cleared when
// config file is reloaded.  All values initialized to 0.
struct State {
	// D3d resolution.
	RECT d3dRect;
	// What resolution the game things it is.
	RECT gameRect;
	char displayingHelp;
	char debug;
	char logging;
	char Excluded;
	char regOnly;
	char forceReg;
	char hakoEditor;
	char stealingFunctions[2];


	char myPath[MAX_PATH];
	char *myName;
	wchar_t exePath[MAX_PATH];
	wchar_t *exeName;
	GameID game;
	GameHandler *gameHandler;
	char gameName[64];

	wchar_t hakoActiveCDT[MAX_PATH];
	char hakoCachedCDT[256];
	char hakoCachedCharName[100];

	// Used for formatting text.  These are actual sizes,
	// not configured sizes.
	int fontHeight;
	int spaceWidth;
};

extern State state;


struct Config {
	char ag3Names;
	char hakoRomanizeNames;
	char hideGirlNames;
	char displayClock;
	char hacksEnabled;
	char viewEnabled;
	char hakoImmortality;
	char ahmNoLimit;
	union {
		unsigned char ahmBreastSizes[3];
		int ahmBreastSizesDword;
	};

	int width;
	int height;

	unsigned int minSubDuration;

	unsigned long defaultColors[6];
	unsigned long defaultShadow[6];
	unsigned long defaultOutline[6];

	wchar_t *forceFontFace;
	int forceFontHeight;

	int numSubFiles;
	wchar_t **subFiles;

	int numExtraChars;
	CharInfo *extraChars;

	int numExeSettings;
	ExeSettings *exeSettings;
	LCID forceLocale;
};
extern Config config;

struct RegistryInfo {
	HKEY root;
	GameID game;
	char *path;
};
extern RegistryInfo regInfo[];

void ConfigDialog(HWND hWnd);

// Don't nuke font on quit, as D3D9 may already be unloaded.
void ClearConfig(int noNukeFont=0);

void LoadConfig();
void SaveConfig(HWND hWndDlg=0);

void LocaleRestart(HWND hWndDlg);
