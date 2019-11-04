
#include <SFML/Graphics.hpp>

#include "editor.hpp"
#include "system.hpp"


int main() {
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	sf::RenderWindow window;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	window.setFramerateLimit(1.f/FRAME_RATE);
	Editor e(&window);
	e.level.filename = cwd() + "\\assets\\levels\\level3.json";
	e.start();
	e.run();
	e.stop();
}
