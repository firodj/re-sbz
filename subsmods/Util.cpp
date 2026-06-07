#include "Shrink.h"
#include "pp.h"

#include "Util.h"

#include <Windows.h>

int GetPlayLen(char *lpBuffer, int size, const FormatData* format) {
	char temp[400];
	int len = 0;
	memcpy(temp, lpBuffer, 48+sizeof(WAVEFORMATEX));
	Decrypt(temp, 48+sizeof(WAVEFORMATEX), format);
	if (!strncmp(temp, "RIFF", 4) && !strncmp(temp+8, "WAVE", 4) && !strncmp(temp+12, "fmt ", 4) && 0x10 == *(int*)(temp+16)) {
		int s = *(int*)(temp+4);
		WAVEFORMATEX *format = (WAVEFORMATEX *)(temp+20);
		if (format->wFormatTag == WAVE_FORMAT_PCM) {
			len = (size - (20 + sizeof(WAVEFORMATEX)))*1000/format->nAvgBytesPerSec;
		}
	}
	if (len < 0) len = 0;
	return len;
}

unsigned long myhtonl(unsigned long x) {
	return (x>>24) + ((x>>8) & 0xFF00) + ((x<<8) & 0xFF0000) + (x<<24);
}

int ExtensionIs(const char *name, const char *ext) {
	int i = (int)strlen(ext);
	int l = (int)strlen(name);
	if (i > l) return 0;
	return !stricmp(ext, name+l-i);
}

int ExtensionIsW(const wchar_t *name, const wchar_t *ext) {
	int i = (int)wcslen(ext);
	int l = (int)wcslen(name);
	if (i > l) return 0;
	return !wcsnicmp(ext, name+l-i, i);
}

struct CONVERSION_TABLE_ENTRY {
	wchar_t jap[3];
	char eng[4];
};

typedef struct CONVERSION_TABLE_ENTRY ConversionTableEntry;

