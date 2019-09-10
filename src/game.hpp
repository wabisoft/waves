#pragma once
#include <cstdint>

#include "clock.hpp"
#include "graphics.hpp"
#include "stage.hpp"
#include "prelude.hpp"

struct ScreenInputState {
	Vector2 position;
	bool pressed = false;
};

struct Game {
	Stage stage;
	Graphics graphics;
	Clock drawClock;
	Clock updateClock;
	ScreenInputState screenInputState;
	float timeScale = 1.f;
	bool end = false; // stop the game
};

void start(Game& game);
void run(Game& game);
void processEvents(Game& game);
void keyEvent(Game& game, sf::Event& event);
void stop(Game& game);
