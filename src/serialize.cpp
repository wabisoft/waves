#include <iostream>
#include <cstdio>
#include <fstream>
#include "serialize.hpp"
#include "json.hpp"

using namespace std;
using namespace wabi;
using namespace glm;

// TODO: serialize a more complete game state (i.e. every piece of state)
string serialize(const Stage& stage) {
	stringstream stream;
	stream << "{" <<
		"\"seas\"" << ":" << serialize<Sea>(stage.seas, [](const Sea& s) { return serialize(s.shape); }) << "," <<
		"\"platforms\"" << ":" << serialize<Platform>(stage.platforms, [](const Platform& p) { return serialize(p.shape); }) << "," <<
		"\"ship\"" 		<< ":" << serialize(stage.ship.shape) << "," <<
		"\"rock_spawn\""<< ":" << serialize(stage.rockSpawn) << "," <<
		"\"win\""		<< ":" << serialize(stage.win) <<
	"}";
	return stream.str();
}

// std::string serialize(const std::vector<Sea> seas) {
// 	stringstream stream;
// 	stream << "[";
// 	for (int i = 0 ; i < seas.size(); ++i) {
// 		// TODO: serialize more stuff
// 		stream << serialize(seas[i].shape);
// 		if ( i < seas.size() - 1) { stream << ","; }
// 	}
// 	stream << "]";
// 	return stream.str();
// }

// std::string serialize(const std::vector<Wave> waves) {
// 	stringstream stream;
// 	stream << "[";
// 	for(int i = 0; i < waves.size(); ++i) {
// 		stream << "{" <<
// 		"}";
// 		if ( i < waves.size() - 1) { stream << ","; }
// 	}
// 	return stream.str();
// }

// std::string serialize(const std::vector<Platform> platforms) {
// 	stringstream stream;
// 	stream << "[";
// 	for (int i = 0 ; i < platforms.size(); ++i) {
// 		stream << serialize(platforms[i].shape);
// 		if ( i < platforms.size() - 1) { stream << ","; }
// 	}
// 	stream << "]";
// 	return stream.str();
// }

std::string serialize(const Rectangle& rectangle) {
	stringstream stream;
	stream << "{" <<
		// "\"width\"" << ":" << rectangle.width << "," <<
		// "\"height\"" << ":" << rectangle.height << "," <<
		"\"position\"" << ":" << serialize(rectangle.position) << "," <<
		"\"rotation\"" << ":" << rectangle.rotation << "," <<
		"\"model\"" << ":" << serialize<vec2>(rectangle.model, [](const vec2& v) {return serialize(v);}) <<
	"}";
	return stream.str();
}

std::string serialize(const vec2 v) {
	stringstream stream;
	stream << "[" << v.x << "," << v.y << "]";
	return stream.str();
}

std::string serialize(const Win w) {
	stringstream stream;
	stream << "{" <<
		"\"time\"" << ":" << w.timeToWin << "," <<
		"\"region\"" << ":" <<serialize(w.region) <<
	"}";
	return stream.str();
}


template <typename T>
inline std::vector<T> extractArray(JSON j, JSONError& e, T(*extractT)(JSON, JSONError&)) {
	if(j.type != JSON::ARRAY) { return {}; }
	JSONArray array = getArray(j, e);
	std::vector<T> ret;
	for(int i = 0; i < array.size(); ++i) {
		ret.push_back(extractT(array[i], e));
		if(e.no) { return ret; }
	}
	return ret;
}

inline vec2 extractVec2(JSON j, JSONError& e) {
	std::vector<float> nums = extractArray(j, e, &getNumber);
	return {nums[0], nums[1]};
}

inline Rectangle extractRectangle(JSON j, JSONError& e) {
	Rectangle r;
	JSONObject object = getObject(j, e);
	if(e.no) { return r;}
	r.position = extractVec2(getValue(object, "position", e), e);
	if(e.no) { return r;}
	r.rotation = getNumber(getValue(object, "rotation", e), e);
	if(e.no) { return r;}
	r.model = extractArray(getValue(object, "model", e), e, &extractVec2);
	if(e.no) { return r;}
	update(r);
	return r;
}

bool loadStageFromString(std::string data, Stage& stage, SerializeError& err) {
	JSONError e;
	string_it litStart = data.begin();
	string_it litEnd = data.end();
	JSON json = getJSON(litStart, litEnd, e);
	if(e.no) {err.what = e.what; return false;}
	if(json.type != JSON::OBJECT) {
		err.what = "Stage data should be a valid json object";
		return false;
	}
	JSONObject object = getObject(json, e);
	if(e.no) {err.what = e.what; return false;}
	auto seaRectangles = extractArray(getValue(object, "seas", e), e, &extractRectangle);
	if (e.no) {err.what = e.what; return false;}
	for(Rectangle& r : seaRectangles) {
		createSea(stage, r);
	}
	if(e.no) {err.what = e.what; return false;}
	auto platformRectangles = extractArray(getValue(object, "platforms", e), e, &extractRectangle);
	if(e.no) {err.what = e.what; return false;}
	for(Rectangle &r : platformRectangles) {
		createPlatform(stage, r);
	}
	Rectangle r = extractRectangle(getValue(object, "ship", e), e);
	if(e.no) {err.what = e.what; return false;}
	createShip(stage, r);
	stage.rockSpawn = extractVec2(getValue(object, "rock_spawn", e), e);
	JSONObject jWinObject = getObject(getValue(object, "win", e), e);
	if(e.no) {err.what = e.what; return false;}
	stage.win.timeToWin = getNumber(getValue(jWinObject, "time", e), e);
	stage.win.region = extractRectangle(getValue(jWinObject, "region", e), e);
	return true;
}


bool loadStageFromFile(std::string filename, Stage& stage, SerializeError& err) {
	std::string data = "";
	std::ifstream ifs(filename, std::ifstream::in);
	if(!ifs.is_open()) { err.what = "Could not open file: " + filename; return false; }
	while (!ifs.eof()) {
		data += ifs.get();
	}
	ifs.close();
	return loadStageFromString(data, stage, err);
}

std::string dumpStageToString(Stage& stage, SerializeError & err) {
	return serialize(stage);
}

bool dumpStageToFile(std::string filename, Stage& stage, SerializeError& err) {
	FILE* pFile;
	pFile = fopen(filename.c_str(), "w");
	if (pFile == nullptr){ err.what = "Could not open file: " + filename; return false; }
	fputs(serialize(stage).c_str(), pFile);
	fclose(pFile);
	return true;
}
