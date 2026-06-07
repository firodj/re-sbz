#include "Shrink.h"
#include "AHM.h"
#include "D3DTextOverlay.h"
#include <time.h>

inline static char** GetAHMBaseGirlMem() {
	return (char**)0x789214;
}

#ifdef GOAT

struct AHMSexInfo {
	float *guyOrgasm, *girlOrgasm;
	/*int *girlArousal, *girlLewdness;
	float *scrollPos;
	int *cumGauge;//*/
	unsigned short *orgasmCounter;
};

static int GetAHMSexInfo(AHMSexInfo &info) {
	/*info.guyOrgasm = (float*)0x6BC750;
	info.girlOrgasm = (float*)0x6BC74C;
	info.girlArousal = (int*)0x6BC738;
	info.girlLewdness = (int*)0x6BC734;

	info.scrollPos = (float*)0x699078;
	info.cumGauge = (int*)0x6BC91C;
	//*/
	char *base = GetAHMBaseMem();
	if (!base) return 0;
	info.guyOrgasm = 0;
	info.orgasmCounter = 0;
	info.girlOrgasm = (float*)(base + 0x00614E4);
	float ***w = (float***) 0x789458;
	if (0 && w[0]) {
		float **w2 = w[0] + 15;
		if (w2[0]) {
			float *w3 = w2[0];
			if (w3)
				info.guyOrgasm = w3+27;
		}
	}
	if (*(float**)(base+0x35D4)) {
		info.guyOrgasm = (*(float**)(base+0x35D4))+10;
	}
	if (!info.guyOrgasm) return 0;

	const int offsets[3] = {
		0x30b9,
		0x6991,
		0xa039
	};

	for (int i=0; i<3; i++) {
		if (*(int*)(base + offsets[i] - 1 - 0x80)) {
			info.orgasmCounter = (unsigned short*)(base + offsets[i] + 0xB7);
		}
	}
	if (!info.orgasmCounter) return 0;
	return 1;

	/*info.orgasmCounter = (unsigned short*)(base + 0x3170);
	info.guyOrgasm = (float*)(base + 0x1E6A4);
	info.girlOrgasm = (float*)(base + 0x1E6AC);
	// 0x00614DC
	info.guyOrgasm = (float*)(base + 0x00614DC);
	info.girlOrgasm = (float*)(base + 0x00614E4);

	return info.guyOrgasm != 0;//*/
	//int *test = (int*)0x793860;
	//return *test != 0;
}

#endif


static void DisplayAHMCheatHelp() {
	AddText(20000, "     Ctrl-[Shift]-I increases your stock of all item types by 1 [20]", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-M toggles multiple item usage per girl cheat", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     F5/F6/F7 set and lock breast size", D3DCOLOR_RGBA(0,255,0,255));
	/*AddText(20000, "     Ctrl-[Shift]-G selects a girl", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Ctrl-[Shift]-S selects a stat", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     +/- adjust selected stat", D3DCOLOR_RGBA(0,255,0,255));
	AddText(20000, "     Shift/Ctrl +/- adjust girl/guy orgasm", D3DCOLOR_RGBA(0,255,0,255));
	//*/
}

void AHMHandler::DisplayHelp() {
	AddText(20000, "     Ctrl-X toggles cheats", D3DCOLOR_RGBA(0,255,255,255));
	// AddText(20000, "     Ctrl-V toggles stat view", D3DCOLOR_RGBA(0,255,255,255));
	/*
	AddText(20000, "     Ctrl-T toggles clock", D3DCOLOR_RGBA(0,255,255,255));
	AddText(20000, "     F12 to take screenshot w/o stats", D3DCOLOR_RGBA(0,255,255,255));
	//*/
	if (config.hacksEnabled) {
		DisplayAHMCheatHelp();
	}
}

static int SetBreastSize(int girl, unsigned char size) {
	char **base = GetAHMBaseGirlMem();
	if (!base[girl] || size > 3 || !size) return 0;
	base[girl][0x30b9] = size-1;
	return 1;
}

