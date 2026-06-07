#include "Shrink.h"
#include <Windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <d3d9.h>

#include <process.h>

#include "Config.h"
#include "resource.h"
#include "subs.h"
#include "D3DTextOverlay.h"
#include "pp.h"

extern HINSTANCE hInst;

Config config = {0};
State state = {0};

// For registry fix stuff.  Also used for iding game.
RegistryInfo regInfo[] = {
	{HKEY_CURRENT_USER,	SM2, "SchoolMate2\\"},
	{HKEY_CURRENT_USER,	YUUSHA, "Yusya\\Yusya\\"},
	{HKEY_CURRENT_USER,	YUUSHA, "Yusya\\UnLimitedBotsu\\"},
	{HKEY_CURRENT_USER,	AG3, "JS3\\"},
	{HKEY_CURRENT_USER,	SM_SWEETS, "SchoolMateSS\\"},
	{HKEY_CURRENT_USER, SM, "SchoolMate\\"},
	{HKEY_CURRENT_USER, HAKO, "Hako\\"},
	{HKEY_CURRENT_USER, HAKO_TRIAL, "HakoTrial\\"},
	{HKEY_CURRENT_USER, AHM, "@HomeMate\\"},
	{HKEY_CURRENT_USER, RG, "RealKanojo\\"},
	{HKEY_CURRENT_USER, SB_ZERO, "SexyBeachZERO\\"},
	{HKEY_LOCAL_MACHINE, SB3_PLUS, "SexyBeach3Plus\\"},
	{HKEY_LOCAL_MACHINE, SB3, "SexyBeach3\\"},
	{HKEY_LOCAL_MACHINE, RL, "RapeLay\\"},
	// End marker
	{0,Dunno,0}
};

int UpdateGirlValues(HWND hWndDlg) {
	HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
	int set = -1;
	if (ListView_GetSelectedCount(hWndList) == 1)
		set = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
	for (int i=0; i<5; i++) {
		LVITEMW item;
		wchar_t text[1000];
		text[0] = 0;
		if (set >= 0) {
			item.iItem = set;
			item.iSubItem = i;
			item.cchTextMax = sizeof(text)/2;
			item.pszText = text;
			SendMessageW(hWndList, LVM_GETITEMTEXTW, set, (LPARAM)&item);
			text[999] = 0;
		}
		HWND h = GetDlgItem(hWndDlg, IDC_PP_NAME+i);
		SetWindowTextW(h, text);
	}
	EnableWindow(GetDlgItem(hWndDlg, ID_EDIT_GIRL), set >= 0);
	return set;
}

void RemoveSpaceW(wchar_t *s) {
	wchar_t *p = s;
	while (*p == ' ' || *p == '\t') {
		*p++;
	}
	int out = 0;
	while (*p) {
		s[out++] = p++[0];
	}
	while (out && (s[out-1] == ' ' || s[out-1] == '\t')) out--;
	s[out] = 0;
}

void MakeNewSubFile(HWND hWnd) {
	OPENFILENAMEW ofn;
	wchar_t *data = (wchar_t*)malloc(sizeof(wchar_t) * 1000000);
	data[0] = 0;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = L"PP Files\0*.pp\0";
	ofn.lpstrFile = data;
	ofn.nMaxFile = 1000000;
	ofn.lpstrInitialDir = L"data";
	ofn.lpstrTitle = L"Select PPs with voice wave files";
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameW(&ofn) && data[0]) {
		wchar_t *s = data;
		wchar_t **files = 0;
		int numFiles = 0;
		if (ofn.nFileOffset && data[ofn.nFileOffset-1] == '\\') {
			data[ofn.nFileOffset-1] = 0;
		}
		s += wcslen(s)+1;
		while (s[0]) {
			files = (wchar_t**) realloc(files, sizeof(wchar_t*)*(numFiles+1));
			files[numFiles++] = s;
			s += wcslen(s)+1;
		}
		if (numFiles) {
			wchar_t path[MAX_PATH];
			wcscpy(path, L"Blank Subs.txt");
			ofn.lpstrFilter = L"Text File\0*.txt\0";
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrInitialDir = L".";
			ofn.lpstrTitle = L"Create blank sub file";
			ofn.Flags = OFN_EXPLORER | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetSaveFileNameW(&ofn)) {
				FILE *out = _wfopen(path, L"wb");
				if (out) {
					LoadSubsFromPPs(data, files, numFiles);
					SaveSubs(out);
				}
			}
		}
		free(files);
	}
	free(data);
}

void UpdateSubList(HWND hWndDlg) {
	HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
	int i;
	for (i=0; i<config.numSubFiles; i++) {
		free(config.subFiles[i]);
	}
	free(config.subFiles);
	config.subFiles = 0;
	config.numSubFiles = 0;
	int count = ListView_GetItemCount(hWndList);
	ClearSubs();

	for (i=0; i<count; i++) {
		LVITEMW item;
		wchar_t text[1000];
		text[0] = 0;
		item.iItem = i;
		item.iSubItem = 0;
		item.cchTextMax = sizeof(text)/sizeof(wchar_t);
		item.pszText = text;
		int len = SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item);
		int skipped, loaded;
		int totalLoaded = 0;
		if (len > 0 && len < 1000) {
			config.subFiles = (wchar_t**) realloc(config.subFiles, sizeof(wchar_t*) * (config.numSubFiles+1));
			config.subFiles[config.numSubFiles] = wcsdup(text);
			config.numSubFiles++;
			LoadSubs(text, 0, 0, 0, &skipped, &loaded);
			item.mask = LVIF_TEXT;
			item.pszText = text;
			item.iSubItem = 1;
			swprintf(text, L"%i", loaded - totalLoaded);
			totalLoaded = loaded;
			SendMessage(hWndList, LVM_SETITEMTEXTW, i, (LPARAM)&item);
			item.iSubItem = 2;
			swprintf(text, L"%i", skipped);
			SendMessage(hWndList, LVM_SETITEMTEXTW, i, (LPARAM)&item);
		}
	}
}

int CALLBACK EnumFontsProc(CONST LOGFONTW *lpelfe, CONST TEXTMETRICW *, DWORD dwType, LPARAM lParam) {
	HWND hWndFont = (HWND) lParam;
	SendMessageW(hWndFont, CB_ADDSTRING, 0, (LPARAM)lpelfe->lfFaceName);
	return 1;
}

// Can't pass args, so have to use globals.
static union {
	HWND ghLocale;
	wchar_t *seekLocaleName;
	LCID foundLocaleLCID;
};

BOOL CALLBACK EnumLocalesProc(wchar_t *lpLocaleString) {
	// Seems to work.  This behavior is unspecified by MSDN.
	// "Locale string" sounds more like locale name than LCID to me.
	LCID lcid = wcstoul(lpLocaleString, 0, 16);
	wchar_t name[80];
	if (lcid && GetLocaleInfoW(lcid, LOCALE_SNATIVELANGNAME, name, sizeof(name)/2)>0) {
		int w = SendMessageW(ghLocale, CB_FINDSTRINGEXACT, -1, (LPARAM)name);
		if (w < 0)
			SendMessageW(ghLocale, CB_ADDSTRING, 0, (LPARAM)name);
	}
	return 1;
}

BOOL CALLBACK FindLocaleProc(wchar_t *lpLocaleString) {
	// Seems to work.  This behavior is unspecified by MSDN.
	// "Locale string" sounds more like locale name than LCID to me.
	LCID lcid = wcstoul(lpLocaleString, 0, 16);
	wchar_t name[80];
	if (lcid && GetLocaleInfoW(lcid, LOCALE_SNATIVELANGNAME, name, sizeof(name)/2)>0 && !wcsicmp(name, seekLocaleName)) {
		foundLocaleLCID = lcid;
		return 0;
	}
	return 1;
}

