#include <iostream>
#include <fstream>

#include "game.hpp"
#include "logging.hpp"
#include "printing.hpp"
#include "graphics.hpp"
#include "serialize.hpp"
#include "system.hpp"



int main() {
	logging::Logger logger = logging::Logger("main");

	logger.setLoggerLevel("EventManager", logging::INFO);
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves", sf::Style::Default, settings);
	window.setFramerateLimit(1.f/FRAME_RATE);
	Graphics g;
	EventManager eventManager;
	try {
		Clock drawClock;
		Game game;
		game.subscribe(eventManager, {Event::Closed, Event::MouseButtonPressed, Event::MouseButtonReleased, Event::MouseMoved, Event::KeyPressed});
		game.start();
		while(!game.end && window.isOpen()) {
			eventManager.dispatchEvents(window);
			game.update();
			float drawDelta = drawClock.getElapsedTime().asSeconds();
			if (drawDelta >= FRAME_RATE) {
				g.drawStage(window, game.stage, true);
				// drawInfoText(window, game.stage, drawDelta, game.updateDelta, 0);
				window.display();
				drawClock.restart();
			}
		}
	} catch (SerializeError & e) {
		std::cout << e.what << std::endl;
		return 1;
	}
}
