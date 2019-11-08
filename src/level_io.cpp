
#include "level_io.hpp"
#include "json.hpp"
#include "logging.hpp"
#include "serialize.hpp"
#include "stage.hpp"
#include "system.hpp"


bool LevelIO::open(Stage& stage, std::string& message) {
	Stage s_ = {};
	SerializeError e;
	if(!loadStageFromFile(filename, s_, e)) {
		message = "Failed to load file: " + filename + ".\nError: " + e.what;
		logger.debug(message.c_str());
		open_ = false;
	} else {
		message = "Opened file: " + filename + ".";
		logger.debug("%s\n", message.c_str());
		filename = filename;
		stage = s_;
		// s.state = editor->stage.state;
		open_ = true;
	}
	return open_;
}

bool LevelIO::open(sf::WindowHandle windowHandle, Stage& stage, std::string& message) {
	std::string filename = "";
	if(selectAFileForOpen(windowHandle, filename, "Select a level to edit", "JSON Files\0*.json\0\0")) {
		return open(stage, message);
	} else {
		message = "File selection cancelled";
		logger.debug("%s\n", message.c_str());
		return open_;
	}
}

bool LevelIO::saveAndValidate(Stage& stage, std::string& message) {
	SerializeError e;
	if (! dumpStageToFile(filename, stage, e)) {
		message = "Failed to save stage to file.\nError: " + e.what;
		logger.debug("%s\n", message.c_str());
		return false;
	} else {
		message = "Successfully saved to " + filename + "!";
		logger.debug("%s\n", message.c_str());
		return true;
	}
}

bool LevelIO::save(sf::WindowHandle windowHandle, Stage& stage, std::string& message) {
	if(filename.empty()) {
		if(selectAFileForSave(windowHandle, filename, "Select a file to save level", "JSON Files\0*.json\0\0")) {
			return saveAndValidate(stage, message);
		}
		else {
			return false;
		}
	} else {
		return saveAndValidate(stage, message);
	}
}

bool LevelIO::saveAs(sf::WindowHandle windowHandle, Stage& stage, std::string& message) {
   	SerializeError e;
	std::string fname;
	if(selectAFileForSave(windowHandle, fname, "Select file to save level as", "JSON Files\0*.json\0\0")) {
		filename = fname;
		return saveAndValidate(stage, message);
	}
	return false;
}
