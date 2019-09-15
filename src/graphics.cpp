#include <iostream>
#include <vector>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "constants.hpp"
#include "graphics.hpp"
#include "physics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "stage.hpp"

sf::Font font;

void initGraphics() {
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
	 	std::cout << "Couldn't load font" << std::endl;
	}

}

void drawStage(sf::RenderTarget& target, Stage& stage, bool drawInfo) {
	target.clear(sf::Color::Black);
	drawGrid(target);
	drawPlatforms(target, stage);
	drawSea(target, stage.sea);
	if(stage.selection.active && stage.selection.state == Selection::PULL && stage.selection.pullPosition != VECTOR2_ZERO) {
		Rock& rock = findRock(stage, stage.selection.entity.id);
		drawLine(target, rock.shape.position, stage.selection.pullPosition, sf::Color::Blue);
	}
	drawRocks(target, stage);
	drawShip(target, stage.ship);
	if(drawInfo) {drawInfoText(target, stage);}
	sf::Vector2u targetSize = target.getSize();
	if (stage.state.type == StageState::PAUSED) {
		drawText(target, "Paused", {(float)targetSize.x/2, (float)targetSize.y/2}, 24);
	} else if (stage.state.type == StageState::FINISHED && stage.state.finished.win == true) {
		drawText(target, "Good Job!", {(float)targetSize.x/2, (float)targetSize.y/2}, 24);
	}
	drawPolygon(target, stage.win.region, sf::Color(0, 204, 102));
	drawPullParabola(target, stage);
}


inline void drawSea(sf::RenderTarget& target, const Sea& sea) {
	std::vector<sf::Vertex> vertices;
	float step = 1 / pixelsPerUnit(target);
	for (float i = 0; i < STAGE_WIDTH; i += step)
	{
		// this draw pattern causes the gpu hardware to interpolate the color alpha
		// from 1 to 0 (found this by accident but I like it)
		vertices.push_back(sf::Vertex(game2ScreenPos(target, {i, heightAtX(sea, i)}), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, {i+step, heightAtX(sea, i+step)}), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, {i, -5}), SEA_COLOR));
	}
	target.draw(&vertices[0], vertices.size(), sf::Triangles);
}

inline void drawShip(sf::RenderTarget& target, const Ship& ship) {
	drawPolygon(target, ship.shape, sf::Color::White);
	drawId(target, ship.id, ship.shape.position);
}

inline void drawRocks(sf::RenderTarget& target, const Stage& stage) {
	for (int i = 0; i < stage.numRocks; ++i){
		sf::Color c;
		if (stage.selection.active && stage.rocks[i].id == stage.selection.entity.id) {
			c = sf::Color::Cyan;
		} else if (stage.rocks[i].sized) {
			c = sf::Color::Red;
		} else {
			c = sf::Color::Green;
		}
		drawCircle(target, stage.rocks[i].shape, c);
		drawId(target, stage.rocks[i].id, stage.rocks[i].shape.position);
	}
}

inline void drawPlatforms(sf::RenderTarget& target, const Stage& stage) {
	for (int i = 0; i < stage.numPlatforms; ++i){
		drawPolygon(target, stage.platforms[i].shape, sf::Color::Magenta);
		drawId(target, stage.platforms[i].id, stage.platforms[i].shape.position);
	}
}

inline void drawGrid(sf::RenderTarget& target) {
	// sf::VertexArray vertices(sf::Lines, (int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	std::vector<sf::Vertex> vertices;
	vertices.reserve((int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	sf::Color c = sf::Color(175,175,175,32);
	for(float i = 0; i < STAGE_WIDTH; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(target, Vector2{i, 0.f}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, Vector2{i, STAGE_HEIGHT}), c));
	}
	for(float i = 0; i < STAGE_HEIGHT; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(target, Vector2{0.f, i}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, Vector2{STAGE_WIDTH, i}), c));
	}
	target.draw(&vertices[0], vertices.size(), sf::Lines);
}

inline void drawPullParabola(sf::RenderTarget& target, Stage& stage) {
	switch(stage.selection.state) {
		case Selection::SELECT: break;
		case Selection::RESIZE: break;
		case Selection::PULL:
			{
				std::vector<Vector2> parabola = pullParabola(stage);
				std::vector<sf::Vertex> drawVertices;
				drawVertices.reserve(parabola.size());
				sf::Color c = {183, 183,183};
				for (Vector2 worldPoint : parabola) {
					drawVertices.push_back(sf::Vertex(game2ScreenPos(target, worldPoint), c));
				}
				target.draw(&drawVertices[0], drawVertices.size(), sf::LineStrip);
			}
			break;
	}
}


inline void drawInfoText(sf::RenderTarget& target, const Stage& stage) {
	std::stringstream infostream;
	// infostream << "Draw (Hz): 				" << 1/graphics.drawDelta << std::endl;
	// infostream << "Update (Hz): 			" << 1/graphics.updateDelta << std::endl;
	// infostream << "Loops/Update: 			" << graphics.loopsPerUpdate <<std::endl;
	if (stage.state.type == StageState::RUNNING) {
		infostream << "Running Time: 			" << stage.state.running.time <<std::endl;
	} else if (stage.state.type == StageState::PAUSED) {
		infostream << "Paused Time: 			" << stage.state.paused.time <<std::endl;
	}
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
	drawText(target, infostream.str(), {3, 3}, 13);
}

inline void drawText(sf::RenderTarget& target, std::string text, sf::Vector2f position, int size, bool centered) {
	sf::Text sfText((sf::String)text, font, size);
	sfText.setFillColor(sf::Color::White);
	sfText.setPosition(position);
	if(centered) {
		sf::FloatRect bounds = sfText.getGlobalBounds();
		sfText.setOrigin(bounds.width/2, bounds.height/2);
	}
	target.draw(sfText);
}

inline void drawId(sf::RenderTarget& target, int id, sf::Vector2f position) {
	drawText(target, std::to_string(id), position, 15, true);
}

inline void drawId(sf::RenderTarget& target, int id, Vector2 position) {
	drawId(target, id, game2ScreenPos(target,position));
}

inline void drawCircle(sf::RenderTarget& target, const Circle& circle, sf::Color c, bool fill) {
	sf::CircleShape shape;
	float ppu = pixelsPerUnit(target);
	float radius = circle.radius * ppu;
	auto pos = game2ScreenPos(target, circle.position);
	shape.setPosition(pos);
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	if(!fill) {
		shape.setFillColor(sf::Color(0,0,0,0));
	} else {
		shape.setFillColor(c);
	}
	shape.setOutlineColor(c);
	shape.setOutlineThickness(1);
	target.draw(shape);
}


inline void drawLine(sf::RenderTarget& target, Vector2 a, Vector2 b, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, 2);
	sfVertices[0] = sf::Vertex(game2ScreenPos(target, a), c);
	sfVertices[1] = sf::Vertex(game2ScreenPos(target, b), c);
	target.draw(sfVertices);
}



