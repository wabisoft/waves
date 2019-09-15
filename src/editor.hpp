#pragma once

#include <vector>

#include "clock.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"


// TODO: Write an editor

struct Editor {

	enum Component {
		NONE = -1,
		ROCK = 0,
		SHIP = 1,
		PLATFORM,
		ROCK_SPAWN,
		WIN_REGION,
		SIZE
	};

	enum Mode {
		DEFAULT,
	};

	Stage stage;
	Component currentComponent = SHIP;
	Mode mode = DEFAULT;
};

void initEditor(Editor& editor);
void handleEvents(Editor& editor, sf::Window& window);
void keyEvent(Editor&, sf::Event);
void startMouseInput(Editor&, sf::Event::MouseButtonEvent);
void continueMouseInput(Editor&, sf::Event::MouseMoveEvent);
void endMouseInput(Editor&, sf::Event::MouseButtonEvent);

