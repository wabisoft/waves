
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


#include <windows.h>

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}



inline bool OpenAGodDamnFile(sf::RenderWindow& window, std::string& filename, const char* PopupTitle, const char* filter = "Any File\0*.*\0\0") {
	OPENFILENAME ofn;
	char buffer[MAX_PATH];
	if(!filename.empty()){
		sprintf(buffer, "%s", filename.c_str());
	} else {
		ZeroMemory( &buffer, sizeof( buffer ) );
    	GetModuleFileName( NULL, buffer, MAX_PATH );
	}
	ZeroMemory( &ofn,      sizeof( ofn ) );
	ofn.lStructSize  = sizeof( ofn );
	ofn.hwndOwner    = window.getSystemHandle();  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter  = filter;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrTitle   = PopupTitle;
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameA( &ofn ))
	{
		filename = std::string(buffer);
		return true;
	}
	else return false;
}

inline bool SaveAGodDamnFile(sf::RenderWindow& window, std::string& filename, const char* PopupTitle, const char* filter = "Any File\0*.*\0\0") {
	OPENFILENAME ofn;
	char buffer[MAX_PATH];
	if(!filename.empty()){
		sprintf(buffer, "%s", filename.c_str());
	} else {
		ZeroMemory( &buffer, sizeof( buffer ) );
    	GetModuleFileName( NULL, buffer, MAX_PATH );
	}
	ZeroMemory( &ofn,      sizeof( ofn ) );
	ofn.lStructSize  = sizeof( ofn );
	ofn.hwndOwner    = window.getSystemHandle();  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter  = filter;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrTitle   = PopupTitle;
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (GetSaveFileNameA( &ofn ))
	{
		filename = std::string(buffer);
		return true;
	}
	else return false;
}

inline void drawEditorGui(sf::RenderWindow& window, Editor& editor) {
	// ImGui::SetNextWindowPos({3, 3});
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")){
			if(ImGui::MenuItem("Open", "Ctrl+O")) {
			 // TODO open things I guess
			 	if(OpenAGodDamnFile(window, editor.filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
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
			 	if(SaveAGodDamnFile(window, editor.filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
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
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 100;
	window.create(videoMode, "Waves: Editor", sf::Style::Default, settings);
	// TODO: Why doesn't the screen start at the top left corner?
	window.setFramerateLimit(1.f/FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	window.setPosition({0,0});
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