void Populate(HWND hWndDlg) {
	int i=0;
	{
		char *s = state.gameName;
		if (!s[0]) s = "Unknown Game";
		char temp[256];
		sprintf(temp, "Configure " OVERLAY_NAME " (%s)", s);
		SetWindowTextA(hWndDlg, temp);
	}

	{
		EnableWindow(GetDlgItem(hWndDlg, ID_FIX_REG), 0);
		while (regInfo[i].game != Dunno) {
			if (regInfo[i].game == state.game || (regInfo[i].game == SB3 && state.game == SB3_PLUS)) {
				HKEY subkey = 0;
				wchar_t temp[MAX_PATH+2];
				swprintf(temp, L"Software\\illusion\\%hs", regInfo[i].path);
				LONG res = RegOpenKeyExW(regInfo[i].root, temp, 0, KEY_ALL_ACCESS, &subkey);
				if (res == ERROR_SUCCESS) {
					DWORD maxlen = sizeof(temp)/2;
					DWORD type = REG_SZ;
					res = RegQueryValueExW(subkey, L"INSTALLDIR", 0, &type, (BYTE*)temp, &maxlen);
					if (type != REG_SZ) res = !ERROR_SUCCESS;
					if (res == ERROR_SUCCESS) {
						res = !ERROR_SUCCESS;
						int len = (int) wcslen(temp);
						if (!wcsnicmp(temp, state.exePath, len)) {
							if (len && temp[len-1] == '\\') len--;
							if (wcsrchr(state.exePath, '\\')-state.exePath == len) {
								res = ERROR_SUCCESS;
							}
						}
					}
					RegCloseKey(subkey);
				}

				if (res != ERROR_SUCCESS) {
					EnableWindow(GetDlgItem(hWndDlg, ID_FIX_REG), 1);
				}
			}
			i++;
		}
	}

	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	HWND hWndRes = GetDlgItem(hWndDlg, IDC_RESOLUTION);
	if (d3d9) {
		SendMessage(hWndRes, CB_RESETCONTENT, 0, 0);
		int numModes = d3d9->GetAdapterModeCount(0, D3DFMT_X8R8G8B8);
		int * modes = (int*) malloc(sizeof(int)*2*(numModes+1));
		int rNumModes = 0;
		for (i=0; i<numModes; i++) {
			D3DDISPLAYMODE mode;
			if (D3D_OK == d3d9->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &mode) && mode.Width>0 && mode.Height>0) {
				int j;
				for (j=0; j<rNumModes; j++) {
					if (modes[2*j] == (int)mode.Width && modes[2*j+1]==(int)mode.Height) break;
				}
				if (j==rNumModes) {
					while (j && mode.Width * mode.Height > (unsigned int)modes[2*j-2] * (unsigned int)modes[2*j-1]) {
						modes[2*j] = modes[2*j-2];
						modes[2*j+1] = modes[2*j-1];
						j--;
					}
					modes[2*j] = mode.Width;
					modes[2*j+1] = mode.Height;
					rNumModes++;
				}
			}
		}
		SendMessage(hWndRes, CB_ADDSTRING, 0, (LPARAM)"Game Setting");
		SendMessage(hWndRes, CB_SETCURSEL, 0, 0);
		for (i=0; i<rNumModes; i++) {
			char temp[100];
			int v1 = modes[i*2], v2 = modes[i*2+1];
			if (v1 < v2) {
				v2 = modes[i*2];
				v1 = modes[i*2+1];
			}
			while (v2) {
				int temp = v1 % v2;
				v1 = v2;
				v2 = temp;
			}
			if (modes[i*2+1]/v1 == 5 && !(v1 % 2)) v1/=2;
			sprintf(temp, "%i x %i (%i:%i)", modes[i*2], modes[i*2+1], modes[i*2]/v1, modes[i*2+1]/v1);
			SendMessage(hWndRes, CB_ADDSTRING, 0, (LPARAM)temp);
			if (modes[i*2] == config.width && modes[i*2+1] == config.height) {
				SendMessage(hWndRes, CB_SETCURSEL, i+1, 0);
			}
		}
		free(modes);
		d3d9->Release();
	}

	{
		HWND hWndFont = GetDlgItem(hWndDlg, IDC_FONT_FACE);
		SendMessage(hWndFont, CB_RESETCONTENT, 0, 0);
		HDC hDC = GetWindowDC(hWndFont);
		if (hDC) {
			LOGFONTW lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfCharSet = DEFAULT_CHARSET;
			EnumFontsW(hDC, 0, EnumFontsProc, (LPARAM)hWndFont);
			ReleaseDC(hWndFont, hDC);
			int w = SendMessageW(hWndFont, CB_FINDSTRINGEXACT, -1, (LPARAM)config.forceFontFace);
			if (w < 0) w = 0;
			SendMessageW(hWndFont, CB_SETCURSEL, w, 0);
		}
	}

	{
		LVCOLUMNW c;
		HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
		ListView_DeleteAllItems(hWndList);
		while (SendMessage(hWndList, LVM_DELETECOLUMN, 0, 0));
		ListView_SetExtendedListViewStyleEx(hWndList, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
		SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		c.mask = LVCF_TEXT | LVCF_WIDTH;
		c.cx = 128;
		c.pszText = L"File";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 0, (LPARAM)&c);
		c.cx = 50;
		c.pszText = L"Used";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 1, (LPARAM)&c);
		c.cx = 50;
		c.pszText = L"Skipped";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 2, (LPARAM)&c);
		for (i=0; i<config.numSubFiles; i++) {
			LVITEMW item;
			item.mask = LVIF_TEXT;
			item.pszText = config.subFiles[i];
			item.iItem = ListView_GetItemCount(hWndList);
			item.iSubItem = 0;
			SendMessageW(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);
		}
		UpdateSubList(hWndDlg);
	}

	{
		CheckRadioButton(hWndDlg, IDC_AG3_FIRST, IDC_AG3_FILE, IDC_AG3_FIRST+config.ag3Names);
	}

	{
		SetWindowTextW(GetDlgItem(hWndDlg, IDC_FONT_FACE), config.forceFontFace);
		wchar_t temp[100];
		swprintf(temp, L"%i", config.forceFontHeight);
		SetWindowTextW(GetDlgItem(hWndDlg, IDC_FONT_SIZE), temp);

		swprintf(temp, L"%i", config.minSubDuration);
		SetWindowTextW(GetDlgItem(hWndDlg, IDC_MIN_SUB_DURATION), temp);
	}

	{
		LVCOLUMNW c;
		HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
		ListView_DeleteAllItems(hWndList);
		while (SendMessage(hWndList, LVM_DELETECOLUMN, 0, 0));
		ListView_SetExtendedListViewStyleEx(hWndList, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
		SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		c.mask = LVCF_TEXT | LVCF_WIDTH;
		c.cx = 110;
		c.pszText = L"File";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 0, (LPARAM)&c);
		c.cx = 90;
		c.pszText = L"Girl Name";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 1, (LPARAM)&c);
		c.cx = 50;
		c.pszText = L"Color";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 2, (LPARAM)&c);
		c.cx = 65;
		c.pszText = L"Outline";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 3, (LPARAM)&c);
		c.cx = 65;
		c.pszText = L"Shadow";
		SendMessageW(hWndList, LVM_INSERTCOLUMNW, 4, (LPARAM)&c);
		for (i=0; i<config.numExtraChars; i++) {
			LVITEMW item;
			item.mask = LVIF_TEXT;
			item.pszText = config.extraChars[i].file;
			if (!item.pszText) item.pszText = L"";
			item.iItem = ListView_GetItemCount(hWndList);
			item.iSubItem = 0;
			SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);

			wchar_t temp[1000];
			item.pszText = temp;

			item.iSubItem = 1;
			MultiByteToWideChar(CP_UTF8, 0, config.extraChars[i].name, -1, temp, sizeof(temp)/2);
			SendMessageW(hWndList, LVM_SETITEMW, 0, (LPARAM)&item);

			item.iSubItem = 2;
			swprintf(temp, L"%06X", config.extraChars[i].color & 0xFFFFFF);
			SendMessageW(hWndList, LVM_SETITEMW, 0, (LPARAM)&item);

			item.iSubItem = 3;
			swprintf(temp, L"%08X", ((config.extraChars[i].outline) << 8) + ((config.extraChars[i].outline) >>24));
			SendMessageW(hWndList, LVM_SETITEMW, 0, (LPARAM)&item);

			item.iSubItem = 4;
			swprintf(temp, L"%08X", ((config.extraChars[i].shadow) << 8) + ((config.extraChars[i].shadow) >>24));
			SendMessageW(hWndList, LVM_SETITEMW, 0, (LPARAM)&item);
		}
	}

	CheckDlgButton(hWndDlg, IDC_HIDE_GIRL_NAMES, BST_CHECKED * config.hideGirlNames);

	{
		HWND hLocale = GetDlgItem(hWndDlg, IDC_FORCE_LOCALE);
		SendMessage(hLocale, CB_RESETCONTENT, 0, 0);
		// Can pass no args to enum function.  Brilliant!
		ghLocale = hLocale;
		SendMessage(hLocale, CB_ADDSTRING, 0, (LPARAM)" Disabled");
		SendMessage(hLocale, CB_SETCURSEL, 0, 0);
		EnumSystemLocalesW(EnumLocalesProc, LCID_INSTALLED);

		wchar_t name[80];
		if (config.forceLocale && GetLocaleInfoW(config.forceLocale, LOCALE_SNATIVELANGNAME, name, sizeof(name)/2)>0) {
			int w = SendMessageW(hLocale, CB_FINDSTRINGEXACT, -1, (LPARAM)name);
			if (w > 0) SendMessageW(hLocale, CB_SETCURSEL, w, 0);
		}
	}

	SendMessage(GetDlgItem(hWndDlg, IDC_PP_NAME), EM_LIMITTEXT, 999, 0);
	SendMessage(GetDlgItem(hWndDlg, IDC_GIRL_NAME), EM_LIMITTEXT, 999, 0);
	SendMessage(GetDlgItem(hWndDlg, IDC_TEXT_COLOR), EM_LIMITTEXT, 6, 0);
	SendMessage(GetDlgItem(hWndDlg, IDC_OUTLINE_COLOR), EM_LIMITTEXT, 8, 0);
	SendMessage(GetDlgItem(hWndDlg, IDC_SHADOW_COLOR), EM_LIMITTEXT, 8, 0);
	SendMessage(GetDlgItem(hWndDlg, IDC_MIN_SUB_DURATION), EM_LIMITTEXT, 8, 0);
	EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 0);
}

