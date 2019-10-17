#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <cassert>

namespace logging {

enum LogLevel { // just copying python levels
	DEBUG = 0,
	INFO = 1,
	WARNING,
	CRITICAL,
	COUNT
};

const char * const LogLevelNames[]= {
		"DEBUG", "INFO", "WARNING", "CRITICAL"
};

struct LoggerRecord {
	const char * name = nullptr;
	LogLevel level = DEBUG;
	bool SHUT_UP = false;
	int refs = 0;
};

using LoggerRecordList = std::vector<LoggerRecord>;

class Logger {
public:
// instance
	Logger(const char * name);
	~Logger();

	template <typename... Args>
	void log(LogLevel, const char *, Args&&... args);

	template <typename... Args>
	void debug(const char *, Args&&...);

	template <typename... Args>
	void info(const char *, Args&&...);

	template <typename... Args>
	void warning(const char *, Args&&...);

	template <typename... Args>
	void error(const char *, Args&&...);

	template <typename... Args>
	void critical(const char *, Args&&...);

	void setLevel(LogLevel level);

	const char * name;

// static
 	static LogLevel level;
	static LoggerRecordList _loggers;

	static LoggerRecordList::iterator _findLoggerRecord(const char* aName);
	static void setLoggerLevel(const char* aName, LogLevel level);
	static void SHUT_UP(const char* aName);

};

template <typename... Args>
void Logger::log(LogLevel level, const char * format, Args&&... args) {
	auto record = _findLoggerRecord(name);
	assert(record != _loggers.end());
	if(record->SHUT_UP) { return; }
	else if(Logger::level <= level && record->level <= level) {
		std::string format_str = "[%s] <%s> " + std::string(format);
		printf(format_str.c_str(), name, LogLevelNames[level], std::forward<Args>(args)...);
	}
}

template <typename... Args>
void Logger::debug(const char * format, Args&&... args) {
	log(DEBUG, format, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::info(const char * format, Args&&... args) {
	log(INFO, format, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::warning(const char * format, Args&&... args) {
	log(WARNING, format, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::error(const char * format, Args&&... args) {
	log(CRITICAL, format, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::critical(const char * format, Args&&... args) {
	log(CRITICAL, format, std::forward<Args>(args)...);
}

}; // logging
