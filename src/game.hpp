#pragma once
#include <cstdint>
#include <string>

#include "clock.hpp"
#include "events.hpp"
#include "graphics.hpp"
#include "stage.hpp"
#include "prelude.hpp"



struct Game : EventListener{
	Game() {
		name = "Game";
		_subscribedEvents = {Event::Closed, Event::MouseButtonPressed, Event::MouseButtonReleased, Event::MouseMoved, Event::KeyPressed};
	}

	virtual void onClosed(sf::Window&)											override;
	virtual void onMouseButtonPressed(sf::Window&, Event::MouseButtonEvent)		override;
	virtual void onMouseButtonReleased(sf::Window&, Event::MouseButtonEvent)	override;
	virtual void onMouseMoved(sf::Window&, Event::MouseMoveEvent)				override;
	virtual void onKeyPressed(sf::Window&, Event::KeyEvent)						override;

	void update();
	void loadStage(std::string filename = "");
	void unloadStage();
	void reloadStage();

	Stage stage;
	Clock updateClock;
	float timeScale = 1.f;
	int loopsPerUpdate = 0;
	float updateDelta = 0.f;
	bool end = false; // stop the game
	std::string filename = "";
};


void processEvent(Game& game, const sf::Event&, const sf::RenderWindow&);
void keyEvent(Game& game, const sf::Event& event);