void WriteUTF8(wchar_t *s, FILE *out) {
	char temp[4000];
	int len = WideCharToMultiByte(CP_UTF8, 0, s, -1, temp, sizeof(temp), 0, 0);
	if (len > 0 && len < sizeof(temp)) {
		fwrite(temp, 1, strlen(temp), out);
	}
}

void WriteUTF8(wchar_t *s1, wchar_t *s2, wchar_t *s3, FILE *out) {
	WriteUTF8(s1, out);
	if (s2) {
		fprintf(out, "(");
		WriteUTF8(s2, out);
		fprintf(out, ")");
	}
	fprintf(out, " = ");
	WriteUTF8(s3, out);
	fprintf(out, "\r\n");
}

struct GirlInfo {
	wchar_t name[1000];
	wchar_t file[1000];
	union {
		struct {
			unsigned int color, outline, shadow;
		};
		unsigned int colors[3];
	};
};

int GetGirlInfo(GirlInfo &g, int index, HWND hWndList) {
	LVITEMW item;
	item.iItem = index;

	item.cchTextMax = sizeof(g.name)/2;
	item.iSubItem = 1;
	item.pszText = g.name;
	int len = SendMessageW(hWndList, LVM_GETITEMTEXTW, index, (LPARAM)&item);
	if (len <= 0 || len >= item.cchTextMax) return 0;

	item.cchTextMax = sizeof(g.file)/2;
	item.iSubItem = 0;
	item.pszText = g.file;
	len = SendMessageW(hWndList, LVM_GETITEMTEXTW, index, (LPARAM)&item);
	if (len >= item.cchTextMax) return 0;

	for (int i=0; i<3; i++) {
		wchar_t temp[100];
		item.iSubItem = 2+i;
		item.cchTextMax = sizeof(temp)/sizeof(wchar_t);
		item.pszText = temp;
		len = SendMessageW(hWndList, LVM_GETITEMTEXTW, index, (LPARAM)&item);
		if (len <= 0 || len >= item.cchTextMax) return 0;
		g.colors[i] = wcstoul(temp, 0, 16);
	}
	return 1;
};

// Saves from the dialog, if there is one, and from memory if there isn't.
// Updates some, but not all, entries in config object, so reloading config
// is necessary afterwards if hWndDlg is not null.
void SaveConfig(HWND hWndDlg) {
	FILE *out = fopen("Subtitle.cfg", "wb");
	fprintf(out, "// Note that this file must be in UTF-8 format.\r\n\r\n");
	int needRestart = 0;
	wchar_t temp[10000];
	int i;
	{
		if (hWndDlg) {
			HWND hWndRes = GetDlgItem(hWndDlg, IDC_RESOLUTION);
			int w = SendMessageW(hWndRes, CB_GETCURSEL, 0, 0);
			config.width = config.height = 0;
			if (w >= 0) {
				int l = SendMessageA(hWndRes, CB_GETLBTEXTLEN, w, 0);
				int x, y;
				if (l > 0 && l < sizeof(temp)/sizeof(wchar_t)) {
					if (SendMessageA(hWndRes, CB_GETLBTEXT, w, (LPARAM)temp)>0) {
						if (sscanf((char*)temp, " %i x %i", &x, &y) == 2 && y > 0 && x > 0) {
							config.width = x;
							config.height = y;
						}
					}
				}
			}
		}
		if (config.width && config.height) {
			fprintf(out, "Resolution = %i, %i\r\n", config.width, config.height);
		}
	}

	{
		if (hWndDlg)
			GetWindowTextW(GetDlgItem(hWndDlg, IDC_MIN_SUB_DURATION), temp, sizeof(temp)/sizeof(wchar_t));
		else
			_itow(config.minSubDuration, temp, 10);
		WriteUTF8(L"MinSubDuration", 0, temp, out);
	}

	{
		if (hWndDlg)
			GetWindowTextW(GetDlgItem(hWndDlg, IDC_MIN_SUB_DURATION), temp, sizeof(temp)/sizeof(wchar_t));
		else
			_itow(config.minSubDuration, temp, 10);
		WriteUTF8(L"MinSubDuration", 0, temp, out);
	}

	{
		if (hWndDlg)
			config.hakoRomanizeNames = (IsDlgButtonChecked(hWndDlg, IDC_ROMANIZE_NAMES) == BST_CHECKED);
		fprintf(out, "HakoRomanizeNames = %i\r\n", config.hakoRomanizeNames);
	}

	{
		if (hWndDlg) {
			HWND hLocale = GetDlgItem(hWndDlg, IDC_FORCE_LOCALE);
			int i = SendMessage(hLocale, CB_GETCURSEL, 0, 0);
			if (i == 0) {
				if (config.forceLocale != foundLocaleLCID) {
					config.forceLocale = 0;
					needRestart = 1;
				}
			}
			else {
				// Can pass no args to enum function.  Brilliant!
				wchar_t name[80];
				int l = SendMessageW(hLocale, CB_GETLBTEXTLEN, i, 0);
				if (l > 0 && l < sizeof(temp)/sizeof(wchar_t) &&
					l == SendMessageW (hLocale, CB_GETLBTEXT, i, (LPARAM)name)) {
						seekLocaleName = name;
						if (EnumSystemLocalesW(FindLocaleProc, 0) &&
							seekLocaleName != name) {
								if (config.forceLocale != foundLocaleLCID) {
									config.forceLocale = foundLocaleLCID;
									needRestart = 1;
								}
						}
				}
			}
		}
		fprintf(out, "ForceLocale = %04X\r\n", config.forceLocale);
	}

	{
		if (hWndDlg) {
			HWND hWndFont = GetDlgItem(hWndDlg, IDC_FONT_FACE);
			int w = SendMessageW(hWndFont, CB_GETCURSEL, 0, 0);
			if (w >= 0) {
				int l = SendMessageW(hWndFont, CB_GETLBTEXTLEN, w, 0);
				if (l > 0) {
					wchar_t *temp = (wchar_t*) malloc((l+1) * sizeof(wchar_t));
					if (l == SendMessageW(hWndFont, CB_GETLBTEXT, w, (LPARAM)temp)) {
						free(config.forceFontFace);
						config.forceFontFace = temp;
					}
					else free(temp);
				}
			}
		}
		WriteUTF8(L"Font", L"Face", config.forceFontFace, out);
	}

	{
		if (hWndDlg) {
			GetWindowTextW(GetDlgItem(hWndDlg, IDC_FONT_SIZE), temp, sizeof(temp)/sizeof(wchar_t));
			config.forceFontHeight = _wtoi(temp);
			if (config.forceFontHeight < 0) config.forceFontHeight = 0;
			else if (config.forceFontHeight >= 100) config.forceFontHeight = 100;
		}
		fprintf(out, "Font(Height) = %i\r\n", config.forceFontHeight);
	}

	fprintf(out, "\r\nClock = %i\r\n", config.displayClock);
	fprintf(out, "Hacks = %i\r\n", config.hacksEnabled);
	fprintf(out, "View = %i\r\n", config.viewEnabled);
	fprintf(out, "HakoImmortality = %i\r\n", config.hakoImmortality);
	fprintf(out, "ahmNoLimit = %i\r\n", config.ahmNoLimit);
	fprintf(out, "ahmBreastSizes = %06X\r\n", config.ahmBreastSizesDword);

	if (hWndDlg)
		config.hideGirlNames = (IsDlgButtonChecked(hWndDlg, IDC_HIDE_GIRL_NAMES) == BST_CHECKED);
	fprintf(out, "HideGirlNames = %i\r\n", config.hideGirlNames);

	{
		fprintf(out,
			"\r\n\r\n"
			"// Prevents drawing text in exes that it shouldn't.  Not having these lines\r\n"
			"// won't cause a crash, but will have interesting effects.\r\n"
			"\r\n"
			"// \"RegOnly\" means that it will only check installation queries, and\r\n"
			"// give the current directory if no install reg keys exist.  Subtitles\r\n"
			"// will not be loaded or displayed.\r\n"
			"\r\n"
			"// Using \"Exclude\" instead will prevent that in addition to the subtitle stuff.\r\n"
			"\r\n"
			"// Using \"ForceReg\" will ignore the install directory in the  registry,\r\n"
			"// if there is one, and give the current directory instead.  It's compatible\r\n"
			"// with \"RegOnly\", though you'll need two \"Exe()\" lines to use them both.\r\n\r\n");
		for (i=0; i<config.numExeSettings; i++) {
			if (config.exeSettings[i].setting & EXE_EXCLUDE) {
				WriteUTF8(L"Exe", L"Exclude", config.exeSettings[i].exe, out);
			}
			else {
				if (config.exeSettings[i].setting & EXE_FORCE_REG) {
					WriteUTF8(L"Exe", L"ForceReg", config.exeSettings[i].exe, out);
				}
				if (config.exeSettings[i].setting & EXE_REG_ONLY) {
					WriteUTF8(L"Exe", L"RegOnly", config.exeSettings[i].exe, out);
				}
			}
		}
	}
	fprintf(out, "\r\n");

	for (i=0; i<config.numSubFiles; i++) {
		WriteUTF8(L"LoadSubs", 0, config.subFiles[i], out);
	}

	if (hWndDlg) {
		config.ag3Names = 0;
		if (BST_CHECKED == IsDlgButtonChecked(hWndDlg, IDC_AG3_LAST))
			config.ag3Names = 1;
		else if (BST_CHECKED == IsDlgButtonChecked(hWndDlg, IDC_AG3_FILE))
			config.ag3Names = 2;
	}
	fprintf(out, "\r\nAG3Names = %i\r\n", config.ag3Names);

	{
		if (hWndDlg) {
			HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
			GirlInfo g;
			i = 0;
			while (GetGirlInfo(g, i++, hWndList)) {
				fprintf(out, "\r\nOutline(Color) = %08X\r\nShadow(Color)  = %08X\r\n", g.outline, g.shadow);
				if (!g.file[0]) {
					swprintf(temp, L"%06X", g.color);
					WriteUTF8(L"Color", g.name, temp, out);
				}
				else {
					swprintf(temp, L"%s, %06X", g.name, g.color);
					WriteUTF8(L"Girl", g.file, temp, out);
				}
			}
		}
		else {
			for (i=0; i<config.numExtraChars; i++) {
				CharInfo *g = config.extraChars+i;
				fprintf(out, "\r\nOutline(Color) = %08X\r\n", (g->outline<<8) + (g->outline>>24));
				fprintf(out, "Shadow(Color)  = %08X\r\n", (g->shadow<<8) + (g->shadow>>24));
				if (!g->file) {
					fprintf(out, "Color(%s) = %06X\r\n", g->name, g->color & 0xFFFFFF);
				}
				else {
					char temp[4000];
					int len = WideCharToMultiByte(CP_UTF8, 0, g->file, -1, temp, sizeof(temp), 0, 0);
					if (len > 0 && len < sizeof(temp)) {
						fprintf(out, "Girl(%s) = %s, %06X\r\n", temp, g->name, g->color & 0xFFFFFF);
					}
				}
			}
		}
	}
	fclose(out);
	if (needRestart) LocaleRestart(hWndDlg);
}

