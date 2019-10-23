#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <glm/vec2.hpp>

#include "shapes.hpp"
#include "stage.hpp"
#include "json.hpp"

// Serialize
std::string serialize(const Stage& stage);
std::string serialize(const std::vector<Sea> seas);
// std::string serialize(const std::vector<Wave> waves);
// std::string serialize(const std::vector<Platform> platforms);
std::string serialize(const wabi::Rectangle& rectangle);
std::string serialize(const glm::vec2 v);
std::string serialize(const Win w);

template<typename T>
std::string serialize(const std::vector<T> v, std::string(*subserialize)(const T&)) {
	stringstream stream;
	stream << "[";
	for (int i = 0 ; i < v.size(); ++i) {
		stream << subserialize(v[i]);
		if ( i < v.size() - 1) { stream << ","; }
	}
	stream << "]";
	return stream.str();
}

struct SerializeError {
	std::string what;
};


bool loadStageFromString(std::string data, Stage&, SerializeError&);
bool loadStageFromFile(std::string filename, Stage&, SerializeError&);
std::string dumpStageToString(Stage&, SerializeError&);
bool dumpStageToFile(std::string filename, Stage& stage, SerializeError&);
