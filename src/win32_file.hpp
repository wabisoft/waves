#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include <string>
#include <SFML/Graphics.hpp>

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}

std::string CurrentWorkingDir() {
	char buff[FILENAME_MAX];
  	_getcwd( buff, FILENAME_MAX );
  	std::string currentWorkingDir(buff);
	return currentWorkingDir;
}


inline bool selectAFileForOpen(sf::RenderWindow& window, std::string& filename, const char* PopupTitle, const char* filter = "Any File\0*.*\0\0") {
	OPENFILENAME ofn;
	char buffer[MAX_PATH];
	if(!filename.empty()){
		sprintf(buffer, "%s", filename.c_str());
	} else {
		ZeroMemory( &buffer, sizeof( buffer ) );
    	GetModuleFileName( NULL, buffer, MAX_PATH );
	}
	ZeroMemory( &ofn,      sizeof( ofn ) );
	ofn.lStructSize  = sizeof( ofn );
	ofn.hwndOwner    = window.getSystemHandle();  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter  = filter;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrTitle   = PopupTitle;
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameA( &ofn ))
	{
		filename = std::string(buffer);
		return true;
	}
	else return false;
}

inline bool selectAFileForSave(sf::RenderWindow& window, std::string& filename, const char* PopupTitle, const char* filter = "Any File\0*.*\0\0") {
	OPENFILENAME ofn;
	char buffer[MAX_PATH];
	if(!filename.empty()){
		sprintf(buffer, "%s", filename.c_str());
	} else {
		ZeroMemory( &buffer, sizeof( buffer ) );
    	GetModuleFileName( NULL, buffer, MAX_PATH );
	}
	ZeroMemory( &ofn,      sizeof( ofn ) );
	ofn.lStructSize  = sizeof( ofn );
	ofn.hwndOwner    = window.getSystemHandle();  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter  = filter;
	ofn.lpstrFile    = buffer;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrTitle   = PopupTitle;
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (GetSaveFileNameA( &ofn ))
	{
		filename = std::string(buffer);
		return true;
	}
	else return false;
}


