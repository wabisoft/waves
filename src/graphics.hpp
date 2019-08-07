#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "vector2.hpp"
#include "rock.hpp"
#include "sea.hpp"

#define SEA_COLOR sf::Color::Cyan

struct Graphics {
	sf::ContextSettings settings;
	sf::VideoMode videoMode;
	sf::RenderWindow window;
	sf::Font gameFont;
	float ppu = 0; // pixelsPerUnit;
	float deltaTime = 0;
	float fixedDeltaTime = 0;
	int loopsPerFixedUpdate = 0;
};


void initGraphics(Graphics & graphics, const char * title);

void draw(Graphics& graphics, const Stage& stage);
void drawInfoText(Graphics& graphics, const Stage& stage);
inline void draw(Graphics& graphics, const Sea& sea);
inline void draw(Graphics& graphics, const Rock* rocks, int numRocks);


inline sf::Vector2f game2ScreenPos(const Graphics& graphics, const Vector2 &v)
{
	return sf::Vector2f(v[0] * graphics.ppu, graphics.videoMode.height - (v[1] * graphics.ppu));
}

inline Vector2 screen2GamePos(const Graphics& graphics, const sf::Vector2i &v)
{
	return Vector2((float)v.x, std::fabs((float)graphics.videoMode.height - (float)v.y)) / (graphics.ppu);
}

