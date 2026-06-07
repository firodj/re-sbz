#pragma once
#include "Config.h"
#include "GameHandler.h"

class AHMHandler : public GameHandler {
public:
	inline AHMHandler() : GameHandler(AHM) {
	}

	void DisplayHelp();
	int HandleKeyPress(int c, int control, int shift);

	void DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite);
};


