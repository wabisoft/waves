
#include "editor.hpp"
#include "editor_imgui.hpp"
#include "editor_level.hpp"
#include "json.hpp"
#include "logging.hpp"
#include "serialize.hpp"
#include "stage.hpp"
#include "system.hpp"

// static logging::Logger logger("Editor::Level");

void Level::open() {
	Stage s = {};
	SerializeError e;
	if(!loadStageFromFile(filename, s, e)) {
		Popup p = {"load-failed", "Failed to load file: " + filename + ".\nError: " + e.what, RED};
		editor->popups.push_back(p);
		logger.debug(p.message.c_str());
		return;
	} else {
		Popup p = {"load-successful", "Opened file: " + filename + ".", GREEN};
		editor->popups.push_back(p);
		logger.debug("%s\n", p.message.c_str());
		filename = filename;
		s.state = editor->stage.state;
		stage = s;
		editor->stage = stage;
		open_ = true;
	}
}

void Level::open(sf::WindowHandle windowHandle) {
	std::string filename = "";
	if(selectAFileForOpen(windowHandle, filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
		open();
		open_ = true;
	} else {
		Popup p = {"noop", "File selection cancelled", GREEN};
		editor->popups.push_back(p);
		logger.debug("%s\n", p.message.c_str());
	}
}

void Level::saveAndValidate(Stage& stage) {
	SerializeError e;
	if (! dumpStageToFile(filename, stage, e)) {
		Popup p = {"dump-failed", "Failed to save stage to file.\nError: " + e.what, RED};
		editor->popups.push_back(p);
		logger.debug("%s\n", p.message.c_str());
	} else {
		Popup p = {"save-successful", "Successfully saved to " + filename + "!", GREEN};
		editor->popups.push_back(p);
		logger.debug("%s\n", p.message.c_str());
	}
}

void Level::save(sf::WindowHandle windowHandle, Stage& stage) {
	if(filename.empty()) {
		if(selectAFileForSave(windowHandle, filename, "Select a file to save level", "JSON Files\0*.json\0\0")) {
			saveAndValidate(stage);
		}
	} else {
		saveAndValidate(stage);
	}
}

void Level::saveAs(sf::WindowHandle windowHandle, Stage& stage) {
   	SerializeError e;
	std::string fname;
	if(selectAFileForSave(windowHandle, fname, "Select file to save level as", "JSON Files\0*.json\0\0")) {
		filename = fname;
		saveAndValidate(stage);
	}
}
