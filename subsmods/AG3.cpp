// Almost all AG3-specific code is here.

#include "Shrink.h"
#include "AG3.h"
#include "D3DTextOverlay.h"
#include <time.h>

struct AG3Clock {
	unsigned int time, day;
};

struct AG3SexInfo {
	float *guyOrgasm, *girlOrgasm;
	int *girlArousal, *girlLewdness;
	float *scrollPos;
	int *cumGauge;
	int *orgasmCounter;
};

struct AG3CharStatus {
	union {
		struct {
			short *jealousy, *love, *friendship;
			// Actually 32-bit, but upper 16-bits aren't used.
			short *pee, *sleep, *food, *bath, *ecchi;
		};
		short *feelings[8];
	};

	union {
		struct {
			char *first, *last, *file;
		};
		char *names[3];
	};
};



static AG3Clock *GetAG3Time() {
	// Prevent a massive number of exceptions,
	// which can really slow things down quite a bit.
	static time_t lastTime = 0;
	static char happy = 0;
	if (!happy) {
		time_t c = time(0);
		if (c == lastTime) return 0;
		lastTime = c;
	}
	int *pos = ((int**)0x6B329C)[0]+10;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0]+61;
	if (IsBadReadPtr(pos, 8)) return 0;
	happy = 1;
	return (AG3Clock*) pos;
}

static int GetAG3SexInfo(AG3SexInfo &info) {
	info.guyOrgasm = (float*)0x6BC750;
	info.girlOrgasm = (float*)0x6BC74C;
	info.girlArousal = (int*)0x6BC738;
	info.girlLewdness = (int*)0x6BC734;

	info.scrollPos = (float*)0x699078;
	info.cumGauge = (int*)0x6BC91C;
	info.orgasmCounter = (int*)0x6BC92C;

	int *test = (int*)0x6BC7B0;
	return *test != 0;
}

static int GetAG3CharStatus(int index, AG3CharStatus &out) {
	// Prevent a massive number of exceptions,
	// which can really slow things down quite a bit.
	static time_t lastTime = 0;
	static char happy = 0;
	if (!happy) {
		time_t c = time(0);
		if (c == lastTime) return 0;
		lastTime = c;
	}
	int *pos = ((int**)0x6B329C)[0]+10;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0]+11;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0]+index;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0];
	// Minor optimization.  Odd pointer to data that's aligned
	// only occurs when fewer than index girls in the game, so
	// avoid an exception in some cases.
	if ((1&(int)pos) || IsBadReadPtr(pos, 4*6853)) return 0;
	out.file = (char*)((int)pos + 12);
	out.first = (char*)((int)pos + 2332);
	out.last = (char*)((int)pos + 2268);
	pos = (int*)pos[6853] + 1241;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0];
	if (IsBadReadPtr(pos, 0xA4)) return 0;
	int **needs = ((int***)pos)[4];
	if (IsBadReadPtr(needs, 20)) return 0;
	pos = (int*)pos[40] + 2;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*)pos[0]+13;
	if (IsBadReadPtr(pos, 4)) return 0;
	pos = (int*) pos[0];
	if (IsBadReadPtr(pos, 6)) return 0;

	out.jealousy = (short*)pos;
	out.love = (short*)pos+1;
	out.friendship = (short*)pos+2;

	out.pee = (short*)needs[0];
	out.sleep = (short*)needs[1];
	out.food = (short*)needs[2];
	out.bath = (short*)needs[3];
	out.ecchi = (short*)needs[4];
	happy = 1;
	return 1;
}


static int GetAG3CharInfo(CharInfo &tempChar, int id) {
	AG3CharStatus status;
	static char UTF8Name[1000];
	wchar_t wName[1000];
	if (id <= 0 || id > 5) {
		id = 0;
	}
	tempChar.color = config.defaultColors[id];
	tempChar.outline = config.defaultOutline[id];
	tempChar.shadow = config.defaultShadow[id];
	tempChar.game = Dunno;
	tempChar.name = 0;
	if (!id) {
		return 0;
	}
	if (!GetAG3CharStatus(id-1, status) || strlen(status.names[config.ag3Names]) > 100)
		return 0;
	if (MultiByteToWideChar(CP_ACP, 0, status.names[config.ag3Names], -1, wName, sizeof(wName)/2) <= 0) return 0;
	if (WideCharToMultiByte(CP_UTF8, 0, wName, -1, UTF8Name, sizeof(UTF8Name), 0, 0) <= 0) return 0;
	tempChar.name = UTF8Name;
	tempChar.game = AG3;
	for (int i=0; i<config.numExtraChars; i++) {
		if (!config.extraChars[i].file && !stricmp(config.extraChars[i].name, UTF8Name)) {
			tempChar.color = config.extraChars[i].color;
			tempChar.shadow = config.extraChars[i].shadow;
			tempChar.outline = config.extraChars[i].outline;
		}
	}
	return 1;
}

