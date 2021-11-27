cmake_minimum_required(VERSION 3.20)
# Add each of the subdirectories
set(COMPONENTS
	Window
	Vulkan
	Shaders
	Log
	Interactions
	Model
	UI
	Camera
	Timing
	GameplayLoop
	Animation
	AssetLoader
)

foreach(directory ${COMPONENTS})
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${directory})
endforeach()