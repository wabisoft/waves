#include <iostream>

#include "entity.hpp"
#include "editor.hpp"
#include "graphics.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"

void Editor::onClosed(sf::Window& window) {
	window.close();
}

void Editor::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton)	{
	// TODO: something state dependent
	Vector2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	selectedEntity = findEntityAtPosition(stage, position);
	mouseState.down = true;
	mouseState.downPosition = position;
}

void Editor::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	// TODO: Not sure yet...
	mouseState.down = false;
}

void Editor::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	// TODO: something state dependent
}


// void handleEvents(Editor& editor, sf::RenderWindow& window) {
void processEvent(Editor& editor, const sf::Event& event, const sf::RenderWindow& window) {
	switch(event.type){
		// case sf::Event::Closed: window.close(); break;
	    // case sf::Event::KeyPressed: keyEvent(editor, event);
		// case sf::Event::MouseButtonPressed: processStartInput(editor.stage, screen2GamePos(window, {event.mouseButton.x, event.mouseButton.y})); break;
		// case sf::Event::MouseMoved: processContinuingInput(editor.stage, screen2GamePos(window, {event.mouseMove.x, event.mouseMove.y})); break;
		// case sf::Event::MouseButtonReleased: processEndInput(editor.stage, screen2GamePos(window, {event.mouseButton.x, event.mouseButton.y})); break;
	    // case sf::Event::TouchBegan: startMouseInput(editor, event.mouseButton); break;
	    // case sf::Event::TouchMoved: continueMouseInput(editor, event.mouseMove); break;
	    // case sf::Event::TouchEnded: endMouseInput(editor, event.mouseButton); break;
		default: break;
	}
}

void keyEvent(Editor& editor, sf::Event event) {
}

void levelOpen(Editor& editor, std::string filename) {
	Stage s = {};
	SerializeError e;
	if(!loadStageFromFile(filename, s, e)) {
		ErrorPopupState es = {"load-failed", "Failed to load file: " + filename + ".\nError: " + e.what };
		editor.errorPopups.push_back(es);
		std::cout << es.message << std::endl;
		return;
	} else {
		editor.filename = filename;
		s.state = editor.stage.state;
		editor.stage = s;
	}
}

void levelOpen(sf::WindowHandle windowHandle, Editor& editor) {
	std::string filename = "";
	if(selectAFileForOpen(windowHandle, filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
		levelOpen(editor, filename);
	} else {
		// I don't, this is probably fine
	}
}

void validateSaveToFile(Editor& editor) {
	SerializeError e;
	if (! dumpStageToFile(editor.filename, editor.stage, e)) {
		ErrorPopupState es = {"dump-failed", "Failed to save stage to file.\nError: " + e.what };
		editor.errorPopups.push_back(es);
		std::cout << es.message << std::endl;
		return;
	}
}

void levelSave(sf::WindowHandle windowHandle, Editor& editor) {
	if(editor.filename.empty()) {
		if(selectAFileForSave(windowHandle, editor.filename, "Select a file to save level", "JSON Files\0*.json\0\0")) {
			validateSaveToFile(editor);
		}
	} else {
		validateSaveToFile(editor);
	}

}

void levelSaveAs(sf::WindowHandle windowHandle, Editor& editor) {
   	SerializeError e;
	std::string filename;
	if(selectAFileForSave(windowHandle, filename, "Select file to save level as", "JSON Files\0*.json\0\0")) {
		editor.filename = filename;
		validateSaveToFile(editor);
	}

}
