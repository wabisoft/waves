#include "graphics.hpp"

void initGraphics(EditorGraphics& graphics, sf::RenderWindow& window) {
	window.create(graphics.videoMode, "Waves Editor", graphics.style, graphics.settings);
	window.setFramerateLimit((float)graphics.framerate);
}

void destroyEditorGraphics(EditorGraphics&) {
}

void draw(EditorGraphics& graphics, Editor& editor, sf::RenderWindow& window) {
	float delta = graphics.clock.getElapsedTime().asSeconds();
	if(delta >= 1/graphics.framerate) {
		graphics.clock.restart();
		window.display();
	}
}

