cmake_minimum_required(VERSION 3.20)
# Add the sources for the windowing system

set(HEADERS
	Keyboard_input.h
	Player_object.h
)

set(SOURCES
	Keyboard_input.cpp
	Player_object.cpp
)

target_sources(${PROJ_NAME} PRIVATE ${HEADERS} ${SOURCES})
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})