#include <iostream>
#include <fstream>

#include "game.hpp"
#include "printing.hpp"
#include "graphics.hpp"
#include "serialize.hpp"


int main() {
	sf::RenderWindow window;
	sf::ContextSettings settings;
	// sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves", sf::Style::Default, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
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
			if(e.type == sf::Event::Resized) {
				auto view = window.getView();
				view.setCenter({e.size.width/2.f, e.size.height/2.f});
				view.setSize({(float)e.size.width, (float)e.size.height});
				window.setView(view);
			}
		}
		update(game);
		float drawDelta = drawClock.getElapsedTime().asSeconds();
		if (drawDelta >= FRAME_RATE) {
			drawStage(window, game.stage, true);
			drawInfoText(window, game.stage, drawDelta, game.updateDelta, 0);
			window.display();
			drawClock.restart();
		}
	}
	stop(game);
}
