
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdio>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "editor.hpp"
#include "graphics.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	// ImGui::SetNextWindowPos({3, 3});
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")){
			if(ImGui::MenuItem("Open", "Ctrl+O")) {
			 // TODO open things I guess
			 	if(selectAFileForOpen(window, editor.filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
					editor.stage = {};
					SerializeError e;
					if(loadStageFromFile(editor.filename, editor.stage, e)) {
						if(ImGui::BeginPopup("Load Failed")) {
							ImGui::Text("Failed to load file: %s (%s)", editor.filename.c_str(), e.what.c_str());
							ImGui::EndPopup();
						}
					}
				} else {
					if(ImGui::BeginPopup("Open Failed")) {
						ImGui::Text("Failed to open file: %s", editor.filename.c_str());
						ImGui::EndPopup();
					}
				}
    		}

			if(ImGui::MenuItem("Save", "Ctrl+S")) {
				// TODO: Serialize stage and write to filename in editor.filename
			}
			if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
				// TODO: Select file, Serialize stage and write to filename in editor.filename
			 	if(selectAFileForSave(window, editor.filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
    ImGui::Begin("Editor");
	ImGui::BeginChild("Stage");
	ImGui::InputFloat("SeaLevel", &editor.stage.sea.level);
    ImGui::EndChild();
    ImGui::End();
    ImGui::SFML::Render(window);
}

int main()
{
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	// window.setPosition({0,0});
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
	 	std::cout << "Couldn't load font" << std::endl;
	}

    ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO();
	ImFont* pFont = io.Fonts->AddFontFromFileTTF((ExePath() + "/assets/fonts/IBMPlexMono-Regular.ttf").c_str(), 15.f);
	ImGui::SFML::UpdateFontTexture();

	Clock drawClock;
	Editor editor;
	editor.stage.state.type = StageState::RUNNING;
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            switch(event.type) {
				case sf::Event::Closed:
                	window.close();
					break;
				case sf::Event::Resized:
					{
						auto view = window.getView();
						view.setCenter({event.size.width/2.f, event.size.height/2.f});
						view.setSize({(float)event.size.width, (float)event.size.height});
						window.setView(view);
					}
					break;
				default: break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
		drawStage(window, editor.stage, true);
		drawEditorGui(window, editor);
		window.display();
    }

    ImGui::SFML::Shutdown();
	return 0;
}
