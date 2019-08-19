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
};


void initGraphics(Graphics & graphics, const char * title);

void draw(Graphics& graphics, Stage& stage);
inline void draw(Graphics& graphics, const Sea& sea);
inline void draw(Graphics& graphics, const Rock* rocks, int numRocks);
inline void draw(Graphics& graphics, const Platform* platforms, int numPlatforms);

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

inline void drawCircle(Graphics& graphics, const Circle& circle, sf::Color c) {
	sf::CircleShape shape;
	float radius = circle.radius * graphics.ppu;
	auto pos = game2ScreenPos(graphics, circle.position);
	shape.setPosition(pos);
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(sf::Color(0,0,0,0));
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

