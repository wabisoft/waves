#pragma once

#include <vector>
#include <string>

#include "clock.hpp"
#include "events.hpp"
#include "json.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "SFML/Window/WindowHandle.hpp"
#include "SFML/Graphics.hpp"


// TODO: Write an editor
struct ErrorPopupState {
	std::string popupId;
	std::string message;
	bool opened = false;
};

struct MouseState {
	bool down = false;
	glm::vec2 downPosition;
	glm::vec2 prevPosition;
};

namespace Cursor {
	enum Type {
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


struct Editor : public EventListener {
	Editor() {
		name = "Editor";
	 }
	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;

	std::string filename;
	Stage stage;
	MouseState mouseState;
	EntityHandle selectedEntity;
	EntityHandle hotEntity;
	std::vector<ErrorPopupState> errorPopups;
	sf::Cursor cursors [(int)sf::Cursor::NotAllowed + 1];
};

Cursor::Type getCursorStyle(sf::Window&, Editor&);
void initEditor(Editor& editor);
void keyEvent(Editor&, sf::Event);

void levelOpen(Editor& editor, std::string filename);
void levelOpen(sf::WindowHandle windowHandle, Editor& editor);
void validateSaveToFile(Editor& editor);
void levelSave(sf::WindowHandle windowHandle, Editor& editor);
void levelSaveAs(sf::WindowHandle windowHandle, Editor& editor);
