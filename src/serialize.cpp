#include <iostream>
#include "serialize.hpp"

using namespace std;

string serialize(const Stage& stage) {
	stringstream stream;
	stream << "{" <<
		"\"sea_level\"" << ":" << stage.sea.level << "," <<
		"\"platforms\"" << ":" << serialize(stage.platforms, stage.numPlatforms) << "," <<
		"\"ship\"" 		<< ":" << stage.ship.shape << "," <<
		"\"rock_spawn\""<< ":" << stage.rockSpawn << "," <<
		"\"win\""		<< ":" << stage.win <<
	"}";
	return stream.str();
}

std::string serialize(const Platform * platforms, const int numPlatforms) {
	stringstream stream;
	stream << "[";
	for (int i = 0 ; i < numPlatforms; ++i) {
		stream << platforms[i].shape;
		if ( i < numPlatforms - 1) { stream << ","; }
	}
	stream << "]";
	return stream.str();
}

ostream& operator<<(ostream& ss, const Rectangle& rectangle) {
	return ss << "{" <<
		"\"width\"" << ":" << rectangle.width << "," <<
		"\"height\"" << ":" << rectangle.height << "," <<
		"\"position\"" << ":" << rectangle.position << "," <<
		"\"rotation\"" << ":" << rectangle.rotation <<
	"}";

}

ostream& operator<<(ostream& ss, const Vector2 v) {
	return ss << "[" << v.x << "," << v.y << "]";
}

ostream& operator<<(ostream& ss, const Win w) {
	return ss << "{" <<
		"\"time\"" << ":" << w.timeToWin << "," <<
		"\"region\"" << ":" << w.region <<
	"}";
}

typedef std::string::iterator string_it;

inline char peek(string_it& it) {
	return *(std::next(it));
}

inline void consumeWhitespace(string_it& it, string_it end) {
	while (it != end) {
		switch(*it) {
			case '\t':
			case ' ':
			case '\n':
				std::advance(it, 1);
				break;
			default:
				return; break;
		}
	}
}

inline void expect(string_it& it, char c, Error& error) {
	if(*it != c) {
		error.no = Error::INCORRECT;
		error.what = "Expected '";
		error.what += c;
		error.what += "'";
		return;
	}
	std::advance(it, 1);
}

inline void allow(string_it& it, char c) {
	if(*it == c) {
		std::advance(it, 1);
	}
}

inline string parseString(string_it& it, string_it end, Error& error) {
	expect(it, '"', error);
	if (error.no) { return ""; }
	string_it start = it;
	while(*it != '"' && it != end) {
		++it;
	}
	if (it==end) {
		error.no = Error::INCOMPLETE;
		error.what = "Input ended unexpectedly.";
		error.where = end - it;
		return "";

	}
	string ret = string(start, it);
	expect(it, '"', error);
	if (error.no) { return ""; }
	return ret;
}

float parseNumber(string_it& it, string_it end, Error& error) {
	string_it start = it;
	bool period_used = false;
	while((isdigit(*it) || (*it == '.' && !period_used)) && it != end){
		if(*it == '.') { period_used = true;}
		++it;
	}
	if (it==end) {
		error.no = Error::INCOMPLETE;
		error.what = "Input ended unexpectedly.";
		error.where = end - it;
		return 0.f;
	}
	return stof(string(start, it));
}

template <typename T>
inline std::vector<T> parseList(string_it& it, string_it end, Error& error, T(*subparse)(string_it&, string_it end, Error&)) {
	vector<T> ret;
	expect(it, '[', error);
	if (error.no) { return {}; }
	while(*it != ']' && it != end) {
		ret.push_back(subparse(it, end, error));
		consumeWhitespace(it, end);
		allow(it, ',');
		consumeWhitespace(it, end);
	}
	expect(it, ']', error);
	if (error.no) { return {}; }
	return ret;
}

inline Vector2 parseVector2(string_it& it, string_it end, Error& error) {
	std::vector<float> vector = parseList(it, end, error, &parseNumber);
	if (error.no) { return VECTOR2_ZERO; }
	return {vector[0], vector[1]};
}