COLORREF GetColorValue(int i, HWND hWndDlg, wchar_t *text=0, int alphaHint = 1, unsigned char *alpha = 0) {
	HWND h = GetDlgItem(hWndDlg, IDC_PP_NAME+i);
	wchar_t text2[9];
	GetWindowTextW(h, text2, 9);
	wchar_t * temp = L"FFFFFFFF";
	if (i == 3) temp = L"000000FF";
	else if (i == 4) temp = L"0000005A";
	int len = 0;
	while (len < 8) {
		wchar_t w = text2[len];
		if (!iswxdigit(w)) break;
		if ('a' <= w && w <= 'f') {
			text2[len] += 'A'-'a';
		}
		len++;
	}
	wcscpy(text2+len, temp+len);
	union {
		unsigned long colors;
		struct {
			unsigned char a, b, g, r;
		};
	};
	colors = wcstoul(text2, 0, 16);
	if (i == 2) {
		text2[6] = 0;
		a = 255;
	}
	if (alpha) *alpha = a;
	double nAlpha = 1;
	while (alphaHint) {
		nAlpha *= (255-a)/255.0;
		alphaHint --;
	}
	a = 255 - (unsigned char) (255*nAlpha);
	if (text) wcscpy(text, text2);
	unsigned short a2 = a;
	return RGB(a2 * r/255 + (255-a2), a2 * g/255 + (255-a2), a2 * b/255 + (255-a2));
}

void DrawSample(HWND hWndDlg) {
	int i;
	HWND hWndSample = GetDlgItem(hWndDlg, IDC_SAMPLE);
	HDC hDC = GetWindowDC(hWndSample);
	if (hDC) {
		HDC memDC = CreateCompatibleDC(hDC);
		RECT r;
		GetClientRect(hWndSample, &r);
		HBITMAP hBMP = CreateCompatibleBitmap(hDC, r.right, r.bottom);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(memDC, hBMP);
		FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

		wchar_t temp[1000];
		GetWindowTextW(GetDlgItem(hWndDlg, IDC_FONT_SIZE), temp, sizeof(temp)/sizeof(wchar_t));
		int size = _wtoi(temp);
		if (!size) size = 40;
		HWND hWndFont = GetDlgItem(hWndDlg, IDC_FONT_FACE);
		int w = SendMessageW(hWndFont, CB_GETCURSEL, 0, 0);
		if (w >= 0) {
			if (!SendMessageW(hWndFont, CB_GETLBTEXT, w, (LPARAM)temp)) {
				wcscpy(temp, L"Arial");
			}
		}
		HFONT hFont = CreateFontW(size, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, temp);
		if (hFont) {
			HFONT hOldFont = (HFONT)SelectObject(memDC, hFont);
			if (hOldFont) {
				r.left+=2;

				SetBkMode(memDC, TRANSPARENT);
				if (GetWindowTextW(GetDlgItem(hWndDlg, IDC_GIRL_NAME), temp, sizeof(temp)/sizeof(wchar_t)) < 2) {
					wcscpy(temp, L"Sample");
				}

				for (i=3; i>0; i--) {
					int dx = i%2+2;
					int dy = i/2+2;
					RECT r2 = r;
					r2.left += dx;
					r2.top += dy;
					SetTextColor(memDC, GetColorValue(4, hWndDlg, 0, 1 + (dx==2) + (dy==2)));
					DrawTextW(memDC, temp, -1, &r2, DT_BOTTOM);
				}

				for (int j=0; j<2; j++) {
					for (i=0; i<9; i++) {
						if (i%2 != j) continue;
						int dx = i%3-1;
						int dy = i/3-1;
						RECT r2 = r;
						r2.left += dx;
						r2.top += dy;
						unsigned char alpha;
						SetTextColor(memDC, GetColorValue(3, hWndDlg, 0, 1 + !dx + !dy, &alpha));
						if (!alpha) continue;
						DrawTextW(memDC, temp, -1, &r2, DT_BOTTOM);
					}
				}
				SetTextColor(memDC, GetColorValue(2, hWndDlg));
				DrawTextW(memDC, temp, -1, &r, DT_BOTTOM);

				SelectObject(memDC, hOldFont);
			}
			DeleteObject(hFont);
		}

		SelectObject(memDC, hOldBmp);
		DeleteDC(memDC);
		ReleaseDC(hWndSample, hDC);
		if (hBMP = (HBITMAP)SendMessage(hWndSample, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBMP)) {
			DeleteObject(hBMP);
		}
	}
}

