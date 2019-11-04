#pragma once

#include <string>
#include <SFML/Window/WindowHandle.hpp>

#include "logging.hpp"
#include "stage.hpp"

struct Editor;

struct LevelIO {
	LevelIO() : logger("Level") { }
	bool open(Stage& stage, std::string& message);
	bool open(sf::WindowHandle windowHandle, Stage& stage, std::string& message);
	bool saveAndValidate(Stage& stage, std::string& message);
	bool save(sf::WindowHandle windowHandle, Stage& stage, std::string& message);
	bool saveAs(sf::WindowHandle windowHandle, Stage& stage, std::string& message);
	bool is_open() const { return open_; }
	std::string filename = ""; // the name of the file to save the level
	Stage stage = {}; // the unchanged start state of the stage

private:
	bool open_ = false;
	logging::Logger logger;

};

