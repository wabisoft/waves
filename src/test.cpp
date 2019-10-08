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
#include "shapes.hpp"
#include "stage.hpp"
#include "test.hpp"
#include "util.hpp"

using namespace wabi;
void test_shapes_and_shit(sf::RenderWindow& window) {
	// Testing shapes and shit
	wabi::Rectangle rectangle = wabi::makeRectangle({STAGE_WIDTH/2, STAGE_HEIGHT/2}, 30, 5);
	wabi::Circle circle = {{STAGE_WIDTH/2-14, STAGE_HEIGHT/2 + 5}, 3.f};
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

	Clock clock;
	std::srand(std::time(nullptr));
	bool stop = false;
	while(!stop) {
		window.clear(sf::Color::Blue);
		sf::Event event;
		while (window.pollEvent(event))
	 	{
			switch (event.type) {
			    case sf::Event::Closed:	stop = true; break;
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
		drawPolygon(window, rectangle, sf::Color::White);
		drawCircle(window, circle, sf::Color::White);

		Vector2 intersection;
		float depth = 0;
		Collision col;
		col = collision(circle, rectangle);
		if(col.collides) {
			sf::CircleShape c;
			c.setRadius(2);
			c.setOrigin(2, 2);
			c.setFillColor(sf::Color::Red);
			c.setPosition(game2ScreenPos(window, col.intersection));
			window.draw(c);
			// draw the normal
			Vector2 normalOffset = col.intersection + 1 * col.normal;
			sf::VertexArray arr(sf::Lines, 2);
			arr[0] = sf::Vertex(game2ScreenPos(window, col.intersection), sf::Color::Magenta);
			arr[1] = sf::Vertex(game2ScreenPos(window, normalOffset), sf::Color::Magenta);
			window.draw(arr);
		}
		Vector2 l, u;
		boundingPoints(rectangle, l, u);
		Rectangle rr = makeRectangle(rectangle.position, u.x - l.x, u.y - l.y);
		drawPolygon(window, rr, sf::Color(std::rand()%256,std::rand()%256,std::rand()%256));
		window.display();
	}
}

void test_deserializer() {
	std::string data = "{\"sea_level\":13.3,\"platforms\":[{\"width\":35,\"height\":30,\"position\":[93,21],\"rotation\":0},{\"width\":6,\"height\":48,\"position\":[10,24],\"rotation\":0}],\"ship\":{\"width\":5,\"height\":3,\"position\":[42.6667,67],\"rotation\":0},\"rock_spawn\":[10,54],\"win\":{\"time\":0.25,\"region\":{\"width\":15,\"height\":5,\"position\":[93,37.5],\"rotation\":0}}}";
	SerializeError e;
	Stage stage;
	loadStageFromString(data, stage, e);
	std::cout << e.what << std::endl;
}

void test_utils() {
//  	int blah [10] =  {};
// 	int blah_size = 0;
// 	std::cout << "Empty" << std::endl;
// 	print_array(blah, blah_size);
// 
// 	std::cout << "Inserting" << std::endl;
// 	sorted_insert(1, blah, blah_size);
// 	print_array(blah, blah_size);
// 
// 	sorted_insert(2, blah, blah_size);
// 	print_array(blah, blah_size);
// 
// 
// 	sorted_insert(8, blah, blah_size);
// 	print_array(blah, blah_size);
// 
// 	sorted_insert(6, blah, blah_size);
// 	print_array(blah, blah_size);
// 
// 	sorted_insert(3, blah, blah_size);
// 	print_array(blah, blah_size);
// 
// 	std::cout << "Pre-sort" << std::endl;
// 	print_array(blah, blah_size);
//  	insertion_sort(blah, blah_size);
// 	std::cout << "Post-sort" << std::endl;
// 	print_array(blah, blah_size);
// 	sorted_insert(4, blah, blah_size);
// 	std::cout << "Insert 4" << std::endl;
// 	print_array(blah, blah_size);
// 	std::cout << "4 is at index: ";
// 	std::cout << binary_find_where(4, blah, blah_size, [] (int a) { return a; });
// 	std::cout << std::endl;
// 	std::cout << "10 is at index: ";
// 	std::cout << binary_find_where(10, blah, blah_size, [] (int a) { return a; });
// 	std::cout << std::endl;
// 	std::cout << "1 is at index: ";
// 	std::cout << binary_find_where(1, blah, blah_size, [] (int a) { return a; });
// 	std::cout << std::endl;
// 	std::cout << "6 is at index: ";
// 	std::cout << binary_find_where(6, blah, blah_size, [] (int a) { return a; });
// 	std::cout << std::endl;
}
