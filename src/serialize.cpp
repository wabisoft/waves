#include <iostream>
#include "serialize.hpp"
#include "json.hpp"

using namespace std;

string serialize(const Stage& stage) {
	stringstream stream;
	stream << "{" <<
		"\"sea_level\"" << ":" << stage.sea.level << "," <<
		"\"platforms\"" << ":" << serialize(stage.platforms, stage.numPlatforms) << "," <<
		"\"ship\"" 		<< ":" << serialize(stage.ship.shape) << "," <<
		"\"rock_spawn\""<< ":" << serialize(stage.rockSpawn) << "," <<
		"\"win\""		<< ":" << serialize(stage.win) <<
	"}";
	return stream.str();
}

std::string serialize(const Platform * platforms, const int numPlatforms) {
	stringstream stream;
	stream << "[";
	for (int i = 0 ; i < numPlatforms; ++i) {
		stream << serialize(platforms[i].shape);
		if ( i < numPlatforms - 1) { stream << ","; }
	}
	stream << "]";
	return stream.str();
}

std::string serialize(const Rectangle& rectangle) {
	stringstream stream;
	stream << "{" <<
		"\"width\"" << ":" << rectangle.width << "," <<
		"\"height\"" << ":" << rectangle.height << "," <<
		"\"position\"" << ":" << serialize(rectangle.position) << "," <<
		"\"rotation\"" << ":" << rectangle.rotation <<
	"}";
	return stream.str();
}

std::string serialize(const Vector2 v) {
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

inline Vector2 extractVector2(JSON j, JSONError& e) {
	std::vector<float> nums = extractArray(j, e, &getNumber);
	return {nums[0], nums[1]};
}

inline Rectangle extractRectangle(JSON j, JSONError& e) {
	Rectangle r;
	JSONObject object = getObject(j, e);
	if(e.no) { return r;}
	r.width = getNumber(getValue(object, "width"), e);
	if(e.no) { return r;}
	r.height = getNumber(getValue(object, "height"), e);
	if(e.no) { return r;}
	r.position = extractVector2(getValue(object, "position"), e);
	if(e.no) { return r;}
	r.rotation = getNumber(getValue(object, "rotation"), e);
	if(e.no) { return r;}
	return makeRectangle(r.position, r.width, r.height, r.rotation);
}


Stage deserializeStage(std::string data, SerializeError& err) {
	Stage stage;
	JSONError e;
	JSON json = getJSON(data.begin(), data.end(), e);
	if(e.no) {err.what = e.what; return stage;}
	if(json.type != JSON::OBJECT) {
		err.what = "Stage data should be a valid json object";
		return stage;
	}
	JSONObject object = getObject(json, e);
	if(e.no) {err.what = e.what; return stage;}
	stage.sea.level = getNumber(getValue(object, "sea_level"), e);
	if(e.no) {err.what = e.what; return stage;}
	auto platformRectangles = extractArray(getValue(object, "platforms"), e, &extractRectangle);
	if(e.no) {err.what = e.what; return stage;}
	for(Rectangle &r : platformRectangles) {
		createPlatform(stage, r);
	}
	Rectangle r = extractRectangle(getValue(object, "ship"), e);
	if(e.no) {err.what = e.what; return stage;}
	createShip(stage, r);
	stage.rockSpawn = extractVector2(getValue(object, "rock_spawn"), e);
	JSONObject jWinObject = getObject(getValue(object, "win"), e);
	if(e.no) {err.what = e.what; return stage;}
	stage.win.timeToWin = getNumber(getValue(jWinObject, "time"), e);
	stage.win.region = extractRectangle(getValue(jWinObject, "region"), e);
	return stage;
}

