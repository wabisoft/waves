#include <iostream>
#include <vector>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


#include "constants.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "stage.hpp"

void initGraphics(Graphics & graphics, const char * title) {
	graphics.settings.antialiasingLevel = 100;
	graphics.videoMode = sf::VideoMode::getDesktopMode();
	graphics.window.create(graphics.videoMode, title, sf::Style::Default, graphics.settings);
	graphics.window.setFramerateLimit(100);
	graphics.window.setVerticalSyncEnabled(true);
	graphics.ppu = (float)graphics.videoMode.width / STAGE_WIDTH;
	if (!graphics.gameFont.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
		std::cout << "Couldn't load font" << std::endl;
	}
}


void draw(Graphics& graphics, Stage& stage) {
	graphics.window.clear(sf::Color::Black);
	drawGrid(graphics);
	draw(graphics, stage.platforms, stage.numPlatforms);
	draw(graphics, stage.sea);
	if(stage.selection.active && stage.selection.state == Selection::PULL && stage.selection.pullPosition != VECTOR2_ZERO) {
		Rock& rock = findRock(stage, stage.selection.entity.id);
		drawLine(graphics, rock.shape.position, stage.selection.pullPosition, sf::Color::Blue);
	}
	drawRocks(graphics, stage);
	drawShip(graphics, stage.ship);
	drawInfoText(graphics, stage);
	if (stage.paused) {
		drawText(graphics, "Paused", {(float)graphics.videoMode.width/2, (float)graphics.videoMode.height/2}, 24);
	}
	graphics.window.display();
}


inline void draw(Graphics& graphics, const Sea& sea) {
	std::vector<sf::Vertex> vertices;
	float step = 1 / graphics.ppu;
	for (float i = 0; i < STAGE_WIDTH; i += step)
	{
		// this draw pattern causes the gpu hardware to interpolate the color alpha
		// from 1 to 0 (found this by accident but I like it)
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, {i, heightAtX(sea, i)}), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, {i+step, heightAtX(sea, i+step)}), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, {i, -5}), SEA_COLOR));
	}
	graphics.window.draw(&vertices[0], vertices.size(), sf::Triangles);
}

inline void drawShip(Graphics& graphics, const Ship& ship) {

	drawPolygon(graphics, ship.shape, sf::Color::White);
	drawId(graphics, ship.id, ship.shape.position);
}

inline void drawRocks(Graphics& graphics, const Stage& stage) {
	for (int i = 0; i < stage.numRocks; ++i){
		sf::Color c;
		if (stage.selection.active && stage.rocks[i].id == stage.selection.entity.id) {
			c = sf::Color::Cyan;
		} else if (stage.rocks[i].sized) {
			c = sf::Color::Red;
		} else {
			c = sf::Color::Green;
		}
		drawCircle(graphics, stage.rocks[i].shape, c);
		drawId(graphics, stage.rocks[i].id, stage.rocks[i].shape.position);
	}
}

inline void draw(Graphics& graphics, const Platform* platforms, int numPlatforms) {
	for (int i = 0; i < numPlatforms; ++i){
		drawPolygon(graphics, platforms[i].shape, sf::Color::Magenta);
		drawId(graphics, platforms[i].id, platforms[i].shape.position);
	}
}

inline void drawGrid(Graphics& graphics) {
	// sf::VertexArray vertices(sf::Lines, (int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	std::vector<sf::Vertex> vertices;
	vertices.reserve((int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	sf::Color c = sf::Color(175,175,175,32);
	for(float i = 0; i < STAGE_WIDTH; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2{i, 0.f}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2{i, STAGE_HEIGHT}), c));
	}
	for(float i = 0; i < STAGE_HEIGHT; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2{0.f, i}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2{STAGE_WIDTH, i}), c));
	}
	graphics.window.draw(&vertices[0], vertices.size(), sf::Lines);
}

inline void drawInfoText(Graphics& graphics, const Stage& stage) {
	std::stringstream infostream;
	infostream << "Draw (Hz): 				" << 1/graphics.drawDelta << std::endl;
	infostream << "Update (Hz): 			" << 1/graphics.updateDelta << std::endl;
	infostream << "Loops/Update: 			" << graphics.loopsPerUpdate <<std::endl;
	infostream << std::endl;
	infostream << "#Rocks: 					" << stage.numRocks << std::endl;
	infostream << "#Waves: 					" << stage.sea.numWaves << std::endl;
	infostream << "#AABBs: 					" << stage.numAABBS << std::endl;
	if(stage.numRocks > 0 ) {
		infostream << "P:						" << stage.rocks[0].shape.position<< std::endl;
		infostream << "V:						" << stage.rocks[0].velocity << std::endl;
	}
	infostream << "RockState:				" << (int)stage.rocks[0].state.type << std::endl;
	infostream << "ShipState:				" << (int)stage.ship.state.type << std::endl;
	drawText(graphics, infostream.str(), {3, 3}, 13);
}
