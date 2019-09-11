#pragma once

#include "clock.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"


// TODO: Write an editor

struct Editor;
struct Button;

typedef sf::Rect<int> Recti;

struct Button {
	Recti rect = {0,0,0,0};
	std::string label = "";
	void (*action)(Editor& editor);
};


struct Editor {
	struct Mouse {
		sf::Vector2i startPosition;
		sf::Vector2i currentPosition;
		sf::Vector2i endPosition;
		sf::Mouse::Button button;
		bool pressed = false;
	};

	enum Component : uint8_t{
		NONE = 1 << 0,
		ROCK = 1 << 0,
		SHIP = 1 << 0,
		PLATFORM = 1 << 0,
		ROCK_SPAWN = 1 << 0,
		WIN_REGION = 1 << 0
	};

	enum Mode {
		DEFAULT,
		DRAW,
		TYPE
	};

	Stage stage;
	Mouse mouse;
	Component currentComponent = NONE;
	Mode mode = DEFAULT;
};

void initEditor(Editor& editor);
void handleEvents(Editor& editor, sf::Window& window);
void keyEvent(Editor&, sf::Event);
void startInput(Editor&, sf::Event);
void continueInput(Editor&, sf::Event);
void endInput(Editor&, sf::Event);

