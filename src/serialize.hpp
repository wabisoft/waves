#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "stage.hpp"
#include "json.hpp"

// Serialize
std::string serialize(const Stage& stage);
std::string serialize(const std::vector<Platform> platforms);
std::string serialize(const Rectangle& rectangle);
std::string serialize(const Vector2 v);
std::string serialize(const Win w);

struct SerializeError {
	std::string what;
};


bool loadStageFromString(std::string, Stage&, SerializeError&);
bool loadStageFromFile(std::string, Stage&, SerializeError&);