int AHMHandler::HandleKeyPress(int c, int control, int shift) {
	if(c == VK_F12) {
		screenShot = 1;
		keybd_event(VK_F11, 0x57, 0, 0);
	}
	if (config.hacksEnabled) {
		if (c >= VK_F5 && c <= VK_F7) {
			int girl = c - VK_F5;
			char * names[] = {"Nanoha", "Noa", "Yakumo"};
			char defaults[] = {2,1,3};
			unsigned char *val = &config.ahmBreastSizes[girl];
			char * size[] = {"small", "medium", "huge"};
			char temp[1000];
			if (val[0] >= 3) {
				sprintf(temp, "%s: Lock breast size disabled, set to default size", names[girl]);
				val[0] = 0;
				SetBreastSize(girl, defaults[girl]);
			}
			else {
				sprintf(temp, "%s: Breast size set and locked to \"%s\"", names[girl], size[val[0]]);
				val[0]++;
			}
			DWORD color = D3DCOLOR_RGBA(0,255,0,255);
			for (int i=0; i<config.numExtraChars; i++) {
				if (config.extraChars[i].name && !stricmp(config.extraChars[i].name, names[girl])) {
					color = config.extraChars[i].color;
					break;
				}
			}
			AddText(3000, temp, color);
			SaveConfig();
		}
	}
	if (!control) return 0;
	if (config.hacksEnabled) {
		if (c == 'I') {
			unsigned int add = 1;
			if (shift) add = 20;
			unsigned char *counts = (unsigned char*)0x7801b5;
			for (int i=0; i<40; i++) {
				unsigned int n = counts[i] + add;
				if (n > 99) n = 99;
				counts[i] = (unsigned char)n;
			}
			char temp[1000];
			sprintf(temp, "Added %i of each item type.", add);
			AddText(3000, temp, D3DCOLOR_RGBA(0,255,0,255));
		}
		else if (c == 'M') {
			config.ahmNoLimit = !config.ahmNoLimit;
			if (!config.ahmNoLimit) {
				AddText(3000, "Multiple items per girl disabled", D3DCOLOR_RGBA(0,255,0,255));
			}
			else {
				AddText(3000, "Multiple items per girl enabled", D3DCOLOR_RGBA(0,255,0,255));
			}
			SaveConfig();
		}
	}
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
			DisplayAHMCheatHelp();
		}
		SaveConfig();
		return 1;
	}
	/*
	if (c == 'W') {
		char* w = GetAHMBaseMem();
		char name[1000], name2[1000];
		for (int i=0; i<1000; i++) {
			sprintf(name, "debug-%i.bin", i);
			FILE *in = fopen(name, "rb");
			if (!in) break;
			fclose(in);
		}
		FILE *out = fopen(name, "wb");
		fwrite(&w, 4, 1, out);
		fwrite(w, 1, 0xC000, out);
		fclose(out);
		sprintf(name2, "Wrote debug info to %s", name);
		AddText(3000, name2, D3DCOLOR_RGBA(255,255,255,255));
	}
	/*
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
	}//*/
	return 0;
}


void AHMHandler::DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite) {
	/*if (!resMatch) {
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
	}//*/
	//*(char*)(0x7801ED) = 0;
	if (config.hacksEnabled) {
		if (config.ahmNoLimit) {
			*(int*) 0x007801ed |= 0x010101;
		}
		for (int i=0; i<3; i++) {
			if (config.ahmBreastSizes[i]) {
				SetBreastSize(i, config.ahmBreastSizes[i]);
			}
		}
	}
	/*if (*(char**)0x789214) {
		*((*(char**)0x789214)+0x30b9) = 0;
		*((*(char**)0x789214)+0x6991) = 0;
		*((*(char**)0x789214)+0xa039) = 0;
	}
	//*((*(char**)0x789214)+0xA269) = 2;
	/*for (int skip = 1; skip<10000; skip++) {
		if (*((*(char**)0x789214)+0x30b9+skip) == 2 &&
			*((*(char**)0x789214)+0x30b9+2*skip) == 2) {
				skip = skip;
		}
	//*/
	//*((char*)0x98C1CA8) = 2;
	if(screenShot > 0) {
		if (--screenShot == 0) keybd_event(VK_F11, 0x57, KEYEVENTF_KEYUP, 0);
		return;
	}
	// if (!config.hacksEnabled  && !config.viewEnabled) return;
	/*
	if (d3dSprite) {
		d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	if (config.viewEnabled && d3dMiniFont) {
		char output[1000];
		int bottom = state.d3dRect.bottom;
		int pos = 20;
		bottom -= bottom/24;
		RECT rect = {
			0, bottom - 143, pos, bottom
		};
		if (config.viewEnabled) {
			AHMSexInfo info;
			rect.right = pos + 60;

			if (GetAHMSexInfo(info)) {
				rect.top -= 17;
				rect.right = 150;
				DisplayMiniTextAtPos("Guy Orgasm:\nGirl Orgasm:\n\nOrgasm Counter:\n?:\n?:\n?:\n?:\n?:", &rect, D3DCOLOR_RGBA(0,255,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
				rect.right += 35;
				sprintf(output, "%3.0f\n%3.0f\n\n%i\n%i\n%i\n%i\n%i\n%i", info.guyOrgasm[0]/2, info.girlOrgasm[0]/2, info.orgasmCounter[0], info.orgasmCounter[-1], info.orgasmCounter[-2], info.orgasmCounter[-3], info.orgasmCounter[-4], info.orgasmCounter[-5]);
				DisplayMiniTextAtPos(output, &rect, D3DCOLOR_RGBA(255,255,255,255), D3DCOLOR_RGBA(0,0,0,255), d3dSprite, DT_RIGHT | DT_TOP | DT_NOCLIP);
			}
		}
	}
	if (d3dSprite) {
		d3dSprite->End();
	}//*/
}
