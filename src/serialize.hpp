#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "shapes.hpp"
#include "stage.hpp"
#include "json.hpp"

// Serialize
std::string serialize(const Stage& stage);
std::string serialize(const std::vector<Sea> seas);
std::string serialize(const std::vector<Wave> waves);
std::string serialize(const std::vector<Platform> platforms);
std::string serialize(const wabi::Rectangle& rectangle);
std::string serialize(const Vector2 v);
std::string serialize(const Win w);

struct SerializeError {
	std::string what;
};


bool loadStageFromString(std::string data, Stage&, SerializeError&);
bool loadStageFromFile(std::string filename, Stage&, SerializeError&);
std::string dumpStageToString(Stage&, SerializeError&);
bool dumpStageToFile(std::string filename, Stage& stage, SerializeError&);
