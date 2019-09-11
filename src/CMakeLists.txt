cmake_minimum_required(VERSION 3.0)
project(waves)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_SUPRESS_REGENERATION true)


# SFML
if (WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "${SFML_DIR}/lib/cmake/SFML")
endif (WIN32)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
find_library (SFML REQUIRED sfml-window sfml-system sfml-graphics sfml-audio)
include_directories(${SFML_INCLUDE_DIR})


file(GLOB wavesSource
	"*.hpp"
	"*.inl"
	"*.cpp"
)

add_executable(editor editor.cxx ${wavesSource})
add_executable(game game.cxx ${wavesSource})

target_compile_definitions(editor PRIVATE SFML_STATIC)
target_compile_definitions(game PRIVATE SFML_STATIC)
target_link_libraries(editor sfml-window sfml-system sfml-graphics sfml-audio)
target_link_libraries(game sfml-window sfml-system sfml-graphics sfml-audio)
