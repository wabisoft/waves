#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <glm/vec2.hpp>

#include "shapes.hpp"
#include "stage.hpp"
#include "json.hpp"

// Serialize
std::string serializeStage(const Stage& stage);
// std::string serialize(const wabi::Polygon& rectangle);
// std::string serialize(const Entity& entity, std::vector<std::string> extra = {});
// std::string serialize(const glm::vec2 v);
// std::string serialize(const Win w);


struct SerializeError {
	std::string what;
};


bool loadStageFromString(std::string data, Stage&, SerializeError&);
bool loadStageFromFile(std::string filename, Stage&, SerializeError&);
std::string dumpStageToString(Stage&, SerializeError&);
bool dumpStageToFile(std::string filename, Stage& stage, SerializeError&);
