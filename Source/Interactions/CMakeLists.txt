cmake_minimum_required(VERSION 3.20)
# Add the sources for the windowing system

set(HEADERS
	Keyboard_input.h
	Cursor_input.h
	Game_object.h
	collision.h
)

set(SOURCES
	Keyboard_input.cpp
	Cursor_input.cpp
	Game_object.cpp
	collision.cpp
)

target_sources(${PROJ_NAME} PRIVATE ${HEADERS} ${SOURCES})
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})