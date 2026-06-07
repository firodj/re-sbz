// Almost all AG3-specific code is here.

#include "Shrink.h"
#include "Hako.h"
#include "D3DTextOverlay.h"
#include "Util.h"
#include <time.h>

#define HAKO_MORNING	0
#define HAKO_DAY		1
#define HAKO_NIGHT		2
#define HAKO_END_PERIOD	3

struct Food {
	union {
		int stocks[2];
		struct {
			int yourStock;
			int storeStock;
		};
	};
	int whoKnows[42];
};

struct HakoGirlInfo {
	char *name;
	int *age;
	int *lifespan;
	int *mood;
	int *health;
	int *hspecialty;
	int *hspecialty2;
	unsigned long color;
	char loaded;
	char exists;
	// Can't seem to find either of these.
	//int *daysHungry;
	//int *totalDays;
};

struct HakoData {
	char ingame;
	unsigned int *limitBreaks;
	unsigned int *money;
	unsigned int *points;
	unsigned int *day;
	unsigned int *period;
	char *activeChar;
	int activeCharIndex;
	float *friendBar;
	float *loveBar;
	Food *food;
	HakoGirlInfo girls[3];
};

struct HakoGirlOffsetInfo {
	char shortcut;
	char *name;
	int offset;
	int minVal;
	int maxVal;
	int size;
};

HakoGirlOffsetInfo HakoGirlInfo[] = {
	{'Q', "Gender",			0x20, 0,   2, 4},
	{'P', "Personality",	0x24, 1,   6, 4},
	{'B', "Breast Size",	0x28, 0,   3, 4},
	{'T', "Skin Tone",		0x2C, 0,   3, 4},
	{'F', "Face Shape",		0x30, 1,   6, 4},
	{'E', "Eye Color",		0x34, 0,  15, 4},
	{'S', "Hair Style",		0x38, 0,  34, 4},
	{'C', "Hair Color",		0x3C, 0,  14, 4},
	{'G', "Glasses",		0x40, 0,   9, 4},
	{'R', "Ears",			0x44, 0,   5, 4},
	{'6', "H Specialty",	0x48, 0,  11, 4},
	{'D', "Days lived",		0x9C, 0,9999, 4},
	{'L', "Lifespan",		0xA0, 5,9999, 4},
	{'H', "Health",			0xB6, 0,   9, 4},
	{'M', "Mood",			0xBA, 0,   9, 4},
	{'1', "Clothes Set",	0xF2, 0,   3, 4},
	{'2', "Clothing",		0xF6, 0,  20, 4},
};

static int masks[5] = {0, 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF};

