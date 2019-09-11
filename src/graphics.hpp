#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "maths.hpp"
#include "rock.hpp"
#include "sea.hpp"

#define SEA_COLOR sf::Color::Cyan

struct Graphics {
	sf::ContextSettings settings;
	sf::VideoMode videoMode;
	sf::RenderWindow window;
	sf::Font gameFont;
	float ppu = 0; // pixelsPerUnit;
	float drawDelta = 0;
	float updateDelta = 0;
	int loopsPerUpdate = 0;
	float desiredFrameRate = FRAME_RATE;
};


void initGraphics(Graphics & graphics, const char * title);

void drawStage(Graphics& graphics, Stage& stage);
inline void drawSea(Graphics& graphics, const Sea& sea);
inline void drawShip(Graphics& graphics, const Ship& ship);
inline void drawRocks(Graphics& graphics, const Stage& stage);
inline void drawPlatforms(Graphics& graphics, const Platform* platforms, int numPlatforms);
inline void drawUI(Graphics& graphics, Stage& stage);

inline void drawGrid(Graphics& graphics);
inline void drawInfoText(Graphics& graphics, const Stage& stage);


inline sf::Vector2f game2ScreenPos(const Graphics& graphics, const Vector2 &v) {
	return sf::Vector2f(v[0] * graphics.ppu, graphics.videoMode.height - (v[1] * graphics.ppu));
}

inline Vector2 screen2GamePos(const Graphics& graphics, const sf::Vector2i &v) {
	return Vector2{(float)v.x, std::fabs((float)graphics.videoMode.height - (float)v.y)} / (graphics.ppu);
}

inline void drawLine(Graphics& graphics, Vector2 a, Vector2 b, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, 2);
	sfVertices[0] = sf::Vertex(game2ScreenPos(graphics, a), c);
	sfVertices[1] = sf::Vertex(game2ScreenPos(graphics, b), c);
	graphics.window.draw(sfVertices);
}

inline void drawCircle(Graphics& graphics, const Circle& circle, sf::Color c, bool fill=false) {
	sf::CircleShape shape;
	float radius = circle.radius * graphics.ppu;
	auto pos = game2ScreenPos(graphics, circle.position);
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
	graphics.window.draw(shape);
}

template <int N>
inline void drawPolygon(Graphics& graphics, const Polygon<N>& polygon, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, N+1);
	for (int i = 0; i < N; ++i) {
		sfVertices[i] = sf::Vertex(game2ScreenPos(graphics, polygon.vertices[i]), c);
	}
	sfVertices[N] = sfVertices[0];
	graphics.window.draw(sfVertices);
}

inline void drawText(Graphics& graphics, std::string text, sf::Vector2f position, int size=15, bool centered=false) {
	sf::Text idText;
	idText.setFont(graphics.gameFont);
	idText.setFillColor(sf::Color::White);
	idText.setCharacterSize(size);
	idText.setPosition(position);
	idText.setString((sf::String)text);
	if(centered) {
		sf::FloatRect bounds = idText.getGlobalBounds();
		idText.setOrigin(bounds.width/2, bounds.height/2);
	}
	graphics.window.draw(idText);

}
inline void drawId(Graphics& graphics, int id, sf::Vector2f position) {
	drawText(graphics, std::to_string(id), position, 15, true);
}

inline void drawId(Graphics& graphics, int id, Vector2 position) {
	drawId(graphics , id, game2ScreenPos(graphics,position));
}
