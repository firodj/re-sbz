#include "Shrink.h"
#include "Subs.h"

#include "pp.h"
#include "Util.h"

#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "D3DTextOverlay.h"

// Could be a bit cleaner, but subs themselves aren't too big.
// May shove be more careful later.
char *subStrings = 0;
int subStringsLen = 0;
Sub *subs = 0;
int numSubs = 0;

FileInfo *fileInfo = 0;
int numFileInfo=0;

int __cdecl compareStrings(const void *v1, const void *v2) {
	return stricmp(((Sub*)v1)->fileName, ((Sub*)v2)->fileName);
}

int __cdecl compareSubs(const void *v1, const void *v2) {
	if (state.game == HAKO) {
		int res = strnicmp(((Sub*)v1)->fileName, ((Sub*)v2)->fileName, 4);
		if (res) return res;
		res = stricmp(((Sub*)v1)->fileName+6, ((Sub*)v2)->fileName+6);
		if (res) return res;
	}
	else if (state.game == YUUSHA || state.game == RG) {
		int res = stricmp(((Sub*)v1)->fileName+3, ((Sub*)v2)->fileName+3);
		if (res) return res;
	}
	else if (state.game == SB_ZERO) {
		int res = strnicmp(((Sub*)v1)->fileName, ((Sub*)v2)->fileName, 2);
		if (res) return res;
		res = stricmp(((Sub*)v1)->fileName+5, ((Sub*)v2)->fileName+5);
		if (res) return res;
	}
	else {
		int res = stricmp(((Sub*)v1)->fileName+1, ((Sub*)v2)->fileName+1);
		if (res) return res;
	}
	return stricmp(((Sub*)v1)->fileName, ((Sub*)v2)->fileName);
}

// Sorts based on start time, too.  Used for qsort.
int __cdecl compareSubsExact(const void *v1, const void *v2) {
	int res = compareSubs(v1, v2);
	if (res) return res;
	if (((Sub*)v1)->start < ((Sub*)v2)->start) return -1;
	return ((Sub*)v1)->start > ((Sub*)v2)->start;
}

int __cdecl compareFileInfo(const void *v1, const void *v2) {
	int c = wcsicmp(((FileInfo*)v1)->ppName, ((FileInfo*)v2)->ppName);
	if (c) return c;
	if (((FileInfo*)v1)->offset < ((FileInfo*)v2)->offset) return -1;
	return ((FileInfo*)v1)->offset > ((FileInfo*)v2)->offset;
}

void ClearSubs() {
	free(subStrings);
	subStrings = 0;
	free(subs);
	subs = 0;
	numSubs = 0;
	for (int i=0; i<numFileInfo; i++) {
		if (fileInfo[i].name)
			free(fileInfo[i].name);
		else
			free(fileInfo[i].ppName);
	}
	free(fileInfo);
	fileInfo = 0;
	numFileInfo = 0;
	subStringsLen = 0;
}

void ShrinkString(char **s) {
	while (**s > 0 && isspace(**s)) {
		s[0]++;
	}
	char *e = strchr(*s, 0);
	while (e != *s && (e[-1] == ' ' || e[-1] == '\t')) e--;
	*e = 0;
}