static const unsigned int HakoColors[] = {
	D3DCOLOR_RGBA(0xFF,0xFF,0xFF,255),
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


static int GetHakoData(HakoData &data) {
	memset(&data, 0, sizeof(data));
	if (IsBadReadPtr((void*) 0x6CE668, 0x6CF000 - 0x6CE668)) return 0;
	data.limitBreaks = (unsigned int*)0x6CE72C;
	data.money = (unsigned int*)0x6CEE40;
	data.points = (unsigned int*)0x6CEE44;
	data.day = (unsigned int*)0x6CEE50;
	data.period = (unsigned int*)0x6CEE58;
	if (*data.period > 3) return 0;
	int *test = (int*)0x6CEF70;
	if (test[0]) {
		float *v = (float*)(test[0] + 1676);
		if (!IsBadReadPtr(v, 24)) {
			data.friendBar = v;
			data.loveBar = v+2;
		}
	}
	int *p = (int*)0x006CE668;
	if (!IsBadReadPtr(p, 4)) {
		p = (int*) (p[0] + 0xA4);
		if (!IsBadReadPtr(p, sizeof(Food)*20)) {
			data.food = (Food*)p;
			// 		((int*)(((int*)0x006CE668)[0]+0xA4))[0]
		}
	}


	//int **days = (int**)0x6CE734;
	/*FILE *in = fopen("data\\memory\\000005.cdt", "rb");
	if (in) {
		char stuff[1024];
		int size = fread(stuff, 1, sizeof(stuff), in);
		for (int q=0; q<size; q++) {
			stuff[q] ^=0xFF;
			if (q>=4 &&((char*)(days[2]+20))[q-4] != stuff[q]) {
				days=days;
			}
		}
		days=days;
		memcpy(days[1]+20, stuff+4, 190-4);
		days=days;
		days=days;
		days=days;
		days=days;
		fclose(in);
	}//*/
	int i;
	int **girls = (int**)0x6CE734;
	for (i=0; i<3; i++) {
		if (girls[i] &&  !IsBadReadPtr(girls[i], 0xEC)) {
			if (girls[i][20]) {
				data.girls[i].name = (char*)(girls[i]+20);
				data.girls[i].loaded = girls[i][1] != 0;
				if (data.girls[i].loaded) {
					data.activeChar = data.girls[i].name-4;
					data.activeCharIndex = i;
				}
				data.girls[i].color = girls[i][34];
				data.girls[i].exists = girls[i][98];
				data.girls[i].age = &girls[i][0xE8/4];
				data.girls[i].lifespan = data.girls[i].age+1;
				data.girls[i].mood = data.girls[i].age+8;
				data.girls[i].health = data.girls[i].age+7;

				data.ingame = 1;
			}
		}
	}
	if (!state.hakoCachedCharName[0] && state.hakoActiveCDT[0]) {
		FILE *in = _wfopen(state.hakoActiveCDT, L"rb");
		if (in) {
			int size = fread(state.hakoCachedCDT, 1, 256, in);
			if (size == 255) {
				if (state.hakoCachedCDT[3]) {
					for (i=0; i<255; i++) {
						state.hakoCachedCDT[i] ^= 0xFF;
					}
				}
				char *info = state.hakoCachedCDT;
				wchar_t wideName[1024];
				if (!MultiByteToWideChar(932, 0, &state.hakoCachedCDT[4], -1, wideName, sizeof(wideName)/sizeof(wchar_t)))
					size = 0;
				else {
					if (config.hakoRomanizeNames) {
						wchar_t *name = JISNameToRomanji(wideName);
						if (name) {
							char temp2[24];
							AddFunkyASCII(name);
							if (WideCharToMultiByte(932, 0, name, -1, temp2, sizeof(temp2), 0, 0)) {
								for (i=0; i<24; i++) {
									state.hakoCachedCDT[4+i] = temp2[i];
								}
							}
							wcscpy(wideName, name);
							free(name);
						}
					}
					StripFunkyASCII(wideName);
					if (!WideCharToMultiByte(932, 0, wideName, -1, state.hakoCachedCharName, sizeof(state.hakoCachedCharName),0, 0)) {
						size = 0;
					}
				}
			}
			if (size != 255) {
				state.hakoCachedCDT[0] = 0;
				state.hakoCachedCharName[0] = 0;
			}
			fclose(in);
		}
	}
	return 1;
}

int HakoHandler::GetCharInfo(CharInfo &tempChar, int *stack, wchar_t *ppFile, char *waveName) {
	if (!wcsicmp(ppFile, L"hk003.pp")) {
		char *c = strchr(waveName, '_');
		if (c && '0'<=c[1] && c[1]<='9' && '0'<=c[2] && c[2]<='9') {
			//int id = (c[1]-'0')*10 + (c[2]-'0');
			int i;
			HakoData data;
			//if (id > 5) id = 0;
			tempChar.color = config.defaultColors[0];
			tempChar.outline = config.defaultOutline[0];
			tempChar.shadow = config.defaultShadow[0];
			tempChar.game = Dunno;
			tempChar.name = 0;
			if (GetHakoData(data)) {
				char *JISName = 0;
				for (i=0; i<4; i++) {
					if (i<3) {
						if (data.girls[i].loaded) {
							JISName = data.girls[i].name;
							if (data.girls[i].color < sizeof(HakoColors)/sizeof(HakoColors[0])) {
								tempChar.color = HakoColors[data.girls[i].color];
							}
							break;
						}
					}
					else if (state.hakoCachedCharName[0]) {
						JISName = state.hakoCachedCharName;
						unsigned int test = *(int*)&state.hakoCachedCDT[0x3C];
						if (test < sizeof(HakoColors)/sizeof(HakoColors[0])) {
							tempChar.color = HakoColors[test];
						}
					}
				}
				if (JISName) {
					if (!(tempChar.color&0xFFFFFF) && !(tempChar.outline&0xFFFFFF)) {
						tempChar.outline |= 0xFFFFFF;
					}
					tempChar.game = HAKO;
					wchar_t temp[1000];
					if (MultiByteToWideChar(932, 0, JISName, -1, temp, sizeof(temp)/sizeof(wchar_t))) {
						if (config.hakoRomanizeNames) {
							wchar_t *name = JISNameToRomanji(temp);
							if (name) {
								wcscpy(temp, name);
								free(name);
							}
						}
						StripFunkyASCII(temp);
						if (WideCharToMultiByte(CP_UTF8, 0, temp, -1, cachedName, sizeof(cachedName), 0, 0)) {
							tempChar.name = cachedName;
							for (i=0; i<config.numExtraChars; i++) {
								if (!config.extraChars[i].file && !stricmp(config.extraChars[i].name, cachedName)) {
									tempChar.color = config.extraChars[i].color;
									tempChar.outline = config.extraChars[i].outline;
									tempChar.shadow = config.extraChars[i].shadow;
									tempChar.name = cachedName;
									break;
								}
							}
						}
					}
				}
			}

			//wchar_t temp[100];
			/*swprintf(temp, L"%s\\%c%c", ppFile, c[1], c[2]);
			for (int i=0; i<config.numExtraChars; i++) {
				if (config.extraChars[i].file && !wcsicmp(config.extraChars[i].file, temp)) {
					tempChar.color = config.extraChars[i].color;
					tempChar.outline = config.extraChars[i].outline;
					tempChar.shadow = config.extraChars[i].shadow;
					tempChar.game = HAKO;
					tempChar.name = config.extraChars[i].name;
					break;
				}
			}
			/*
			//int *w = (int*)stack[0x45];
			//w=w;
			int vals[2] = {0x6CE734, 0x76E46050};
			// Code to figure out where character info is found.
			char *target = (char*)((0+(int*)0x6CE738)[0] + 0x050);
			int targetLen = strlen(target);
			//int *w2 = (int*)stack[0x67];
			//w2=w2;
			//int *w3 = (int*)w2[76388/4];
			//w3=w3;
			//char *c2 = (char*)w3[0x68/4]+0x50;
			for (int k=0; 0 && k<1000; k++) {
				int v = stack[k];
				char *test = (char*)&v;
				if (!strcmp(test, target)) {
					test=test;
				}
				if (!IsBadReadPtr((void*) test[k],4) && !(test[k]&3)) {
					for (int n=0; n<1000; n++) {
						if (!IsBadReadPtr(((int*) test[k])+n,3)) {
							int *m = ((int*) test[k])+n;
							if (!strcmp((char*)m, target)) {
								test=test;
							}
							if (!IsBadReadPtr((int*) m,4)) {
								for (int n2=0; n2<1000; n2++) {
									if (!IsBadReadPtr((int*) (*m)+n,4)) {
										int *m2 = (int*) (*m)+n;
										if (!strcmp((char*)m2, target)) {
											test=test;
										}
									}
								}
							}//*/
			/*			}
					}
				}
			}//*/
			/*
			if (1) {
				static int test[1000];
				int check = -1;
				int check2 = -1;
				//memset(test, 0, sizeof(test));
				for (int q=0; q<700; q++) {
					if (test2[q] != 1)
						test[q]--;
					if (!test[q] && check<0) check = q;
					if (!test[q]) check2 = q;
				}
				check = check;
			}
			//*/
			return 1;
		}
	}
	return 0;
}

void HakoHandler::DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite) {
	/*int *res = (int*) 0x6CDAB8;
	if (!IsBadWritePtr(res, 8)) {
		res[0] = state.d3dRect.right;
		res[1] = state.d3dRect.bottom;
		((float*)res)[15] = state.d3dRect.bottom/768.0f;
		((int*)0x6C50DC)[0] = state.d3dRect.right;
		((int*)0x6C50DC)[1] = state.d3dRect.bottom;
		((int*)0x703444)[0] = state.d3dRect.right;
		((int*)0x703444)[1] = state.d3dRect.bottom;
		((int*)0x703678)[0] = state.d3dRect.right;
		((int*)0x703678)[1] = state.d3dRect.bottom;
		((int*)0x703690)[0] = state.d3dRect.right;
		((int*)0x703690)[1] = state.d3dRect.bottom;
	}//*/
	if (state.hakoEditor || config.hacksEnabled || config.hakoImmortality) {
		HakoData data;
		if (GetHakoData(data)) {
			if (config.hakoImmortality && config.hacksEnabled) {
				for (int i=0; i<3; i++) {
					if (config.hakoImmortality & 1) {
						if (data.girls[i].age && data.girls[i].lifespan) {
							if (data.girls[i].age[0] + 1 >= data.girls[i].lifespan[0]) {
								if (data.day && data.day[0] >= 16)
									data.girls[i].lifespan[0] = data.girls[i].age[0]+2;
							}
						}
					}
					if ((config.hakoImmortality & 2) && data.girls[i].mood && data.girls[i].health) {
						data.girls[i].mood[0] = 0;
						data.girls[i].health[0] = 0;
					}
				}
			}
			if (!data.ingame) {
				state.hakoEditor = 0;
				state.hakoActiveCDT[0] = 0;
				state.hakoCachedCDT[0] = 0;
				state.hakoCachedCharName[0] = 0;
			}
			else if (state.hakoEditor && !state.hakoActiveCDT[0] && !data.activeChar) {
				state.hakoEditor = 0;
			}
			if (d3dSprite) {
				d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
			}
			//*
			if (config.hacksEnabled == 2) {
				for (int i=0; i<3; i++) {
					if (data.girls[i].exists) {
						int bottom = state.d3dRect.bottom - (3-i) * (state.fontHeight*3+2);
						wchar_t text[1000];
						if (MultiByteToWideChar(932, 0, data.girls[i].name, -1, text, sizeof(text)/sizeof(wchar_t))) {
							char utf8Name[4000];
							StripFunkyASCII(text);
							WideCharToMultiByte(CP_UTF8, 0, text, -1, utf8Name, sizeof(utf8Name),0,0);
							wchar_t *p = wcsrchr(text, 0);

							unsigned long color = config.defaultColors[0];
							unsigned long outline = config.defaultOutline[0];
							unsigned long shadow = config.defaultShadow[0];
							if (data.girls[i].color < sizeof(HakoColors)/sizeof(HakoColors[0])) {
								color = HakoColors[data.girls[i].color];
							}
							if (!(color&0xFFFFFF) && !(outline&0xFFFFFF)) {
								outline |= 0xFFFFFF;
							}
							for (int j=0; j<config.numExtraChars; j++) {
								if (!config.extraChars[j].file && !stricmp(config.extraChars[j].name, utf8Name)) {
									color = config.extraChars[j].color;
									outline = config.extraChars[j].outline;
									shadow = config.extraChars[j].shadow;
									break;
								}
							}
							swprintf(p, L"'s Age: %i / %i", data.girls[i].age[0], data.girls[i].lifespan[0]);
							DisplayTextAtPos(text, state.d3dRect.right-10, bottom, color, outline, shadow, d3dSprite, DT_TOP | DT_NOCLIP | DT_RIGHT);
							swprintf(text, L"Sick / Unhappy: %i / %i", data.girls[i].health[0], data.girls[i].mood[0]);
							DisplayTextAtPos(text, state.d3dRect.right-10, bottom + state.fontHeight, color, outline, shadow, d3dSprite, DT_TOP | DT_NOCLIP | DT_RIGHT);
							//wsprintf(text, L"%s: %i", 
						}
					}
				}
			}
			/*
			data=data;
			int *test = (int*)0x6CEF70;
			float *v = (float*)(test[0] + 1676);
			float q = 19.0f;
			q=q;
			//*(short*)0x30BD946 = 16955;
			//*(short*)0x30BD946 = 55;
			/*
			if (!config.displayClock) return;
			if (config.displayClock) {
				HakoData *data = GetHakoData();
				if (data) {
					wchar_t time[100];
					int hour = data->time/3600000;
					int min = data->time/60000 - hour*60;
					if (config.displayClock == 1)
						swprintf(time, L"Day %i, %02i:%02i", data->day, hour, min);
					else {
						char c = 'a';
						if (hour >= 12) {
							c = 'p';
							hour -= 12;
						}
						if (!hour) hour = 12;
						swprintf(time, L"Day %i, %i:%02i %cm", data->day, hour, min, c);
					}
					DisplayTextAtPos(time, state.d3dRect.right-8, state.d3dRect.bottom-6, D3DCOLOR_RGBA(255,255,255,255), config.defaultOutline[0], config.defaultShadow[0], d3dSprite, DT_RIGHT | DT_BOTTOM | DT_NOCLIP);
				}
			}
			//*/
			if (state.hakoEditor) {
				char *info = state.hakoCachedCDT;
				if (!info[0]) {
					if (data.activeChar)
						info = data.activeChar;
					else
						info[4] = 0;
				}
				if (info[4]) {
					int bottom = state.d3dRect.bottom;
					if (state.gameRect.bottom < bottom) {
						bottom = state.gameRect.bottom;
					}
					bottom = (int) (bottom * 0.87);
					RECT r;
					int pos = bottom -= 16 * sizeof(HakoGirlInfo)/sizeof(HakoGirlInfo[0]);
					r.left = r.right = 5;
					for (int i=0; i<sizeof(HakoGirlInfo)/sizeof(HakoGirlInfo[0]); i++) {
						int offset = HakoGirlInfo[i].offset;
						if (!state.hakoCachedCDT[0]) {
							if (offset == 0xF2)
								offset = 0x168;
							if (offset == 0xF6)
								offset = 0x16C;
							if (offset == 0xB6 || offset == 0xBA)
								offset += 2;
						}
						char text[1000];
						sprintf(text, "(%c) %s: %i", HakoGirlInfo[i].shortcut, HakoGirlInfo[i].name, *(int*)(info + offset) & masks[HakoGirlInfo[i].size]);
						r.top = r.bottom = pos;
						unsigned long color = D3DCOLOR_RGBA(255,255,255,255);
						if (i == selectedStat) {
							color = D3DCOLOR_RGBA(0,255,0,255);
						}
						DisplayMiniTextAtPos(text, &r, color, D3DCOLOR_RGBA(0,0,0,255), d3dSprite);
						pos += 16;
					}
					if (state.hakoCachedCDT[0]) {
						bottom -= (state.fontHeight + 5);
						DisplayTextAtPos(L"  Ctrl-S to save", 5, bottom, D3DCOLOR_RGBA(0,255,0,255), config.defaultOutline[0], config.defaultShadow[0], d3dSprite);
						bottom -= (state.fontHeight + 5);
						DisplayTextAtPos(L"  Ctrl-E to cancel edits", 5, bottom, D3DCOLOR_RGBA(0,255,0,255), config.defaultOutline[0], config.defaultShadow[0], d3dSprite);
					}
					bottom -= (state.fontHeight + 5);
					wchar_t temp[2000] = L"Editing ";
					wchar_t *s = wcschr(temp, 0);
					if (MultiByteToWideChar(932, 0, info+4, -1, s, 900)) {
						if (config.hakoRomanizeNames) {
							wchar_t *name = JISNameToRomanji(s);
							if (name) {
								wcscpy(temp, s);
								free(name);
							}
						}
						if (state.hakoCachedCDT[0]) {
							wchar_t *p = wcsrchr(state.hakoActiveCDT, '\\');
							if (p) p++;
							else p = state.hakoActiveCDT;
							swprintf(wcschr(s, 0), L" (%s)", p);
						}
						else {
							swprintf(wcschr(s, 0), L" (Girl %i)", data.activeCharIndex);
						}
						StripFunkyASCII(temp);
						DisplayTextAtPos(temp, 5, bottom, D3DCOLOR_RGBA(255,255,255,255), config.defaultOutline[0], config.defaultShadow[0], d3dSprite);
					}
				}
			}
			if (d3dSprite) {
				d3dSprite->End();
			}
		}
	}
}

