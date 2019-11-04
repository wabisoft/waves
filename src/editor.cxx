
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <glm/vec2.hpp>

#include "collision.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "editor.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "logging.hpp"
#include "serialize.hpp"
#include "str.hpp"
#include "win32_file.hpp"


int main() {
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	sf::RenderWindow window;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	window.setFramerateLimit(1.f/FRAME_RATE);
	Editor e(&window);
	e.level.filename = cwd() + "\\assets\\levels\\level3.json";
	e.start();
	e.run();
	e.stop();
}
