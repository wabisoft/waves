#include <iostream>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


#include "constants.hpp"
#include "graphics.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "stage.hpp"

void initGraphics(Graphics & graphics, const char * title) {
	graphics.settings.antialiasingLevel = 100;
	graphics.videoMode = sf::VideoMode::getDesktopMode();
	graphics.window.create(graphics.videoMode, title, sf::Style::Default, graphics.settings);
	graphics.window.setFramerateLimit(100);
	graphics.window.setVerticalSyncEnabled(true);
	graphics.ppu = (float)graphics.videoMode.width / STAGE_WIDTH;
	if (!graphics.gameFont.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
		std::cout << "Couldn't load font" << std::endl;
	}
}


void draw(Graphics& graphics, Stage& stage) {
	graphics.window.clear(sf::Color::Black);
	draw(graphics, stage.platforms, stage.numPlatforms);
	draw(graphics, stage.sea);
	if(stage.selection.active && stage.pullPosition != VECTOR2_ZERO) {
		Rock& rock = findRock(stage, stage.selection.entity.id);
		drawLine(graphics, rock.shape.position, stage.pullPosition, sf::Color::Blue);
	}
	draw(graphics, stage.rocks, stage.numRocks);
	drawInfoText(graphics, stage);
	graphics.window.display();
}


inline void draw(Graphics& graphics, const Sea& sea) {
	static std::vector<sf::Vertex> vertices;
	float step = 1 / graphics.ppu;
	for (float i = 0; i < STAGE_WIDTH; i += step)
	{
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, {i, heightAtX(sea, i)}), SEA_COLOR));
		vertices.push_back(sf::Vertex(game2ScreenPos(graphics, {i, 0}), SEA_COLOR));
	}
	graphics.window.draw(&vertices[0], vertices.size(), sf::TriangleStrip);
	vertices.clear();
}

inline void draw(Graphics& graphics, const Rock* rocks, int numRocks) {
	sf::Text idText;
	idText.setFont(graphics.gameFont);
	idText.setFillColor(sf::Color::White);
	idText.setCharacterSize(15);
	sf::CircleShape circle;
	circle.setFillColor(sf::Color(0,0,0,0));
	circle.setOutlineThickness(2);
	sf::FloatRect bounds;
	for (int i = 0; i < numRocks; ++i){
		float adjustedRadius = rocks[i].shape.radius * graphics.ppu;
		auto RockPosition = game2ScreenPos(graphics, rocks[i].shape.position);
		if (rocks[i].sized) {
			circle.setOutlineColor(sf::Color::Red);
		} else {
			circle.setOutlineColor(sf::Color::Green);
		}
		circle.setPosition(RockPosition);
		circle.setRadius(adjustedRadius);
		circle.setOrigin(adjustedRadius, adjustedRadius);
		graphics.window.draw(circle);
		idText.setPosition(RockPosition);
		idText.setString((sf::String)std::to_string(rocks[i].id));
		bounds = idText.getGlobalBounds();
		idText.setOrigin(bounds.width/2, bounds.height/2);
		graphics.window.draw(idText);
	}
}

inline void draw(Graphics& graphics, const Platform* platforms, int numPlatforms) {
	sf::Text idText;
	idText.setFont(graphics.gameFont);
	idText.setFillColor(sf::Color::White);
	idText.setCharacterSize(15);
	// sf::RectangleShape rectangle;
	// rectangle.setFillColor(sf::Color::Magenta);
	sf::FloatRect bounds;
	for (int i = 0; i < numPlatforms; ++i){
		drawPolygon(graphics, platforms[i].shape, sf::Color::Magenta);
		// float adjustedWidth = platforms[i].width * graphics.ppu;
		// float adjustedHeight = platforms[i].height * graphics.ppu;
		// auto platformPosition = game2ScreenPos(graphics, platforms[i].position);
		// rectangle.setPosition(platformPosition);
		// rectangle.setSize({adjustedWidth, adjustedHeight});
		// rectangle.setOrigin(adjustedWidth/2.f, adjustedHeight/2.f);
		// graphics.window.draw(rectangle);
		idText.setPosition(game2ScreenPos(graphics, platforms[i].shape.position));
		idText.setString((sf::String)std::to_string(platforms[i].id));
		bounds = idText.getGlobalBounds();
		idText.setOrigin(bounds.width/2, bounds.height/2);
		graphics.window.draw(idText);
	}
}

inline void drawInfoText(Graphics& graphics, const Stage& stage) {
	sf::Text text;
	text.setFont(graphics.gameFont);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(15);
	std::stringstream infostream;
	infostream << "Draw (Hz): 				" << 1/graphics.drawDelta << std::endl;
	infostream << "Update (Hz): 			" << 1/graphics.updateDelta << std::endl;
	infostream << "Loops/Update: 			" << graphics.loopsPerUpdate <<std::endl;
	infostream << std::endl;
	infostream << "#Rocks: 					" << stage.numRocks << std::endl;
	infostream << "#Waves: 					" << stage.sea.numWaves << std::endl;
	infostream << "#AABBs: 					" << stage.numAABBS << std::endl;
	if(stage.numRocks > 0 ) {
		infostream << "P:						" << stage.rocks[0].shape.position<< std::endl;
		infostream << "V:						" << stage.rocks[0].velocity << std::endl;
	}
	text.setString(infostream.str());
	text.setPosition(3, 3);
	text.setPosition(3, 3);
	graphics.window.draw(text);

	if (stage.paused) {
		text.setCharacterSize(24);
		text.setString("Paused");
		auto bounds = text.getGlobalBounds();
		text.setOrigin(bounds.width/2, bounds.height/2);
		text.setPosition((float)graphics.videoMode.width/2, (float)graphics.videoMode.height/2);
		graphics.window.draw(text);
	}
}