void LoadSubs(wchar_t *file, int display, int *subbed, int *total, int *skippedLines, int *loadedLines) {
	HANDLE in = CreateFileW(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	int len = 0;
	int i;
	int oldNumSubs = numSubs;
	if (skippedLines && loadedLines) {
		*skippedLines = 0;
		*loadedLines = 0;
	}
	if (in != INVALID_HANDLE_VALUE) {
		int loaded = 0;
		int skipped = 0;
		DWORD high;
		DWORD size = GetFileSize(in, &high);
		if (!high && size) {
			char *subStrings2 = (char*) malloc(size+3);
			if (ReadFile(in, subStrings2, size, &high, 0)) {
				char *out = subStrings2;
				subStrings2[high] = 0;
				subStrings2[high+1] = 0;
				subStrings2[high+2] = 0;
				char *pos = subStrings2;
				// Big endian UTF 16.  Flip to little endian.
				if ((pos[0] == -2 && pos[1] == -1) || (pos[0] == 0 && pos[1] > 0)) {
					for (i=0; i<(int)size; i+=2) {
						char temp = subStrings2[i];
						subStrings2[i] = subStrings2[i+1];
						subStrings2[i+1] = temp;
					}
				}
				// Little endian UTF 16.  Convert to UTF8.
				if ((pos[0] == -1 && pos[1] == -2) || (pos[0] > 0 && pos[1] == 0)) {
					char *utf8 = (char*) malloc(size*2+1);
					int size2;
					if (size2 = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)pos, 1+(size+1)/2, utf8, size*2-1, 0, 0)) {
						free(pos);
						out = pos = subStrings2 = utf8;
						high = size2;
					}
					else free(utf8);
				}
				// Take care of the BOM marker, if there is one.
				if (pos[0] == (char)0xEF && pos[1] == (char)0xBB && pos[2] == (char)0xBF) pos += 3;
				while (*pos) {
					while (*pos > 0 && isspace(*pos)) {
						pos++;
					}
					if (!*pos) break;
					char *end = pos + strcspn(pos, "\r\n");
					if (*end != 0) {
						*end = 0;
						end ++;
					}
					if (pos[0] != '/' && pos[1] != '/') {
						char *v[4];
						v[0] = strtok(pos, "|");
						ShrinkString(&v[0]);
						if (((v[0][0] == '#' && (v[1] = "0") && (v[2] = "3000")) ||
							 (((v[1] = strtok(0, "|")) && (v[2] = strtok(0, "|"))))) &&
							(v[3] = strtok(0, ""))) {
								ShrinkString(&v[3]);
								int start = atoi(v[1]);
								int end = atoi(v[2]);
								if (!v[1][strspn(v[1], "1234567890 \t")] &&
									!v[2][strspn(v[2], "1234567890 \t")] &&
									start <= end && start >= 0 && end <120000) {
										if (!oldNumSubs || !bsearch(&v[0], subs, oldNumSubs, sizeof(Sub), compareSubs)) {
											loaded++;
											if (numSubs % 1000 == 0) {
												subs = (Sub*) realloc(subs, sizeof(Sub) * (numSubs+1000));
											}
											subs[numSubs].start = start;
											subs[numSubs].end = end;

											if (!numSubs || stricmp(v[0], subs[numSubs-1].fileName)) {
												size_t len = 1+strlen(v[0]);
												memmove(out, v[0], len);
												subs[numSubs].fileName = out;
												out += len;
											}
											else {
												subs[numSubs].fileName = subs[numSubs-1].fileName;
											}

											size_t len = 1+strlen(v[3]);
											memmove(out, v[3], len);
											subs[numSubs].text = out;
											out += len;
											numSubs++;
										}
										else skipped++;
								}
						}
					}
					pos = end;
				}
				// Saves ~200k RAM.  Nothing huge, but better than nothing.
				char *old = subStrings;
				int oldLen = subStringsLen;
				subStrings = (char*)realloc(subStrings, subStringsLen+=(out-subStrings2));
				char *newBase = subStrings + oldLen;

				for (i=0; i<oldNumSubs; i++) {
					subs[i].fileName = subStrings + (subs[i].fileName-old);
					subs[i].text = subStrings + (subs[i].text-old);
				}
				for (; i<numSubs; i++) {
					strcpy(newBase + (subs[i].fileName-subStrings2), subs[i].fileName);
					subs[i].fileName = newBase + (subs[i].fileName-subStrings2);
					strcpy(newBase + (subs[i].text-subStrings2), subs[i].text);
					subs[i].text = newBase + (subs[i].text-subStrings2);
				}
				free(subStrings2);
				qsort(subs, numSubs, sizeof(Sub), compareSubsExact);
				if (display) {
					wchar_t temp[1000];
					swprintf(temp, L"%i lines loaded from %s (%i skipped)", loaded, file, skipped);
					AddText(15000, temp, -1);
				}
				if (skippedLines && loadedLines) {
					loadedLines[0] = loaded;
					skippedLines[0] = skipped;
				}
			}
			else {
				free(subStrings);
				subStrings=0;
			}
		}
		CloseHandle(in);
	}
	if (subbed && total) {
		for (i=0; i<256; i++) {
			subbed[i] = total[i] = 0;
		}
		for (i=0; i<numSubs; i++) {
			if (subs[i].fileName[0] == '#') continue;
			if (!i || stricmp(subs[i-1].fileName, subs[i].fileName)) {
				unsigned char c;
				if (state.game != HAKO) c = toupper(subs[i].fileName[0]);
				else {
					c = 1+atoi(subs[i].fileName+4);
				}
				total[c]++;
				if (subs[i].text[0]) subbed[c]++;
			}
		}
		for (i=1; i<256; i++) {
			subbed[0] += subbed[i];
			total[0]  += total[i];
		}
	}
}