const ConversionTableEntry HiraganaTable[] = {
	{L"\x304D\x3083", "kya"},
	{L"\x304D\x3085", "kyu"},
	{L"\x304D\x3087", "kyo"},

	{L"\x3057\x3083", "sha"},
	{L"\x3057\x3085", "shu"},
	{L"\x3057\x3087", "sho"},

	{L"\x3061\x3083", "cha"},
	{L"\x3061\x3085", "chu"},
	{L"\x3061\x3087", "cho"},

	{L"\x306B\x3083", "nya"},
	{L"\x306B\x3085", "nyu"},
	{L"\x306B\x3087", "nyo"},

	{L"\x3072\x3083", "hya"},
	{L"\x3072\x3085", "hyu"},
	{L"\x3072\x3087", "hyo"},

	{L"\x308A\x3083", "rya"},
	{L"\x308A\x3085", "ryu"},
	{L"\x308A\x3087", "ryo"},

	{L"\x304E\x3083", "gya"},
	{L"\x304E\x3085", "gyu"},
	{L"\x304E\x3087", "gyo"},

	{L"\x3058\x3083", "ja"},
	{L"\x3058\x3085", "ju"},
	{L"\x3058\x3087", "jo"},

	{L"\x3073\x3083", "bya"},
	{L"\x3073\x3085", "byu"},
	{L"\x3073\x3087", "byo"},

	{L"\x3074\x3083", "pya"},
	{L"\x3074\x3085", "pyu"},
	{L"\x3074\x3087", "pyo"},

	{L"\x3042", "a"}, {L"\x3041", "a"},
	{L"\x3044", "i"}, {L"\x3043", "i"},
	{L"\x3046", "u"}, {L"\x3045", "u"},
	{L"\x3048", "e"}, {L"\x3047", "e"},
	{L"\x304A", "o"}, {L"\x3059", "o"},

	{L"\x304B", "ka"}, {L"\x304C", "ga"},
	{L"\x304D", "ki"}, {L"\x304E", "gi"},
	{L"\x304F", "ku"}, {L"\x3050", "gu"},
	{L"\x3051", "ke"}, {L"\x3052", "ge"},
	{L"\x3053", "ko"}, {L"\x3054", "go"},

	{L"\x3055", "sa"},  {L"\x3056", "za"},
	{L"\x3057", "shi"}, {L"\x3058", "ji"},
	{L"\x3059", "su"},  {L"\x305A", "zu"},
	{L"\x305B", "se"},  {L"\x305C", "ze"},
	{L"\x305D", "so"},  {L"\x305E", "zo"},

	{L"\x305F",  "ta"}, {L"\x3060", "da"},
	{L"\x3061", "chi"}, {L"\x3062", "di"},
	{L"\x3064", "tsu"}, {L"\x3065", "du"},
	{L"\x3066",  "te"}, {L"\x3067", "de"},
	{L"\x3068",  "to"}, {L"\x3069", "do"},

	{L"\x306A", "na"},
	{L"\x306B", "ni"},
	{L"\x306C", "nu"},
	{L"\x306D", "ne"},
	{L"\x306E", "no"},

	{L"\x306F", "ha"}, {L"\x3070", "ba"}, {L"\x3071", "pa"},
	{L"\x3072", "hi"}, {L"\x3073", "bi"}, {L"\x3074", "pi"},
	{L"\x3075", "fu"}, {L"\x3076", "bu"}, {L"\x3077", "pu"},
	{L"\x3078", "he"}, {L"\x3079", "be"}, {L"\x307A", "pe"},
	{L"\x307B", "ho"}, {L"\x307C", "bo"}, {L"\x307D", "po"},

	{L"\x307E", "ma"},
	{L"\x307F", "mi"},
	{L"\x3080", "mu"},
	{L"\x3081", "me"},
	{L"\x3082", "mo"},

	{L"\x3084", "ya"}, {L"\x3083", "ya"},
	{L"\x3086", "yu"}, {L"\x3085", "yu"},
	{L"\x3088", "yo"}, {L"\x3087", "yo"},

	{L"\x3089", "ra"},
	{L"\x308A", "ri"},
	{L"\x308B", "ru"},
	{L"\x308C", "re"},
	{L"\x308D", "ro"},

	{L"\x308F", "wa"},
	{L"\x3090", "wi"},
	{L"\x3091", "we"},
	{L"\x3092", "wo"},

	{L"\x3093", "n"},
	{L"\x3094", "vu"},

	// dash.
	{L"\x309C", ""},
};

