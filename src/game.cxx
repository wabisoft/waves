#include "game.hpp"

int main() {
	Game game;
	auto modes = game.graphics.videoMode.getFullscreenModes();
	start(game);
	run(game);
	stop(game);
}
