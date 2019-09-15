#include <iostream>
#include <fstream>

#include "game.hpp"
#include "graphics.hpp"
#include "serialize.hpp"


int main() {
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves", sf::Style::Fullscreen, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
	 	std::cout << "Couldn't load font" << std::endl;
	}

	Clock drawClock;
	Game game;
	start(game);
	while(!game.end && window.isOpen()) {
		sf::Event e;
		while(window.pollEvent(e)) {
			processEvent(game, e, window);
		}
		update(game);
		float drawDelta = drawClock.getElapsedTime().asSeconds();
		if (drawDelta >= FRAME_RATE) {
			drawStage(window, game.stage, true);
			window.display();
			drawClock.restart();
		}
	}
	stop(game);
}
