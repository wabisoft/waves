#include "game.hpp"

#include <iostream>
#include <fstream>
#include "serialize.hpp"

int main() {
	Game game;
	start(game);
	run(game);
	stop(game);
}
