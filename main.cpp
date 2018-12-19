#include "Game.h"
#include "SDL2/SDL.h"

int main(int arc, char **argv) {
	Game game;
	bool success = game.Initialize();
	if (success) {
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}