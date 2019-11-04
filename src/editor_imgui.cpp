
#include "editor.hpp"
#include "editor_imgui.hpp"
#include "str.hpp"

using namespace wabi;

void Editor::drawImGui() {
	// MenuBar
	sf::WindowHandle windowHandle = window->getSystemHandle();
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")){
			if(ImGui::MenuItem("Open", "Ctrl+O")) {
				level.open(windowHandle);
    		}
			if(ImGui::MenuItem("Save", "Ctrl+S")) {
				level.save(windowHandle, stage);
			}
			if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
				level.saveAs(windowHandle, stage);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	// Run Popups
	for(std::vector<Popup>::iterator it = popups.begin(); it != popups.end(); ++it) {
		if(!ImGui::IsPopupOpen(it->popupId.c_str()) && ! it->opened){
			ImGui::OpenPopup(it->popupId.c_str());
			it->opened = true;
		}
		if(ImGui::BeginPopup(it->popupId.c_str())) {
			ImGui::TextColored(it->color, it->message.c_str());
			ImGui::EndPopup();
		} else {
			it = popups.erase(it);
			if(it == popups.end()) {break;}
		}
	}
	ImGui::Begin("Editor");
	// Stage info
	ImGui::BeginChild("Selection");
	if(selectedEntity) {
		char buff[3];
		ImGui::Text("ID: %s", itoa(selectedEntity->id, buff, 10));
		ImGui::Text("Type: %s", str(selectedEntity->type).c_str());
		if (ImGui::InputFloat2(" Position", reinterpret_cast<float*>(&selectedEntity->position))) {
			wabi::update(selectedEntity->shape, selectedEntity->position);
			updateAABBS(stage);
		}
		if (ImGui::InputFloat(" Rotation", &selectedEntity->shape.rotation)) {
			wabi::update(selectedEntity->shape, selectedEntity->position);
			updateAABBS(stage);
		}
	}
	if(ImGui::Button("Play", {40, 20})) {
		setMode(PLAY);
	}
	ImGui::EndChild();
    ImGui::End();
}

