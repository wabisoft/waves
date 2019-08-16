#include <bitset>
#include <iostream>
#include <climits>

#include <SFML/Window.hpp>

#include "clock.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "stage.hpp"
#include "util.hpp"

int main(){
	Stage stage;
	stage.sea.level = 13.3f;
	createPlatform(stage, {STAGE_WIDTH/2.f, STAGE_HEIGHT/2}, 30, 5);
	std::cout << "Stage: " << sizeof(stage) << std::endl;
	std::cout << "Ship : " << sizeof(stage.ship) << std::endl;
	std::cout << "Sea: " << sizeof(stage.sea) << std::endl;
	std::cout << "Rock: " << sizeof(stage.rocks[0]) << std::endl;
	std::cout << "Wave: " << sizeof(stage.sea.waves[0]) << std::endl;
	std::cout << "Platform: " << sizeof(stage.platforms[0]) << std::endl;
	std::cout << "AABB: " << sizeof(AABB(stage.platforms[0])) << std::endl;
	std::cout << "EntityType: " << sizeof((EntityType)1) << std::endl;
	std::cout << "Vector2: " << sizeof(Vector2{0,0}) << std::endl;
	std::cout << "RockState: " << sizeof(RockState{}) << std::endl;
	runStage(stage);

	// Testing shapes and shit
	// Rectangle rectangle = makeRectangle({STAGE_WIDTH/2, STAGE_HEIGHT/2 -1.5}, 5, 2);
	// Circle circle = {{STAGE_WIDTH/2, STAGE_HEIGHT/2 + 4}, 5.f};
	// Graphics graphics;
	// initGraphics(graphics, "Waves!!!");
	// while(graphics.window.isOpen()) {
	// 	graphics.window.clear(sf::Color::Blue);
	// 	sf::Event event;
	// 	while (graphics.window.pollEvent(event))
	//  	{
	// 		switch (event.type) {
	// 		    case sf::Event::Closed:	graphics.window.close(); break;
	// 			default: break;
	// 		}
	// 	}
	// 	drawPolygon(graphics, rectangle, sf::Color::White);
	// 	drawCircle(graphics, circle, sf::Color::White);

	// 	Vector2 intersection;
	// 	float depth = 0;
	// 	Collision col;
	// 	col = collision(circle, rectangle);
	// 	if(col.collides) {
	// 		sf::CircleShape c;
	// 		c.setRadius(2);
	// 		c.setOrigin(2, 2);
	// 		c.setFillColor(sf::Color::Red);
	// 		c.setPosition(game2ScreenPos(graphics, col.intersection));
	// 		graphics.window.draw(c);
	// 		// draw the normal
	// 		Vector2 normalOffset = col.intersection + 2 * col.normal;
	// 		sf::VertexArray arr(sf::Lines, 2);
	// 		arr[0] = sf::Vertex(game2ScreenPos(graphics, col.intersection), sf::Color::Magenta);
	// 		arr[1] = sf::Vertex(game2ScreenPos(graphics, normalOffset), sf::Color::Magenta);
	// 		graphics.window.draw(arr);
	// 	}
	// 	graphics.window.display();
	// }
	// return 0;
}
