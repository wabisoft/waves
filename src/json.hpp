#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

/****************
 * Declarations *
 ****************/

typedef std::string::iterator string_it;

struct JSONError{
	enum Type {
		INCOMPLETE, // expected more json
		INCORRECT, // malformed json
		OTHER // catch all
	};
	int no = 0;
	string_it where;
	std::string what = "";
};


struct JSON {
	enum Type {
		NUMBER,
		STRING,
		BOOLEAN,
		ARRAY,
		OBJECT,
		NUL, // NULL is taken :(
		ERROR, // sure let's have an error type
	};

	Type type = NUL;
	string_it start;
	string_it end;
};

typedef std::vector<JSON> JSONArray;
typedef std::vector<std::pair<std::string,JSON>> JSONObject;

// Lexiing tools
inline char peek(string_it&);
inline void consumeWhitespace(string_it&, string_it);
inline void expect(string_it&, char, JSONError&);
inline void allow(string_it&, char);
inline string_it getNext(string_it, string_it, char, JSONError&);
inline string_it getNextBalanced(string_it, string_it, char, char, JSONError &);
inline string_it getMatch(string_it, string_it, std::string, JSONError&);
inline string_it getUntill(string_it, string_it, bool(*)(char));

// Parsing functions
inline JSON getJSON(string_it&, string_it&, JSONError&);
inline float getNumber(string_it&, string_it&, JSONError&);
inline std::string getString(string_it&, string_it&, JSONError&);
inline bool getBoolean(string_it&, string_it&, JSONError&);
inline JSONArray getArray(string_it&, string_it&, JSONError&);
inline JSONObject getObject(string_it&, string_it&, JSONError&);

// Convenience
inline float getNumber(JSON, JSONError&);
inline std::string getString(JSON, JSONError&);
inline bool getBoolean(JSON, JSONError&);
inline JSONArray getArray(JSON, JSONError&);
inline JSONObject getObject(JSON, JSONError&);

// Utility function
inline JSON getValue(JSONObject, std::string);

/***************
 * Definitions *
 ***************/

inline char peek(string_it& it) {
	return *(std::next(it));
}

inline void consumeWhitespace(string_it& it, string_it end) {
	while (it != end) {
		switch(*it) {
			case '\t':
			case ' ':
			case '\n':
			case '\r':
				++it;
				break;
			default:
				return; break;
		}
	}
}

inline void expect(string_it& it, char c, JSONError& error) {
	if(*it != c) {
		error.no = JSONError::INCORRECT;
		error.what = "Expected '";
		error.what += c;
		error.what += "'";
		error.where = it;
		return;
	}
	++it;
}

inline void allow(string_it& it, char c) {
	if(*it == c) {
		++it;
	}
}

inline string_it getNext(string_it start, string_it end, char c, JSONError& e) {
	auto search = std::find(std::next(start), end, c);
	if (search == end) {
		e.no = JSONError::INCOMPLETE;
		e.what = "Expected '";
		e.what += c;
		e.what += "'.";
		e.where = start;
	}
	return std::next(search);
}


inline string_it getNextBalanced(string_it start, string_it end, char open, char close, JSONError &e) {
	assert(*start == open);
	++start;
	int count = 1;
	while(count > 0) {
		if (start == end) {
			e.no = JSONError::INCOMPLETE;
			e.what = "Expected closing '";
			e.what += close;
			e.what += "'";
			e.where = start;
		}
		if(*start == open) { ++count; }
		else if (*start == close) { --count; }
		++start;
	}
	return start;
}

inline string_it getMatch(string_it start, string_it end, std::string cmp, JSONError& e) {
	int cmplen = (int)cmp.length();
	auto fail = [&] () -> string_it {
		e.no = JSONError::INCORRECT;
		e.what = "Expected '" + cmp + "'";
		e.where = start;
		return end;
	};
	if(start + cmplen > end) { return fail(); }
	else if(std::string(start, start+cmplen) != cmp) { return fail();}
	else return start+cmplen;
}

inline string_it getUntill(string_it start, string_it end, bool(*f)(char)){
	string_it it = start;
	while(f(*it) && it != end){
		++it;
	}
	return it;
}

