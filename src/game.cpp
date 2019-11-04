#include <iostream>

#include "game.hpp"

using namespace glm;

void Game::onClosed(sf::Window& window) {
	window.close();
}

void Game::onMouseButtonPressed(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	processStartInput(stage, position);
}

void Game::onMouseButtonReleased(sf::Window& window, Event::MouseButtonEvent mouseButton) {
	vec2 position = screen2GamePos(window, {mouseButton.x, mouseButton.y});
	processEndInput(stage, position);
}

void Game::onMouseMoved(sf::Window& window, Event::MouseMoveEvent mouseMove) {
	vec2 position = screen2GamePos(window, {mouseMove.x, mouseMove.y});
	processContinuingInput(stage, position);
}

void Game::onKeyPressed(sf::Window&, Event::KeyEvent) {

}

void Game::start() {
	stage = {};
	if(!levelIO.filename.empty()) {
		std::string message;
		levelIO.open(stage, message);
		std::cout << message << std::endl;
	}
	subscribe(eventManager);
	logging::Logger::setLoggerLevel("EventManager", logging::INFO);
}

void Game::run() {
	Clock updateClock;
	Clock renderClock;
	while (window->isOpen()) {
		eventManager.dispatchEvents(*window);
		updateDelta = updateClock.getElapsedTime().asSeconds();
		if(updateDelta >= FIXED_TIMESTEP) {
			update(updateClock.restart());
		}
		renderDelta = renderClock.getElapsedTime().asSeconds();
		if (renderDelta >= FRAME_RATE) {
			render(renderClock.restart());
		}
	}
}

void Game::stop() {
	unsubscribe(eventManager);
}

void Game::update(sf::Time deltaTime) {
	Stage::update(stage, std::min(deltaTime.asSeconds(), FIXED_TIMESTEP));
	if(stage.state.type == StageState::FINISHED && !stage.state.finished.win) {
		std::string message;
		levelIO.open(stage, message); // reload the stage
		std::cout << message << std::endl;
	}
}

void Game::render(sf::Time deltaTime) {
	graphics.drawStage(*window, stage, false);
	wabi::Polygon c = clip(stage.ship.shape, stage.seas[0].shape);
	graphics.drawPolygon(*window, c, sf::Color(255, 153, 153), sf::Color::Green);
	window->display();
}


