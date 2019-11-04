#pragma once

#include <string>
#include <SFML/Window/WindowHandle.hpp>

#include "logging.hpp"
#include "stage.hpp"

struct Editor;

struct Level {
	Level(Editor* e) : editor(e), logger("Editor::Level") { }
	void open();
	void open(sf::WindowHandle windowHandle);
	void saveAndValidate(Stage& stage);
	void save(sf::WindowHandle windowHandle, Stage& stage);
	void saveAs(sf::WindowHandle windowHandle, Stage& stage);
	bool is_open() const { return open_; }

	std::string filename = ""; // the name of the file to save the level
	Stage stage = {}; // the unchanged start state of the stage
	Editor* editor;
private:
	bool open_ = false;
	logging::Logger logger;

};


