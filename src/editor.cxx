
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


inline void levelOpen(sf::RenderWindow& window, Editor& editor) {
	std::string filename = "";
	if(selectAFileForOpen(window, filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
		Stage s = {};
		editor.stage = {};
		SerializeError e;
		if(!loadStageFromFile(filename, s, e)) {
			ErrorPopupState es = {"load-failed", "Failed to load file: " + filename + ".\nError: " + e.what };
			editor.errorPopups.push_back(es);
			std::cout << es.message << std::endl;
			return;
		} else {
			editor.filename = filename;
			editor.stage = s;
		}
	} else {
		// I don't, this is probably fine
	}
}

inline void validateSaveToFile(sf::RenderWindow& window, Editor& editor) {
	SerializeError e;
	if (! dumpStageToFile(editor.filename, editor.stage, e)) {
		ErrorPopupState es = {"dump-failed", "Failed to save stage to file.\nError: " + e.what };
		editor.errorPopups.push_back(es);
		std::cout << es.message << std::endl;
		return;
	}
}

inline void levelSave(sf::RenderWindow& window, Editor& editor) {
	if(editor.filename.empty()) {
		if(selectAFileForSave(window, editor.filename, "Select a file to save level", "JSON Files\0*.json\0\0")) {
			validateSaveToFile(window, editor);
		}
	} else {
		validateSaveToFile(window, editor);
	}

}

inline void levelSaveAs(sf::RenderWindow& window, Editor& editor) {
   	SerializeError e;
	std::string filename;
	if(selectAFileForSave(window, filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
		editor.filename = filename;
		validateSaveToFile(window, editor);
	}

}

inline void editorGuiMenuBar(sf::RenderWindow& window, Editor& editor) {
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")){
			if(ImGui::MenuItem("Open", "Ctrl+O")) {
				levelOpen(window, editor);
    		}
			if(ImGui::MenuItem("Save", "Ctrl+S")) {
				levelSave(window, editor);
			}
			if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
				levelSaveAs(window, editor);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

inline void editorGuiErrorPopups(sf::RenderWindow& window, Editor& editor) {
	for(std::vector<ErrorPopupState>::iterator it = editor.errorPopups.begin(); it != editor.errorPopups.end(); ++it) {
		if(!ImGui::IsPopupOpen(it->popupId.c_str()) && ! it->opened){
			ImGui::OpenPopup(it->popupId.c_str());
			it->opened = true;
		}
		if(ImGui::BeginPopup(it->popupId.c_str())) {
			ImGui::TextColored({255, 0, 0, 255}, it->message.c_str());
			ImGui::EndPopup();
		} else {
			it = editor.errorPopups.erase(it);
			if(it == editor.errorPopups.end()) {break;}
		}
	}

}

inline void editorGuiStage(sf::RenderWindow& window, Editor& editor) {
	ImGui::BeginChild("Stage");

    ImGui::EndChild();
}

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	// ImGui::SetNextWindowPos({3, 3});
	editorGuiMenuBar(window, editor);
	editorGuiErrorPopups(window, editor);
	// editorGuiStage(window, editor);
	// ImGui::Begin("Editor");
	// ImGui::InputFloat("SeaLevel", &editor.stage.sea.level);
    // ImGui::End();
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
			processEvent(editor, event, window);
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
