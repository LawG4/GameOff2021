cmake_minimum_required(VERSION 3.20)
# Add each of the subdirectories
set(COMPONENTS
	Window
	Vulkan
	Log
	GL
	Render
	ImGui
)

foreach(directory ${COMPONENTS})
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${directory})
endforeach()