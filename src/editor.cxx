
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "editor.hpp"
#include "graphics.hpp"
#include "serialize.hpp"
#include "win32_file.hpp"


inline void editorGuiMenuBar(sf::WindowHandle windowHandle, Editor& editor) {
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")){
			if(ImGui::MenuItem("Open", "Ctrl+O")) {
				levelOpen(windowHandle, editor);
    		}
			if(ImGui::MenuItem("Save", "Ctrl+S")) {
				levelSave(windowHandle, editor);
			}
			if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
				levelSaveAs(windowHandle, editor);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

inline void editorGuiErrorPopups(Editor& editor) {
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

inline void editorGuiStage(Editor& editor) {
	ImGui::Begin("Selection");
	if(editor.stage.selection.active) {
		char buff[3];
		ImGui::Text("ID: %s", itoa(editor.stage.selection.entity.id, buff, 10));
		ImGui::Text("Type: %s", str(editor.stage.selection.entity.type));
	}
    ImGui::End();
}

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	// ImGui::SetNextWindowPos({3, 3});
	editorGuiMenuBar(window.getSystemHandle(), editor);
	editorGuiErrorPopups(editor);
	editorGuiStage(editor);
	ImGui::Begin("Editor");
	ImGui::Text("Selection State: %s", str(editor.stage.selection.state));
	// ImGui::InputFloat("SeaLevel", &editor.stage.sea.level);
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
	EventManager eventManager;
	Editor editor;
	eventManager.subscribe(editor, Event::Closed);
	eventManager.subscribe(editor, Event::MouseButtonPressed);
	eventManager.subscribe(editor, Event::MouseButtonReleased);
	eventManager.subscribe(editor, Event::MouseMoved);
    sf::Clock deltaClock;
	levelOpen(editor, "D:/code/wabisoft/waves/assets/levels/level1.json");

    while (window.isOpen()) {
        sf::Event event;
		eventManager.dispatchEvents(reinterpret_cast<sf::Window&>(window));
        // while (window.pollEvent(event)) {
        //     // ImGui::SFML::ProcessEvent(event);
		// 	// processEvent(editor, event, window);
        //     switch(event.type) {
		// 		case sf::Event::Closed:
        //         	window.close();
		// 			break;
		// 		case sf::Event::Resized:
		// 			{
		// 				auto view = window.getView();
		// 				view.setCenter({event.size.width/2.f, event.size.height/2.f});
		// 				view.setSize({(float)event.size.width, (float)event.size.height});
		// 				window.setView(view);
		// 			}
		// 			break;
		// 		default: break;
        //     }
        // }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();
		drawStage(window, editor.stage, true);
		drawEditorGui(window, editor);
		window.display();
    }
    ImGui::SFML::Shutdown();
	return 0;
}
