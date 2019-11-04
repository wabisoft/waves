#pragma once


inline std::string exePath();
inline std::string cwd();

inline bool selectAFileForOpen(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter);
inline bool selectAFileForSave(sf::WindowHandle windowHandle, std::string& filename, const char* PopupTitle, const char* filter);

# ifdef _WIN32
	#include "win32_file.hpp"
#endif
