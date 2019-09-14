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
inline void drawPullParabola(Graphics& graphics, Stage& stage);
inline void drawGrid(Graphics& graphics);
inline void drawInfoText(Graphics& graphics, const Stage& stage);
inline sf::Vector2f game2ScreenPos(const Graphics& graphics, const Vector2 &v);
inline Vector2 screen2GamePos(const Graphics& graphics, const sf::Vector2i &v);
inline void drawLine(Graphics& graphics, Vector2 a, Vector2 b, sf::Color c);
inline void drawCircle(Graphics& graphics, const Circle& circle, sf::Color c, bool fill=false);
template <int N>
inline void drawPolygon(Graphics& graphics, const Polygon<N>& polygon, sf::Color c);
inline void drawText(Graphics& graphics, std::string text, sf::Vector2f position, int size=15, bool centered=false);
inline void drawId(Graphics& graphics, int id, sf::Vector2f position);
inline void drawId(Graphics& graphics, int id, Vector2 position);

#include "graphics.inl"


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
