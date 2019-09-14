#pragma once
#include <cstdint>

#include "clock.hpp"
#include "graphics.hpp"
#include "stage.hpp"
#include "prelude.hpp"



struct Game {
	Stage stage;
	Clock updateClock;
	float timeScale = 1.f;
	int loopsPerUpdate = 0;
	bool end = false; // stop the game
};


void start(Game& game);
void update(Game& game);
void processEvent(Game& game, const sf::Event&, const sf::RenderTarget&);
void keyEvent(Game& game, const sf::Event& event);
void stop(Game& game);
