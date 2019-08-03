cmake_minimum_required(VERSION 3.0)
project(libwabi)
file(GLOB SOURCES
	"*.hpp"
	"*.cpp"
)

add_library(libwabi STATIC SHARED ${SOURCES})

target_include_directories(libwabi PRIVATE .)