inline JSON getJSON(string_it& start, string_it& end, JSONError& e) {
	consumeWhitespace(start, end);
	switch(*start) {
		case '{': // object
			return {JSON::OBJECT, start, getNextBalanced(start, end, '{', '}', e)};
			break;
		case '[': // array
			return {JSON::ARRAY, start, getNextBalanced(start, end, '[', ']', e)};
			break;
		case '"': // string
			return {JSON::STRING, start, getNext(start, end, '"', e)};
			break;
		case 't':
			return {JSON::BOOLEAN, start, getMatch(start, end, "true", e)};
		case 'f': // boolean
			return {JSON::BOOLEAN, start, getMatch(start, end, "false", e)};
			break;
		case 'n': // null
			return {JSON::NUL, start, getMatch(start, end, "null", e)};
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		case '-':
		case '.': // number;
			return {JSON::NUMBER, start, getUntill(start, end, [](char c) { return isdigit(c) || c == '.' ;})};
			break;
	}
	return {JSON::ERROR, start, end};
}

inline float getNumber(string_it& start, string_it& end, JSONError& e){
	try {
		return stof(std::string(start, end));
	} catch(...) {
		e.no = JSONError::INCORRECT;
		e.where = start;
		e.what = "Could not parse number from: " + std::string(start, end);
		return 0.f;
	}
}

inline std::string getString(string_it& start, string_it& end, JSONError& e) {
	expect(start, '"', e);
	string_it it = start;
	if (e.no) { return ""; }
	while(*it != '"' && it != end) {
		++it;
	}
	std::string ret = std::string(start, it);
	expect(it, '"', e);
	if (e.no) { return ""; }
	return ret;
}

inline bool getBoolean(string_it& start, string_it& end, JSONError& e) {
	if (*start == 't') {
		getMatch(start, end, "true", e);
		return true; // check e to verify
	} else {
		getMatch(start, end, "false", e);
		return false; // check e to verify
	}
}

inline JSONArray getArray(string_it& start, string_it& end, JSONError& e) {
	consumeWhitespace(start, end);
	expect(start, '[', e);
	if(e.no) { return {}; }
	consumeWhitespace(start, end);
	JSONArray arr;
	while(*start != ']' && start != end) {
		consumeWhitespace(start, end);
		arr.push_back(getJSON(start, end,e ));
		start = arr.back().end;
		consumeWhitespace(start, end);
		allow(start, ',');
		consumeWhitespace(start, end);
	}
	expect(start, ']', e);
	return arr;
}

inline JSONObject getObject(string_it& start, string_it& end, JSONError& e) {
	consumeWhitespace(start, end);
	// TODO: keys and shit
	expect(start, '{', e);
	if(e.no) { return {}; }
	consumeWhitespace(start, end);
	JSONObject object;
	while(*start != '}' && start != end) {
		consumeWhitespace(start, end);
		JSON j = getJSON(start, end, e);
		if(e.no) { return object; };
		if(j.type != JSON::STRING) {
			e.no = JSONError::INCORRECT;
			e.what = "Expected object key";
			e.where = j.start;
			return object;
		}
		std::string key = getString(j.start, j.end, e);
		start = j.end;
		consumeWhitespace(start, end);
		expect(start, ':', e);
		if(e.no) { return object; };
		consumeWhitespace(start, end);
		j = getJSON(start, end, e);
		if(e.no) { return object; };
		object.push_back({key, j});
		start = j.end;
		allow(start, ',');
		consumeWhitespace(start, end);
	}
	expect(start, '}', e);
	return object;
}

inline float getNumber(JSON j, JSONError& e) { return getNumber(j.start, j.end, e); }
inline std::string getString(JSON j, JSONError& e) { return getString(j.start, j.end, e);  }
inline bool getBoolean(JSON j, JSONError& e) { return getBoolean(j.start, j.end, e); }
inline JSONArray getArray(JSON j, JSONError& e) { return getArray(j.start, j.end, e); }
inline JSONObject getObject(JSON j, JSONError& e) { return getObject(j.start, j.end, e); }

inline JSON getValue(JSONObject o, std::string key) {
	for(auto && pair : o) {
		if(pair.first == key) { return pair.second;}
	}
	return {JSON::ERROR};
}
