#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "clock.hpp"
#include "maths.hpp"
#include "rock.hpp"
#include "shapes.hpp"
#include "sea.hpp"

#define SEA_COLOR sf::Color::Cyan


/************************
 *  Game Graphics stuff *
 ************************/
struct Graphics {
	Graphics();
	void drawText(sf::RenderWindow&, std::string, sf::Vector2f, int=15, bool=false);


// extern sf::Font font;
	void drawStage(sf::RenderWindow&, Stage&, bool=false);
	void drawSeas(sf::RenderWindow&, const Stage&);
	void drawShip(sf::RenderWindow&, const Ship&);
	void drawRocks(sf::RenderWindow&, const Stage&);
	void drawPlatforms(sf::RenderWindow&, const Stage&);
	void drawPullParabola(sf::RenderWindow&, Stage&);
	void drawGrid(sf::RenderWindow&);
	void drawInfoText(sf::RenderWindow&, const Stage& , float, float, int);
	void drawPolygon(sf::RenderWindow&, const wabi::Polygon&, sf::Color, sf::Color verticesColor =sf::Color(0,0,0,0));
	void drawId(sf::RenderWindow&, int, sf::Vector2f);
	void drawId(sf::RenderWindow&, int, glm::vec2);
	// void drawCircle(sf::RenderWindow&, const wabi::Circle&, sf::Color, bool fill=false);
	void drawLine(sf::RenderWindow&, glm::vec2, glm::vec2, sf::Color);

	sf::Font font;
};

inline float ppu(sf::Window&);
inline sf::Vector2f game2ScreenPos(const sf::Window&, glm::vec2);
inline glm::vec2 screen2GamePos(const sf::Window&, sf::Vector2i);


inline glm::vec2 pixelsPerUnit(const sf::Window& target) {
	// sf::Vector2f targetSize = target.getView().getSize();
	sf::Vector2<unsigned> targetSize = target.getSize();
	return {(float)targetSize.x / STAGE_WIDTH, (float) targetSize.y / STAGE_HEIGHT};
}

inline sf::Vector2f game2ScreenPos(const sf::Window & target, glm::vec2 v) {
	// sf::Vector2f targetSize = target.getView().getSize();
	sf::Vector2<unsigned> targetSize = target.getSize();
	glm::vec2 ppu = pixelsPerUnit(target);
	return sf::Vector2f(v[0] * ppu.x, targetSize.y - (v[1] * ppu.y));
}

inline glm::vec2 screen2GamePos(const sf::Window& target, sf::Vector2i v) {
	// sf::Vector2f targetSize = target.getView().getSize();
	sf::Vector2<unsigned> targetSize = target.getSize();
	glm::vec2 ppu = pixelsPerUnit(target);
	return glm::vec2{(float)v.x / ppu.x, std::fabs((float)targetSize.y - (float)v.y) / ppu.y};
}

