#pragma once

#include <vector>

#include "clock.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"


// TODO: Write an editor

struct Editor;
struct Button;
struct Action;
struct Mouse;

typedef sf::Rect<float> Rectf;

struct Button {
	sf::Vector2f position;
	float width = 0;
	float height= 0;
	std::string label = "";
	void (*action)(Editor& editor, Action& action);
};

struct ToggleButton : Button {
	bool toggled = false;
};

struct ToggleButtonCluster {
	sf::Vector2f position;
	std::vector<ToggleButton> buttons;
};

struct Mouse {
	sf::Vector2i startPosition;
	sf::Vector2i currentPosition;
	sf::Vector2i endPosition;
	sf::Mouse::Button button;
	bool pressed = false;
};

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
		DRAW,
		TYPE
	};

	Stage stage;
	Mouse mouse;
	Component currentComponent = SHIP;
	Mode mode = DEFAULT;
};

struct Action {
	Mouse mouse; // Do I need such a struct? maybe I can get by with just a position (sf::Vector2i)
};

void initEditor(Editor& editor);
void handleEvents(Editor& editor, sf::Window& window);
void keyEvent(Editor&, sf::Event);
void startMouseInput(Editor&, sf::Event::MouseButtonEvent);
void continueMouseInput(Editor&, sf::Event::MouseMoveEvent);
void endMouseInput(Editor&, sf::Event::MouseButtonEvent);

/***********
 * Actions *
 ***********/
void placeRock(Editor& editor, Action& action);
void placeShip(Editor& editor, Action& action);
void placePlatform(Editor& editor, Action& action);
void placeRockSpawn(Editor& editor, Action& action);
void placeWinRegion(Editor& editor, Action& action);