INT_PTR CALLBACK DialogProc(
    HWND hWndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {
		if (uMsg == WM_INITDIALOG) {
			Populate(hWndDlg);
			DrawSample(hWndDlg);
			return 1;
		}
		else if (uMsg == WM_NOTIFY) {
			PSHNOTIFY* n = (PSHNOTIFY*) lParam;
			if (n->hdr.idFrom == IDC_COLOR_LIST) {
				if (n->hdr.code == LVN_ITEMCHANGED) {
					NMLISTVIEW *nlv = (NMLISTVIEW*)n;
					if ((nlv->uChanged&LVIF_STATE) &&
						((nlv->uNewState^nlv->uOldState) & LVIS_SELECTED))
						UpdateGirlValues(hWndDlg);
				}
				else if (n->hdr.code == LVN_KEYDOWN) {
					NMLVKEYDOWN *key = (NMLVKEYDOWN *) n;
					HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
					if (key->wVKey == VK_DELETE ||
						key->wVKey == VK_BACK) {
							while (1) {
								int i = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
								if (i < 0) break;
								ListView_DeleteItem(hWndList, i);
								EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
							}
						UpdateGirlValues(hWndDlg);
					}
				}
			}
			else if (n->hdr.idFrom == IDC_SUBTITLE_LIST) {
				HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
				if (n->hdr.code == LVN_ITEMCHANGED) {
					int i = ListView_GetSelectedCount(hWndList);
					EnableWindow(GetDlgItem(hWndDlg, ID_SUBTITLE_UP), i);
					EnableWindow(GetDlgItem(hWndDlg, ID_SUBTITLE_DOWN), i);
				}
				else if (n->hdr.code == LVN_KEYDOWN) {
					NMLVKEYDOWN *key = (NMLVKEYDOWN *) n;
					if (key->wVKey == VK_DELETE ||
						key->wVKey == VK_BACK) {
							while (1) {
								int i = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
								if (i < 0) break;
								ListView_DeleteItem(hWndList, i);
								EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
							}
							UpdateSubList(hWndDlg);
					}
				}
			}
		}
		else if (uMsg == WM_COMMAND) {
			int cmd = LOWORD(wParam);
			if (HIWORD(wParam)==BN_CLICKED) {
				if (cmd == IDC_HIDE_GIRL_NAMES || cmd == IDC_ROMANIZE_NAMES)
					EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
				else if (cmd == ID_APPLY) {
					SaveConfig(hWndDlg);
					LoadConfig();
					Populate(hWndDlg);
				}
				else if (cmd == IDCANCEL || cmd == IDOK) {
					if (cmd == IDOK) {
						SaveConfig(hWndDlg);
					}
					LoadConfig();

					// Not sure if I have to do this, but just in case...
					HWND hSample = GetDlgItem(hWndDlg, IDC_SAMPLE);
					HBITMAP hBMP = (HBITMAP) SendMessage(hSample, STM_GETIMAGE, IMAGE_BITMAP, 0);
					if (hBMP) {
						SendMessage(hSample, STM_SETIMAGE, IMAGE_BITMAP, 0);
						DeleteObject(hBMP);
					}

					EndDialog(hWndDlg, 0);
					return 1;
				}
				else if (cmd == ID_CREATE_NEW_SUBTITLE) {
					MakeNewSubFile(hWndDlg);
				}
				else if (cmd == ID_ADD) {
					OPENFILENAMEW ofn;
					wchar_t *data = (wchar_t*)malloc(sizeof(wchar_t) * 1000000);
					data[0] = 0;
					memset(&ofn, 0, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hWndDlg;
					ofn.hInstance = 0;
					ofn.lpstrFilter = L"Text Files\0*.txt\0";
					ofn.lpstrFile = data;
					ofn.nMaxFile = 1000000;
					ofn.lpstrInitialDir = L".";
					ofn.lpstrTitle = L"Select PP files from which to load wave file names";
					ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
					if (GetOpenFileNameW(&ofn) && data[0]) {
						HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
						wchar_t *s = data;
						int numFiles = 0;
						if (ofn.nFileOffset && data[ofn.nFileOffset-1] == '\\') {
							data[ofn.nFileOffset-1] = 0;
						}
						wchar_t *path = data;
						if (state.exeName != state.exePath &&
							!wcsnicmp(data, state.exePath, state.exeName-state.exePath)) {
							path = data + (state.exeName-state.exePath);
						}
						s += wcslen(s)+1;
						while (s[0]) {
							LVITEMW item;
							EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
							wchar_t temp[10*MAX_PATH];
							item.mask = LVIF_TEXT;
							swprintf(temp, L"%s\\%s", path, s);
							item.pszText = temp;
							item.iItem = ListView_GetItemCount(hWndList);
							item.iSubItem = 0;
							SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);
							s += wcslen(s)+1;
						}
					}
					free(data);
					UpdateSubList(hWndDlg);
				}
				else if (cmd == ID_SUBTITLE_UP) {
					HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
					int p = 0;
					int i = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
					while (i == p) {
						i = ListView_GetNextItem(hWndList, i, LVNI_SELECTED);
						p++;
					}
					while (i >= 0) {
						LVITEMW item;
						wchar_t text[1000];
						text[0] = 0;
						item.iItem = i;
						item.iSubItem = 0;
						item.cchTextMax = sizeof(text)/sizeof(wchar_t);
						item.pszText = text;
						int len = SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item);
						int i2 = ListView_GetNextItem(hWndList, i, LVNI_SELECTED);
						ListView_DeleteItem(hWndList, i);

						item.iItem --;
						item.state = LVIS_SELECTED;
						item.stateMask = LVIS_SELECTED;
						item.mask = LVIF_TEXT | LVIF_STATE;
						SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);
						i = i2;
						EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
					}
					UpdateSubList(hWndDlg);
				}
				else if (cmd == ID_SUBTITLE_DOWN) {
					HWND hWndList = GetDlgItem(hWndDlg, IDC_SUBTITLE_LIST);
					int p = 0;
					int i = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
					while (i >= 0) {
						int next;
						int start = i;
						while ((next = ListView_GetNextItem(hWndList, i, LVNI_SELECTED)) == i+1) {
							i++;
						}
						int end = i;
						if (end + 1 == ListView_GetItemCount(hWndList)) break;
						LVITEMW item;
						wchar_t text[1000];
						text[0] = 0;
						item.iItem = end+1;
						item.iSubItem = 0;
						item.cchTextMax = sizeof(text)/sizeof(wchar_t);
						item.pszText = text;
						int len = SendMessageW(hWndList, LVM_GETITEMTEXTW, end+1, (LPARAM)&item);
						ListView_DeleteItem(hWndList, end+1);
						item.iItem = start;
						item.mask = LVIF_TEXT;
						SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);
						i = next;
						EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
					}
					UpdateSubList(hWndDlg);
				}
				else if (cmd == ID_FIX_REG) {
					int happy = 1;
					int i=0;
					while (regInfo[i].game != Dunno) {
						if (regInfo[i].game == state.game ||
							(regInfo[i].game == SB3 && state.game == SB3_PLUS && IDYES == MessageBox(hWndDlg, "Use SB3 Plus directory for SB3?", "Confirm SB3 location", MB_YESNO))) {
							HKEY subkey = 0;
							wchar_t temp[MAX_PATH];
							swprintf(temp, L"Software\\illusion\\%hs", regInfo[i].path);
							LONG res = RegCreateKeyExW(regInfo[i].root, temp, 0, 0, 0, KEY_ALL_ACCESS, 0, &subkey, 0);
							if (res != ERROR_SUCCESS) break;
							wchar_t *end = wcsrchr(state.exePath, '\\');
							if (end) {
								wchar_t e = end[1];
								end[1] = 0;
								res = RegSetValueExW(subkey, L"INSTALLDIR", 0, REG_SZ, (BYTE*)state.exePath, (end+2-state.exePath) * sizeof(wchar_t));
								if (res != ERROR_SUCCESS) {
									happy = 0;
								}
								end[1] = e;
							}
							RegCloseKey(subkey);
						}
						i++;
					}
					if (happy) EnableWindow(GetDlgItem(hWndDlg, ID_FIX_REG), 0);
				}
				else if (cmd == ID_EDIT_GIRL) {
					EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
					HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);

					int i = 0;
					wchar_t file[1000];
					wchar_t name[1000];
					GetWindowTextW(GetDlgItem(hWndDlg, IDC_PP_NAME), file, sizeof(file)/2);
					GetWindowTextW(GetDlgItem(hWndDlg, IDC_GIRL_NAME), name, sizeof(name)/2);
					file[999] = 0;
					name[999] = 0;
					int cmp = -1;
					while (1) {
						LVITEMW item;
						wchar_t file2[1000];
						wchar_t name2[1000];
						item.iItem = i;
						item.cchTextMax = sizeof(file2)/2;

						item.pszText = name2;
						item.iSubItem = 1;
						if (SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item) <= 0)
							break;

						item.iSubItem = 0;
						item.pszText = file2;
						SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item);
						file2[999] = 0;
						name2[999] = 0;
						RemoveSpaceW(file2);
						RemoveSpaceW(name2);
						cmp = wcsicmp(file2, file);
						if (cmp < 0) {
							i++;
							continue;
						}
						if (cmp > 0) {
							break;
						}
						cmp = wcsicmp(name2, name);
						if (cmp < 0) {
							i++;
							continue;
						}
						// cmp >= 0
						break;
					}
					int index = i;
					if (cmp) {
						LVITEMW item;
						item.mask = LVIF_TEXT;
						item.pszText = L"";
						item.iItem = index;
						item.iSubItem = 0;
						SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&item);
					}
					for (i=0; i<5; i++) {
						LVITEMW item;
						wchar_t text[1000];
						HWND h = GetDlgItem(hWndDlg, IDC_PP_NAME+i);
						if (i > 1) {
							GetColorValue(i, hWndDlg, text);
						}
						else {
							GetWindowTextW(h, text, sizeof(text)/2);
							text[999] = 0;
							RemoveSpaceW(text);
						}

						item.iSubItem = i;
						item.pszText = text;
						SendMessageW(hWndList, LVM_SETITEMTEXTW, index, (LPARAM)&item);
						SetWindowTextW(h, text);
					}
					while (1) {
						int i = ListView_GetNextItem(hWndList, -1, LVNI_SELECTED);
						if (i < 0) break;
						ListView_SetItemState(hWndList, i, 0, LVIS_SELECTED);
					}
					ListView_SetItemState(hWndList, index, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
					//ListView_SetItem(hWndList, &item);
					//ListView_SetHotItem(hWndList, index);
				}
				else if (cmd == ID_ALL_OUTLINE || cmd == ID_ALL_SHADOW) {
					wchar_t text[1000];
					int col = 3+(cmd==ID_ALL_SHADOW);
					GetColorValue(col, hWndDlg, text);
					HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
					for (int i = ListView_GetItemCount(hWndList) - 1; i >= 0; i--) {
						LVITEMW item;
						item.iSubItem = col;
						item.pszText = text;
						SendMessageW(hWndList, LVM_SETITEMTEXTW, i, (LPARAM)&item);
					}
					HWND h = GetDlgItem(hWndDlg, IDC_PP_NAME+col);
					SetWindowTextW(h, text);
					EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
				}
			}
			else if (HIWORD(wParam) == EN_CHANGE) {
				if (cmd == IDC_PP_NAME || cmd == IDC_GIRL_NAME) {
					HWND hWndList = GetDlgItem(hWndDlg, IDC_COLOR_LIST);
					wchar_t file[1000];
					wchar_t name[1000];
					GetWindowTextW(GetDlgItem(hWndDlg, IDC_PP_NAME), file, sizeof(file)/2);
					GetWindowTextW(GetDlgItem(hWndDlg, IDC_GIRL_NAME), name, sizeof(name)/2);
					file[999] = 0;
					name[999] = 0;
					RemoveSpaceW(file);
					RemoveSpaceW(name);
					int i = 0;
					int ChangedGirl = -1;
					while (1) {
						LVITEMW item;
						wchar_t file2[1000];
						wchar_t name2[1000];
						item.iItem = i;
						item.cchTextMax = sizeof(file2)/2;

						item.pszText = name2;
						item.iSubItem = 1;
						if (SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item) <= 0)
							break;

						item.iSubItem = 0;
						item.pszText = file2;
						SendMessageW(hWndList, LVM_GETITEMTEXTW, i, (LPARAM)&item);
						if (!wcsicmp(file2, file)) {
							if (file[0] || !wcsicmp(name, name2)) {
								ChangedGirl = i;
								break;
							}
						}
						i++;
					}
					if (ChangedGirl >= 0) {
						SetWindowText(GetDlgItem(hWndDlg, ID_EDIT_GIRL), "Edit");
					}
					else {
						SetWindowText(GetDlgItem(hWndDlg, ID_EDIT_GIRL), "Add");
					}
					EnableWindow(GetDlgItem(hWndDlg, ID_EDIT_GIRL), name[0]);
				}
				else if (cmd == IDC_MIN_SUB_DURATION || cmd == IDC_FONT_SIZE) {
					EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
				}
				if (cmd == IDC_GIRL_NAME || cmd == IDC_FONT_SIZE || cmd == IDC_TEXT_COLOR || cmd == IDC_OUTLINE_COLOR || cmd==IDC_SHADOW_COLOR) {
					DrawSample(hWndDlg);
				}
			}
			else if (HIWORD(wParam) == CBN_SELCHANGE) {
				if (cmd == IDC_FONT_FACE || cmd == IDC_RESOLUTION || cmd == IDC_FORCE_LOCALE)
					EnableWindow(GetDlgItem(hWndDlg, ID_APPLY), 1);
				if (cmd == IDC_FONT_FACE)
					DrawSample(hWndDlg);
			}
		}
		return 0;
}

