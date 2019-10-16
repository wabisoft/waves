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

extern sf::Font font;
void initGraphics();
void drawStage(sf::RenderWindow&, Stage&, bool=false);
inline void drawSeas(sf::RenderWindow&, const Stage&);
inline void drawShip(sf::RenderWindow&, const Ship&);
inline void drawRocks(sf::RenderWindow&, const Stage&);
inline void drawPlatforms(sf::RenderWindow&, const Stage&);
inline void drawPullParabola(sf::RenderWindow&, Stage&);
inline void drawGrid(sf::RenderWindow&);
void drawInfoText(sf::RenderWindow&, const Stage& , float, float, int);

template <int N>
inline void drawPolygon(sf::RenderWindow&, const wabi::Polygon<N>&, sf::Color);
inline void drawText(sf::RenderWindow&, std::string, sf::Vector2f, int=15, bool=false);
inline void drawId(sf::RenderWindow&, int, sf::Vector2f);
inline void drawId(sf::RenderWindow&, int, glm::vec2);
inline void drawCircle(sf::RenderWindow&, const wabi::Circle&, sf::Color, bool fill=false);
inline void drawLine(sf::RenderWindow&, glm::vec2, glm::vec2, sf::Color);

inline sf::Vector2f game2ScreenPos(const sf::Window&, glm::vec2);
inline glm::vec2 screen2GamePos(const sf::Window&, sf::Vector2i);
float ppu(sf::Window&);

template <int N>
void drawPolygon(sf::RenderWindow& target, const wabi::Polygon<N>& polygon, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, N+1);
	for (int i = 0; i < N; ++i) {
		sfVertices[i] = sf::Vertex(game2ScreenPos(target, polygon.vertices[i]), c);
	}
	sfVertices[N] = sfVertices[0];
	target.draw(sfVertices);
}

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


#include "editor.hpp"
/*************************
 * Editor Graphics stuff *
 * ***********************/

struct EditorGraphics {
	EditorGraphics() { videoMode = videoMode.getDesktopMode(); }
	sf::ContextSettings settings;
	sf::VideoMode videoMode;
	sf::RenderTexture StageTexture;
	sf::RenderTexture UITexture;
	sf::Font editorFont;
	int style = sf::Style::Default;
	Clock clock;
	int framerate = 60;
};

void initGraphics(EditorGraphics&, sf::RenderWindow&);
// void destroyEditorGraphics(EditorGraphics&);
//
void draw(EditorGraphics&, Editor&, sf::RenderWindow&);
inline void drawUI(EditorGraphics& graphics, Editor& editor);
inline void drawStage(EditorGraphics& graphics, Editor& editor);

