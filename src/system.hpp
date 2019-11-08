#pragma once

#include <string>
#include <SFML/Window/WindowHandle.hpp>

inline std::string exePath();
inline std::string cwd();

inline bool selectAFileForOpen(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter);
inline bool selectAFileForSave(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter);

# ifdef _WIN32
	#include "win32_file.hpp"
# else
	#include "unix_file.hpp"
#endif

