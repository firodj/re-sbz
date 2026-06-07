#pragma once
#include "Config.h"
#include "GameHandler.h"

class HakoHandler : public GameHandler {
public:
	char selectedStat;
	char cachedName[128];
	inline HakoHandler() : GameHandler(AG3) {
	}
	void DisplayHelp();
	int HandleKeyPress(int c, int control, int shift);

	void DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite);
	int GetCharInfo(CharInfo &tempChar, int *stack, wchar_t *ppFile, char *waveName);
};




