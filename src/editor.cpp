#include <iostream>

#include "editor.hpp"
#include "graphics.hpp"


// void handleEvents(Editor& editor, sf::RenderWindow& window) {
void processEvent(Editor& editor, const sf::Event& event, const sf::RenderWindow& window) {
	// sf::Event event;
	//while(window.pollEvent(event)) {
		switch(event.type){
			// case sf::Event::Closed: window.close(); break;
	        case sf::Event::KeyPressed: keyEvent(editor, event);
			case sf::Event::MouseButtonPressed: startMouseInput(editor, screen2GamePos(window, {event.mouseButton.x, event.mouseButton.y})); break;
			case sf::Event::MouseMoved: continueMouseInput(editor, screen2GamePos(window, {event.mouseMove.x, event.mouseMove.y})); break;
			case sf::Event::MouseButtonReleased: endMouseInput(editor, screen2GamePos(window, {event.mouseButton.x, event.mouseButton.y})); break;
	        // case sf::Event::TouchBegan: startMouseInput(editor, event.mouseButton); break;
	        // case sf::Event::TouchMoved: continueMouseInput(editor, event.mouseMove); break;
	        // case sf::Event::TouchEnded: endMouseInput(editor, event.mouseButton); break;
			default: break;
		}
	// }
}

void keyEvent(Editor& editor, sf::Event event) {
}

void startMouseInput(Editor& editor, Vector2 position) {
	// Entity entity = selectEntityAtPosition({event.;
	Entity entity = findEntityAtPosition(editor.stage, position);
	std::cout << (int)entity.id << ": " << str(entity.type) << std::endl;
}

void continueMouseInput(Editor& editor, Vector2 position) {
}

void endMouseInput(Editor& editor, Vector2 position) {
}
