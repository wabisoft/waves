#include "game.hpp"

void runStage(Stage& stage){
	Graphics graphics;
	initGraphics(graphics, "Waves!!!");
	Clock fixedUpdateClock;
	Clock drawClock;
	bool mousePressed = false;
	Vector2 mousePosition;
	int loopsPerFixedUpdate = 0;
	float slowDown = 1.f;
	while(graphics.window.isOpen()){
		sf::Event event;
		while (graphics.window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				graphics.window.close();
			} else if (event.type == sf::Event::MouseButtonPressed && !mousePressed) {
				mousePosition = screen2GamePos(graphics, sf::Mouse::getPosition());
				mousePressed = true;
			} else if (event.type == sf::Event::MouseButtonReleased) {
				// Ideally there are a fixed number of rocks and we shouldn't need any error handling here
				createRock(stage, mousePosition, 3.f);
				mousePressed = false;
			} else if (event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Key::P){
					stage.paused = !stage.paused;
				} else if(event.key.code == sf::Keyboard::Key::Num1){
					slowDown = 1.f;
				} else if(event.key.code == sf::Keyboard::Key::Num2){
					slowDown = 2.f;
				} else if(event.key.code == sf::Keyboard::Key::Num3){
					slowDown = 3.f;
				} else if(event.key.code == sf::Keyboard::Key::Num4){
					slowDown = 4.f;
				}



			}
		}
		float fixedUpdateElapsedTime = fixedUpdateClock.getElapsedTime().asSeconds();
		if (fixedUpdateElapsedTime >= FIXED_TIMESTEP * slowDown) {
			graphics.fixedDeltaTime = fixedUpdateElapsedTime;
			graphics.loopsPerFixedUpdate = loopsPerFixedUpdate;
			fixedUpdate(stage, fixedUpdateElapsedTime);
			fixedUpdateClock.restart();
			loopsPerFixedUpdate = 0;
		} else { ++loopsPerFixedUpdate; }
		float drawElapsedTime = drawClock.getElapsedTime().asSeconds();
		if (drawElapsedTime >= FRAME_RATE) {
			graphics.deltaTime = drawElapsedTime;
			draw(graphics, stage);
			graphics.window.display();
			drawClock.restart();
		}
	}
}
