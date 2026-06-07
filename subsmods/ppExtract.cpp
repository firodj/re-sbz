#include "Shrink.h"
#include <Windows.h>
#include "pp.h"
#include "Util.h"
#include <locale.h>

// Makes exe a little smaller.
#ifndef _DEBUG
	#if (_MSC_VER<1300)
		#pragma comment(linker,"/merge:.text=.data")
		#pragma comment(linker,"/merge:.rdata=.data")
		#pragma comment(linker,"/merge:.reloc=.data")
		#pragma comment(linker,"/ignore:4078")
	#endif
#endif

char *Help =
"Usage: PPExtractor [/retro | /noretro] [pp file list]\n"
"       PPExtractor [/nobackup] [/merge] [directory list]\n"
"       PPExtractor /extract [pp file] [file list]\n"
"Switches:\n"
"/nobackup - Prevents creation of .old files.  Won't delete existing .old files.\n"
"/merge - Merges directories with pre-existing pp files.  PP file must exist.\n"
"         Combined with /nobackup, is much faster when all files in the folder\n"
"         have a corresponding file in the pp file of the exact same size.\n"
"         If directory name is \"*-merge-<pp file name>\", will automatically\n"
"         enable merge for that directory, merging into the specified pp file.\n"
"/retro - Extract *.ema files as ema-*.bmp/tga.  This is the default for\n"
"         non-Hako pp files.  Needed for compatibility with mods for use with\n"
"         IluPak or Alamar's decrypters.  Ignored when creating pps.\n"
"/noretro - Extract *.ema files as *.ema.bmp/tga.  This is the default for Hako\n"
"           pp files.  Needed for compatibility for mods designed for use with\n"
"           PPExtractor.  Ignored when creating pps.\n"
"/extract - Opposite of /merge.  Will only extract the names files from the pp.\n"
"           .ema files identified as .ema will use the current renaming scheme\n"
"           (retro/noretro).  Those identified using either renaming scheme\n"
"           (ema-*.bmp/tga or *.ema.bmp/tga) will be extracted using that naming\n"
"           scheme.\n"
"/<format> - Where format is one of Hako, AG3, RL, DT, SB3, SM, SMS, SM2, AHM,\n"
"            EskMate, Yuusha, YuushaTrial, CharacolleBench, Null, or\n"
"            SBZeroRetail. Disables format autodetection and forces using the\n"
"            specified format for all listed files.  Null inserts/extracts\n"
"            without decryption.\n"
"\n";

int forceRetro = 0;
int forceNoRetro = 0;

int ChooseRetro(PPHeader *pp) {
	if (forceRetro) return 1;
	if (forceNoRetro) return 0;
	if (pp->format->gameID != HAKO) return 1;
	return 0;
}

