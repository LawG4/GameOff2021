cmake_minimum_required(VERSION 3.20)
# Add the sources for Vulkan system

set(HEADERS
	Vulkan.h
	VulkanObjects.h
	Memory.h
)

set(SOURCES
	Vulkan.cpp
	Instance.cpp
	Validation.cpp
	PhysicalDevice.cpp
	LogicalDevice.cpp
	Surface.cpp
	Renderpass.cpp
	Shader.cpp
	CommndBuffer.cpp
	Present.cpp
	Buffer.cpp
	Descriptors.cpp
)

target_sources(${PROJ_NAME} PRIVATE ${HEADERS} ${SOURCES})
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Pipelines)