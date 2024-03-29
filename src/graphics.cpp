#include <iostream>
#include <vector>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <glm/vec2.hpp>

#include "constants.hpp"
#include "graphics.hpp"
#include "physics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "stage.hpp"

using namespace wabi;
using namespace glm;

Graphics::Graphics() {
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
		std::cout << "Couldn't load font" << std::endl;
	}

}

void Graphics::drawStage(sf::RenderWindow& target, Stage& stage,  bool showGrid) {
	target.clear(sf::Color::Black);
	if (showGrid ) {
		drawGrid(target);
	}
	drawPlatforms(target, stage);
	drawSeas(target, stage);
	if(stage.selection.active && stage.selection.state == Selection::PULL && stage.selection.pull.pullPosition != VEC2_ZERO) {
		Rock& rock = *findRock(stage, stage.selection.entity->id);
		drawLine(target, rock.position, stage.selection.pull.pullPosition, sf::Color::Blue);
	}
	drawRocks(target, stage);
	drawShip(target, stage.ship);
	sf::Vector2u targetSize = target.getSize();
	if (stage.state.type == StageState::PAUSED) {
		drawText(target, "Paused", {(float)targetSize.x/2, (float)targetSize.y/2}, 24);
	} else if (stage.state.type == StageState::FINISHED && stage.state.finished.win == true) {
		drawText(target, "Good Job!", {(float)targetSize.x/2, (float)targetSize.y/2}, 24, true);
	}
	drawPolygon(target, stage.win.shape, sf::Color(0, 204, 102));
	// drawPullParabola(target, stage);
	// for (AABB aabb : stage.aabbs) {
	// 	vec2 diff = aabb.upper - aabb.lower;
	// 	vec2 pos = { aabb.lower.x + 0.5f * diff.x, aabb.lower.y + 0.5f * diff.y };
	// 	auto poly = makeRectangle(diff.x, diff.y);
	// 	update(poly, pos);
	// 	drawPolygon(target, poly, sf::Color::Yellow);
	// }
}


void Graphics::drawSeas(sf::RenderWindow& target, const Stage& stage) {
	for (const Sea& sea: stage.seas) {
		std::vector<sf::Vertex> vertices;
		// float step = 1 / pixelsPerUnit(target).x;
		// vec2 lower = lowerBound(sea.shape);
		// vec2  upper = upperBound(sea.shape);
		for (float i = 0; i < sea.shape.size; ++i) {
			vertices.push_back(sf::Vertex(game2ScreenPos(target, sea.shape.vertices[i]), SEA_COLOR));
			// vertices.push_back(sf::Vertex(game2ScreenPos(target, {i, sea.heightAtX(i)}), SEA_COLOR));
		}
		// drawPolygon(target, sea.shape, sf::Color::Yellow);
		target.draw(&vertices[0], vertices.size(), sf::LineStrip);
		// for(const Wave& wave : sea.waves) {
		// 	float minx = wave.minimumX();
		// 	float maxx = wave.maximumX();
		// 	sf::CircleShape c;
		// 	c.setFillColor(sf::Color::Red);
		// 	c.setRadius(3);
		// 	c.setPosition(game2ScreenPos(target, {minx, wave.position.y}));
		// 	target.draw(c);
		// 	c.setPosition(game2ScreenPos(target, {maxx, wave.position.y}));
		// 	target.draw(c);
		// }
	}
}

void Graphics::drawShip(sf::RenderWindow& target, const Ship& ship) {
	drawPolygon(target, ship.shape, sf::Color::White);
	drawId(target, ship.id, ship.position);
}

void Graphics::drawRocks(sf::RenderWindow& target, const Stage& stage) {
	for(const Rock& rock : stage.rocks) {
		sf::Color c;
		switch(rock.kind) {
			case Rock::RED: c = sf::Color::Red; break;
			case Rock::GREEN: c = sf::Color::Green; break;
			case Rock::BLUE: c = sf::Color::Blue; break;
		}
		drawPolygon(target, rock.shape, c);
		drawId(target, rock.id, rock.position);
	}
}

void Graphics::drawPlatforms(sf::RenderWindow& target, const Stage& stage) {
	for (const Platform& platform : stage.platforms) {
		drawPolygon(target, platform.shape, sf::Color::Magenta);
		drawId(target, platform.id, platform.position);
	}
}

void Graphics::drawGrid(sf::RenderWindow& target) {
	// sf::VertexArray vertices(sf::Lines, (int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	static std::vector<sf::Vertex> vertices;
	vertices.reserve((int)STAGE_WIDTH*2 + (int)STAGE_HEIGHT*2);
	sf::Color c = sf::Color(175,175,175,32);
	for(float i = 0; i < STAGE_WIDTH; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(target, vec2{i, 0.f}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, vec2{i, STAGE_HEIGHT}), c));
	}
	for(float i = 0; i < STAGE_HEIGHT; ++i) {
		vertices.push_back(sf::Vertex(game2ScreenPos(target, vec2{0.f, i}), c));
		vertices.push_back(sf::Vertex(game2ScreenPos(target, vec2{STAGE_WIDTH, i}), c));
	}
	target.draw(&vertices[0], vertices.size(), sf::Lines);
	vertices.clear();
}

