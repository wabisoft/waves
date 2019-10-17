
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <glm/vec2.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "editor.hpp"
#include "graphics.hpp"
#include "serialize.hpp"
#include "str.hpp"
#include "win32_file.hpp"

struct ImGuiEL : public EventListener {
	ImGuiEL() { name = "ImGui"; }
	virtual void onAll(sf::Window& window, Event& event) override {
		ImGui::SFML::ProcessEvent(event);
	}
};


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
	if(editor.selection.entity.type != NONE) {
		char buff[3];
		ImGui::Text("ID: %s", itoa(editor.selection.entity.id, buff, 10));
		ImGui::Text("Type: %s", str(editor.selection.entity.type).c_str());
		ImGui::Text("Position: %s", str(editor.selection.entityPosition).c_str());
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
}

int main() {
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
	ImFont* pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF((ExePath() + "/assets/fonts/IBMPlexMono-Regular.ttf").c_str(), 15.f);
	ImGui::SFML::UpdateFontTexture();
	Clock drawClock;
	EventManager eventManager;
	Editor editor;
	eventManager.subscribe(editor, Event::Closed);
	eventManager.subscribe(editor, Event::MouseButtonPressed);
	eventManager.subscribe(editor, Event::MouseButtonReleased);
	eventManager.subscribe(editor, Event::MouseMoved);
	ImGuiEL imguiListener;
	eventManager.subscribe(imguiListener, Event::Count);
    sf::Clock deltaClock;
	levelOpen(editor, "D:/code/wabisoft/waves/assets/levels/level1.json");
    while (window.isOpen()) {
        sf::Event event;
		eventManager.dispatchEvents(window);
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();
		drawStage(window, editor.stage, true);
		drawEditorGui(window, editor);
		ImGui::SFML::Render(window);
		window.display();
    }
    ImGui::SFML::Shutdown();
	return 0;
}