void ConfigDialog(HWND hWnd) {
	DialogBoxParamW(hInst, MAKEINTRESOURCEW(IDD_CONFIG_DIALOG), hWnd, DialogProc, 0);
}

void ClearConfig(int noNukeFont) {
	int i;
	ClearSubs();
	for (i=0; i<config.numSubFiles; i++) {
		free(config.subFiles[i]);
	}
	free(config.subFiles);

	for (i=0; i<config.numExeSettings; i++) {
		free(config.exeSettings[i].exe);
	}
	free(config.exeSettings);

	for (i=0; i<config.numExtraChars; i++) {
		free(config.extraChars[i].name);
		free(config.extraChars[i].file);
	}
	free(config.extraChars);
	free(config.forceFontFace);
	memset(&config, 0, sizeof(config));

	for (i=0; i<6; i++) {
		config.defaultShadow[i] = D3DCOLOR_RGBA(0,0,0,90);
		config.defaultOutline[i] = D3DCOLOR_RGBA(0,0,0,255);
		config.defaultColors[i] = D3DCOLOR_RGBA(255,255,255,255);
	}

	config.minSubDuration = 2000;

	// Prevents a crash on quit, as d3d will be unloaded first and tends not
	// to clean up nicely.
	if (!noNukeFont)
		NukeFont();
}