void Graphics::drawPullParabola(sf::RenderWindow& target, Stage& stage) {
	switch (stage.state.type) {
		case StageState::PAUSED: break;
		case StageState::FINISHED:
		case StageState::RUNNING:
			switch(stage.selection.state) {
				case Selection::SELECT: break;
				case Selection::PULL:
					{
						std::vector<vec2> parabola = pullParabola(stage);
						static std::vector<sf::Vertex> drawVertices;
						drawVertices.reserve(parabola.size());
						sf::Color c = {183, 183,183};
						for (vec2 worldPoint : parabola) {
							drawVertices.push_back(sf::Vertex(game2ScreenPos(target, worldPoint), c));
						}
						target.draw(&drawVertices[0], drawVertices.size(), sf::LineStrip);
						drawVertices.clear();
					}
					break;
			}
			break;
	}
}

void Graphics::drawText(sf::RenderWindow& target, std::string text, sf::Vector2f position, int size, bool centered) {
	sf::Text sfText((sf::String)text, Graphics::font, size);
	sfText.setFillColor(sf::Color::White);
	sfText.setPosition(position);
	if(centered) {
		sf::FloatRect bounds = sfText.getGlobalBounds();
		sfText.setOrigin(bounds.width/2, bounds.height/2);
	}
	target.draw(sfText);
}

void Graphics::drawInfoText(sf::RenderWindow& target, const Stage& stage, float drawDelta, float updateDelta, int loopsPerUpdate) {
	std::stringstream infostream;
	infostream << "Draw (Hz):				" << 1/drawDelta << std::endl;
	infostream << "Update (Hz):				" << 1/updateDelta << std::endl;
	infostream << "Loops/Update:			" << loopsPerUpdate <<std::endl;
	if (stage.state.type == StageState::RUNNING) {
		infostream << "Running Time:			" << stage.state.running.time <<std::endl;
	} else if (stage.state.type == StageState::PAUSED) {
		infostream << "Paused Time:				" << stage.state.paused.time <<std::endl;
	}
	infostream << std::endl;
	infostream << "#Rocks:					" << stage.rocks.size() << std::endl;
	// infostream << "#Waves:					" << stage.sea.waves.size()<< std::endl;
	infostream << "#AABBs:					" << stage.aabbs.size() << std::endl;
	if(stage.rocks.size() > 0 ) {
		infostream << "P:						" << stage.rocks[0].position<< std::endl;
		infostream << "V:						" << stage.rocks[0].velocity << std::endl;
	}
	if (!stage.rocks.empty())
		infostream << "RockState:				" << (int)stage.rocks[0].state.type << std::endl;
	infostream << "ShipState:				" << (int)stage.ship.state.type << std::endl;
	drawText(target, infostream.str(), {3, 3}, 13);
}

void Graphics::drawId(sf::RenderWindow& target, int id, sf::Vector2f position) {
	drawText(target, std::to_string(id), position, 15, true);
}

void Graphics::drawId(sf::RenderWindow& target, int id, vec2 position) {
	drawId(target, id, game2ScreenPos(target,position));
}

void Graphics::drawLine(sf::RenderWindow& target, vec2 a, vec2 b, sf::Color c) {
	static sf::Vertex sfVertices [] = {sf::Vertex(), sf::Vertex()};
	sfVertices[0] = sf::Vertex(game2ScreenPos(target, a), c);
	sfVertices[1] = sf::Vertex(game2ScreenPos(target, b), c);
	target.draw(sfVertices, 2, sf::LinesStrip);
}


void Graphics::drawPolygon(sf::RenderWindow& target, const wabi::Polygon& polygon, sf::Color c, sf::Color verticesColor) {
	static sf::CircleShape circ(2);
	circ.setOrigin(1, 1);
	circ.setFillColor(verticesColor);
	// sf::VertexArray sfVertices(sf::LineStrip, polygon.size+1);
	int stop = polygon.isChain ? polygon.size-1 : polygon.size;
	for (int i = 0; i < stop; ++i) {
		// sf::Vector2f p1 = game2ScreenPos(target, polygon.vertices[i]);
		// sfVertices[i] = sf::Vertex(screenPos, c);
		drawLine(target, polygon.vertices[i], polygon.vertices[(i+1)%polygon.size], c);
		circ.setPosition(game2ScreenPos(target,polygon.vertices[i]));
		target.draw(circ);
	}
	// sfVertices[polygon.size] = sfVertices[0];
	// target.draw(sfVertices);
	if (polygon.isChain) {
		circ.setPosition(game2ScreenPos(target, polygon.vertices[polygon.size-1]));
		target.draw(circ);
	}
	auto cent = game2ScreenPos(target, centroid(polygon.vertices));
	circ.setPosition(cent);
	circ.setFillColor(sf::Color(255, 51, 0));
	target.draw(circ);
}