wchar_t * JISToRomanji(wchar_t *string) {
	int i;
	int changed = 0;
	wchar_t *out = (wchar_t*) malloc(sizeof(wchar_t) * (wcslen(string) * 3 + 2));
	int pos = 0;
	int w = 0;
	while (string[w]) {
		if (0x30A0 <= string[w] && string[w] <= 0x30FF) {
			string[w] += 0x3040 - 0x30A0;
		}
		w++;
	}
	for (w=0; string[w]; w++) {
		int k;
		int d = 0;
		while (string[w] == 0x3063) {
			d = 1;
			w++;
		}
		if (0x3040 <= string[w] && string[w] <= 0x30FF) {
			for (i=0; i<sizeof(HiraganaTable)/sizeof(HiraganaTable[0]); i++) {
				if (string[w] == HiraganaTable[i].jap[0] &&
					(!HiraganaTable[i].jap[1] || string[w+1] == HiraganaTable[i].jap[1])) {
						if (d && HiraganaTable[i].eng[0] &&
								 HiraganaTable[i].eng[0] != 'e' &&
								 HiraganaTable[i].eng[0] != 'i' &&
								 HiraganaTable[i].eng[0] != 'o' &&
								 HiraganaTable[i].eng[0] != 'u' &&
								 HiraganaTable[i].eng[0] != 'a' &&
								 HiraganaTable[i].eng[0] != 'y' &&
								 HiraganaTable[i].eng[1] != 'h' &&
								 HiraganaTable[i].eng[1] != 'y') {
									 out[pos++] = HiraganaTable[i].eng[0];
						}
						for (k=0; HiraganaTable[i].eng[k]; k++) {
							out[pos++] = HiraganaTable[i].eng[k];
						}
						w += HiraganaTable[i].jap[1] != 0;
						changed = 1;
						break;
				}
			}
			if (i<sizeof(HiraganaTable)/sizeof(HiraganaTable[0])) continue;
		}
		out[pos++] = string[w];
	}
	out[pos] = 0;
	if (!changed) {
		free(out);
		return 0;
	}
	return out;
/*
	if (!string[i]) return 0;
	int len = strlen(string)+1;
	wchar_t *temp = (wchar_t*) malloc(sizeof(wchar_t) * len);
	int pos = 0;
	int changed = 0;
	if (MultiByteToWideChar(932, 0, string, -1, temp, len)) {
		int w;
		for (w=0; temp[w]; w++) {
		}
		for (w=0; temp[w]; w++) {
			int d = 0;
			while (temp[w] == 0x3063) {
				d = 1;
				w++;
			}
			if (temp[w] < 0x80) {
				string[pos++] = (char)temp[w];
			}
			else if (temp[w] >= 0xFF01 && temp[w] <= 0xFF5E) {
				string[pos++] = (char)temp[w]+0x20;
			}
			else for (i=0; i<sizeof(HiraganaTable)/sizeof(HiraganaTable[0]); i++) {
				if (temp[w] == HiraganaTable[i].jap[0] &&
					(!HiraganaTable[i].jap[1] || temp[w+1] == HiraganaTable[i].jap[1])) {
						if (d && HiraganaTable[i].eng[0] &&
								 HiraganaTable[i].eng[0] != 'e' &&
								 HiraganaTable[i].eng[0] != 'i' &&
								 HiraganaTable[i].eng[0] != 'o' &&
								 HiraganaTable[i].eng[0] != 'u' &&
								 HiraganaTable[i].eng[0] != 'a' &&
								 HiraganaTable[i].eng[0] != 'y' &&
								 HiraganaTable[i].eng[1] != 'h' &&
								 HiraganaTable[i].eng[1] != 'y') {
									 string[pos++] = HiraganaTable[i].eng[0];
						}
						for (int k=0; HiraganaTable[i].eng[k]; k++) {
							string[pos++] = HiraganaTable[i].eng[k];
						}
						w += HiraganaTable[i].jap[1] != 0;
						changed = 1;
						break;
				}
			}
			if (i == sizeof(HiraganaTable)/sizeof(HiraganaTable[0]))
				string[pos++] = (char)'?';
		}
		string[pos] = 0;
	}
	free(temp);
	return changed;
	//*/
}

wchar_t* JISNameToRomanji(wchar_t *string) {
	wchar_t *out = JISToRomanji(string);
	int i;
	if (!out) return 0;
	i = wcslen(out);
	if (i > 4 && out[i-5] != '-' && !wcscmp(out+i-4, L"chan")) {
		wcscpy(out+i-4, L"-chan");
	}
	else if (i > 3 && out[i-4] != '-' && !wcscmp(out+i-3, L"kun")) {
		wcscpy(out+i-3, L"-kun");
	}
	else if (i > 3 && out[i-4] != '-' && !wcscmp(out+i-3, L"san")) {
		wcscpy(out+i-3, L"-san");
	}
	out[0] = towupper(out[0]);
	return out;
}

int StripFunkyASCII(wchar_t *string) {
	int changed = 0;
	int w = 0;
	while (string[w]) {
		if (string[w] >= 0xFF01 && string[w] <= 0xFF5E) {
			string[w] = (char)string[w]+0x20;
			changed ++;
		}
		w++;
	}
	return changed;
}

int AddFunkyASCII(wchar_t *string) {
	int changed = 0;
	int w = 0;
	while (string[w]) {
		if (string[w] >= 0x21 && string[w] <= 0x7E) {
			string[w] = ((wchar_t)string[w])-0x20+0xFF00;
			changed ++;
		}
		w++;
	}
	return changed;
}
