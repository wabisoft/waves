#pragma once

#include <vector>
#include <stack>
#include <string>

#include "SFML/Window/WindowHandle.hpp"
#include "SFML/Graphics.hpp"


#include "clock.hpp"
#include "events.hpp"
#include "json.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "util.hpp"

#include "imgui.h"

const ImVec4 WHITE = {1, 1, 1, 1};
const ImVec4 RED = {1, 0, 0, 1};
const ImVec4 GREEN = {0, 1, 0, 1};

struct PopupState {
	std::string popupId;
	std::string message;
	ImVec4 color = WHITE;
	bool opened = false;
};

struct MouseState {
	bool down = false;
	glm::vec2 downPosition;
	glm::vec2 prevPosition;
};

namespace Cursor {
	enum Style{
	    Arrow = 0,
	    TextInput,
	    ResizeAll,
	    ResizeNS,
	    ResizeEW,
	    ResizeNESW,
	    ResizeNWSE,
	    Hand,
	    COUNT
	};
};

struct Editor;

struct Action {
	enum Type {
		SELECT,
		MOVE,
		RESIZE,
		COUNT
	};
	virtual Action* onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor);
	virtual Action* onMouseButtonReleased(sf::Window&, glm::vec2, Editor& editor);
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor);
	virtual Cursor::Style getCursorStyle() { return Cursor::Arrow; }
};

struct SelectAction : Action {
	Action* operator()() { return this; }
	virtual Action* onMouseButtonPressed(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override { return Cursor::Arrow; }
};

struct MoveAction : Action {
	Action* operator()() { return this; }
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override { return Cursor::ResizeAll; }
};

struct ResizeAction : Action {
	Action* operator()(int edgeIdx, bool isDiag) { edgeIndex = edgeIdx; isDiagonal = isDiag; return this;}
	virtual Action* onMouseMoved(sf::Window&, glm::vec2, Editor& editor) override;
	virtual Cursor::Style getCursorStyle() override {
		Cursor::Style straight[]= {
			Cursor::ResizeNS,
			Cursor::ResizeEW
		};
		Cursor::Style diagonal[]= {
			Cursor::ResizeNWSE,
			Cursor::ResizeNESW
		};
	 	return isDiagonal ? diagonal[edgeIndex%2] : straight[edgeIndex%2];
	}

	int edgeIndex;
	bool isDiagonal;

};

struct StaticActions {
	static SelectAction select;
	static MoveAction move;
	static ResizeAction resize;
};

struct Editor : public EventListener {
	Editor() {
		name = "Editor";
		actions.push(StaticActions::select());
	 }
	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;
	virtual void onKeyPressed(sf::Window&, Event::KeyEvent)						override;
	virtual void onKeyReleased(sf::Window&, Event::KeyEvent)						override;

	Action* Editor::getAction();
	std::string filename;
	Stage stage;
	MouseState mouseState;
	EntityHandle selectedEntity;
	EntityHandle hotEntity;
	std::vector<PopupState> popups;
	Stack<Action*> actions;
	std::vector<sf::Keyboard::Key> keysDown;
};

// Cursor::Type getCursorStyle(sf::Window&, Editor&);
void keyEvent(Editor&, sf::Event);

void levelOpen(Editor& editor, std::string filename);
void levelOpen(sf::WindowHandle windowHandle, Editor& editor);
void validateSaveToFile(Editor& editor);
void levelSave(sf::WindowHandle windowHandle, Editor& editor);
void levelSaveAs(sf::WindowHandle windowHandle, Editor& editor);
