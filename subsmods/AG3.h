#pragma once
#include "Config.h"
#include "GameHandler.h"

class AG3Handler : public GameHandler {
	// 1-based index of selected girl for modifying stats.
	// 0 means modify all girls.
	int selectedGirl;
	// 0-based stat index to modify.
	int selectedStat;
	int resMatch;
public:
	inline AG3Handler() : GameHandler(AG3) {
		selectedGirl = 0;
		selectedStat = 0;
		resMatch = 0;
	}
	void DisplayHelp();
	int HandleKeyPress(int c, int control, int shift);

	void DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite);
	int GetCharInfo(CharInfo &tempChar, int *stack, wchar_t *ppFile, char *waveName);
};




