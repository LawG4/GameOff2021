cmake_minimum_required(VERSION 3.20)

# Mainly to make sure that main doesn't get absolutley massive!

# Add the sources for the windowing system

set(HEADERS
	Gameplay.h
)

set(SOURCES
	Gameplay.cpp
	PlayerPhysics.cpp
)

target_sources(${PROJ_NAME} PRIVATE ${HEADERS} ${SOURCES})
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})