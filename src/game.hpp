#pragma once

#include "SFML/Window/WindowHandle.hpp"
#include "SFML/Graphics.hpp"

#include "clock.hpp"
#include "events.hpp"
#include "graphics.hpp"
#include "level_io.hpp"
#include "logging.hpp"
#include "stage.hpp"
#include "prelude.hpp"


struct Game : public EventListener {
	Game(sf::RenderWindow* p_Window) : EventListener("Game"), window(p_Window) {
		_subscribedEvents = {Event::Closed, Event::MouseButtonPressed, Event::MouseButtonReleased, Event::MouseMoved, Event::KeyPressed};
	 }
	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;
	virtual void onKeyPressed(sf::Window&, Event::KeyEvent)						override;

	void start();
	void run();
	void stop();

	void update(sf::Time deltaTime);
	void render(sf::Time deltaTime);

	LevelIO levelIO;
	Stage stage;

	float loopsPerUpdate = 0;
	float renderDelta = 0;
	float updateDelta = 0;

	sf::RenderWindow* window = nullptr;
	Graphics graphics;
	EventManager eventManager;
};


