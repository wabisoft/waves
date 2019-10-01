#include "editor.hpp"


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
}

void continueMouseInput(Editor& editor, sf::Event::MouseMoveEvent event) {
}

void endMouseInput(Editor& editor, sf::Event::MouseButtonEvent event) {
}