inline Rectangle parseRectangle(string_it& it, string_it end, Error& error) {
	Vector2 position;
	float width = 0.f, height = 0.f, rotation = 0.f;
	consumeWhitespace(it, end);
	expect(it, '{', error);
	if (error.no) { return {}; }
	consumeWhitespace(it, end);
	while(it != end && *it != '}') {
		string key = parseString(it, end, error);
		if (error.no) { return {}; }
		consumeWhitespace(it, end);
		expect(it, ':', error);
		if (error.no) { return {}; }
		consumeWhitespace(it, end);

		if (key == "position") {
			position = parseVector2(it, end, error);
			if (error.no) { return {}; }
		} else if (key == "width") {
			width = parseNumber(it, end, error);
			if (error.no) { return {}; }
		} else if (key == "height") {
			height = parseNumber(it, end, error);
			if (error.no) { return {}; }
		}  else if (key == "rotation") {
			rotation = parseNumber(it, end, error);
			if (error.no) { return {}; }
		} else {
			error.no = Error::INCORRECT;
			error.what = "Expected key to be one of [\"position\", \"width\", \"height\", \"rotation\"] found \"" + key + "\" instead.";
			error.where = end-it;
			return {};
		}
		consumeWhitespace(it, end);
		allow(it, ',');
	}
	expect(it, '}', error);
	if (error.no) { return {}; }
	return makeRectangle(position, width, height, rotation);
}

Stage deserializeStage(std::string data, Error& error) {
	Stage stage{};
	auto fail = [stage, error]() -> Stage {
		std::cerr << "Error: [" << error.no << "] " << error.what << "( @ " << error.where << " )\n";
		return stage;
	};
	string_it end = data.end();
	string_it it = data.begin();
	consumeWhitespace(it, end);
	expect(it, '{', error);
	if (error.no) { return fail(); }
	while(it != end && *it != '}') {
		consumeWhitespace(it, end);
		string key = parseString(it, end, error);
		if (error.no) { return fail(); }
		consumeWhitespace(it, end);
		expect(it, ':', error);
		if (error.no) { return fail(); }
		if (key == "sea_level") {
			float number = parseNumber(it, end, error);
			if (error.no) { return fail(); }
			stage.sea.level = number;
		} else if (key == "platforms") {
			vector<Rectangle> platformRects = parseList(it, end, error, &parseRectangle);
			if (error.no) { return fail(); }
			for(Rectangle& rect : platformRects) {
				createPlatform(stage, rect.position, rect.width, rect.height);
			}
		} else if (key == "ship") {
			Rectangle rect = parseRectangle(it, end, error);
			if (error.no) { return fail(); }
			createShip(stage, rect.position, rect.width, rect.height);
		} else if (key == "rock_spawn") {
			Vector2 rock_spawn = parseVector2(it, end, error);
			// std::vector<float> vector = parseList(it, end, error, &parseNumber);
			if (error.no) { return fail(); }
			stage.rockSpawn = rock_spawn;
		} else if (key == "win") {
			consumeWhitespace(it, end);
			expect(it, '{', error);
			if (error.no) { return fail(); }
			while(it != end && peek(it) != '}') {
				consumeWhitespace(it, end);
				key = parseString(it, end, error);
				consumeWhitespace(it, end);
				expect(it, ':', error);
				if (error.no) { return fail(); }
				if (key == "time") {
					float num = parseNumber(it, end, error);
					if (error.no) { return fail(); }
					stage.win.timeToWin = num;
				}
				else if (key == "region") {
					Rectangle rect = parseRectangle(it, end, error);
					if (error.no) { return fail(); }
					stage.win.region = rect;
				}
				consumeWhitespace(it, end);
				allow(it, ',');
			}
			consumeWhitespace(it, end);
			expect(it, '}', error);
			if (error.no) { return fail(); }
		}
		consumeWhitespace(it, end);
		allow(it, ',');
	}
	consumeWhitespace(it, end);
	expect(it, '}', error);
	if (error.no) { return fail(); }
	return stage;
}
