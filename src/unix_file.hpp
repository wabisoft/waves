#pragma once

#include <unistd.h>
#include <limits.h>

#include <string>

#include <SFML/Window/WindowHandle.hpp>

inline std::string exePath() {
	// TODO: (owen) right for mac and linux
	throw "Not implemented";
}

inline std::string cwd() {
	char buffer[PATH_MAX];
	char * res = getcwd(buffer, PATH_MAX);
	if(res == nullptr) {
		throw "Error getting cwd, errno was: " + std::to_string(errno);
	}
	return std::string(buffer);
}

inline bool selectAFileForOpen(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter) {
	// TODO: (owen) right for mac and linux
	throw "Not implemented";
}

inline bool selectAFileForSave(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter){
	// TODO: (owen) right for mac and linux
	throw "Not implemented";
}
