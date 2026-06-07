#include "Shrink.h"
#include "pp.h"
#include "resource1.h"

int numFiles = 0;
PPHeader **files = 0;

wchar_t **cleanup = 0;
int numCleanup = 0;

INT_PTR CALLBACK DialogProc(      
    HWND hWndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {
		static HWND nextClipboardListener = 0;
		if (uMsg == WM_INITDIALOG) {
			nextClipboardListener = SetClipboardViewer(hWndDlg);
			SetWindowText(hWndDlg, WAVE_MONITOR_NAME);
			return 1;
		}
		else if (uMsg == WM_DESTROY) {
			if (nextClipboardListener) {
				ChangeClipboardChain(hWndDlg, nextClipboardListener);
				nextClipboardListener  = 0;
			}
		}
		else if (uMsg == WM_COMMAND) {
			int cmd = LOWORD(wParam);
			if (HIWORD(wParam)==BN_CLICKED) {
				if (cmd == IDCANCEL) {
					if (nextClipboardListener) {
						ChangeClipboardChain(hWndDlg, nextClipboardListener);
						nextClipboardListener  = 0;
					}
					EndDialog(hWndDlg, 0);
					return 1;
				}
			}
		}
		else if (uMsg == WM_CHANGECBCHAIN) {
			if ((HWND)wParam == nextClipboardListener) nextClipboardListener = (HWND) lParam;
			else SendMessage(nextClipboardListener, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_DRAWCLIPBOARD) {
			if (IsClipboardFormatAvailable(CF_TEXT)) {
				if (OpenClipboard(hWndDlg)) {
					GLOBALHANDLE h = GetClipboardData(CF_TEXT);
					char * text;
					if (h && (text = (char*)GlobalLock(h))) {
						int size = (int) GlobalSize(text);
						int len = 0;
						while (len < size && text[len]) len++;
						char *temp = (char*) malloc(2*(len+15));
						if (len >= 4 && (!stricmp(text+len-4, ".wav") || !stricmp(text+len-4, ".ogg"))) strcpy(temp, text);
						else sprintf(temp, "%s.wav", text);
						for (int i=0; i<numFiles; i++) {
							for (int j=0; j<files[i]->numFiles; j++) {
								if (!stricmp(temp, files[i]->headers[j].name)) {
									wchar_t path[MAX_PATH*6];
									int l = GetEnvironmentVariableW(L"Temp", path, sizeof(path)/3);
									if (l) {
										swprintf(path, L"%s\\%hs", path, temp);
										int size=0;
										char *data = LoadDecryptedFile(files[i], j, &size, 0);
										if (data) {
											FILE *out = _wfopen(path, L"wb");
											if (out) {
												fwrite(data, 1, size, out);
												fclose(out);
											}
											free(data);
											int k;
											for (k=0; k<numCleanup; k++) {
												if (!wcsicmp(cleanup[k], path)) break;
											}
											if (k == numCleanup) {
												cleanup = (wchar_t **) realloc(cleanup, sizeof(wchar_t*)*(numCleanup+1));
												cleanup[numCleanup++] = wcsdup(path);
											}
											ShellExecuteW(hWndDlg, 0, path, 0, 0, SW_SHOW);
										}
									}
								}
							}
						}
						free(temp);
						GlobalUnlock(h);
					}
					CloseClipboard();
				}
			}
			SendMessage(nextClipboardListener, uMsg, wParam, lParam);
		}
		return 0;
}
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {
		wchar_t path[MAX_PATH*2];
		path[0] = 0;
		int i;
		for (i=0; i<3; i++) {
			WIN32_FIND_DATAW findData;
			size_t w = wcslen(path);
			wcscat(path, L"data\\*.pp");
			HANDLE hFind = FindFirstFileW(path, &findData);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					wcscpy(path+w+5, findData.cFileName);
					int error = 0;
					PPHeader *h = LoadPP(path, &error, Dunno);
					if (h) {
						files = (PPHeader**)realloc(files, sizeof(PPHeader*) * (numFiles+1));
						files[numFiles++] = h;
					}
				}
				while (FindNextFileW(hFind, &findData));
				FindClose(hFind);
			}
			wcscpy(path+w, L"..\\");
		}

		if (!numFiles) {
			MessageBox(0, "Can't find pp files", "Eeeep!", MB_OK | MB_ICONERROR);
		}
		else {
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_MONITOR_DIALOG), 0, DialogProc);
		}

		for (i=0; i<numFiles; i++) {
			FreePP(files[i]);
		}
		free(files);
		for (i=0; i<numCleanup; i++) {
			DeleteFileW(cleanup[i]);
			free(cleanup[i]);
		}
		free(cleanup);
		return 0;
}