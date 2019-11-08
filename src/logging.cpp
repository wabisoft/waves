
#include "algorithms.hpp"
#include "logging.hpp"
#include "util.hpp"


namespace logging{

/**********
 * STATIC *
 **********/

LogLevel Logger::level = DEBUG;
LoggerRecordList Logger::_loggers = {};

LoggerRecordList::iterator Logger::_findLoggerRecord(const char * aName) {
	return std::lower_bound(_loggers.begin(), _loggers.end(), aName, [] (const LoggerRecord a, const char * b) {
		return std::string(a.name) < std::string(b);
	});
}

void Logger::SHUT_UP(const char * aName) {
	auto record = _findLoggerRecord(aName);
	assert(record != _loggers.end());
	record->SHUT_UP = true;
}



void Logger::setLoggerLevel(const char* aName, LogLevel level) {
	auto record = _findLoggerRecord(aName);
	assert(record != _loggers.end());
	record->level = level;
}

/************
 * Instance *
 ************/

Logger::Logger(const char * name) : name(name) {
	if (_loggers.empty()) {
		_loggers.push_back({name});
		_loggers[0].refs++;
		return;
	} else {
		auto record = _findLoggerRecord(name);
		if (record == _loggers.end()) {
			sorted_insert(_loggers, {name}, [] (const LoggerRecord a, const LoggerRecord b) {
				return std::string(a.name) < std::string(b.name);
			});
		} else {
			record->refs++;
		}
	}
}

Logger::~Logger() {
	auto record = _findLoggerRecord(name);
	assert(record != _loggers.end());
	record->refs--;
	if(record->refs<=0) {
		_loggers.erase(record);
	}
}

void Logger::setLevel(LogLevel level) {
	auto record = _findLoggerRecord(name);
	assert(record != _loggers.end());
	record->level = level;
}
};
