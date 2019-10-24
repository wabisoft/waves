#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"
#include "editor.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"

using namespace glm;

// Static shit
SelectAction StaticActions::select = SelectAction();
MoveAction StaticActions::move = MoveAction();
ResizeAction StaticActions::resize = ResizeAction();

// Editor shit

void Editor::onClosed(sf::Window& window) {
	end = true;
	window.close();
}

void Editor::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton)	{
	// TODO: something state dependent
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	mouseState.down = true;
	mouseState.downPosition = position;
	mouseState.prevPosition = position;
	actions.push(actions.rpop()->onMouseButtonPressed(window, position, *this));
}

void Editor::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	// TODO: Not sure yet...
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	mouseState.down = false;
	actions.push(actions.rpop()->onMouseButtonReleased(window, position, *this));
}

void Editor::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	vec2 position = screen2GamePos(window, {mouseMove.x, mouseMove.y});
	hotEntity = findEntityAtPosition(stage, position);
	actions.push(actions.rpop()->onMouseMoved(window, position, *this));
	mouseState.prevPosition = position;
}

void Editor::onKeyPressed(sf::Window& window, Event::KeyEvent key) {
	using Key = sf::Keyboard;
	switch(key.code) {
		case Key::S:
			if(key.control && key.shift) {
				levelSaveAs(window.getSystemHandle(), *this);
			} else if (key.control) {
				levelSave(window.getSystemHandle(), *this);
			}
			break;
		case Key::O:
			break;
		default: break;
	}
	// sorted_insert(keysDown, keyEvent.code);
}

void Editor::onKeyReleased(sf::Window&, Event::KeyEvent) { }

void keyEvent(Editor& editor, sf::Event event) {

}

Action* Editor::getAction() {
	if(selectedEntity == nullptr || hotEntity == nullptr || hotEntity->id != selectedEntity->id) {
		return StaticActions::select();
	} else {
		// check closeness to edge and do either translate or resize pointers
		int edgeIndex = 0;
		bool onVert = false;
		float distFromSelectedEntityEdge = minDistFromEdge(mouseState.prevPosition, selectedEntity->shape, edgeIndex, onVert);
		if(distFromSelectedEntityEdge <= 1) {
			// RESIZE
			return StaticActions::resize(edgeIndex, onVert);
		} else {
			// MOVE
			return StaticActions::move();
		}
	}
}


// Action shit
Action* Action::onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor) { return editor.getAction(); };
Action* Action::onMouseButtonReleased(sf::Window&, glm::vec2, Editor& editor) {return editor.getAction(); };
Action* Action::onMouseMoved(sf::Window&, glm::vec2, Editor& editor) { return editor.getAction(); };



Action* SelectAction::onMouseButtonPressed(sf::Window& window, glm::vec2 position, Editor& editor) {
	editor.selectedEntity = editor.hotEntity;
	return editor.getAction();
}

Action* MoveAction::onMouseMoved(sf::Window& window, glm::vec2 position, Editor& editor) {
	if(editor.mouseState.down) {
		vec2 delta = position - editor.mouseState.prevPosition;
		editor.selectedEntity->position += delta;
		update(editor.selectedEntity->shape, editor.selectedEntity->position);
		updateAABBS(editor.stage);
		return this;
	} else {
		return editor.getAction();
	}
}

Action* ResizeAction::onMouseMoved(sf::Window& window, glm::vec2 position, Editor& editor) {
	// NOTE: I'm pretty sure this is only gonna work on rectangles
	// so we may need to rethink this if we add other shapes
	if(editor.mouseState.down) {
		vec2 delta = position - editor.mouseState.prevPosition;
		wabi::Polygon& polygon = editor.selectedEntity->shape;
		if(isDiagonal) {
			glm::vec2& prev = edgeIndex == 0 ? polygon.model[polygon.size-1] : polygon.model[edgeIndex-1];
			glm::vec2& next = polygon.model[(edgeIndex+1)%polygon.size];
			polygon.model[edgeIndex] += delta;
			if(edgeIndex % 2) {
				prev.y += delta.y;
				next.x += delta.x;
			} else {
				prev.x += delta.x;
				next.y += delta.y;
			}
		} else {
			// Todo don't allow a resize to cross itself
			// (i.e. so you can't pull edge 1to the left of edge 3 or edge 0 below edge 2 (etc)
			// 				0 (vertical)
			//				 -------
			// 3(horizontal |		| 1 (horizontal)
			// 				 -------
			// 				2 (vertical)
			if(edgeIndex%2) { // horizontal
				polygon.model[edgeIndex].x += delta.x;
				polygon.model[(edgeIndex+1)%polygon.size].x += delta.x;
			} else { // vertical
				polygon.model[edgeIndex].y += delta.y;
				polygon.model[(edgeIndex+1)%polygon.size].y += delta.y;
			}
		}
		update(polygon, editor.selectedEntity->position);
		updateAABBS(editor.stage);
		return this;
	} else {
		return editor.getAction();
	}
}


// File menu shit

void levelOpen(Editor& editor, std::string filename) {
	Stage s = {};
	SerializeError e;
	if(!loadStageFromFile(filename, s, e)) {
		PopupState es = {"load-failed", "Failed to load file: " + filename + ".\nError: " + e.what, RED};
		editor.popups.push_back(es);
		std::cout << es.message << std::endl;
		return;
	} else {
		PopupState p = {"load-successful", "Opened file: " + filename + ".", GREEN};
		editor.popups.push_back(p);
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
		PopupState es = {"dump-failed", "Failed to save stage to file.\nError: " + e.what, RED};
		editor.popups.push_back(es);
		std::cout << es.message << std::endl;
		return;
	}
	PopupState p = {"save-successful", "Save successful!", GREEN};
	editor.popups.push_back(p);

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
