cmake_minimum_required(VERSION 3.20)
# Welcome to the CMake list build process for this Game jam project for Github's Game Off 2021

# Set the project's name into the cache
set(PROJ_NAME "BUG" CACHE STRING "Name of the project stored in the CMake Cache")
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
