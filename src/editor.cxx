
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
#include "logging.hpp"
#include "serialize.hpp"
#include "str.hpp"
#include "win32_file.hpp"


static logging::Logger logger = logging::Logger("main");

struct ImGuiEL : public EventListener {
	ImGuiEL() { name = "ImGui"; }
	virtual void onAll(sf::Window& window, Event& event) override {
		ImGui::SFML::ProcessEvent(event);
    	ImGuiIO& io = ImGui::GetIO();
		event.handled = io.WantCaptureKeyboard || io.WantCaptureMouse;
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
	ImGui::BeginChild("Selection");
	if(editor.selectedEntity.type != NONE) {
		char buff[3];
		ImGui::Text("ID: %s", itoa(editor.selectedEntity.id, buff, 10));
		ImGui::Text("Type: %s", str(editor.selectedEntity.type).c_str());
		if (ImGui::InputFloat2(" Position", reinterpret_cast<float*>(editor.selectedEntity.pPosition))) {
			update(*editor.selectedEntity.pShape);
			updateAABBS(editor.stage);
		}
	}
    ImGui::EndChild();
}

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	editorGuiMenuBar(window.getSystemHandle(), editor);
	editorGuiErrorPopups(editor);
	ImGui::Begin("Editor");
	editorGuiStage(editor);
    ImGui::End();
}

int main() {
	logger.setLoggerLevel("EventManager", logging::CRITICAL);
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::VideoMode videoMode = {800, 450};
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	if (!font.loadFromFile("assets/fonts/IBMPlexMono-Regular.ttf")){
	 	std::cout << "Couldn't load font" << std::endl;
	}

    ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO();
	ImFont* pFont = io.Fonts->AddFontFromFileTTF((exePath() + "/assets/fonts/IBMPlexMono-Regular.ttf").c_str(), 15.f);
	ImGui::SFML::UpdateFontTexture();
	Clock drawClock;
	EventManager eventManager;
	Editor editor;
	editor.subscribe(eventManager, {Event::Closed, Event::MouseButtonPressed, Event::MouseButtonReleased, Event::MouseMoved});
	ImGuiEL imguiListener;
	eventManager._subscribe(imguiListener, {Event::Count});
    sf::Clock deltaClock;
	levelOpen(editor, "D:/code/wabisoft/waves/assets/levels/level1.json");
	while (window.isOpen()) {
		eventManager.dispatchEvents(window);
		auto style = getCursorStyle(window, editor);
		ImGui::SetMouseCursor(style);
        ImGui::SFML::Update(window, deltaClock.restart());
		// window.setMouseCursor(cursors[style]);
        window.clear();
		drawStage(window, editor.stage, true);
		if(editor.selectedEntity.type != NONE) {
			drawPolygon(window, *editor.selectedEntity.pShape, sf::Color(0, 255, 204), sf::Color::Red);
		}
		drawEditorGui(window, editor);
		ImGui::SFML::Render(window);
		window.display();
    }
    ImGui::SFML::Shutdown();
	return 0;
}
