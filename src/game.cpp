#include <iostream>
#include "clock.hpp"
#include "constants.hpp"
#include "graphics.hpp"
#include "stage.hpp"
#include "rock.hpp"
#include "game.hpp"
#include "util.hpp"
#include "maths.hpp"

void start(Game& game) {
	initGraphics(game.graphics, "Waves!!!");
	// TODO: this should happen during stage loading which ideal is read from a file
	game.stage = new Stage();
	game.stage->sea.level = 13.3f;
	createPlatform(*game.stage, {STAGE_WIDTH-15, STAGE_HEIGHT/2 -15}, 30, 30);
	createPlatform(*game.stage, {10.f, STAGE_HEIGHT/3}, 6, 2*STAGE_HEIGHT/3); // launching platform
	game.stage->rockSpawn = {10.f, 2*STAGE_HEIGHT/3 + 6};
	// TODO: Menu stuff
	// TODO: Load first Stage, blah blah blah
}

void run(Game& game) {
	assert(game.stage != nullptr);
	Stage& stage = *game.stage;
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
		if (drawDelta >= FRAME_RATE* game.timeScale) {
			game.graphics.drawDelta = drawDelta;
			draw(game.graphics, stage);
			game.drawClock.restart();
		}
	}
}

inline void startInput(Game& game, sf::Event& event) {
	game.screenInputState.position = screen2GamePos(game.graphics, {event.mouseButton.x, event.mouseButton.y});
	game.screenInputState.pressed = true;
	processStartInput(*game.stage, game.screenInputState.position);
}
inline void continueInput(Game& game, sf::Event& event) {
	game.screenInputState.position = screen2GamePos(game.graphics, {event.mouseMove.x, event.mouseMove.y});
	processContinuingInput(*game.stage, game.screenInputState.position);
}

inline void endInput(Game& game, sf::Event& event) {
	game.screenInputState.pressed = false;
	processEndInput(*game.stage, game.screenInputState.position);
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
		case sf::Keyboard::Key::P: game.stage->paused = !game.stage->paused; break;
		case sf::Keyboard::Key::Num1: game.timeScale = 1.f; break;
		case sf::Keyboard::Key::Num2: game.timeScale = 2.f; break;
		case sf::Keyboard::Key::Num3: game.timeScale = 3.f; break;
		case sf::Keyboard::Key::Num4: game.timeScale = 4.f; break;
		case sf::Keyboard::Key::Num5: game.timeScale = 100.f; break;
		default: break;
	}
}

void stop(Game& game) {
	delete game.stage;
}

