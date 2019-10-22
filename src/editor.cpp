#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"
#include "editor.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"

using namespace glm;

void Editor::onClosed(sf::Window& window) {
	window.close();
}

void Editor::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton)	{
	// TODO: something state dependent
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	selectedEntity = hotEntity;
	mouseState.down = true;
	mouseState.downPosition = position;
	mouseState.prevPosition = position;
}

void Editor::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	// TODO: Not sure yet...
	mouseState.down = false;
}

void Editor::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	vec2 position = screen2GamePos(window, {mouseMove.x, mouseMove.y});
	if(!mouseState.down) {
		hotEntity = findEntityAtPosition(stage, position);
	}
	else if(selectedEntity.type != NONE) {
		vec2 delta = position - mouseState.prevPosition;
		*selectedEntity.pPosition += delta;
		update(*selectedEntity.pShape);
		updateAABBS(stage);
	}
	mouseState.prevPosition = position;
}

Cursor::Type getCursorStyle(sf::Window& window, Editor& editor) {
	if(editor.hotEntity.type == NONE) {
		// normal pointer
		return Cursor::Arrow;
	} else if(editor.selectedEntity.type == NONE || editor.hotEntity.id != editor.selectedEntity.id) {
		return Cursor::Hand;
	} else {
		// check closeness to edge and do either translate or resize pointers
		int edgeIndex = 0;
		float distFromHotEntityEdge = minDistFromEdge(editor.mouseState.prevPosition, *editor.hotEntity.pShape, edgeIndex);
		if(distFromHotEntityEdge <=0.5) {
			// use resize cursor
			return Cursor::ResizeNESW;
		} else {
			// use move cursor
			return Cursor::ResizeAll;
		}
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
