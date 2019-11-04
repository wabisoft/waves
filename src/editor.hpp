#pragma once

#include <vector>
#include <stack>
#include <string>

#include "SFML/Window/WindowHandle.hpp"
#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

#include "clock.hpp"
#include "editor_action.hpp"
#include "editor_imgui.hpp"
#include "editor_level.hpp"
#include "events.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "json.hpp"
#include "prelude.hpp"
#include "stage.hpp"
#include "util.hpp"

struct MouseState {
	bool down = false;
	glm::vec2 downPosition;
	glm::vec2 prevPosition;
};

struct ImGuiListener : public EventListener {
	ImGuiListener() : EventListener("ImGui") { _subscribedEvents = {Event::Count}; }
	virtual void onAll(sf::Window& window, Event& event) override {
		ImGui::SFML::ProcessEvent(event);
    	ImGuiIO& io = ImGui::GetIO();
		bool keyHandled = (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && io.WantCaptureKeyboard;
		bool mouseHandled = (
			event.type == sf::Event::MouseButtonPressed
			|| event.type == sf::Event::MouseButtonReleased
			|| event.type == sf::Event::MouseEntered
			|| event.type == sf::Event::MouseLeft
			|| event.type == sf::Event::MouseMoved) && io.WantCaptureMouse;
		event.handled = keyHandled || mouseHandled;
	}
};

struct Editor : public EventListener {
	enum { EDIT, PLAY };

	Editor(sf::RenderWindow* p_Window) : EventListener("Editor"), window(p_Window), level(this) {
		actions.push(StaticActions::select());
		_subscribedEvents = {Event::Closed, Event::MouseButtonPressed, Event::MouseButtonReleased, Event::MouseMoved, Event::KeyPressed};
		enterMode(EDIT);
	 }
	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;
	virtual void onKeyPressed(sf::Window&, Event::KeyEvent)						override;

	Action* Editor::getAction();
	void start();
	void stop();
	void run();
	void drawImGui();

	void enterMode(int mode);
	void exitMode(int mode);
	void setMode(int mode) {
		exitMode(mode_);
		enterMode(mode);
	}
	void update(sf::Time deltaTime);
	void render(sf::Time deltaTime);

	Action* popAction() {
		return actions.rpop();
	}
	void pushAction(Action* action) {
		actions.push(action);
	}

	Level level;
	Stage stage;
	Stage playStage;
	MouseState mouseState;
	Entity* selectedEntity = nullptr;
	Entity* hotEntity = nullptr;
	std::vector<Popup> popups;
	Stack<Action*> actions;

	int mode_ = -1;
	float loopsPerUpdate = 0;
	float renderDelta = 0;
	float updateDelta = 0;

	sf::RenderWindow* window = nullptr;
	Game game;
	ImGuiListener imguiListener;
	Graphics graphics;
	EventManager eventManager;
};


