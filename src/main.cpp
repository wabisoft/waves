#include <bitset>
#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>

#include <SFML/Window.hpp>

#include "clock.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "serialize.hpp"
#include "stage.hpp"
#include "util.hpp"



void test_shapes_and_shit() {
	// Testing shapes and shit
	Rectangle rectangle = makeRectangle({STAGE_WIDTH/2, STAGE_HEIGHT/2}, 30, 5);
	Circle circle = {{STAGE_WIDTH/2-14, STAGE_HEIGHT/2 + 5}, 3.f};
	Vector2 p1 = rectangle.position;
	Vector2 p2 = rectangle.position - Vector2{rectangle.width, 0.f};
	Vector2 p3 = rectangle.position - Vector2{rectangle.width/2.f, 0.f};
	if (pointInsidePolygon(p1, rectangle)){
		std::cout << "p1 inside rectangle" << std::endl;
	} else {
		std::cout << "p1 outside rectangle" << std::endl;
	}
	if (pointInsidePolygon(p2, rectangle)){
		std::cout << "p2 inside rectangle" << std::endl;
	} else {
		std::cout << "p2 outside rectangle" << std::endl;
	}
	if (pointInsidePolygon(p3, rectangle)){
		std::cout << "p3 inside rectangle" << std::endl;
	} else {
		std::cout << "p3 outside rectangle" << std::endl;
	}

	Graphics graphics;
	initGraphics(graphics, "Waves!!!");
	Clock clock;
	std::srand(std::time(nullptr));
	while(graphics.window.isOpen()) {
		graphics.window.clear(sf::Color::Blue);
		sf::Event event;
		while (graphics.window.pollEvent(event))
	 	{
			switch (event.type) {
			    case sf::Event::Closed:	graphics.window.close(); break;
				default: break;
			}
		}
		float updateDelta = clock.getElapsedTime().asSeconds();
		if (updateDelta >= FIXED_TIMESTEP) {
			// Yay rotation!
			rectangle.rotation += PI * FIXED_TIMESTEP;
			updateVertices(rectangle);
			clock.restart();
		}
		drawPolygon(graphics, rectangle, sf::Color::White);
		drawCircle(graphics, circle, sf::Color::White);

		Vector2 intersection;
		float depth = 0;
		Collision col;
		col = collision(circle, rectangle);
		if(col.collides) {
			sf::CircleShape c;
			c.setRadius(2);
			c.setOrigin(2, 2);
			c.setFillColor(sf::Color::Red);
			c.setPosition(game2ScreenPos(graphics, col.intersection));
			graphics.window.draw(c);
			// draw the normal
			Vector2 normalOffset = col.intersection + 1 * col.normal;
			sf::VertexArray arr(sf::Lines, 2);
			arr[0] = sf::Vertex(game2ScreenPos(graphics, col.intersection), sf::Color::Magenta);
			arr[1] = sf::Vertex(game2ScreenPos(graphics, normalOffset), sf::Color::Magenta);
			graphics.window.draw(arr);
		}
		Vector2 l, u;
		boundingPoints(rectangle, l, u);
		Rectangle rr = makeRectangle(rectangle.position, u.x - l.x, u.y - l.y);
		drawPolygon(graphics, rr, sf::Color(std::rand()%256,std::rand()%256,std::rand()%256));
		graphics.window.display();
	}
}

inline void test_deserializer() {
	std::string data = "{\"sea_level\":13.3,\"platforms\":[{\"width\":35,\"height\":30,\"position\":[93,21],\"rotation\":0},{\"width\":6,\"height\":48,\"position\":[10,24],\"rotation\":0}],\"ship\":{\"width\":5,\"height\":3,\"position\":[42.6667,67],\"rotation\":0},\"rock_spawn\":[10,54],\"win\":{\"time\":0.25,\"region\":{\"width\":15,\"height\":5,\"position\":[93,37.5],\"rotation\":0}}}";
	Error e;
	Stage stage = deserializeStage(data, e);
	if (e.no) {
		std::cout << "Shoot!" << std::endl;
	} else {
		std::cout << "Yippee!" << std::endl;
	}
}

void do_normal_shit() {
	Game game;
	Stage& stage = game.stage;
	// std::cout << "Game: " << sizeof(game) << std::endl;
	// std::cout << "Stage: " << sizeof(stage) << std::endl;
	// std::cout << "Ship : " << sizeof(stage.ship) << std::endl;
	// std::cout << "Sea: " << sizeof(stage.sea) << std::endl;
	// std::cout << "Rock: " << sizeof(stage.rocks[0]) << std::endl;
	// std::cout << "Wave: " << sizeof(stage.sea.waves[0]) << std::endl;
	// std::cout << "Platform: " << sizeof(stage.platforms[0]) << std::endl;
	// std::cout << "AABB: " << sizeof(AABB(stage.platforms[0])) << std::endl;
	// std::cout << "EntityType: " << sizeof((EntityType)1) << std::endl;
	// std::cout << "Vector2: " << sizeof(Vector2{0,0}) << std::endl;
	// std::cout << "RockState: " << sizeof(RockState{}) << std::endl;
	start(game);
	run(game);
	stop(game);
}

int main() {
	do_normal_shit();
	// test_shapes_and_shit();
	// test_deserializer();
	return 0;
}