FileInfo *GetSubsAndHeaderInfo(wchar_t *ppFile, __int64 startOffset) {
	FileInfo temp;
	temp.ppName = ppFile;
	temp.offset = startOffset;
	FileInfo *info = (FileInfo *)bsearch(&temp, fileInfo, numFileInfo, sizeof(FileInfo), compareFileInfo);
	if (info) return info;
	temp.offset = 0;
	if (bsearch(&temp, fileInfo, numFileInfo, sizeof(FileInfo), compareFileInfo)) return 0;
	int error;
	PPHeader *pp = LoadPP(ppFile, &error, Dunno);
	if (!pp) {
		wchar_t temp[1000];
		swprintf(temp, L"Can't open file: %s", ppFile);
		AddText(50000, temp, -1);
		return 0;
	}
	fileInfo = (FileInfo*)realloc(fileInfo, sizeof(FileInfo)*(numFileInfo+1+pp->numFiles));
	// Value that means the file has been loaded.
	memset(fileInfo + numFileInfo, 0, sizeof(FileInfo)*(1+pp->numFiles));
	fileInfo[numFileInfo].ppName = wcsdup(ppFile);
	numFileInfo++;
	for (int i=0; i<pp->numFiles; i++) {
		fileInfo[numFileInfo].ppName = fileInfo[numFileInfo-1].ppName;
		fileInfo[numFileInfo].name = strdup(pp->headers[i].name);
		char *w = strrchr(fileInfo[numFileInfo].name, '.');
		//if (w && !stricmp(w, ".ogg")) {
		//	strcpy(w, ".wav");
		//}
		fileInfo[numFileInfo].name = strdup(pp->headers[i].name);
		fileInfo[numFileInfo].offset = pp->headers[i].offset;
		fileInfo[numFileInfo].size = pp->headers[i].size;
		fileInfo[numFileInfo].format = pp->format;

		Sub *s = (Sub*) bsearch(&fileInfo[numFileInfo].name, subs, numSubs, sizeof(Sub), compareSubs);
		if (s) {
			while (s > subs && !stricmp(s[0].fileName, s[-1].fileName)) {
				s--;
			}
			fileInfo[numFileInfo].subs = s;
			while (s < subs+numSubs-1 && !stricmp(s[0].fileName, s[1].fileName)) {
				s++;
			}
			fileInfo[numFileInfo].numSubs = 1+(int)(s - fileInfo[numFileInfo].subs);
		}

		char temp = fileInfo[numFileInfo].name[0];
		fileInfo[numFileInfo].name[0] = '#';
		fileInfo[numFileInfo].extraSub = (Sub*) bsearch(&fileInfo[numFileInfo].name, subs, numSubs, sizeof(Sub), compareSubs);
		if (fileInfo[numFileInfo].extraSub && !fileInfo[numFileInfo].extraSub->text[0]) {
			fileInfo[numFileInfo].extraSub = 0;
		}
		fileInfo[numFileInfo].name[0] = temp;

		numFileInfo++;
	}
	FreePP(pp);
	qsort(fileInfo, numFileInfo, sizeof(FileInfo), compareFileInfo);
	temp.offset = startOffset;
	return (FileInfo *)bsearch(&temp, fileInfo, numFileInfo, sizeof(FileInfo), compareFileInfo);
};

void LoadConfigSubs(int display, int *subbed, int *total) {
	// Shouldn't be needed, but just in case...
	ClearSubs();

	for (int i=0; i<config.numSubFiles; i++)
		LoadSubs(config.subFiles[i], display, subbed, total);
}

