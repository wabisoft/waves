#include "editor.hpp"


void initEditor(Editor& editor) {
}

void handleEvents(Editor& editor, sf::Window& window) {
	sf::Event event;
	while(window.pollEvent(event)) {
		switch(event.type){
			case sf::Event::Closed: window.close(); break;
	        case sf::Event::KeyPressed: keyEvent(editor, event);
			case sf::Event::MouseButtonPressed: startMouseInput(editor, event.mouseButton); break;
			case sf::Event::MouseMoved: continueMouseInput(editor, event.mouseMove); break;
			case sf::Event::MouseButtonReleased: endMouseInput(editor, event.mouseButton); break;
	        case sf::Event::TouchBegan: startMouseInput(editor, event.mouseButton); break;
	        case sf::Event::TouchMoved: continueMouseInput(editor, event.mouseMove); break;
	        case sf::Event::TouchEnded: endMouseInput(editor, event.mouseButton); break;
			default: break;
		}
	}
}

void keyEvent(Editor& editor, sf::Event event) {
}

void startMouseInput(Editor& editor, sf::Event::MouseButtonEvent event) {
	if (editor.mouse.pressed || editor.mouse.button != event.button) { return; }
	editor.mouse.pressed = true;
	editor.mouse.startPosition = {event.x, event.y};
}

void continueMouseInput(Editor& editor, sf::Event::MouseMoveEvent event) {
	if (!editor.mouse.pressed) { return; }
	editor.mouse.currentPosition = {event.x, event.y};
}

void endMouseInput(Editor& editor, sf::Event::MouseButtonEvent event) {
	if (!editor.mouse.pressed || editor.mouse.button != event.button) { return; }
	editor.mouse.endPosition = {event.x, event.y};
	editor.mouse.pressed = false;
}

void placeRock(Editor& editor, Action& action) {

}

void placeShip(Editor& editor, Action& action) {

}

void placePlatform(Editor& editor, Action& action) {

}

void placeRockSpawn(Editor& editor, Action& action) {

}

void placeWinRegion(Editor& editor, Action& action) {

}

