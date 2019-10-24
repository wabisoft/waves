#include <iostream>
#include <cstdio>
#include <fstream>
#include "serialize.hpp"
#include "json.hpp"

using namespace std;
using namespace wabi;
using namespace glm;

std::string serializeVec2(const vec2& v) {
	stringstream stream;
	stream << "[" << v.x << "," << v.y << "]";
	return stream.str();
}

template<typename TElement, typename TParam>
std::string serializeArray(const std::vector<TElement>& v, std::string(*serializeT)(const TParam&)) {
	stringstream stream;
	stream << "[";
	for (int i = 0 ; i < v.size(); ++i) {
		stream << serializeT(v[i]);
		if ( i < v.size() - 1) { stream << ","; }
	}
	stream << "]";
	return stream.str();
}

std::string serializePolygon(const Polygon& polygon) {
	stringstream stream;
	stream << "{" <<
		"\"rotation\"" << ":" << polygon.rotation << "," <<
		"\"model\"" << ":" << serializeArray(polygon.model, &serializeVec2) <<
	"}";
	return stream.str();
}

std::string serializeEntity(const Entity& entity) {
	stringstream stream;
	stream << "{" <<
		"\"position\"" << ":" << serializeVec2(entity.position) << "," <<
		"\"shape\"" << ":" << serializePolygon(entity.shape) << "," <<
		"\"type\"" << ":" << (int)entity.type  <<
	"}";
	return stream.str();
}

std::string serializeWin(const Win& win) {
	stringstream stream;
	stream << "{" <<
		"\"position\"" << ":" << serializeVec2(win.position) << "," <<
		"\"shape\"" << ":" << serializePolygon(win.shape) << "," <<
		"\"type\"" << ":" << (int)win.type  << "," <<
		"\"time\"" << ":" << win.timeToWin <<
	"}";
	return stream.str();
}


// TODO: serialize a more complete game state (i.e. every piece of state)
string serializeStage(const Stage& stage) {
	stringstream stream;
	stream << "{" <<
		"\"seas\"" << ":" << serializeArray<Sea>(stage.seas, &serializeEntity) << "," <<
		"\"platforms\"" << ":" << serializeArray<Platform>(stage.platforms, &serializeEntity) << "," <<
		"\"ship\"" 		<< ":" << serializeEntity(stage.ship) << "," <<
		"\"rock_spawn\""<< ":" << serializeVec2(stage.rockSpawn) << "," <<
		"\"win\""		<< ":" << serializeWin(stage.win) <<
	"}";
	return stream.str();
}


// std::string serialize(const Win w) {
// 	stringstream stream;
// 	stream << "{" <<
// 		"\"time\"" << ":" << w.timeToWin << "," <<
// 		"\"region\"" << ":" <<serialize(w.shape) <<
// 	"}";
// 	return stream.str();
// }

template <typename T>
inline std::vector<T> extractArray(JSON j, JSONError& e, T(*extractT)(JSON, JSONError&)) {
	if(e.no) { return {}; }
	if(j.type != JSON::ARRAY) { return {}; }
	JSONArray array = getArray(j, e);
	if(e.no) { return {}; }
	std::vector<T> ret;
	ret.reserve(array.size());
	for(int i = 0; i < array.size(); ++i) {
		ret.push_back(extractT(array[i], e));
		if(e.no) { return ret; }
	}
	return ret;
}

inline vec2 extractVec2(JSON j, JSONError& e) {
	if(e.no) { return vec2(0); }
	std::vector<float> nums = extractArray(j, e, &getNumber);
	return {nums[0], nums[1]};
}

inline Polygon extractPolygon(JSON j, JSONError& e) {
	if(e.no) { return Polygon();}
	JSONObject object = getObject(j, e);
	if(e.no) { return Polygon();}
	float rotation = getNumber(getValue(object, "rotation", e), e);
	if(e.no) { return Polygon(0, rotation);}
	auto model = extractArray(getValue(object, "model", e), e, &extractVec2);
	if(e.no) { return Polygon(model, rotation); }
	return Polygon(model, rotation);
}

inline Entity extractEntity(JSON j, JSONError& err) {
	Entity e;
	if(err.no) {return e;}
	JSONObject object = getObject(j, err);
	e.shape = extractPolygon(getValue(object, "shape", err), err);
	if(err.no) {return e;}
	e.position = extractVec2(getValue(object, "position", err), err);
	if(err.no) {return e;}
	e.type = (Entity::Type)((int)getNumber(getValue(object, "type", err), err));
	update(e.shape, e.position);
	return e;
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
	auto seaEntities = extractArray(getValue(object, "seas", e), e, &extractEntity);
	if (e.no) {err.what = e.what; return false;}
	for(Entity & ent : seaEntities) {
		createSea(stage, ent.position, ent.shape);
	}
	if(e.no) {err.what = e.what; return false;}
	auto platformEntities = extractArray(getValue(object, "platforms", e), e, &extractEntity);
	if(e.no) {err.what = e.what; return false;}
	for(Entity& ent : platformEntities) {
		createPlatform(stage, ent.position, ent.shape);
	}
	Entity s = extractEntity(getValue(object, "ship", e), e);
	if(e.no) {err.what = e.what; return false;}
	createShip(stage, s.position, s.shape);
	stage.rockSpawn = extractVec2(getValue(object, "rock_spawn", e), e);
	if(e.no) {err.what = e.what; return false;}
	Entity w = extractEntity(getValue(object, "win", e), e);
	if(e.no) {err.what = e.what; return false;}
	JSONObject jWinObject = getObject(getValue(object, "win", e), e);
	if(e.no) {err.what = e.what; return false;}
	float timeToWin = getNumber(getValue(jWinObject, "time", e), e);
	if(e.no) {err.what = e.what; return false;}
	createWin(stage, w.position, w.shape);
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
	return serializeStage(stage);
}

bool dumpStageToFile(std::string filename, Stage& stage, SerializeError& err) {
	FILE* pFile;
	pFile = fopen(filename.c_str(), "w");
	if (pFile == nullptr){ err.what = "Could not open file: " + filename; return false; }
	fputs(serializeStage(stage).c_str(), pFile);
	fclose(pFile);
	return true;
}
