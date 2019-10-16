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
};

struct Ghost {
	Entity entity;
};

class Editor : public EventListener {
public:
	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;

	std::string filename;
	Stage stage;
	MouseState mouseState;
	Entity selectedEntity;
	std::vector<ErrorPopupState> errorPopups;
};

void initEditor(Editor& editor);
// void handleEvents(Editor& editor, sf::RenderWindow& window);
void processEvent(Editor& editor, const sf::Event& event, const sf::RenderWindow& window);
void keyEvent(Editor&, sf::Event);
void startMouseInput(Editor&, glm::vec2 );
void continueMouseInput(Editor&, glm::vec2 );
void endMouseInput(Editor&, glm::vec2 );

void levelOpen(Editor& editor, std::string filename);
void levelOpen(sf::WindowHandle windowHandle, Editor& editor);
void validateSaveToFile(Editor& editor);
void levelSave(sf::WindowHandle windowHandle, Editor& editor);
void levelSaveAs(sf::WindowHandle windowHandle, Editor& editor);
