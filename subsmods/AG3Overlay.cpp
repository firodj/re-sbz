/* Contains code to intercept ReadFile and Direct3DCreate9 functions.
 * Also contains function that actually monitors ReadFile, looks for the file
 * in pp headers and finds text, girl's name, and girl's id.
 *
 * Despite file name, no longer has any AG3-specific code.
 *
 * D3DTextOverlay.cpp handles the actual display of text.
 */

#include "Shrink.h"
#include <Time.h>
#include <d3dx9.h>
#include <stdio.h>
#include <commctrl.h>
#include "D3DTextOverlay.h"
#include "myIDirect3DDevice9.h"
#include "Subs.h"
#include "resource.h"
#include "Config.h"
#include "Util.h"
#include "pp.h"
#include "GameHandler.h"

HINSTANCE hInst;

char *lastLines[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void DisplayHelp() {
	AddText(20000, "     Ctrl-Z changes subtitle debug mode", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-L changes logging mode", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-B logs last 15 sound files", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-R reloads subs and config file", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-C clears displayed text", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-H displays hotkey descriptions", D3DCOLOR_RGBA(0,255,255,255));
	if (state.gameHandler)
		state.gameHandler->DisplayHelp();
}

int HandleKeyPress(int c, int control, int shift) {
	if (state.Excluded) return 0;
	if (state.gameHandler) {
		if (state.gameHandler->HandleKeyPress(c, control, shift)) return 1;
	}
	int w = VK_NUMPAD1;
	if (c == VK_F11) {
		CleanupAllLines();
	}
	if (!control) return 0;
	if (c == 'R') {
		int subbed[256], total[256];
		char temp[1000];
		CleanupAllLines();
		LoadConfig();
		LoadConfigSubs(1, subbed, total);
		sprintf(temp, "Subs and config files reloaded.  %0.1f%% of %i timed files subbed.", 100*subbed[0]/(float)total[0], total[0]);
		AddText(15000, temp, D3DCOLOR_RGBA(255,255,255,255));
		for (int i=1; i<256; i++) {
			if (total[i]) {
				if (state.game != HAKO)
					sprintf(temp, "     %c: %0.1f%% of %i lines.", (char)i, 100*subbed[i]/(float)total[i], total[i]);
				else
					sprintf(temp, "     %02i: %0.1f%% of %i lines.", i-1, 100*subbed[i]/(float)total[i], total[i]);
				AddText(15000, temp, D3DCOLOR_RGBA(0,255,255,255));
			}
		}
	}
	else if (c == 'Z') {
		state.debug = (state.debug+1)%5;
		if (state.debug == 0) {
			AddText(3000, "Debug Mode 0: Disabled", D3DCOLOR_RGBA(100,100,100,255));
		}
		else if (state.debug == 1) {
			AddText(3000, "Debug Mode 1: Sounds with text only", D3DCOLOR_RGBA(145,145,145,255));
		}
		else if (state.debug == 2) {
			AddText(3000, "Debug Mode 2: Sounds with timings only", D3DCOLOR_RGBA(185,185,185,255));
		}
		else if (state.debug == 3) {
			AddText(3000, "Debug Mode 3: All sounds associated with a girl", D3DCOLOR_RGBA(225,225,225,255));
		}
		else if (state.debug == 4) {
			AddText(3000, "Debug Mode 4: All sounds", D3DCOLOR_RGBA(255,255,255,255));
		}
	}
	else if (c == 'L') {
		state.logging = (state.logging+1)%6;
		if (state.logging == 0) {
			AddText(3000, "Log Mode 0: Disabled", D3DCOLOR_RGBA(100,100,100,255));
		}
		else if (state.logging == 1) {
			AddText(3000, "Log Mode 1: Sounds with text only", D3DCOLOR_RGBA(145,145,145,255));
		}
		else if (state.logging == 2) {
			AddText(3000, "Log Mode 2: Sounds with timings only", D3DCOLOR_RGBA(185,185,185,255));
		}
		else if (state.logging == 3) {
			AddText(3000, "Log Mode 3: All sounds associated with a girl", D3DCOLOR_RGBA(225,225,225,255));
		}
		else if (state.logging == 4) {
			AddText(3000, "Log Mode 4: All sounds", D3DCOLOR_RGBA(255,255,255,255));
		}
		else if (state.logging == 5) {
			AddText(3000, "Log Mode 5: All reads. Log will get quite big. For debugging stability issues.", D3DCOLOR_RGBA(255,255,255,255));
		}
	}
	else if (c == 'B') {
		FILE *out = fopen("log.txt", "ab");
		int count = 0;
		static int lines = 0;
		for (int i=0; i<sizeof(lastLines)/sizeof(lastLines[0]); i++) {
			if (lastLines[i]) {
				fprintf(out, "%s\r\n\r\n", lastLines[i]);
				count++;
			}
		}
		fclose(out);
		char temp[100];
		sprintf(temp, "Last %i lines logged", count);
		AddText(3000, temp, D3DCOLOR_RGBA(255,255,255,255));
	}
	else if (c == 'C') {
		CleanupAllLines();
	}
	else if (c == 'H') {
		CleanupAllLines();
		DisplayHelp();
	}
	else return 0;
	return 1;
}





// Credit for this function goes to Nasser R. Rowhani.
// http://www.codeproject.com/KB/DLL/DLL_Injection_tutorial.aspx
void *OverrideFunction(wchar_t *stealFromModuleName, char *oldFunctionModule, char *giveToModuleName, char *functionName, void *newFunction) {

	HMODULE stealFrom = GetModuleHandleW(stealFromModuleName);
	HMODULE giveTo = GetModuleHandleA(giveToModuleName);
	HMODULE oldModule = GetModuleHandleA(oldFunctionModule);
	if (!stealFrom || !giveTo || !oldModule) return 0;
	void *originalAddress = GetProcAddress(oldModule, functionName);
	if (!originalAddress) return 0;
	IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER*) stealFrom;
	char *base = (char*)stealFrom;
	if (IsBadReadPtr(dosHeader, sizeof(IMAGE_DOS_HEADER)) || dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;
	IMAGE_NT_HEADERS *ntHeader = (IMAGE_NT_HEADERS*) (base + dosHeader->e_lfanew);
	if (IsBadReadPtr(ntHeader, sizeof(IMAGE_NT_HEADERS)) || ntHeader->Signature != IMAGE_NT_SIGNATURE)
		return 0;
	if (!ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) return 0;
	IMAGE_IMPORT_DESCRIPTOR *import = (IMAGE_IMPORT_DESCRIPTOR*) (base + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	while (import->Name) {
		char *name = base + import->Name;
		if (!stricmp(name, oldFunctionModule)) break;
		import++;
	}
	if (!import->Name) return 0;
	IMAGE_THUNK_DATA *thunk = (IMAGE_THUNK_DATA*) (base + import->FirstThunk);
	while (thunk->u1.Function) {
		if ((DWORD)thunk->u1.Function == (DWORD) originalAddress) {
			DWORD* addr = (DWORD*)&thunk->u1.Function;

			MEMORY_BASIC_INFORMATION mbi;

			if(VirtualQuery((LPVOID)(addr), &mbi, sizeof(mbi)) == sizeof(mbi)) {
				DWORD dwOldProtect;
				if (VirtualProtect(mbi.BaseAddress, ((DWORD)addr + 4)-(DWORD)mbi.BaseAddress, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
					*addr = (DWORD) newFunction;
					VirtualProtect(mbi.BaseAddress, ((DWORD)addr + 4)-(DWORD)mbi.BaseAddress, dwOldProtect, &dwOldProtect);
					return originalAddress;
				}
			}

		}
		thunk++;
	}

	return 0;
}




int GetCharInfo(CharInfo &c, int *stack, wchar_t *ppFile, char *waveName) {
	wchar_t *fname = wcsrchr(ppFile, '\\');
	if (fname) fname++;
	else fname = ppFile;
	if (state.gameHandler && state.gameHandler->GetCharInfo(c, stack, fname, waveName)) return 1;

	c.name = 0;
	c.color = config.defaultColors[0];
	c.outline = config.defaultOutline[0];
	c.shadow = config.defaultShadow[0];
	c.game = Dunno;

	int out = 0;
	for (int i=0; i<config.numExtraChars; i++) {
		if (config.extraChars[i].file) {
			if (!wcsicmp(config.extraChars[i].file, fname)) {
				c.color = config.extraChars[i].color;
				c.shadow = config.extraChars[i].shadow;
				c.outline = config.extraChars[i].outline;
				c.name = config.extraChars[i].name;
				if (c.game == Dunno) c.game = Custom;
				out = 1;
			}
		}
	}
	return out;
}

// Caching file info when files are openned is much faster than
// querying handles for their file names.  Blame Microsoft.
struct OpenFileInfo {
	__int64 pos;
	wchar_t *path;
	wchar_t *name;
	wchar_t *ext;
	HANDLE handle;
	int reallyOpen;
};
// Static array makes the code threadsafe,
// as long as two threads aren't fooling
// around with the same handle at the same time.
OpenFileInfo openFiles[8] = {0};


OpenFileInfo *FindOpenFile(HANDLE hFile) {
	for (int i=0; i<sizeof(openFiles)/sizeof(openFiles[0]); i++) {
		if (openFiles[i].handle == hFile) return &openFiles[i];
	}
	return 0;
}

void DeleteOpenFile(HANDLE hFile) {
	// Since handles are unique, don't need mutex here.
	// Only need it to open files.
	for (int i=0; i<sizeof(openFiles)/sizeof(openFiles[0]); i++) {
		if (openFiles[i].handle == hFile && openFiles[i].reallyOpen) {
			free(openFiles[i].path);
			openFiles[i].reallyOpen = 0;
			return;
		}
	}
}

void DeleteOpenFiles() {
	for (int i=0; i<sizeof(openFiles)/sizeof(openFiles[0]); i++) {
		DeleteOpenFile(openFiles[i].handle);
	}
}

DWORD WINAPI MySetFilePointer(
  __in         HANDLE hFile,
  __in         LONG lDistanceToMove,
  __inout_opt  PLONG lpDistanceToMoveHigh,
  __in         DWORD dwMoveMethod) {

	DWORD res = SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
	// Failure.
	if (res == INVALID_SET_FILE_POINTER && (!lpDistanceToMoveHigh || GetLastError() != 0))
		return res;
	OpenFileInfo *info = FindOpenFile(hFile);
	if (info) {
		__int64 newPos = res;
		if (lpDistanceToMoveHigh) newPos += lpDistanceToMoveHigh[0] * (__int64)0x100000000;
		info->pos = newPos;
	}
	return res;
}

BOOL WINAPI MyCloseHandle(__in  HANDLE hObject) {
	DeleteOpenFile(hObject);
	return CloseHandle(hObject);
}

int AHMUpdateSetup(char *old, int oldSize, char *out) {
	int i;
	for (i=0; i<oldSize; i++) {
		old[i] ^= 0xFF;
	}
	int outSize = 0;
	for (i=0; i<oldSize; i++) {
		if (old[i] == '$') {
			char temp[5];
			memcpy(temp, old+i+1, 4);
			temp[4] = 0;
			int cmd = atoi(temp);
			if (cmd == 24 || cmd == 25) {
				memcpy(out+outSize, old+i, 5);
				outSize += 5;
				i+=5;
				int end = i;
				while (end < oldSize && old[end] != ';') end++;
				int scaledWidth = (config.width * 768+config.height-1) / config.height;
				if (cmd == 24) {
					//int fakeWidth = config.width/(float)config.height * 768;
					int middle = scaledWidth/2;
					// sprintf(out+outSize, "%i,653,%i,758", middle - 467, middle+467);
					//sprintf(out+outSize, "%i,%i,%i,%i", middle - 467, config.height - 115, middle+467, config.height - 8);
					sprintf(out+outSize, "%i,653,%i,758", middle - 467,middle+467);
				}
				else if (cmd == 25) {
					while (i < oldSize && old[i-1] != ',') {
						out[outSize++] = old[i++];
					}
					if (old[i-2] == '0') {
						sprintf(out+outSize, "%i,%i", scaledWidth, 64);
					}
					else {
						sprintf(out+outSize, "%i,%i", scaledWidth, 200);
					}
				}
				// So I'll add back the semi-colon or whatever.
				outSize += strlen(out+outSize);
				i = end-1;
				continue;
			}
		}
		out[outSize++] = old[i];
	}
	for (i=0; i<outSize; i++) {
		out[i] ^= 0xFF;
	}
	return outSize;
}

BOOL WINAPI MyReadFile(
  __in         HANDLE hFile,
  __out        LPVOID lpBuffer,
  __in         DWORD nNumberOfBytesToRead,
  __out_opt    LPDWORD lpNumberOfBytesRead,
  __inout_opt  LPOVERLAPPED lpOverlapped) {
	OpenFileInfo *openFileInfo = FindOpenFile(hFile);
	BOOL res = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	int i;
	if (res && openFileInfo) {
		if (state.game == AHM && config.width && !wcsicmp(openFileInfo->name, L"AHM03_00.pp")) {
			int w = 5;
			static int setupIndexStart = 0;
			static int setupIndexEnd = 0;
			static int setupStart = 0;
			static int setupRealSize = 0;
			static int setupNewSize = 0;
			static int scrambledRealSize = 0;
			static int scrambledNewSize = 0;
			static FormatData *fd;
			if (!setupStart) {
				int error;
				PPHeader *pp = LoadPP(openFileInfo->path, &error, AHM);
				setupIndexStart = -1;
				if (pp) {
					for (i=0; i<pp->numFiles; i++) {
						if (!stricmp(pp->headers[i].name, "!__setup.dat")) {
							HANDLE hTemp = INVALID_HANDLE_VALUE;
							int size;
							char *oldAhmSetup = LoadDecryptedFile(pp, i, &size, &hTemp);
							if (hTemp != INVALID_HANDLE_VALUE) CloseHandle(hTemp);
							if (oldAhmSetup) {
								char *newAhmSetup = (char*)malloc(size + 100);
								setupNewSize = AHMUpdateSetup(oldAhmSetup, size, newAhmSetup);
								if (setupNewSize) {
									fd = pp->format;
									setupIndexStart = 5 + 260*i;
									setupIndexEnd = setupIndexStart + 260;
									setupStart = pp->headers[i].offset32;
									setupRealSize = pp->headers[i].size;
									scrambledRealSize = size;
									DecryptHeader((char*)&scrambledRealSize, 4, i*268+260);
									scrambledNewSize = setupNewSize;
									DecryptHeader((char*)&scrambledNewSize, 4, i*268+260);
								}
								free(newAhmSetup);
								free(oldAhmSetup);
							}
						}
					}
					FreePP(pp);
				}
			}
			else if (nNumberOfBytesToRead == 4 && *(int*)lpBuffer == scrambledRealSize && setupStart) {
				int w = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
				// Position *after* reading the 4 bytes.
				if (w == setupIndexStart + 264) {
					*(int*)lpBuffer = scrambledNewSize;
				}
			}
			else if ((int)nNumberOfBytesToRead == setupNewSize && setupStart) {
				int w = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
				if (w == setupStart+setupNewSize) {
					if (lpNumberOfBytesRead) *lpNumberOfBytesRead = setupRealSize;
					char *newAhmSetup = (char*)malloc(nNumberOfBytesToRead + 100);
					Decrypt((char*)lpBuffer, setupRealSize, fd);
					AHMUpdateSetup((char*)lpBuffer, setupRealSize, newAhmSetup);
					memcpy(lpBuffer, newAhmSetup, setupNewSize);
					Decrypt((char*)lpBuffer, setupNewSize, fd);
					free(newAhmSetup);
				}
				/*
				int offset = w-5-260;
				if (w != INVALID_SET_FILE_POINTER && offset>=0 && offset <= 260000) {
					char header[260];
					while (1) {
						memcpy(header, (char*)lpBuffer, 4);
						DecryptHeader(header, 4, offset);
						header[0] = header[0];
						break;
					}
				}//*/
			}
			w=w;
		}
		else if (!wcsicmp(openFileInfo->ext, L".pp")) {
			if (state.logging == 5) {
				FILE *out = fopen("log.txt", "ab");
				fprintf(out, "%ls\t%I64i\r\n", openFileInfo->path, openFileInfo->pos);
				fclose(out);
			}
			// Debugging code to go through all lines.  Also have to modify GetCharInfo() to avoid crash.
			/*
			static int test = -1;
			extern FileInfo *fileInfo;
			extern int numFileInfo;
			test++;
			if (GetSubsAndHeaderInfo(openFileInfo->path, 0) && test >= 0 && test < numFileInfo) {
				wcscpy(openFileInfo->path, fileInfo[test].ppName);
				pos = fileInfo[test].offset;
			}
			else {
				test=test;
				test = -1;
			}
			//*/
			FileInfo *info;
			if (nNumberOfBytesToRead > 1000 && openFileInfo->pos && (info = GetSubsAndHeaderInfo(openFileInfo->path, openFileInfo->pos))) {
				if (state.logging == 5) {
					FILE *out = fopen("log.txt", "ab");
					fprintf(out, "%s\r\n", openFileInfo->path, openFileInfo->pos);
					fclose(out);
				}
				// Need to query file pointer position before reading, but as I read anyways,
				// best to do it as early as I can so I can stop if it fails.
				if (res && (info->numSubs || (ExtensionIs(info->name, ".wav") || ExtensionIs(info->name, ".ogg"))) && (lpNumberOfBytesRead && *lpNumberOfBytesRead > 1000)) {
					int display = 0;
					int log = 0;
					int debugDisplay = 0;
					if (info->numSubs >= 2 || (info->numSubs == 1 && info->subs->text[0])) {
						display = 1;
						if (state.debug) debugDisplay = 1;
						if (state.logging) log = 1;
					}
					else if (info->numSubs) {
						if (state.debug >= 2) debugDisplay = 1;
						if (state.logging >= 2) log = 1;
					}
					else {
						if (state.debug >= 3) debugDisplay = 1;
						if (state.logging >= 3) log = 1;
					}
					int *stack;
					__asm {
						mov stack, ebp
					}
					CharInfo c;
					GetCharInfo(c, stack, openFileInfo->path, info->name);
					if (c.game == Dunno && !display) {
						if (state.logging <4) log = 0;
						if (state.debug <4) debugDisplay = 0;
					}
					int max = info->numSubs;
					if (!max) max = 1;
					int duration = GetPlayLen((char*)lpBuffer, info->size, info->format);

					int logLen = 40;
					for (i=0; i<max; i++) {
						int len = 10 + strlen(info->name);
						if (info->subs)
							len += strlen(info->subs[i].text);
						if (c.name)
							len += strlen(c.name);
						logLen += len;
						if (debugDisplay || display) {
							char *temp = (char*) malloc(len);
							if (debugDisplay) {
								if (c.name)
									sprintf(temp, "[%s %s] ", c.name, info->name);
								else {
									sprintf(temp, "[%s] ", info->name);
								}
							}
							else {
								if (!config.hideGirlNames && c.name)
									sprintf(temp, "%s:  ", c.name);
								else
									temp[0] = 0;
							}
							if (info->subs && info->subs[i].text[0]) {
								strcat(temp, info->subs[i].text);
								AddText(info->subs[i].start, info->subs[i].end, temp, c.color, c.outline, c.shadow);
							}
							else {
								if (temp[0]) {
									AddText(0, 3000, temp, c.color, c.outline, c.shadow);
								}
							}
							free(temp);
						}
					}
					logLen *=2;
					if (info->extraSub) logLen += 15 + strlen(info->extraSub->text);
					char *temp = (char*) malloc(logLen);
					time_t t = time(0);
					tm *date = localtime(&t);
					char *pos = temp;
					if (info->extraSub) {
						sprintf(pos, "// %s\r\n", info->extraSub->text);
						pos = strchr(pos, 0);
					}
					sprintf(pos, "[%02i:%02i:%02i] ", date->tm_hour, date->tm_min, date->tm_sec);
					pos = strchr(pos, 0);
					if (c.name) {
						sprintf(pos, "%s: ", c.name);
						pos = strchr(pos, 0);
					}
					sprintf(pos, "%s (%i ms)\r\n", info->name, duration);
					pos = strchr(pos, 0);
					if (!info->numSubs) max = 0;
					for (i=0; i<max; i++) {
						sprintf(pos, "%5i |%5i | ", info->subs[i].start, info->subs[i].end);
						pos = strchr(pos, 0);
						strcpy(pos, info->subs[i].text);
						pos = strchr(pos, 0);
						strcpy(pos, "\r\n");
						pos+=2;
					}
					if (log) {
						FILE *out = fopen("log.txt", "ab");
						fprintf(out, "%s\r\n", temp);
						fclose(out);
					}
					free(lastLines[0]);
					for (i=0; i<sizeof(lastLines)/sizeof(lastLines[0])-1; i++)
						lastLines[i]=lastLines[i+1];
					lastLines[i] = strdup(temp);
				}
			}
		}
		else if (state.game == HAKO && !wcsicmp(openFileInfo->ext, L".cdt")) {
			// Code to figure out address of Hako color and color mappings.
			/*if (ExtensionIsW(openFileInfo->name, L".cdt") && STATUS_SUCCESS == ZwQueryopenFileInformationFile(hFile, &ioStatusBlock, (FILE_POSITION_openFileInfoRMATION*)&pos, sizeof(pos), FilePositionopenFileInformation)) {
				int mod = 0x3C;
				res = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
				read = 1;
				if (res && mod >= pos && mod < pos + nNumberOfBytesToRead) {
					char old = ((char*)lpBuffer)[mod-pos]^0xFF;
					int v = 16;
					((char*)lpBuffer)[mod-pos] = ((v)^0xFF);
					unsigned int colors[] = {
						D3DCOLOR_RGBA(255,255,255,255),
						D3DCOLOR_RGBA(0xFF,0xF0,0xC0,255),
						D3DCOLOR_RGBA(0x90,0x20,0x00,255),
						D3DCOLOR_RGBA(0x00,0x00,0x00,255),
						D3DCOLOR_RGBA(0xFF,0xDA,0x17,255),
						D3DCOLOR_RGBA(0xFF,0x82,0x17,255),
						D3DCOLOR_RGBA(0xFF,0x00,0x00,255),
						D3DCOLOR_RGBA(0xD0,0x1B,0x17,255),
						D3DCOLOR_RGBA(0x82,0xCA,0xFA,255),
						D3DCOLOR_RGBA(0x13,0x46,0xFF,255),
						D3DCOLOR_RGBA(0x7A,0xFB,0x62,255),
						D3DCOLOR_RGBA(0x30,0x92,0x30,255),
						D3DCOLOR_RGBA(0xFA,0xAF,0xBE,255),
						D3DCOLOR_RGBA(0xBD,0x32,0xBD,255),
					};
					CleanupAllLines();
					AddText(0, 3000000, "Girl test text", colors[v], D3DCOLOR_RGBA(0,0,0,255), D3DCOLOR_RGBA(0,0,0,80));
				}
			}//*/
			// Hako specific code, to get girl name from last loaded cdt file and rename her, if needed.
			static int invert = 0;
			if (!openFileInfo->pos && nNumberOfBytesToRead>=4) {
				invert = !((char*)lpBuffer)[3];
			}
			if (invert) {
				for (unsigned int i=0; i<nNumberOfBytesToRead; i++) {
					((char*)lpBuffer)[i] ^= 0xFF;
				}
			}
			if (state.hakoEditor) {
				if (state.hakoCachedCDT[0] && openFileInfo->pos + nNumberOfBytesToRead <= 255) {
					for (unsigned int i=0; i<nNumberOfBytesToRead; i++) {
						((char*)lpBuffer)[i] = 0xFF ^ state.hakoCachedCDT[openFileInfo->pos+i];
					}
				}
			}
			else {
				if (res && wcsicmp(openFileInfo->path, state.hakoActiveCDT)) {
					state.hakoCachedCDT[0] = 0;
					state.hakoCachedCharName[0] = 0;
					if (wcslen(openFileInfo->path) >= MAX_PATH) {
						state.hakoActiveCDT[0] = 0;
					}
					else {
						wcscpy(state.hakoActiveCDT, openFileInfo->path);
					}
				}
			}
			if (res && openFileInfo->pos == 4 && nNumberOfBytesToRead == 24 && config.hakoRomanizeNames) {
				char temp[24];
				for (i=0; i<24; i++) {
					temp[i] = ((char*)lpBuffer)[i]^0xFF;
				}
				if (!temp[23]) {
					wchar_t wideName[1024];
					if (MultiByteToWideChar(932, 0, temp, -1, wideName, sizeof(wideName)/sizeof(wchar_t))) {
						wchar_t *name = JISNameToRomanji(wideName);
						if (name) {
							wcscpy(wideName, name);
							free(name);
						}
						AddFunkyASCII(wideName);
						if (WideCharToMultiByte(932, 0, wideName, -1, temp, sizeof(temp), 0, 0)) {
							for (i=0; i<24; i++) {
								((char*)lpBuffer)[i] = temp[i]^0xFF;
							}
						}
					}
				}
			}
		}
		openFileInfo->pos += nNumberOfBytesToRead;
	}
	return res;
}

HANDLE OpenFileMutex = 0;

// Make sure I can open another handle to the open file.
// Could just reuse the handle given to ReadFile, but
// my pp code uses a FILE* instead of a HANDLE.
HANDLE WINAPI MyCreateFileA(
  __in      LPCTSTR lpFileName,
  __in      DWORD dwDesiredAccess,
  __in      DWORD dwShareMode,
  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  __in      DWORD dwCreationDisposition,
  __in      DWORD dwFlagsAndAttributes,
  __in_opt  HANDLE hTemplateFile) {
	HANDLE h = CreateFileA(lpFileName, dwDesiredAccess, FILE_SHARE_READ | dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	if (h != INVALID_HANDLE_VALUE) {
		wchar_t temp[MAX_PATH*2];
		if (MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, temp, sizeof(temp)/sizeof(wchar_t))) {
			WaitForSingleObject(OpenFileMutex, INFINITE);
			for (int i=0; i<sizeof(openFiles)/sizeof(openFiles[0]); i++) {
				if (openFiles[i].reallyOpen) continue;
				openFiles[i].reallyOpen = 1;
				openFiles[i].handle = h;
				openFiles[i].pos = 0;
				openFiles[i].path = wcsdup(temp);
				wchar_t *e = openFiles[i].path;
				while (e = wcschr(e, '/')) {
					*e = '\\';
				}
				e = wcsrchr(openFiles[i].path, '\\');
				if (!e) e = openFiles[i].path;
				else e++;
				openFiles[i].name = e;
				wchar_t *e2 = wcsrchr(e, '.');
				if (!e2) e2 = e;
				openFiles[i].ext = e2;
				break;
			}
			ReleaseMutex(OpenFileMutex);
		}
	}
	return h;
}

// Fake an INSTALLDIR registry entry if there isn't one already.
LONG WINAPI MyRegQueryValueExA(
  __in         HKEY hKey,
  __in_opt     LPCTSTR lpValueName,
  __reserved   LPDWORD lpReserved,
  __out_opt    LPDWORD lpType,
  __out_opt    LPBYTE lpData,
  __inout_opt  LPDWORD lpcbData) {
	LONG res = RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
	if ((res != ERROR_SUCCESS || state.forceReg) && !stricmp(lpValueName, "INSTALLDIR") && lpData && lpcbData) {
		char path[MAX_PATH*2];
		int len = GetModuleFileNameA(0, path, sizeof(path));
		if (len && len < sizeof(path)) {
			char *e = strrchr(path, '\\');
			if (e) *e = 0;
			int len = strlen(path)+1;
			if ((int)*lpcbData >= len) {
				if (lpType) *lpType = REG_SZ;
				strcpy((char*)lpData, path);
				*lpcbData = len;
				return ERROR_SUCCESS;
			}
		}
	}
	return res;
}

LONG WINAPI MyRegOpenKeyExA(
  __in        HKEY hKey,
  __in_opt    LPCTSTR lpSubKey,
  __reserved  DWORD ulOptions,
  __in        REGSAM samDesired,
  __out       PHKEY phkResult) {
	LONG res = RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
	if (!strnicmp("Software\\Illusion\\", lpSubKey, 18)) {
		const char *name = lpSubKey+18;
		int i = 0;
		while (regInfo[i].game != Dunno) {
			if (!stricmp(name, regInfo[i].path)) {
				if (!state.gameHandler) {
					if (state.game == Dunno || state.game == SB3) {
						state.game = regInfo[i].game;
						strcpy(state.gameName, name);
						state.gameName[strlen(state.gameName)-1] = 0;
					}
				}
				break;
			}
			i++;
		}

		if (regInfo[i].game != Dunno) {
			if (res != ERROR_SUCCESS &&
				ERROR_SUCCESS == RegCreateKeyEx(hKey, lpSubKey, 0, 0, 0, KEY_ALL_ACCESS, 0, phkResult, 0))
					return ERROR_SUCCESS;
		}
		else if (res == ERROR_SUCCESS && (!strnicmp(name, "JS3_", 4) || strstr(lpSubKey, "Trial"))) {
			BYTE temp[MAX_PATH];
			DWORD size = sizeof(temp);
			DWORD type = 0;
			if (res == ERROR_SUCCESS && ERROR_SUCCESS != RegQueryValueExA(*phkResult, "INSTALLDIR", 0, &type, temp, &size)) {
				RegCloseKey(*phkResult);
				RegDeleteKey(hKey, lpSubKey);
				res = RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
			}
		}
	}
	return res;
}

DLGPROC stolenDialogFunc = 0;
HWND hWndDefault = 0;

WNDPROC eatenWindowWndProc = 0;
unsigned short BUTTON_ID = 0;

// Not sure what eats my buttons messages, but this seems the only way to get them.
LRESULT CALLBACK WndProcTemp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_COMMAND) {
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == BUTTON_ID) {
				// Temporarily set this to 1 while the dialog's visible, as a flag.
				// If launch with subs, clear it.  Otherwise, it's used to prevent
				// doing anything else.
				state.Excluded = 0;

				// Somewhat hidden, but this is where final initialization before the game
				// starts loading takes place.

				LoadConfigSubs();

				// Display version and shortcut info.  Could do later in the process, but
				// simplest to have it in this file.
				state.gameHandler = CreateGameHandler(state.game);
				AddText(20000, OVERLAY_NAME, D3DCOLOR_RGBA(255,0,0,255));
				DisplayHelp();
				state.displayingHelp = 1;

				// Pretend launch button was pressed.
				wParam = (wParam & ~0xFFFF) | GetDlgCtrlID (hWndDefault);
				lParam = (WPARAM)hWndDefault;
				/*
				if (!hook) {
					hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
				}//*/
			}
			else if (LOWORD(wParam) == BUTTON_ID + 1) {
				ConfigDialog(hWnd);
				return 0;
			}
		}
	}
	else if (uMsg == WM_DESTROY && eatenWindowWndProc) {
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)eatenWindowWndProc);
		eatenWindowWndProc = 0;
	}
	return CallWindowProc(eatenWindowWndProc, hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK StartupDialogProc(      
    HWND hwndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {
		if (uMsg == WM_INITDIALOG) {
			int i;
			RECT rects[1000];
			HWND hWnds[1000];
			HWND hWndChild = 0;
			int count = 0;
			hWndDefault = 0;
			BUTTON_ID = 0;
			while (1) {
				WINDOWINFO info;
				hWndChild = FindWindowEx(hwndDlg, hWndChild, "Button", 0);
				if (!hWndChild) break;
				info.cbSize = sizeof(info);
				if (!GetWindowInfo(hWndChild, &info)) continue;
				int id = (short int) GetDlgCtrlID (hWndChild);
				if (id > BUTTON_ID) {
					BUTTON_ID = id;
				}
				if (!(info.dwStyle&WS_TABSTOP) || !(info.dwStyle&WS_VISIBLE)) continue;
				if (info.dwStyle & 1) {
					hWndDefault = hWndChild;
				}
				GetWindowRect(hWndChild, &rects[count+2]);
				if (rects[count+2].right == rects[count+2].left ||
					rects[count+2].top == rects[count+2].bottom) {
						continue;
				}
				hWnds[2+count++] = hWndChild;
			}
			BUTTON_ID += 0x1111;
			int h = 3;
			int v = 3;
			RECT r = rects[2];
			count +=2;
			for (i=3; i<count; i++) {
				if (rects[i].bottom != r.bottom) {
					if (!v)	continue;
					h &= ~1;
					if (rects[i].bottom > r.bottom) {
						r.bottom = rects[i].bottom;
					}
				}
				if (rects[i].top != r.top) {
					if (!v)	continue;
					h &= ~2;
					if (rects[i].top < r.top) {
						r.top = rects[i].top;
					}
				}
				if (rects[i].left != r.left) {
					if (!h)	continue;
					v &= ~2;
					if (rects[i].left < r.left) {
						r.left = rects[i].left;
					}
				}
				if (rects[i].right != r.right) {
					if (!h)	continue;
					v &= ~1;
					if (rects[i].right > r.right) {
						r.right = rects[i].right;
					}
				}
			}
			if (h != v) {
				ScreenToClient(hwndDlg, (POINT*)&r);
				ScreenToClient(hwndDlg, ((POINT*)&r)+1);
				eatenWindowWndProc = (WNDPROC) SetWindowLongPtr(hwndDlg, GWLP_WNDPROC, (LONG_PTR)WndProcTemp);
				if (h < v) {
					hWnds[0] = CreateWindowEx(WS_EX_NOPARENTNOTIFY, "Button", "Run with Subs", WS_TABSTOP|WS_CHILD|WS_VISIBLE |WS_OVERLAPPED, 0,0,150,150, hwndDlg, (HMENU)BUTTON_ID, hInst, 0);
					hWnds[1] = CreateWindowEx(WS_EX_NOPARENTNOTIFY, "Button", "Config Subs", WS_TABSTOP|WS_CHILD|WS_VISIBLE |WS_OVERLAPPED, 0,0,150,150, hwndDlg, (HMENU)(BUTTON_ID+1), hInst, 0);
					int size = (r.bottom - r.top - 4 * (count-1))/count;
					for (i=0; i<count; i++) {
						MoveWindow(hWnds[i], r.left, r.top, r.right-r.left, size, 1);
						r.top += size + 4;
					}
				}
				else {
					// RL doesn't have a whole lot of space...
					hWnds[0] = CreateWindowEx(WS_EX_NOPARENTNOTIFY, "Button", "Subs", WS_TABSTOP|WS_CHILD|WS_VISIBLE |WS_OVERLAPPED, 0,0,150,150, hwndDlg, (HMENU)BUTTON_ID, hInst, 0);
					hWnds[1] = CreateWindowEx(WS_EX_NOPARENTNOTIFY, "Button", "Cfg", WS_TABSTOP|WS_CHILD|WS_VISIBLE |WS_OVERLAPPED, 0,0,150,150, hwndDlg, (HMENU)(BUTTON_ID+1), hInst, 0);
					int size = (r.right - r.left - 4 * (count-1))/count;
					for (i=0; i<count; i++) {
						MoveWindow(hWnds[i], r.left, r.top, size, r.bottom-r.top, 1);
						r.left += size + 4;
					}
				}
				SendMessage(hWnds[0], WM_SETFONT, SendMessage(hWnds[2], WM_GETFONT, 0, 0), 0);
				SendMessage(hWnds[1], WM_SETFONT, SendMessage(hWnds[2], WM_GETFONT, 0, 0), 0);
				PostMessage(hwndDlg, WM_NEXTDLGCTL, (WPARAM)hWnds[0], 1);
			}
		}
		return stolenDialogFunc(hwndDlg, uMsg, wParam, lParam);
}

void Uninit();

INT_PTR WINAPI MyDialogBoxParamA (
    HINSTANCE hInstance,
    LPCTSTR lpTemplateName,
    HWND hWndParent,
    DLGPROC lpDialogFunc,
    LPARAM dwInitParam) {
		if (stolenDialogFunc) {
			// Not sure this ever happens, but just in case a config dialog uses this.
			return DialogBoxParamA (
				hInstance,
				lpTemplateName,
				hWndParent,
				lpDialogFunc,
				dwInitParam);
		}
		state.Excluded = 1;
		stolenDialogFunc = lpDialogFunc;
		INT_PTR res = DialogBoxParamA (
			hInstance,
			lpTemplateName,
			hWndParent,
			StartupDialogProc,
			dwInitParam);
		stolenDialogFunc = 0;
		if (state.Excluded) {
			ClearConfig();
			Uninit();
		}
		return res;
}

BOOL WINAPI MySetWindowPos(      
    HWND hWnd,
    HWND hWndInsertAfter,
    int X,
    int Y,
    int cx,
    int cy,
    UINT uFlags) {
		// By default, cx and cy are a little higher than the actual res,
		// even when making a full screen window. No idea why.  Seems to work
		// fine when I use the real values instead.
		if (!state.regOnly && !state.Excluded) {
			char name[1000];
			GetClassName(hWnd, name, sizeof(name));
			if (!stricmp(name, "RapeLay") || strstr(name, "_Class__")) {
				WINDOWINFO info;
				info.cbSize = sizeof(info);
				GetWindowInfo(hWnd, &info);
				RECT r = {0,0,cx,cy};
				AdjustWindowRectEx(&r, info.dwStyle & ~(WS_BORDER), 1, info.dwExStyle);
				// Get slightly off values when full screen.  Not sure if I get what I really 
				// want or not.
				state.gameRect.left = state.gameRect.top = 0;
				state.gameRect.right = 2*cx - (r.right-r.left);
				state.gameRect.bottom = 2*cy - (r.bottom-r.top);
				if (config.width && config.height) {
					RECT r = {0,0,config.width,config.height};
					// Don't ask me where the border style goes to.
					AdjustWindowRectEx(&r, info.dwStyle & ~(WS_BORDER), 1, info.dwExStyle);
					cx = r.right - r.left;
					cy = r.bottom - r.top;
				}
			}
		}
		return SetWindowPos(
			hWnd,
			hWndInsertAfter,
			X,
			Y,
			cx,
			cy,
			uFlags
		);
}

int Init() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	if (!ntdll) return 0;
	//RealDirect3DCreate9 = (RealDirect3DCreate9Type*)OverrideFunction(exeName, "d3d9.dll", myName, "Direct3DCreate9", MyDirect3DCreate9);
	if (!state.Excluded) {
		state.stealingFunctions[0] = 1;
		if (!state.regOnly) {
			state.stealingFunctions[1] = 1;
			//void *test = OverrideFunction(config.exeName, "d3d9.dll", state.myName, "Direct3DCreate9", Direct3DCreate9);
			//if (test != (void*) Direct3DCreate9) d3dOverridden = 1;
			OverrideFunction(state.exeName, "kernel32.dll", state.myName, "ReadFile", MyReadFile);
			OverrideFunction(state.exeName, "kernel32.dll", state.myName, "CreateFileA", MyCreateFileA);
			OverrideFunction(state.exeName, "kernel32.dll", state.myName, "SetFilePointer", MySetFilePointer);
			OverrideFunction(state.exeName, "kernel32.dll", state.myName, "CloseHandle", MyCloseHandle);
			OverrideFunction(state.exeName, "user32.dll", state.myName, "SetWindowPos", MySetWindowPos);
			OverrideFunction(state.exeName, "user32.dll", state.myName, "DialogBoxParamA", MyDialogBoxParamA);
		}
		OverrideFunction(state.exeName, "advapi32.dll", state.myName, "RegQueryValueExA", MyRegQueryValueExA);
		OverrideFunction(state.exeName, "advapi32.dll", state.myName, "RegOpenKeyExA", MyRegOpenKeyExA);
	}
	return 1;
}


void Uninit() {
	// Should all be unnecessary, but you never know.
	if (state.stealingFunctions[0]) {
		state.stealingFunctions[0] = 0;
		if (state.stealingFunctions[1]) {
			state.stealingFunctions[1] = 0;
			//if (d3dOverridden) OverrideFunction(config.exeName, "d3d9.dll", "d3d9.dll", "Direct3DCreate9", RealDirect3DCreate9);
			OverrideFunction(state.exeName, "kernel32.dll", "kernel32.dll", "ReadFile", ReadFile);
			OverrideFunction(state.exeName, "kernel32.dll", "kernel32.dll", "CreateFileA", CreateFileA);
			OverrideFunction(state.exeName, "kernel32.dll", "kernel32.dll", "SetFilePointer", SetFilePointer);
			OverrideFunction(state.exeName, "kernel32.dll", "kernel32.dll", "CloseHandle", CloseHandle);
			OverrideFunction(state.exeName, "user32.dll", "user32.dll", "SetWindowPos", SetWindowPos);
			OverrideFunction(state.exeName, "user32.dll", "user32.dll", "DialogBoxParamA", DialogBoxParamA);
		}
		OverrideFunction(state.exeName, "advapi32.dll", "advapi32.dll", "RegQueryValueExA", RegQueryValueExA);
		OverrideFunction(state.exeName, "advapi32.dll", "advapi32.dll", "RegOpenKeyExA", RegOpenKeyExA);
	}
	/*if (hook) {
		UnhookWindowsHookEx(hook);
		hook = 0;
	}
	//*/
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, void* lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DeleteFile("log.txt");
		//SetEnvironmentVariable("__COMPAT_LAYER", "#ApplicationLocale");
		//SetEnvironmentVariable("AppLocaleID", "0411");
		hInst = hInstance;
		DisableThreadLibraryCalls(hInstance);

		GetModuleFileNameA(hInst, state.myPath, sizeof(state.myPath));
		state.myName = strrchr(state.myPath, '\\');
		if (state.myName) state.myName++;
		else state.myName = state.myPath;

		GetModuleFileNameW(0, state.exePath, sizeof(state.exePath));
		state.exeName = wcsrchr(state.exePath, '\\');
		if (state.exeName) state.exeName++;
		else state.exeName = state.exePath;

		LoadConfig();
		if (!state.Excluded) {
			// Checks if need to restart, and does so if necessary.
			LocaleRestart(0);
		}

		OpenFileMutex = CreateMutex(0, 0, 0);

		if (!Init()) return 0;
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		CloseHandle(OpenFileMutex);
		ClearConfig(1);
		for (int i=0; i<10; i++) {
			free(lastLines[i]);
			lastLines[i] = 0;
		}
		CleanupAllLines();
		Uninit();
		if (state.gameHandler) {
			delete state.gameHandler;
			state.gameHandler = 0;
		}
		DeleteOpenFiles();
	}
	return 1;
}
