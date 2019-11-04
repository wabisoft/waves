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

