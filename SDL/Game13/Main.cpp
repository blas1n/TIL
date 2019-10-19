#include "Game.h"

int main(int argc, char** argv) {
	Game game;

	if (game.Initialize())
		game.RunLoop();

	game.Shutdown();
	system("pause");
	return 0;
}
