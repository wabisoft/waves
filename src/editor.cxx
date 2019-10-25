
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


static logging::Logger logger = logging::Logger("main");

struct ImGuiEL : public EventListener {
	ImGuiEL() { name = "ImGui";  _subscribedEvents = {Event::Count}; }
	virtual void onAll(sf::Window& window, Event& event) override {
		ImGui::SFML::ProcessEvent(event);
    	ImGuiIO& io = ImGui::GetIO();
		bool keyHandled = (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && io.WantCaptureKeyboard;
		bool mouseHandled = (
			event.type == sf::Event::MouseButtonPressed
			|| event.type == sf::Event::MouseButtonReleased
			|| event.type == sf::Event::MouseEntered
			|| event.type == sf::Event::MouseLeft
			|| event.type == sf::Event::MouseMoved) && io.WantCaptureMouse;
		event.handled = keyHandled || mouseHandled;
	}
};

struct Loop{
	Loop() {
		sf::ContextSettings settings;
		sf::VideoMode videoMode = {800, 450};
		settings.antialiasingLevel = 100;
		window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
		window.setFramerateLimit(1.f/FRAME_RATE);
		ImGui::SFML::Init(window);
		ImGuiIO& io = ImGui::GetIO();
		ImFont* pFont = io.Fonts->AddFontFromFileTTF((exePath() + "/assets/fonts/IBMPlexMono-Regular.ttf").c_str(), 15.f);
		ImGui::SFML::UpdateFontTexture();
	}

	~Loop() {
		ImGui::SFML::Shutdown();
	}

	void startGame();
	void runGame();
	void stopGame();
	void startEditor();
	void runEditor();
	void stopEditor();
	void drawEditorGui();

	bool bRunGame = false;

	sf::RenderWindow window;
	Clock updateClock;
	Clock drawClock;
	EventManager eventManager;
	Editor editor;
	Game game;
	ImGuiEL imguiListener;
	Graphics graphics;
};


void Loop::drawEditorGui() {
	// MenuBar
	sf::WindowHandle windowHandle = window.getSystemHandle();
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
	// Run Popups
	for(std::vector<PopupState>::iterator it = editor.popups.begin(); it != editor.popups.end(); ++it) {
		if(!ImGui::IsPopupOpen(it->popupId.c_str()) && ! it->opened){
			ImGui::OpenPopup(it->popupId.c_str());
			it->opened = true;
		}
		if(ImGui::BeginPopup(it->popupId.c_str())) {
			ImGui::TextColored(it->color, it->message.c_str());
			ImGui::EndPopup();
		} else {
			it = editor.popups.erase(it);
			if(it == editor.popups.end()) {break;}
		}
	}
	ImGui::Begin("Editor");
	// Stage info
	ImGui::BeginChild("Selection");
	if(editor.selectedEntity) {
		char buff[3];
		ImGui::Text("ID: %s", itoa(editor.selectedEntity->id, buff, 10));
		ImGui::Text("Type: %s", str(editor.selectedEntity->type).c_str());
		if (ImGui::InputFloat2(" Position", reinterpret_cast<float*>(&editor.selectedEntity->position))) {
			update(editor.selectedEntity->shape, editor.selectedEntity->position);
			updateAABBS(editor.stage);
		}
	}
	if(ImGui::Button("Play", {40, 20})) {
		editor.end = true;
		bRunGame = true;
	}
	ImGui::EndChild();
    ImGui::End();
}

void Loop::startGame() {
	if(editor.filename.empty()){
		levelOpen(window.getSystemHandle(), editor);
	}
	game.loadStage(editor.filename);
	game.subscribe(eventManager);
	game.end = false;
}

void Loop::runGame() {
	startGame();
	while(!game.end) {
		eventManager.dispatchEvents(window);
		game.update();
		float drawDelta = drawClock.getElapsedTime().asSeconds();
		if (drawDelta >= FRAME_RATE) {
			graphics.drawStage(window, game.stage, true);
			graphics.drawInfoText(window, game.stage, drawDelta, game.updateDelta, 0);
			window.display();
			drawClock.restart();
		}
	}
	stopGame();
}

void Loop::stopGame() {
	game.unsubscribe(eventManager);
	bRunGame = false;
}

void Loop::startEditor() {
	if(!editor.filename.empty()) {
		levelOpen(editor, editor.filename);
	}
	editor.subscribe(eventManager);
	imguiListener.subscribe(eventManager);
	editor.end = false;
}

void Loop::runEditor() {
	startEditor();
	while (!editor.end) {
		eventManager.dispatchEvents(window);
		auto style = editor.actions.top()->getCursorStyle();
		ImGui::SetMouseCursor(style);
        ImGui::SFML::Update(window, updateClock.restart());
		float drawDelta = drawClock.getElapsedTime().asSeconds();
		if (drawDelta >= FRAME_RATE) {
			window.clear();
			graphics.drawStage(window, editor.stage, true);
			if(editor.selectedEntity) {
				graphics.drawPolygon(window, editor.selectedEntity->shape, sf::Color(0, 255, 204), sf::Color::Red);
			}
			wabi::Polygon c = clip(editor.stage.ship.shape, editor.stage.win.shape);
			graphics.drawPolygon(window, c, sf::Color(255, 153, 204), sf::Color::Green);
			drawEditorGui();
			ImGui::SFML::Render(window);
			window.display();
		}
    }
	stopEditor();
}

void Loop::stopEditor() {
	if(!editor.filename.empty()) {
		levelSave(window.getSystemHandle(), editor);
	}
	editor.unsubscribe(eventManager);
	imguiListener.unsubscribe(eventManager);
}

int main() {
	logger.setLoggerLevel("EventManager", logging::INFO);
    Loop loop;
	loop.editor.filename = cwd() + "\\assets\\levels\\level3.json";
    while (loop.window.isOpen()) {
		if(loop.bRunGame) {
			loop.runGame();
		} else {
			loop.runEditor();
		}
	}
	return 0;
}