int AG3Handler::GetCharInfo(CharInfo &tempChar, int *stack, wchar_t *ppFile, char *waveName) {
	if (!wcsnicmp(ppFile, L"js3_00_05_", 9) || !wcsnicmp(ppFile, L"js3_01_05_", 9)) {
		int ident = stack[152];
		GetAG3CharInfo(tempChar, ident);
		return 1;
	}
	return 0;
}

void AG3Handler::DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite) {
	if (!resMatch) {
		if (!config.height || !config.width) resMatch = 1;
		else if (!IsBadReadPtr((void*)0x6B2C74, 60)) {
			POINT *p = (POINT*)0x6B2C74;
			float *scale = (float*) 0x6B2CAC;
			if (state.gameRect.right == p->x && state.gameRect.bottom == p->y && fabs(*scale * 768 - p->y) < 1) {
				p->x = config.width;
				p->y = config.height;
				*scale = p->y / 768.0f;
				resMatch = 1;
			}
		}
	}
	if(screenShot > 0) {
		if (--screenShot == 0) keybd_event(VK_F11, 0x57, KEYEVENTF_KEYUP, 0);
		return;
	}
	if (!config.hacksEnabled  && !config.viewEnabled && !config.displayClock) return;
	if (d3dSprite) {
		d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	if ((config.hacksEnabled || config.viewEnabled) && d3dMiniFont) {
		int bottom = state.d3dRect.bottom;
		if (!resMatch && state.gameRect.bottom < bottom) {
			bottom = state.gameRect.bottom;
		}
		int pos = 20;
		bottom -= bottom/24;
		RECT rect = {
			0, bottom - 143, pos, bottom
		};
		RECT start = rect;
		char output[1000];
		int j, i;
		for (i=0; i<5; i++) {
			AG3CharStatus stats;
			if (!GetAG3CharStatus(i, stats)) break;
			// Simplest way to get color, if rather messy.
			CharInfo info;
			GetAG3CharInfo(info, i+1);

			RECT rect3 = {
				0,0,0,0
			};
			d3dMiniFont->DrawTextA(d3dSprite, stats.names[config.ag3Names], -1, &rect3, DT_CALCRECT | DT_TOP | DT_NOCLIP, D3DCOLOR_RGBA(255,255,255,255));
			int width = 40;
			if (rect3.right > width) width = rect3.right;
			pos += width+10;
			RECT rect2 = {
				0, bottom - 160, pos, bottom
			};
			DisplayMiniTextAtPos(stats.names[config.ag3Names], &rect2, info.color, D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
			rect2.top += 17;
			for (j=0; j<8; j++) {
				itoa(stats.feelings[j][0], output, 10);
				unsigned long  color = D3DCOLOR_RGBA(255,255,255,255);
				if (config.hacksEnabled  && j == selectedStat && (!selectedGirl || selectedGirl-1 == i))
					color = D3DCOLOR_RGBA(0,255,0,255);
				DisplayMiniTextAtPos(output, &rect2, color, D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
				rect2.top += 16;
			}
		}
		if (i) {
			AG3SexInfo info;
			rect.right = pos + 60;

			DisplayMiniTextAtPos("J:\nL:\nF:\nP:\nS:\nH:\nB:\nE:", &start, D3DCOLOR_RGBA(0,255,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
			DisplayMiniTextAtPos("[2000]\n[2000]\n[2000]\n[100]\n[180]\n[160]\n[140]\n[240]", &rect, D3DCOLOR_RGBA(255,0,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);

			if (GetAG3SexInfo(info)) {
				rect.top -= 17;
				rect.right = state.d3dRect.right-50;
				DisplayMiniTextAtPos("Guy Orgasm:\nGirl Orgasm:\n\nGirl Arousal:\nGirl Lewdness:\n\nSpeed:\nCum Guage:\nOrgasm Counter:", &rect, D3DCOLOR_RGBA(0,255,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
				rect.right += 35;
				sprintf(output, "%0.0f\n%0.0f\n\n%i\n%i\n\n%0.0f\n%i\n%i", *info.guyOrgasm, *info.girlOrgasm, *info.girlArousal, *info.girlLewdness, (*info.scrollPos-1)*25, *info.cumGauge, *info.orgasmCounter);
				DisplayMiniTextAtPos(output, &rect, D3DCOLOR_RGBA(255,255,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
			}
		}
	}
	if (config.displayClock) {
		AG3Clock *clock = GetAG3Time();
		if (clock) {
			wchar_t time[100];
			int hour = clock->time/3600000;
			int min = clock->time/60000 - hour*60;
			if (hour < 12) hour += 12;
			else hour -= 12;
			if (config.displayClock == 1)
				swprintf(time, L"Day %i, %02i:%02i", clock->day+1, hour, min);
			else {
				char c = 'a';
				if (hour >= 12) {
					c = 'p';
					hour -= 12;
				}
				if (!hour) hour = 12;
				swprintf(time, L"Day %i, %i:%02i %cm", clock->day+1, hour, min, c);
			}
			DisplayTextAtPos(time, state.d3dRect.right-8, state.d3dRect.bottom-6, D3DCOLOR_RGBA(255,255,255,255), config.defaultOutline[0], config.defaultShadow[0], d3dSprite, DT_RIGHT | DT_BOTTOM | DT_NOCLIP);
		}
	}
	if (d3dSprite) {
		d3dSprite->End();
	}
}

static void DisplayAG3CheatHelp() {
	AddText(20000, "     Ctrl-[Shift]-G selects a girl", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-[Shift]-S selects a stat", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     +/- adjust selected stat", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Shift/Ctrl +/- adjust girl/guy orgasm", D3DCOLOR_RGBA(0,255,0,255));
}

void AG3Handler::DisplayHelp() {
	AddText(20000, "     Ctrl-X toggles cheats", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-V toggles stat view", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-T toggles clock", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     F12 to take screenshot w/o stats", D3DCOLOR_RGBA(0,255,255,255));
	if (config.hacksEnabled) {
		DisplayAG3CheatHelp();
	}
}

int AG3Handler::HandleKeyPress(int c, int control, int shift) {
	if (config.hacksEnabled) {
		const short max[8] = {
			2000, 2000, 2000, 100, 180, 160, 140, 240
		};
		AG3CharStatus stats;
		AG3SexInfo info;
		if (c == VK_ADD || c == VK_OEM_PLUS) {
			if (shift || control) {
				if (GetAG3SexInfo(info)) {
					int delta = 100*(c == VK_ADD) + 20 * (c == VK_OEM_PLUS);
					*info.girlOrgasm += shift * delta;
					if (*info.girlOrgasm >= 99) *info.girlOrgasm = 99;
					*info.guyOrgasm += control * delta;
					if (*info.guyOrgasm >= 99) *info.guyOrgasm = 99;
				}
			}
			else for (int i=0; i<5; i++) {
				if (selectedGirl == 0 || selectedGirl - 1 == i) {
					if (!GetAG3CharStatus(i, stats)) break;
					stats.feelings[selectedStat][0] += max[selectedStat]/10;
					if (stats.feelings[selectedStat][0] > max[selectedStat])
						stats.feelings[selectedStat][0] = max[selectedStat];
				}
			}
			return 1;
		}
		else if (c == VK_SUBTRACT || c == VK_OEM_MINUS) {
			if (shift || control) {
				if (GetAG3SexInfo(info)) {
					int delta = 100*(c == VK_SUBTRACT) + 20 * (c == VK_OEM_MINUS);
					*info.girlOrgasm -= shift * delta;
					if (*info.girlOrgasm < 0) *info.girlOrgasm = 0;
					*info.guyOrgasm -= control * delta;
					if (*info.guyOrgasm < 0) *info.guyOrgasm = 0;
				}
			}
			else for (int i=0; i<5; i++) {
				if (selectedGirl == 0 || selectedGirl - 1 == i) {
					if (!GetAG3CharStatus(i, stats)) break;
					stats.feelings[selectedStat][0] -= max[selectedStat]/10;
					stats.feelings[selectedStat][0] &= (-stats.feelings[selectedStat][0]>>31);
				}
			}
			return 1;
		}
		else if (control) {
			if (c == 'G') {
				selectedGirl = (selectedGirl + 1 + shift*4) % 6;
				if (selectedGirl && !GetAG3CharStatus(selectedGirl-1, stats)) {
					selectedGirl = 0;
				}
				return 1;
			}
			else if (c == 'S') {
				selectedStat = (selectedStat + 1 + shift*6) % 8;
				return 1;
			}
		}
	}
	if(c == VK_F12) {
		screenShot = 1;
		keybd_event(VK_F11, 0x57, 0, 0);
	}
	if (!control) return 0;
	if (c == 'X') {
		config.hacksEnabled = !config.hacksEnabled;
		if (!config.hacksEnabled) {
			//AddText(3000, "Hack Mode 0: Disabled", D3DCOLOR_RGBA(255,255,255,255));
			AddText(3000, "Cheats Disabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		else if (config.hacksEnabled == 1) {
			if (!config.viewEnabled) CleanupAllLines();
			AddText(3000, "Cheats Enabled", D3DCOLOR_RGBA(255,255,255,255));
			//AddText(3000, "Hack Mode 1: Enabled, no display", D3DCOLOR_RGBA(255,255,255,255));
			DisplayAG3CheatHelp();
		}
		/*else if (config.hacksEnabled == 2) {
			//AddText(3000, "Hack Mode 2: Enabled with display", D3DCOLOR_RGBA(255,255,255,255));
		}//*/
		SaveConfig();
		return 1;
	}
	else if (c == 'V') {
		config.viewEnabled = !config.viewEnabled;
		if (!config.viewEnabled) {
			AddText(3000, "Stats View Disabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		else {
			if (!config.hacksEnabled) CleanupAllLines();
			AddText(3000, "Stats View Enabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		SaveConfig();
		return 1;
	}
	else if (c == 'T') {
		config.displayClock = (config.displayClock+1)%3;
		if (!config.displayClock) {
			AddText(3000, "Clock Disabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		else if (config.displayClock == 1) {
			AddText(3000, "24-Hour Clock Enabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		else {
			AddText(3000, "12-Hour Clock Enabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		SaveConfig();
		return 1;
	}
	return 0;
}