void DisplayHakoCheatHelp() {
	AddText(20000, "     Ctrl-M gives 10000 yen", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-P gives 10 'Hyper Modes'", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-I toggles immortality mode", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-[Shift]-F increase your stock of all food types by 3 [99]", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Shift-F maximizes store stock of all food types", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-7/0 max blue/red bar during skinship", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-8/9 max blue/red bar during skinship and set the other to 95%", D3DCOLOR_RGBA(0,255,0,255));
}

void HakoHandler::DisplayHelp() {
	AddText(20000, "     Ctrl-X toggles cheat mode", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     Ctrl-E toggles editor", D3DCOLOR_RGBA(0,255,255,255));
	if (config.hacksEnabled) {
		DisplayHakoCheatHelp();
	}
}

int HakoHandler::HandleKeyPress(int c, int control, int shift) {
	static int dir;
	if (state.hakoEditor) {
		if (c == VK_DOWN) {
			selectedStat = (selectedStat+1) % (sizeof(HakoGirlInfo)/sizeof(HakoGirlInfo[0]));
			return 1;
		}
		else if (c == VK_UP) {
			if (!selectedStat) {
				selectedStat = sizeof(HakoGirlInfo)/sizeof(HakoGirlInfo[0])-1;
			}
			else selectedStat --;
			return 1;
		}
		else if (c == VK_LEFT || c==VK_RIGHT) {
			shift = (c == VK_LEFT);
			control = 0;
			c = HakoGirlInfo[selectedStat].shortcut;
		}
		char *info = state.hakoCachedCDT;
		HakoData data;
		data.activeChar = 0;
		if (!info[0]) {
			if (GetHakoData(data) && data.activeChar) {
				info = data.activeChar;
			}
			else info[4] = 0;
		}
		if (!info[4]) {
			state.hakoEditor = 0;
		}
		else if (!control) {
			int changed = 0;
			for (int i=0; i<sizeof(HakoGirlInfo)/sizeof(HakoGirlInfo[0]); i++) {
				if (HakoGirlInfo[i].shortcut == c) {
					int offset = HakoGirlInfo[i].offset;
					if (!state.hakoCachedCDT[0]) {
						if (offset == 0xF2)
							offset = 0x168;
						if (offset == 0xF6)
							offset = 0x16C;
						if (offset == 0xB6 || offset == 0xBA)
							offset += 2;
					}
					if (offset == 0xA0 || offset == 0x9C) {
						if (data.activeChar && (!data.day || data.day[0] <= 15))
							return 1;
					}
					int mask = masks[HakoGirlInfo[i].size];
					int *addr = (int*)(info + offset);
					int val = addr[0] & mask;
					//int *val = (int*)(editorData + 0x40);
					if (!shift) {
						val++;
						if (val >= HakoGirlInfo[i].maxVal)
							val = HakoGirlInfo[i].minVal;
					}
					else {
						val --;
						if (val < HakoGirlInfo[i].minVal)
							val = HakoGirlInfo[i].maxVal-1;
					}
					addr[0] = (addr[0] & ~mask) + (val & mask);
					changed = 1;
					if (!state.hakoCachedCDT[0]) {
						if (info[0x168] == 2)
							info[0x16C] = 0;
					}
					else {
						if (info[0xF2] == 2)
							info[0xF6] = 0;
					}
					break;
				}
			}
			if (changed && state.hakoCachedCDT[0]) {
				dir = -dir;
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, dir, 0);
				return 1;
			}
		}
		else if (c == 'S') {
			if (state.hakoCachedCDT[0]) {
				FILE *out = _wfopen(state.hakoActiveCDT, L"wb");
				if (!out) {
					AddText(15000, L"Save Failed", D3DCOLOR_RGBA(255,0,0,255));
				}
				else {
					for (int i=0; i<255; i++) {
						fputc(state.hakoCachedCDT[i] ^ 0xFF, out);
					}
					wchar_t text[1000];
					wchar_t *p = wcsrchr(state.hakoActiveCDT, '\\');
					if (p) p++;
					else p = state.hakoActiveCDT;
					swprintf(text, L"Saved to %s", p);
					AddText(10000, text, -1);
					fclose(out);
				}
			}
		}
		/*if (changed) {
			FILE *out = fopen("data\\memory\\editor.cdt", "wb");
			FILE *out2 = fopen("data\\memory\\editor2.cdt", "wb");
			if (out) {
				fwrite(editorData, 1, editorDataSize, out);
				fclose(out);
			}
			if (out2) {
				fwrite(editorData, 1, editorDataSize, out2);
				fclose(out2);
			}
			mouse_event(MOUSEEVENTF_WHEEL, 0, 0, WHEEL_DELTA, 0);
			return 1;
		}//*/
	}
	if (c == 'F' && config.hacksEnabled && (shift || control)) {
		HakoData data;
		if (GetHakoData(data) && data.food) {
			int delta = 3;
			if (shift) delta = 99;
			char *strings[2] = {"Store", "Your"};
			char temp[1000];
			sprintf(temp, "%s stock of all foods increased by %i", strings[control], delta);
			AddText(3000, temp, D3DCOLOR_RGBA(255,255,255,255));
			for (int i=0; i<20; i++) {
				int *val = &data.food[i].stocks[!control];
				val[0] += delta;
				if (val[0] > 99) val[0] = 99;
			}
		}
	}
	if (!control) return 0;
	if (c == 'E') {
		HakoData data;
		selectedStat = 0;
		if (state.hakoEditor) {
			state.hakoCachedCDT[0] = 0;
			state.hakoCachedCharName[0] = 0;
			state.hakoEditor = 0;
			// AddText(3000, "Editor Disabled", D3DCOLOR_RGBA(255,255,255,255));
			if (dir != WHEEL_DELTA) {
				// Return to original character, if never changed character manually.
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, dir, 0);
			}
		}
		else if (!state.hakoCachedCharName[0] && (!GetHakoData(data) || !data.activeChar)) {
			AddText(3000, "No character to edit", D3DCOLOR_RGBA(255,255,255,255));
		}
		else {
			dir = WHEEL_DELTA;
			state.hakoEditor = 1;
		}
		return 1;
	}
	else if (c == 'X') {
		config.hacksEnabled = (config.hacksEnabled+1)%3;
		if (!config.hacksEnabled) {
			AddText(3000, "Cheat Mode 0: Disabled", D3DCOLOR_RGBA(255,255,255,255));
			if (config.hakoImmortality) {
				AddText(3000, "  Girls are no longer immortal", D3DCOLOR_RGBA(255,255,255,255));
			}
		}
		else if (config.hacksEnabled == 1) {
			CleanupAllLines();
			AddText(3000, "Cheat Mode 1: Cheats enabled, no status display", D3DCOLOR_RGBA(255,255,255,255));
			if (config.hakoImmortality) {
				char temp[100];
				sprintf(temp, "  Immortality mode %i restored", config.hakoImmortality);
				AddText(3000, temp, D3DCOLOR_RGBA(255,255,255,255));
			}
			DisplayHakoCheatHelp();
		}
		else if (config.hacksEnabled == 2) {
			AddText(3000, "Cheat Mode 2: Cheats and status display enabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		SaveConfig();
		return 1;
	}
	else if (c == 'I') {
		config.hakoImmortality = (config.hakoImmortality+1)%4;
		if (!config.hakoImmortality) {
			AddText(3000, "Immortality Disabled", D3DCOLOR_RGBA(255,255,255,255));
		}
		else if (config.hakoImmortality == 1) {
			AddText(3000, "Immortality Mode 1: Girls don't die of age, but need food", D3DCOLOR_RGBA(255,255,255,255));
		}
		else if (config.hakoImmortality == 2) {
			AddText(3000, "Immortality Mode 2: Girls die of age, but don't need food", D3DCOLOR_RGBA(255,255,255,255));
		}
		else {
			AddText(3000, "Immortality Mode 3: Girls neither die of age nor need food", D3DCOLOR_RGBA(255,255,255,255));
		}
		SaveConfig();
		return 1;
	}
	else if (config.hacksEnabled) {
		HakoData data;
		if (GetHakoData(data)) {
			if (data.friendBar) {
				if (c == '7') {
					*data.friendBar = 100.0f;
					return 1;
				}
				else if (c == '8') {
					*data.friendBar = 100.0f;
					*data.loveBar = 95.0f;
					return 1;
				}
				else if (c == '9') {
					*data.friendBar = 95.0f;
					*data.loveBar = 100.0f;
					return 1;
				}
				else if (c == '0') {
					*data.loveBar = 100.0f;
					return 1;
				}
			}
			if (c == 'M') {
				if (data.money[0] < 10000000) {
					char temp[1000];
					data.money[0] += 10000;
					sprintf(temp, "Added %i yen, you now have %i yen.", 10000, data.money[0]);
					AddText(3000, temp, D3DCOLOR_RGBA(0,255,0,255));
				}
			}
			else if (c == 'P') {
				if (data.limitBreaks[0] < 999) {
					int add = 10;
					if (data.limitBreaks[0]+add >= 1000) add = 999 - data.limitBreaks[0];
					data.limitBreaks[0] += add;
					if (add > 1) {
						char temp[1000];
						sprintf(temp, "Added %i 'Hyper Modes', you now have %i.", add, data.limitBreaks[0]);
						AddText(3000, temp, D3DCOLOR_RGBA(0,255,0,255));
					}
				}
			}
			else return 0;
			return 1;
		}
	}
	return 0;
}
