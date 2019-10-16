cmake_minimum_required(VERSION 3.0)
project(waves)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# SFML
if (WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "${SFML_DIR}/lib/cmake/SFML")
	set(CMAKE_SUPPRESS_REGENERATION true)
	get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
endif (WIN32)
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
find_library (SFML REQUIRED sfml-window sfml-system sfml-graphics sfml-audio)
include_directories(${SFML_INCLUDE_DIR})


# ImGui-SFML
set(ImGui-SFML_DIR "${ImGui-SFML_DIR}/lib/cmake/ImGui-SFML")
find_package(ImGui-SFML REQUIRED)

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})


file(GLOB_RECURSE wavesSource
	"src/*.hpp"
	"src/*.inl"
	"src/*.cpp"
)


add_executable(editor src/editor.cxx ${wavesSource})
add_executable(game src/game.cxx ${wavesSource})

target_compile_definitions(editor PRIVATE SFML_STATIC)
target_compile_definitions(game PRIVATE SFML_STATIC)

target_link_libraries(editor sfml-window sfml-system sfml-graphics sfml-audio ImGui-SFML::ImGui-SFML)
target_link_libraries(game sfml-window sfml-system sfml-graphics sfml-audio)

if(WIN32)
	set_property(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_DIR})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
endif(WIN32)