int __cdecl wmain(int argc, wchar_t* argv[]) {
	setlocale (LC_NUMERIC,"");
	setlocale (LC_MONETARY,"");
	struct lconv * conf = localeconv();
	int i;
	int retro;
	WIN32_FIND_DATAW findData;
	HANDLE hFind;

	int nobackup = 0;
	int merge = 0;
	int extract = 0;
	static char noad = 0;
	if (!noad) {
		printf (PP_EXTRACTOR_NAME " by ScumSuckingPig\n\n");
		noad = 1;
	}
	for (i=1; i<argc; i++) {
		// Just to be safe.
		if (wcslen(argv[i]) > MAX_PATH) {
			printf ("Argument longer than MAX_PATH (%i)\n", MAX_PATH);
			exit(0);
		}
	}
	i = 1;
	GameID forceFormat = Dunno;
	wchar_t *gameIDString = 0;
	while (i < argc && (argv[i][0] == '/' || argv[i][0] == '-')) {
		wchar_t *command = argv[i]+1;
		if (!wcsicmp(command, L"nobackup")) {
			nobackup = 1;
		}
		else if (!wcsicmp(command, L"merge")) {
			merge = 1;
		}
		else if (!wcsicmp(command, L"extract")) {
			extract = 1;
		}
		else if (!wcsicmp(command, L"retro")) {
			forceRetro = 1;
		}
		else if (!wcsicmp(command, L"noretro")) {
			forceNoRetro = 1;
		}
		else {
			GameID id = GetGameID(command);
			if (id == Dunno || forceFormat != Dunno) {
				printf(Help);
				return 0;
			}
			gameIDString = argv[i];
			forceFormat = id;
		}
		i++;
	}
	if (argc == i) {
		hFind = FindFirstFileW(L"*.pp", &findData);
		printf(Help);

		if (hFind != INVALID_HANDLE_VALUE) {
			if (!merge && IDOK == MessageBox(0, "Extract all pp files in directory?", "Extract all", MB_OKCANCEL)) {
				do {
					if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
						if (gameIDString) {
							wchar_t *temp[3] = {0, gameIDString, findData.cFileName};
							wmain(3, temp);
						}
						else {
							wchar_t *temp[2] = {0, findData.cFileName};
							wmain(2, temp);
						}
					}
					printf("\n");
				}
				while (FindNextFileW(hFind, &findData));
			}
			FindClose(hFind);
		}
	}
	else for (; i<argc; i++) {
		int error = 0;
		wchar_t tempPath[2*MAX_PATH];
		wchar_t outPath[2*MAX_PATH] = L"";
		size_t temp = wcslen(argv[i]);
		int nukeMergeFile = 0;
		FILE *in;
		int cantMerge = 0;

		PPHeader *pp = 0;
		DWORD attrib = GetFileAttributesW(argv[i]);
		if (attrib == INVALID_FILE_ATTRIBUTES) {
			wprintf(L"%s: Can't find file or directory.\n", argv[i]);
			continue;
		}
		if (!(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
			if (!merge) {
				pp = LoadPP(argv[i], &error, forceFormat);
				if (pp) {
					int retro = ChooseRetro(pp);
					if (pp->format->gameID == AUTO_SM) {
						wprintf(L"%s: Unrecognized SM subformat guessed from game exe.\n", argv[i]);
					}
					wprintf(L"%s: Extracting files.\n", argv[i]);
					if (extract) {
						if (argc-i-1 <= 0)
							wprintf(L"%s: Nothing to do.\n", argv[i]);
						else
							ExtractPP(pp, retro, argv+i+1, argc-i-1);
						FreePP(pp);
						return 0;
					}
					ExtractPP(pp, retro, 0, 0);
					FreePP(pp);
					continue;
				}
				else {
					wprintf(L"%s: Error reading file.\n", argv[i]);
				}
			}
			else {
				wprintf(L"%s: Must use directories as arguments with /merge.\n", argv[i]);
			}
			continue;
		}

		if (extract) {
			wprintf(L"%s: Can't use /extract with directories.\n", argv[i]);
			return 0;
		}

		while (temp && (argv[i][temp-1] == '\\' || argv[i][temp-1] == '/')) temp--;
		if (error || !temp) {
			wprintf(L"%s: Problem reading file.\n", argv[i]);
			continue;
		}
		argv[i][temp] = 0;

		// Destination pp file.  If merge directory, will chop off merge suffix in next step.
		// Only used if merging.
		wsprintfW(tempPath, L"%s.pp", argv[i]);

		wchar_t *dirStart = tempPath;
		wchar_t *subPath = wcsrchr(dirStart, '\\');
		if (subPath) dirStart = subPath+1;
		subPath = wcsrchr(dirStart, '/');
		if (subPath) dirStart = subPath+1;

		for (unsigned int w = dirStart-tempPath; w<temp; w++) {
			if (!wcsnicmp(&argv[i][w], L"Merge", 5)) {
				if ((w == (unsigned int)(dirStart-tempPath) || argv[i][w-1] == ' ' || argv[i][w-1] == '-') &&
					(argv[i][w+5] == ' ' || argv[i][w+5] == '-') &&
					argv[i][w+6]) {
						wsprintfW(dirStart, L"%s.pp", argv[i]+w+6);
						merge |= 2;
						break;
				}
			}
		}

		if (!merge) {
			pp = CreatePP(argv[i], 0, forceFormat);
		}
		else {
			pp = LoadPP(tempPath, &error, forceFormat);
			if (!pp) {
				wprintf(L"%s: Problem reading file.\n", tempPath);
				continue;
			}
		}
		if (!pp) continue;
		wcscpy(outPath, pp->file);

		wsprintfW(tempPath, L"%s.old", pp->file);
		if (!nobackup) {
			in = _wfopen(tempPath, L"rb");
			if (!in) {
				MoveFileW(pp->file, tempPath);
				wcscpy(pp->file, tempPath);
			}
			else {
				fclose(in);
			}
		}
		in = _wfopen(outPath, L"rb");
		if (in) {
			fclose(in);
			if (!merge) DeleteFileW(outPath);
			else {
				wsprintfW(pp->file, L"%s_merge", pp->file);
				MoveFileW(outPath, pp->file);
				nukeMergeFile = 1;
			}
		}

		wcscpy(tempPath, argv[i]);
		wcscpy(tempPath+temp, L"\\*");

		hFind = FindFirstFileW(tempPath, &findData);

		retro = ChooseRetro(pp);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					char fileName[MAX_PATH*4];
					BOOL junk;
					WideCharToMultiByte(932, 0, findData.cFileName, -1, fileName, sizeof(fileName), "_", &junk);
					if (strlen(fileName) > 240 ||
						ExtensionIs(fileName, ".old") ||
						ExtensionIs(fileName, ".exe") ||
						!stricmp(fileName, "Thumbs.db")) continue;
					wcscpy(tempPath+1+temp, findData.cFileName);
					pp = UpdatePP(pp, fileName, tempPath, &cantMerge);
				}
			}
			while (FindNextFileW(hFind, &findData));
			FindClose(hFind);
		}
		if (pp->numFiles) {
			if (pp->format->gameID == AUTO_SM) {
				wprintf(L"%s: Unrecognized SM subformat guessed from game exe.\n", outPath);
			}
			if (nukeMergeFile && !cantMerge) {
				wprintf(L"%s: Fast merging.\n", outPath);
				nukeMergeFile = 0;
				MoveFileW(pp->file, outPath);
				wcscpy(pp->file, outPath);
				if (!MergePP(pp)) {
					wprintf(L"%s: Merge failed. File may be corrupt\n", outPath);
				}
			}
			else {
				wprintf(L"%s: Writing to file.\n", outPath);
				if (!SavePP(pp, outPath)) {
					wprintf(L"%s: Creation failed. File may be corrupt\n", outPath);
				}
			}
			printf("\n");
		}
		else {
			wprintf(L"%s: No files to add.\n", argv[i]);
		}
		if (nukeMergeFile) {
			DeleteFileW(pp->file);
		}
		FreePP(pp);
		// Clear merge flag if detected a merge directory.
		merge &= 1;
	}
	return 0;
}

