#pragma once

#include <string>
#include <sstream>
#include "stage.hpp"

// Serialize
std::string serialize(const Stage& stage);
std::string serialize(const Platform * platforms, const int numPlatforms);
std::ostream& operator<<(std::ostream& ss, const Rectangle& rectangle);
std::ostream& operator<<(std::ostream& ss, const Vector2 v);
std::ostream& operator<<(std::ostream& ss, const Win w);

// Deserialize
struct Error{
	enum Type:uint8_t {
		INCOMPLETE, // expected more json
		INCORRECT, // malformed json
		OTHER // catch all
	};
	int no = 0;
	int where = -1;
	std::string what = "";
};

// TODO: encrypt main game level data probably to prevent cheating (how do you keep secret string out of the binary?) (see: https://stackoverflow.com/questions/1356896/how-to-hide-a-string-in-binary-code)
Stage deserializeStage(std::string data, Error& error); // allocates!
