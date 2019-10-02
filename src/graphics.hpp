#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "clock.hpp"
#include "maths.hpp"
#include "rock.hpp"
#include "sea.hpp"

#define SEA_COLOR sf::Color::Cyan


/************************
 *  Game Graphics stuff *
 ************************/

extern sf::Font font;
void initGraphics();
void drawStage(sf::RenderTarget&, Stage&, bool=false);
inline void drawSeas(sf::RenderTarget&, const Stage&);
inline void drawShip(sf::RenderTarget&, const Ship&);
inline void drawRocks(sf::RenderTarget&, const Stage&);
inline void drawPlatforms(sf::RenderTarget&, const Stage&);
inline void drawPullParabola(sf::RenderTarget&, Stage&);
inline void drawGrid(sf::RenderTarget&);
void drawInfoText(sf::RenderTarget&, const Stage& , float, float, int);

template <int N>
inline void drawPolygon(sf::RenderTarget&, const Polygon<N>&, sf::Color);
inline void drawText(sf::RenderTarget&, std::string, sf::Vector2f, int=15, bool=false);
inline void drawId(sf::RenderTarget&, int, sf::Vector2f);
inline void drawId(sf::RenderTarget&, int, Vector2);
inline void drawCircle(sf::RenderTarget&, const Circle&, sf::Color, bool fill=false);
inline void drawLine(sf::RenderTarget&, Vector2, Vector2, sf::Color);

inline sf::Vector2f game2ScreenPos(const sf::RenderTarget&, Vector2);
inline Vector2 screen2GamePos(const sf::RenderTarget&, sf::Vector2i);
float ppu(sf::RenderTarget&);

template <int N>
void drawPolygon(sf::RenderTarget& target, const Polygon<N>& polygon, sf::Color c) {
	sf::VertexArray sfVertices(sf::LineStrip, N+1);
	for (int i = 0; i < N; ++i) {
		sfVertices[i] = sf::Vertex(game2ScreenPos(target, polygon.vertices[i]), c);
	}
	sfVertices[N] = sfVertices[0];
	target.draw(sfVertices);
}

inline Vector2 pixelsPerUnit(const sf::RenderTarget& target) {
	sf::Vector2f targetSize = target.getView().getSize();
	return {(float)targetSize.x / STAGE_WIDTH, (float) targetSize.y / STAGE_HEIGHT};
}

inline sf::Vector2f game2ScreenPos(const sf::RenderTarget& target, Vector2 v) {
	sf::Vector2f targetSize = target.getView().getSize();
	Vector2 ppu = pixelsPerUnit(target);
	return sf::Vector2f(v[0] * ppu.x, targetSize.y - (v[1] * ppu.y));
}

inline Vector2 screen2GamePos(const sf::RenderTarget& target, sf::Vector2i v) {
	sf::Vector2f targetSize = target.getView().getSize();
	Vector2 ppu = pixelsPerUnit(target);
	return Vector2{(float)v.x / ppu.x, std::fabs((float)targetSize.y - (float)v.y) / ppu.y};
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

