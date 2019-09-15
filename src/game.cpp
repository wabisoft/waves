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

inline void loadStage(Game& game) {
	// this is not how this function should work but for now let's hard code some level data
	// std::string data = "{\"sea_level\":13.3,\"platforms\":[{\"width\":35,\"height\":30,\"position\":[93,21],\"rotation\":0},{\"width\":6,\"height\":48,\"position\":[10,24],\"rotation\":0}],\"ship\":{\"width\":5,\"height\":3,\"position\":[42.6667,67],\"rotation\":0},\"rock_spawn\":[10,54],\"win\":{\"time\":0.25,\"region\":{\"width\":15,\"height\":5,\"position\":[93,37.5],\"rotation\":0}}}";
	// std::string data = "";
	// std::ifstream ifs("assets/levels/level1.json", std::ifstream::in);
	// if(!ifs.is_open()) { std::cout << "Fuck!" << std::endl; throw "fuck";}
	// while (!ifs.eof()) {
	// 	data += ifs.get();
	// }
	// ifs.close();
	// game.stage = deserializeStage(data, e);
	SerializeError e;
	if(loadStageFromFile("assets/levels/level1.json", game.stage, e)) {
		game.stage.state.type = StageState::RUNNING;
	}
}

inline void unloadStage(Game& game) {
	game.stage = {};
}

inline void reloadStage(Game& game) {
	unloadStage(game);
	loadStage(game);
}

void start(Game& game) {
	loadStage(game);
}

void update(Game& game) {
	Stage& stage = game.stage;
	float updateDelta = game.updateClock.getElapsedTime().asSeconds();
	if (updateDelta >= FIXED_TIMESTEP * game.timeScale) {
		update(stage, updateDelta);
		game.updateClock.restart();
		game.loopsPerUpdate = 0;
	} else { ++game.loopsPerUpdate; }
	if (game.stage.state.type == StageState::FINISHED && !game.stage.state.finished.win) {
		reloadStage(game);
	}
}

void processEvent(Game& game, const sf::Event& event, const sf::RenderTarget& target) {
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
		default: break;
	}
}

void stop(Game& game) {
}



