#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "system.hpp"

int main() {
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	sf::RenderWindow window;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	window.setFramerateLimit(1.f/FRAME_RATE);
	Game game(&window);
	game.levelIO.filename = cwd() + "/assets/levels/level3.json";
	game.start();
	game.run();
	game.stop();
}

