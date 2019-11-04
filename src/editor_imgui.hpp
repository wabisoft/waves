#pragma once

#include <string>

#include "imgui.h"

const ImVec4 WHITE = {1, 1, 1, 1};
const ImVec4 RED = {1, 0, 0, 1};
const ImVec4 GREEN = {0, 1, 0, 1};

struct Popup {
	std::string popupId;
	std::string message;
	ImVec4 color = WHITE;
	bool opened = false;
};