void LoadConfig() {
	int i;
	ClearConfig();

	// Defaults.  Correspdonging lines in cfg file overrides.
	config.forceFontFace = wcsdup(L"Arial");
	const static CharInfo CharacterInfo[] = {
		{AG3, "0", 0, D3DCOLOR_RGBA(255,255,255,255)},
		{AG3, "1", 0, D3DCOLOR_RGBA(255,  0,  0,255)},
		{AG3, "2", 0, D3DCOLOR_RGBA(  0,255,  0,255)},
		{AG3, "3", 0, D3DCOLOR_RGBA(  0,255,255,255)},
		{AG3, "4", 0, D3DCOLOR_RGBA(255,255,  0,255)},
		{AG3, "5", 0, D3DCOLOR_RGBA(255,  0,255,255)},

		{SB3, "Manami", L"sb3_0601.pp", D3DCOLOR_RGBA(200,  0,200,255)},
		{SB3, "Esk",    L"sb3_0602.pp", D3DCOLOR_RGBA(255,210, 25,255)},//FDD017
		{SB3, "Eo",     L"sb3_0603.pp", D3DCOLOR_RGBA(215,160, 25,255)},
		{SB3, "Reiko",  L"sb3_0604.pp", D3DCOLOR_RGBA(235, 35, 25,255)},
		{SB3, "Maria",  L"sb3_0605.pp", D3DCOLOR_RGBA(200,255, 95,255)},
		{SB3, "Maya",  L"sb3x_1606.pp", D3DCOLOR_RGBA(160,110, 34,255)},
		{SB3, "Bael",  L"sb3x_1607.pp", D3DCOLOR_RGBA( 90,140,255,255)},
		{SB3, "Fei",   L"sb3x_1608.pp", D3DCOLOR_RGBA(120,100,255,255)},

		{RL, "Manaka", L"RPP_11.pp", D3DCOLOR_RGBA(  0,128,255,255)},
		{RL, "Aoi",    L"RPP_12.pp", D3DCOLOR_RGBA(255,  0,  0,255)},
		{RL, "Yuko",   L"RPP_13.pp", D3DCOLOR_RGBA(  0,230,  0,255)},

		{SM, "Yume",    L"sm03_00.pp", D3DCOLOR_RGBA(255,201,206,255)},
		{SM, "Natsume", L"sm03_01.pp", D3DCOLOR_RGBA( 36, 84,255,255)},
		{SM, "Fumi",    L"sm03_02.pp", D3DCOLOR_RGBA( 40,200,120,255)},
		{SM, "Mikoto",  L"sm03_03.pp", D3DCOLOR_RGBA(255,120,135,255)},
		{SM, "Hiyo",    L"sm03_04.pp", D3DCOLOR_RGBA(180,110,110,255)},

		{AHM, "Nanoha",    L"AHM02_00.pp", D3DCOLOR_RGBA(210,50,90,255)},
		{AHM, "Noa",    L"AHM02_01.pp", D3DCOLOR_RGBA(235,201,128,255)},
		{AHM, "Yakumo",    L"AHM02_02.pp", D3DCOLOR_RGBA(210,155,215,255)},

		{YUUSHA, "Saga", L"mo_00_00_11.pp", D3DCOLOR_RGBA(0x9a, 0x9d, 0xe0, 255)},
		{YUUSHA, "Sefi", L"mo_00_00_12.pp", D3DCOLOR_RGBA(0x28, 0x2c, 0x8e, 255)},
		{YUUSHA, "Enis",   L"mo_00_00_13.pp", D3DCOLOR_RGBA(0x3e, 0x5c, 0x9a, 255)},
		{YUUSHA, "Eclair", L"mo_00_00_14.pp", D3DCOLOR_RGBA(0xf4, 0xf3, 0xd5, 255)},
		{YUUSHA, "Ruidia", L"mo_00_00_15.pp", D3DCOLOR_RGBA(0x96, 0xa8, 0xbc, 255)},
		{YUUSHA, "Viara",  L"mo_00_00_16.pp", D3DCOLOR_RGBA(0xe9, 0x7c, 0xc6, 255)},
		{YUUSHA, "Mimas",  L"mo_00_00_17.pp", D3DCOLOR_RGBA(0x70, 0x0a, 0xab, 255)},

		{SM2, "Yukariko",  L"qa_00_00_09.pp", D3DCOLOR_RGBA(0xD8, 0x00, 0xFF, 255)},
		{SM2, "Suho",  L"qa_00_00_10.pp", D3DCOLOR_RGBA(0xFF, 0x00, 0x00, 255)},
		{SM2, "Asagi",  L"qa_00_00_11.pp", D3DCOLOR_RGBA(0x75, 0xB4, 0xFF, 255)},
		{SM2, "Kohaku",  L"qa_00_00_12.pp", D3DCOLOR_RGBA(0xF9, 0xBB, 0x00, 255)},

		{SB_ZERO, "Ai",      L"sb02_00.pp", D3DCOLOR_RGBA(0xC8, 0x7C, 0x5B, 255)},
		{SB_ZERO, "Bael",    L"sb02_01.pp", D3DCOLOR_RGBA(0x60, 0x90, 0xFF, 255)},
		{SB_ZERO, "Rin",     L"sb02_02.pp", D3DCOLOR_RGBA(0xFF, 0xC4, 0x8E, 255)},
		{SB_ZERO, "Hotaru",  L"sb02_03.pp", D3DCOLOR_RGBA(0xFF, 0xDD, 0x75, 255)},
		{SB_ZERO, "Setsuna", L"sb02_04.pp", D3DCOLOR_RGBA(0x7A, 0x73, 0xFF, 255)},
	};
	for (i=0; i<sizeof(CharacterInfo)/sizeof(CharacterInfo[0]); i++) {
		config.extraChars = (CharInfo*) realloc(config.extraChars, sizeof(CharInfo)*(config.numExtraChars+1));
		config.extraChars[config.numExtraChars].color = CharacterInfo[i].color;
		config.extraChars[config.numExtraChars].outline = config.defaultOutline[0];
		config.extraChars[config.numExtraChars].shadow = config.defaultShadow[0];
		config.extraChars[config.numExtraChars].name = strdup(CharacterInfo[i].name);
		config.extraChars[config.numExtraChars].game = CharacterInfo[i].game;
		config.extraChars[config.numExtraChars].file = 0;
		if (CharacterInfo[i].file)
			config.extraChars[config.numExtraChars].file = wcsdup(CharacterInfo[i].file);
		config.numExtraChars++;
	}

	HANDLE in = CreateFileA("Subtitle.cfg", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (in == INVALID_HANDLE_VALUE) {
		MoveFile("AG3Overlay.cfg", "Subtitle.cfg");
		in = CreateFileA("Subtitle.cfg", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	}

	if (in != INVALID_HANDLE_VALUE) {
		DWORD size = GetFileSize(in, 0), d;
		char *data = (char*) malloc(size+1);
		D3DCOLOR outline = config.defaultOutline[0];
		D3DCOLOR shadow = config.defaultShadow[0];
		int t = ReadFile(in, data, size, &d, 0);
		CloseHandle(in);
		if (t && d == size) {
			data[size] = 0;
			char *pos = data;
			// Remove BOM marker, if there is one.
			if (pos[0] == (char)0xEF && pos[1] == (char)0xBB && pos[2] == (char)0xBF) pos += 3;
			while (*pos) {
				while (*pos == '\r' || *pos == '\n' || *pos == '\t' || *pos == ' ') pos++;
				char *end = pos+1+strcspn(pos+1, "\r\n");
				if (*end) {
					end[0] = 0;
					end++;
				}
				if (*pos == '/') {
					pos = end;
					continue;
				}
				char *v1, *v2, *v3;
				v3 = v2 = v1 = pos;
				while (*pos && *pos != '(' && *pos != '=') pos++;
				if (*pos == '=') {
					v2 = pos;
					*pos = 0;
					v3 = pos+1;
				}
				if (*pos == '(') {
					*pos = 0;
					v3 = v2 = ++pos;
					while (*pos && *pos != ')') pos++;
					if (*pos == ')') {
						*pos = 0;
						pos++;
					}
					while (*pos == ' ' || *pos == '\t') pos++;
					if (*pos == '=') {
						v3 = pos+1;
						*pos = 0;
					}
				}
				if (v1 != v2 && v2 != v3) {
					while (*v2 == ' ' || *v2 == '\t') v2++;
					while (*v3 == ' ' || *v3 == '\t') v3++;
					// Should be a better way to do this, but I'm lazy.
					char *e1 = strchr(v1, 0);
					char *e2 = strchr(v1, 0);
					char *e3 = strchr(v1, 0);
					while (v1 < e1 && (e1[-1] == ' ' || e1[-1] == '\t')) e1--;
					e1[0] = 0;
					while (v2 < e2 && (e2[-1] == ' ' || e2[-1] == '\t')) e2--;
					e2[0] = 0;
					while (v3 < e3 && (e3[-1] == ' ' || e3[-1] == '\t')) e3--;
					e3[0] = 0;
					if (*v1 && !*v2 && *v3) {
						if (!stricmp(v1, "Resolution")) {
							int w, h;
							if (2 == sscanf(v3, " %i , %i", &w, &h) && w >= 100 && h >= 100) {
								config.width = w;
								config.height = h;
							}
						}
						else if (!stricmp(v1, "LoadSubs")) {
							config.subFiles = (wchar_t **) realloc(config.subFiles, sizeof(wchar_t*)*(config.numSubFiles+1));
							int len = (int) strlen(v3) + 1, len2;
							config.subFiles[config.numSubFiles] = (wchar_t*) malloc(len * sizeof(wchar_t));
							if ((len2 = MultiByteToWideChar(CP_UTF8, 0, v3, -1, config.subFiles[config.numSubFiles], len)) &&
								len2 <= len) {
									config.numSubFiles++;
							}
							else {
								free(config.subFiles[config.numSubFiles]);
							}
						}
						else if (!stricmp(v1, "AG3Names")) {
							unsigned int i = atoi(v3);
							if (i < 3) {
								config.ag3Names = (char)i;
							}
						}
						else if (!stricmp(v1, "HideGirlNames")) {
							config.hideGirlNames = (0 != atoi(v3));
						}
						else if (!stricmp(v1, "Clock")) {
							config.displayClock = atoi(v3)%3;
						}
						else if (!stricmp(v1, "Hacks")) {
							config.hacksEnabled = atoi(v3)%3;
						}
						else if (!stricmp(v1, "View")) {
							config.viewEnabled = atoi(v3)%3;
						}
						else if (!stricmp(v1, "HakoImmortality")) {
							config.hakoImmortality = atoi(v3)%4;
						}
						else if (!stricmp(v1, "ahmNoLimit")) {
							config.ahmNoLimit = atoi(v3)%2;
						}
						else if (!stricmp(v1, "ahmBreastSizes")) {
							config.ahmBreastSizesDword = strtoul(v3, 0, 16);
						}
						else if (!stricmp(v1, "ForceLocale") && strlen(v3) < 5) {
							config.forceLocale = strtoul(v3, 0, 16);
						}
						else if (!stricmp(v1, "MinSubDuration")) {
							unsigned int t = atoi(v3);
							if (t > 100000) t = 100000;
							config.minSubDuration = t;
						}
						else if (!stricmp(v1, "HakoRomanizeNames")) {
							config.hakoRomanizeNames = atoi(v3);
						}
					}
					else if (*v1 && *v2 && *v3) {
						D3DCOLOR c;
						int w = sscanf(v3, " %x", &c);
						if (!stricmp(v1, "DefaultOutline") && !stricmp(v2, "Color") && sscanf(v3, " %x", &c) > 0) {
							config.defaultOutline[0] = (c>>8)|c<<(24);
						}
						else if (!stricmp(v1, "DefaultShadow") && !stricmp(v2, "Color") && sscanf(v3, " %x", &c) > 0) {
							config.defaultShadow[0] = (c>>8)|c<<(24);
						}
						else if (!stricmp(v1, "Shadow") && !stricmp(v2, "Color") && sscanf(v3, " %x", &c) > 0) {
							shadow = (c>>8)|c<<(24);
						}
						else if (!stricmp(v1, "Outline") && !stricmp(v2, "Color") && sscanf(v3, " %x", &c) > 0) {
							outline = (c>>8)|c<<(24);
						}
						else if (!stricmp(v1, "Color") && sscanf(v3, " %x", &c) > 0) {
							c |= 0xFF000000;
							config.extraChars = (CharInfo*) realloc(config.extraChars, sizeof(CharInfo)*(config.numExtraChars+1));
							config.extraChars[config.numExtraChars].color = c;
							config.extraChars[config.numExtraChars].outline = outline;
							config.extraChars[config.numExtraChars].shadow = shadow;
							config.extraChars[config.numExtraChars].name = strdup(v2);
							config.extraChars[config.numExtraChars].game = Custom;
							config.extraChars[config.numExtraChars].file = 0;
							config.numExtraChars++;
						}
						else if (!stricmp(v1, "Girl")) {
							char *p = strrchr(v3, ',');
							if (p) {
								char *p2 = p+1;
								while (p > v3 && (p[-1] == '\t' || p[-1] == ' ')) p--;
								*p = 0;
								if (sscanf(p2, " %x", &c)>0 && *v2 && *v3 && strlen(v2) < 2000) {
									c |= 0xFF000000;
									config.extraChars = (CharInfo*) realloc(config.extraChars, sizeof(CharInfo)*(config.numExtraChars+1));
									config.extraChars[config.numExtraChars].color = c;
									config.extraChars[config.numExtraChars].outline = outline;
									config.extraChars[config.numExtraChars].shadow = shadow;
									config.extraChars[config.numExtraChars].name = strdup(v3);
									config.extraChars[config.numExtraChars].game = Custom;
									wchar_t temp[10000];
									MultiByteToWideChar(CP_UTF8, 0, v2, -1, temp, sizeof(temp)/sizeof(wchar_t));
									config.extraChars[config.numExtraChars].file = wcsdup(temp);
									config.numExtraChars++;
								}
							}
						}
						else if (!stricmp(v1, "Font")) {
							if (!stricmp(v2, "Face")) {
								wchar_t temp[10000];
								MultiByteToWideChar(CP_UTF8, 0, v3, -1, temp, sizeof(temp)/sizeof(wchar_t));
								free(config.forceFontFace);
								config.forceFontFace = wcsdup(temp);
							}
							else if (!stricmp(v2, "Height")) {
								config.forceFontHeight = atoi(v3);
							}
						}
						else if (!stricmp(v1, "Exe") && strlen(v3) < MAX_PATH) {
							wchar_t temp[MAX_PATH+1];
							if (MultiByteToWideChar(CP_UTF8, 0, v3, -1, temp, sizeof(temp)/2)) {
								int w;
								for (w=0; w<config.numExeSettings; w++) {
									if (!wcsicmp(config.exeSettings[w].exe, temp)) break;
								}
								if (w == config.numExeSettings) {
									config.exeSettings = (ExeSettings*) realloc(config.exeSettings, sizeof(ExeSettings) * (++config.numExeSettings));
									config.exeSettings[w].exe = wcsdup(temp);
									config.exeSettings[w].setting = 0;
								}
								int match = !wcsicmp(state.exeName, temp);
								if (!stricmp(v2, "Exclude")) {
									config.exeSettings[w].setting |= EXE_EXCLUDE;
									state.Excluded |= match;
								}
								else if (!stricmp(v2, "RegOnly")) {
									config.exeSettings[w].setting |= EXE_REG_ONLY;
									state.regOnly |= match;
								}
								else if (!stricmp(v2, "ForceReg")) {
									config.exeSettings[w].setting |= EXE_FORCE_REG;
									state.forceReg |= match;
								}
							}
						}
					}
				}
				pos = end;
			}
		}
		free(data);
	}
	else {
		// Loading failed.
		// Use default exe values.  Everything else will always have defaults preloaded.
		const static wchar_t *RegOnly[] = {
			L"AG3_Make.exe",
			L"JS3_Make.exe",
			L"AG3_Play.subtitle_v1.5.3.exe",
			L"AG3_Play.subtitle_v1.5.3.debug.exe",
			L"AG3_Play.subtitle_v1.5.3.debug2.exe",
			L"AG3_Play.subtitle_v1.5.3.debug2+log.exe"
		};
		config.exeSettings = (ExeSettings*)malloc(sizeof(ExeSettings) * (sizeof(RegOnly)/sizeof(RegOnly[0])));
		while (config.numExeSettings < sizeof(RegOnly)/sizeof(RegOnly[0])) {
			config.exeSettings[config.numExeSettings].exe = wcsdup(RegOnly[config.numExeSettings]);
			config.exeSettings[config.numExeSettings].setting = EXE_REG_ONLY;
			config.numExeSettings++;
		}
	}

	if (!config.numSubFiles) {
		config.numSubFiles = 3;
		config.subFiles = (wchar_t **) realloc(config.subFiles, sizeof(wchar_t*)*3);
		config.subFiles[0] = wcsdup(L"data\\subs\\Override.txt");
		config.subFiles[1] = wcsdup(L"data\\subs\\subtitles.txt");
		config.subFiles[2] = wcsdup(L"subtitles.txt");
	}

	for (i=0; i<config.numExtraChars; i++) {
		CharInfo info = config.extraChars[i];
		if (!info.file && strlen(info.name) == 1 && info.name[0] >= '0' && info.name[0] <= '5') {
			config.defaultColors[info.name[0] - '0'] = info.color;
			config.defaultShadow[info.name[0] - '0'] = info.shadow;
			config.defaultOutline[info.name[0] - '0'] = info.outline;
		}
		if (!i) continue;
		int j = i-1;
		int cmp = 0;
		while (j >= 0) {
			if (!info.file) {
				if (config.extraChars[j].file) {
					cmp = -1;
				}
				else {
					cmp = stricmp(info.name, config.extraChars[j].name);
				}
			}
			else {
				if (!config.extraChars[j].file) cmp = 1;
				else {
					cmp = wcsicmp(info.file, config.extraChars[j].file);
				}
			}
			if (cmp < 0) {
				config.extraChars[j+1] = config.extraChars[j];
				j--;
				continue;
			}
			if (cmp > 0) break;
			break;
		}
		if (!cmp) {
			free(config.extraChars[j].file);
			free(config.extraChars[j].name);
			config.extraChars[j] = info;
			memmove(config.extraChars+j+1, config.extraChars+j+2, (config.numExtraChars - j - 2) * sizeof(config.extraChars[0]));
			config.numExtraChars--;
			i--;
			continue;
		}
		config.extraChars[j+1] = info;
	}
}

void LocaleRestart(HWND hWndDlg) {
	char temp[10];
	int needRestart = 0;
	int t2 = GetEnvironmentVariable("AppLocaleID", temp, 10) <= 0;
	if (!config.forceLocale) return;
	/*
	if (!config.forceLocale && !t2) {
		// Only restart with 0000 when modified in the dialog.
		if (hWndDlg)
			needRestart = 1;
	}//*/
	if (config.forceLocale && (t2 || strtoul(temp, 0, 16) != config.forceLocale)) {
		needRestart = 1;
	}
	if (!needRestart) return;

	if (hWndDlg) {
		MessageBox(hWndDlg, "Changing locale requires restart.", "Restart Needed", MB_OK);
	}
	if (config.forceLocale) {
		SetEnvironmentVariable("__COMPAT_LAYER", "#ApplicationLocale");
		sprintf(temp, "%04X", config.forceLocale);
		SetEnvironmentVariable("AppLocaleID", temp);
	}
	else {
		SetEnvironmentVariable("__COMPAT_LAYER", 0);
		SetEnvironmentVariable("AppLocaleID", 0);
	}
	wchar_t *v = GetCommandLineW();
	int argc = 1;
	wchar_t **argv = &v;
	wchar_t **argv2 = 0;

	// What's this for?  When launching an exe with spaces in its path
	// with no args, v may be an unquoted path, which doesn't work with
	// CommandLineToArgv.  This checks for that case.
	//
	// Conveniently, this behavior is not mentioned by Microsoft.  They state
	// CommandLineToArgv always works in their docs.
	if (wcsicmp(v, state.exePath)) {
		argv = argv2 = CommandLineToArgvW(v, &argc);
	}

	wchar_t **temp2 = (wchar_t **)calloc(argc+1, sizeof(wchar_t*));
	memcpy(temp2, argv, sizeof(wchar_t*) * argc);
	_flushall();
	_wexecvp(argv[0], temp2);
	// Process killed here, so no need to do anything else, if all goes well.
	// Just in case, though...
	free(temp2);
	LocalFree(argv2);
}
