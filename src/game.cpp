#include <iostream>
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
	std::string data = "{\"sea_level\":13.3,\"platforms\":[{\"width\":35,\"height\":30,\"position\":[93,21],\"rotation\":0},{\"width\":6,\"height\":48,\"position\":[10,24],\"rotation\":0}],\"ship\":{\"width\":5,\"height\":3,\"position\":[42.6667,67],\"rotation\":0},\"rock_spawn\":[10,54],\"win\":{\"time\":0.25,\"region\":{\"width\":15,\"height\":5,\"position\":[93,37.5],\"rotation\":0}}}";
	Error e;
	game.stage = deserializeStage(data, e);
	game.stage.state.type = StageState::RUNNING;
}

inline void unloadStage(Game& game) {
	game.stage = {};
}

inline void reloadStage(Game& game) {
	unloadStage(game);
	loadStage(game);
}

void start(Game& game) {
	initGraphics(game.graphics, "Waves!!!");
	loadStage(game);
}

void run(Game& game) {
	Stage& stage = game.stage;
	int loopsPerUpdate = 0;
	while (!game.end) {
		processEvents(game);
		float updateDelta = game.updateClock.getElapsedTime().asSeconds();
		if (updateDelta >= FIXED_TIMESTEP * game.timeScale) {
			game.graphics.updateDelta = updateDelta;
			game.graphics.loopsPerUpdate = loopsPerUpdate;
			update(stage, updateDelta);
			game.updateClock.restart();
			loopsPerUpdate = 0;
		} else { ++loopsPerUpdate; }
		float drawDelta = game.drawClock.getElapsedTime().asSeconds();
		if (drawDelta >= FRAME_RATE) {
			game.graphics.drawDelta = drawDelta;
			drawStage(game.graphics, stage);
			game.drawClock.restart();
		}
		if (game.stage.state.type == StageState::FINISHED && !game.stage.state.finished.win) { reloadStage(game); }
	}
}

inline void startInput(Game& game, sf::Event& event) {
	game.screenInputState.position = screen2GamePos(game.graphics, {event.mouseButton.x, event.mouseButton.y});
	game.screenInputState.pressed = true;
	processStartInput(game.stage, game.screenInputState.position);
}
inline void continueInput(Game& game, sf::Event& event) {
	game.screenInputState.position = screen2GamePos(game.graphics, {event.mouseMove.x, event.mouseMove.y});
	processContinuingInput(game.stage, game.screenInputState.position);
}

inline void endInput(Game& game, sf::Event& event) {
	game.screenInputState.pressed = false;
	processEndInput(game.stage, game.screenInputState.position);
}

void processEvents(Game& game) {
	sf::Event event;
	while (game.graphics.window.pollEvent(event)) {
		// TODO (owen): Factor the identical portions of mouse and touch input into functions or something
		switch (event.type) {
		    case sf::Event::Closed:	game.end = true; break;
	        case sf::Event::KeyPressed: keyEvent(game, event);
			case sf::Event::MouseButtonPressed: startInput(game, event); break;
			case sf::Event::MouseMoved: continueInput(game, event); break;
			case sf::Event::MouseButtonReleased: endInput(game, event); break;
	        case sf::Event::TouchBegan: startInput(game, event); break;
	        case sf::Event::TouchMoved: continueInput(game, event); break;
	        case sf::Event::TouchEnded: endInput(game, event); break;
			default: break;
		}
	}
}

void keyEvent(Game& game, sf::Event& event) {
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



