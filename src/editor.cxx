#include "editor.hpp"
#include "graphics.hpp"

int main() {
	Editor editor;
	sf::RenderWindow window;
	EditorGraphics graphics;
	initGraphics(graphics, window);
	while(window.isOpen()) {
		handleEvents(editor, window);
		draw(graphics, editor, window);
	}
};