void LoadSubsFromPPs(wchar_t *path, wchar_t **files, int numFiles) {
	int error;
	int i;
	PPHeader ** pps = (PPHeader**) malloc(sizeof(PPHeader) * numFiles);
	int maxSubs=0, maxStringLen=0;
	ClearSubs();
	for (i=0; i<numFiles; i++) {
		wchar_t temp[2*MAX_PATH];
		swprintf(temp, L"%s\\%s", path, files[i]);
		pps[i] = LoadPP(temp, &error, Dunno);
		if (pps[i]) {
			maxSubs += pps[i]->numFiles;
			for (int j=0; j<pps[i]->numFiles; j++) {
				maxStringLen += 1+(int)strlen(pps[i]->headers[j].name);
			}
		}
	}
	subStrings = (char*)malloc(maxStringLen);
	subs = (Sub*)malloc(sizeof(Sub) * maxSubs);

	for (i=0; i<numFiles; i++) {
		if (pps[i]) {
			HANDLE f = INVALID_HANDLE_VALUE;
			for (int j=0; j<pps[i]->numFiles; j++) {
				if (ExtensionIs(pps[i]->headers[j].name, ".wav") || ExtensionIs(pps[i]->headers[j].name, ".ogg")) {
					int size = pps[i]->headers[j].size;
					if (pps[i]->headers[j].size > 100) pps[i]->headers[j].size = 100;
					if (LoadFile(pps[i]->headers+j, pps[i]->file, &f, pps[i]->format)) {
						int len = 0;
						if (!ExtensionIs(pps[i]->headers[j].name, ".ogg"))
							len = GetPlayLen(pps[i]->headers[j].data, size, pps[i]->format);
						Sub *sub = subs+numSubs;
						sub->text = 0;
						sub->start = 0;
						sub->end = len;
						sub->fileName = subStrings + subStringsLen;
						strcpy(subStrings + subStringsLen, pps[i]->headers[j].name);
						subStringsLen += 1 + strlen(sub->fileName);
						numSubs++;
					}
				}
			}
			if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
			FreePP(pps[i]);
		}
	}
	qsort(subs, numSubs, sizeof(Sub), compareSubsExact);
}

void SaveSubs(FILE *out) {
	int maxLen = 0;
	int i;
	for (i=0; i<numSubs; i++) {
		int len = strlen(subs[i].fileName);
		if (maxLen < len) maxLen = len;
	}
	char lineTemplate[32];
	sprintf(lineTemplate, "%%-%is | %%5i | %%5i | ", maxLen);
	for (i=0; i<numSubs; i++) {
		fprintf(out, lineTemplate, subs[i].fileName, subs[i].start, subs[i].end);
		if (subs[i].text) fprintf(out, "%s", subs[i].text);
		// else (fprintf(out, "%s", subs[i].fileName));
		fprintf(out, "\r\n");
		int br = (i == numSubs-1);
		if (!br) {
			if (state.game == HAKO) {
				if (strnicmp(subs[i].fileName, subs[i+1].fileName, 4) || stricmp(subs[i].fileName+6, subs[i+1].fileName+6))
					br = 1;
			}
			else if (state.game == YUUSHA || state.game == RG) {
				if (stricmp(subs[i].fileName+2, subs[i+1].fileName+2))
					br = 1;
			}
			else if (state.game == SB_ZERO) {
				if (strnicmp(subs[i].fileName, subs[i+1].fileName, 2))
					br = 1;
				if (stricmp(subs[i].fileName+5, subs[i+1].fileName+5))
					br = 1;
			}
			else if (stricmp(subs[i].fileName+1, subs[i+1].fileName+1)) {
				br = 1;
			}
		}
		if (br) {
			if (state.game == HAKO) {
				fprintf(out, "//%c%c_##_%s: \r\n\r\n\r\n", subs[i].fileName[0], subs[i].fileName[1], subs[i].fileName[2], subs[i].fileName+7);
			}
			else if (state.game == YUUSHA || state.game == RG) {
				char *suffix = subs[i].fileName+2;
				if (suffix[0] == '_') suffix++;
				fprintf(out, "//%s: \r\n\r\n\r\n", suffix);
			}
			else if (state.game == SB_ZERO) {
				char *suffix = subs[i].fileName+5;
				fprintf(out, "//%c%c_x_%s: \r\n\r\n\r\n", subs[i].fileName[0], subs[i].fileName[1], suffix);
			}
			else {
				char *suffix = subs[i].fileName+1;
				if (suffix[0] == '_') suffix++;
				fprintf(out, "//%s: \r\n\r\n\r\n", suffix);
			}
		}
	}
	fclose(out);
}

