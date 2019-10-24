#include <iostream>
#include <fstream>

#include "clock.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "rock.hpp"
#include "util.hpp"
#include "serialize.hpp"
#include "stage.hpp"
#include "system.hpp"

void Game::loadStage(std::string aFilename) {
	SerializeError e;
	if(aFilename.empty()) {aFilename = filename;}
	if (loadStageFromFile(aFilename, stage, e)) {
		filename = aFilename;
		stage.state.type = StageState::RUNNING;
		return;
	} else {
		throw e;
	}
}

void Game::unloadStage() {
	stage = {};
}

void Game::reloadStage() {
	unloadStage();
	loadStage();
}

void Game::update() {
	float updateDelta = updateClock.getElapsedTime().asSeconds();
	if (updateDelta >= FIXED_TIMESTEP * timeScale) {
		Stage::update(stage, std::min(updateDelta, FIXED_TIMESTEP)); // the min call is to prevent too large a time step from fucking up collision detection
		updateClock.restart();
		loopsPerUpdate = 0;
		updateDelta = updateDelta;
	} else { ++loopsPerUpdate; }
	if (stage.state.type == StageState::FINISHED && !stage.state.finished.win) {
		reloadStage();
	}
}

void Game::onClosed(sf::Window& w) {
	end = true;
	w.close();
}

void Game::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	processStartInput(stage, screen2GamePos(window, {mouseButton.x, mouseButton.y}));
}

void Game::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	processEndInput(stage, screen2GamePos(window, {mouseButton.x, mouseButton.y}));
}

void Game::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	processContinuingInput(stage, screen2GamePos(window, {mouseMove.x, mouseMove.y}));
}

void Game::onKeyPressed(sf::Window& window, Event::KeyEvent key) {
	switch (key.code) {
		case sf::Keyboard::Key::P:
			if (stage.state.type == StageState::PAUSED) {
				stage.state.type = StageState::RUNNING;
			} else {
				stage.state.type = StageState::PAUSED;
			}
			break;
		case sf::Keyboard::Key::Q: end = true; break;
		case sf::Keyboard::Key::Num1: timeScale = 1.f; break;
		case sf::Keyboard::Key::Num2: timeScale = 2.f; break;
		case sf::Keyboard::Key::Num3: timeScale = 3.f; break;
		case sf::Keyboard::Key::Num4: timeScale = 4.f; break;
		case sf::Keyboard::Key::Num5: timeScale = 100.f; break;
		case sf::Keyboard::Key::Num6: timeScale = 1000.f; break;
		case sf::Keyboard::Key::R: 
			if (key.control) {
				reloadStage();
			} else {
				stage.rockKind = {Rock::RED};
			}
			break;
		case sf::Keyboard::Key::G: stage.rockKind = {Rock::GREEN}; break;
		case sf::Keyboard::Key::B: stage.rockKind = {Rock::BLUE}; break;
		default: break;
	}

}



void processEvent(Game& game, const sf::Event& event, const sf::RenderWindow& target) {
	// TODO (owen): Factor the identical portions of mouse and touch input into functions or something
	switch (event.type) {
	    case sf::Event::Closed:	game.end = true; break;
	    case sf::Event::KeyPressed: keyEvent(game, event);
		case sf::Event::MouseButtonPressed: processStartInput(game.stage, screen2GamePos(target, {event.mouseButton.x, event.mouseButton.y})); break;
		case sf::Event::MouseMoved: processContinuingInput(game.stage, screen2GamePos(target, {event.mouseMove.x, event.mouseMove.y})); break;
		case sf::Event::MouseButtonReleased: processEndInput(game.stage, screen2GamePos(target, {event.mouseButton.x, event.mouseButton.y})); break;
	    case sf::Event::TouchBegan: processStartInput(game.stage, screen2GamePos(target, {event.mouseButton.x, event.mouseButton.y})); break;
	    case sf::Event::TouchMoved: processContinuingInput(game.stage, screen2GamePos(target, {event.mouseMove.x, event.mouseMove.y})); break;
	    case sf::Event::TouchEnded: processEndInput(game.stage, screen2GamePos(target, {event.mouseButton.x, event.mouseButton.y})); break;
		default: break;
	}
}

void keyEvent(Game& game, const sf::Event& event) {
	switch (event.key.code) {
		case sf::Keyboard::Key::P:
			if (game.stage.state.type == StageState::PAUSED) {
				game.stage.state.type = StageState::RUNNING;
			} else {
				game.stage.state.type = StageState::PAUSED;
			}
			break;
		case sf::Keyboard::Key::Q: game.end = true; break;
		case sf::Keyboard::Key::Num1: game.timeScale = 1.f; break;
		case sf::Keyboard::Key::Num2: game.timeScale = 2.f; break;
		case sf::Keyboard::Key::Num3: game.timeScale = 3.f; break;
		case sf::Keyboard::Key::Num4: game.timeScale = 4.f; break;
		case sf::Keyboard::Key::Num5: game.timeScale = 100.f; break;
		case sf::Keyboard::Key::Num6: game.timeScale = 1000.f; break;
		case sf::Keyboard::Key::R: game.stage.rockKind = {Rock::RED}; break;
		case sf::Keyboard::Key::G: game.stage.rockKind = {Rock::GREEN}; break;
		case sf::Keyboard::Key::B: game.stage.rockKind = {Rock::BLUE}; break;
		default: break;
	}
}


