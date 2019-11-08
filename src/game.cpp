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
			Timer timer;
			update(updateClock.restart());
			auto res = timer.elapsed();
			update_total += res;
			update_count++;
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
	std::string t = std::to_string(update_total/update_count);
	graphics.drawText(*window, t, game2ScreenPos(*window, {STAGE_WIDTH-10, 10}), 15, true);
	auto s = sizeof(stage);
	if(stage.rocks.size()) {
		s += stage.rocks.size() * sizeof(stage.rocks[0]);
	}
	if(stage.platforms.size()) {
		s += stage.platforms.size() * sizeof(stage.platforms[0]);
	}
	for(auto & sea : stage.seas) {
		if(sea.waves.size()) {
			s += sea.waves.size() * sizeof(sea.waves[0]);
		}
	}
	t = std::to_string(s);
	graphics.drawText(*window, t, game2ScreenPos(*window, {STAGE_WIDTH-10, 50}), 15, true);

	window->display();
}


