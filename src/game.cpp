#include <iostream>
#include "game.hpp"

void runStage(Stage& stage){
	Graphics graphics;
	initGraphics(graphics, "Waves!!!");
	Clock fixedUpdateClock;
	Clock drawClock;
	bool mousePressed = false;
	Vector2 mousePosition;
	int loopsPerFixedUpdate = 0;
	float timeScale = 1.f;
	while(graphics.window.isOpen()){
		sf::Event event;
		while (graphics.window.pollEvent(event))
	 	{
			switch (event.type) {
			    case sf::Event::Closed:	graphics.window.close(); break;
		        case sf::Event::KeyPressed:
					switch (event.key.code) {
        				case sf::Keyboard::Key::P:
	 						stage.paused = !stage.paused;
							break;
       					case sf::Keyboard::Key::Num1:
	   						timeScale = 1.f; break;
       					case sf::Keyboard::Key::Num2:
	   						timeScale = 2.f; break;
       					case sf::Keyboard::Key::Num3:
	   						timeScale = 3.f; break;
       					case sf::Keyboard::Key::Num4:
	   						timeScale = 4.f; break;
						default: break;
					}
					break;
		        case sf::Event::MouseButtonPressed:
					mousePosition = screen2GamePos(graphics, sf::Mouse::getPosition());
	 				mousePressed = true;
					break;
		        case sf::Event::MouseButtonReleased:
					createRock(stage, mousePosition, 3.f);
	 				mousePressed = false;
					break;
		        case sf::Event::TouchBegan: break; // bummer city man, we can mess with touches till we're on our target devices
		        case sf::Event::TouchMoved: break;
		        case sf::Event::TouchEnded: break;
				default: break;
			}
	 	}
		float fixedUpdateElapsedTime = fixedUpdateClock.getElapsedTime().asSeconds();
		if (fixedUpdateElapsedTime >= FIXED_TIMESTEP * timeScale) {
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
