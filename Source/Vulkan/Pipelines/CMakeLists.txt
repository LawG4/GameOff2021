cmake_minimum_required(VERSION 3.20)
# Add the sources for Vulkan system

set(HEADERS
	Pipelines.h
	Sprites.h
)

set(SOURCES
	Pipelines.cpp
	PipelineTemplates.cpp
	Sprite.cpp
	SpritePipeline.cpp
	SpriteSheet.cpp
	SpriteInstance.cpp
)

target_sources(${PROJ_NAME} PRIVATE ${HEADERS} ${SOURCES})
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})