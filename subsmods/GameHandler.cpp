#include "Shrink.h"
#include "GameHandler.h"
#include "AG3.h"
#include "Hako.h"
#include "AHM.h"

char screenShot = 0;

GameHandler *CreateGameHandler(GameID game) {
	if (game == AG3) {
		return new AG3Handler();
	}
	else if (game == HAKO) {
		return new HakoHandler();
	}
	else if (game == AHM) {
		return new AHMHandler();
	}
	// Always check for a null GameHandler anyways,
	// but prefer to play it safe, since I don't test
	// older games much/at all.
	return new GameHandler(game);
}

