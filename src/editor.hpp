#pragma once

#include <vector>
#include <string>

#include "clock.hpp"
#include "json.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"


// TODO: Write an editor
struct ErrorPopupState {
	std::string popupId;
	std::string message;
	bool opened = false;
};


struct Editor {

	std::string filename;
	Stage stage;
	std::vector<ErrorPopupState> errorPopups;
};

void initEditor(Editor& editor);
// void handleEvents(Editor& editor, sf::RenderWindow& window);
void processEvent(Editor& editor, const sf::Event& event, const sf::RenderWindow& window);
void keyEvent(Editor&, sf::Event);
void startMouseInput(Editor&, Vector2);
void continueMouseInput(Editor&, Vector2);
void endMouseInput(Editor&, Vector2);

