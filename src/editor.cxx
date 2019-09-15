
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "editor.hpp"
#include "graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	// ImGui::SetNextWindowPos({3, 3});
    ImGui::Begin("Editor");
	std::string blah = "";
	ImGui::InputText("thingy", &blah);
    ImGui::Button("Look at this pretty button");
	ImGui::BeginGroup();
	ImGui::InputFloat("SeaLevel", &editor.stage.sea.level);
    ImGui::EndGroup();
    ImGui::End();
    ImGui::SFML::Render(window);
}

int main()
{
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
	 	std::cout << "Couldn't load font" << std::endl;
	}

    ImGui::SFML::Init(window);

	Clock drawClock;
	Editor editor;
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
		drawStage(window, editor.stage);
		drawEditorGui(window, editor);
        window.display();
    }

    ImGui::SFML::Shutdown();
	return 0;
}
