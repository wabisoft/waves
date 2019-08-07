cmake_minimum_required(VERSION 3.0)
project(waves)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SFML_STATIC_LIBRARIES TRUE)

# SFML
set(SFML_DIR "${SFML_DIR}/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
find_library (SFML REQUIRED sfml-window sfml-system sfml-graphics sfml-audio)
include_directories(${SFML_INCLUDE_DIR})


file(GLOB wavesSource 
    "*.hpp"
    "*.cpp"
)

add_executable(waves main.cpp ${wavesSource})

target_compile_definitions(waves PRIVATE SFML_STATIC)
target_link_libraries(waves sfml-window sfml-system sfml-graphics sfml-audio)
