#include <iostream>
#include <sstream>


#include "constants.hpp"
#include "graphics.hpp"
#include "printing.hpp"
#include "sea.hpp"
#include "stage.hpp"

#ifdef _WIN32
#include <windows.h>
std::string workingDir() {
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	std::string str = std::string(buffer);
	std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
	return std::string( buffer ).substr( 0, pos);
}
#endif

void initGraphics(Graphics & graphics, const char * title) {
	graphics.settings.antialiasingLevel = 100;
	graphics.videoMode = sf::VideoMode::getDesktopMode();
	graphics.window.create(graphics.videoMode, title, sf::Style::Default, graphics.settings);
	graphics.window.setFramerateLimit(100);
	graphics.window.setVerticalSyncEnabled(true);
	graphics.ppu = (float)graphics.videoMode.width / STAGE_WIDTH;
	if (!graphics.gameFont.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
		std::cout << "Couldn't load font" << std::endl;
		std::cout << workingDir() << std::endl;
	}
}


void draw(Graphics& graphics, const Stage& stage) {
	graphics.window.clear(sf::Color::Black);
	draw(graphics, stage.sea);
	draw(graphics, stage.rocks, stage.numRocks);
	drawInfoText(graphics, stage);
}

void drawInfoText(Graphics& graphics, const Stage& stage) {
	sf::Text text;
	text.setFont(graphics.gameFont);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(15);
	std::stringstream infostream;
	infostream << "FrameRate(Hz): " << 1/graphics.deltaTime <<std::endl;
	infostream << "Simulation Frequency(Hz): " << 1/graphics.fixedDeltaTime <<std::endl;
	infostream << "Loops/SimulationStep(Hz): " << graphics.loopsPerFixedUpdate <<std::endl;
	text.setString(infostream.str());
	text.setPosition(3, 3);
	graphics.window.draw(text);

	if (stage.paused) {
		text.setCharacterSize(24);
		text.setString("Paused");
		auto bounds = text.getGlobalBounds();
		text.setOrigin(bounds.width/2, bounds.height/2);
		text.setPosition(graphics.videoMode.width/2, graphics.videoMode.height/2);
		graphics.window.draw(text);
	}
}

inline void draw(Graphics& graphics, const Rock* rocks, int numRocks) {
	sf::Text idText;
	idText.setFont(graphics.gameFont);
	idText.setFillColor(sf::Color::White);
	idText.setCharacterSize(15);
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Red);
	sf::FloatRect bounds;
	for (int i = 0; i < numRocks; ++i){
		float adjustedRadius = rocks[i].radius * graphics.ppu;
		auto RockPosition = game2ScreenPos(graphics, rocks[i].position);
		circle.setPosition(RockPosition);
		circle.setRadius(adjustedRadius);
		circle.setOrigin(adjustedRadius, adjustedRadius);
		graphics.window.draw(circle);
		idText.setPosition(RockPosition);
		idText.setString(std::to_string(i));
		bounds = idText.getGlobalBounds();
		idText.setOrigin(bounds.width/2, bounds.height/2);
		graphics.window.draw(idText);
	}
}


inline void draw(Graphics& graphics, const Sea& sea) {
	static std::vector<sf::Vertex> vertices;
	float step = 1 / graphics.ppu;
	for (float i = 0; i < STAGE_WIDTH; i += step)
	{
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2(i, heightAtX(sea, i))), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, Vector2(i, 0)), SEA_COLOR));
	}
	graphics.window.draw(&vertices[0], vertices.size(), sf::TriangleStrip);
	vertices.clear();
}
