#include "editor.hpp"


void handleEvents(Editor& editor, sf::Window& window) {
	sf::Event event;
	while(window.pollEvent(event)) {
		switch(event.type){
			case sf::Event::Closed: window.close(); break;
	        case sf::Event::KeyPressed: keyEvent(editor, event);
			case sf::Event::MouseButtonPressed: startInput(editor, event); break;
			case sf::Event::MouseMoved: continueInput(editor, event); break;
			case sf::Event::MouseButtonReleased: endInput(editor, event); break;
	        case sf::Event::TouchBegan: startInput(editor, event); break;
	        case sf::Event::TouchMoved: continueInput(editor, event); break;
	        case sf::Event::TouchEnded: endInput(editor, event); break;
			default: break;
		}
	}
}

void keyEvent(Editor& editor, sf::Event event) {
}

void startMouseInput(Editor& editor, sf::Event::MouseButtonEvent event) {
	if (editor.mouse.pressed || editor.mouse.button != event.button) { return; }
	editor.mouse.pressed = true;
	editor.mouse.startPosition = {event.x, event.y}
}

void continueMouseInput(Editor& editor, sf::Event::MouseMoveEvent event) {
	if (!editor.mouse.pressed) { return; }
	editor.mouse.currentPosition = {event.x, event.y};
}

void endMouseInput(Editor& editor, sf::Event::MouseButtonEvent event) {
	if (!editor.mouse.pressed || editor.mouse.button != event.button) { return; }
	editor.mouse.endPosition = {event.x, event.y}
	editor.mouse.pressed = false;
}

