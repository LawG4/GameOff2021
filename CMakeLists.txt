cmake_minimum_required(VERSION 3.20)
# Welcome to the CMake list build process for this Game jam project for Github's Game Off 2021

# Set the project's name into the cache
set(PROJ_NAME "BarryChronicals" CACHE STRING "Name of the project stored in the CMake Cache")
set(PROJ_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "Root directory of the CMake Project")
project(${PROJ_NAME})

# Include the utility CMake Function that lets us copy assets nicely
include(${CMAKE_CURRENT_LIST_DIR}/CMake/AssetCopy.cmake)

# Create a target executable for the game 
# We first include the Main.cpp file so that the linker language can be determined
# Further sources will be added later with target_sources
add_executable(${PROJ_NAME} "Source/Main.cpp")

# Ensure that all of the submodules have been downloaded and added added to the project
include(${CMAKE_CURRENT_LIST_DIR}/CMake/Submodules.cmake)

# Now add in the actual source files
add_subdirectory(Source)

# Ensure that VSCode gets all of it's nice formatting
# Make sure that this is done last
include(${CMAKE_CURRENT_LIST_DIR}/CMake/VisualStudio.cmake)

# Add the user settings Json
AddAsset("${CMAKE_CURRENT_LIST_DIR}/UserSettings.json" "")

# Set c++ to be 17 standard
set_property(TARGET ${PROJ_NAME} PROPERTY CXX_STANDARD 17)

set(WORK_DIR ${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Release>/)
message(STATUS "Runtime dir : ${WORK_DIR}")

# Once we've gotten to the bottom add a custom rule for each of the shaders to be compiled
foreach(ShaderPath ${SHADER_FILES})
	# Get the name of the shader
	get_filename_component(ShaderName ${ShaderPath} NAME)
	get_filename_component(ShaderDir ${ShaderPath} DIRECTORY)

	set(SPV_OUT ${ShaderName}.spv)

	# Add the custom command
	add_custom_command(
		OUTPUT ${PROJ_ROOT}/bin/$<CONFIG>/Shaders/${SPV_OUT}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJ_ROOT}/bin/$<CONFIG>/Shaders
		COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${ShaderPath} -o ${PROJ_ROOT}/bin/$<CONFIG>/Shaders/${SPV_OUT}
		DEPENDS ${ShaderPath}
		COMMENT "Compiling shader : ${ShaderName}")

	# Add the Compiled shader to the list of spirv shaders
	set(SPV_SHADERS ${SPV_SHADERS} ${SPV_OUT})
endforeach()

# Once the list of SPIRV shaders has been completed add a target to track them
add_custom_target(SPIRV_TARGET ALL DEPENDS ${SPV_SHADERS})

# Make the main project depend on the spirv target
add_dependencies(${PROJ_NAME} SPIRV_TARGET)

# Add in the assets
add_subdirectory(Assets)

# ===============================================================================================
# Add in the requirements for copying over the licenses into the release folder 
set(LICENSE_LIST
External/irrKlang/irrKlang_License.txt
External/stb/stb_License.txt
External/glfw/License.md
External/glm/copying.txt
External/json/LICENSE.MIT
External/spdlog/LICENSE
)

# for each license create a custom command for copying it over
foreach(License ${LICENSE_LIST})

	# Get the name of the directory containing the license and use it as the name of the directory
	get_filename_component(LicenseOUT ${License} DIRECTORY)
	get_filename_component(LicenseOUT ${LicenseOUT} NAME)
	set(LicenseOUT "${LicenseOUT}_License.txt")

	# Track the output file name so we can track it's dependencies
	set(LICENSES_LIST_OUT ${LICENSES_LIST_OUT} ${LicenseOUT})

	# Add the command that copies this license to the release directory
	add_custom_command(
		OUTPUT ${PROJ_ROOT}/bin/$<CONFIG>/Licenses/${LicenseOUT}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJ_ROOT}/bin/$<CONFIG>/Licenses
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_LIST_DIR}/${License}"
			"${PROJ_ROOT}/bin/$<CONFIG>/Licenses/${LicenseOUT}"
		DEPENDS ${License}
		COMMENT "Copying License : ${LicenseOUT}"
	)
endforeach()

# Create the target for copying all textures
add_custom_target(License_Target ALL DEPENDS ${LICENSES_LIST_OUT})
add_dependencies(${PROJ_NAME} License_Target)
