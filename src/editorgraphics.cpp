#include <iostream>
#include "graphics.hpp"

void initGraphics(EditorGraphics& graphics, sf::RenderWindow& window) {
	graphics.settings.antialiasingLevel = 100;
	graphics.videoMode = sf::VideoMode::getDesktopMode();
	window.create(graphics.videoMode, "Waves Editor", graphics.style, graphics.settings);
	window.setFramerateLimit((float)graphics.framerate);
	if (!graphics.editorFont.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
		std::cout << "Couldn't load font" << std::endl;
	}
	graphics.UITexture.create(graphics.videoMode.width, graphics.videoMode.height);
}

void destroyEditorGraphics(EditorGraphics&) {
}

void draw(EditorGraphics& graphics, Editor& editor, sf::RenderWindow& window) {
	float delta = graphics.clock.getElapsedTime().asSeconds();
	if(delta >= 1/graphics.framerate) {
		window.clear();
		graphics.clock.restart();
		drawUI(graphics, editor);
		drawStage(graphics, editor);
		const sf::Texture uiTex = graphics.UITexture.getTexture();
		sf::Sprite uiSpr = sf::Sprite(uiTex);
		window.draw(uiSpr);
		const sf::Texture stageTex = graphics.StageTexture.getTexture();
		sf::Sprite stageSpr = sf::Sprite(stageTex);
		window.draw(stageSpr);
		window.display();
	}
}

inline void drawUI(EditorGraphics& graphics, Editor& editor) {
	graphics.UITexture.clear();
	graphics.UITexture.display();
}


inline void drawStage(EditorGraphics& graphics, Editor& editor) {
}


