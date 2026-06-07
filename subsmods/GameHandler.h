#pragma once

#include "pp.h"
#include <d3dx9.h>

struct CharInfo;

// Contains all game-specific code, except for saving game-specific
// state across session, which is handled by the config save/load code.
// Currently that just means hack and clock setting.
class GameHandler {
protected:
	inline GameHandler(GameID gameID) {
		game = gameID;
	}
public:
	// Not really needed, but doesn't hurt.
	GameID game;
	friend GameHandler *CreateGameHandler(GameID game);

	inline virtual ~GameHandler() {
	}

	inline virtual void DisplayHelp() {
	}

	inline virtual int HandleKeyPress(int c, int control, int shift) {
		return 0;
	}

	inline virtual void DisplayOverlay(ID3DXFont * d3dFont, ID3DXFont * d3dMiniFont, ID3DXSprite * d3dSprite) {
	}

	inline virtual int GetCharInfo(CharInfo &tempChar, int *stack, wchar_t *ppFile, char *waveName) {
		return 0;
	}
};

extern char screenShot;
